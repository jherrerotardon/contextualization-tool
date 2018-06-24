#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gui/guicontextualizationcontroller.h"
#include "console/consolecontextualizationcontroller.h"

int main(int argc, char *argv[])
{
    // Console mode.
    //if (argc > 1) { ///< Console Mode. //TODO: decomenar en un futuro. El 2 para que entre con el debbuger
    if (argc > 2) {
        ConsoleContextualizationController consoleController(argc, argv);
        consoleController.exec();

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
    qmlRegisterType<GuiContextualizationController>("io.controllers.guicontroller", 1, 0, "Controller");

    engine.load(QUrl(QStringLiteral("qrc:/views/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
