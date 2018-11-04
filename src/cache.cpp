#include <QDir>
#include <QDebug>
#include <QDataStream>
#include <QStandardPaths>

#include "cache.h"

static QDataStream &operator>>(QDataStream &stream, WorkLog &worklog);
static QDataStream &operator<<(QDataStream &stream, const WorkLog &worklog);
static QDataStream &operator<<(QDataStream &stream, Task &task);
static QDataStream &operator>>(QDataStream &stream, Task &task);

Cache::Cache()
    : Cache(getDataDir())
{
}

Cache::Cache(const QString &dataDir, const QString &dataFile)
    : mDataFile(QDir::toNativeSeparators(QDir(dataDir).absolutePath() + "/" + dataFile))
{
}

bool Cache::saveTasks(const TaskList &tasks)
{
    qDebug() << "Saving tasks to: " << mDataFile;
    QFile file(mDataFile);
    file.open(QIODevice::WriteOnly);
    auto stream = QDataStream(&file);
    stream.setVersion(QDataStream::Qt_5_10);

    stream << static_cast<quint32>(tasks.size());
    for (const auto &task : tasks) {
        stream << *task;
    }

    return true;
}

Cache::TaskList Cache::loadTasks() const
{
    qDebug() << "Loading tasks from: " << mDataFile;
    QFile file(mDataFile);
    file.open(QIODevice::ReadOnly);
    auto stream = QDataStream(&file);
    stream.setVersion(QDataStream::Qt_5_10);

    auto tasks = TaskList{};
    auto tasksCount = quint32{};
    stream >> tasksCount;
    qDebug() << "Tasks count: " << tasksCount;

    while (tasksCount--) {
        auto task = std::make_shared<Task>();
        stream >> *task;
        tasks[task->key()] = task;
    }

    return tasks;
}

QString Cache::getDataDir()
{
    const auto dir = QDir{ QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) };
    const auto path = dir.path();
    dir.mkpath(path);
    return path;
}

static QDataStream &operator>>(QDataStream &stream, std::chrono::system_clock::time_point &tp)
{
    static const auto formatStr = "%FT%H:%M:%3S%z";
    QString str;
    stream >> str;
    std::istringstream in{ str.toStdString() };
    in >> date::parse(formatStr, tp);
    if (!in.good()) {
        qDebug() << "Cannot parse date: " << str;
    }
    return stream;
}

static QDataStream &operator<<(QDataStream &stream, const std::chrono::system_clock::time_point &tp)
{
    static const auto formatStr = "%FT%H:%M:%S%z";
    std::ostringstream ss;
    ss << date::format(formatStr, tp);
    stream << QString(ss.str().c_str());
    return stream;
}

static QDataStream &operator>>(QDataStream &stream, std::chrono::seconds &secs)
{
    quint64 data;
    stream >> data;
    secs = std::chrono::seconds{ data };
    return stream;
}

static QDataStream &operator<<(QDataStream &stream, const std::chrono::seconds &secs)
{
    stream << static_cast<quint64>(secs.count());
    return stream;
}

static QDataStream &operator<<(QDataStream &stream, Task &task)
{
    auto wlCount = task.workLogCount();
    stream << task.title() << task.key() << task.id() << task.url() << task.updated() << task.timeSpent()
           << task.priority() << task.status() << task.lastWorklogFetch() << wlCount;
    for (int i = 0; i < wlCount; ++i) {
        stream << *task.workLogItem(i);
    }
    return stream;
}

static QDataStream &operator>>(QDataStream &stream, Task &task)
{
    auto title = decltype(task.title()){};
    auto key = decltype(task.key()){};
    auto id = decltype(task.id()){};
    auto url = decltype(task.url()){};
    auto updated = decltype(task.updated()){};
    auto timeSpent = decltype(task.timeSpent()){};
    auto priority = decltype(task.priority()){};
    auto status = decltype(task.status()){};
    auto lastWorklogFetch = decltype(task.lastWorklogFetch()){};
    auto wlCount = int{ 0 };
    stream >> title >> key >> id >> url >> updated >> timeSpent >> priority >> status >> lastWorklogFetch >> wlCount;
    task.set_title(title);
    task.set_url(url);
    task.set_key(key);
    task.set_id(id);
    task.set_updated(updated);
    task.set_timeSpent(timeSpent);
    task.set_priority(priority);
    task.set_status(status);
    task.set_lastWorklogFetch(lastWorklogFetch);

    for (int i = 0; i < wlCount; ++i) {
        auto wl = new WorkLog(&task);
        stream >> *wl;
        task.appendWorkLogItem(wl);
    }

    return stream;
}

static QDataStream &operator<<(QDataStream &stream, const WorkLog &worklog)
{
    stream << worklog.author() << worklog.comment() << worklog.created() << worklog.started() << worklog.updated()
           << worklog.id() << worklog.issueId() << worklog.url() << worklog.timeSpentSec();
    return stream;
}

static QDataStream &operator>>(QDataStream &stream, WorkLog &worklog)
{
    auto author = decltype(worklog.author()){};
    auto comment = decltype(worklog.comment()){};
    auto created = decltype(worklog.created()){};
    auto started = decltype(worklog.started()){};
    auto updated = decltype(worklog.updated()){};
    auto id = decltype(worklog.id()){};
    auto issueId = decltype(worklog.issueId()){};
    auto url = decltype(worklog.url()){};
    auto timeSpentSec = decltype(worklog.timeSpentSec()){};
    stream >> author >> comment >> created >> started >> updated >> id >> issueId >> url >> timeSpentSec;
    worklog.set_author(author);
    worklog.set_comment(comment);
    worklog.set_created(created);
    worklog.set_started(started);
    worklog.set_updated(updated);
    worklog.set_id(id);
    worklog.set_issueId(issueId);
    worklog.set_url(url);
    worklog.set_timeSpentSec(timeSpentSec);
    return stream;
}
