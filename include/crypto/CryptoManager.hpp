#pragma once
#include <sodium.h>

#include "storage/CryptoStorage.hpp"

class CryptoManager final : public QObject
{
    Q_OBJECT
public:
    explicit CryptoManager(CryptoStorage& storage, QObject* parent = nullptr);

    void initKeys();
    void initServerSession(const Key& peerIdentity);
    void initClientSession(const Key& peerIdentity, const Key& peerPreKey);

    void save() const;
    bool load();

    Key getPublicKey() const;
    Key getPublicPreKey() const;

    std::vector<uint8_t> encrypt(const QString& text) const;
    QString decrypt(const std::vector<uint8_t>& data) const;

private:
    CryptoStorage& storage;

private:
    struct KeyPair { Key publicKey, privateKey; };

    KeyPair identity{}, preKey{};

    std::array<uint8_t, crypto_aead_xchacha20poly1305_ietf_KEYBYTES> sessionKeyRx{}, sessionKeyTx{};
};
