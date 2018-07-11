#include "consolecontextualizationcontroller.h"

ConsoleContextualizationController::ConsoleContextualizationController()
{
    ConsoleContextualizationController(0, Q_NULLPTR);
}

ConsoleContextualizationController::ConsoleContextualizationController(int argc, char **argv)
    : ContextualizationControllerBase()
{
    // Set default values.
    appName_ = QString(argv[0]);
    action_ = NoAction;
    findType_ = ByID;

    // Decode arguments to know what to do.
    decodeArguments(argc, argv);
}

void ConsoleContextualizationController::exec()
{
    switch (action_) {
    case PrintHelp:
        printUsage();
        break;

    default:
        break;
    }
}

bool ConsoleContextualizationController::decodeArguments(int argc, char **argv)
{    
    if (argc == 0 || argv == Q_NULLPTR) {
        return false;
    }

    // Cases with dynamic number of arguments.
    if (argc >= 3) {
        if (QString::compare(argv[1], "delete") == 0 && QString::compare(argv[2], "project") == 0) {
            parameter_ = argc == 4 ? QVariant(argv[4]) : QVariant();
            action_ = DeleteProject;
        } else if (QString::compare(argv[1], "add") == 0) {
            for (int i = 2; i < argc; ++i) {
                if (i == argc-1 && QString::compare(argv[i], "--help") == 0) {
                    action_ = PrintAddHelp;
                } else if (i == argc-1) { // The last parameter must be the string to search.
                    parameter_ = QVariant(argv[i]);
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

            action_ = AddString;
        }
    } else if (argc >= 2) {
        if (QString::compare(argv[1], "detect") == 0) {
            for (int i = 2; i < argc; ++i) {
                if (i == argc-1 && QString::compare(argv[i], "--help") == 0) {
                    action_ = PrintAddHelp;
                } else if (i == argc-1) { // The last parameter must be the string to search.
                    parameter_ = QVariant(argv[i]);
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

            action_ = DetectStrings;
        }
    }

    // Cases with constant number of arguments.
    switch (argc) {
    case 2:
        if (QString::compare(argv[1], "-h") == 0 || QString::compare(argv[1], "--help") == 0) {
            action_ = PrintHelp;
        } else if (QString::compare(argv[1], "list") == 0) {
            action_ = ListProjects;
        } else if (QString::compare(argv[1], "detail") == 0) {
            action_ = DetailProject;
        } else if (QString::compare(argv[1], "add") == 0) {
            action_ = PrintAddHelp;
        }

        break;

    case 3:
        if (QString::compare(argv[1], "select") == 0) {
            parameter_ = QVariant(argv[2]);
            action_ = SelectProject;
        } else if (QString::compare(argv[1], "clear") == 0) {
            if (QString::compare(argv[2], "strings") == 0) {
                action_ = ClearStrings;
            } else if (QString::compare(argv[2], "image") == 0) {
                action_ = ClearImage;
            } else if (QString::compare(argv[2], "all") == 0) {
                action_ = ClearAll;
            } else {
                action_ = PrintClearHelp;
            }
        } else if (QString::compare(argv[1], "new") == 0) {
            parameter_ = QVariant(argv[2]);
            action_ = NewProject;
        } else if (QString::compare(argv[1], "remove") == 0) {
            parameter_ = QVariant(argv[2]);
            action_ = RemoveString;
        } else if (QString::compare(argv[1], "image") == 0) {
            if (QString::compare(argv[1], "-c") == 0 || QString::compare(argv[1], "--capture") == 0) {
                action_ = CaptureArea;
            } else if (QString::compare(argv[1], "--help") == 0) {
                action_ = PrintImageHelp;
            } else {
                parameter_ = QVariant(argv[2]);
                action_ = SetImage;
            }
        } else if (QString::compare(argv[1], "export") == 0) {
            parameter_ = QVariant(argv[2]);
            action_ = ExportProject;
        } else if (QString::compare(argv[1], "import") == 0) {
            parameter_ = QVariant(argv[2]);
            action_ = ImportPorject;
        } else if (QString::compare(argv[1], "send") == 0) {
            parameter_ = QVariant(argv[2]);
            action_ = Send;
        }

        break;

    default:
        action_ = PrintHelp;
        break;
    }

    return true;
}

void ConsoleContextualizationController::setAction(ActionType action, QVariant parameter)
{
    action_ = action;
    parameter_ = parameter;
}

void ConsoleContextualizationController::printUsage()
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

void ConsoleContextualizationController::printClearDetails()
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

void ConsoleContextualizationController::printAddDetails()
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

void ConsoleContextualizationController::printImageDetails()
{
    const QString help =
        appName_ + " image <option>\n\n"
        "OPTIONS:\n"
        "  -c, --capture     Launch a process where the user have to capture a screen area to be set as model image.\n"
        "  <path>            Sets the input image path as image model if exists.\n"
        ;

    std::cout << help.toStdString() << std::endl;
}

void ConsoleContextualizationController::printDetectOptions()
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

