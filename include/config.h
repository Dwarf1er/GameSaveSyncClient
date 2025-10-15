#pragma once

#include <QSettings>

namespace config {
inline QSettings getConfig() { return QSettings(); }
QList<int> returnAllIds();
QString getPath(int pathID);
void addId(int newId);
void removeId(int idToRemove);
void removePath(int pathID);
void updatePath(int pathID, QString path);
} // namespace config
