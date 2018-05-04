#include "contextualizationcontroller.h"
/************/
#include <QDebug>
/***************/

ContextualizationController::ContextualizationController(QObject *view, QObject *parent) : QObject(parent)
{
    QObject *stringsTable;

    fpFile = "/home/jorge/Downloads/aa.fp";
    this->username = qgetenv("USER");
    this->view = view;

    if (view != nullptr)
    {
        //Initialize TableView and his model.
        this->tableModel = new StringsTableModel();
        stringsTable = view->findChild<QObject *>("stringsTable");
        if (stringsTable)
            stringsTable->setProperty("model", QVariant::fromValue(this->tableModel));


        //Connect signals and slots
        QObject::connect(view->findChild<QObject *>("clearButton"), SIGNAL(clicked()),
                         this, SLOT(clearTable()));
        QObject::connect(view->findChild<QObject *>("addStringButton"), SIGNAL(customClicked(QString)),
                         this, SLOT(addString(QString)));
        QObject::connect(view->findChild<QObject *>("buttonsColumn"), SIGNAL(buttonClicked(int)),
                         this, SLOT(deleteString(int)));
        QObject::connect(view->findChild<QObject *>("cancelButton"), SIGNAL(clicked()),
                         this, SLOT(cancel()));
        QObject::connect(view->findChild<QObject *>("sendButton"), SIGNAL(clicked()),
                         this, SLOT(send()));
        QObject::connect(view->findChild<QObject *>("captureAreaButon"), SIGNAL(clicked()),
                         this, SLOT(captureArea()));
        QObject::connect(view->findChild<QObject *>("loadImageButton"), SIGNAL(clicked()),
                         this, SLOT(loadImage()));
    }
}

ContextualizationController::~ContextualizationController()
{
    delete tableModel;
}

StringsTableModel *ContextualizationController::getTableModel()
{
    return tableModel;
}

void ContextualizationController::setTableModel(StringsTableModel *tableModel)
{
    this->tableModel = tableModel;
}

int ContextualizationController::validateModel()
{

    return 0;
}

int ContextualizationController::generatePackage(const QString &path)
{

    return 0;
}

void ContextualizationController::captureArea()
{
    QProcess *capture;
    QStringList arguments;
    QObject *image;
    QString path("/tmp/capture.png");

    arguments << "-quality" << "9" << path;

    capture = new QProcess();
    capture->start("import", arguments);
    if (capture->waitForFinished(30000))
    {
        image = view->findChild<QObject *>("captureImage");
        image->setProperty("source", "");
        image->setProperty("source", "file:/tmp/capture.png");
    }
    else
    {
        //Pasaron 30 segs si que usuario haga captura
        qDebug() << "TIMEOUT";
    }
    delete capture;

}

void ContextualizationController::loadImage()
{
    /*
     * La segunda vez se abre la ventana detrás de la principal.
     * Cuando está abierto en FileDialog, deja seleccionar cosas de la ventana principal
     * y se ejecutan cuando se cierra el FileDialog.
     */

    QStringList fileNames;
    QObject *image;
    QFileDialog dialog(Q_NULLPTR,tr("Open Image"), QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first(),
                       tr("Images (*.png *.xpm *.jpg)"));

    dialog.setViewMode(QFileDialog::Detail);

    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();

        if (!fileNames.isEmpty()){
            qDebug() << "NO ES NULL\n" << fileNames.first();
            image = view->findChild<QObject *>("captureImage");
            image->setProperty("source", "");
            image->setProperty("source", "file:" + fileNames.first());
        }
    }
}

