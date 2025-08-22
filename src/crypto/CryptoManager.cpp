#include "crypto/CryptoManager.hpp"

#include <QDebug>

CryptoManager::CryptoManager(CryptoStorage& storage, QObject* parent)
    : QObject(parent), storage(storage)
{
    if(sodium_init() < 0)
        qFatal() << "Failed to initialize crypto";
}

void CryptoManager::initKeys()
{
    crypto_kx_keypair(identity.publicKey.data(), identity.privateKey.data());
    crypto_kx_keypair(preKey.publicKey.data(), preKey.privateKey.data());
}

void CryptoManager::initServerSession(const Key& peerIdentity)
{
    std::array<uint8_t, crypto_kx_SESSIONKEYBYTES> rx{}, tx{};

    const int ret = crypto_kx_server_session_keys(
        rx.data(), tx.data(),
        preKey.publicKey.data(), preKey.privateKey.data(),
        peerIdentity.data()
    );

    if(ret != 0)
        qCritical() << "Failed to initialize session keys";

    sessionKeyRx = rx;
    sessionKeyTx = tx;
}

void CryptoManager::initClientSession(const Key& peerIdentity, const Key& peerPreKey)
{
    std::array<uint8_t, crypto_kx_SESSIONKEYBYTES> rx{}, tx{};

    const int ret = crypto_kx_client_session_keys(
        rx.data(), tx.data(),
        identity.publicKey.data(), identity.privateKey.data(),
        peerPreKey.data()
    );

    if(ret != 0)
        qCritical() << "Failed to initialize session keys";

    sessionKeyRx = rx;
    sessionKeyTx = tx;

    // Will be improved with fingerprint saving and verification
    std::array<uint8_t, crypto_hash_sha256_BYTES> fingerprint{};
    crypto_hash_sha256(fingerprint.data(), peerIdentity.data(), peerIdentity.size());

    qDebug() << "Peer identity fingerprint:" << QByteArray(reinterpret_cast<const char*>(fingerprint.data()), fingerprint.size()).toHex();
}

void CryptoManager::save() const
{
    storage.saveIdentityKeyPair(identity.publicKey, identity.privateKey);
    storage.savePreKeyPair(preKey.publicKey, preKey.privateKey);
}

bool CryptoManager::load()
{
    return storage.loadIdentityKeyPair(identity.publicKey, identity.privateKey)
        && storage.loadPreKeyPair(preKey.publicKey, preKey.privateKey);
}

Key CryptoManager::getPublicKey() const
{
    return identity.publicKey;
}

Key CryptoManager::getPublicPreKey() const
{
    return preKey.publicKey;
}

std::vector<uint8_t> CryptoManager::encrypt(const QString& text) const
{
    std::array<uint8_t, crypto_aead_xchacha20poly1305_ietf_NPUBBYTES> nonce{};
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
    std::array<uint8_t, crypto_aead_xchacha20poly1305_ietf_NPUBBYTES> nonce{};
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

    return QString::fromUtf8(reinterpret_cast<const char*>(decrypted.data()), static_cast<qsizetype>(size));
}
