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
    QML_WRITABLE_AUTO_PROPERTY(QDateTime, created)
    QML_WRITABLE_AUTO_PROPERTY(QDateTime, started)
    QML_WRITABLE_AUTO_PROPERTY(QDateTime, updated)
    QML_WRITABLE_AUTO_PROPERTY(QString, id)
    QML_WRITABLE_AUTO_PROPERTY(QString, issueId)
    QML_WRITABLE_AUTO_PROPERTY(QString, url)
    QML_WRITABLE_AUTO_PROPERTY(int, timeSpentSec)

public:
    explicit WorkLog(QString p_author, QString p_comment, QDateTime p_created, QDateTime p_started, QDateTime p_updated,
                     QString p_id, QString p_issueId, QString p_url, int p_timeSpentSec, QObject *parent = nullptr);

signals:

public slots:
};

#endif  // WORKLOG_H
