#include "detailsViewWidget.h"
#include "pathItemDelegate.h"
#include "utilGameSyncServer.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QVBoxLayout>

DetailsViewWidget::DetailsViewWidget(QWidget* parent) : QWidget(parent) {
    setLayout(new QVBoxLayout());
    gameNameLabel = new QLabel(this);
    pathModel = new PathListModel(this);
    pathList = new QListView(this);
    pathList->setModel(pathModel);
    pathList->setEditTriggers(QAbstractItemView::AllEditTriggers);
    pathList->setItemDelegate(new PathItemDelegate());
    executableList = new QListWidget(this);

    this->layout()->addWidget(gameNameLabel);
    this->layout()->addWidget(pathList);
    this->layout()->addWidget(executableList);
}

void DetailsViewWidget::setGameID(int gameID) {
    this->gameID = gameID;
    refresh();
}

void DetailsViewWidget::refresh() {
    if (gameID == 0) {
        return;
    }

    gameNameLabel->setText(UtilGameSyncServer::getInstance()
                               .getGameMetadata(gameID)
                               .value(UtilGameSyncServer::defaultName)
                               .toString());

    pathModel->loadForGame(gameID);

    QVector<UtilGameSyncServer::ExecutableJson> executablesJson =
        UtilGameSyncServer::getInstance().getExecutableByGameId(gameID);
    executableList->clear();
    for (auto executableJson : executablesJson) {
        executableList->addItem(executableJson.executablePath);
    }
}
