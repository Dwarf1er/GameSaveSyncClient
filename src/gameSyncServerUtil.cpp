#include "gameSyncServerUtil.h"
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <qcontainerfwd.h>
#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qlogging.h>

QJsonDocument GameSyncServerUtil::fetchRemoteEndpoint(QString endpoint) {
    QNetworkAccessManager manager;
    QUrl baseUrl = remoteUrl.adjusted(QUrl::StripTrailingSlash);
    baseUrl.setPath(baseUrl.path() + endpoint);
    QNetworkRequest request(baseUrl);
    QNetworkReply* reply = manager.get(request);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "Error fetching game metadata:" << reply->errorString();
        reply->deleteLater();
        return QJsonDocument();
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return QJsonDocument();
    }

    return doc;
}

QJsonDocument GameSyncServerUtil::getGameMetadataList(bool forceFetch) {
    if (forceFetch || gameMetadataList.isEmpty()) {
        gameMetadataList = fetchRemoteEndpoint("/v1/games");
    }
    return gameMetadataList;
}

QJsonObject GameSyncServerUtil::getGameMetadata(int gameID) {
    QJsonDocument document = getGameMetadataList();
    for (const QJsonValue& innerVal : document.array()) {
        QJsonObject object = innerVal.toObject();
        if (gameID == object.value(GameSyncServerUtil::id).toInt()) {
            return object;
        }
    }
    return QJsonObject();
}

QJsonDocument GameSyncServerUtil::getPathByGameId(int id, bool forceFetch) {
    if (forceFetch || !this->gamePathMap.contains(id)) {
        QString endpoint = "/v1/games/" + QString::number(id) + "/paths";
        gamePathMap[id] = fetchRemoteEndpoint(endpoint);
    }
    return gamePathMap.value(id);
}

QJsonDocument GameSyncServerUtil::getExecutableByGameId(int id,
                                                        bool forceFetch) {
    if (forceFetch || !this->gameExecutableMap.contains(id)) {
        QString endpoint = "/v1/games/" + QString::number(id) + "/executables";
        gameExecutableMap[id] = fetchRemoteEndpoint(endpoint);
    }
    return gameExecutableMap.value(id);
}
