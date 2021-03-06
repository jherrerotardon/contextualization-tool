/**
 * @file windowsconsolecontroller.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class WindowsConsoleController
 *
 * @brief This is the controller class that works a Windows CLI environment.
 */

/////////////////////////////////////////////////////
//  NOT IMPLEMENTED CLASS YET.                  ////
////////////////////////////////////////////////////

#ifndef WINDOWSCONSOLECONTROLLER_H
#define WINDOWSCONSOLECONTROLLER_H

#include "consolecontroller.h"

class WindowsConsoleController : public ConsoleController
{
public:

    /**
     * @brief Creates an empty WindowsConsoleController
     */
    WindowsConsoleController();

    /**
     * @brief Creates a WindowsConsoleController initialited with the parameter received in argv.
     * @param argc Number of elements of argv.
     * @param argv Parameters
     */
    WindowsConsoleController(int argc, char **argv);

    /**
     * @copydoc ContextualizationController::takeCaptureArea()
     */
    QString takeCaptureArea() override;

    /**
     * @copydoc ContextualizationController::generateDoneFpFile()
     */
    int generateDoneFpFile() override;
};

#endif // WINDOWSCONSOLECONTROLLER_H
