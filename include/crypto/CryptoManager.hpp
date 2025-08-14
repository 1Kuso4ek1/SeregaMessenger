#pragma once
#include <sodium.h>

#include "storage/CryptoStorage.hpp"

class CryptoManager final : public QObject
{
    Q_OBJECT
public:
    explicit CryptoManager(CryptoStorage& storage, QObject* parent = nullptr);

    void initKeys();
    void initSession(const std::array<uint8_t, crypto_kx_PUBLICKEYBYTES>& peerPublic, bool isServer);

    void save() const;
    bool load();

    std::array<uint8_t, crypto_kx_PUBLICKEYBYTES> getPublicKey() const;
    std::array<uint8_t, crypto_kx_PUBLICKEYBYTES> getPublicPreKey() const;

    std::vector<uint8_t> encrypt(const QString& text) const;
    QString decrypt(const std::vector<uint8_t>& data) const;

private:
    CryptoStorage& storage;

private:
    struct KeyPair { Key publicKey, privateKey; };

    KeyPair identity{}, prekey{};

    std::array<uint8_t, crypto_aead_xchacha20poly1305_ietf_KEYBYTES> sessionKeyRx{}, sessionKeyTx{};
};
