#include "window.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    GameSaveSyncClient gameSaveSyncClient;
    gameSaveSyncClient.show();

    app.exec();
}
