#pragma once
#include "storage/SecureStorage.hpp"
#include "utils/RequestHandler.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>


// TODO: Add a reference to the global SecureStorage inside the AuthApi class
class AuthApi final : public QObject
{
    Q_OBJECT
public:
    AuthApi(
        SecureStorage& secureStorage,
        RequestHandler& requestHandler,
        QNetworkAccessManager& networkManager,
        QNetworkRequestFactory& requestFactory,
        QObject* parent = nullptr
    );

    Q_INVOKABLE void registerUser(const QString& username, const QString& password);
    Q_INVOKABLE void login(const QString& username, const QString& password);

    void refresh();

signals:
    void userLoggedIn();

private:
    SecureStorage& secureStorage;
    RequestHandler& requestHandler;

    QNetworkAccessManager& networkManager;
    QNetworkRequestFactory& requestFactory;
};