#pragma once
#include "AuthApi.hpp"
#include "ChatsApi.hpp"
#include "UpdatesApi.hpp"
#include "UsersApi.hpp"

// Will expose different parts of the API through properties
class Api final : public QObject
{
    Q_OBJECT
public:
    explicit Api(SecureStorage& secureStorage, QObject* parent = nullptr);

    Q_PROPERTY(AuthApi* auth READ getAuth CONSTANT)
    AuthApi* getAuth() { return &authApi; }

    Q_PROPERTY(ChatsApi* chats READ getChats CONSTANT)
    ChatsApi* getChats() { return &chatsApi; }

    Q_PROPERTY(UpdatesApi* updates READ getUpdates CONSTANT)
    UpdatesApi* getUpdates() { return &updatesApi; }

    Q_PROPERTY(UsersApi* users READ getUsers CONSTANT)
    UsersApi* getUsers() { return &usersApi; }

    Q_PROPERTY(RequestHandler* requestHandler READ getRequestHandler CONSTANT)
    RequestHandler* getRequestHandler() { return &requestHandler; }

private: // Shared with SeregaApp
    SecureStorage& secureStorage;

private: // All the API parts
    AuthApi authApi;
    ChatsApi chatsApi;
    UpdatesApi updatesApi;
    UsersApi usersApi;

private: // Shared with API parts
    RequestHandler requestHandler;

    QNetworkAccessManager networkManager;
    QNetworkRequestFactory requestFactory;
};
