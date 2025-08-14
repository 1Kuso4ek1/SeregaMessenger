#include "crypto/CryptoManager.hpp"

#include <QDebug>

CryptoManager::CryptoManager(QObject* parent)
    : QObject(parent)
{
    if(sodium_init() < 0)
        qFatal() << "Failed to initialize crypto";
}

void CryptoManager::initKeys()
{
    crypto_kx_keypair(identity.publicKey.data(), identity.privateKey.data());
    crypto_kx_keypair(prekey.publicKey.data(), prekey.privateKey.data());
}

void CryptoManager::initSession(const std::array<uint8_t, crypto_kx_PUBLICKEYBYTES>& peerPublic, const bool isServer)
{
    std::array<uint8_t, crypto_kx_SESSIONKEYBYTES> rx, tx;

    int ret{};
    if(isServer)
        ret = crypto_kx_server_session_keys(
            rx.data(), tx.data(),
            identity.publicKey.data(), identity.privateKey.data(),
            peerPublic.data()
        );
    else
        ret = crypto_kx_client_session_keys(
            rx.data(), tx.data(),
            identity.publicKey.data(), identity.privateKey.data(),
            peerPublic.data()
        );

    if(ret != 0)
        qCritical() << "Failed to initialize session keys";

    sessionKeyRx = std::move(rx);
    sessionKeyTx = std::move(tx);
}

void CryptoManager::save()
{
    // TODO
}

void CryptoManager::load()
{
    // TODO
}

std::array<uint8_t, 32> CryptoManager::getPublicKey() const
{
    return identity.publicKey;
}

std::array<uint8_t, 32> CryptoManager::getPublicPreKey() const
{
    return prekey.publicKey;
}

std::vector<uint8_t> CryptoManager::encrypt(const QString& text) const
{
    std::array<uint8_t, crypto_aead_xchacha20poly1305_ietf_NPUBBYTES> nonce;
    randombytes_buf(nonce.data(), nonce.size());

    const auto utf8 = text.toUtf8();

    std::vector<uint8_t> encrypted(utf8.size() + crypto_aead_xchacha20poly1305_ietf_ABYTES);

    unsigned long long size;
    crypto_aead_xchacha20poly1305_ietf_encrypt(
        encrypted.data(), &size,
        reinterpret_cast<const uint8_t*>(utf8.constData()), utf8.size(),
        nullptr, 0, nullptr,
        nonce.data(), sessionKeyTx.data()
    );

    encrypted.resize(size);
    encrypted.insert(encrypted.begin(), nonce.begin(), nonce.end());

    return encrypted;
}

QString CryptoManager::decrypt(const std::vector<uint8_t>& data) const
{
    std::array<uint8_t, crypto_aead_xchacha20poly1305_ietf_NPUBBYTES> nonce;
    std::copy_n(data.begin(), nonce.size(), nonce.begin());

    const std::vector plainData(data.begin() + nonce.size(), data.end());

    std::vector<uint8_t> decrypted(plainData.size() - crypto_aead_xchacha20poly1305_ietf_ABYTES);

    unsigned long long size;
    const int res = crypto_aead_xchacha20poly1305_ietf_decrypt(
        decrypted.data(), &size, nullptr,
        plainData.data(), plainData.size(),
        nullptr, 0,
        nonce.data(), sessionKeyRx.data()
    );

    if(res != 0)
    {
        qCritical() << "Failed to decrypt data";
        return {};
    }

    return QString::fromUtf8(reinterpret_cast<const char*>(decrypted.data()), size);
}
