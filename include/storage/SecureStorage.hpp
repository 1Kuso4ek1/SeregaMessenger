#pragma once
#include <QObject>

using Key = std::array<uint8_t, 32>;

class SecureStorage final : public QObject
{
    Q_OBJECT
public:
    explicit SecureStorage(QObject* parent = nullptr) : QObject(parent) {}

    void saveIdentityKeyPair(const Key& pub, const Key& priv);
    void savePreKeyPair(const Key& pub, const Key& priv);

    bool loadIdentityKeyPair(Key& pub, Key& priv);
    bool loadPreKeyPair(Key& pub, Key& priv);

private:
    void writeSecret(const QString& type, const Key& key);
    bool readSecret(const QString& type, Key& key);
};
