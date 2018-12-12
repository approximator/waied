#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QQmlListProperty>

#include "date/date.h"
#include "QQmlListPropertyHelper.h"
#include "QQmlAutoPropertyHelpers.h"

#include "worklog.h"

class Task : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString, title)
    QML_WRITABLE_AUTO_PROPERTY(QString, key)
    QML_WRITABLE_AUTO_PROPERTY(QString, taskId)
    QML_WRITABLE_AUTO_PROPERTY(QString, url)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::system_clock::time_point, updated)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::seconds, timeSpent)
    QML_WRITABLE_AUTO_PROPERTY(QString, priority)
    QML_WRITABLE_AUTO_PROPERTY(QString, status)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::system_clock::time_point, lastWorklogFetch)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::system_clock::time_point, lastCurrentUserLog)
    QML_LIST_PROPERTY(WorkLog, workLog)

    Q_PROPERTY(QString timeSpentStr READ timeSpentStr NOTIFY timeSpentStrChanged)

public:
    explicit Task(QObject *parent = nullptr);
    Task(QString p_title, QString p_key, QString p_id, QString p_url, std::chrono::system_clock::time_point p_updated,
         std::chrono::seconds p_timeSpent, QString p_priority, QString p_status,
         std::chrono::system_clock::time_point p_lastWorklogFetch, QObject *parent = nullptr);

    void appendWorkLogItem(WorkLog *wl);
    int workLogCount();
    WorkLog *workLogItem(int i);
    void clearWorkLog();

    QString timeSpentStr() const;

signals:
    void timeSpentStrChanged(void);

public slots:
};

#endif  // TASK_H
