// Note: these tests require a running backend server
#include <catch2/catch_test_macros.hpp>

#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QUuid>

#include "storage/SecureStorage.hpp"

#include "api/AuthApi.hpp"
#include "api/ChatsApi.hpp"
#include "api/UpdatesApi.hpp"

TEST_CASE("Updates fetch test", "[api][updates]")
{
    QEventLoop loop, updatesLoop;

    SecureStorage storage;
    storage.saveIdentityKeyPair({ 1, 2, 3, 4 }, { 2, 3, 4, 5 });
    storage.savePreKeyPair({ 4, 3, 2, 1 }, { 5, 4, 3, 2 });
    storage.saveJwtPair({}, {});

    RequestHandler requestHandler;
    QNetworkAccessManager networkAccessManager;
    QNetworkRequestFactory networkRequestFactory;
    networkRequestFactory.setBaseUrl({ "http://localhost:8080" });

    AuthApi auth(storage, requestHandler, networkAccessManager, networkRequestFactory);
    ChatsApi chats(storage, requestHandler, networkAccessManager, networkRequestFactory);
    UpdatesApi updates(storage, requestHandler, networkAccessManager, networkRequestFactory);

    QObject::connect(&auth, &AuthApi::userLoggedIn, [&]
    {
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    QObject::connect(&requestHandler, &RequestHandler::errorOccurred, [&](const QString& error)
    {
        qDebug() << "Error occurred: " << error;
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
        QTimer::singleShot(0, &updatesLoop, &QEventLoop::quit);
    });

    auth.registerUser(QUuid::createUuid().toString(), "qtTestUserPassword");
    loop.exec();

    QObject::connect(&chats, &ChatsApi::messageSent, [&](const auto messageId)
    {
        qDebug() << "Message sent: " << messageId;
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    bool updatesFetched{};
    QObject::connect(&updates, &UpdatesApi::updatesFetched, [&](const QVariantList& newUpdates)
    {
        updatesFetched = true;
        qDebug() << "Messages fetched: " << newUpdates;
        QTimer::singleShot(0, &updatesLoop, &QEventLoop::quit);
    });

    // Starting long polling
    updates.getUpdates();

    chats.sendMessage(1, "Test message");
    loop.exec();

    updatesLoop.exec();

    REQUIRE(updatesFetched);
}
