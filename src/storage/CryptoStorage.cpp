#include "storage/CryptoStorage.hpp"

#include <QDebug>
#include <QTimer>

#include <sodium.h>
#include <qtkeychain/keychain.h>

void CryptoStorage::saveIdentityKeyPair(const Key& pub, const Key& priv)
{
    writeSecret("identityPub", pub);
    writeSecret("identityPriv", priv);
}

void CryptoStorage::savePreKeyPair(const Key& pub, const Key& priv)
{
    writeSecret("preKeyPub", pub);
    writeSecret("preKeyPriv", priv);
}

bool CryptoStorage::loadIdentityKeyPair(Key& pub, Key& priv)
{
    return readSecret("identityPub", pub)
        && readSecret("identityPriv", priv);
}

bool CryptoStorage::loadPreKeyPair(Key& pub, Key& priv)
{
    return readSecret("preKeyPub", pub)
        && readSecret("preKeyPriv", priv);
}

void CryptoStorage::writeSecret(const QString& type, const Key& key)
{
    QEventLoop loop;

    const auto base64 = QByteArray(reinterpret_cast<const char*>(key.data()), key.size()).toBase64();

    const auto job = new QKeychain::WritePasswordJob("SeregaApp", this);
    job->setKey(type);
    job->setTextData(QString::fromLatin1(base64));

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

bool CryptoStorage::readSecret(const QString& type, Key& key)
{
    QEventLoop loop;

    bool ok = false;

    const auto job = new QKeychain::ReadPasswordJob("SeregaApp", this);
    job->setKey(type);

    connect(job, &QKeychain::Job::finished, &loop, [&ok, &loop, &key](QKeychain::Job* j)
    {
        if(const auto readJob = qobject_cast<QKeychain::ReadPasswordJob*>(j);
            (ok = readJob->error() == QKeychain::NoError))
        {
            const auto array = QByteArray::fromBase64(readJob->textData().toLatin1());
            std::copy_n(array.constData(), array.size(), key.data());
        }
        else
            qCritical() << "Keychain read error:" << readJob->errorString();

        j->deleteLater();

        QTimer::singleShot(0, &loop, &QEventLoop::quit);
    });

    job->start();

    loop.exec();

    return ok;
}
