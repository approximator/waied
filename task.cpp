#include "task.h"

Task::Task(QString p_title, QString p_key, QString p_id, QString p_url, int p_timeSpent, QObject *parent)
    : m_title(std::move(p_title))
    , m_key(std::move(p_key))
    , m_id(std::move(p_id))
    , m_url(std::move(p_url))
    , m_timeSpent(std::move(p_timeSpent))
    , QObject(parent)
{
}

void Task::appendWorkLogItem(WorkLog *wl)
{
    m_workLog.append(wl);
}

int Task::workLogCount() const
{
    m_workLog.count();
}

WorkLog *Task::workLogItem(int i) const
{
    return m_workLog.at(i);
}

void Task::clearWorkLog()
{
    m_workLog.clear();
}

void Task::appendWorkLogItem(QQmlListProperty<WorkLog> *list, WorkLog *wl)
{
    reinterpret_cast<Task *>(list->data)->appendWorkLogItem(wl);
}

int Task::workLogCount(QQmlListProperty<WorkLog> *list)
{
    return reinterpret_cast<Task *>(list->data)->workLogCount();
}

WorkLog *Task::workLogItem(QQmlListProperty<WorkLog> *list, int i)
{
    return reinterpret_cast<Task *>(list->data)->workLogItem(i);
}

void Task::clearWorkLog(QQmlListProperty<WorkLog> *list)
{
    reinterpret_cast<Task *>(list->data)->clearWorkLog();
}

QQmlListProperty<WorkLog> Task::workLog()
{
    return QQmlListProperty<WorkLog>(this, this, &Task::appendWorkLogItem, &Task::workLogCount, &Task::workLogItem,
                                     &Task::clearWorkLog);
}
