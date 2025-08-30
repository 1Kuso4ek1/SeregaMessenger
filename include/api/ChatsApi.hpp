#pragma once
#include "storage/SecureStorage.hpp"
#include "utils/RequestHandler.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequestFactory>

class ChatsApi final : public QObject
{
    Q_OBJECT
public:
    ChatsApi(
        SecureStorage& secureStorage,
        RequestHandler& requestHandler,
        QNetworkAccessManager& networkManager,
        QNetworkRequestFactory& requestFactory,
        QObject* parent = nullptr
    );

    Q_INVOKABLE void sendMessage(int chatId, const QString& message);
    Q_INVOKABLE void fetchMessages(int chatId, int limit = 50, int offset = 0);

signals:
    void messageSent(uint64_t messageId);
    void messagesFetched(const QVariantList& messages);

private:
    SecureStorage& secureStorage;
    RequestHandler& requestHandler;

    QNetworkAccessManager& networkManager;
    QNetworkRequestFactory& requestFactory;
};
