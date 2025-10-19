#include "backgroundSyncWorker.h"
#include "mainWindow.h"
#include "status.h"
#include "utilGameSyncServer.h"
#include <QApplication>
#include <QThread>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("GameSaveSync");
    QCoreApplication::setApplicationName("GameSaveSyncClient");

    UtilGameSyncServer::getInstance().setServerURL(
        QUrl("http://localhost:3000"));

    app.setQuitOnLastWindowClosed(false);

    auto workerThread = new QThread;
    auto worker = new BackgroundSyncWorker;
    worker->moveToThread(workerThread);

    QObject::connect(workerThread, &QThread::started, worker,
                     &BackgroundSyncWorker::start);

    auto mainWindow = new MainWindow;
    QObject::connect(worker, &BackgroundSyncWorker::errorOccurred, mainWindow,
                     &MainWindow::onErrorOccurred);
    QObject::connect(worker, &BackgroundSyncWorker::pathStatusUpdate,
                     &Status::getInstance(), &Status::setPathStatus);

    workerThread->start();

    mainWindow->show();

    int ret = app.exec();

    worker->deleteLater();
    workerThread->deleteLater();

    return ret;
}
