#include "log.h"

const string Log::debugFile = "/home/jorge/Downloads/debug.log";

Log::Log()
{

}

void Log::writeDebug(string text)
{
    ofstream fichero;

    fichero.open(debugFile.c_str(), ios::out | ios::app);

    if (fichero.is_open()) {
        fichero << text << '\n';

        fichero.close();
    }
    else {
        cout << "Error en apertura de: " << debugFile;
    }
}

void Log::writeLog(QString text)
{
    QFile file("out.txt");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << text << "\n";
}
