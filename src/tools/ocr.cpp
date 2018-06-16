#include "ocr.h"

Ocr::Ocr()
{
    Ocr(QString(""));
}

Ocr::Ocr(QString image, QString datapath, tesseract::OcrEngineMode engineMode, tesseract::PageSegMode pageSegMode)
{
    image_.setFileName(image);
    datapath_ = datapath;
    engineMode_ = engineMode;
    pageSegMode_ = pageSegMode;
    language_ = "eng"; ///< Default language always "eng".

    setlocale (LC_NUMERIC, "C"); ///< Necessary for the api to work.
    api_ = new tesseract::TessBaseAPI();
}

Ocr::~Ocr()
{
    delete api_;
    setlocale (LC_NUMERIC, "");
}

QStringList * Ocr::run()
{
    char *text;
    QString source;
    Pix *imagePix;

    if (!image_.exists()) {
        Log::writeError(image_.fileName() + " does not exist when ocr process was going run.");

        return Q_NULLPTR;
    }

    imagePix = pixRead(image_.fileName().toStdString().c_str());
    if (!imagePix) {
        Log::writeError(image_.fileName() + " can't be converted into Pix object.");

        return Q_NULLPTR;
    }

    if (this->initApi()) {
        return Q_NULLPTR;
    }

    api_->SetPageSegMode(pageSegMode_);
    api_->SetImage(imagePix);

    text = api_->GetUTF8Text(); ///< OCR result.
    source = QString(text);

    ///< Destroy used objects and release memory
    delete []text;
    api_->End();
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

    api_->GetAvailableLanguagesAsVector(&languages);
    for (int index = 0; index < languages.size(); ++index) {
        availableLanguages << languages[index].string();
    }

    api_->End();

    return availableLanguages;
}

QStringList Ocr::getLanguages()
{
    return language_.split('+');
}

bool Ocr::addLanguage(QString language)
{
    if(!this->isAvailableLanguage(language) || language_.split(QString("+")).contains(language)) {
        return false;
    }

    if (language_.isEmpty()) {
        language_ = language;
    } else {
        language_.append("+" + language);
    }

    return true;
}

bool Ocr::removeLanguage(QString language)
{
    int position;

    position = language_.indexOf(language);
    if (position != -1) {
        language_.remove(position, language.length()+1);

        return true;
    }

    return false;
}

QString Ocr::getImage()
{
    return image_.fileName();
}

void Ocr::setImage(QString image)
{
    image_.setFileName(image);
}

QString Ocr::getDataPath()
{
    return datapath_;
}

void Ocr::setDataPath(QString datapath)
{
    datapath_ = datapath.endsWith('/') ? datapath : datapath + '/';
}

tesseract::PageSegMode Ocr::getPageSegMode()
{
    return pageSegMode_;
}

void Ocr::setPageSegMode(tesseract::PageSegMode pageSegMode)
{
    pageSegMode_ = pageSegMode;
}

tesseract::OcrEngineMode Ocr::getEngineMode()
{
    return engineMode_;
}

void Ocr::setEngineMode(tesseract::OcrEngineMode engineMode)
{
    engineMode_ = engineMode;
}

int Ocr::initApi()
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
