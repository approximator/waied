#ifndef JIRA_H
#define JIRA_H

#include <list>
#include <chrono>

#include <QObject>
#include <QNetworkAccessManager>

struct JiraWorkLog
{
    QString author;
    QString comment;
    std::chrono::system_clock::time_point created;
    std::chrono::system_clock::time_point started;
    std::chrono::system_clock::time_point updated;
    QString id;
    QString issueId;
    QString url;
    std::chrono::seconds timeSpentSec;

    JiraWorkLog(QString p_author, QString p_comment, std::chrono::system_clock::time_point p_created,
                std::chrono::system_clock::time_point p_started, std::chrono::system_clock::time_point p_updated,
                QString p_id, QString p_issueId, QString p_url, std::chrono::seconds p_timeSpentSec)
        : author(std::move(p_author))
        , comment(std::move(p_comment))
        , created(std::move(p_created))
        , started(std::move(p_started))
        , updated(std::move(p_updated))
        , id(std::move(p_id))
        , issueId(std::move(p_issueId))
        , url(std::move(p_url))
        , timeSpentSec(std::move(p_timeSpentSec))
    {
    }
};

struct JiraTask
{
    QString title;
    QString key;
    QString id;
    QString url;
    std::chrono::seconds timeSpent;
    std::vector<JiraWorkLog> worklog;

    JiraTask(QString p_title, QString p_key, QString p_id, QString p_url, std::chrono::seconds p_timeSpent)
        : title(std::move(p_title))
        , key(std::move(p_key))
        , id(std::move(p_id))
        , url(std::move(p_url))
        , timeSpent(std::move(p_timeSpent))
    {
    }
};

class Jira : public QObject
{
    Q_OBJECT
public:
    explicit Jira(const QString &jiraApiUrl = "", QObject *parent = nullptr);
    void searchTasks(const QString &query);

    const std::list<JiraTask> &tasks() const;

private:
    QNetworkAccessManager *netManager{ new QNetworkAccessManager(this) };
    QString mJiraApiUrl;
    std::list<JiraTask> mTasks;
    std::vector<QNetworkReply *> expectedReplies;

signals:
    void taskSearchComplete();

public slots:

private slots:
    void onTasksSearchFinished();
    void onWorklogs(JiraTask &task, QNetworkReply *reply);
};

#endif  // JIRA_H
