#include "api/Api.hpp"

Api::Api(SecureStorage& secureStorage, QObject* parent)
    : QObject(parent), secureStorage(secureStorage),
      authApi(secureStorage, requestHandler, networkManager, requestFactory, this)
{}
