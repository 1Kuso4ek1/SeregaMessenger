#pragma once
#include <QQmlEngine>

#include "crypto/CryptoManager.hpp"
#include "storage/CryptoStorage.hpp"

class SeregaApp final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit SeregaApp(QObject *parent = nullptr);

    static SeregaApp* instance(QQmlEngine*, QJSEngine*) { return new SeregaApp; }

private:
    CryptoStorage cryptoStorage;
    CryptoManager crypto;
};
