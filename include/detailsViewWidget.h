#pragma once

#include <QLabel>
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
    int gameID = 0;
    QLabel* gameNameLabel;
    QListWidget* pathList;
    QListWidget* executableList;
};
