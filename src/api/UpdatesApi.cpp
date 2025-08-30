#include "api/UpdatesApi.hpp"

#include <QJsonArray>

using namespace Qt::Literals::StringLiterals;

UpdatesApi::UpdatesApi(
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

void UpdatesApi::getUpdates(const int limit, const int offset)
{
    QString access;
    secureStorage.loadJwtAccess(access);

    auto req = requestFactory.createRequest(
        u"/updates?limit=%1&offset=%2"_s
            .arg(limit).arg(offset));
    req.setRawHeader("Authorization", u"Bearer %1"_s.arg(access).toLatin1());
    req.setTransferTimeout(INT32_MAX);

    requestHandler.executeRequest(
        networkManager.get(req),
        [this](const auto& data)
        {
            if(data)
            {
                const auto json = QJsonDocument::fromJson(*data).array();

                emit updatesFetched(json.toVariantList());
            }
            else
            {
                qCritical() << "Failed to fetch messages";

                emit requestHandler.errorOccurred(data.error());
            }
        });
}
