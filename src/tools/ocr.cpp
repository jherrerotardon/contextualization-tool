#include "ocr.h"

Ocr::Ocr()
{
    Ocr(QString(""));
}

Ocr::Ocr(QString image, QString datapath, tesseract::OcrEngineMode engineMode, tesseract::PageSegMode pageSegMode)
{
    this->image.setFileName(image);
    this->datapath = datapath;
    this->engineMode = engineMode;
    this->pageSegMode = pageSegMode;
    this->language = "eng"; ///< Default language always "eng".

    setlocale (LC_NUMERIC, "C"); ///< Necessary for the api to work.
    this->api = new tesseract::TessBaseAPI();
}

Ocr::~Ocr()
{
    delete this->api;
    setlocale (LC_NUMERIC, "");
}

QStringList * Ocr::run()
{
    char *text;
    QString source;
    Pix *imagePix;

    if (!image.exists()) {
        Log::writeError(this->image.fileName() + " does not exist when ocr process was going run.");

        return Q_NULLPTR;
    }

    imagePix = pixRead(this->image.fileName().toStdString().c_str());
    if (!imagePix) {
        Log::writeError(this->image.fileName() + " can't be converted into Pix object.");

        return Q_NULLPTR;
    }

    if (this->initApi()) {
        return Q_NULLPTR;
    }

    this->api->SetPageSegMode(this->pageSegMode);
    this->api->SetImage(imagePix);

    text = this->api->GetUTF8Text(); ///< OCR result.
    source = QString(text);

    ///< Destroy used objects and release memory
    delete []text;
    this->api->End();
    pixDestroy(&imagePix);

    return this->processExtration(source);;
}

QStringList Ocr::getAvailableLanguages()
{
    GenericVector<STRING> languages;
    QStringList availableLanguages;

    if (this->initApi()) {
        return availableLanguages;
    }

    api->GetAvailableLanguagesAsVector(&languages);
    for (int index = 0; index < languages.size(); ++index) {
        availableLanguages << languages[index].string();
    }

    this->api->End();

    return availableLanguages;
}

QStringList Ocr::getLanguages()
{
    return this->language.split('+');
}

bool Ocr::addLanguage(QString language)
{
    if(!this->isAvailableLanguage(language) || this->language.split(QString("+")).contains(language)) {
        return false;
    }

    if (this->language.isEmpty()) {
        this->language = language;
    } else {
        this->language.append("+" + language);
    }

    return true;
}

bool Ocr::removeLanguage(QString language)
{
    int position;

    position = this->language.indexOf(language);
    if (position != -1) {
        this->language.remove(position, language.length()+1);

        return true;
    }

    return false;
}

QString Ocr::getImage()
{
    return this->image.fileName();
}

void Ocr::setImage(QString image)
{
    this->image.setFileName(image);
}

QString Ocr::getDataPath()
{
    return this->datapath;
}

void Ocr::setDataPath(QString datapath)
{
    this->datapath = datapath.endsWith('/') ? datapath : datapath + '/';
}

tesseract::PageSegMode Ocr::getPageSegMode()
{
    return this->pageSegMode;
}

void Ocr::setPageSegMode(tesseract::PageSegMode pageSegMode)
{
    this->pageSegMode = pageSegMode;
}

tesseract::OcrEngineMode Ocr::getEngineMode()
{
    return this->engineMode;
}

void Ocr::setEngineMode(tesseract::OcrEngineMode engineMode)
{
    this->engineMode = engineMode;
}

int Ocr::initApi()
{
    int hasError;

    hasError = api->Init(
        this->datapath.toStdString().c_str(),
        this->language.toStdString().c_str(),
        this->engineMode
    );

    if (hasError) {
        Log::writeError("Could not initialize tesseract.");
    }

    return hasError;
}

QStringList * Ocr::processExtration(QString &source)
{
    QStringList *result = new QStringList();

    if (source.isEmpty()) {
        return result;
    }

    *result << source.split('\n',QString::SkipEmptyParts);

    return result;
}

bool Ocr::isAvailableLanguage(QString &language)
{
    return this->getAvailableLanguages().contains(language);
}
