#ifndef SORTFILTERPROXYMODEL_H
#define SORTFILTERPROXYMODEL_H

#include <QString>
#include <QSortFilterProxyModel>

class TasksSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString sortBy READ sortBy WRITE set_sortBy NOTIFY sortByChanged)

public:
    explicit TasksSortFilterProxyModel(QObject *parent = nullptr);

    QString sortBy() const;

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

private:
    QString m_sortBy{ "updated" };

signals:

    void sortByChanged(QString sortBy);

public slots:
    void set_sortBy(const QString &p_sortBy);
};

#endif  // SORTFILTERPROXYMODEL_H
