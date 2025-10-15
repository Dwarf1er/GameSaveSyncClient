#include "config.h"

namespace config {
void addId(int newId) {
    QSettings settings = config::getConfig();
    QVariantList ids = settings.value("ids").toList();

    if (!ids.contains(QVariant(newId))) {
        ids.append(QVariant(newId));
        settings.setValue("ids", ids);
    }
}

void removeId(int idToRemove) {
    QSettings settings = getConfig();
    QVariantList ids = settings.value("ids").toList();

    const auto iter =
        std::ranges::find_if(ids, [&](const QVariant& variant) -> bool {
            return variant.toInt() == idToRemove;
        });

    if (iter != ids.end()) {
        ids.erase(iter);
        settings.setValue("ids", ids);
    }
}

QList<int> returnAllIds() {
    QSettings settings = config::getConfig();
    QVariantList ids = settings.value("ids").toList();
    QList<int> idList;
    for (const QVariant& variant : ids) {
        idList.push_back(variant.toInt());
    }
    return idList;
}

inline QString getPathKey(int pathID) {
    return "Path: " + QString::number(pathID);
}

void updatePath(int pathID, QString path) {
    QSettings settings = config::getConfig();
    settings.setValue(getPathKey(pathID), path);
}
void removePath(int pathID) {
    QSettings settings = config::getConfig();
    settings.remove(getPathKey(pathID));
}

QString getPath(int pathID) {
    QSettings settings = config::getConfig();
    return settings.value(getPathKey(pathID), QString{}).toString();
}

} // namespace config
