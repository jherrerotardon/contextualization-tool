#ifndef HPCONTEXTUALIZATIONFACTORY_H
#define HPCONTEXTUALIZATIONFACTORY_H

#include "contextualizationfactoryabstract.h"
#include "contextualization/controller/linuxconsolecontroller.h"
#include "contextualization/controller/linuxguicontroller.h"
#include "contextualization/controller/windowsconsolecontroller.h"
#include "contextualization/controller/windowsguicontroller.h"

class HpContextualizationFactory : public ContextualizationFactoryAbstract
{
public:
    HpContextualizationFactory();
    ContextualizationController createController(char **params, int count) override;
};

#endif // HPCONTEXTUALIZATIONFACTORY_H
