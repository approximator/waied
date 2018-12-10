
#include <QDebug>

#include "taskmanager.h"

#include "log.h"

TaskManager::TaskManager(QObject *parent)
    : QObject(parent)
    , m_model(new TTasksModel(this))
    , m_reportedToday(std::chrono::seconds{ 0 })
    , m_reportedYesterday(std::chrono::seconds{ 0 })
    , m_reportedThisWeek(std::chrono::seconds{ 0 })
{
    qDebug() << __FUNCTION__;
    connect(this, &TaskManager::reportedTodayChanged, this, [this]() { emit reportedTodayStrChanged(); });
    connect(this, &TaskManager::reportedYesterdayChanged, this, [this]() { emit reportedYesterdayStrChanged(); });
    connect(this, &TaskManager::reportedThisWeekChanged, this, [this]() { emit reportedThisWeekStrChanged(); });
    connect(&mJira, &Jira::taskSearchComplete, this, &TaskManager::processReceivedTasks);
    connect(&mJira, &Jira::worklogsUpdated, this, &TaskManager::updateReportSummary);

    QSettings settings;
    updateSettings(settings.value("jira/apiUrl", "").toString(), settings.value("jira/username", "").toString(),
                   settings.value("jira/pass", "").toString());

    loadTasksFromCache();
    updateTasks();
}

TaskManager::~TaskManager()
{
    mCache.saveTasks(mTasks);
}

void TaskManager::updateTasks()
{
    if (mJira.apiUrl().isEmpty()) {
        qWarning() << "Not updating tasks as there is no Jira API url.";
        return;
    }
    // mJira.searchTasks("worklogAuthor=currentuser() AND worklogDate >= startOfDay()+order+by+updatedDate");
    mJira.searchTasks("( assignee=currentuser() OR worklogAuthor=currentuser() )+order+by+updatedDate");
}

void TaskManager::updateSettings(const QString &jiraUrl, const QString &username, const QString &pass)
{
    qDebug() << "Updating settings... Username: " << username;
    mCurrentUser = username;
    mJira.set_apiUrl(jiraUrl);
    mJira.set_username(username);
    mJira.set_password(pass);
    qDebug() << "Jira API: " << jiraUrl;
}

void TaskManager::processReceivedTasks()
{
    qDebug() << "Tasks found: " << mJira.tasks().size();

    for (const auto &jiraTask : mJira.tasks()) {
        if (!mTasks.contains(jiraTask->key())) {
            qInfo() << "New task: " << jiraTask->key();
            mTasks[jiraTask->key()] = jiraTask;
            mJira.updateWorklog(*jiraTask);
            m_model->append(jiraTask.get());
        } else {
            auto task = mTasks[jiraTask->key()];
            if (task->lastWorklogFetch() < jiraTask->updated()) {
                qInfo() << "The task" << jiraTask->key() << "was updated on" << jiraTask->updated()
                        << "but worklogs for this task were fetched on" << task->lastWorklogFetch();
                task->set_title(jiraTask->title());
                task->set_taskId(jiraTask->taskId());
                task->set_url(jiraTask->url());
                task->set_updated(jiraTask->updated());
                task->set_timeSpent(jiraTask->timeSpent());
                task->set_priority(jiraTask->priority());
                task->set_status(jiraTask->status());
                mJira.updateWorklog(*task);
            }
        }
    }

    mCache.saveTasks(mTasks);
}

void TaskManager::updateReportSummary(const Task *updatedTask)
{
    qDebug() << "Updated worklogs of " << updatedTask->key();

    const auto today = date::floor<date::days>(std::chrono::system_clock::now());
    // day starts from 06:00
    const auto startOfToday = today + std::chrono::hours{ 6 };
    const auto startOfYesterday = startOfToday - date::days{ 1 };
    const auto startOfThisWeek = date::sys_days{ today } - (date::weekday{ today } - date::weekday{ 1 });

    auto reportedToday = std::chrono::seconds{ 0 };
    auto reportedYesterday = std::chrono::seconds{ 0 };
    auto reportedThisWeek = std::chrono::seconds{ 0 };

    for (const auto &task : mTasks) {
        for (const auto &worklogItem : task->workLog()) {
            if (worklogItem->author() == mCurrentUser) {
                if (worklogItem->started() >= startOfToday) {  // today
                    reportedToday += worklogItem->timeSpentSec();
                    reportedThisWeek += worklogItem->timeSpentSec();
                } else if (worklogItem->started() >= startOfYesterday
                           && worklogItem->started() < startOfToday) {  // yesterday
                    reportedYesterday += worklogItem->timeSpentSec();
                    reportedThisWeek += worklogItem->timeSpentSec();
                } else if (worklogItem->started() >= startOfThisWeek) {  //start of week
                    reportedThisWeek += worklogItem->timeSpentSec();
                }
            }
        }
    }
    set_reportedToday(reportedToday);
    set_reportedYesterday(reportedYesterday);
    set_reportedThisWeek(reportedThisWeek);
}

bool TaskManager::loadTasksFromCache()
{
    mTasks.clear();
    m_model->clear();
    for (const auto &task : mCache.loadTasks()) {
        mTasks[task->key()] = task;
        m_model->append(task.get());
    }
    if (!mTasks.isEmpty()) {
        updateReportSummary(mTasks.values().first().get());
    }
    return true;
}
