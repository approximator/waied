
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QAuthenticator>
#include <QSettings>

#include <fmt/format.h>

#include "jira.h"

// https://docs.atlassian.com/software/jira/docs/api/REST/7.10.2/
Jira::Jira(const QString &jiraApiUrl, QObject *parent)
    : QObject(parent)
    , mJiraApiUrl(jiraApiUrl)
{

}

void Jira::searchTasks(const QString &query)
{
    qDebug() << __FUNCTION__ ;

    QSettings settings;
    qDebug() << "Reading " << settings.fileName();

    const auto jiraApiUrl = settings.value("jira/apiUrl", "https://someserver.com/jira/rest/api/2").toString();
    const auto userName = settings.value("jira/username", "username").toString();
    const auto pass = settings.value("jira/pass", "pass").toString();
    const auto fields = "id,key,summary";

    QNetworkRequest request;
    const auto url = fmt::format("{jiraUrl}/search?jql={jqlRequest}&fields={fields}&maxResults=20",
                                 fmt::arg("jiraUrl", jiraApiUrl.toStdString()),
                                 fmt::arg("jqlRequest", query.toStdString()),
                                 fmt::arg("fields", fields));
    request.setUrl(QUrl(QString(url.c_str())));
    const auto auth = QString("%1:%2").arg(userName).arg(pass);
    request.setRawHeader("Authorization", "Basic " + auth.toLocal8Bit().toBase64());

    QNetworkReply* reply = netManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &Jira::onTasksSearchFinished);
}

const std::list<JiraTask> &Jira::tasks() const
{
    return mTasks;
}

void Jira::onTasksSearchFinished()
{
    qDebug() << "Tasks search complete";
    QNetworkReply *reply = dynamic_cast<QNetworkReply *>(QObject::sender());

    if (reply->error()) {
        qDebug() << "Request error: " << reply->errorString();
        return;
    }
    const auto requestPath = reply->request().url().path();
    qDebug() << "Got reply for " << requestPath;

    auto answer = reply->readAll();
    QJsonParseError parseError;
    auto doc = QJsonDocument::fromJson(answer, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qDebug() << "Parse error: " << parseError.errorString();
        return;
    }

    auto issues = doc.object().value("issues");
    if (issues.isUndefined()) {
        qDebug() << "Cannot parse issues from response";
        return;
    }

    // auto str = QString::fromUtf8(document.toJson(QJsonDocument::JsonFormat::Indented));
    // std::cout << str.toStdString() << std::endl;

    for (auto value : issues.toArray()) {
        auto issue = value.toObject();
        const auto task = JiraTask{
                issue.value("fields").toObject().value("summary").toString(),
                issue.value("key").toString(),
                issue.value("id").toString()};
        mTasks.emplace_back(task);
    }

    reply->close();
    reply->disconnect();
    reply->deleteLater();
    emit taskSearchComplete();
}
