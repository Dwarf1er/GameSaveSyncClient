#pragma once

#include <qlabel.h>
#include <qlistview.h>
#include <qlistwidget.h>
#include <qpushbutton.h>
#include <qwidget.h>

class DetailsViewWidget : public QWidget {
    Q_OBJECT
  public:
    DetailsViewWidget(QWidget* parent = nullptr);
    ~DetailsViewWidget() override = default;
    void setGameID(int id);
    void refresh();

  private:
    int gameID = 0;
    QLabel* gameNameLabel;
    QListWidget* pathList;
    QListWidget* executableList;
};
