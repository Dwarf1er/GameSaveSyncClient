#pragma once

#include <QSettings>

namespace config {
inline QSettings getConfig() { return QSettings(); }
void addId(int newId);
QList<int> returnAllIds();
void removeId(int idToRemove);
} // namespace config
