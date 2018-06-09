#include "consolecontextualizationcontroller.h"

ConsoleContextualizationController::ConsoleContextualizationController(int argc, char *argv[], QObject *parent)
    : ContextualizationControllerBase(parent)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

}

void ConsoleContextualizationController::exec()
{

}

int ConsoleContextualizationController::decodeArguments()
{

    return 0;
}

void ConsoleContextualizationController::printUsage()
{

}
