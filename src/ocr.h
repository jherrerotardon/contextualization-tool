/**
 * @class Ocr
 * @brief Optical Character Recognition.
 *
 * Interface class to use tesseract api.
 */
#ifndef OCR_H
#define OCR_H

#include <QString>
#include <QFile>
#include <QProcess>
#include <leptonica/allheaders.h>
#include "baseapi.h"
#include "log.h"

class Ocr
{
public:
    Ocr();
    Ocr(QString image, QString language = QString("eng"), QString datapath = QString());
    Ocr(QString image, QStringList languages, QString datapath = QString());

    /**
     * @brief Extract strings contained in the image.
     *
     * Return an only string
     * @return QStringList
     */
    QStringList * run();
    QStringList getAvailableLanguages();
    QStringList getLanguages();

    /**
     * @brief Add a language to try recognize characters in the image.
     *
     * Check if the string recieved is an available language.
     * Return true if the lenguage is added, if not return false.
     *
     * @param lang QString that contains the language to add.
     * @return true|false
     */
    bool addLanguage(QString language);
    bool removeLanguage(QString language);
    QString getImage();
    void setImage(QString image);
    QString getDataPath();
    void setDataPath(QString datapath);
    tesseract::PageSegMode getPageSegMode();
    void setPageSegMode(tesseract::PageSegMode pageSegMode);
    tesseract::OcrEngineMode getEngineMode();
    void setEngineMode(tesseract::OcrEngineMode engineMode);

private:
    tesseract::TessBaseAPI *api;
    QFile image;
    QString languages; ///< Languagues used to detect.
    QString datapath; ///< Path where are languages.
    tesseract::PageSegMode pageSegMode;
    tesseract::OcrEngineMode engineMode;
    //TODO: que hacer con estas variables.
    //GenericVector<STRING> vars_vec; ///< Location of user words file.
    //GenericVector<STRING> vars_values; ///< Location of user patterns file.

    int initApi();
    QStringList * processExtration(QString &source);
    bool isAvailableLanguage(QString &language);

};

#endif // OCR_H
