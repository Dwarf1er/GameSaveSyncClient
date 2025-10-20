#include "utilGameSyncServer.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QEventLoop>
#include <QFile>
#include <QHttpMultiPart>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QStandardPaths>
#include <QThread>
#include <QTimer>

QJsonDocument UtilGameSyncServer::fetchRemoteEndpoint(QString endpoint) {
    QNetworkAccessManager manager;
    QUrl baseUrl = remoteUrl.adjusted(QUrl::StripTrailingSlash);
    baseUrl.setPath(baseUrl.path() + endpoint);
    QNetworkRequest request(baseUrl);
    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Error fetching endpoint:" << reply->errorString();
        reply->deleteLater();
        return {};
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return {};
    }

    return doc;
}

QJsonDocument UtilGameSyncServer::getGameMetadataList(bool forceFetch) {
    if (forceFetch || gameMetadataList.isEmpty()) {
        gameMetadataList = fetchRemoteEndpoint("/v1/games");
    }
    return gameMetadataList;
}

QJsonObject UtilGameSyncServer::getGameMetadata(int gameID) {
    QJsonDocument document = getGameMetadataList();
    for (const QJsonValue& innerVal : document.array()) {
        QJsonObject object = innerVal.toObject();
        if (gameID == object.value(UtilGameSyncServer::id).toInt()) {
            return object;
        }
    }
    return {};
}

QJsonDocument UtilGameSyncServer::getPathByGameId(int id, bool forceFetch) {
    if (forceFetch || !this->gamePathMap.contains(id)) {
        QString endpoint = "/v1/games/" + QString::number(id) + "/paths";
        gamePathMap[id] = fetchRemoteEndpoint(endpoint);
    }
    return gamePathMap.value(id);
}

QJsonDocument UtilGameSyncServer::getExecutableByGameId(int id,
                                                        bool forceFetch) {
    if (forceFetch || !this->gameExecutableMap.contains(id)) {
        QString endpoint = "/v1/games/" + QString::number(id) + "/executables";
        gameExecutableMap[id] = fetchRemoteEndpoint(endpoint);
    }
    return gameExecutableMap.value(id);
}

QJsonDocument UtilGameSyncServer::getSavesForPathId(int id) {
    QString endpoint = "/v1/paths/" + QString::number(id) + "/saves";
    return fetchRemoteEndpoint(endpoint);
}

std::optional<QString> UtilGameSyncServer::postGameSavesForPathId(
    int pathId, int gameId, // NOLINT
    std::vector<utilFileSystem::FileHash> hashOfContent) {
    QString endpoint = "/v1/paths/" + QString::number(pathId) + "/saves/upload";
    QUrl url = remoteUrl.adjusted(QUrl::StripTrailingSlash);
    url.setPath(url.path() + endpoint);

    QJsonArray hashArray;
    for (const auto& fileHash : hashOfContent) {
        QJsonObject hashObj;
        hashObj["relative_path"] = fileHash.relativePath;
        hashObj["hash"] = fileHash.hash;
        hashArray.append(hashObj);
    }
    QJsonDocument hashDoc(hashArray);

    QString tempDir =
        QStandardPaths::writableLocation(QStandardPaths::TempLocation) +
        "/GameSaveSyncClient";
    QString zipPath = QDir(tempDir).filePath(QString::number(gameId) + ".zip");
    qDebug() << "Zip path:" << zipPath;

    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart httpPartHashArray;
    httpPartHashArray.setHeader(QNetworkRequest::ContentDispositionHeader,
                                QVariant("form-data; name=\"file_hash\""));
    httpPartHashArray.setBody(
        hashDoc.toJson(QJsonDocument::JsonFormat::Compact));

    QHttpPart httpPartFile;
    httpPartFile.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"file\""));
    auto file = new QFile(zipPath);
    if (!file->open(QIODevice::ReadOnly)) {
        qWarning() << "Opening file failed";
        return std::nullopt;
    }
    httpPartFile.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(httpPartHashArray);
    multiPart->append(httpPartFile);

    QNetworkRequest request(url);
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.post(request, multiPart);
    multiPart->setParent(reply);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool success = (reply->error() == QNetworkReply::NoError);
    if (!success) {
        qWarning() << "Upload failed:" << reply->errorString();
        return std::nullopt;
    }

    QString uuid = QString::fromUtf8(reply->readAll());

    reply->deleteLater();

    return uuid;
}
