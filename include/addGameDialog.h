#pragma once

#include <QDialog>
#include <QJsonDocument>
#include <QListWidget>
#include <QSplitter>
#include <QTreeView>
#include <qpushbutton.h>

class AddGameDialog : public QDialog {
    Q_OBJECT

  public:
    AddGameDialog(QWidget* parent = nullptr);
    ~AddGameDialog();

  private:
    QSplitter* mainSplitter;
    QListWidget* syncList;
    QTreeView* detailsView;
    QJsonDocument remoteGameList;
    QPushButton* cancelButton;
    QPushButton* addButton;
};
