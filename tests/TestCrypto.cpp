#include <catch2/catch_test_macros.hpp>

#include <QDebug>

#include "core/CryptoManager.hpp"

TEST_CASE("Basic encryption and decryption")
{
    CryptoManager a, b;

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
