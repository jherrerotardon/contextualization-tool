#include "ocr.h"

Ocr::Ocr()
{
    this->languages = "eng";
}

Ocr::Ocr(QString image, QString language, QString datapath)
{
    this->image.setFileName(image);
    this->languages = language;
    this->datapath = datapath;
    this->pageSegMode = tesseract::PSM_AUTO;
    this->engineMode = tesseract::OEM_TESSERACT_CUBE_COMBINED;
}

Ocr::Ocr(QString image, QStringList languages, QString datapath)
{
    QString language;

    foreach (QString lang, languages) {
        language += "+" + lang;
    }

    language.remove(0, 1); ///< Remove the first '+'.
    Ocr(image, language.isEmpty() ? "eng" : language, datapath);

}

QStringList * Ocr::run()
{
//    char *text;
//    QString source;
//    QStringList *out;
//    Pix *imagePix;

    if (!image.exists()) {
        Log::writeError(this->image.fileName() + " does not exist when ocr process was going run.");
        return nullptr;
    }
//    imagePix = pixRead(this->image.fileName().toStdString().c_str());

//    if (this->initApi()) {
//        Log::writeError("Could not initialize tesseract.");
//        return nullptr;
//    }

//    this->api->SetPageSegMode(this->pageSegMode);
//    this->api->SetImage(imagePix);

//    text = this->api->GetUTF8Text(); ///< OCR result.
//    source = text;
//    delete [] text; ///< Release memory of the extraction.
//    out = this->processExtration(source);

//    // Destroy used object and release memory
//    this->api->End();
//    pixDestroy(&imagePix);

//    return out;

    QProcess *ocrProcess;
    QStringList arguments;

    ocrProcess = new QProcess();
    arguments << image.fileName();
    ocrProcess->start("/home/jorge/Projects/contextualization-tool/build-OcrCustom-Desktop_Qt_5_10_0_GCC_64bit-Debug/OcrCustom", arguments);
    ocrProcess->waitForFinished(30000);
    QString output(ocrProcess->readAllStandardOutput());

    delete ocrProcess;

    return new QStringList(output.split('\n'));
}

//QStringList Ocr::getAvailableLanguages()
//{
//    GenericVector<STRING> languages;
//    QStringList availableLanguages;

//    if (this->initApi()) {
//        Log::writeError("Could not initialize tesseract.");
//        return availableLanguages;
//    }

//    api->GetAvailableLanguagesAsVector(&languages);
//    for (int index = 0; index < languages.size(); ++index) {
//        availableLanguages << languages[index].string();
//    }

//    this->api->End();
//    return availableLanguages;
//}

//QStringList Ocr::getLanguages()
//{
//    return this->languages.split('+');
//}

//bool Ocr::addLanguage(QString language)
//{
//    if(!this->isAvailableLanguage(language) || this->languages.split(QString("+")).contains(language)) {
//        return false;
//    }

//    this->languages = this->languages + "+" + language;
//    return true;
//}

//bool Ocr::removeLanguage(QString language)
//{
//    int position;

//    position = this->languages.indexOf(language);
//    if (position != -1) {
//        this->languages.remove(position, language.length()+1);
//        return true;
//    }

//    return false;
//}

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

//void Ocr::setDataPath(QString datapath)
//{
//    this->datapath = datapath;
//}

//tesseract::PageSegMode Ocr::getPageSegMode()
//{
//    return this->pageSegMode;
//}

//void Ocr::setPageSegMode(tesseract::PageSegMode pageSegMode)
//{
//    this->pageSegMode = pageSegMode;
//}

//tesseract::OcrEngineMode Ocr::getEngineMode()
//{
//    return this->engineMode;
//}

//void Ocr::setEngineMode(tesseract::OcrEngineMode engineMode)
//{
//    this->engineMode = engineMode;
//}

//int Ocr::initApi()
//{
//    int a = api->Init(
//                "/home/jorge/Projects/contextualization-tool/tesseract/tessdata/",
//                "eng",
//                tesseract::OEM_TESSERACT_CUBE_COMBINED
//    );
//    return a;
////    return api->Init(
////        "/home/jorge/Projects/contextualization-tool/tesseract/tessdata",//this->datapath.isNull() ? NULL : this->datapath.toStdString().c_str(),
////        //this->languages.toStdString().c_str(),
////        "eng",
////        tesseract::OEM_TESSERACT_ONLY
////        //this->engineMode
////    );
//}

//QStringList * Ocr::processExtration(QString &source)
//{
//    QStringList *result = new QStringList();

//    if (source.isEmpty()) {
//        return result;
//    }

//    // TODO: procesar las cadenas para sacar los textos lo mejor posible.
//    *result << source.split('\n');

//    return result;
//}

//bool Ocr::isAvailableLanguage(QString &language)
//{
//    return this->getAvailableLanguages().contains(language);
//}
