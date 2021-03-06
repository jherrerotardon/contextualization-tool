/**
 * @file fpfileconnector.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class FpFileConnector
 *
 * @brief This is a class to access a database saved as fp file by HP company.
 */

#ifndef FPFILECONNECTOR_H
#define FPFILECONNECTOR_H

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "databaseconnectorabstract.h"
#include "contextualization/model/firmwarestring.h"
#include "tools/utils.h"

class FpFileConnector : public DatabaseConnectorAbstract
{
public:

    /**
     * @brief Creates an empty FpFileConnector
     */
    FpFileConnector();

    /**
     * @brief Creates a FpFileConnector and sets the path of file.
     * @param path Database file path
     */
    FpFileConnector(const QString &path);

    /**
     * @brief Returns a path of database file.
     * @return File path
     */
    QString getPath();

    /**
     * @brief Sets the path of database file.
     *
     * Returns true if the file exists and sets path succesfully, otherwise, returns false.
     * @return bool
     */
    bool setPath(const QString &path);

    /**
     * @copydoc DatabaseConnectorAbstract::getAllStrings(const QString &value)
     */
    QList<String *> getAllStrings() override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringsWithValue(const QString &value, bool caseSensitive)
     */
    QList<String *> getStringsWithValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringsWithApproximateValue(const QString &value, bool caseSensitive)
     */
    QList<String *> getStringsWithApproximateValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringWithId(const QString &id, bool caseSensitive)
     */
    QList<String *> getStringWithId(const QString &id, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringsWithState(const QString state)
     */
    QList<String *> getStringsWithState(const QString state) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertString(const String &string, const QString language)
     */
    bool insertString(const String &string, const QString language = QString()) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertStrings(const QList<String *> &strings, const QString language)
     */
    int insertStrings(const QList<String *> &strings, const QString language = QString()) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithValue(const QString &value, bool caseSensitive)
     */
    int removeStringsWithValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithId(const QString &id, bool caseSensitive)
     */
    int removeStringsWithId(const QString &id, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getLanguages()
     */
    QStringList getLanguages() override;

    /**
     * @copydoc DatabaseConnectorAbstract::getLanguageIds()
     */
    QStringList getLanguageIds() override;

private:
    QFile file_;          ///< File where strings is stored.
};

#endif // FPFILECONNECTOR_H
