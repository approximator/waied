
#include <algorithm>

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <fmt/format.h>

#include "jira.h"

struct ReplyParseResult
{
    bool ok;
    QJsonDocument doc;
};

ReplyParseResult parseReply(QNetworkReply *reply);

static auto parseJiraDate(const QString &str)
{
    const auto formatStr = "%FT%H:%M:%S%z";
    // https://howardhinnant.github.io/date/date.html#from_stream_formatting
    // %F	Equivalent to %Y-%m-%d. If modified with a width, the width is applied to only %Y.
    // %T	Equivalent to %H:%M:%S.
    // %M	The minutes as a decimal number.
    // %S	The seconds as a decimal number
    // %z	The offset from UTC in the format [+|-]hh[mm].

    std::chrono::system_clock::time_point utc_tp;
    std::istringstream in{ str.toStdString() };
    in >> date::parse(formatStr, utc_tp);
    if (!in.good()) {
        qDebug() << "Cannot parse date: " << str;
    }
    return utc_tp;
}

// https://docs.atlassian.com/software/jira/docs/api/REST/7.10.2/
Jira::Jira(QObject *parent)
    : QObject(parent)
    , m_apiUrl("")
    , m_username("")
    , m_password("")
{
}

void Jira::searchTasks(const QString &query)
{
    qDebug() << __FUNCTION__;
    mTasks.clear();

    // https://docs.atlassian.com/software/jira/docs/api/REST/7.10.2/#api/2/search
    const auto jiraApiUrl = m_apiUrl;
    const auto fields = "id,key,summary,aggregatetimespent,priority,status,updated";
    const auto url = fmt::format("{jiraUrl}/search?jql={jqlRequest}&fields={fields}&maxResults=200",
                                 fmt::arg("jiraUrl", jiraApiUrl.toStdString()),
                                 fmt::arg("jqlRequest", query.toStdString()), fmt::arg("fields", fields));
    const auto request = makeRequest(QString(url.c_str()));
    const auto reply = netManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Jira::onTasksSearchFinished);
}

const Jira::TaskList &Jira::tasks() const
{
    return mTasks;
}

void Jira::onTasksSearchFinished()
{
    qDebug() << "Tasks search complete";
    const auto reply = dynamic_cast<QNetworkReply *>(QObject::sender());
    const auto parsedReply = parseReply(reply);
    if (!parsedReply.ok) {
        return;
    }

    // std::ofstream f("/tmp/task_list.json");
    // f << QString::fromUtf8(parsedReply.doc.toJson(QJsonDocument::JsonFormat::Indented)).toStdString();
    // f.close();

    const auto issues = parsedReply.doc.object().value("issues");
    if (issues.isUndefined()) {
        qDebug() << "Cannot parse issues from response";
        return;
    }

    for (const auto &value : issues.toArray()) {
        const auto issue = value.toObject();
        const auto fields = issue.value("fields").toObject();
        const auto key = issue.value("key").toString();
        mTasks[key] = std::make_shared<Task>(
            fields.value("summary").toString(), key, issue.value("id").toString(), issue.value("self").toString(),
            parseJiraDate(fields.value("updated").toString()),
            std::chrono::seconds{ fields.value("aggregatetimespent").toInt() },
            fields.value("priority").toObject().value("name").toString(),
            fields.value("status").toObject().value("name").toString(), std::chrono::system_clock::time_point{}, this);
    }

    reply->close();
    reply->disconnect();
    reply->deleteLater();
    emit taskSearchComplete();
}

bool Jira::updateWorklog(Task &task)
{
    // https://docs.atlassian.com/software/jira/docs/api/REST/7.10.2/#api/2/issue-getIssueWorklog
    qDebug() << "Updating worklog of task " << task.key();
    auto req = makeRequest(task.url() + "/worklog");
    const auto rep = netManager->get(req);
    connect(rep, &QNetworkReply::finished, this,
            [&task, this]() { onWorklogs(task, dynamic_cast<QNetworkReply *>(QObject::sender())); });
    return true;
}

