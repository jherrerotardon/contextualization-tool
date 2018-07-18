#ifndef FPFILECONNECTOR_H
#define FPFILECONNECTOR_H

#include <QFile>
#include "databaseconnectorabstract.h"
#include "contextualization/model/firmwarestring.h"

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
     * @copydoc DatabaseConnectorAbstract::getStringsWithValue()
     */
    QList<String *> getStringsWithValue(const QString &value) override;

    /**
     * @copydoc DatabaseConnectorAbstract::getStringWithId(const QString &id)
     */
    QList<String *> getStringWithId(const QString &id) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertString(const String &string)
     */
    bool insertString(const String &string) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertStrings(const QList<String *> &strings)
     */
    bool insertStrings(const QList<String *> &strings) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithValue(const QString &value)
     */
    int removeStringsWithValue(const QString &value) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringWithId(const QString &id)
     */
    bool removeStringWithId(const QString &id) override;

private:
    QFile file_;          ///< File where strings is stored.
};

#endif // FPFILECONNECTOR_H
