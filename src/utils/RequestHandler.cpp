#include "utils/RequestHandler.hpp"

void RequestHandler::executeRequest(QNetworkReply* reply, const Callback& callback)
{
    connect(reply, &QNetworkReply::finished, this, [reply, callback]
    {
        reply->deleteLater();

        if(reply->error() != QNetworkReply::NoError)
        {
            callback(std::unexpected(reply->errorString()));
            return;
        }

        callback(reply->readAll());
    });
}
