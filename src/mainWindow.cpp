#include "mainWindow.h"
#include "addGameDialog.h"
#include <QAction>
#include <QIcon>
#include <QKeySequence>
#include <QListWidget>
#include <QMenuBar>
#include <QSizePolicy>
#include <QSplitter>
#include <QTreeView>
#include <QtLogging>

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
    detailsView = new QTreeView(mainSplitter);

    setCentralWidget(mainSplitter);
}

void MainWindow::addGameDialogOpen() {
    AddGameDialog dialog = new AddGameDialog(this);
    dialog.exec();
}

MainWindow::~MainWindow() {}
