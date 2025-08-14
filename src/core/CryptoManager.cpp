#include "core/CryptoManager.hpp"

#include <QDebug>

CryptoManager::CryptoManager(QObject* parent)
    : QObject(parent)
{
    if(sodium_init() < 0)
        qCritical() << "Failed to initialize crypto";
}

void CryptoManager::initKeys()
{
    crypto_kx_keypair(identity.publicKey.data(), identity.privateKey.data());
    crypto_kx_keypair(prekey.publicKey.data(), prekey.privateKey.data());
}

void CryptoManager::initSession(const std::array<uint8_t, 32>& peerPublic)
{
}

void CryptoManager::save()
{
}

void CryptoManager::load()
{
}

std::array<uint8_t, 32> CryptoManager::getPublicKey() const
{
    return identity.publicKey;
}

std::array<uint8_t, 32> CryptoManager::getPublicPreKey() const
{
    return prekey.publicKey;
}

std::vector<uint8_t> CryptoManager::encrypt(const QString& text)
{
}

QString CryptoManager::decrypt(const std::vector<uint8_t>& data)
{
}
