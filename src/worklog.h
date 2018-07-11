#ifndef WORKLOG_H
#define WORKLOG_H

#include <QObject>
#include <QDateTime>

#include "QQmlAutoPropertyHelpers.h"

class WorkLog : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString, author)
    QML_WRITABLE_AUTO_PROPERTY(QString, comment)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::system_clock::time_point, created)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::system_clock::time_point, started)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::system_clock::time_point, updated)
    QML_WRITABLE_AUTO_PROPERTY(QString, id)
    QML_WRITABLE_AUTO_PROPERTY(QString, issueId)
    QML_WRITABLE_AUTO_PROPERTY(QString, url)
    QML_WRITABLE_AUTO_PROPERTY(std::chrono::seconds, timeSpentSec)

public:
    explicit WorkLog(QString p_author, QString p_comment, std::chrono::system_clock::time_point p_created,
                     std::chrono::system_clock::time_point p_started, std::chrono::system_clock::time_point p_updated,
                     QString p_id, QString p_issueId, QString p_url, std::chrono::seconds p_timeSpentSec,
                     QObject *parent = nullptr);

signals:

public slots:
};

#endif  // WORKLOG_H
