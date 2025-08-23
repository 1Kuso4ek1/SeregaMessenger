#pragma once
#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>
#include <qqmlintegration.h>

// Will expose different parts of the API through properties
class Api final : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit Api(QObject* parent = nullptr) : QObject(parent) {}

private:
    QNetworkAccessManager networkManager;
    QNetworkRequestFactory requestFactory;
};