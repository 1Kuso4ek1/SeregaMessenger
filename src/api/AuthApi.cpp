#include "api/AuthApi.hpp"

#include <QJsonObject>

AuthApi::AuthApi(
    RequestHandler& requestHandler,
    QNetworkAccessManager& networkManager,
    QNetworkRequestFactory& requestFactory,
    QObject* parent
) : QObject(parent), requestHandler(requestHandler),
    networkManager(networkManager), requestFactory(requestFactory)
{}

void AuthApi::registerUser(
    const QString& username, const QString& password,
    const QString& identityKey, const QString& preKey
)
{
    const auto req = requestFactory.createRequest("/auth/register");

    const QJsonDocument doc{ QJsonObject{
        { "username", username },
        { "password", password },
        { "identityKey", identityKey },
        { "preKey", preKey }
    } };

    requestHandler.executeRequest(
        networkManager.post(req, doc.toJson()),
        [this](const auto& data)
        {

        });
}

void AuthApi::login(const QString& login, const QString& password)
{
}

void AuthApi::refresh()
{
}
