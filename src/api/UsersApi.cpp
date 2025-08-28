#include "api/UsersApi.hpp"

#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

UsersApi::UsersApi(
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

void UsersApi::getUserById(const int id)
{
    fetchUser(u"/users/%1"_s.arg(id));
}

void UsersApi::getUserByUsername(const QString& username)
{
    fetchUser(u"/users/name/%1"_s.arg(username));
}

void UsersApi::getMe()
{
    fetchUser("/users/me");
}

void UsersApi::fetchUser(const QString& endpoint)
{
    QString access;
    secureStorage.loadJwtAccess(access);

    auto req = requestFactory.createRequest(endpoint);
    req.setRawHeader("Authorization", u"Bearer %1"_s.arg(access).toLatin1());

    requestHandler.executeRequest(
        networkManager.get(req),
        [this](const auto& data)
        {
            if(data)
            {
                const auto json = QJsonDocument::fromJson(*data).object();

                emit userFetched(json.toVariantMap());
            }
            else
            {
                qCritical() << "Failed to fetch user";

                emit requestHandler.errorOccurred(data.error());
            }
        });
}
