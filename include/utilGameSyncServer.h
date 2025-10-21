#pragma once

#include "utilFileSystem.h"
#include <QJsonDocument>
#include <QMap>
#include <QNetworkAccessManager>
#include <QUrl>

class UtilGameSyncServer {
  public:
    struct GameMetadata {
        int id;
        QString defaultName;
        QString steamAppId;
        QVector<QString> knownNames;
    };

    struct GamePath {
        int id;
        QString operatingSystem;
        QString path;
    };

    struct SaveHash {
        QString hash;
        QString relativePath;
    };

    struct SaveJson {
        QVector<SaveHash> savesHash;
        int pathId;
        qint64 unixTime;
        QString uuid;
    };

    struct ExecutableJson {
        QString executablePath;
    };

    static constexpr QStringView windowsOS = u"windows";
    static constexpr QStringView linuxOS = u"linux";
    static constexpr QStringView undefined = u"undefined";

    static UtilGameSyncServer& getInstance() {
        static UtilGameSyncServer instance;
        return instance;
    }

    void setServerURL(QUrl url) { this->remoteUrl = url; }
    QUrl getServerURL() { return this->remoteUrl; }

    QVector<GameMetadata> getGameMetadataList(bool forceFetch = false);
    std::optional<UtilGameSyncServer::GameMetadata> getGameMetadata(int id);
    std::optional<QVector<UtilGameSyncServer::GamePath>>
    getPathByGameId(int gameId, bool forceFetch = false);
    QVector<ExecutableJson> getExecutableByGameId(int id,
                                                  bool forceFetch = false);
    QVector<SaveJson> getSavesForPathId(int id);
    std::optional<QString>
    postGameSavesForPathId(int pathId, int gameId,
                           std::vector<utilFileSystem::FileHash> hashOfContent);

    UtilGameSyncServer(UtilGameSyncServer const&) = delete;
    UtilGameSyncServer& operator=(UtilGameSyncServer const&) = delete;

  protected:
    UtilGameSyncServer() = default;
    ~UtilGameSyncServer() = default;

  private:
    QUrl remoteUrl;
    QVector<GameMetadata> gameMetadataList;
    QMap<int, QVector<GamePath>> gamePathMap;
    QMap<int, QVector<ExecutableJson>> gameExecutableMap;

    QJsonDocument fetchRemoteEndpoint(QString endpoint);
};