static std::string formatTimeForJira(const std::chrono::system_clock::time_point started)
{
    static const auto formatStr = "%FT%H:%M:%S%z";
    std::ostringstream startedStream;
    startedStream << date::format(formatStr, started);
    auto startedStr = startedStream.str();
    const auto it = std::find(std::rbegin(startedStr), std::rend(startedStr), '.');
    startedStr.erase(--(it.base()), std::end(startedStr));
    return startedStr + ".000+0000";
}

bool Jira::logWork(const std::shared_ptr<Task> &task, const std::chrono::seconds timeSpent,
                   const std::chrono::system_clock::time_point started)
{
    qDebug() << "Logging" << timeSpent.count() << "seconds for" << task->key();

    QJsonDocument jsonData(
        { QJsonObject{ { "comment", QJsonValue::fromVariant("") },
                       { "started", QJsonValue::fromVariant(formatTimeForJira(started).c_str()) },
                       { "timeSpentSeconds", QJsonValue::fromVariant(static_cast<qint64>(timeSpent.count())) } } });
    qDebug() << jsonData;
    const auto data = jsonData.toJson(QJsonDocument::Compact);

    auto request = makeRequest(task->url() + "/worklog");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("Content-Length", QString::number(data.size()).toLatin1());

    const auto reply = netManager->post(request, data);
    connect(reply, &QNetworkReply::finished, this, [task, this]() mutable {
        const auto jiraReply = dynamic_cast<QNetworkReply *>(QObject::sender());
        const auto parsedReply = parseReply(jiraReply);
        if (!parsedReply.ok) {
            return;
        }
        if (jiraReply->error() != QNetworkReply::NoError) {
            qWarning() << "Could not log time";
            return;
        }

        // TODO: update task data
        updateWorklog(*task);
    });

    return true;
}

void Jira::onWorklogs(Task &task, QNetworkReply *reply)
{
    //    qDebug() << "Got worklogs for " << task.id;

    const auto parsedReply = parseReply(reply);
    if (!parsedReply.ok) {
        return;
    }

    //     std::ofstream f(std::string("/tmp/worklog_") + task.id.toStdString() + " .json");
    //     f << QString::fromUtf8(parsedReply.doc.toJson(QJsonDocument::JsonFormat::Indented)).toStdString();
    //     f.close();

    const auto worklogs = parsedReply.doc.object().value("worklogs");
    if (worklogs.isUndefined()) {
        qDebug() << "Cannot parse worklogs from response";
        return;
    }

    task.clearWorkLog();
    for (const auto &value : worklogs.toArray()) {
        const auto worklog = value.toObject();
        task.appendWorkLogItem(new WorkLog(
            worklog.value("author").toObject().value("key").toString(), worklog.value("comment").toString(),
            parseJiraDate(worklog.value("created").toString()), parseJiraDate(worklog.value("started").toString()),
            parseJiraDate(worklog.value("updated").toString()), worklog.value("id").toString(),
            worklog.value("issueId").toString(), worklog.value("self").toString(),
            std::chrono::seconds{ worklog.value("timeSpentSeconds").toInt() }, &task));
    }
    task.set_lastWorklogFetch(std::chrono::system_clock::now());

    reply->close();
    reply->disconnect();
    reply->deleteLater();
    emit worklogsUpdated(&task);
}

QNetworkRequest Jira::makeRequest(const QString &url)
{
    const auto userName = m_username;
    const auto pass = m_password;
    const auto auth = QString("%1:%2").arg(userName).arg(pass);
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("Authorization", "Basic " + auth.toLocal8Bit().toBase64());
    return request;
}

ReplyParseResult parseReply(QNetworkReply *reply)
{
    ReplyParseResult res;
    res.ok = false;

    if (reply->error()) {
        qDebug() << "Request error: " << reply->errorString() << reply->readAll();
        return res;
    }

    QJsonParseError parseError;
    const auto replyContent = reply->readAll();
    res.doc = QJsonDocument::fromJson(replyContent, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parse error: " << parseError.errorString() << "\n" << replyContent;
        return res;
    }

    // fmt::print("{}", QString::fromUtf8(res.doc.toJson(QJsonDocument::JsonFormat::Indented)).toStdString());

    res.ok = true;
    return res;
}
