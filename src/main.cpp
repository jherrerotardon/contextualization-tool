#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tools/hpcontextualizationfactory.h"

int main(int argc, char *argv[])
{
    // Console mode.
    // WARNING!! To debug in Qt creator, argc is greater than 2.
    //if (argc > 1) { ///< Console Mode. //TODO: decomenar en un futuro. El 2 para que entre con el debbuger
    if (argc > 2) {
        ContextualizationController *controller = HpContextualizationFactory().createController(argv, argc);
        static_cast<ConsoleController *>(controller)->exec();

        delete controller;

        return 0;
    }

    // GUI Mode.
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv); // QApplication to use QWidgets like QMessageBox.
    QQmlApplicationEngine engine;

    /**
     * The GuiContextualizationController class is registered as a type, which is accessible from QML by importing
     * the URL, "io.controllers.guicontroller 1.0".
     */
    qmlRegisterType<GuiController>("io.controllers.guicontroller", 1, 0, "Controller");

    engine.load(QUrl(QStringLiteral("qrc:/views/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
