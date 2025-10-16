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
                    std::optional<QString> error =
                        utilFileSystem::validatePath(configPath);
                    pathStatus.insert(pathId, error.value_or(QString{}));
                }
            }
        }
    }
    emit pathStatusUpdate(pathStatus);
}

void BackgroundSyncWorker::update() {
    try {
        validatePaths();
        emit syncFinished();
    } catch (const std::exception& e) {
        emit errorOccurred(QString::fromStdString(e.what()));
    }
}
