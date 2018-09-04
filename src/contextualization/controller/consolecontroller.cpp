/**
 * @file consolecontroller.cpp
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class ConsoleController
 *
 * @brief This is the controller class that works a CLI environment.
 */

#include "consolecontroller.h"

ConsoleController::ConsoleController()
{
    // Set default values.
    appName_ = QString();
    action_ = NoAction;
    findType_ = ByID;
    model_ = new ContextualizationModel();
}

ConsoleController::ConsoleController(int argc, char **argv)
    : ContextualizationController()
{
    // Set default values.
    appName_ = argc > 1 && argv != Q_NULLPTR ? QString(argv[0]) : QString();
    action_ = NoAction;
    findType_ = ByID;

    // Decode arguments to know what to do.
    decodeArguments(argc, argv);

    loadProjectFromConfiguration();
}

void ConsoleController::exec()
{
    switch (action_) {
    case PrintHelp:
        printUsage();
        break;
    case ListProjects:
        listProjects();
        break;
    case DetailProject:
        detailCurrentProject();
        break;
    case NewProject:
        newProject();
        break;
    case DeleteProject:
        deleteProject();
        break;
    case SelectProject:
        open();
        break;
    case ClearStrings:
        clear();
        break;
    case ClearImage:
        clearImage();
        break;
    case ClearAll:
        clearAll();
        break;
    case AddString:
        add(parameter_.toString(), findType_);
        break;
    case RemoveString:
        remove(parameter_.toString());
        break;
    case SelectString:
        select(parameter_.toString());
        break;
    case UnselectString:
        unselect(parameter_.toString());
        break;
    case SetImage:
        load();
        break;
    case CaptureArea:
        capture();
        break;
    case DetectStrings:
        detect();
        break;
    case ExportProject:
        save();
        break;
    case ImportProject:
        open();
        break;
    case Send:
        send();
        break;
    case PrintClearHelp:
        printClearDetails();
        break;
    case PrintAddHelp:
        printAddDetails();
        break;
    case PrintDetectHelp:
        printDetectDetails();
        break;
    case PrintImageHelp:
        printImageDetails();
        break;
    default:
        std::cout << Utils::formatText(
            "Nothing to do.",
             QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
        break;
    }
}

bool ConsoleController::decodeArguments(int argc, char **argv)
{    
    if (argc == 0 || argv == Q_NULLPTR) {
        return false;
    }

    // Cases with dynamic number of arguments.
    if (argc >= 3) {
        if (QString::compare(argv[1], "delete") == 0 && QString::compare(argv[2], "project") == 0) {
           setAction(DeleteProject, argc == 4 ? QVariant(argv[3]) : QVariant());
        } else if (QString::compare(argv[1], "add") == 0) {
            for (int i = 2; i < argc; ++i) {
                if (i == argc-1 && QString::compare(argv[i], "--help") == 0) {
                    setAction(PrintAddHelp);
                } else if (i == argc-1) { // The last parameter must be the string to search.
                   setAction(AddString, QVariant(argv[i]));
                } else if (QString::compare(argv[i], "--id") == 0) {
                    findType_ = ByID;
                } else if (QString::compare(argv[i], "--value") == 0) {
                    findType_ = ByValue;
                } else if (QString::compare(argv[i], "--case-sensitive") == 0) {
                    caseSensitive_ = true;
                } else if (QString::compare(argv[i], "--case-insensitive") == 0) {
                    caseSensitive_ = false;
                } else if (QString::compare(argv[i], "--only-done-strings") == 0) {
                    onlyDoneStrings_ = true;
                } else if (QString::compare(argv[i], "--all-states") == 0) {
                    onlyDoneStrings_ = false;
                } else {
                    setAction(PrintAddHelp);
                }
            }
        }
    }

    if (argc >= 2) {
        if (QString::compare(argv[1], "detect") == 0) {
            setAction(DetectStrings);
            for (int i = 2; i < argc; ++i) {
                if (i == argc-1 && QString::compare(argv[i], "--help") == 0) {
                    setAction(PrintDetectHelp);
                } else if (QString::compare(argv[i], "--case-sensitive") == 0) {
                    caseSensitive_ = true;
                } else if (QString::compare(argv[i], "--case-insensitive") == 0) {
                    caseSensitive_ = false;
                } else if (QString::compare(argv[i], "--only-done-strings") == 0) {
                    onlyDoneStrings_ = true;
                } else if (QString::compare(argv[i], "--all-states") == 0) {
                    onlyDoneStrings_ = false;
                } else {
                    setAction(PrintAddHelp);
                }
            }
        }
    }

    // Cases with constant number of arguments.
    switch (argc) {
    case 2:
        if (QString::compare(argv[1], "-h") == 0 || QString::compare(argv[1], "--help") == 0) {
            setAction(PrintHelp);
        } else if (QString::compare(argv[1], "list") == 0) {
            setAction(ListProjects);
        } else if (QString::compare(argv[1], "detail") == 0) {
            setAction(DetailProject);
        } else if (QString::compare(argv[1], "add") == 0) {
            setAction(PrintAddHelp);
        }

        break;

    case 3:
        if (QString::compare(argv[1], "open") == 0) {
            setAction(SelectProject, QVariant(argv[2]));
        } else if (QString::compare(argv[1], "select") == 0) {
            setAction(SelectString, QVariant(argv[2]));
        } else if (QString::compare(argv[1], "unselect") == 0) {
            setAction(UnselectString, QVariant(argv[2]));
        } else if (QString::compare(argv[1], "clear") == 0) {
            if (QString::compare(argv[2], "strings") == 0) {
                setAction(ClearStrings);
            } else if (QString::compare(argv[2], "image") == 0) {
                setAction(ClearImage);
            } else if (QString::compare(argv[2], "all") == 0) {
                setAction(ClearAll);
            } else {
                setAction(PrintClearHelp);
            }
        } else if (QString::compare(argv[1], "new") == 0) {
            setAction(NewProject, QVariant(argv[2]));
        } else if (QString::compare(argv[1], "remove") == 0) {
            setAction(RemoveString, QVariant(argv[2]));
        } else if (QString::compare(argv[1], "image") == 0) {
            if (QString::compare(argv[2], "-c") == 0 || QString::compare(argv[2], "--capture") == 0) {
                setAction(CaptureArea);
            } else if (QString::compare(argv[1], "--help") == 0) {
                setAction(PrintImageHelp);
            } else {
                setAction(SetImage, QVariant(argv[2]));
            }
        } else if (QString::compare(argv[1], "export") == 0) {
            setAction(ExportProject, QVariant(argv[2]));
        } else if (QString::compare(argv[1], "import") == 0) {
            setAction(ImportProject, QVariant(argv[2]));
        } else if (QString::compare(argv[1], "send") == 0) {
            setAction(Send, QVariant(argv[2]));
        }

        break;

    default:
        break;
    }

    if (action_ == NoAction) {
        setAction(PrintHelp);
        return true;
    }

    return false;
}

void ConsoleController::setAction(ActionType action, QVariant parameter)
{
    action_ = action;
    parameter_ = parameter;
}

void ConsoleController::printUsage()
{
    const QString help =
        "Usage:\n"
        "  " + appName_ + " -h | --help             Show minimal help message.\n"
        "  " + appName_ + " list                    List available projects.\n"
        "  " + appName_ + " detail                  Show details of active project.\n"
        "  " + appName_ + " new <name>              Create a new empty project.\n"
        "  " + appName_ + " delete project [name]   Delete current project or project \"name\".\n"
        "  " + appName_ + " clear <option>          Clear selected elements in active project.\n"
        "  " + appName_ + " open <project>          Open a project and select it as the active project.\n"
        "  " + appName_ + " add [option] <string>   Add new string on active project. Use --help to see options\n"
        "  " + appName_ + " remove <string ID>      Remove from the active project the string with the input ID.\n"
        "  " + appName_ + " image <option>          Sets an image in active project. Use --help to see options.\n"
        "  " + appName_ + " detect <options>        Extract strings from image in active project and add them. Use --help to see options.\n"
        "  " + appName_ + " export <path>           Saves the active project in the input path.\n"
        "  " + appName_ + " import <path>           Import a project from the input path.\n"
        "  " + appName_ + " send [user]@hostname    Send the active project to input host.\n"
        ;

    std::cout << help.toStdString() << std::endl;
}

void ConsoleController::printClearDetails()
{
    const QString help =
        appName_ + " clear <option>\n\n"
        "OPTIONS:\n"
        "  strings              Clear strings list of the active project.\n"
        "  image                Clear image of the active project.\n"
        "  all                  Completely empty the model."
        ;

    std::cout << help.toStdString() << std::endl;
}

void ConsoleController::printAddDetails()
{
    const QString help =
        appName_ + " add <option>\n\n"
        "OPTIONS:\n"
        "  --id                 The input string will be searched as an identifier of string.\n"
        "  --value              The input string will be searched as a value of string.\n"
        "  --case-sensitive     The search will be case sensitive.\n"
        "  --case-insensitive   The search will not be case sensitive.\n"
        "  --all-states         Tool will find strings with all valid states.\n"
        "  --only-done-strings  Tool will only find string with DONE state.\n\n\n"
        "By default, searches will be by identifier, case sensitive and only find strings with DONE state."
        ;

    std::cout << help.toStdString() << std::endl;
}

void ConsoleController::printDetectDetails()
{
    const QString help =
        appName_ + " detect <option>\n\n"
        "OPTIONS:\n"
        "  --id                 The input string will be searched as an identifier of string.\n"
        "  --value              The input string will be searched as a value of string.\n"
        "  --case-sensitive     The search will be case sensitive.\n"
        "  --case-insensitive   The search will not be case sensitive.\n"
        "  --all-states         Tool will find strings with all valid states.\n"
        "  --only-done-strings  Tool will only find string with DONE state.\n\n\n"
        "By default, searches will be by identifier, case sensitive and only find strings with DONE state."
        ;

    std::cout << help.toStdString() << std::endl;
}


void ConsoleController::printImageDetails()
{
    const QString help =
        appName_ + " image <option>\n\n"
        "OPTIONS:\n"
        "  -c, --capture     Launch a process where the user have to capture a screen area to be set as model image.\n"
        "  <path>            Sets the input image path as image model if exists.\n"
        ;

    std::cout << help.toStdString() << std::endl;
}

void ConsoleController::printDetectOptions()
{
    const QString help =
        appName_ + " add <option>\n\n"
        "OPTIONS:\n"
        "  --case-sensitive     The search will be case sensitive.\n"
        "  --case-insensitive   The search will not be case sensitive.\n"
        "  --all-states         Tool will find strings with all valid states.\n"
        "  --only-done-strings  Tool will only find string with DONE state.\n\n\n"
        "By default, searchs will be case sensitive and only find strings with DONE state."
        ;

    std::cout << help.toStdString() << std::endl;
}

void ConsoleController::add(QString newString, int findType)
{
    QList <FirmwareString *> stringsFound;
    int count;

    stringsFound = findString(newString, static_cast<MatchType>(findType));

    count = addStrings(stringsFound);

    if (save()) {
        std::cout << "Added " << count << " string into project." << std::endl;
    } else {
        std::cout << Utils::formatText(
            "There was a problem saving the project.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

void ConsoleController::remove(QString stringId)
{
    if (removeString(stringId)) {
        std::cout << "The string '" << stringId.toStdString() << "' has been removed succesfully." << std::endl;

        if (!save()) {
            std::cout << Utils::formatText(
                "There was a problem saving the project.",
                QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
        }
    } else {
        std::cout << "There are not any string with the indentifier '" << stringId.toStdString() << "'." << std::endl;
    }
}

void ConsoleController::clear()
{
    bool done;

    done = removeAllStrings();

    if (done) {        
        if (save()) {
            std::cout << "All string removed succesfully." << std::endl;
        } else {
            std::cout << Utils::formatText(
                "There was a problem saving the project.",
                QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
        }
    } else {
        std::cout << "There are not strings to be removed." << std::endl;
    }
}

void ConsoleController::capture(bool detectStringsOnLoad)
{
    Q_UNUSED(detectStringsOnLoad); // In CLI mode is not util.

    QString path;

    if (!projectPath_.isEmpty()) {
        path = takeCaptureArea();

        if (setImage(path)) {
            if (save()) {
                std::cout << "Capture set succesfully." << std::endl;
            } else{
                std::cout << Utils::formatText(
                    "There was a problem saving the project.",
                    QList<Utils::TextModifier>() << Utils::FG_RED
                ).toStdString() << std::endl;
            }
        } else {
            std::cout << Utils::formatText(
                "There was a problem with a capture.",
                QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
        }
    }
}

void ConsoleController::load(bool detectStringsOnLoad)
{
    Q_UNUSED(detectStringsOnLoad); // In CLI mode is not util.

    if(!setImage(parameter_.toString())) {
        std::cout << Utils::formatText(
            "Image not exists.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }

    if (save()) {
        std::cout << "Image set succesfully." << std::endl;
    } else{
        std::cout << Utils::formatText(
            "There was a problem saving the project.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

void ConsoleController::detect()
{
    QList<FirmwareString *> extractedStrings;
    QList<FirmwareString *> copy;
    int count;

    if (!model_->hasImage()) {
        std::cout << Utils::formatText(
            "Not image. You have to set an image to can detect strings in it.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;

        return;
    }
    
    std::cout << Utils::formatText(
        "Detecting...",
        QList<Utils::TextModifier>() << Utils::FG_GREEN
    ).toStdString() << std::endl;

    // Extract strings on image.
    extractedStrings = detectStringsOnImage(model_->getImage());

    // A copy if creates because extracted strings are in a different thread and this is in conflict with Q_PROPERTYs.
    foreach (FirmwareString *fwString, extractedStrings) {
        copy << new FirmwareString(*fwString);

        delete fwString;
        fwString = Q_NULLPTR;
    }

    count = addStrings(copy);

    if (save()) {
        std::cout << count << " strings added into the contextualization." << std::endl;
    } else {
        std::cout << Utils::formatText(
            "There was a problem saving the project.",
             QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

void ConsoleController::send()
{
    QString contextualizationPath;
    QString username;
    QString password;
    int hasError;

    switch (validateModel()) {
        case OkModel:
            contextualizationPath = generateContextualization();
            if (contextualizationPath.isEmpty()) {
                std::cout << Utils::formatText(
                    "Fail to send. Failure to package contextualization.",
                    QList<Utils::TextModifier>() << Utils::FG_RED
                ).toStdString() << std::endl;

                return;
            }

            username = requestUsername();
            // If user press cancel, remove contextualization file and cancel process.
            if (username.isEmpty()) {
                QFile::remove(contextualizationPath);

                return;
            }

            password = requestPassword();
            // If user press cancel, remove contextualization file and cancel process.
            if (password.isEmpty()) {
                QFile::remove(contextualizationPath);

                return;
            }

            std::cout << Utils::formatText(
                "Sending...",
                QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;

            // Send contextualization
            hasError = sendContextualization(contextualizationPath, username, password);

            // Remove contextualization file.
            QFile::remove(contextualizationPath);

            if (hasError) {
                std::cout << Utils::formatText(
                    "Finished. Send process has finished with some errors. See log for more information.",
                    QList<Utils::TextModifier>() << Utils::FG_RED
                ).toStdString() << std::endl;
            } else {
                std::cout << "Finished. Send process has finished succesfully." << std::endl;
            }

            break;
        case NoImage:
            std::cout << Utils::formatText(
                "Fail to send!! There isn't an image asociated.",
                 QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
            break;
        case ImageNotExist:
            std::cout << Utils::formatText(
                 "Fail to send!! The image associated doesn't exist. Please reload the image.",
                 QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
            break;
        case NoStrings:
            std::cout << Utils::formatText(
                 "Fail to send!! There aren't any string asociated.",
                 QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
            break;
        default:
            std::cout << Utils::formatText(
                "Fail to send!! Unknown error.",
                QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
            break;
    }
}

void ConsoleController::cancel()
{
    // Not util in console mode.
}

bool ConsoleController::save()
{
    if (!projectPath_.isEmpty()) {
        return exportToJsonFile(projectPath_);
    } else {
        Log::writeError(QString(Q_FUNC_INFO) + " There is not a project to be saved.");
        return false;
    }
}

bool ConsoleController::saveAs()
{
    // Is not an option in console mode.
    return false;
}

void ConsoleController::open()
{
    QString projectName = parameter_.toString();

    if (QFile::exists(PROJECTS_FOLDER + projectName)) {
        if (setParameterInConfigFile("selected_project", projectName)) {
            std::cout << Utils::formatText(
                "Project has been selected.",
                QList<Utils::TextModifier>() << Utils::FG_GREEN
            ).toStdString() << std::endl;

            return;
        } else {
            std::cout << Utils::formatText(
                "Project could not be selected",
                QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
        }
    }

    std::cout << Utils::formatText(
        "The project " + projectName + " does not exist.",
        QList<Utils::TextModifier>() << Utils::FG_RED
    ).toStdString() << std::endl;
}

void ConsoleController::newProject()
{
    QString name = parameter_.toString();

    // Add json extension.
    if (!name.endsWith(".json")) {
        name += ".json";
    }

    delete model_;

    model_ = new ContextualizationModel();

    if (!QFile::exists(PROJECTS_FOLDER + name)) {
        if (exportToJsonFile(PROJECTS_FOLDER + name)) {
            std::cout << "New project created succesfully." << std::endl;
        } else {
            std::cout << Utils::formatText(
                "Imposible to create a new project now.",
                QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
        }
    } else {
        std::cout << Utils::formatText(
            "Project already exists.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

void ConsoleController::listProjects()
{
    QDir projectsDir(PROJECTS_FOLDER);
    QStringList projects;
    QString selectedProject = getParameterFromConfigFile("selected_project");

    projects = projectsDir.entryList(QStringList() << "*.json");

    foreach (QString project, projects) {
        if (selectedProject == project) {
            std::cout << Utils::formatText(
                "(X) " + project,
                QList<Utils::TextModifier>() << Utils::FG_GREEN << Utils::BOLD
            ).toStdString() << std::endl;
        } else {
            std::cout << Utils::formatText(
                "( ) " + project,
                QList<Utils::TextModifier>() << Utils::BOLD
            ).toStdString() << std::endl;
        }
    }
}

void ConsoleController::detailCurrentProject()
{
    QFileInfo project(projectPath_);
    FirmwareString * fwString;
    QString rowSeparator;
    QString selected;

    if (model_ != Q_NULLPTR) {
        for (int i = 0; i < 94; i++) {
            rowSeparator += '*';
        }

        std::cout.setf(std::ios_base::adjustfield, std::ios_base::left);
        std::cout << std::endl;
        std::cout << "Name: " << project.fileName().toStdString() << std::endl;
        std::cout << "Image: " << model_->getImage().toStdString() << std::endl;
        std::cout << "Strings: " << std::endl;

        std::cout << std::setw(4) << "    " << std::setw(30) << "ID" << std::setw(50) << "VALUE" << std::setw(10) << "STATE"<< std::endl;
        foreach (QObject *object, model_->getStringsList()) {
            selected = QString("(:selected)");
            fwString = static_cast<FirmwareString *>(object);

            std::cout << rowSeparator.toStdString() << std::endl;
            std::cout << std::setw(4)  << selected.replace(":selected", fwString->isSelected() ? "X" : " ").toStdString()
                      << std::setw(30) << fwString->getId().toStdString()
                      << std::setw(50) << fwString->getValue().toStdString()
                      << std::setw(10) << fwString->getState().toStdString() << std::endl;
        }
    }
    else {
        std::cout << Utils::formatText(
            "There are not any project selected.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}


void ConsoleController::deleteProject()
{
    QString projectName = parameter_.toString();
    projectName = projectName.isEmpty() ? QFileInfo(projectPath_).fileName() : projectName;

    if (QFile::remove(PROJECTS_FOLDER + projectName)) {
        QFile::remove(model_->getImage());
        std::cout << Utils::formatText(
            "Project " + projectName + " removed succesfully.",
            QList<Utils::TextModifier>() << Utils::FG_GREEN
        ).toStdString() << std::endl;
    } else {
        std::cout << Utils::formatText(
            "Imposible to remove the project: " + projectName + ". Are you sure that projec exists?",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

void ConsoleController::clearAll()
{
    removeAllStrings();
    setImage("");

    if (!projectPath_.isEmpty() && save()) {
        std::cout << "Projects empty succesfully" << std::endl;
    } else {
        std::cout << Utils::formatText(
            "There was a problem saving the project.",
             QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

void ConsoleController::clearImage()
{
    setImage("");

    if (!projectPath_.isEmpty() && save()) {
        std::cout << "Image empty succesfully" << std::endl;
    } else {
        std::cout << Utils::formatText(
            "There was a problem saving the project.",
             QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

void ConsoleController::select(const QString id)
{
    if (!projectPath_.isEmpty() && selectString(id, true)) {
        if (save()) {
            std::cout << "String selected succesfully." << std::endl;
        } else {
            std::cout << Utils::formatText(
                "There was a problem saving the project.",
                 QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
        }
    } else {
        std::cout << Utils::formatText(
            "There are not any string with " + id + " identifier in selected project.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

void ConsoleController::unselect(const QString id)
{
    if (!projectPath_.isEmpty() && selectString(id, false)) {
        if (save()) {
            std::cout << "String unselected succesfully." << std::endl;
        } else {
            std::cout << Utils::formatText(
                "There was a problem saving the project.",
                 QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
        }
    } else {
        std::cout << Utils::formatText(
            "There are not any string with " + id + " identifier in selected project.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

void ConsoleController::import()
{
    QFileInfo projectToImport(parameter_.toString());

    if (projectToImport.suffix() == ".json") {
        if (QFile::copy(projectToImport.absoluteFilePath(), PROJECTS_FOLDER + projectToImport.fileName())) {
             std::cout << "Project copied succesfully." << std::endl;
        } else {
            std::cout << Utils::formatText(
                "The project could be not copied to " + PROJECTS_FOLDER + ".",
                QList<Utils::TextModifier>() << Utils::FG_RED
            ).toStdString() << std::endl;
        }
    } else {
        std::cout << Utils::formatText(
            "The project is not a valid contesxtualization projects, it is not a json file.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
    }
}

QString ConsoleController::requestUsername()
{
    std::string username;

    std::getline(std::cin, username);

    return QString::fromStdString(username);

}

QString ConsoleController::requestPassword()
{
    int character;
    QString password;

    do {
        character = getchar();
        password.append(QChar(character));
        putchar('*');
    }while(character != '\n');

    return password;
}

ContextualizationController::CodeError ConsoleController::loadProjectFromConfiguration()
{
    QString projectName;
    CodeError error;

    projectName = getParameterFromConfigFile("selected_project");

    if (projectName.isEmpty()) {
        std::cout << Utils::formatText(
            "There are not any selected project.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;

        return NoImportFile;
    }

    error = importProjectFromJsonFile(PROJECTS_FOLDER + projectName);
    switch (error) {
    case NoError:
        projectPath_ = PROJECTS_FOLDER + projectName;
        break;

    case NoImportFile:
        std::cout << Utils::formatText(
            "The selected project does not exist.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
        break;

    case ImportFileFormat:
        std::cout << Utils::formatText(
            "The selected project has a format error.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
        break;

    default:
        std::cout << Utils::formatText(
            "A critical error ocurred during load selected project.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString() << std::endl;
        break;
    }

    return error;
}
