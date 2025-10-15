#include "pathListModel.h"
#include "gameSyncServerUtil.h"
#include <QJsonArray>
#include <QJsonObject>

PathListModel::PathListModel(QObject* parent) : QAbstractListModel(parent) {}

int PathListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid())
        return 0;
    return pathItems.size(); // NOLINT
}

QVariant PathListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= pathItems.size())
        return {};

    const PathItem& item = pathItems.at(index.row());
    switch (role) {
    case Qt::DisplayRole:
    case Role::DbPathRole:
        return item.dbPath;
    case Role::IdRole:
        return item.id;
    case Role::ConfigPathRole:
        return item.configPath;
    default:
        return {};
    }
}

Qt::ItemFlags PathListModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void PathListModel::loadForGame(int gameID) {
    beginResetModel();
    pathItems.clear();

    if (gameID == 0) {
        endResetModel();
        return;
    }

    QJsonDocument doc =
        GameSyncServerUtil::getInstance().getPathByGameId(gameID);
    if (!doc.isArray()) {
        endResetModel();
        return;
    }

    const QJsonArray arr = doc.array();
    for (const QJsonValue& val : arr) {
        if (!val.isObject())
            continue;
        QJsonObject obj = val.toObject();
        PathItem item;
        item.id = obj.value("id").toInt();
        item.dbPath = obj.value("path").toString();
        pathItems.append(item);
    }

    endResetModel();
}
