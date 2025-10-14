#pragma once

#include <QJsonDocument>
#include <QMap>
#include <QUrl>

class GameSyncServerUtil {
  public:
    static constexpr QStringView defaultName = u"default_name";
    static constexpr QStringView id = u"id";
    static constexpr QStringView windowsOS = u"windows";
    static constexpr QStringView linuxOS = u"linux";
    static constexpr QStringView undefined = u"undefined";

    static GameSyncServerUtil& getInstance() {
        static GameSyncServerUtil instance;
        return instance;
    }
    void setServerURL(QUrl url) { this->remoteUrl = url; }
    QUrl getServerURL() { return this->remoteUrl; }

    QJsonDocument getGameMetadataList(bool forceFetch = false);
    QJsonObject getGameMetadata(int id);
    QJsonDocument getPathByGameId(int id, bool forceFetch = false);
    QJsonDocument getExecutableByGameId(int id, bool forceFetch = false);

    GameSyncServerUtil(GameSyncServerUtil const&) = delete;
    GameSyncServerUtil& operator=(GameSyncServerUtil const&) = delete;

  protected:
    GameSyncServerUtil() = default;
    ~GameSyncServerUtil() = default;

  private:
    QUrl remoteUrl;
    QJsonDocument gameMetadataList;
    QMap<int, QJsonDocument> gamePathMap;
    QMap<int, QJsonDocument> gameExecutableMap;

    QJsonDocument fetchRemoteEndpoint(QString endpoint);
};
