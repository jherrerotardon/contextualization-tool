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
