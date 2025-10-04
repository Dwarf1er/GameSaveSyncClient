#include <QJsonDocument>
#include <QUrl>
#include <qjsondocument.h>

class GameSyncServerUtil {
  public:
    static GameSyncServerUtil& getInstance() {
        static GameSyncServerUtil instance;
        return instance;
    }
    void setServerURL(QUrl url) { this->remoteUrl = url; }
    QUrl getServerURL() { return this->remoteUrl; }

    QJsonDocument getGameMetadataList(bool forceFetch = false);

    GameSyncServerUtil(GameSyncServerUtil const&) = delete;
    GameSyncServerUtil& operator=(GameSyncServerUtil const&) = delete;

  protected:
    GameSyncServerUtil() {}
    ~GameSyncServerUtil() {}

  private:
    QUrl remoteUrl;
    QJsonDocument gameMetadataList;
};
