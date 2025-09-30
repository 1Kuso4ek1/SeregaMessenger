#pragma once
#include <sodium.h>

#include "storage/SecureStorage.hpp"

class CryptoManager final : public QObject
{
    Q_OBJECT
public:
    explicit CryptoManager(SecureStorage& storage, QObject* parent = nullptr);

    Q_INVOKABLE void initKeys();
    Q_INVOKABLE void initServerSession(const Key& peerIdentity);
    Q_INVOKABLE void initClientSession(const Key& peerIdentity, const Key& peerPreKey);

    Q_INVOKABLE void save() const;
    Q_INVOKABLE bool load();

    Q_INVOKABLE Key getPublicKey() const;
    Q_INVOKABLE Key getPublicPreKey() const;

    Q_INVOKABLE std::vector<uint8_t> encrypt(const QString& text) const;
    Q_INVOKABLE QString decrypt(const std::vector<uint8_t>& data) const;

private:
    SecureStorage& storage;

private:
    struct KeyPair { Key publicKey, privateKey; };

    KeyPair identity{}, preKey{};

    std::array<uint8_t, crypto_aead_xchacha20poly1305_ietf_KEYBYTES> sessionKeyRx{}, sessionKeyTx{};
};
