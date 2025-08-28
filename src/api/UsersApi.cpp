#include "api/UsersApi.hpp"

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

}

void UsersApi::getUserByUsername(const QString& username)
{
}

void UsersApi::getMe()
{
}
