#pragma once
#include "utils/RequestHandler.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

// TODO: Rename CryptoStorage to the SecureStorage with an additional functionality of storing access and refresh tokens
// TODO: Add a reference to the global SecureStorage inside the AuthApi class
class AuthApi final : public QObject
{
    Q_OBJECT
public:
    AuthApi(
        RequestHandler& requestHandler,
        QNetworkAccessManager& networkManager,
        QNetworkRequestFactory& requestFactory,
        QObject* parent = nullptr
    );

    Q_INVOKABLE void registerUser(const QString& username, const QString& password, const QString& identityKey, const QString& preKey);
    Q_INVOKABLE void login(const QString& login, const QString& password);

    void refresh();

private:
    RequestHandler& requestHandler;

    QNetworkAccessManager& networkManager;
    QNetworkRequestFactory& requestFactory;
};