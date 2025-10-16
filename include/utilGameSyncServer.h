#pragma once

#include <QJsonDocument>
#include <QMap>
#include <QUrl>

class UtilGameSyncServer {
  public:
    static constexpr QStringView defaultName = u"default_name";
    static constexpr QStringView id = u"id";
    static constexpr QStringView windowsOS = u"windows";
    static constexpr QStringView linuxOS = u"linux";
    static constexpr QStringView undefined = u"undefined";

    static UtilGameSyncServer& getInstance() {
        static UtilGameSyncServer instance;
        return instance;
    }
    void setServerURL(QUrl url) { this->remoteUrl = url; }
    QUrl getServerURL() { return this->remoteUrl; }

    QJsonDocument getGameMetadataList(bool forceFetch = false);
    QJsonObject getGameMetadata(int id);
    QJsonDocument getPathByGameId(int id, bool forceFetch = false);
    QJsonDocument getExecutableByGameId(int id, bool forceFetch = false);

    UtilGameSyncServer(UtilGameSyncServer const&) = delete;
    UtilGameSyncServer& operator=(UtilGameSyncServer const&) = delete;

  protected:
    UtilGameSyncServer() = default;
    ~UtilGameSyncServer() = default;

  private:
    QUrl remoteUrl;
    QJsonDocument gameMetadataList;
    QMap<int, QJsonDocument> gamePathMap;
    QMap<int, QJsonDocument> gameExecutableMap;

    QJsonDocument fetchRemoteEndpoint(QString endpoint);
};
