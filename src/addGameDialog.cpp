#include "addGameDialog.h"
#include "gameSyncServerUtil.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <algorithm>
#include <qboxlayout.h>
#include <qcontainerfwd.h>
#include <qjsonobject.h>
#include <qlistwidget.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <tuple>

QJsonDocument getRemoteGameList() {
    return GameSyncServerUtil::getInstance().getGameMetadataList();
}

void addRemoteGameListToSyncList(QJsonDocument doc, QListWidget* list) {
    QJsonArray outerArray = doc.array();
    QList<std::tuple<int, QString>> defaultNames;

    for (const QJsonValue& innerVal : outerArray) {
        QJsonObject object = innerVal.toObject();

        QString defaultName =
            object.value(GameSyncServerUtil::default_name).toString();
        int id = object.value("id").toInt();

        defaultNames.push_back({id, defaultName});
    }

    std::sort(defaultNames.begin(), defaultNames.end(),
              [](const auto& value1, const auto& value2) {
                  return QString::compare(std::get<QString>(value1),
                                          std::get<QString>(value2),
                                          Qt::CaseInsensitive) < 0;
              });

    for (const auto& value : defaultNames) {
        QListWidgetItem* item =
            new QListWidgetItem(std::get<QString>(value), list);
        item->setData(Qt::UserRole, std::get<int>(value));
        list->addItem(item);
    }
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

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    cancelButton = new QPushButton("Cancel", this);
    connect(cancelButton, &QPushButton::clicked, this,
            [this]() { this->reject(); });
    buttonLayout->addWidget(cancelButton);
    addButton = new QPushButton("Add", this);
    connect(addButton, &QPushButton::clicked, this, [this]() {
        if (syncList->selectedItems().length() > 0) {
            this->done(
                syncList->selectedItems().first()->data(Qt::UserRole).toInt());
        }
    });
    buttonLayout->addWidget(addButton);
    layout()->addItem(buttonLayout);
}

AddGameDialog::~AddGameDialog() {}
