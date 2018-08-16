/**
 * @file linuxguicontroller.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class LinuxGuiController
 *
 * @brief This is the controller class that works a linux GUI environment.
 */

#include "linuxguicontroller.h"

LinuxGuiController::LinuxGuiController(QQuickWindow *view, QObject *parent) : GuiController(view, parent)
{

}

QString LinuxGuiController::takeCaptureArea()
{
    int hasError;
    QStringList arguments;
    QString path(Utils::getTmpDirectory() + "capture_" + Utils::getDateTime() + ".png");

    arguments << path;
    hasError = Utils::executeProgram("import", arguments, QProcess::nullDevice(), QString(), 30000);

    if (hasError) {
        Log::writeError(QString(Q_FUNC_INFO) + " Error taking capture. Code exit of import process: " +
            QString::number(hasError));
    }

    return hasError ? QString() : path;
}

