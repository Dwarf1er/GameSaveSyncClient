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
        QList<QString> knownNames;
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
        QList<SaveHash> savesHash;
        int pathId;
        qint64 unixTime;
        QString uuid;
    };

    struct ExecutableJson {
        int id;
        QString executablePath;
        QString operatingSystem;
    };

    static constexpr QStringView windowsOS = u"windows";
    static constexpr QStringView linuxOS = u"linux";
    static constexpr QStringView undefined = u"undefined";

    // Normally one per OS, but on Linux we take the Windows one too
#if defined(Q_OS_WIN)
    QList<QString> listOfAcceptableOs = {QString(windowsOS)};
#elif defined(Q_OS_LINUX)
    QList<QString> listOfAcceptableOs = {QString(windowsOS), QString(linuxOS)};
#endif

    static UtilGameSyncServer& getInstance() {
        static UtilGameSyncServer instance;
        return instance;
    }

    void setServerURL(QUrl url) { this->remoteUrl = url; }
    QUrl getServerURL() { return this->remoteUrl; }

    QList<GameMetadata> getGameMetadataList(bool forceFetch = false);
    std::optional<UtilGameSyncServer::GameMetadata> getGameMetadata(int id);
    std::optional<QList<UtilGameSyncServer::GamePath>>
    getPathByGameId(int gameId, bool forceFetch = false);
    QList<ExecutableJson> getExecutableByGameId(int id,
                                                bool forceFetch = false);
    QList<SaveJson> getSavesForPathId(int id);
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
    QList<GameMetadata> gameMetadataList;
    QMap<int, QList<GamePath>> gamePathMap;
    QMap<int, QList<ExecutableJson>> gameExecutableMap;

    QJsonDocument fetchRemoteEndpoint(QString endpoint);
};
