/**
 * @file hpcontextualizationfactory.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class HpContextualizationFactory
 *
 * @brief This is a factory to create a concrete class of ContextualizationController specific for HP company.
 */

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

    // Remove Qt Creator debbuger parameter.
    count = QString(params[count-1]).contains("-qmljsdebugger") ? count - 1 : count;

    cliMode = count > 1 ? true : false;

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
