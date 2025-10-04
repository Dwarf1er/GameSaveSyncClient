#pragma once

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

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

  private:
    QSplitter* mainSplitter;
    QListWidget* syncList;
    QTreeView* detailsView;
    QMenuBar* mainMenuBar;
    QMenu* syncMenu;
    QAction* addGameDialogAction;

    QSystemTrayIcon* trayIcon;
    QMenu* trayMenu;
    QAction* showAction;
    QAction* quitAction;

    void refreshFromIDFromConfig();

  public slots:
    void showWindow();
    void onSyncFinished();
    void onErrorOccurred(QString);
  private slots:
    void addGameDialogOpen();

  protected:
    void closeEvent(QCloseEvent* event) override; // hide on close
};
