#pragma once

#include <QString>

namespace utilFileSystem {
struct FileHash {
    QString relativePath;
    QString hash;

    bool operator==(const FileHash& other) const noexcept {
        return relativePath == other.relativePath && hash == other.hash;
    }

    bool operator!=(const FileHash& other) const noexcept {
        return !(*this == other);
    }
};

bool validatePath(const QString path);
QString getBasePath(const QString path);
std::vector<FileHash> getHashFiles(const std::vector<QString>& filePaths,
                                   const QString& basePath);
std::vector<FileHash> createZip(const QString gameId, const QString path);
std::vector<QString> listFiles(const QString basePath, const QString pattern);
QString extractPattern(const QString fullPath);
}; // namespace utilFileSystem
