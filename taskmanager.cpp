
#include <QDebug>

#include "taskmanager.h"

TaskManager::TaskManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << __FUNCTION__;
    // mJira.searchTasks("worklogAuthor=currentuser() AND worklogDate >= startOfDay()+order+by+updatedDate");
    mJira.searchTasks("assignee=currentuser()+order+by+updatedDate");

    connect(&mJira, &Jira::taskSearchComplete, this, [=]() {
        qDebug() << "Tasks found: " << mJira.tasks().size();
        for (const auto &task : mJira.tasks()) {
            qDebug() << "\nTitle: " << task.title << "\nKey  : " << task.key << "\nId   : " << task.id
                     << "\nUrl  : " << task.url;
        }
    });
}
