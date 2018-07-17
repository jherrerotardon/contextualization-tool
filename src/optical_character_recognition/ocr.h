/**
 * @class Ocr
 * @brief Optical Character Recognition.
 *
 * Interface class to use ocr tools.
 */
#ifndef OCR_H
#define OCR_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QRegularExpression>
#include "tools/log.h"

class Ocr
{
public:

    /**
     * @brief Creates an empty Ocr object.
     *
     * Sets default values.
     */
    Ocr();

    /**
     * @brief Extracts strings contained in the image.
     *
     * Returns a QStringsList containing the strings detected in the image.
     * @return Strings list with strings extracted.
     */
    virtual QStringList extract() = 0;

    /**
     * @brief Returns all available languages that can be used to extract strings from image.
     * @return List with all available languages.
     */
    virtual QStringList getAvailableLanguages() const = 0;

    /**
     * @brief Returns set languages to extract strings from image.
     * @return List with set languages.
     */
    virtual QStringList getLanguages() const;

    /**
     * @brief Add a language to try recognize characters in the image.
     *
     * Check if the string recieved is an available language.
     * Return true if the lenguage is added, if not return false.
     *
     * @param lang QString that contains the language to add.
     * @return true|false
     */
    virtual bool addLanguage(const QString &language);

    /**
     * @brief Remove a language to try recognize characters in the image.
     *
     * Returns true if the language was removed succesfully, otherwise, returns false.
     * @param language String language to be removed.
     * @return bool
     */
    virtual bool removeLanguage(const QString &language);

    /**
     * @brief Returns true if the language received by parameter is available to be used in the recognition, otherwise,
     * returns false.
     * @param language String language to check.
     * @return bool
     */
    virtual bool isAvailableLanguage(const QString &language);

    /**
     * @brief Returns the path of image configured to recognize characters in.
     * @return Image path.
     */
    QString getImage() const;

    /**
     * @brief Sets the image path where try to recognize characters.
     * @param image Path of image.
     */
    void setImage(const QString &image);

    /**
     * @brief Returns folder path where are languages files.
     * @return Folder path.
     */
    QString getDataPath() const;

    /**
     * @brief Sets folder path where are languages files.
     * @param datapath Folder path.
     */
    void setDataPath(const QString &datapath);

protected:
    QFile image_;                           ///< Image where will try to detect strings.
    QString language_;                      ///< Languagues used to detect.
    QString datapath_;                      ///< Path where are languages files.

    /**
     * @brief Processes the text received by parameted.
     *
     * Split the string in phrases and returns a list with phrases contained in source.
     * @param source
     * @return
     */
    virtual QStringList processExtration(const QString &source) = 0;
};

#endif // OCR_H
