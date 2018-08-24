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
}

void ConsoleController::exec()
{
    switch (action_) {
    case PrintHelp:
        printUsage();
        break;

    default:
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
           setAction(DeleteProject, argc == 4 ? QVariant(argv[4]) : QVariant());
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
                }
            }
        }
    } else if (argc >= 2) {
        if (QString::compare(argv[1], "detect") == 0) {
            for (int i = 2; i < argc; ++i) {
                if (i == argc-1 && QString::compare(argv[i], "--help") == 0) {
                    setAction(PrintAddHelp);
                } else if (i == argc-1) { // The last parameter must be the string to search.
                    setAction(DetectStrings, parameter_ = QVariant(argv[i]));
                } else if (QString::compare(argv[i], "--case-sensitive") == 0) {
                    caseSensitive_ = true;
                } else if (QString::compare(argv[i], "--case-insensitive") == 0) {
                    caseSensitive_ = false;
                } else if (QString::compare(argv[i], "--only-done-strings") == 0) {
                    onlyDoneStrings_ = true;
                } else if (QString::compare(argv[i], "--all-states") == 0) {
                    onlyDoneStrings_ = false;
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
        if (QString::compare(argv[1], "select") == 0) {
            setAction(SelectProject, QVariant(argv[2]));
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
            if (QString::compare(argv[1], "-c") == 0 || QString::compare(argv[1], "--capture") == 0) {
                setAction(CaptureArea);
            } else if (QString::compare(argv[1], "--help") == 0) {
                setAction(PrintImageHelp);
            } else {
                setAction(SetImage, QVariant(argv[2]));
            }
        } else if (QString::compare(argv[1], "export") == 0) {
            setAction(ExportProject, QVariant(argv[2]));
        } else if (QString::compare(argv[1], "import") == 0) {
            setAction(ImportPorject, QVariant(argv[2]));
        } else if (QString::compare(argv[1], "send") == 0) {
            setAction(Send, QVariant(argv[2]));
        }

        break;

    default:
        setAction(PrintHelp);
        break;
    }

    return true;
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
        "  " + appName_ + " clear <option>          Clear selected elements in acive project."
        "  " + appName_ + " select <project>        Select the active project.\n"
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
        "  --all-states         Tool will find strings with all valid states."
        "  --only-done-strings  Tool will only find string with DONE state.\n\n\n"
        "By default, searchs will be by identifier, case sensitive and only find strings with DONE state."
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
        "  --all-states         Tool will find strings with all valid states."
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

    std::cout << "Added " << count << " string in to project.";
}

void ConsoleController::remove(QString stringId)
{
    if (removeString(stringId)) {
        std::cout << "The string '" << stringId.toStdString() << "' has been removed succesfully.";
    } else {
        std::cout << "There are not any string with the indentifier '" << stringId.toStdString() << "'.";
    }
}

void ConsoleController::clear()
{
    bool done;

    done = removeAllStrings();

    if (done) {
        std::cout << "All string removed succesfully.";
    } else {
        std::cout << "There are not strings to be removed.";
    }
}

void ConsoleController::capture(bool detectStringsOnLoad)
{
    Q_UNUSED(detectStringsOnLoad); // In CLI mode is not util.

    QString path;

    path = takeCaptureArea();

    if (setImage(path)) {
        std::cout << "Capture set succesfully.";
    } else {
        std::cout << Utils::formatText(
            "There was a problem with a capture.",
            QList<Utils::TextModifier>() << Utils::FG_RED
        ).toStdString();
    }
}

void ConsoleController::load(bool detectStringsOnLoad)
{
    Q_UNUSED(detectStringsOnLoad); // In CLI mode is not util.
}

void ConsoleController::detect()
{

}

void ConsoleController::send()
{

}

void ConsoleController::cancel()
{

}

bool ConsoleController::save()
{
    return exportToJsonFile(projectPath_);
}

bool ConsoleController::saveAs()
{
    // Is not an option in console mode.
    return false;
}

void ConsoleController::open()
{

}

void ConsoleController::newProject()
{

}

