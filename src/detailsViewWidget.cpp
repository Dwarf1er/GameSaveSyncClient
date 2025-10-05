#include "detailsViewWidget.h"
#include "gameSyncServerUtil.h"
#include <qboxlayout.h>
#include <qjsondocument.h>
#include <qlabel.h>
#include <qlistwidget.h>
#include <qwidget.h>

DetailsViewWidget::DetailsViewWidget(QWidget* parent) : QWidget(parent) {
    setLayout(new QVBoxLayout());
    gameNameLabel = new QLabel(this);
    pathList = new QListWidget(this);
    executableList = new QListWidget(this);

    this->layout()->addWidget(gameNameLabel);
    this->layout()->addWidget(pathList);
    this->layout()->addWidget(executableList);
}

void DetailsViewWidget::setGameID(int id) {
    gameID = id;
    refresh();
}

void DetailsViewWidget::refresh() {
    if (gameID == 0)
        return;

    gameNameLabel->setText(GameSyncServerUtil::getInstance()
                               .getGameMetadata(gameID)
                               .value(GameSyncServerUtil::default_name)
                               .toString());

    QJsonDocument pathDoc =
        GameSyncServerUtil::getInstance().getPathByGameId(gameID);
    pathList->clear();
    if (pathDoc.isArray()) {
        QJsonArray outerArray = pathDoc.array();
        for (const QJsonValue& objVal : outerArray) {
            if (!objVal.isObject()) {
                continue;
            }
            QJsonObject obj = objVal.toObject();
            QString path = obj.value("path").toString();
            if (!path.isEmpty()) {
                pathList->addItem(path);
            }
        }
    }

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
