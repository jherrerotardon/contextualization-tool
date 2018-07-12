#ifndef CONTEXTUALIZATIONFACTORYABSTRACT_H
#define CONTEXTUALIZATIONFACTORYABSTRACT_H

#include <QSysInfo>
#include "contextualization/controller/contextualizationcontroller.h"

class ContextualizationFactoryAbstract
{
public:

    /**
     * @brief Kernel types for which ContextuationController are implemented.
     */
    enum KernelType {
        Windows,            //< Windows normal desktop builds.
        Linux,              //< Linux systems.
        Undefined,          //< Not implementation for the current kernel type.
    };

    /**
     * @brief Creates an empty ContextualizationFactoryAbstract.
     */
    ContextualizationFactoryAbstract();

    /**
     * @brief Returns the kernel type where the app is running.
     * @return Kernel type of the current system.
     */
    KernelType getCurrentKernelType();

    /**
     * @brief Instantiates a new controller for a contextualization.
     *
     * This is a pure vistuar function that must to be implemented in inherits classes.
     * @param params Array params.
     * @param count Number of elements of params.
     * @return A new controller.
     */
    virtual ContextualizationController * createController(char **params, int count) = 0;
};

#endif // CONTEXTUALIZATIONFACTORYABSTRACT_H
