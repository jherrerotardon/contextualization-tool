#include "utils.h"

Utils::Utils()
{

}

void Utils::errorMessage(const QString &text, const QString &informativeText)
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

void Utils::informativeMessage(const QString &text, const QString &informativeText)
{
    QMessageBox msgBox;

    msgBox.setWindowTitle("Information!!");
    msgBox.setText(text);
    msgBox.setInformativeText(informativeText);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Information);
    //msgBox.setStyleSheet("QLabel{min-width: 450px;}");
    msgBox.exec();
}

int Utils::appendFile(const QString &path, const QString &text)
{
    QFile file(path);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return -1;
    }

    QTextStream out(&file);
    out << text << '\n';
    file.close();
    return 0;
}

int Utils::writeFile(const QString &path, const QString &text)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return -1;
    }

    QTextStream out(&file);
    out << text << '\n';
    file.close();

    return 0;
}

QByteArray Utils::readAllFile(const QString &path)
{
    QFile file(path);
    QByteArray out;

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return QByteArray();
    }

    out = file.readAll();
    file.close();

    return out;
}

int Utils::executeProgram(
        const QString &program,
        const QStringList &arguments,
        const QString &workDirectory,
        const int timeout
) {
    QProcess *process;
    int codeError;

    process = new QProcess();
    if (!workDirectory.isEmpty()) {
        process->setWorkingDirectory(workDirectory);
    }

    process->start(program, arguments);
    process->waitForFinished(timeout);

    codeError = process->exitStatus() == QProcess::NormalExit ? process->exitCode() : -1;

    delete process;

    return codeError;
}

QString Utils::zipCompressDirectoryContents(
    const QString &directory,
    const QString &zipDestination,
    const QString &zipName
) {
    QStringList arguments;
    QString zipFile(QDir(zipDestination).absoluteFilePath(zipName + ".zip"));

    arguments << "-r" << zipFile << ".";
    return Utils::executeProgram("zip", arguments, directory, 10000) ? QString("") : zipFile;
}

