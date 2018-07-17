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
    msgBox.setIcon(QMessageBox::Warning);
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

bool Utils::appendFile(const QString &path, const QString &text)
{
    QFile file(path);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << text << '\n';
    file.close();
    return true;
}

bool Utils::writeFile(const QString &path, const QString &text)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out << text << '\n';
    file.close();

    return true;
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

int Utils::executeProgram(const QString &program,
        const QStringList &arguments,
        const QString &standardOutput,
        const QString &workingDirectory,
        const int timeout
) {
    QProcess *process;
    int codeError;

    process = new QProcess();
    process->setStandardOutputFile(standardOutput.isEmpty() ? QProcess::nullDevice() : standardOutput);
    process->setStandardErrorFile(standardOutput.isEmpty() ? QProcess::nullDevice() : standardOutput);
    if (!workingDirectory.isEmpty()) {
        process->setWorkingDirectory(workingDirectory);
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
    int hasError;
    QStringList arguments;
    QString zipFile(QDir(zipDestination).absoluteFilePath(zipName + ".zip"));

    arguments << "-r" << zipFile << ".";
    hasError = Utils::executeProgram("zip", arguments, QProcess::nullDevice(), directory, 10000);
    return hasError ? QString() : zipFile;
}

QFuture<void> Utils::startProgressDialogCounter(QProgressDialog *dialog, bool *hasFinished, int timeout)
{
    for (int i = 0; i < 5; i++) {
        dialog->setValue(i);
        QThread::msleep(60);
    }

    // Run progress increment thread.
    QFuture<void> future = QtConcurrent::run(
        [dialog, hasFinished, timeout]() {
            for (int i = 5; i < 100; i++) {
                dialog->setValue(i);
                if (*hasFinished) {
                    break;
                }

                QThread::msleep(timeout);
            }
        }
    );

    return future;
}

bool Utils::isValidIp(const QString &ip)
{
    QRegularExpression regex("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    QRegularExpressionMatch match = regex.match(ip);

    return match.hasMatch();
}
