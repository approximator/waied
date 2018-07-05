#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>

#include "QQmlAutoPropertyHelpers.h"

class Task : public QObject
{
    Q_OBJECT
    QML_WRITABLE_AUTO_PROPERTY(QString, title)
    QML_WRITABLE_AUTO_PROPERTY(QString, key)
    QML_WRITABLE_AUTO_PROPERTY(QString, id)

public:
    explicit Task(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TASK_H
