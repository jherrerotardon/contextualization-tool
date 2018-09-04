/**
 * @file windowdguicontroller.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class WindowsGuiController
 *
 * @brief This is the controller class that works a GUI Windows environment.
 */

#include "windowsguicontroller.h"

/***********************************************
 *
 * COMMING SOON !!
 *
 **********************************************/

WindowsGuiController::WindowsGuiController(QQuickWindow *view, QObject *parent) : GuiController(view, parent)
{

}

QString WindowsGuiController::takeCaptureArea()
{
    return QString();
}

int WindowsGuiController::generateDoneFpFile()
{
    return 0;
}
