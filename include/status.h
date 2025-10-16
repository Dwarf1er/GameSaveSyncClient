#include <QMap>
#include <QObject>
#include <QString>

class Status : public QObject {
    Q_OBJECT
  public:
    static Status& getInstance() {
        static Status instance;
        return instance;
    }

    Status(Status const&) = delete;
    Status& operator=(Status const&) = delete;

    QString getPathStatusById(int id) {
        return pathStatus.value(id, QString("path not found in path status"));
    }

  public slots:
    void setPathStatus(QMap<int, QString> pathStatus) {
        this->pathStatus = pathStatus;
    }

  protected:
    Status() : QObject() {}
    ~Status() override = default;

  private:
    QMap<int, QString> pathStatus;
};
