//#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
/**********************/
#include <QDebug>
/*******************/
#include "contextualizationcontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    //QGuiApplication app(argc, argv);
    QApplication app(argc, argv); //QApplication para que se puedan ejecutar widgets (QMessageBox)

    QQmlApplicationEngine engine;

    engine.load(QUrl(QStringLiteral("qrc:/views/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    ContextualizationController controller(engine.rootObjects().first());

    return app.exec();
}

















