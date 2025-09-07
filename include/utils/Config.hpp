#pragma once
#include <QDebug>
#include <QFile>
#include <QJsonValue>

class Config final : public QObject
{
    Q_OBJECT
public:
    explicit Config(const QString& filePath, QObject* parent = nullptr)
        : QObject(parent)
    {
        QFile file(filePath);
        if(!file.open(QFile::ReadOnly | QFile::Text))
            qFatal() << "Could not open config file: " << filePath;

        const auto data = file.readAll();
        file.close();

        const auto json = QJsonDocument::fromJson(data);
        if(json.isNull() || !json.isObject())
            qFatal() << "Invalid config file: " << filePath;

        baseUrl = json["base_url"].toString();
    }

    QString getBaseUrl() const { return baseUrl; }

private:
    QString baseUrl;
};
