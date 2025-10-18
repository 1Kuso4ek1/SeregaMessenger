#pragma once
#include <QQmlEngine>

#include "api/Api.hpp"
#include "crypto/CryptoManager.hpp"
#include "storage/SecureStorage.hpp"
#include "utils/Config.hpp"

class SeregaApp final : public QObject
{
    Q_OBJECT
public:
    explicit SeregaApp(QObject *parent = nullptr);

    Q_PROPERTY(Api* api READ getApi CONSTANT)
    Api* getApi() { return &api; }

    Q_PROPERTY(CryptoManager* crypto READ getCrypto CONSTANT)
    CryptoManager* getCrypto() { return &crypto; }

    // Qt is managing a pointer externally
    static SeregaApp* instance(QQmlEngine*, QJSEngine*) { return new SeregaApp; }

private:
    Config config;

    SecureStorage secureStorage;
    CryptoManager crypto;

    Api api;
};
