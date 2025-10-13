#pragma once

#include "detailsViewWidget.h"
#include <QAction>
#include <QCloseEvent>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QSplitter>
#include <QSystemTrayIcon>
#include <QTreeView>
#include <qaction.h>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

  private:
    DetailsViewWidget* detailsView;
    QAction* addGameDialogAction;
    QAction* quitAction;
    QAction* showAction;
    QAction* removeGameFromSyncAction;
    QListWidget* syncList;
    QMenu* syncMenu;
    QMenu* trayMenu;
    QMenuBar* mainMenuBar;
    QSplitter* mainSplitter;
    QSystemTrayIcon* trayIcon;

    void refreshFromIDFromConfig();

  public slots:
    void onErrorOccurred(QString);
    void onSyncFinished();
    void showWindow();
  private slots:
    void addGameDialogOpen();
    void removeGameFromSync();

  protected:
    void closeEvent(QCloseEvent* event) override; // hide on close
};
