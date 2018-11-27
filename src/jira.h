#ifndef JIRA_H
#define JIRA_H

#include <list>
#include <chrono>
#include <memory>

#include <QObject>
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
    using TaskList = std::list<std::shared_ptr<Task>>;

    explicit Jira(QObject *parent = nullptr);
    void searchTasks(const QString &query);

    const TaskList &tasks() const;

private:
    QNetworkRequest makeRequest(const QString &url);

    QNetworkAccessManager *netManager{ new QNetworkAccessManager(this) };
    QString mJiraApiUrl;
    TaskList mTasks;
    std::vector<QNetworkReply *> expectedReplies;

signals:
    void taskSearchComplete();

public slots:

private slots:
    void onTasksSearchFinished();
    void onWorklogs(Task &task, QNetworkReply *reply);
};

#endif  // JIRA_H
