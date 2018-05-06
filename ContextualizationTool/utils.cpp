#include "utils.h"

Utils::Utils()
{

}

void Utils::errorMessage(const QString text, const QString informativeText)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("Error!!");
    msgBox.setText(text);
    msgBox.setInformativeText(informativeText);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    //msgBox.setStyleSheet("QLabel{min-width: 450px;}");
    msgBox.exec();
}

int Utils::warningMessage(const QString &text, const QString &informativeText)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("Warning!!");
    msgBox.setText(text);
    msgBox.setInformativeText(informativeText);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Critical);
    //msgBox.setStyleSheet("QLabel{min-width: 450px;}");
    return msgBox.exec();
}

int Utils::appendFile(QString &path, QString &text)
{
    QFile file(path);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return -1;

    QTextStream out(&file);
    out << text << '\n';
    file.close();

    return 0;
}


