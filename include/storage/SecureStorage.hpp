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
    void saveJwtPair(const QString& access, const QString& refresh);

    bool loadIdentityKeyPair(Key& pub, Key& priv);
    bool loadPreKeyPair(Key& pub, Key& priv);
    bool loadPublicIdentityKeyBase64(QString& key);
    bool loadPublicPreKeyBase64(QString& key);
    bool loadJwtAccess(QString& access);
    bool loadJwtRefresh(QString& refresh);

private:
    void writeSecret(const QString& type, const Key& key);
    void writeSecret(const QString& type, const QString& key);

    bool readSecret(const QString& type, Key& key);
    bool readSecret(const QString& type, QString& key);

// Including some type of caching is up for debate...
// It will both improve the performance and introduce a new potential vulnerability
};
