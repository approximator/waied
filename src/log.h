#ifndef LOG_H
#define LOG_H

#include <chrono>
#include <sstream>

#include <QDebug>

#include <date/date.h>

inline QDebug operator<<(QDebug dbg, const std::chrono::system_clock::time_point &tp)
{
    static const auto formatStr = "%FT%H:%M:%S%z";
    std::ostringstream ss;
    ss << date::format(formatStr, tp);
    dbg << QString(ss.str().c_str());
    return dbg;
}

#endif  // LOG_H
