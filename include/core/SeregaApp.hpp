#pragma once
#include <QQmlEngine>

#include "crypto/CryptoManager.hpp"

class SeregaApp final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit SeregaApp(QObject *parent = nullptr) : QObject(parent) {}

    static SeregaApp* instance(QQmlEngine*, QJSEngine*) { return new SeregaApp; }

private:
    CryptoManager crypto{};
};
