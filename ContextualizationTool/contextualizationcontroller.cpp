#include "contextualizationcontroller.h"
/************/
#include <QDebug>
/***************/

ContextualizationController::ContextualizationController(QObject *view, QObject *parent) : QObject(parent)
{
    QObject *stringsTable;

    this->fpFile = "/home/jorge/Downloads/english.fp";
    this->username = qgetenv("USER");
    this->view = view;

    if (view != nullptr)
    {
        //Initialize TableView and his model.
        this->tableModel = new StringsTableModel(this->model.getStringsList());
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
    QFile file(this->model.getImagePath());
    if (!file.exists())
    {
        //Image not exists
        return 1;
    }

    if (this->model.getStringsList().size() <= 0)
    {
        //Error, there isn't strings in the model.
        return 2;
    }

    //All OK
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

    ((QWindow *)view)->setVisible(false);

    capture = new QProcess();
    capture->start("import", arguments);
    if (capture->waitForFinished(30000))
    {
        image = view->findChild<QObject *>("captureImage");
        image->setProperty("source", "");
        image->setProperty("source", "file:" + path);
        this->model.setImagePath(path);
    }
    else
    {
        //Pasaron 30 segs si que usuario haga captura
    }
    delete capture;

    ((QWindow *)view)->setVisible(true);

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
            image = view->findChild<QObject *>("captureImage");
            image->setProperty("source", "");
            image->setProperty("source", "file:" + fileNames.first());
            this->model.setImagePath(fileNames.first());
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
    QString message;
    QFile file(fpFile);
    FirmwareString *fwString;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while (!file.atEnd())
        {
            line = in.readLine();
            count ++;

            fwString = this->fragmentFpLine(line, count);
            if (fwString != nullptr)
            {
                if (newString == fwString->getValue())
                {
                    this->model.addNewString(fwString);
                    this->tableModel->insertRows(this->tableModel->rowCount()-1, 1);
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
                //                Utils::errorMessage(QString(fpFile + " format is not correctly."),
                //                                    QString("Format error on line " + QString(std::to_string(count).c_str())));
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
                this->model.addNewString(fwString);
                this->tableModel->insertRows(tableModel->rowCount()-1, 1);
            }
        }
    }
    else
    {
        message = " Fail to open file: " + fpFile;
        Log::writeError(message);
    }
}

void ContextualizationController::deleteString(int row)
{
    this->model.deleteString(row);
    tableModel->removeRows(row, 1);
}

void ContextualizationController::detectStringsOnImage()
{

}

void ContextualizationController::clearTable()
{
    tableModel->removeRows(0, tableModel->rowCount());
}

void ContextualizationController::send()
{
    //    foreach (FirmwareString * a, this->model.getStringsList()) {
    //        qDebug() << a->getValue().toStdString().c_str() << "   ******** ";
    //    }
}

void ContextualizationController::cancel()
{
    int response;

    response = Utils::warningMessage("Are you sure?", "If you not save the proyect it will be deleted.");
    if (response == QMessageBox::Yes)
    {
        //Remove temporal image.
        if (!this->model.getImagePath().isEmpty())
        {
            QFile file(this->model.getImagePath());
            if (file.exists())
                file.remove();
        }

        QApplication::quit();
    }
}

FirmwareString * ContextualizationController::fragmentFpLine(QString &line, int lineNumber)
{
    QStringList list;
    QStringList subListIdText;
    QStringList subListDescription;
    QStringList subListMaxWidth;
    QStringList subListLocalization;
    QString message;
    FirmwareString *fwString;
    int maxLenght;
    bool selected;
    bool ok;

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

                        maxLenght = ((QString)subListMaxWidth.at(1)).toInt(&ok, 10);
                        if (ok)
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
                                message = " englis.fp: String state not valid on line " + QString::number(lineNumber) + ".";
                                Log::writeError(message);
                                return nullptr;
                            }
                        }
                        else
                        {
                            message = "englis.fp: MaxWidth couldn't be converted to int on line " + QString::number(lineNumber) + ".";
                            Log::writeError(message);
                            return nullptr;
                        }
                    }
                    else
                    {
                        message = "englis.fp: Error format in LOCALIZATION column on line " + QString::number(lineNumber) + ".";
                        Log::writeError(message);
                        return nullptr;
                    }
                }
                else
                {
                    message = "englis.fp: Error format in MAX_FIELD_WIDTH column on line " + QString::number(lineNumber) + ".";
                    Log::writeError(message);
                    return nullptr;
                }
            }
            else
            {
                message = "englis.fp: Error format in TEXT_DESCRIPTION column on line " + QString::number(lineNumber) + ".";
                Log::writeError(message);
                return nullptr;
            }
        }
        else
        {
            message = "englis.fp: Error format in MESSAGE_ID column on line " + QString::number(lineNumber) + ".";
            Log::writeError(message);
            return nullptr;
        }
    }
    else
    {   message = "englis.fp: Error format by splitting the string in columns by the separator '||' on line " + QString::number(lineNumber) + ".";
        Log::writeError(message);
        return nullptr;
    }
}
