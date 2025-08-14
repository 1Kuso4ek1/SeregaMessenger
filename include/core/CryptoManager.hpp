#pragma once
#include <QObject>

#include <sodium.h>

class CryptoManager final : public QObject
{
    Q_OBJECT
public:
    explicit CryptoManager(QObject* parent = nullptr);

    void initKeys();
    void initSession(const std::array<uint8_t, crypto_kx_PUBLICKEYBYTES>& peerPublic);

    void save();
    void load();

    std::array<uint8_t, crypto_kx_PUBLICKEYBYTES> getPublicKey() const;
    std::array<uint8_t, crypto_kx_PUBLICKEYBYTES> getPublicPreKey() const;

    std::vector<uint8_t> encrypt(const QString& text);
    QString decrypt(const std::vector<uint8_t>& data);

private:
    struct KeyPair
    {
        std::array<uint8_t, crypto_kx_PUBLICKEYBYTES> publicKey;
        std::array<uint8_t, crypto_kx_SECRETKEYBYTES> privateKey;
    };

    KeyPair identity{}, prekey{};

    std::array<uint8_t, crypto_aead_xchacha20poly1305_ietf_KEYBYTES> sessionKey{};
};
