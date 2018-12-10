#ifndef CACHE_H
#define CACHE_H

#include <list>
#include <memory>

#include "jira.h"
#include "task.h"
#include "worklog.h"

class Cache
{
public:
    using TaskList = Jira::TaskList;

    Cache();
    Cache(const QString &dataDir, const QString &dataFile = "tasks.cache");

    bool saveTasks(const TaskList &tasks);
    TaskList loadTasks() const;

    static QString getDataDir();

private:
    QString mDataFile{};
};

#endif  // CACHE_H
