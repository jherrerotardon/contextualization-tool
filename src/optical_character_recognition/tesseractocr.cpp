/**
 * @file tesseractocr.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class TesseractOcr
 *
 * @brief This is a tool to detect string on images using tesseract api (https://github.com/tesseract-ocr/tesseract).
 */


#include "tesseractocr.h"

TesseractOcr::TesseractOcr()
{
    TesseractOcr(QString());
}

TesseractOcr::TesseractOcr(
    QString image,
    QString datapath,
    tesseract::OcrEngineMode engineMode,
    tesseract::PageSegMode pageSegMode
) {
    image_.setFileName(image);
    datapath_ = datapath;
    engineMode_ = engineMode;
    pageSegMode_ = pageSegMode;
    language_ = "eng"; ///< Default language always "eng".

    setlocale (LC_ALL, "C"); ///< Necessary for the api to work.
    api_ = new tesseract::TessBaseAPI();
}

TesseractOcr::TesseractOcr(const TesseractOcr &other)
{
    image_.setFileName(other.getImage());
    datapath_ = other.getDataPath();
    engineMode_ = other.getEngineMode();
    pageSegMode_ = other.getPageSegMode();
    language_ = other.getLanguages().join('+');

    setlocale (LC_ALL, "C"); ///< Necessary for the api to work.
    api_ = new tesseract::TessBaseAPI();
}

TesseractOcr::~TesseractOcr()
{
    delete api_;
    setlocale (LC_ALL, "");
}

QStringList TesseractOcr::extract()
{
    QFileInfo imageInfo(image_);
    QString source;
    Pix *imagePix;
    QImage image(image_.fileName());
    QString imageCopyPath;
    char *text;
    bool workWithCopy;

    if (getAvailableLanguages().size() == 0)
    {
        Log::writeError(QString(Q_FUNC_INFO) + " Not languages available when ocr process was going run.");
        Log::writeError(QString(Q_FUNC_INFO) + " Selected languages:" + language_);
        Log::writeError(QString(Q_FUNC_INFO) + " Make sure you have at least one .traineddata file for any of the selected languages.");

        return QStringList();
    }

    if (!image_.exists()) {
        Log::writeError(QString(Q_FUNC_INFO) + image_.fileName() + " does not exist when ocr process was going run.");

        return QStringList();
    }

    // Improves quality of image to get better results in strings extraction.
    image = image.convertToFormat(QImage::Format_Grayscale8);

    // Saves a copy in disk to work with her.
    imageCopyPath = Utils::getTmpDirectory() + "/" + imageInfo.baseName() + "_copy." + imageInfo.suffix();
    workWithCopy = image.save(imageCopyPath, Q_NULLPTR, 100);

    //If the improvement is succesfull, set imagePix with the copy.
    if (workWithCopy) {
        imagePix = pixRead(imageCopyPath.toStdString().c_str());
    } else {
        imagePix = pixRead(image_.fileName().toStdString().c_str());

        Log::writeError(QString(Q_FUNC_INFO) + " Could not create image copy in " + imageCopyPath);
    }

    if (!imagePix) {
        Log::writeError(QString(Q_FUNC_INFO) + " " + image_.fileName() + " can't be converted into Pix object.");

        return QStringList();
    }

    if (initApi()) {
        Log::writeError(QString(Q_FUNC_INFO) + " Ocr Tesseract can't be initializated.");

        return QStringList();
    }

    api_->SetPageSegMode(pageSegMode_);
    api_->SetImage(imagePix);

    text = api_->GetUTF8Text(); // OCR result.
    source = QString(text);

    // Destroy used objects and release memory
    delete []text;
    api_->End();
    pixDestroy(&imagePix);

    // If have worked with the copy, the copy is deleted.
    if (workWithCopy) {
        QFile::remove(imageCopyPath);
    }

    return processExtration(source);
}

QStringList TesseractOcr::getAvailableLanguages() const
{
    GenericVector<STRING> languages;
    QStringList availableLanguages;

    if (initApi()) {
        return availableLanguages;
    }

    api_->GetAvailableLanguagesAsVector(&languages);
    for (int index = 0; index < languages.size(); ++index) {
        availableLanguages << languages[index].string();
    }

    api_->End();

    return availableLanguages;
}

bool TesseractOcr::isAvailableLanguage(const QString &language)
{
    return getAvailableLanguages().contains(language);
}

tesseract::PageSegMode TesseractOcr::getPageSegMode() const
{
    return pageSegMode_;
}

void TesseractOcr::setPageSegMode(tesseract::PageSegMode pageSegMode)
{
    pageSegMode_ = pageSegMode;
}

tesseract::OcrEngineMode TesseractOcr::getEngineMode() const
{
    return engineMode_;
}

void TesseractOcr::setEngineMode(tesseract::OcrEngineMode engineMode)
{
    engineMode_ = engineMode;
}

int TesseractOcr::initApi() const
{
    int hasError;

    hasError = api_->Init(
        datapath_.toStdString().c_str(),
        language_.toStdString().c_str(),
        engineMode_
    );

    if (hasError) {
        Log::writeError(QString(Q_FUNC_INFO) + " Could not initialize tesseract.");
    }

    return hasError;
}

QStringList TesseractOcr::processExtration(const QString &source)
{
    QStringList result;
    QString text = source;  // Because future replace will modify the string.

    if (source.isEmpty()) {
        return result;
    }

    // Remove rare characters (Slug)
    text = text.replace(QRegularExpression("[^a-zA-Z0-9 -,\.]"), "\n");

    // Force the beginning of the string to be a one-word character.
    text = text.replace(QRegularExpression("^\\W+\\s*", QRegularExpression::MultilineOption), "");

    // If the first letter of a sentence is lowercase, it is assumed that it belongs to the previous word.
    text = text.replace(QRegularExpression("\n([a-z])"), " \\1");

    result << text.split('\n',QString::SkipEmptyParts);

    result.removeDuplicates();

    // Returns only those strings that have at least any character of a word.
    return result.filter(QRegularExpression("\\w+"));
}

TesseractOcr & TesseractOcr::operator=(const TesseractOcr &other)
{
    if (this != &other) {
        image_.setFileName(other.getImage());
        datapath_ = other.getDataPath();
        engineMode_ = other.getEngineMode();
        pageSegMode_ = other.getPageSegMode();
        language_ = other.getLanguages().join('+');
    }

    return *this;
}
