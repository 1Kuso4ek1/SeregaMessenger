// Note: these tests require a running backend server
#include <catch2/catch_test_macros.hpp>

#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QUuid>

#include "storage/SecureStorage.hpp"

#include "api/AuthApi.hpp"

TEST_CASE("User registration", "[api][auth]")
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

    auth.registerUser(QUuid::createUuid().toString(), "qtTestUserPassword");

    loop.exec();

    QString access, refresh;
    storage.loadJwtPair(access, refresh);

    REQUIRE(loggedIn);
    REQUIRE(!access.isEmpty());
    REQUIRE(!refresh.isEmpty());
}

TEST_CASE("User login", "[api][auth]")
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

    QObject::connect(&auth, &AuthApi::userLoggedIn, [&]
        { QTimer::singleShot(0, &loop, &QEventLoop::quit); });

    QObject::connect(&requestHandler, &RequestHandler::errorOccurred, [&]
        { QTimer::singleShot(0, &loop, &QEventLoop::quit); });

    auth.registerUser("testUser", "qtTestUserPassword");

    loop.exec();

    bool loggedIn{};

    QObject::disconnect(&auth, &AuthApi::userLoggedIn, nullptr, nullptr);
    QObject::disconnect(&requestHandler, &RequestHandler::errorOccurred, nullptr, nullptr);

    QObject::connect(&auth, &AuthApi::userLoggedIn, [&]
    {
        qDebug() << "User logged in";
        loggedIn = true;
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    QObject::connect(&requestHandler, &RequestHandler::errorOccurred, [&]
    {
        qDebug() << "Error occurred on login";
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    auth.login("testUser", "qtTestUserPassword");

    loop.exec();

    QString access, refresh;
    storage.loadJwtPair(access, refresh);

    REQUIRE(loggedIn);
    REQUIRE(!access.isEmpty());
    REQUIRE(!refresh.isEmpty());
}

TEST_CASE("Token refresh", "[api][auth]")
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

    QObject::connect(&auth, &AuthApi::userLoggedIn, [&]
        { QTimer::singleShot(0, &loop, &QEventLoop::quit); });

    QObject::connect(&requestHandler, &RequestHandler::errorOccurred, [&]
        { QTimer::singleShot(0, &loop, &QEventLoop::quit); });

    auth.registerUser(QUuid::createUuid().toString(), "qtTestUserPassword");

    loop.exec();

    // QString access, refresh;
    // storage.loadJwtPair(access, refresh);

    bool refreshed{};

    QObject::disconnect(&auth, &AuthApi::userLoggedIn, nullptr, nullptr);
    QObject::disconnect(&requestHandler, &RequestHandler::errorOccurred, nullptr, nullptr);

    QObject::connect(&auth, &AuthApi::userLoggedIn, [&]
    {
        qDebug() << "Tokens refreshed";
        refreshed = true;
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    QObject::connect(&requestHandler, &RequestHandler::errorOccurred, [&]
    {
        qDebug() << "Error occurred on refresh";
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    // Commented out for the faster test execution, because tokens proved to be different
    // QThread::sleep(1); // Wait some time so the new tokens are different

    auth.refresh();

    loop.exec();

    QString newAccess, newRefresh;
    storage.loadJwtPair(newAccess, newRefresh);

    REQUIRE(refreshed);
    REQUIRE(!newAccess.isEmpty());
    REQUIRE(!newRefresh.isEmpty());

    // Not really useful since it's pretty much guaranteed that backend will give you unique tokens
    // if you wait for some time to pass

    // REQUIRE(newAccess != access);
    // REQUIRE(newRefresh != refresh);
}
