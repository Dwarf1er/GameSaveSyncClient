#include "utilFileSystem.h"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#include <miniz.h>
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"
#include <miniz.h>
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4505)
#include <miniz.h>
#pragma warning(pop)
#else
#include <miniz.h>
#endif

#include <QCryptographicHash>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QStandardPaths>

namespace utilFileSystem {
bool validatePath(QString path) { return !getBasePath(path).isEmpty(); }

QString getBasePath(QString path) {
    if (path.isEmpty()) {
        return {""};
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

    return basePath;
}

std::vector<FileHash> createZip(QString gameId, QString path) { // NOLINT
    QString basePath = getBasePath(path);
    QString pattern = path;
    pattern.remove(0, basePath.length());
    if (pattern.startsWith('/'))
        pattern.remove(0, 1);
    if (pattern.isEmpty())
        pattern = "*";

    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
        "/GameSaveSyncClient";
    QDir(tempDir).mkpath(".");

    QFileInfo baseInfo(basePath);
    QString zipName = gameId + ".zip";
    QString zipPath = QDir(tempDir).filePath(zipName);

    std::vector<FileHash> hashes;

    mz_zip_archive zip;
    memset(&zip, 0, sizeof(zip));
    if (!mz_zip_writer_init_file(&zip, zipPath.toUtf8().constData(), 0)) {
        qWarning() << "Failed to create zip file at" << zipPath;
        return hashes;
    }

    QDirIterator it(basePath, QStringList() << pattern, QDir::Files,
                    QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString filePath = it.next();

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Cannot open file for reading:" << filePath;
            continue;
        }

        QByteArray data = file.readAll();
        file.close();

        QCryptographicHash fileHash(QCryptographicHash::Sha256);
        fileHash.addData(data);
        QByteArray hex = fileHash.result().toHex();

        QString relative = QDir(basePath).relativeFilePath(filePath);
        hashes.push_back(FileHash{.relativePath = relative, .hash = hex});

        QByteArray entryNameUtf8 = relative.toUtf8();
        QByteArray filePathUtf8 = filePath.toUtf8();

        if (!mz_zip_writer_add_file(&zip, entryNameUtf8.constData(),
                                    filePathUtf8.constData(), nullptr, 0,
                                    MZ_BEST_COMPRESSION)) {
            qWarning() << "Failed to add file to zip:" << filePath << "as"
                       << relative;
        }
    }

    if (!mz_zip_writer_finalize_archive(&zip)) {
        qWarning() << "Failed to finalize zip archive:" << zipPath;
    }
    mz_zip_writer_end(&zip);

    return hashes;
}

} // namespace utilFileSystem
