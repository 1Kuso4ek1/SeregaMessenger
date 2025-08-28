#include "storage/SecureStorage.hpp"

#include <QDebug>
#include <QTimer>

#include <sodium.h>
#include <qtkeychain/keychain.h>

void SecureStorage::saveIdentityKeyPair(const Key& pub, const Key& priv)
{
    writeSecret("identityPub", pub);
    writeSecret("identityPriv", priv);
}

void SecureStorage::savePreKeyPair(const Key& pub, const Key& priv)
{
    writeSecret("preKeyPub", pub);
    writeSecret("preKeyPriv", priv);
}

void SecureStorage::saveJwtPair(const QString& access, const QString& refresh)
{
    writeSecret("access", access);
    writeSecret("refresh", refresh);
}

bool SecureStorage::loadIdentityKeyPair(Key& pub, Key& priv)
{
    return readSecret("identityPub", pub)
        && readSecret("identityPriv", priv);
}

bool SecureStorage::loadPreKeyPair(Key& pub, Key& priv)
{
    return readSecret("preKeyPub", pub)
        && readSecret("preKeyPriv", priv);
}

bool SecureStorage::loadPublicIdentityKeyBase64(QString& key)
{
    return readSecret("identityPub", key);
}

bool SecureStorage::loadPublicPreKeyBase64(QString& key)
{
    return readSecret("preKeyPub", key);
}

bool SecureStorage::loadJwtAccess(QString& access)
{
    return readSecret("access", access);
}

bool SecureStorage::loadJwtRefresh(QString& refresh)
{
    return readSecret("refresh", refresh);
}

void SecureStorage::writeSecret(const QString& type, const QString& key)
{
    QEventLoop loop;

    const auto job = new QKeychain::WritePasswordJob("SeregaApp", this);
    job->setKey(type);
    job->setTextData(key);

    connect(job, &QKeychain::Job::finished, &loop, [&loop](QKeychain::Job* j)
    {
        if(j->error() != QKeychain::NoError)
            qCritical() << "Keychain write error:" << j->errorString();

        j->deleteLater();

        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    job->start();

    loop.exec();
}

void SecureStorage::writeSecret(const QString& type, const Key& key)
{
    const auto base64 = QByteArray(reinterpret_cast<const char*>(key.data()), key.size()).toBase64();

    return writeSecret(type, QString::fromLatin1(base64));
}

bool SecureStorage::readSecret(const QString& type, QString& key)
{
    QEventLoop loop;

    bool ok = false;

    const auto job = new QKeychain::ReadPasswordJob("SeregaApp", this);
    job->setKey(type);

    connect(job, &QKeychain::Job::finished, &loop, [&ok, &loop, &key](QKeychain::Job* j)
    {
        if(const auto readJob = qobject_cast<QKeychain::ReadPasswordJob*>(j);
            (ok = readJob->error() == QKeychain::NoError))
            key = readJob->textData();
        else
            qCritical() << "Keychain read error:" << readJob->errorString();

        j->deleteLater();

        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    job->start();

    loop.exec();

    return ok;
}

bool SecureStorage::readSecret(const QString& type, Key& key)
{
    if(QString out; readSecret(type, out))
    {
        const auto array = QByteArray::fromBase64(out.toLatin1());
        std::copy_n(array.constData(), array.size(), key.data());

        return true;
    }
    return false;
}
