#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "core/SeregaApp.hpp"

int main(int argc, char* argv[])
{
    const QGuiApplication app(argc, argv);

    qmlRegisterSingletonType<SeregaApp>("SeregaApp", 1, 0, "SeregaApp", SeregaApp::instance);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        [] { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("SeregaMessenger", "Main");

    return app.exec();
}
