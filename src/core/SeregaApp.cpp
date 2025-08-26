#include "core/SeregaApp.hpp"

SeregaApp::SeregaApp(QObject* parent)
    : QObject(parent),
      storage(this),
      crypto(storage, this)
{
    if(!crypto.load())
    {
        qDebug() << "No keys found in the keychain, generating new ones";

        crypto.initKeys();
        crypto.save();
    }
}
