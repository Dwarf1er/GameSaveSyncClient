#include "gameSyncServerUtil.h"
#include <QDebug>
#include <QEventLoop>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <qjsondocument.h>

QJsonDocument GameSyncServerUtil::getGameMetadataList(bool forceFetch) {
    if (forceFetch || gameMetadataList.isEmpty()) {
        QNetworkAccessManager manager;
        QUrl endpoint = remoteUrl.adjusted(QUrl::StripTrailingSlash);
        endpoint.setPath(endpoint.path() + "/v1/games");
        QNetworkRequest request(endpoint);
        QNetworkReply* reply = manager.get(request);

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop,
                         &QEventLoop::quit);
        loop.exec();

        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Error fetching game metadata:"
                       << reply->errorString();
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

        gameMetadataList = doc;
    }
    return gameMetadataList;
}
