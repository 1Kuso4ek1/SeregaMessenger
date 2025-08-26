#include "core/SeregaApp.hpp"

SeregaApp::SeregaApp(QObject* parent)
    : QObject(parent),
      secureStorage(this),
      crypto(secureStorage, this),
      api(secureStorage)
{
    if(!crypto.load())
    {
        qDebug() << "No keys found in the keychain, generating new ones";

        crypto.initKeys();
        crypto.save();
    }
}
