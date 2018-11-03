
#include <catch2/catch.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>

#include "helpers.h"
#include "../src/cache.h"

TEST_CASE("Test cache read/write")
{
    rc::prop("Tasks write/read", [](const std::list<std::shared_ptr<Task>> &tasks) {
        const auto cacheDir = "./";
        {
            auto cache = Cache(cacheDir);
            const auto writeResult = cache.saveTasks(tasks);
            REQUIRE(writeResult);
        }

        {
            auto cache = Cache(cacheDir);
            const auto readTasks = cache.loadTasks();
            REQUIRE(readTasks.size() == tasks.size());
            REQUIRE(std::equal(begin(tasks), end(tasks), begin(readTasks), compareTasks));
        }
    });
}
