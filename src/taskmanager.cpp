
#include <QDebug>

#include "taskmanager.h"

TaskManager::TaskManager(QObject *parent)
    : m_model(new TTasksModel(this))
    , QObject(parent)
{
    qDebug() << __FUNCTION__;
    // mJira.searchTasks("worklogAuthor=currentuser() AND worklogDate >= startOfDay()+order+by+updatedDate");
    mJira.searchTasks("assignee=currentuser()+order+by+updatedDate");

    connect(&mJira, &Jira::taskSearchComplete, this, [=]() {
        qDebug() << "Tasks found: " << mJira.tasks().size();
        for (const auto &jiraTask : mJira.tasks()) {
            auto task = new Task(jiraTask.title, jiraTask.key, jiraTask.id, jiraTask.url, jiraTask.timeSpent, m_model);
            for (const auto jiraWorklogItem : jiraTask.worklog) {
                auto worklogItem
                    = new WorkLog(jiraWorklogItem.author, jiraWorklogItem.comment, jiraWorklogItem.created,
                                  jiraWorklogItem.started, jiraWorklogItem.updated, jiraWorklogItem.id,
                                  jiraWorklogItem.issueId, jiraWorklogItem.url, jiraWorklogItem.timeSpentSec, task);
                task->appendWorkLogItem(worklogItem);
            }
            m_model->append(task);
        }
    });
}
