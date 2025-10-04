#include "backgroundSyncWorker.h"
#include "gameSyncServerUtil.h"
#include "mainWindow.h"
#include <QApplication>
#include <QThread>
#include <qurl.h>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    GameSyncServerUtil::getInstance().setServerURL(
        QUrl("http://localhost:3000"));

    // Don't quit automatically when the last window is closed
    app.setQuitOnLastWindowClosed(false);

    QThread* workerThread = new QThread;
    BackgroundSyncWorker* worker = new BackgroundSyncWorker;
    worker->moveToThread(workerThread);

    QObject::connect(workerThread, &QThread::started, worker,
                     &BackgroundSyncWorker::start);

    // Optional: connect worker signals to UI updates
    MainWindow* mw = new MainWindow;
    QObject::connect(worker, &BackgroundSyncWorker::syncFinished, mw,
                     &MainWindow::onSyncFinished);
    QObject::connect(worker, &BackgroundSyncWorker::errorOccurred, mw,
                     &MainWindow::onErrorOccurred);

    workerThread->start();

    mw->show();

    int ret = app.exec();

    worker->deleteLater();
    workerThread->deleteLater();

    return ret;
}
