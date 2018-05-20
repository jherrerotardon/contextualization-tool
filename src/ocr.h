/*!
 * Optical Character Recognition.
 *
 * Abraction class to use tesseract api.
 */
#ifndef OCR_H
#define OCR_H

#include <QString>

class Ocr
{
public:
    Ocr();
    Ocr(QString imagePath);

    QString getImagePath();
    void setImagePath(QString imagePath);
    QString run();

private:
    QString imagePath;

};

#endif // OCR_H
