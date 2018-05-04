#include "contextualizationmodel.h"

ContextualizationModel::ContextualizationModel()
{
    image = "";
    stringsList = NULL;
}

ContextualizationModel::~ContextualizationModel(){

    if (stringsList != NULL) {
        /*Borrar todos no solo la primera*/
        delete stringsList;
    }
}

int ContextualizationModel::addNewString(const QString &id, const QString &value, const QString &description, int maxLength, const QString &state, bool selected){

    return 0;
}

int ContextualizationModel::deleteString(QString &id){

    return 0;
}

FirmwareString * ContextualizationModel::getStringsList(){
    return stringsList;
}

void ContextualizationModel::setImagePath(QString &path){
    //Esto elimina el objeto anterior o lo sobreescribe?
    image = path;
}

QString ContextualizationModel::getImagePath(){
    return this->image;
}

int ContextualizationModel::emptyModel(){

    return 0;
}
