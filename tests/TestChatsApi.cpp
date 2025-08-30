// Note: these tests require a running backend server
#include <catch2/catch_test_macros.hpp>

#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QUuid>

#include "storage/SecureStorage.hpp"

#include "api/AuthApi.hpp"
#include "api/ChatsApi.hpp"
#include "api/UsersApi.hpp"

TEST_CASE("Message sending and fetching", "[api][chats]")
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
    ChatsApi chats(storage, requestHandler, networkAccessManager, networkRequestFactory);

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

    bool messageSent{}, messagesFetched{};
    QObject::connect(&chats, &ChatsApi::messageSent, [&](const auto messageId)
    {
        messageSent = true;
        qDebug() << "Message sent: " << messageId;
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    QObject::connect(&chats, &ChatsApi::messagesFetched, [&](const QVariantList& messages)
    {
        messagesFetched = true;
        qDebug() << "Messages fetched: " << messages;
        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    // We don't really care about encryption
    chats.sendMessage(1, "Test message");
    loop.exec();

    REQUIRE(messageSent);

    chats.fetchMessages(1);
    loop.exec();

    REQUIRE(messagesFetched);
}
