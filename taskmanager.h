#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>

#include "jira.h"

class TaskManager : public QObject
{
    Q_OBJECT
public:
    explicit TaskManager(QObject *parent = nullptr);

private:
    Jira mJira;

signals:

public slots:
};

#endif // TASKMANAGER_H
