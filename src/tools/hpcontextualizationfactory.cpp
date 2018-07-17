#include "hpcontextualizationfactory.h"

HpContextualizationFactory::HpContextualizationFactory() : ContextualizationFactoryAbstract()
{

}

ContextualizationController * HpContextualizationFactory::createController(char **params, int count)
{
    bool cliMode;

    // Check required parameters.
    if (params == Q_NULLPTR || count < 1) {
        return Q_NULLPTR;
    }

    if (count > 1) { // Consider parameters from debugger.
        cliMode = QString(params[1]).contains("-qmljsdebugger") ? false : true;
    } else {
        cliMode = false;
    }

    switch (getCurrentKernelType()) {
        case Linux:
        if (cliMode) {
            return new LinuxConsoleController(count, params);
        }

         return new LinuxGuiController();

        case Windows:
            if (cliMode) {
                 return new WindowsConsoleController(count, params);
            }

             return new WindowsGuiController();

        default:
            return Q_NULLPTR;
    }
}
