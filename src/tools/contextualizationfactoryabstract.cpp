/**
 * @file contextualizationfactoryabstract.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class ContextualizationFactoryAbstract
 *
 * @brief This is a interface factory to create a concrete class of ContextualizationController.
 */

#include "contextualizationfactoryabstract.h"

ContextualizationFactoryAbstract::ContextualizationFactoryAbstract()
{

}

ContextualizationFactoryAbstract::KernelType ContextualizationFactoryAbstract::getCurrentKernelType(){
    QString product = QSysInfo::productType();

    if (product == "windows") {
        return Windows;
    } else if (product == "debian" || product == "ubuntu") {
        return Linux;
    } else {
        return Undefined;
    }
}
