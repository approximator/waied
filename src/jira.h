#ifndef JIRA_H
#define JIRA_H

#include <list>
#include <chrono>
#include <memory>

#include <QHash>
#include <QNetworkAccessManager>

#include "QQmlAutoPropertyHelpers.h"

#include "task.h"
#include "worklog.h"

class Jira : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString, apiUrl)
    QML_WRITABLE_AUTO_PROPERTY(QString, username)
    QML_WRITABLE_AUTO_PROPERTY(QString, password)

public:
    using TaskList = QHash<QString, std::shared_ptr<Task>>;

    Jira(const Jira &) = delete;
    Jira &operator=(const Jira &) = delete;

    explicit Jira(QObject *parent = nullptr);
    void searchTasks(const QString &query);
    bool updateWorklog(Task &task);
    bool logWork(const std::shared_ptr<Task> &task, const std::chrono::seconds timeSpent,
                 const std::chrono::system_clock::time_point started = std::chrono::system_clock::now());

    const TaskList &tasks() const;

private:
    QNetworkRequest makeRequest(const QString &url);

    QNetworkAccessManager *netManager{ new QNetworkAccessManager(this) };
    QString mJiraApiUrl;
    TaskList mTasks;

signals:
    void taskSearchComplete();
    void worklogsUpdated(const Task *);

public slots:

private slots:
    void onTasksSearchFinished();
    void onWorklogs(Task &task, QNetworkReply *reply);
};

#endif  // JIRA_H
