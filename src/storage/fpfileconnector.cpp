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

QList<String *> FpFileConnector::getStringsWithValue(const QString &value, bool caseSensitive)
{
    QString line;
    QList<String *> out;
    FirmwareString *fwString;
    Qt::CaseSensitivity isCaseSensitive = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    // Begin read file.
    if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Log::writeError(" Fail to open file: " + file_.fileName());

        return out;
    }

    QTextStream in(&file_);

    while (!in.atEnd()) {
        line = in.readLine();
        if (line.startsWith('#')) { // It is a comment line.
            continue;
        }

        fwString = FirmwareString::fromFpLine(line);
        if (fwString) {
            //If the text belongs to a string, the fwString is saved, otherwise relsease memory of fwString.
            if (value.compare(fwString->getValue(), isCaseSensitive) == 0) {
                out << fwString;
            } else {
                delete fwString;
            }
        }
    }

    file_.close();

    return out;
}

QList<String *> FpFileConnector::getStringsWithApproximateValue(const QString &value, bool caseSensitive)
{
    QString line;
    QList<String *> out;
    FirmwareString *fwString;
    Qt::CaseSensitivity isCaseSensitive = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    // Begin read file.
    if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Log::writeError(" Fail to open file: " + file_.fileName());

        return out;
    }

    QTextStream in(&file_);

    while (!in.atEnd()) {
        line = in.readLine();
        if (line.startsWith('#')) { // It is a comment line.
            continue;
        }

        fwString = FirmwareString::fromFpLine(line);
        if (fwString) {
            /**
             * If the value belongs to a string, the fwString is saved, otherwise relsease memory of fwString.
             * If size of both strings is longer than MIN_LENGTH_FOR_APPROXIMATE and their size difference is less than
             * MAX_LENGTH_DIFFERENCE, a value is considered valid if it is contained within the fwString value or
             * vice versa.
             * If size of any strings is less than MIN_LENGTH_FOR_APPROXIMATE, a value is considered valid only if it
             * is equals than the value of fwString.
             */
            if (value.size() > MIN_LENGTH_FOR_APPROXIMATE && fwString->getValue().size() > MIN_LENGTH_FOR_APPROXIMATE &&
                qAbs(value.size() - fwString->getValue().size()) < MAX_LENGTH_DIFFERENCE) {
                if (value.contains(fwString->getValue(), isCaseSensitive) || fwString->getValue().contains(value, isCaseSensitive)) {
                    out << fwString;
                } else {
                    delete fwString;
                }
            } else {
                if (value.compare(fwString->getValue(), isCaseSensitive) == 0) {
                    out << fwString;
                } else {
                    delete fwString;
                }
            }
        }
    }

    file_.close();

    return out;
}

QList<String *> FpFileConnector::getStringWithId(const QString &id, bool caseSensitive)
{
    QString line;
    QList<String *> out;
    FirmwareString *fwString;
    Qt::CaseSensitivity isCaseSensitive = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    // Begin read file.
    if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Log::writeError(" Fail to open file: " + file_.fileName());

        return out;
    }

    QTextStream in(&file_);

    while (!in.atEnd()) {
        line = in.readLine();
        if (line.startsWith('#')) { // It is a comment line.
            continue;
        }

        fwString = FirmwareString::fromFpLine(line);
        if (fwString) {
            /**
             * If the id belongs to a string, the fwString is saved, otherwise relsease memory of fwString.
             * An identifier is considered valid only if it is equals than the identifier of fwString.
             */
            if (id.compare(fwString->getId(), isCaseSensitive) == 0) {
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

int FpFileConnector::insertStrings(const QList<String *> &strings)
{

}

int FpFileConnector::removeStringsWithValue(const QString &value, bool caseSensitive)
{

}

int FpFileConnector::removeStringsWithId(const QString &id, bool caseSensitive)
{

}
