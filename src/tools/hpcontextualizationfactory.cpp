#include "hpcontextualizationfactory.h"

HpContextualizationFactory::HpContextualizationFactory() : ContextualizationFactoryAbstract()
{

}

ContextualizationController * HpContextualizationFactory::createController(char **params, int count)
{
    bool cliMode = count == 1 ? true : false;

    // Check required parameters.
    if (params == Q_NULLPTR || count < 1) {
        return Q_NULLPTR;
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
