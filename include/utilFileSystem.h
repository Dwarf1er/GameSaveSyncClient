#pragma once

#include <QString>

namespace utilFileSystem {
struct FileHash {
    QString relativePath;
    QString hash;
};

bool validatePath(QString path);
QString getBasePath(QString path);
std::vector<FileHash> createZip(QString gameId, QString path);
std::vector<QString> listFiles(QString basePath, QString pattern);
QString extractPattern(QString basePath, QString fullPath);
}; // namespace utilFileSystem
