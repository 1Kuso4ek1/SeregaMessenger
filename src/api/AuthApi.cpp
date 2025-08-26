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
    auto req = requestFactory.createRequest("/auth/register");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QString identityKey, preKey;
    secureStorage.loadPublicIdentityKeyBase64(identityKey);
    secureStorage.loadPublicPreKeyBase64(preKey);

    const QJsonDocument doc{ QJsonObject{
        { "username", username },
        { "password", password },
        { "identity_key", identityKey },
        { "pre_key", preKey }
    } };

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
                qCritical() << "Failed to register user";

                emit requestHandler.errorOccurred(data.error());
            }
        });
}

void AuthApi::login(const QString& username, const QString& password)
{
    auto req = requestFactory.createRequest("/auth/login");
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    const QJsonDocument doc{ QJsonObject{
        { "username", username },
        { "password", password }
    } };

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
                qCritical() << "Failed to login";

                emit requestHandler.errorOccurred(data.error());
            }
        });
}

void AuthApi::refresh()
{
}
