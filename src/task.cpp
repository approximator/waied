#include "task.h"

Task::Task(QString p_title, QString p_key, QString p_id, QString p_url, std::chrono::seconds p_timeSpent,
           QString p_priority, QString p_status, QObject *parent)
    : QObject(parent)
    , m_title(std::move(p_title))
    , m_key(std::move(p_key))
    , m_id(std::move(p_id))
    , m_url(std::move(p_url))
    , m_timeSpent(std::move(p_timeSpent))
    , m_priority(std::move(p_priority))
    , m_status(std::move(p_status))
    , m_workLog(this)
{
    connect(this, &Task::timeSpentChanged, this, [this]() { emit timeSpentStrChanged(); });
}

void Task::appendWorkLogItem(WorkLog *wl)
{
    m_workLog.append(&m_workLog, wl);
}

int Task::workLogCount()
{
    return m_workLog.count(&m_workLog);
}

WorkLog *Task::workLogItem(int i)
{
    return m_workLog.at(&m_workLog, i);
}

void Task::clearWorkLog()
{
    m_workLog.clear(&m_workLog);
}

QString Task::timeSpentStr() const
{
    return QString(date::format("%X", m_timeSpent).c_str());
}
