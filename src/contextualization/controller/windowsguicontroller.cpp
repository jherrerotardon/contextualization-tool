#include "windowsguicontroller.h"

WindowsGuiController::WindowsGuiController(QQuickWindow *view, QObject *parent) : GuiController(view, parent)
{

}

QString WindowsGuiController::takeCaptureArea()
{
    int hasError;
    QStringList arguments;
    QString path("/tmp/capture.png");

    arguments << path;
    hasError = Utils::executeProgram("import", arguments, QProcess::nullDevice(), QString(), 30000);

    if (hasError) {
        Log::writeError("Error taking capture. Code exit of import process: " + hasError);
    }

    return hasError ? QString() : path;
}
