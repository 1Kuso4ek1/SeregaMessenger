#pragma once
#include "storage/SecureStorage.hpp"
#include "utils/RequestHandler.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

class UpdatesApi final : public QObject
{
    Q_OBJECT
public:
    UpdatesApi(
        SecureStorage& secureStorage,
        RequestHandler& requestHandler,
        QNetworkAccessManager& networkManager,
        QNetworkRequestFactory& requestFactory,
        QObject* parent = nullptr
    );

    void getUpdates(int limit = 50, int offset = 0);

signals:
    void updatesFetched(const QVariantList& updates);

private:
    SecureStorage& secureStorage;
    RequestHandler& requestHandler;

    QNetworkAccessManager& networkManager;
    QNetworkRequestFactory& requestFactory;
};
