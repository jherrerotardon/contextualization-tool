#include "fpfileconnector.h"

FpFileConnector::FpFileConnector() : DatabaseConnectorAbstract()
{

}

FpFileConnector::FpFileConnector(const QString &path) : DatabaseConnectorAbstract()
{
    setPath(path);
}

QString FpFileConnector::getPath()
{
    return file_.fileName();
}

bool FpFileConnector::setPath(const QString &path)
{
    if (QFile::exists(path)) {
        file_.setFileName(path);

        return true;
    }

    return false;
}

QList<String *> FpFileConnector::getAllStrings()
{
    QList<String *> out;
    FirmwareString *fwString;

    // Begin read file.
    if (!file_.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Log::writeError(" Fail to open file: " + file_.fileName());

        return out;
    }

    QTextStream in(&file_);

    while (!in.atEnd()) {
        fwString = FirmwareString::fromFpLine(in.readLine());
        if (fwString) {
            out << fwString;
        }
    }

    file_.close();

    return out;
}

QList<String *> FpFileConnector::getStringsWithValue(const QString &value)
{
    QList<String *> out;
    FirmwareString *fwString;

    // Begin read file.
    if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Log::writeError(" Fail to open file: " + file_.fileName());

        return out;
    }

    QTextStream in(&file_);

    while (!in.atEnd()) {
        fwString = FirmwareString::fromFpLine(in.readLine());
        if (fwString) {
            //If the text belongs to a string the fwString is saved, otherwise relsease memory of fwString.
            if (value == fwString->getValue()) {
                out << fwString;
            } else {
                delete fwString;
            }
        }
    }

    file_.close();

    return out;
}

QList<String *> FpFileConnector::getStringWithId(const QString &id)
{
    QList<String *> out;
    FirmwareString *fwString;

    // Begin read file.
    if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Log::writeError(" Fail to open file: " + file_.fileName());

        return out;
    }

    QTextStream in(&file_);

    while (!in.atEnd()) {
        fwString = FirmwareString::fromFpLine(in.readLine());
        if (fwString) {
            //If the text belongs to a string the fwString is saved, otherwise relsease memory of fwString.
            if (id == fwString->getId()) {
                out << fwString;
            } else {
                delete fwString;
            }
        }
    }

    file_.close();

    return out;
}

bool FpFileConnector::insertString(const String &string)
{

}

bool FpFileConnector::insertStrings(const QList<String *> &strings)
{

}

int FpFileConnector::removeStringsWithValue(const QString &value)
{

}

bool FpFileConnector::removeStringWithId(const QString &id)
{

}
