#include <catch2/catch_test_macros.hpp>

#include <QCoreApplication>
#include <QDebug>

#include "crypto/CryptoManager.hpp"
#include "storage/CryptoStorage.hpp"

TEST_CASE("Basic encryption and decryption")
{
    int argc{};
    QCoreApplication app(argc, nullptr);

    CryptoStorage storage;
    storage.saveIdentityKeyPair({}, {});
    storage.savePreKeyPair({}, {});

    CryptoManager a(storage), b(storage);

    a.initKeys();
    b.initKeys();

    a.initSession(b.getPublicKey(), true);
    b.initSession(a.getPublicKey(), false);

    const QString message = "Hello world!";

    const auto encrypted = a.encrypt(message);
    const auto decrypted = b.decrypt(encrypted);

    qDebug() << "Message: " << message << "\nDecrypted: " << decrypted;

    REQUIRE(message == decrypted);
}

TEST_CASE("Crypto storage test")
{
    int argc{};
    QCoreApplication app(argc, nullptr);

    CryptoStorage storage;

    storage.saveIdentityKeyPair({ 1, 2, 3, 4 }, { 2, 3, 4, 5 });
    storage.savePreKeyPair({ 4, 3, 2, 1 }, { 5, 4, 3, 2 });

    Key identityPub, identityPriv, preKeyPub, preKeyPriv;

    REQUIRE(storage.loadIdentityKeyPair(identityPub, identityPriv));
    REQUIRE(storage.loadPreKeyPair(preKeyPub, preKeyPriv));

    REQUIRE(identityPub[0] == 1);
    REQUIRE(identityPriv[0] == 2);
    REQUIRE(preKeyPub[0] == 4);
    REQUIRE(preKeyPriv[0] == 5);
}
