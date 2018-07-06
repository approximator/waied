#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include <QObject>

#include "QQmlObjectListModel.h"
#include "QQmlAutoPropertyHelpers.h"

#include "jira.h"
#include "task.h"

using TTasksModel = QQmlObjectListModel<Task>;

class TaskManager : public QObject
{
    Q_OBJECT
    QML_READONLY_AUTO_PROPERTY(TTasksModel *, model)
public:
    explicit TaskManager(QObject *parent = nullptr);

private:
    Jira mJira;

signals:

public slots:
};

#endif  // TASKMANAGER_H
