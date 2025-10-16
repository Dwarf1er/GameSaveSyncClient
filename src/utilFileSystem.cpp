#include "utilFileSystem.h"

#include <QDir>
#include <QDirIterator>
#include <QFileInfo>

namespace utilFileSystem {
std::optional<QString> validatePath(QString path) {
    if (path.isEmpty()) {
        return QString("Path cannot be empty");
    }

    QString normalizedPath = QDir::fromNativeSeparators(path);
    QString basePath = normalizedPath;

    if (basePath.contains('*') || basePath.contains('?')) {
        qsizetype starPos = basePath.indexOf('*');
        qsizetype questionPos = basePath.indexOf('?');
        qsizetype wildcardPos = -1;

        if (starPos != -1 && questionPos != -1) {
            wildcardPos = qMin(starPos, questionPos);
        } else if (starPos != -1) {
            wildcardPos = starPos;
        } else if (questionPos != -1) {
            wildcardPos = questionPos;
        }

        if (wildcardPos != -1) {
            qsizetype lastSeparator = basePath.lastIndexOf('/', wildcardPos);
            if (lastSeparator != -1) {
                basePath = basePath.left(lastSeparator);
            } else {
                basePath = ".";
            }
        }
    }

    while (basePath.endsWith('/') && basePath.length() > 1) {
        basePath.chop(1);
    }

    QFileInfo fileInfo(basePath);
    if (!fileInfo.exists()) {
        return QString("Path does not exist: %1").arg(basePath);
    }
    if (!fileInfo.isReadable()) {
        return QString("Path is not readable: %1").arg(basePath);
    }

    return std::nullopt;
}
} // namespace utilFileSystem
