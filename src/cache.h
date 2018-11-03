#ifndef CACHE_H
#define CACHE_H

#include <list>
#include <memory>

#include "task.h"
#include "worklog.h"

class Cache
{
public:
    using TaskList = std::list<std::shared_ptr<Task>>;

    Cache();
    Cache(const QString &dataDir, const QString &dataFile = "tasks.cache");

    bool saveTasks(const TaskList &tasks);
    TaskList loadTasks() const;

    static QString getDataDir();

private:
    QString mDataFile{};
};

#endif  // CACHE_H
