#include "detailsViewWidget.h"
#include "gameSyncServerUtil.h"
#include "pathItemDelegate.h"
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

    gameNameLabel->setText(GameSyncServerUtil::getInstance()
                               .getGameMetadata(gameID)
                               .value(GameSyncServerUtil::defaultName)
                               .toString());

    pathModel->loadForGame(gameID);

    QJsonDocument executableDoc =
        GameSyncServerUtil::getInstance().getExecutableByGameId(gameID);
    executableList->clear();
    if (executableDoc.isArray()) {
        QJsonArray outerArray = executableDoc.array();
        for (const QJsonValue& objVal : outerArray) {
            if (!objVal.isObject()) {
                continue;
            }
            QJsonObject obj = objVal.toObject();
            QString executable = obj.value("executable").toString();
            if (!executable.isEmpty()) {
                executableList->addItem(executable);
            }
        }
    }
}
