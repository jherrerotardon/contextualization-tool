#ifndef CONTEXTUALIZATIONFACTORYABSTRACT_H
#define CONTEXTUALIZATIONFACTORYABSTRACT_H

#include "contextualization/controller/contextualizationcontroller.h"

class ContextualizationFactoryAbstract
{
public:
    ContextualizationFactoryAbstract();
    virtual ContextualizationController createController(char **params, int count) = 0;
};

#endif // CONTEXTUALIZATIONFACTORYABSTRACT_H
