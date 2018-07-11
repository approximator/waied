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

using TTasksModel = QQmlObjectListModel<Task>;

class TaskManager : public QObject
{
    Q_OBJECT
    QML_READONLY_AUTO_PROPERTY(TTasksModel *, model)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::seconds, reportedToday)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::seconds, reportedYesterday)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::seconds, reportedThisWeek)

    Q_PROPERTY(QString reportedTodayStr READ reportedTodayStr NOTIFY reportedTodayStrChanged)
    Q_PROPERTY(QString reportedYesterdayStr READ reportedYesterdayStr NOTIFY reportedYesterdayStrChanged)
    Q_PROPERTY(QString reportedThisWeekStr READ reportedThisWeekStr NOTIFY reportedThisWeekStrChanged)

public:
    explicit TaskManager(QObject *parent = nullptr);

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
    Jira mJira;
    QString mCurrentUser{ QSettings().value("jira/username", "username").toString() };

    void updateReportSummary(const WorkLog *worklogItem);

signals:
    void reportedTodayStrChanged(void);
    void reportedYesterdayStrChanged(void);
    void reportedThisWeekStrChanged(void);

public slots:
    void processReceivedTasks();
};

#endif  // TASKMANAGER_H
