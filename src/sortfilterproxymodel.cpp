#include <string>
#include <unordered_set>

#include "log.h"
#include "sortfilterproxymodel.h"

Q_DECLARE_METATYPE(std::chrono::system_clock::time_point)

static const std::unordered_set<std::string> AllowedSortFields{ "updated", "lastCurrentUserLog", "status" };

TasksSortFilterProxyModel::TasksSortFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

QString TasksSortFilterProxyModel::sortBy() const
{
    return m_sortBy;
}

bool TasksSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    const auto leftData = left.data(sortRole());
    const auto rightData = right.data(sortRole());

    if (m_sortBy == "updated" || m_sortBy == "lastCurrentUserLog") {
        const auto leftUpdated = leftData.value<std::chrono::system_clock::time_point>();
        const auto rightUpdated = rightData.value<std::chrono::system_clock::time_point>();
        return leftUpdated < rightUpdated;
    } else if (m_sortBy == "status") {
        return leftData.value<QString>() < rightData.value<QString>();
    }

    qWarning() << "Don't know how to sort by" << m_sortBy;
    return false;
}

void TasksSortFilterProxyModel::set_sortBy(const QString &p_sortBy)
{
    if (m_sortBy == p_sortBy)
        return;

    if (AllowedSortFields.find(p_sortBy.toStdString()) == std::end(AllowedSortFields)) {
        qWarning() << "Don't know how to sort by" << p_sortBy;
        return;
    }

    m_sortBy = p_sortBy;
    const auto sortRole = roleNames().key(m_sortBy.toUtf8(), -1);
    qDebug() << "Set sort role:" << m_sortBy << "(" << sortRole << ")";

    setSortRole(sortRole);
    invalidate();
    sort(0, Qt::DescendingOrder);

    emit sortByChanged(m_sortBy);
}
