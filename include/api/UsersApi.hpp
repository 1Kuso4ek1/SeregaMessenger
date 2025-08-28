#pragma once
#include "storage/SecureStorage.hpp"
#include "utils/RequestHandler.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

class UsersApi final : public QObject
{
    Q_OBJECT
public:
    UsersApi(
        SecureStorage& secureStorage,
        RequestHandler& requestHandler,
        QNetworkAccessManager& networkManager,
        QNetworkRequestFactory& requestFactory,
        QObject* parent = nullptr
    );

    Q_INVOKABLE void getUserById(int id);
    Q_INVOKABLE void getUserByUsername(const QString& username);
    Q_INVOKABLE void getMe();

signals:
    void userFetched(const QVariantMap& profile);

private:
    void fetchUser(const QString& endpoint);

private:
    SecureStorage& secureStorage;
    RequestHandler& requestHandler;

    QNetworkAccessManager& networkManager;
    QNetworkRequestFactory& requestFactory;
};
