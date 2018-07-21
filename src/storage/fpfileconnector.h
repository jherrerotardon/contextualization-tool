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
     * @copydoc DatabaseConnectorAbstract::insertString(const String &string)
     */
    bool insertString(const String &string) override;

    /**
     * @copydoc DatabaseConnectorAbstract::insertStrings(const QList<String *> &strings)
     */
    int insertStrings(const QList<String *> &strings) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithValue(const QString &value, bool caseSensitive)
     */
    int removeStringsWithValue(const QString &value, bool caseSensitive = true) override;

    /**
     * @copydoc DatabaseConnectorAbstract::removeStringsWithId(const QString &id, bool caseSensitive)
     */
    int removeStringsWithId(const QString &id, bool caseSensitive = true) override;

private:
    QFile file_;          ///< File where strings is stored.
};

#endif // FPFILECONNECTOR_H
