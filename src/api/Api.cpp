#include "api/Api.hpp"

Api::Api(SecureStorage& secureStorage, QObject* parent)
    : QObject(parent), secureStorage(secureStorage),
      authApi(secureStorage, requestHandler, networkManager, requestFactory, this),
      chatsApi(secureStorage, requestHandler, networkManager, requestFactory, this),
      updatesApi(secureStorage, requestHandler, networkManager, requestFactory, this),
      usersApi(secureStorage, requestHandler, networkManager, requestFactory, this)
{
    // TODO: Make a Config class
    requestFactory.setBaseUrl({ "http://localhost:8080" });
}
