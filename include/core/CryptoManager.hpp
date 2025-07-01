#pragma once
#include <QObject>

class CryptoManager final : public QObject
{
    Q_OBJECT
public:
    explicit CryptoManager(QObject* parent = nullptr);
};
