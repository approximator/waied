
#include <catch2/catch.hpp>

#include "helpers.h"
#include "../src/cache.h"

static auto getTask(const QString &key)
{
    return std::make_shared<Task>(
        "title", key, "id", "url", date::floor<std::chrono::seconds>(std::chrono::system_clock::now()),
        std::chrono::seconds{ 1000 }, "Major", "Open", std::chrono::system_clock::time_point{});
}

static WorkLog *getWorklogItem(Task &task)
{
    return new WorkLog("Author", "comment", date::floor<std::chrono::seconds>(std::chrono::system_clock::now()),
                       date::floor<std::chrono::seconds>(std::chrono::system_clock::now()),
                       date::floor<std::chrono::seconds>(std::chrono::system_clock::now()), "id", "issueId", "url",
                       std::chrono::seconds{ 222 }, &task);
}

SCENARIO("Read tasks have to be the same as previously written")
{
    qSetMessagePattern("%{time process}  [%{type}] %{file}:%{line}    %{message}");
    GIVEN("A list with some tasks")
    {
        const auto cacheDir = "./";
        std::shared_ptr<Task> task = getTask("task1");

        task->appendWorkLogItem(getWorklogItem(*task));
        task->appendWorkLogItem(getWorklogItem(*task));
        task->appendWorkLogItem(getWorklogItem(*task));
        auto task2 = getTask("task2");
        task2->appendWorkLogItem(getWorklogItem(*task2));
        task2->appendWorkLogItem(getWorklogItem(*task2));
        const auto tasks = Cache::TaskList{ { task->key(), task }, { task2->key(), task2 } };
        REQUIRE(tasks.size() == 2);

        WHEN("The tasks are written to cache")
        {
            auto cache = Cache(cacheDir);
            const auto writeResult = cache.saveTasks(tasks);

            THEN("Write result is true")
            {
                REQUIRE(writeResult);
            }
        }

        WHEN("Tasks read from the cache")
        {
            auto cache = Cache(cacheDir);
            const auto readTasks = cache.loadTasks();

            THEN("Read tasks are equal to the test tasks")
            {
                REQUIRE(readTasks.size() == tasks.size());
                REQUIRE(std::equal(begin(tasks), end(tasks), begin(readTasks), compareTasks));
            }
        }
    }
}
