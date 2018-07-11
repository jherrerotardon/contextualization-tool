#ifndef LINUXCONSOLECONTROLLER_H
#define LINUXCONSOLECONTROLLER_H

#include "consolecontroller.h"


class LinuxConsoleController : public ConsoleController
{
public:
    LinuxConsoleController();
    LinuxConsoleController(int argc, char **argv);
};

#endif // LINUXCONSOLECONTROLLER_H
