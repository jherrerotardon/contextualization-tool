#ifndef WINDOWSCONSOLECONTROLLER_H
#define WINDOWSCONSOLECONTROLLER_H

#include "consolecontroller.h"

class WindowsConsoleController : public ConsoleController
{
public:
    WindowsConsoleController();
    WindowsConsoleController(int argc, char **argv);
};

#endif // WINDOWSCONSOLECONTROLLER_H
