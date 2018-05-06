#include "firmwarestring.h"

FirmwareString::FirmwareString(QString &id, QString &value, QString &description, QString maxLength, QString &state, bool selected)
{
    this->id = id;
    this->value = value;
    this->description = description;
    this->maxLength = maxLength;
    this->state = state;
    this->selected = selected;
}
/***********/
FirmwareString::FirmwareString(QString &value){
    this->id = "prueba";
    this->value = value;
    this->description = "prueba";
    this->maxLength = "20";
    this->state = "prueba";
    this->selected = true;
}
/***********/

QString FirmwareString::getId() const
{
    return this->id;
}

QString FirmwareString::getValue() const
{
    return this->value;
}

QString FirmwareString::getDescription() const
{
    return this->description;
}

QString FirmwareString::getMaxLength() const
{
    return this->maxLength;
}

QString FirmwareString::getState() const
{
    return this->state;
}

void FirmwareString::setValue(const QString &value)
{
    this->value = value;
}

bool FirmwareString::isSelected() const
{
    return this->selected;
}

void FirmwareString::select()
{
    this->selected = true;
}

void FirmwareString::unselect()
{
    this->selected = false;
}
