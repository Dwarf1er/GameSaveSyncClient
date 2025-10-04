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

    auto it = std::find_if(ids.begin(), ids.end(), [&](const QVariant& v) {
        return v.toInt() == idToRemove;
    });

    if (it != ids.end()) {
        ids.erase(it);                 // remove the entry
        settings.setValue("ids", ids); // persist the updated list
    }
}

QList<int> returnAllIds() {
    QSettings settings = config::getConfig();
    QVariantList ids = settings.value("ids").toList();
    QList<int> idList;
    for (const QVariant& v : ids) {
        idList.push_back(v.toInt());
    }
    return idList;
}
} // namespace config
