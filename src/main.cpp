/**
 * @file main.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 *
 * @brief This main file to exec program.
 */

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "tools/hpcontextualizationfactory.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    ContextualizationController *controller = HpContextualizationFactory().createController(argv, argc);

    // Console mode.
    if (dynamic_cast<ConsoleController *>(controller)) {
        dynamic_cast<ConsoleController *>(controller)->exec();

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
    if (dynamic_cast<LinuxGuiController *>(controller)) {
        qmlRegisterType<LinuxGuiController>("io.controllers.guicontroller", 1, 0, "Controller");
    } else if (dynamic_cast<WindowsGuiController *>(controller)) {
        qmlRegisterType<WindowsGuiController>("io.controllers.guicontroller", 1, 0, "Controller");
    }

    engine.load(QUrl(QStringLiteral("qrc:/views/ContextualizationGUI.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
