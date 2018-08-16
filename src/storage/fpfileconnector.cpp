/**
 * @file fpfileconnector.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class FpFileConnector
 *
 * @brief This is a class to access a database saved as fp file by HP company.
 */

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
        Log::writeError(QString(Q_FUNC_INFO) + "  Fail to open file: " + file_.fileName());

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
        Log::writeError(QString(Q_FUNC_INFO) + "  Fail to open file: " + file_.fileName());

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
        Log::writeError(QString(Q_FUNC_INFO) + "  Fail to open file: " + file_.fileName());

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
        Log::writeError(QString(Q_FUNC_INFO) + "  Fail to open file: " + file_.fileName());

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
    if (getStringWithId(string.getId(), true).size() > 0) {
        Log::writeError(QString(Q_FUNC_INFO) + " Try to add string in " + file_.fileName() +
                        " but ID already exists: " + string.toFpFileFormat());

        return false;
    }

    if (Utils::appendFile(file_.fileName(), string.toFpFileFormat()) == false) {
        Log::writeError(QString(Q_FUNC_INFO) + " Can't append new String " + string.getId() + " in file " + file_.fileName());

        return true;
    }

    return false;
}

int FpFileConnector::insertStrings(const QList<String *> &strings)
{
    int count = 0;

    foreach (String *string, strings) {
        count = insertString(*string) ? count+1 : count;
    }

    return count;
}

int FpFileConnector::removeStringsWithValue(const QString &value, bool caseSensitive)
{
    Qt::CaseSensitivity isCaseSensitive = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
    FirmwareString *fwString;
    int count = 0;
    QString tmpFileName = "tmp_fp_" + Utils::getDateTime();
    QFile tmpFile(tmpFileName);

    if (file_.rename(tmpFileName) == false) {
        Log::writeError(QString(Q_FUNC_INFO) + "  Could not copy " + file_.fileName() + " to remove strings.");

        return count;
    }

    if (!file_.open(QIODevice::WriteOnly | QIODevice::Text)) {
        Log::writeError(QString(Q_FUNC_INFO) + "  Could not write in " + file_.fileName() + " to remove strings.");

        return count;
    }

    if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Log::writeError(QString(Q_FUNC_INFO) + "  Could not open " + file_.fileName() + " to read strings.");

        return count;
    }

    // Read temporal file and only copy strings that will not be removed.
    QTextStream out(&file_);
    QTextStream in(&tmpFile);

    while (!in.atEnd()) {
        fwString = FirmwareString::fromFpLine(in.readLine());
        if (fwString) {
            if (value.compare(fwString->getValue(), isCaseSensitive) == 0) {
                out << fwString->toFpFileFormat() << '\n';
            } else {
                count++; // One more removed string
            }

            delete fwString;
        }
    }

    file_.close();
    tmpFile.close();

    return count;
}

int FpFileConnector::removeStringsWithId(const QString &id, bool caseSensitive)
{
    Qt::CaseSensitivity isCaseSensitive = caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
    FirmwareString *fwString;
    int count = 0;
    QString tmpFileName = "tmp_fp_" + Utils::getDateTime();
    QFile tmpFile(tmpFileName);

    if (file_.rename(tmpFileName) == false) {
        Log::writeError(QString(Q_FUNC_INFO) + "  Could not copy " + file_.fileName() + " to remove strings.");

        return count;
    }

    if (!file_.open(QIODevice::WriteOnly | QIODevice::Text)) {
        Log::writeError(QString(Q_FUNC_INFO) + "  Could not write in " + file_.fileName() + " to remove strings.");

        return count;
    }

    if (!file_.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Log::writeError(QString(Q_FUNC_INFO) + "  Could not open " + file_.fileName() + " to read strings.");

        return count;
    }

    // Read temporal file and only copy strings that will not be removed.
    QTextStream out(&file_);
    QTextStream in(&tmpFile);

    while (!in.atEnd()) {
        fwString = FirmwareString::fromFpLine(in.readLine());
        if (fwString) {
            if (id.compare(fwString->getId(), isCaseSensitive) == 0) {
                out << fwString->toFpFileFormat() << '\n';
            } else {
                count++; // One more removed string
            }

            delete fwString;
        }
    }

    file_.close();
    tmpFile.close();

    return count;
}

QStringList FpFileConnector::getLanguages()
{
    QFileInfo file(file_);

    // Return file names without extensions.
    return file.absoluteDir().entryList(QStringList() << "*.fp").replaceInStrings(QRegularExpression("\\.fp$"), "");

}

QStringList FpFileConnector::getLanguageIds()
{
    // Actually not lenguage keys in fp files.
    return getLanguages();
}