void ContextualizationController::addString(QString newString)
{
    int count = 0;
    int response;
    bool stringFound = false;
    QString emptyString("");
    QString state;
    QString line;
    QFile file(fpFile);
    FirmwareString *fwString;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while (!file.atEnd())
        {
            line = in.readLine();
            count ++;

            fwString = this->fragmentFpLine(line);
            if (fwString != nullptr)
            {
                if (newString == fwString->getValue())
                {
                    tableModel->insertRow(tableModel->rowCount(), fwString);
                    stringFound = true;
                    break; //Stop to read file
                }
                else
                {
                    delete fwString;
                }
            }
            else
            {
                //Mensaje en el log de error
                Utils::errorMessage(QString(fpFile + " format is not correctly."),
                                    QString("Format error on line " + QString(std::to_string(count).c_str())));
            }
        }
        file.close();

        if (stringFound == false)
        {
            response = Utils::warningMessage("String isn't in english.fp file.", "Are you sure to add the string?");
            if (response == QMessageBox::Yes)
            {
                state = "TODO";
                fwString = new FirmwareString(emptyString, newString, emptyString, newString.size(), state, false);
                tableModel->insertRow(tableModel->rowCount(), fwString);
            }
        }
    }
    else
    {
        //Error log
        qDebug() << "Fail to open file: " << fpFile.toStdString().c_str();
    }

}

void ContextualizationController::deleteString(int row)
{
    tableModel->removeRows(row, 1);
}

void ContextualizationController::detectStringsOnImage()
{

}

void ContextualizationController::clearTable()
{

    qDebug() << "Boton clear";

    tableModel->removeRows(0, tableModel->rowCount());

}

void ContextualizationController::send()
{
    qDebug() << QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");
}

void ContextualizationController::cancel()
{

    int response;

    response = Utils::warningMessage("Are you sure?", "If you not save the proyect it will be deleted.");
    if (response == QMessageBox::Yes)
    {
        QApplication::quit();
    }

}

FirmwareString * ContextualizationController::fragmentFpLine(QString &line)
{
    QStringList list;
    QStringList subListIdText;
    QStringList subListDescription;
    QStringList subListMaxWidth;
    QStringList subListLocalization;
    FirmwareString *fwString;
    int maxLenght;
    bool selected;

    //Fragment fp line.
    list = line.split("||");
    if (list.size() == 4)
    {
        subListIdText = list.at(0).split(' ', QString::SkipEmptyParts).mid(0, 2);
        subListIdText << list.at(0).section('\"', 1, 1);
        if (subListIdText.size() == 3)
        {
            subListDescription = list.at(1).split(' ', QString::SkipEmptyParts).mid(0, 1);
            subListDescription << list.at(1).section('\"', 1, 1);
            if (subListDescription.size() == 2)
            {
                //((QString &) subListDescription.at(1)).replace(QString("\""), QString("")); //Remove quotes on text description

                subListMaxWidth = list.at(2).split(' ', QString::SkipEmptyParts);
                if (subListMaxWidth.size() == 2)
                {
                    subListLocalization = list.at(3).split(' ', QString::SkipEmptyParts);
                    if (subListLocalization.size() == 2)
                    {

                        maxLenght = ((QString)subListMaxWidth.at(1)).toInt();
                        if (maxLenght != 0)
                        {

                            QString &state = (QString &)subListLocalization.at(1);
                            if (state == "TODO" || state == "DONE" || state == "VALIDATED")
                            {
                                selected = (state == "TODO") ? true : false;
                                fwString = new FirmwareString((QString &)subListIdText.at(1), (QString &)subListIdText.at(2), (QString &)subListDescription.at(1),
                                                              maxLenght, (QString &)subListLocalization.at(1), selected);
                                return fwString;
                            }
                            else
                            {
                                //Error estado no valido
                                return NULL;
                            }
                        }
                        else
                        {
                            //Error en maxWidth number conversion
                            return NULL;
                        }
                    }
                    else
                    {
                        //Error en localization format
                        return NULL;
                    }
                }
                else
                {
                    //Error en maxWidth format
                    return NULL;
                }
            }
            else
            {
                //Error en description format
                return NULL;
            }
        }
        else
        {
            //Error en id-text format
            return NULL;
        }
    }
    else
    {   //Error en ||
        return NULL;
    }
}
