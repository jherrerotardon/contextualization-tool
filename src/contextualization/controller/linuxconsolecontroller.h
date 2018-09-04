/**
 * @file linuxconsolecontroller.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class LinuxConsoleController
 *
 * @brief This is the controller class that works a linux CLI environment.
 */

#ifndef LINUXCONSOLECONTROLLER_H
#define LINUXCONSOLECONTROLLER_H

#include "consolecontroller.h"


class LinuxConsoleController : public ConsoleController
{
public:

    /**
     * @brief Creates an empty LinuxConsoleController.
     */
    LinuxConsoleController();

    /**
     * @brief Creates a LinuxConsoleController initialited with the parameter received in argv.
     * @param argc Number of elements of argv.
     * @param argv Parameters
     */
    LinuxConsoleController(int argc, char **argv);

    /**
     * @copydoc ContextualizationController::takeCaptureArea()
     */
    QString takeCaptureArea() override;
};

#endif // LINUXCONSOLECONTROLLER_H
