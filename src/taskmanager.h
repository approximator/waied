#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <chrono>

#include <QObject>
#include <QSettings>

#include "QQmlObjectListModel.h"
#include "QQmlAutoPropertyHelpers.h"
#include "date/date.h"

#include "jira.h"
#include "task.h"
#include "cache.h"
#include "sortfilterproxymodel.h"

using TTasksModel = QQmlObjectListModel<Task>;

class TaskManager : public QObject
{
    Q_OBJECT
    QML_READONLY_AUTO_PROPERTY(TTasksModel *, origModel)
    QML_READONLY_AUTO_PROPERTY(TasksSortFilterProxyModel *, model)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::seconds, reportedToday)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::seconds, reportedYesterday)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::seconds, reportedThisWeek)

    Q_PROPERTY(QString reportedTodayStr READ reportedTodayStr NOTIFY reportedTodayStrChanged)
    Q_PROPERTY(QString reportedYesterdayStr READ reportedYesterdayStr NOTIFY reportedYesterdayStrChanged)
    Q_PROPERTY(QString reportedThisWeekStr READ reportedThisWeekStr NOTIFY reportedThisWeekStrChanged)

public:
    explicit TaskManager(QObject *parent = nullptr);
    ~TaskManager();
    TaskManager(const TaskManager &) = delete;
    TaskManager &operator=(const TaskManager &) = delete;

    Q_INVOKABLE void updateTasks();
    Q_INVOKABLE void updateSettings(const QString &jiraUrl, const QString &username, const QString &pass);
    Q_INVOKABLE bool logTime(const QString &taskKey, const QString &timeStr);

    QString reportedTodayStr() const
    {
        return QString(date::format("%X", m_reportedToday).c_str());
    }
    QString reportedYesterdayStr() const
    {
        return QString(date::format("%X", m_reportedYesterday).c_str());
    }
    QString reportedThisWeekStr() const
    {
        return QString(date::format("%X", m_reportedThisWeek).c_str());
    }

private:
    Jira mJira{};
    QString mCurrentUser{};
    Jira::TaskList mTasks{};
    Cache mCache{};

    void updateReportSummary();
    bool loadTasksFromCache();

signals:
    void reportedTodayStrChanged(void);
    void reportedYesterdayStrChanged(void);
    void reportedThisWeekStrChanged(void);

public slots:
    void processReceivedTasks();
};

#endif  // TASKMANAGER_H
