#include "worklog.h"

WorkLog::WorkLog(QString p_author, QString p_comment, QDateTime p_created, QDateTime p_started, QDateTime p_updated,
                 QString p_id, QString p_issueId, QString p_url, int p_timeSpentSec, QObject *parent)
    : m_author(std::move(p_author))
    , m_comment(std::move(p_comment))
    , m_created(std::move(p_created))
    , m_started(std::move(p_started))
    , m_updated(std::move(p_updated))
    , m_id(std::move(p_id))
    , m_issueId(std::move(p_issueId))
    , m_url(std::move(p_url))
    , m_timeSpentSec(std::move(p_timeSpentSec))
    , QObject(parent)
{
}
