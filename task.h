#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDateTime>
#include <QQmlListProperty>

#include "QQmlAutoPropertyHelpers.h"

#include "worklog.h"

class Task : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString, title)
    QML_WRITABLE_AUTO_PROPERTY(QString, key)
    QML_WRITABLE_AUTO_PROPERTY(QString, id)
    QML_WRITABLE_AUTO_PROPERTY(QString, url)
    QML_WRITABLE_AUTO_PROPERTY(int, timeSpent)
    Q_PROPERTY(QQmlListProperty<WorkLog> workLog READ workLog)

public:
    explicit Task(QString p_title, QString p_key, QString p_id, QString p_url, int p_timeSpent,
                  QObject *parent = nullptr);

    QQmlListProperty<WorkLog> workLog();
    void appendWorkLogItem(WorkLog *wl);
    int workLogCount() const;
    WorkLog *workLogItem(int i) const;
    void clearWorkLog();

private:
    QVector<WorkLog *> m_workLog;

    static void appendWorkLogItem(QQmlListProperty<WorkLog> *list, WorkLog *wl);
    static int workLogCount(QQmlListProperty<WorkLog> *list);
    static WorkLog *workLogItem(QQmlListProperty<WorkLog> *list, int i);
    static void clearWorkLog(QQmlListProperty<WorkLog> *list);

signals:

public slots:
};

#endif  // TASK_H
