/**
 * @class Ocr
 * @brief Optical Character Recognition.
 *
 * Interface class to use tesseract api.
 */
#ifndef OCR_H
#define OCR_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QThread>
#include <QRegularExpression>
#include <leptonica/allheaders.h>
#include "baseapi.h"
#include "genericvector.h"
#include "log.h"

class Ocr : public QThread
{
    Q_OBJECT

public:
    Ocr();
    Ocr(
        QString image,
        QString datapath = QDir("../tesseract/tessdata").absolutePath() + '/',
        tesseract::OcrEngineMode engineMode = tesseract::OEM_TESSERACT_LSTM_COMBINED,
        tesseract::PageSegMode pageSegMode = tesseract::PSM_AUTO
    );
    Ocr(const Ocr &other);
    ~Ocr();

    /**
     * @brief Extracts strings contained in the image.
     *
     * Return a QStringsList containing the strings detected in the image.
     * @return Strings list with strins extracted.
     */
    QStringList extract();
    QStringList getAvailableLanguages() const;
    QStringList getLanguages() const;

    /**
     * @brief Add a language to try recognize characters in the image.
     *
     * Check if the string recieved is an available language.
     * Return true if the lenguage is added, if not return false.
     *
     * @param lang QString that contains the language to add.
     * @return true|false
     */
    bool addLanguage(const QString &language);
    bool removeLanguage(const QString &language);
    QString getImage() const;
    void setImage(const QString &image);
    QString getDataPath() const;
    void setDataPath(const QString &datapath);
    tesseract::PageSegMode getPageSegMode() const;
    void setPageSegMode(tesseract::PageSegMode pageSegMode);
    tesseract::OcrEngineMode getEngineMode() const;
    void setEngineMode(tesseract::OcrEngineMode engineMode);

    /**
     * @brief Assigns other to Ocr and returns a reference to this Ocr object.
     * @param other Ocr object to be copied.
     * @return Reference to this Ocr object.
     */
    Ocr & operator=(const Ocr &other);

signals:
    void stringsExtracted(QStringList stringsList);

private:
    tesseract::TessBaseAPI *api_;
    QFile image_;
    QString language_; ///< Languagues used to detect.
    QString datapath_; ///< Path where are languages.
    tesseract::PageSegMode pageSegMode_;
    tesseract::OcrEngineMode engineMode_;

    int initApi() const;
    QStringList processExtration(const QString &source);
    bool isAvailableLanguage(const QString &language);

    /**
     * @brief Extracts strings contained in the image.
     *
     * When extraction has finished, stringnsDetected signal is emited giving the QStringList with extracted strigns.
     *
     * This is an overwritten function that is the starting point for the thread. It is not recommended to call this
     * function directly or it will not work as a separate thread.
     */
    void run() override;


};

#endif // OCR_H
