#include "api/AuthApi.hpp"

#include <QDebug>
#include <QJsonObject>

AuthApi::AuthApi(
    SecureStorage& secureStorage,
    RequestHandler& requestHandler,
    QNetworkAccessManager& networkManager,
    QNetworkRequestFactory& requestFactory,
    QObject* parent
) : QObject(parent),
    secureStorage(secureStorage),
    requestHandler(requestHandler),
    networkManager(networkManager),
    requestFactory(requestFactory)
{}

void AuthApi::registerUser(const QString& username, const QString& password)
{
    QString identityKey, preKey;
    secureStorage.loadPublicIdentityKeyBase64(identityKey);
    secureStorage.loadPublicPreKeyBase64(preKey);

    const QJsonDocument doc{ QJsonObject{
        { "username", username },
        { "password", password },
        { "identity_key", identityKey },
        { "pre_key", preKey }
    } };

    callJwtEndpoint("/auth/register", doc);
}

void AuthApi::login(const QString& username, const QString& password)
{
    const QJsonDocument doc{ QJsonObject{
        { "username", username },
        { "password", password }
    } };

    callJwtEndpoint("/auth/login", doc);
}

void AuthApi::refresh()
{
    QString access, refresh;
    secureStorage.loadJwtPair(access, refresh);

    const QJsonDocument doc{ QJsonObject{
        { "refresh_token", refresh }
    } };

    callJwtEndpoint("/auth/refresh", doc);
}

void AuthApi::callJwtEndpoint(const QString& endpoint, const QJsonDocument& doc)
{
    auto req = requestFactory.createRequest(endpoint);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    requestHandler.executeRequest(
        networkManager.post(req, doc.toJson()),
        [this](const auto& data)
        {
            if(data)
            {
                const auto json = QJsonDocument::fromJson(*data).object();
                secureStorage.saveJwtPair(json["access_token"].toString(), json["refresh_token"].toString());

                emit userLoggedIn();
            }
            else
            {
                qCritical() << "Failed to refresh";

                emit requestHandler.errorOccurred(data.error());
            }
        });
}
