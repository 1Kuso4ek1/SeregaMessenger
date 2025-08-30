#include "api/ChatsApi.hpp"

#include <QJsonArray>
#include <QJsonObject>

using namespace Qt::Literals::StringLiterals;

ChatsApi::ChatsApi(
    SecureStorage& secureStorage,
    RequestHandler& requestHandler,
    QNetworkAccessManager& networkManager,
    QNetworkRequestFactory& requestFactory,
    QObject* parent
) : QObject(parent),
    secureStorage(secureStorage),
    requestHandler(requestHandler),
    networkManager(networkManager),
    requestFactory(requestFactory)
{}

void ChatsApi::sendMessage(const int chatId, const QString& message)
{
    QString access;
    secureStorage.loadJwtAccess(access);

    auto req = requestFactory.createRequest(u"/chats/%1/messages"_s.arg(chatId));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", u"Bearer %1"_s.arg(access).toLatin1());

    const QJsonDocument doc{ QJsonObject{
        { "content", message }
    } };

    requestHandler.executeRequest(
        networkManager.post(req, doc.toJson()),
        [this](const auto& data)
        {
            if(data)
            {
                const auto json = QJsonDocument::fromJson(*data).object();

                emit messageSent(json["message_id"].toInt());
            }
            else
            {
                qCritical() << "Failed to send message";

                emit requestHandler.errorOccurred(data.error());
            }
        });
}

void ChatsApi::getMessages(const int chatId, const int limit, const int offset)
{
    QString access;
    secureStorage.loadJwtAccess(access);

    auto req = requestFactory.createRequest(
        u"/chats/%1/messages?limit=%2&offset=%3"_s
            .arg(chatId).arg(limit).arg(offset));
    req.setRawHeader("Authorization", u"Bearer %1"_s.arg(access).toLatin1());

    requestHandler.executeRequest(
        networkManager.get(req),
        [this](const auto& data)
        {
            if(data)
            {
                const auto json = QJsonDocument::fromJson(*data).array();

                emit messagesFetched(json.toVariantList());
            }
            else
            {
                qCritical() << "Failed to fetch messages";

                emit requestHandler.errorOccurred(data.error());
            }
        });
}
