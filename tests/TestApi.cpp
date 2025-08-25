// Note: these tests require a running backend server
#include <catch2/catch_test_macros.hpp>

#include <QCoreApplication>
#include <QTimer>
#include <QUuid>

#include "crypto/CryptoManager.hpp"
#include "storage/SecureStorage.hpp"

#include "api/AuthApi.hpp"

TEST_CASE("User registration")
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
    // Every user is unique
    auth.registerUser(QUuid::createUuid().toString(), "qtTestUserPassword");

    bool loggedIn{};

    QObject::connect(&auth, &AuthApi::userLoggedIn, [&]
    {
        qDebug() << "User logged in";
        loggedIn = true;
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    QObject::connect(&requestHandler, &RequestHandler::errorOccurred, [&]
    {
        qDebug() << "Error occurred on user registration";
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    loop.exec();

    REQUIRE(loggedIn);
}
