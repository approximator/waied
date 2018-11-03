#ifndef HELPERS_H
#define HELPERS_H

#include <chrono>
#include <memory>

#include <QString>
#include <date/date.h>

#include <catch2/catch.hpp>
#include <rapidcheck.h>

#include "../src/task.h"

/**
 * Stream operators for types Catch doesn't know about
 */
namespace Catch
{

inline std::ostream &operator<<(std::ostream &os, const QString &value)
{
    os << value.toStdString();
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const std::chrono::seconds &sec)
{
    os << sec.count();
    return os;
}

inline std::ostream &operator<<(std::ostream &os, const std::chrono::system_clock::time_point &tp)
{
    return date::operator<<(os, tp);
}

}  // namespace Catch

/**
 * Rapidcheck's generators for our types
 */
namespace rc
{
template <>
struct Arbitrary<QString>
{
    static Gen<QString> arbitrary()
    {
        return gen::map<std::string>([](std::string &&str) { return QString(str.c_str()); });
    }
};

template <>
struct Arbitrary<std::shared_ptr<Task>>
{
    static auto arbitrary()
    {
        auto updated = rc::gen::map(rc::gen::suchThat<std::chrono::system_clock::time_point>([](auto tp) {
                                        const auto y = date::year_month_day{ date::floor<date::days>(tp) }.year();
                                        return y <= date::year{ 9999 } && y >= date::year{ 1 };
                                    }),
                                    [](auto tp) { return date::floor<std::chrono::seconds>(tp); });

        return rc::gen::makeShared<Task>(rc::gen::arbitrary<QString>(), rc::gen::arbitrary<QString>(),
                                         rc::gen::arbitrary<QString>(), rc::gen::arbitrary<QString>(), updated,
                                         rc::gen::arbitrary<std::chrono::seconds>(), rc::gen::arbitrary<QString>(),
                                         rc::gen::arbitrary<QString>());
    }
};

}  // namespace rc

/**
 * Helper comparator to use in std::equal
 */
inline bool compareTasks(const std::shared_ptr<Task> &task1, const std::shared_ptr<Task> &task2)
{
    SECTION("compare task")
    {
        REQUIRE(task1->title() == task2->title());
        REQUIRE(task1->key() == task2->key());
        REQUIRE(task1->id() == task2->id());
        REQUIRE(task1->url() == task2->url());
        REQUIRE(task1->timeSpent() == task2->timeSpent());
        REQUIRE(date::floor<std::chrono::seconds>(task1->updated())
                == date::floor<std::chrono::seconds>(task2->updated()));
        REQUIRE(task1->priority() == task2->priority());
        REQUIRE(task1->status() == task2->status());

        const auto worklogCount1 = task1->workLogCount();
        const auto worklogCount2 = task2->workLogCount();
        REQUIRE(worklogCount1 == worklogCount2);
        auto index = worklogCount1;
        while (index--) {
            const auto wl1 = task1->workLogItem(index);
            const auto wl2 = task2->workLogItem(index);
            REQUIRE(wl1->author() == wl2->author());
            REQUIRE(wl1->comment() == wl2->comment());
            REQUIRE(date::floor<std::chrono::seconds>(wl1->created())
                    == date::floor<std::chrono::seconds>(wl2->created()));
            REQUIRE(date::floor<std::chrono::seconds>(wl1->started())
                    == date::floor<std::chrono::seconds>(wl2->started()));
            REQUIRE(date::floor<std::chrono::seconds>(wl1->updated())
                    == date::floor<std::chrono::seconds>(wl2->updated()));
            REQUIRE(wl1->id() == wl2->id());
            REQUIRE(wl1->issueId() == wl2->issueId());
            REQUIRE(wl1->url() == wl2->url());
            REQUIRE(wl1->timeSpentSec() == wl2->timeSpentSec());
        }
    }

    return true;
}

#endif  // HELPERS_H
