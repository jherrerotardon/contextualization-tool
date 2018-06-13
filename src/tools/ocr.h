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
#include "genericvector.h"
#include "log.h"

class Ocr
{
public:
    Ocr();
    Ocr(
        QString image,
        QString datapath = QString(),
        tesseract::OcrEngineMode engineMode = tesseract::OEM_TESSERACT_LSTM_COMBINED,
        tesseract::PageSegMode pageSegMode = tesseract::PSM_AUTO
    );
    ~Ocr();

    /**
     * @brief Extract strings contained in the image.
     *
     * Return a pointer to QStringsList containing the strings detected in the image.
     * The memory must be released when QStringList is no longer in use.
     *
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
    tesseract::TessBaseAPI *api_;
    QFile image_;
    QString language_; ///< Languagues used to detect.
    QString datapath_; ///< Path where are languages.
    tesseract::PageSegMode pageSegMode_;
    tesseract::OcrEngineMode engineMode_;

    int initApi();
    QStringList * processExtration(QString &source);
    bool isAvailableLanguage(QString &language);

};

#endif // OCR_H
