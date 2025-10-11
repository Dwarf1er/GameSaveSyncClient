#include "mainWindow.h"
#include "addGameDialog.h"
#include "config.h"
#include "detailsViewWidget.h"
#include "gameSyncServerUtil.h"
#include <QAction>
#include <QCloseEvent>
#include <QIcon>
#include <QKeySequence>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QSizePolicy>
#include <QSplitter>
#include <QSystemTrayIcon>
#include <QtLogging>
#include <qapplication.h>
#include <qjsonobject.h>
#include <qlistwidget.h>
#include <qlogging.h>
#include <qnamespace.h>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    mainMenuBar = new QMenuBar(this);

    syncMenu = mainMenuBar->addMenu("&Sync");
    addGameDialogAction = new QAction(
        QIcon::fromTheme(QIcon::ThemeIcon::DocumentNew), "&Add new game", this);
    addGameDialogAction->setShortcut(QKeySequence::New);
    addGameDialogAction->setStatusTip("Add a new game to sync");
    connect(addGameDialogAction, &QAction::triggered, this,
            &MainWindow::addGameDialogOpen);
    syncMenu->addAction(addGameDialogAction);

    setMenuBar(mainMenuBar);

    mainSplitter = new QSplitter(this);
    mainSplitter->setSizePolicy({QSizePolicy::Maximum, QSizePolicy::Maximum});
    syncList = new QListWidget(mainSplitter);
    syncList->setSelectionMode(QAbstractItemView::SingleSelection);
    detailsView = new DetailsViewWidget(mainSplitter);

    setCentralWidget(mainSplitter);
    connect(syncList, &QListWidget::itemSelectionChanged, this, [this]() {
        if (auto item = syncList->currentItem()) {
            const int id = item->data(Qt::UserRole).toInt();
            qDebug() << "Selected id:" << id;
            this->detailsView->setGameID(id);
        }
    });

    trayIcon =
        new QSystemTrayIcon(QIcon::fromTheme("applications-system"), this);
    trayMenu = new QMenu(this);

    showAction = new QAction("Show", this);
    quitAction = new QAction("Quit", this);

    connect(showAction, &QAction::triggered, this, &MainWindow::showWindow);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    trayMenu->addAction(showAction);
    trayMenu->addSeparator();
    trayMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setToolTip("GameSaveSyncClient");
    trayIcon->show();

    refreshFromIDFromConfig();
}

void MainWindow::addGameDialogOpen() {
    AddGameDialog* dialog = new AddGameDialog(this);
    int id = dialog->exec();
    if (id == 0)
        return;
    config::addId(id);

    refreshFromIDFromConfig();
}

void MainWindow::refreshFromIDFromConfig() {
    syncList->clear();
    QList<std::tuple<int, QString>> defaultNames;

    for (auto& id : config::returnAllIds()) {
        QJsonObject gameMetadata =
            GameSyncServerUtil::getInstance().getGameMetadata(id);
        QString defaultName =
            gameMetadata.value(GameSyncServerUtil::defaultName).toString();

        defaultNames.push_back({id, defaultName});
    }

    std::sort(defaultNames.begin(), defaultNames.end(),
              [](const auto& value1, const auto& value2) {
                  return QString::compare(std::get<QString>(value1),
                                          std::get<QString>(value2),
                                          Qt::CaseInsensitive) < 0;
              });

    for (const auto& value : defaultNames) {
        QListWidgetItem* item =
            new QListWidgetItem(std::get<QString>(value), syncList);
        item->setData(Qt::UserRole, std::get<int>(value));
        syncList->addItem(item);
    }

    if (syncList->count() && syncList->selectedItems().empty()) {
        syncList->setCurrentRow(0);
    }
}

void MainWindow::showWindow() {
    this->show();
    this->raise();
    this->activateWindow();
}

void MainWindow::onSyncFinished() { refreshFromIDFromConfig(); }

void MainWindow::onErrorOccurred(QString msg) {
    qWarning() << "Background sync error:" << msg;
}

void MainWindow::closeEvent(QCloseEvent* event) {
    this->hide();
    event->ignore();
}
