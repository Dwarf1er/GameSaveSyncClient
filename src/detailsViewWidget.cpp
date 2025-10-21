#include "detailsViewWidget.h"
#include "pathItemDelegate.h"
#include "utilGameSyncServer.h"
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

    std::optional<UtilGameSyncServer::GameMetadata> maybeMetadata =
        UtilGameSyncServer::getInstance().getGameMetadata(gameID);
    QString gameName = maybeMetadata ? maybeMetadata->defaultName : QString{};
    gameNameLabel->setText(gameName);

    pathModel->loadForGame(gameID);

    QList<UtilGameSyncServer::ExecutableJson> executablesJson =
        UtilGameSyncServer::getInstance().getExecutableByGameId(gameID);
    executableList->clear();
    for (auto executableJson : executablesJson) {
        executableList->addItem(executableJson.executablePath);
    }
}
