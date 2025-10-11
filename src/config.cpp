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
    QVariantList ids = settings.value("ids").toList(); // current list

    const auto iter = std::ranges::find_if(ids, [&](const QVariant& variant) {
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
} // namespace config
