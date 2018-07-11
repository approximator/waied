#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QQmlListProperty>

#include "date/date.h"
#include "QQmlAutoPropertyHelpers.h"

#include "worklog.h"

class Task : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString, title)
    QML_WRITABLE_AUTO_PROPERTY(QString, key)
    QML_WRITABLE_AUTO_PROPERTY(QString, id)
    QML_WRITABLE_AUTO_PROPERTY(QString, url)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::seconds, timeSpent)
    Q_PROPERTY(QQmlListProperty<WorkLog> workLog READ workLog)

    Q_PROPERTY(QString timeSpentStr READ timeSpentStr NOTIFY timeSpentStrChanged)

public:
    explicit Task(QString p_title, QString p_key, QString p_id, QString p_url, std::chrono::seconds p_timeSpent,
                  QObject *parent = nullptr);

    QQmlListProperty<WorkLog> workLog();
    void appendWorkLogItem(WorkLog *wl);
    int workLogCount() const;
    WorkLog *workLogItem(int i) const;
    void clearWorkLog();

    QString timeSpentStr() const
    {
        return QString(date::format("%X", m_timeSpent).c_str());
    }

private:
    QVector<WorkLog *> m_workLog;

    static void appendWorkLogItem(QQmlListProperty<WorkLog> *list, WorkLog *wl);
    static int workLogCount(QQmlListProperty<WorkLog> *list);
    static WorkLog *workLogItem(QQmlListProperty<WorkLog> *list, int i);
    static void clearWorkLog(QQmlListProperty<WorkLog> *list);

signals:
    void timeSpentStrChanged(void);

public slots:
};

#endif  // TASK_H
