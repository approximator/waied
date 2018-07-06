
#include <algorithm>

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QAuthenticator>
#include <QSettings>

#include <fmt/format.h>

#include "jira.h"

struct ReplyParseResult
{
    bool ok;
    QJsonDocument doc;
};
QNetworkRequest makeRequest(const QString &url);
ReplyParseResult parseReply(QNetworkReply *reply);

// https://docs.atlassian.com/software/jira/docs/api/REST/7.10.2/
Jira::Jira(const QString &jiraApiUrl, QObject *parent)
    : QObject(parent)
    , mJiraApiUrl(jiraApiUrl)
{
}

void Jira::searchTasks(const QString &query)
{
    qDebug() << __FUNCTION__;

    QSettings settings;
    qDebug() << "Reading " << settings.fileName();

    // https://docs.atlassian.com/software/jira/docs/api/REST/7.10.2/#api/2/search
    const auto jiraApiUrl = settings.value("jira/apiUrl", "https://someserver.com/jira/rest/api/2").toString();
    const auto fields = "id,key,summary,aggregatetimespent";
    const auto url = fmt::format("{jiraUrl}/search?jql={jqlRequest}&fields={fields}&maxResults=20",
                                 fmt::arg("jiraUrl", jiraApiUrl.toStdString()),
                                 fmt::arg("jqlRequest", query.toStdString()), fmt::arg("fields", fields));
    auto request = makeRequest(QString(url.c_str()));
    QNetworkReply *reply = netManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Jira::onTasksSearchFinished);
}

const std::list<JiraTask> &Jira::tasks() const
{
    return mTasks;
}

void Jira::onWorklogs(JiraTask &task, QNetworkReply *reply)
{
    //    qDebug() << "Got worklogs for " << task.id;

    auto parsedReply = parseReply(reply);
    if (!parsedReply.ok) {
        return;
    }

    auto worklogs = parsedReply.doc.object().value("worklogs");
    if (worklogs.isUndefined()) {
        qDebug() << "Cannot parse worklogs from response";
        return;
    }

    for (auto value : worklogs.toArray()) {
        auto worklog = value.toObject();
        task.worklog.emplace_back(worklog.value("author").toObject().value("key").toString(),
                                  worklog.value("comment").toString(),
                                  QDateTime::fromString(worklog.value("created").toString(), Qt::ISODate),
                                  QDateTime::fromString(worklog.value("started").toString(), Qt::ISODate),
                                  QDateTime::fromString(worklog.value("updated").toString(), Qt::ISODate),
                                  worklog.value("id").toString(), worklog.value("issueId").toString(),
                                  worklog.value("self").toString(), worklog.value("timeSpentSeconds").toInt());
    }

    reply->close();
    reply->disconnect();

    auto isFinished = [](auto r) { return r->isFinished(); };
    if (std::all_of(begin(expectedRelies), end(expectedRelies), isFinished)) {
        for (auto reply : expectedRelies) {
            reply->deleteLater();
        }
        expectedRelies.clear();
        qDebug() << "All request were finished";
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

    auto issues = parsedReply.doc.object().value("issues");
    if (issues.isUndefined()) {
        qDebug() << "Cannot parse issues from response";
        return;
    }

    for (auto value : issues.toArray()) {
        auto issue = value.toObject();
        mTasks.emplace_back(issue.value("fields").toObject().value("summary").toString(), issue.value("key").toString(),
                            issue.value("id").toString(), issue.value("self").toString(),
                            issue.value("fields").toObject().value("aggregatetimespent").toInt());
        auto &task = mTasks.back();

        // https://docs.atlassian.com/software/jira/docs/api/REST/7.10.2/#api/2/issue-getIssueWorklog
        auto req = makeRequest(task.url + "/worklog");
        QNetworkReply *rep = netManager->get(req);
        expectedRelies.push_back(rep);
        connect(rep, &QNetworkReply::finished, this,
                [&task, this]() { onWorklogs(task, dynamic_cast<QNetworkReply *>(QObject::sender())); });
    }

    reply->close();
    reply->disconnect();
    reply->deleteLater();
}

QNetworkRequest makeRequest(const QString &url)
{
    QSettings settings;
    const auto userName = settings.value("jira/username", "username").toString();
    const auto pass = settings.value("jira/pass", "pass").toString();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    const auto auth = QString("%1:%2").arg(userName).arg(pass);
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
    res.doc = QJsonDocument::fromJson(reply->readAll(), &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parse error: " << parseError.errorString();
        return res;
    }

    // fmt::print("{}", QString::fromUtf8(res.doc.toJson(QJsonDocument::JsonFormat::Indented)).toStdString());

    res.ok = true;
    return res;
}
