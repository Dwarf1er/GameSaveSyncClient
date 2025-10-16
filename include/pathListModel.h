#pragma once

#include <QAbstractListModel>
#include <QString>
#include <QVector>

struct PathItem {
    int id{0};
    QString dbPath;
    QString configPath;
};

class PathListModel : public QAbstractListModel {
    Q_OBJECT
  public:
    enum Role { IdRole = Qt::UserRole + 1, DbPathRole, ConfigPathRole };

    PathListModel(QObject* parent = nullptr);

    [[nodiscard]] int
    rowCount(const QModelIndex& parent = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index,
                                int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value,
                 int role) override;
    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex& index) const override;
    [[nodiscard]] const QVector<PathItem>& items() const { return pathItems; }
    void loadForGame(int gameId);

  private:
    QVector<PathItem> pathItems;

    [[nodiscard]] bool isPathValid(PathItem item) const;
};
