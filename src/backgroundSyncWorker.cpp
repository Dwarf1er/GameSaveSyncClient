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

bool shouldUploadLocalFile(const QJsonDocument& document) {
    // TODO complete it
    return document.isEmpty();
}

namespace {

void forEachGamePath(
    int gameId,
    std::function<void(int pathId, const QString& configPath)> callback) {
    auto& server = UtilGameSyncServer::getInstance();
    QJsonDocument pathDoc = server.getPathByGameId(gameId);
    if (!pathDoc.isArray())
        return;
    for (const QJsonValue& v : pathDoc.array()) {
        if (!v.isObject())
            continue;
        QJsonObject obj = v.toObject();
        int pathId = obj.value("id").toInt();
        QString configPath = config::getPath(pathId);
        callback(pathId, configPath);
    }
}

} // namespace

void BackgroundSyncWorker::syncGameSaveToServer() {
    for (int gameId : config::returnAllIds()) {
        forEachGamePath(
            gameId, [&](int pathId, const QString& configPath) -> void {
                if (!utilFileSystem::validatePath(configPath))
                    return;

                QJsonDocument doc =
                    UtilGameSyncServer::getInstance().getSavesForPathId(pathId);
                if (!shouldUploadLocalFile(doc))
                    return;

                auto hashes = utilFileSystem::createZip(QString::number(gameId),
                                                        configPath);
                if (auto uuid =
                        UtilGameSyncServer::getInstance()
                            .postGameSavesForPathId(pathId, gameId, hashes);
                    uuid.has_value()) {
                    config::updateUUIDForPath(pathId, uuid.value());
                }
            });
    }
}

void BackgroundSyncWorker::validatePaths() {
    QMap<int, QString> pathStatus;
    for (int gameId : config::returnAllIds()) {
        forEachGamePath(gameId,
                        [&](int pathId, const QString& configPath) -> void {
                            if (!utilFileSystem::validatePath(configPath))
                                pathStatus.insert(pathId, {"Invalid Path"});
                            else
                                pathStatus.insert(pathId, {});
                        });
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
