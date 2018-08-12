/**
 * @file linuxconsolecontroller.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class LinuxConsoleController
 *
 * @brief This is the controller class that works a linux CLI environment.
 */

#include "linuxconsolecontroller.h"

LinuxConsoleController::LinuxConsoleController() : ConsoleController()
{

}

LinuxConsoleController::LinuxConsoleController(int argc, char **argv) : ConsoleController(argc, argv)
{

}

QString LinuxConsoleController::takeCaptureArea()
{

}
