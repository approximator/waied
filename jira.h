#ifndef JIRA_H
#define JIRA_H

#include <list>

#include <QObject>
#include <QNetworkAccessManager>


struct JiraTask {
    QString title;
    QString key;
    QString id;
};

class Jira : public QObject
{
    Q_OBJECT
public:
    explicit Jira(const QString& jiraApiUrl = "", QObject *parent = nullptr);
    void searchTasks(const QString &query);

    const std::list<JiraTask> &tasks() const;

private:
    QNetworkAccessManager *netManager { new QNetworkAccessManager(this) };
    QString mJiraApiUrl;
    std::list<JiraTask> mTasks;

signals:
    void taskSearchComplete();

public slots:

private slots:
    void onTasksSearchFinished();
};

#endif // JIRA_H
