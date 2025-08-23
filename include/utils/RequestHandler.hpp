#pragma once
#include <expected>
#include <QNetworkReply>

class RequestHandler final : public QObject
{
    Q_OBJECT
public:
    explicit RequestHandler(QObject* parent = nullptr) : QObject(parent) {}

    using Callback = std::function<void(const std::expected<QByteArray, QString>&)>;
    void executeRequest(QNetworkReply* reply, const Callback& callback);

signals:
    void errorOccurred(const QString& error);
};
