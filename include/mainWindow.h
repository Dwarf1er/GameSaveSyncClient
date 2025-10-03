#pragma once

#include <QAction>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QSplitter>
#include <QTreeView>

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  private:
    QSplitter* mainSplitter;
    QListWidget* syncList;
    QTreeView* detailsView;
    QMenuBar* mainMenuBar;
    QMenu* syncMenu;
    QAction* addGameDialogAction;

  private slots:
    void addGameDialogOpen();
};
