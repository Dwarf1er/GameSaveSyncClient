#pragma once

#include <QSettings>

namespace config {
inline QSettings getConfig() {
    return QSettings("config.ini", QSettings::IniFormat);
}
void addId(int newId);
QList<int> returnAllIds();
void removeId(int idToRemove);
} // namespace config
