#include "window.h"
#include "ui_main_window.h"

GameSaveSyncClient::GameSaveSyncClient(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

GameSaveSyncClient::~GameSaveSyncClient()
{
    delete ui;
}
