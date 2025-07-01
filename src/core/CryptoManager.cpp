#include "core/CryptoManager.hpp"

#include <QGuiApplication>

#include <sodium.h>

CryptoManager::CryptoManager(QObject* parent)
{
    if(sodium_init() < 0)
    {
        qCritical << "Failed to initialize crypto";
        QGuiApplication::exit(1);
    }

}
