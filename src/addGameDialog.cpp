#include "addGameDialog.h"
#include "gameSyncServerUtil.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <qcontainerfwd.h>
#include <qjsonobject.h>
#include <qobject.h>

QJsonDocument getRemoteGameList() {
    return GameSyncServerUtil::getInstance().getGameMetadataList();
}

void addRemoteGameListToSyncList(QJsonDocument doc, QListWidget* list) {
    QJsonArray outerArray = doc.array();
    QStringList defaultNames;

    for (const QJsonValue& innerVal : outerArray) {
        QJsonObject object = innerVal.toObject();

        QString defaultName = object.value("default_name").toString();
        defaultNames.push_back(defaultName);
    }

    defaultNames.sort();
    list->addItems(defaultNames);
}

AddGameDialog::AddGameDialog(QWidget* parent) : QDialog(parent) {
    remoteGameList = getRemoteGameList();
    setMinimumSize({500, 200});

    mainSplitter = new QSplitter(this);
    mainSplitter->setSizePolicy(
        {QSizePolicy::Expanding, QSizePolicy::Expanding});
    syncList = new QListWidget(mainSplitter);
    addRemoteGameListToSyncList(remoteGameList, syncList);
    detailsView = new QTreeView(mainSplitter);

    setLayout(new QVBoxLayout(this));
    layout()->addWidget(mainSplitter);
}

AddGameDialog::~AddGameDialog() {}
