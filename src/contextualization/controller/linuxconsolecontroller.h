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
