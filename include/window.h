#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class GameSaveSyncClient : public QMainWindow
{
    Q_OBJECT

public:
    GameSaveSyncClient(QWidget *parent = nullptr);
    ~GameSaveSyncClient();

private:
    Ui::MainWindow *ui;
};
