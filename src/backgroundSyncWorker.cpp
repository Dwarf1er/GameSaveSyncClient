#include "backgroundSyncWorker.h"
#include "config.h"
#include "utilFileSystem.h"
#include "utilGameSyncServer.h"
#include <QJsonArray>
#include <QJsonObject>

constexpr int timerInterval = 30 * 1000;

void BackgroundSyncWorker::start() {
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &BackgroundSyncWorker::update);
    update();
    timer->setInterval(timerInterval);
    timer->start();
}

bool shouldUploadLocalFile(QJsonDocument document) {
    if (document.isEmpty())
        return true;
    // TODO we have other case (comparing remote hash, etc) where we want to
    // upload a save
    return false;
}

void BackgroundSyncWorker::syncGameSaveToServer() {
    QList<int> gameIds = config::returnAllIds();
    for (auto gameId : gameIds) {
        QJsonDocument pathDocument =
            UtilGameSyncServer::getInstance().getPathByGameId(gameId);

        if (pathDocument.isArray()) {
            const QJsonArray array = pathDocument.array();
            for (const QJsonValue& val : array) {
                if (val.isObject()) {
                    QJsonObject obj = val.toObject();
                    int pathId = obj.value("id").toInt();
                    QString configPath = config::getPath(pathId);
                    if (utilFileSystem::validatePath(configPath)) {
                        QJsonDocument document =
                            UtilGameSyncServer::getInstance().getSavesForPathId(
                                pathId);

                        if (shouldUploadLocalFile(document)) {
                            std::vector hashes = utilFileSystem::createZip(
                                QString::number(gameId), configPath);

                            std::optional<QString> uuid =
                                UtilGameSyncServer::getInstance()
                                    .postGameSavesForPathId(pathId, gameId,
                                                            hashes);

                            if (uuid.has_value()) {
                                config::updateUUIDForPath(pathId, uuid.value());
                            }
                        }
                    }
                }
            }
        }
    }
}

void BackgroundSyncWorker::validatePaths() {
    QMap<int, QString> pathStatus;
    QList<int> gameIds = config::returnAllIds();
    for (auto gameId : gameIds) {
        QJsonDocument pathDocument =
            UtilGameSyncServer::getInstance().getPathByGameId(gameId);

        if (pathDocument.isArray()) {
            const QJsonArray array = pathDocument.array();
            for (const QJsonValue& val : array) {
                if (val.isObject()) {
                    QJsonObject obj = val.toObject();
                    int pathId = obj.value("id").toInt();
                    QString configPath = config::getPath(pathId);
                    if (utilFileSystem::validatePath(configPath))
                        pathStatus.insert(pathId, {"Invalid Path"});
                    else
                        pathStatus.insert(pathId, {});
                }
            }
        }
    }
    emit pathStatusUpdate(pathStatus);
}

void BackgroundSyncWorker::update() {
    try {
        validatePaths();
        syncGameSaveToServer();
        emit syncFinished();
    } catch (const std::exception& e) {
        emit errorOccurred(QString::fromStdString(e.what()));
    }
}
