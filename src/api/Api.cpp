#include "api/Api.hpp"

Api::Api(Config& config, SecureStorage& secureStorage, QObject* parent)
    : QObject(parent),
      config(config), secureStorage(secureStorage),
      authApi(secureStorage, requestHandler, networkManager, requestFactory, this),
      chatsApi(secureStorage, requestHandler, networkManager, requestFactory, this),
      updatesApi(secureStorage, requestHandler, networkManager, requestFactory, this),
      usersApi(secureStorage, requestHandler, networkManager, requestFactory, this)
{
    requestFactory.setBaseUrl({ config.getBaseUrl() });
}
