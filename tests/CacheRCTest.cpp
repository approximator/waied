
#include <catch2/catch.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>

#include "helpers.h"
#include "../src/cache.h"

TEST_CASE("Test cache read/write")
{
    qSetMessagePattern("%{time process}  [%{type}] %{file}:%{line}    %{message}");
    rc::prop("Tasks write/read", [](const std::list<std::shared_ptr<Task>> &tasks) {
        auto qTasks = QHash<QString, std::shared_ptr<Task>>{};
        for (const auto &task : tasks) {
            qTasks[task->key()] = task;
        }

        const auto cacheDir = "./";
        {
            auto cache = Cache(cacheDir);
            const auto writeResult = cache.saveTasks(qTasks);
            REQUIRE(writeResult);
        }

        {
            auto cache = Cache(cacheDir);
            const auto readTasks = cache.loadTasks();
            REQUIRE(readTasks.size() == qTasks.size());
            REQUIRE(std::equal(begin(qTasks), end(qTasks), begin(readTasks), compareTasks));
        }
    });
}
