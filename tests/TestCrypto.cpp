#include <catch2/catch_test_macros.hpp>

#include <QCoreApplication>
#include <QDebug>

#include "crypto/CryptoManager.hpp"
#include "storage/SecureStorage.hpp"

TEST_CASE("Crypto storage test", "[crypto]")
{
    SecureStorage storage;

    storage.saveIdentityKeyPair({ 1, 2, 3, 4 }, { 2, 3, 4, 5 });
    storage.savePreKeyPair({ 4, 3, 2, 1 }, { 5, 4, 3, 2 });
    storage.saveJwtPair("accessToken", "refreshToken");

    Key identityPub, identityPriv, preKeyPub, preKeyPriv;
    QString access, refresh;

    REQUIRE(storage.loadIdentityKeyPair(identityPub, identityPriv));
    REQUIRE(storage.loadPreKeyPair(preKeyPub, preKeyPriv));
    REQUIRE(storage.loadJwtAccess(access));
    REQUIRE(storage.loadJwtRefresh(refresh));

    REQUIRE(identityPub[0] == 1);
    REQUIRE(identityPriv[0] == 2);
    REQUIRE(preKeyPub[0] == 4);
    REQUIRE(preKeyPriv[0] == 5);
    REQUIRE(access == "accessToken");
    REQUIRE(refresh == "refreshToken");
}

TEST_CASE("Encryption using preKey", "[crypto]")
{
    SecureStorage storageA;
    SecureStorage storageB;

    storageA.saveIdentityKeyPair({}, {});
    storageA.savePreKeyPair({}, {});
    storageB.saveIdentityKeyPair({}, {});
    storageB.savePreKeyPair({}, {});

    CryptoManager a(storageA);
    CryptoManager b(storageB);

    a.initKeys();
    b.initKeys();

    a.initClientSession(b.getPublicKey(), b.getPublicPreKey());
    b.initServerSession(a.getPublicKey());

    const QString message = "Hello with preKey!";
    const auto encrypted = a.encrypt(message);

    const auto decrypted = b.decrypt(encrypted);

    qDebug() << "Message: " << message << "\nDecrypted: " << decrypted;

    REQUIRE(message == decrypted);
}
