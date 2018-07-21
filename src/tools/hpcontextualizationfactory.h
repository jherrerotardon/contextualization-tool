#ifndef HPCONTEXTUALIZATIONFACTORY_H
#define HPCONTEXTUALIZATIONFACTORY_H

#include "contextualizationfactoryabstract.h"
#include "contextualization/controller/linuxconsolecontroller.h"
#include "contextualization/controller/linuxguicontroller.h"
#include "contextualization/controller/windowsconsolecontroller.h"
#include "contextualization/controller/windowsguicontroller.h"
#include "tools/utils.h"

class HpContextualizationFactory : public ContextualizationFactoryAbstract
{
public:

    /**
     * @brief Creates an empty HpContextualizationFactory.
     */
    HpContextualizationFactory();

    /**
     * @brief Creates a new ContextualizationController
     *
     * Creates one type of ContextualizationController depending of received params and the operate system where de
     * application is running.
     * @param params Array params.
     * @param count Number of elements of params array.
     * @return Contextualization pointer.
     */
    ContextualizationController * createController(char **params, int count) override;
};

#endif // HPCONTEXTUALIZATIONFACTORY_H
