#ifndef TESSERACTOCR_H
#define TESSERACTOCR_H

#include <leptonica/allheaders.h>
#include "baseapi.h"
#include "genericvector.h"
#include "ocr.h"

class TesseractOcr : public Ocr
{
public:

    /**
     * @brief Creates an empty TesseractOcr instance.
     *
     * Sets default values.
     */
    TesseractOcr();

    /**
     * @brief Creates a TesseractOcr instante with the values received by parameter.
     * @param image Path of image where strings will be ectracted.
     * @param datapath Folder path where data languages will be.
     * @param engineMode Engine mode wich will use tesseract.
     * @param pageSegMode Page segmentation mode will use tesseract.
     */
    TesseractOcr(
        QString image,
        QString datapath = QDir("../tesseract/tessdata").absolutePath() + '/',
        tesseract::OcrEngineMode engineMode = tesseract::OEM_TESSERACT_LSTM_COMBINED,
        tesseract::PageSegMode pageSegMode = tesseract::PSM_AUTO
    );

    /**
     * @brief Creates a copy from other TesseractOcr instance.
     * @param other
     */
    TesseractOcr(const TesseractOcr &other);

    /**
     * @brief Destroys the tesseract ocr instance.
     */
    ~TesseractOcr();

    /**
     * @copydoc Ocr::extract()
     */
    QStringList extract() override;

    /**
     * @copydoc Ocr::getAvailableLanguages()
     */
    QStringList getAvailableLanguages() const override;

    /**
     * @copydoc Ocr::isAvailableLanguage()
     */
    bool isAvailableLanguage(const QString &language) override;

    /**
     * @brief Returns the page segmentation mode configured to be used.
     * @return Page segmentation mode.
     */
    tesseract::PageSegMode getPageSegMode() const;

    /**
     * @brief Sets the page segmentation mode to be used in the extraction.
     * @param pageSegMode Page segmentation mode.
     */
    void setPageSegMode(tesseract::PageSegMode pageSegMode);

    /**
     * @brief Returns the engine mode configured to be used.
     * @return
     */
    tesseract::OcrEngineMode getEngineMode() const;

    /**
     * @brief Sets the engine mode to be used in the extraction.
     * @param engineMode Engine mode.
     */
    void setEngineMode(tesseract::OcrEngineMode engineMode);

    /**
     * @brief Assigns other to TesseractOcr and returns a reference to this TesseractOcr object.
     * @param other TesseractOcr object to be copied.
     * @return Reference to this TesseractOcr object.
     */
    TesseractOcr & operator=(const TesseractOcr &other);

private:
    tesseract::TessBaseAPI *api_;           ///< Api pointer class wich work the recognition.
    tesseract::PageSegMode pageSegMode_;    ///< Page segmentation mode to be used by tesseract.
    tesseract::OcrEngineMode engineMode_;   ///< Engine mode to be used by tesseract.

    /**
     * @brief Inits tesseract API.
     *
     * Returns 0 if there was no errors, else, returns a numer longer than 0.
     * @return Code error.
     */
    int initApi() const;

    /**
     * @copydoc Ocr::processExtration()
     */
    QStringList processExtration(const QString &source) override;

};

#endif // TESSERACTOCR_H
