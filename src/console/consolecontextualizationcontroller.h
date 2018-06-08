#ifndef CONSOLECONTEXTUALIZATIONCONTROLLER_H
#define CONSOLECONTEXTUALIZATIONCONTROLLER_H

#include "base/contextualizationcontrollerbase.h"

class ConsoleContextualizationController : public ContextualizationControllerBase
{
public:
    ConsoleContextualizationController(int argc, char *argv[], QObject *parent = nullptr);

    void exec();
    int decodeArguments();
    void printUsage();
};

#endif // CONSOLECONTEXTUALIZATIONCONTROLLER_H