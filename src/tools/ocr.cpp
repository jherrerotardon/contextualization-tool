#include "ocr.h"

Ocr::Ocr()
{
    Ocr();
}

Ocr::Ocr(QString image, QString datapath, tesseract::OcrEngineMode engineMode, tesseract::PageSegMode pageSegMode)
    : QThread()
{
    image_.setFileName(image);
    datapath_ = datapath;
    engineMode_ = engineMode;
    pageSegMode_ = pageSegMode;
    language_ = "eng"; ///< Default language always "eng".

    setlocale (LC_NUMERIC, "C"); ///< Necessary for the api to work.
    api_ = new tesseract::TessBaseAPI();
}

Ocr::Ocr(const Ocr &other) : QThread()
{
    image_.setFileName(other.getImage());
    datapath_ = other.getDataPath();
    engineMode_ = other.getEngineMode();
    pageSegMode_ = other.getPageSegMode();
    language_ = other.getLanguages().join('+');

    setlocale (LC_NUMERIC, "C"); ///< Necessary for the api to work.
    api_ = new tesseract::TessBaseAPI();
}

Ocr::~Ocr()
{
    delete api_;
    setlocale (LC_NUMERIC, "");
}

QStringList Ocr::extract()
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
        Log::writeError("Not languages available when ocr process was going run.");
        Log::writeError("Selected languages:" + language_);
        Log::writeError("Make sure you have at least one .traineddata file for any of the selected languages.");

        return QStringList();
    }

    if (!image_.exists()) {
        Log::writeError(image_.fileName() + " does not exist when ocr process was going run.");

        return QStringList();
    }

    // Improves quality of image to get better results in strings extraction.
    image = image.convertToFormat(QImage::Format_Grayscale8);

    // Saves a copy in disk to work with her.
    imageCopyPath = "/tmp/" + imageInfo.baseName() + "_copy." + imageInfo.suffix();
    workWithCopy = image.save(imageCopyPath, Q_NULLPTR, 100);

    //If the improvement is succesfull, set imagePix with the copy.
    if (workWithCopy) {
        imagePix = pixRead(imageCopyPath.toStdString().c_str());
    } else {
        imagePix = pixRead(image_.fileName().toStdString().c_str());
    }

    if (!imagePix) {
        Log::writeError(image_.fileName() + " can't be converted into Pix object.");

        return QStringList();
    }

    if (initApi()) {
        Log::writeError("Ocr Tesseract can't be initializated.");

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

QStringList Ocr::getAvailableLanguages() const
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

QStringList Ocr::getLanguages() const
{
    return language_.split('+');
}

bool Ocr::addLanguage(const QString &language)
{
    if(!isAvailableLanguage(language) || language_.split(QString("+")).contains(language)) {
        return false;
    }

    if (language_.isEmpty()) {
        language_ = language;
    } else {
        language_.append("+" + language);
    }

    return true;
}

bool Ocr::removeLanguage(const QString &language)
{
    int position;

    position = language_.indexOf(language);
    if (position != -1) {
        language_.remove(position, language.length()+1);

        return true;
    }

    return false;
}

QString Ocr::getImage() const
{
    return image_.fileName();
}

void Ocr::setImage(const QString &image)
{
    image_.setFileName(image);
}

QString Ocr::getDataPath() const
{
    return datapath_;
}

void Ocr::setDataPath(const QString &datapath)
{
    datapath_ = datapath.endsWith('/') ? datapath : datapath + '/';
}

tesseract::PageSegMode Ocr::getPageSegMode() const
{
    return pageSegMode_;
}

void Ocr::setPageSegMode(tesseract::PageSegMode pageSegMode)
{
    pageSegMode_ = pageSegMode;
}

tesseract::OcrEngineMode Ocr::getEngineMode() const
{
    return engineMode_;
}

void Ocr::setEngineMode(tesseract::OcrEngineMode engineMode)
{
    engineMode_ = engineMode;
}

int Ocr::initApi() const
{
    int hasError;

    hasError = api_->Init(
        datapath_.toStdString().c_str(),
        language_.toStdString().c_str(),
        engineMode_
    );

    if (hasError) {
        Log::writeError("Could not initialize tesseract.");
    }

    return hasError;
}

QStringList Ocr::processExtration(const QString &source)
{
    QStringList result;
    QString text = source;  // Because future replace will modify the string.

    if (source.isEmpty()) {
        return result;
    }

    // Remove rare characters (Slug)
    text = text.replace(QRegularExpression("[^a-zA-Z0-9 -,\.]"), "\n");

    // Force the beginning of the string to be a one-word character.
    text = text.replace(QRegularExpression("^\\W\\s*"), "");

    result << text.split('\n',QString::SkipEmptyParts);

    result.removeDuplicates();

    // Returns only those strings that have at least any character of a word.
    return result.filter(QRegularExpression("\\w+"));
}

bool Ocr::isAvailableLanguage(const QString &language)
{
    return getAvailableLanguages().contains(language);
}

void Ocr::run()
{
    Log::writeDebug("Hello baby: " + image_.fileName());
    QStringList extraction;

    extraction = extract();

    emit stringsExtracted(extraction);
}

Ocr & Ocr::operator=(const Ocr &other)
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
