/**
 * @file ocr.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class Ocr
 *
 * @brief This is an interface for a optical character recognition api.
 */

#include "ocr.h"

Ocr::Ocr()
{
    language_ = "eng"; ///< Default language always "eng".
}

Ocr::~Ocr()
{

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

bool Ocr::isAvailableLanguage(const QString &language)
{
    Q_UNUSED(language);

    return true;
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
