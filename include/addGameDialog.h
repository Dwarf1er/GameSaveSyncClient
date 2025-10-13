#pragma once

#include <QDialog>
#include <QJsonDocument>
#include <QListWidget>
#include <QSplitter>
#include <QTreeView>

class AddGameDialog : public QDialog {
    Q_OBJECT

  public:
    AddGameDialog(QWidget* parent = nullptr);
    ~AddGameDialog();

  private:
    QListWidget* syncList;
    QJsonDocument remoteGameList;
    QPushButton* cancelButton;
    QPushButton* addButton;
};
