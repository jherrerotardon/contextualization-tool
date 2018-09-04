/**
 * @file windowsconsolecontroller.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class WindowsConsoleController
 *
 * @brief This is the controller class that works a Windows CLI environment.
 */

#include "windowsconsolecontroller.h"

/***********************************************
 *
 * COMMING SOON !!
 *
 **********************************************/

WindowsConsoleController::WindowsConsoleController() : ConsoleController()
{

}

WindowsConsoleController::WindowsConsoleController(int argc, char **argv) : ConsoleController(argc, argv)
{

}

QString WindowsConsoleController::takeCaptureArea()
{
    return QString();
}

int WindowsConsoleController::generateDoneFpFile()
{
    return 0;
}
