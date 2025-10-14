#include "backgroundSyncWorker.h"
#include "gameSyncServerUtil.h"

constexpr int timerInterval = 30 * 1000;

void BackgroundSyncWorker::start() {
    try {
        GameSyncServerUtil::getInstance().getGameMetadataList();
    } catch (const std::exception& e) {
        emit errorOccurred(QString::fromStdString(e.what()));
    }

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() -> void {
        try {
            GameSyncServerUtil::getInstance().getGameMetadataList();
            emit syncFinished();
        } catch (const std::exception& e) {
            emit errorOccurred(QString::fromStdString(e.what()));
        }
    });
    timer->setInterval(timerInterval);
    timer->start();
}
