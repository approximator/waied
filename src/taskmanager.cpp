
#include <QDebug>

#include "taskmanager.h"

TaskManager::TaskManager(QObject *parent)
    : m_model(new TTasksModel(this))
    , m_reportedToday(std::chrono::seconds{ 0 })
    , m_reportedYesterday(std::chrono::seconds{ 0 })
    , m_reportedThisWeek(std::chrono::seconds{ 0 })
    , QObject(parent)
{
    qDebug() << __FUNCTION__;
    connect(this, &TaskManager::reportedTodayChanged, this, [this]() { emit reportedTodayStrChanged(); });
    connect(this, &TaskManager::reportedYesterdayChanged, this, [this]() { emit reportedYesterdayStrChanged(); });
    connect(this, &TaskManager::reportedThisWeekChanged, this, [this]() { emit reportedThisWeekStrChanged(); });
    connect(&mJira, &Jira::taskSearchComplete, this, &TaskManager::processReceivedTasks);

    QSettings settings;
    updateSettings(settings.value("jira/apiUrl", "").toString(), settings.value("jira/username", "").toString(),
                   settings.value("jira/pass", "").toString());
    updateTasks();
}

void TaskManager::updateTasks()
{
    if (mJira.apiUrl().isEmpty()) {
        qDebug() << "Not updating tasks as there is no Jira API url.";
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
    set_reportedToday(std::chrono::seconds{ 0 });
    set_reportedYesterday(std::chrono::seconds{ 0 });
    set_reportedThisWeek(std::chrono::seconds{ 0 });
    m_model->clear();
    mTasks.clear();  // TODO: do not clear and re-add existing tasks

    for (const auto &jiraTask : mJira.tasks()) {
        mTasks.emplace_back(jiraTask);
        m_model->append(jiraTask.get());

        for (const auto &worklog : jiraTask->workLog()) {
            updateReportSummary(worklog);
        }
    }
}

void TaskManager::updateReportSummary(const WorkLog *worklogItem)
{
    const auto today = date::floor<date::days>(std::chrono::system_clock::now());
    // day starts from 06:00
    const auto startOfToday = today + std::chrono::hours{ 6 };
    const auto startOfYesterday = startOfToday - date::days{ 1 };
    const auto startOfThisWeek = date::sys_days{ today } - (date::weekday{ today } - date::weekday{ 1 });

    if (worklogItem->author() == mCurrentUser) {
        if (worklogItem->started() >= startOfToday) {  // today
            set_reportedToday(m_reportedToday + worklogItem->timeSpentSec());
            set_reportedThisWeek(m_reportedThisWeek + worklogItem->timeSpentSec());
        } else if (worklogItem->started() >= startOfYesterday && worklogItem->started() < startOfToday) {  // yesterday
            set_reportedYesterday(m_reportedYesterday + worklogItem->timeSpentSec());
            set_reportedThisWeek(m_reportedThisWeek + worklogItem->timeSpentSec());
        } else if (worklogItem->started() >= startOfThisWeek) {  //start of week
            set_reportedThisWeek(m_reportedThisWeek + worklogItem->timeSpentSec());
        }
    }
}
