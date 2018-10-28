
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
    m_model->clear();
    set_reportedToday(std::chrono::seconds{ 0 });
    set_reportedYesterday(std::chrono::seconds{ 0 });
    set_reportedThisWeek(std::chrono::seconds{ 0 });

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
        const auto startOfThisWeek = date::floor<date::weeks>(std::chrono::system_clock::now()) - date::days {3} + std::chrono::hours{ 6 };

        if (worklogItem->started() >= startOfToday) {  // today
            set_reportedToday(m_reportedToday + worklogItem->timeSpentSec());
            set_reportedThisWeek(m_reportedThisWeek + worklogItem->timeSpentSec());
        } else if (worklogItem->started() >= startOfYesterday && worklogItem->started() < startOfToday) {  // yesterday
            set_reportedYesterday(m_reportedYesterday + worklogItem->timeSpentSec());
            set_reportedThisWeek(m_reportedThisWeek + worklogItem->timeSpentSec());
        }
        else if (worklogItem->started() >= startOfThisWeek) { //start of week
            std::cout << "setting week" << std::endl;
            set_reportedThisWeek(m_reportedThisWeek + worklogItem->timeSpentSec());
        }
    }
}
