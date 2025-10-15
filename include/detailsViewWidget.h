#pragma once

#include "pathListModel.h"
#include <QLabel>
#include <QListView>
#include <QListWidget>
#include <QWidget>

class DetailsViewWidget : public QWidget {
    Q_OBJECT
  public:
    DetailsViewWidget(QWidget* parent = nullptr);
    ~DetailsViewWidget() override = default;
    void setGameID(int id);
    void refresh();

  private:
    PathListModel* pathModel;
    QLabel* gameNameLabel;
    QListView* pathList;
    QListWidget* executableList;
    int gameID = 0;
};
