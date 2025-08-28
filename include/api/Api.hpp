#pragma once
#include "api/AuthApi.hpp"

// Will expose different parts of the API through properties
class Api final : public QObject
{
    Q_OBJECT
public:
    explicit Api(SecureStorage& secureStorage, QObject* parent = nullptr);

private: // Shared with SeregaApp
    SecureStorage& secureStorage;

private: // All the API parts
    AuthApi authApi;

private: // Shared with API parts
    RequestHandler requestHandler;

    QNetworkAccessManager networkManager;
    QNetworkRequestFactory requestFactory;
};
