#pragma once

#include <QObject>
#include <QTimer>

class BackgroundSyncWorker : public QObject {
    Q_OBJECT

  public:
    BackgroundSyncWorker(QObject* parent = nullptr) : QObject(parent) {}

  public slots:
    void start();

  signals:
    void syncFinished();
    void errorOccurred(QString);
};
