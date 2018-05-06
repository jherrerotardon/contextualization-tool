#include "contextualizationmodel.h"

ContextualizationModel::ContextualizationModel()
{
    image = "";
}

ContextualizationModel::~ContextualizationModel(){

    foreach (FirmwareString *fwString, this->stringsList) {
        delete fwString;
    }
}

int ContextualizationModel::addNewString(const QString &id, const QString &value, const QString &description, QString maxLength, const QString &state, bool selected){

    return 0;
}

void ContextualizationModel::addNewString(FirmwareString *newString)
{
    this->stringsList.append(newString);
}

void ContextualizationModel::deleteString(QString &id)
{
    int i = 0;

    foreach (FirmwareString *fwString, this->stringsList) {
        if (fwString->getId() == id){
            delete fwString;
            this->stringsList.removeAt(i);
            break;
        }
        i++;
    }
}

void ContextualizationModel::deleteString(int pos){

    delete this->stringsList.at(pos);
    this->stringsList.removeAt(pos);
}

void ContextualizationModel::clearStringsList()
{
    this->stringsList.clear();
}

QList<FirmwareString *> & ContextualizationModel::getStringsList(){
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
