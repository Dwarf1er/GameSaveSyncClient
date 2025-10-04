#include "gameSyncServerUtil.h"
#include "mainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    GameSyncServerUtil::getInstance().setServerURL(
        QUrl("http://localhost:3000"));

    // We do some prefetch
    // TODO: Add a progress bar at some point
    GameSyncServerUtil::getInstance().getGameMetadataList();

    MainWindow gameSaveSyncClient;
    gameSaveSyncClient.show();

    app.exec();
}
