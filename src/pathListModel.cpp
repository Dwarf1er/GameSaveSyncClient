#include "pathListModel.h"
#include "config.h"
#include "gameSyncServerUtil.h"
#include <QBrush>
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
        if (item.configPath.isEmpty())
            return item.dbPath;
        else
            return item.configPath;
    case Role::DbPathRole:
        return item.dbPath;
    case Role::IdRole:
        return item.id;
    case Role::ConfigPathRole:
        return item.configPath;
    case Qt::BackgroundRole:
        if (item.configPath.isEmpty()) {
            return QBrush(QColor(255, 0, 0, 40));
        }
        return {};
    case Qt::ForegroundRole:
        if (item.configPath.isEmpty()) {
            return QBrush(Qt::gray);
        }
        return {};
    default:
        return {};
    }
}

bool PathListModel::setData(const QModelIndex& index, const QVariant& value,
                            int role) {
    if (!index.isValid() || index.row() < 0 || index.row() >= pathItems.size())
        return false;

    PathItem& item = pathItems[index.row()];
    bool changed = false;

    if (role == Role::ConfigPathRole || role == Qt::EditRole) {
        const QString newVal = value.toString();
        if (item.configPath != newVal) {
            item.configPath = newVal;
            changed = true;
            if (item.configPath.isEmpty())
                config::removePath(item.id);
            else
                config::updatePath(item.id, item.configPath);
        }
    }

    if (changed) {
        emit dataChanged(
            index, index,
            {Role::ConfigPathRole, Qt::BackgroundRole, Qt::ForegroundRole});
        return true;
    }

    return false;
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
        item.configPath = config::getPath(item.id);
        pathItems.append(item);
    }

    endResetModel();
}
