// Note: these tests require a running backend server
#include <catch2/catch_test_macros.hpp>

#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QUuid>

#include "storage/SecureStorage.hpp"

#include "api/AuthApi.hpp"
#include "api/UsersApi.hpp"

// IDK if making this a single test was a good idea...
// But to me, it seems logical and convenient - we're just fetching the same user with 3 different methods
TEST_CASE("User fetching", "[api][users]")
{
    QEventLoop loop;

    SecureStorage storage;
    storage.saveIdentityKeyPair({ 1, 2, 3, 4 }, { 2, 3, 4, 5 });
    storage.savePreKeyPair({ 4, 3, 2, 1 }, { 5, 4, 3, 2 });
    storage.saveJwtPair({}, {});

    RequestHandler requestHandler;
    QNetworkAccessManager networkAccessManager;
    QNetworkRequestFactory networkRequestFactory;
    networkRequestFactory.setBaseUrl({ "http://localhost:8080" });

    AuthApi auth(storage, requestHandler, networkAccessManager, networkRequestFactory);
    UsersApi users(storage, requestHandler, networkAccessManager, networkRequestFactory);

    QObject::connect(&auth, &AuthApi::userLoggedIn, [&]
    {
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    QObject::connect(&requestHandler, &RequestHandler::errorOccurred, [&](const QString& error)
    {
        qDebug() << "Error occurred: " << error;
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    auth.registerUser(QUuid::createUuid().toString(), "qtTestUserPassword");

    loop.exec();

    bool userFetched{};

    int meId{};
    QString username;

    QObject::connect(&users, &UsersApi::userFetched, [&](const QVariantMap& user)
    {
        userFetched = true;
        qDebug() << "User profile fetched: " << user;

        meId = user["id"].toInt();
        username = user["username"].toString();

        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    users.getMe();
    loop.exec();

    REQUIRE(userFetched);

    userFetched = false;

    users.getUserById(meId);
    loop.exec();

    REQUIRE(userFetched);

    userFetched = false;

    users.getUserByUsername(username);
    loop.exec();

    REQUIRE(userFetched);
}
