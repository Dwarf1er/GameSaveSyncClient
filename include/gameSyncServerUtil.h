#include <QJsonDocument>
#include <QUrl>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qkeysequence.h>
#include <qstringview.h>

class GameSyncServerUtil {
  public:
    static constexpr QStringView default_name = u"default_name";
    static constexpr QStringView id = u"id";

    static GameSyncServerUtil& getInstance() {
        static GameSyncServerUtil instance;
        return instance;
    }
    void setServerURL(QUrl url) { this->remoteUrl = url; }
    QUrl getServerURL() { return this->remoteUrl; }

    QJsonDocument getGameMetadataList(bool forceFetch = false);
    QJsonObject getGameMetadata(int id);

    GameSyncServerUtil(GameSyncServerUtil const&) = delete;
    GameSyncServerUtil& operator=(GameSyncServerUtil const&) = delete;

  protected:
    GameSyncServerUtil() {}
    ~GameSyncServerUtil() {}

  private:
    QUrl remoteUrl;
    QJsonDocument gameMetadataList;
};
