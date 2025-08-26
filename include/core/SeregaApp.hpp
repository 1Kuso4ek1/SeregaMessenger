#pragma once
#include <QQmlEngine>

#include "api/Api.hpp"
#include "crypto/CryptoManager.hpp"
#include "storage/SecureStorage.hpp"

class SeregaApp final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit SeregaApp(QObject *parent = nullptr);

    // Pointer is being managed externally by Qt
    static SeregaApp* instance(QQmlEngine*, QJSEngine*) { return new SeregaApp; }

private:
    SecureStorage storage;
    CryptoManager crypto;

    Api api;
};
