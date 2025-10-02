#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>

class GameSaveSyncClient : public QMainWindow
{
    Q_OBJECT

    int32_t mButtonPressedAmt;

    QLabel* mpLabel;
    QPushButton* mpPushButton;

    void updateLabelText();

public:
    GameSaveSyncClient(QWidget* parent = nullptr);
    ~GameSaveSyncClient() = default;

signals:
    void sigLabelTextUpdated(std::string_view);
};
