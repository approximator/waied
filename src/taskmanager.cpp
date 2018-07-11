
#include <QDebug>

#include "taskmanager.h"

TaskManager::TaskManager(QObject *parent)
    : m_model(new TTasksModel(this))
    , m_reportedToday(0)
    , m_reportedYesterday(0)
    , m_reportedThisWeek(0)
    , QObject(parent)
{
    qDebug() << __FUNCTION__;
    connect(this, &TaskManager::reportedTodayChanged, this, [this]() { emit reportedTodayStrChanged(); });
    connect(this, &TaskManager::reportedYesterdayChanged, this, [this]() { emit reportedYesterdayStrChanged(); });
    connect(this, &TaskManager::reportedThisWeekChanged, this, [this]() { emit reportedThisWeekStrChanged(); });
    connect(&mJira, &Jira::taskSearchComplete, this, &TaskManager::processReceivedTasks);

    // mJira.searchTasks("worklogAuthor=currentuser() AND worklogDate >= startOfDay()+order+by+updatedDate");
    mJira.searchTasks("( assignee=currentuser() OR worklogAuthor=currentuser() )+order+by+updatedDate");
}

void TaskManager::processReceivedTasks()
{
    qDebug() << "Tasks found: " << mJira.tasks().size();
    for (const auto &jiraTask : mJira.tasks()) {
        auto task = new Task(jiraTask.title, jiraTask.key, jiraTask.id, jiraTask.url, jiraTask.timeSpent, m_model);
        // fmt::print("{} = {}\n", jiraTask.title.toStdString(), jiraTask.id.toStdString());
        for (const auto &jiraWorklogItem : jiraTask.worklog) {
            auto worklogItem
                = new WorkLog(jiraWorklogItem.author, jiraWorklogItem.comment, jiraWorklogItem.created,
                              jiraWorklogItem.started, jiraWorklogItem.updated, jiraWorklogItem.id,
                              jiraWorklogItem.issueId, jiraWorklogItem.url, jiraWorklogItem.timeSpentSec, task);
            task->appendWorkLogItem(worklogItem);
            updateReportSummary(worklogItem);
        }
        m_model->append(task);
    }
}

void TaskManager::updateReportSummary(const WorkLog *worklogItem)
{
    if (worklogItem->author() == mCurrentUser) {
        // day starts from 06:00
        const auto startOfToday = date::floor<date::days>(std::chrono::system_clock::now()) + std::chrono::hours{ 6 };
        const auto startOfYesterday = startOfToday - date::days{ 1 };

        if (worklogItem->started() >= startOfToday) {  // today
            set_reportedToday(m_reportedToday + worklogItem->timeSpentSec());
        } else if (worklogItem->started() >= startOfYesterday && worklogItem->started() < startOfToday) {  // yesterday
            set_reportedYesterday(m_reportedYesterday + worklogItem->timeSpentSec());
        }
        // TODO: this week
    }
}
