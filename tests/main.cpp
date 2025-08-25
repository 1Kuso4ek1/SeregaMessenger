#define CATCH_CONFIG_RUNNER

#include <QCoreApplication>
#include <QTimer>

#include <catch2/catch_all.hpp>

int main(int argc, char** argv)
{
    const QCoreApplication app(argc, argv);

    QTimer::singleShot(0, [&] {
        app.exit(Catch::Session().run(argc, argv));
    });

    return app.exec();
}
