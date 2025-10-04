#include "backgroundSyncWorker.h"
#include "gameSyncServerUtil.h"

void BackgroundSyncWorker::start() {
    try {
        GameSyncServerUtil::getInstance().getGameMetadataList();
    } catch (const std::exception& e) {
        emit errorOccurred(QString::fromStdString(e.what()));
    }

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        try {
            GameSyncServerUtil::getInstance().getGameMetadataList();
            emit syncFinished();
        } catch (const std::exception& e) {
            emit errorOccurred(QString::fromStdString(e.what()));
        }
    });
    timer->setInterval(30 * 1000);
    timer->start();
}
