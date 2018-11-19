
#include <algorithm>

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "fmt/format.h"
#include "date/tz.h"

#include "jira.h"

struct ReplyParseResult
{
    bool ok;
    QJsonDocument doc;
};
QNetworkRequest makeRequest(const QString &url);
ReplyParseResult parseReply(QNetworkReply *reply);

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
    const auto fields = "id,key,summary,aggregatetimespent";
    const auto url = fmt::format("{jiraUrl}/search?jql={jqlRequest}&fields={fields}&maxResults=20",
                                 fmt::arg("jiraUrl", jiraApiUrl.toStdString()),
                                 fmt::arg("jqlRequest", query.toStdString()), fmt::arg("fields", fields));
    auto request = makeRequest(QString(url.c_str()));
    QNetworkReply *reply = netManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Jira::onTasksSearchFinished);
}

const Jira::TaskList &Jira::tasks() const
{
    return mTasks;
}

void Jira::onWorklogs(Task &task, QNetworkReply *reply)
{
    //    qDebug() << "Got worklogs for " << task.id;

    auto parsedReply = parseReply(reply);
    if (!parsedReply.ok) {
        return;
    }

    // std::ofstream f(std::string("/tmp/worklog_") + task.id.toStdString() + " .json");
    // f << QString::fromUtf8(parsedReply.doc.toJson(QJsonDocument::JsonFormat::Indented)).toStdString();
    // f.close();

    auto worklogs = parsedReply.doc.object().value("worklogs");
    if (worklogs.isUndefined()) {
        qDebug() << "Cannot parse worklogs from response";
        return;
    }

    auto parseDate = [](const QString &str) {
        std::chrono::system_clock::time_point utc_tp;
        std::istringstream in{ str.toStdString() };
        in >> date::parse("%FT%H:%M:%S%z", utc_tp);
        if (!in.good()) {
            qDebug() << "Cannot parse date: " << str;
        }
        return utc_tp;
    };

    for (auto value : worklogs.toArray()) {
        auto worklog = value.toObject();
        task.appendWorkLogItem(
            new WorkLog(worklog.value("author").toObject().value("key").toString(), worklog.value("comment").toString(),
                        parseDate(worklog.value("created").toString()), parseDate(worklog.value("started").toString()),
                        parseDate(worklog.value("updated").toString()), worklog.value("id").toString(),
                        worklog.value("issueId").toString(), worklog.value("self").toString(),
                        std::chrono::seconds{ worklog.value("timeSpentSeconds").toInt() }, &task));
    }

    reply->close();
    reply->disconnect();

    auto isFinished = [](auto r) { return r->isFinished(); };
    if (std::all_of(begin(expectedReplies), end(expectedReplies), isFinished)) {
        for (auto reply : expectedReplies) {
            reply->deleteLater();
        }
        expectedReplies.clear();
        qDebug() << "All requests were finished";
        emit taskSearchComplete();
    }
}

void Jira::onTasksSearchFinished()
{
    qDebug() << "Tasks search complete";
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(QObject::sender());

    auto parsedReply = parseReply(reply);
    if (!parsedReply.ok) {
        return;
    }

    // std::ofstream f("/tmp/task_list.json");
    // f << QString::fromUtf8(parsedReply.doc.toJson(QJsonDocument::JsonFormat::Indented)).toStdString();
    // f.close();

    auto issues = parsedReply.doc.object().value("issues");
    if (issues.isUndefined()) {
        qDebug() << "Cannot parse issues from response";
        return;
    }

    for (auto value : issues.toArray()) {
        auto issue = value.toObject();
        mTasks.emplace_back(std::make_shared<Task>(
            issue.value("fields").toObject().value("summary").toString(), issue.value("key").toString(),
            issue.value("id").toString(), issue.value("self").toString(),
            std::chrono::seconds{ issue.value("fields").toObject().value("aggregatetimespent").toInt() }, this));
        auto &task = mTasks.back();

        // https://docs.atlassian.com/software/jira/docs/api/REST/7.10.2/#api/2/issue-getIssueWorklog
        auto req = makeRequest(task->url() + "/worklog");
        QNetworkReply *rep = netManager->get(req);
        expectedReplies.push_back(rep);
        connect(rep, &QNetworkReply::finished, this,
                [&task, this]() { onWorklogs(*task, dynamic_cast<QNetworkReply *>(QObject::sender())); });
    }

    reply->close();
    reply->disconnect();
    reply->deleteLater();
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
        qDebug() << "Request error: " << reply->errorString();
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
