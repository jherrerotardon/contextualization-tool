/**
 * @file consolecontroller.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class ConsoleController
 *
 * @brief This is the controller class that works a CLI environment.
 */

#ifndef CONSOLECONTROLLER_H
#define CONSOLECONTROLLER_H

#include <iostream>
#include <QVariant>
#include <QList>
#include <stdio.h>
#include <iomanip>
#include <string.h>
#include "contextualization/controller/contextualizationcontroller.h"

/**
 * @brief The ConsoleContextualizationController class is responsible for controll the Contextualization Tool
 * application when is executed from a terminal (CLI).
 */
class ConsoleController : public ContextualizationController
{
public:

    /**
     * @brief Constains all actions that can be done by the exec function.
     */
    enum ActionType {
        PrintHelp = 0,      ///< Indicates that the app have to print the general help.
        NoAction,           ///< Indicates that the app have to do anything.
        ListProjects,       ///< Indicates that the app have to list all available projects.
        DetailProject,      ///< Indicates that the app have to show detail for avitve project.
        NewProject,         ///< Indicates that the app have to create new project.
        DeleteProject,      ///< Indicates that the app have to delete a project.
        SelectProject,      ///< Indicates that the app have to ativate a project.
        ClearStrings,       ///< Indicates that the app have to clear all strings from the active project.
        ClearImage,         ///< Indicates that the app have to unset current image from the active project.
        ClearAll,           ///< Indicates that the app have to empty the active project.
        AddString,          ///< Indicates that the app have to add new string in the active project.
        RemoveString,       ///< Indicates that the app have to remove a string from the active project.
        SetImage,           ///< Indicates that the app have to set the image in the active project.
        CaptureArea,        ///< Indicates that the app have to let the user capture an area of sreen and set it as image model.
        DetectStrings,      ///< Indicates that the app have to extract strings from image model and add it in active project.
        ExportProject,      ///< Indicates that the app have to save the active project in the path entered by the user.
        ImportProject,      ///< Indicates that the app have to import a project entered by the user.
        Send,               ///< Indicates that the app have to send the active contextualization.
        PrintClearHelp,     ///< Indicates that the app have to print detailed help for clear command.
        PrintAddHelp,       ///< Indicates that the app have to print extended help for add command.
        PrintImageHelp,     ///< Indicates that the app have to print extended help for image command.
    };

    /**
     * @brief Creates an empty controller.
     */
    ConsoleController();

    /**
     * @brief Creates a controller decoding arguments received by argument.
     * @param argc Number of argv elements.
     * @param argv Arguments entered by the user when executed the app.
     */
    ConsoleController(int argc, char **argv);

    /**
     * @brief Executes the logic of controller.
     *
     * It has mutiples type of behavior depending the value of action_ variable.
     *
     * Types of action can be shown in ActionType enum.
     */
    void exec();

    /**
     * @brief Decode arguments entered to set the behavior of the exec method.
     *
     * The first element of argv always must be the name of the app.
     * @param argc Number of argv elements.
     * @param argv Arguments to decode.
     * @return
     */
    bool decodeArguments(int argc, char **argv);

    /**
     * @brief Sets the action to be executed and his parameter,
     * @param action Actions to be executed by exec() function.
     * @param parameter Required parameter to execute the action.
     */
    inline void setAction(ActionType action, QVariant parameter = QVariant());

    /**
     * @brief Print in screen the general usage of application.
     */
    void printUsage();

    /**
     * @brief Print extended help for clear command.
     */
    void printClearDetails();

    /**
     * @brief Print extended help for add command.
     */
    void printAddDetails();

    /**
     * @brief Print extended help for image command.
     */
    void printImageDetails();

    /**
     * @brief Print extended help for detect command.
     */
    void printDetectOptions();

private slots:

    /**
     * @copydoc ContextualizationController::add(QString newString, int findType);
     */
    void add(QString newString, int findType) override;

    /**
     * @copydoc ContextualizationController::remove(QString stringId);
     */
    void remove(QString stringId) override;

    /**
     * @copydoc ContextualizationController::clear();
     */
    void clear() override;

    /**
     * @copydoc ContextualizationController::capture();
     */
    void capture(bool detectStringsOnLoad = false) override;

    /**
     * @copydoc ContextualizationController::load();
     */
    void load(bool detectStringsOnLoad = false) override;

    /**
     * @copydoc ContextualizationController::detect();
     */
    void detect() override;

    /**
     * @copydoc ContextualizationController::send();
     */
    void send() override;

    /**
     * @copydoc ContextualizationController::cancel();
     */
    void cancel() override;

    /**
     * @copydoc ContextualizationController::save();
     */
    bool save() override;

    /**
     * @copydoc ContextualizationController::saveAs();
     */
    bool saveAs() override;

    /**
     * @copydoc ContextualizationController::open();
     */
    void open() override;

    /**
     * @copydoc ContextualizationController::newProject();
     */
    void newProject() override;

    /**
     * @brief Prints in console all available projects.
     */
    void listProjects();

    /**
     * @brief Prints in console details of selected project.
     */
    void detailCurrentProject();

    /**
     * @brief Deletes project introduce by user un console parameter.
     */
    void deleteProject();

private:
    QString appName_;       ///< Application name.
    QVariant parameter_;    ///< Value of parameter that will be used by exec depending the behavior.
    ActionType action_;     ///< Behavior of exec() function.
    MatchType findType_;    ///< Indicates the type of find that will be done.
    QString projectPath_;   ///< Path of loaded project.

    /**
     * @brief Request by console the username.
     * @return User input.
     */
    QString requestUsername();

    /**
     * @brief Request by console the password.
     * @return User input.
     */
    QString requestPassword();

    /**
     * @brief Loads a project(model) from configuration file.
     *
     * Create a model with project selected in configuration file.
     * @return
     */
    CodeError loadProjectFromConfiguration();
};

#endif // CONSOLECONTROLLER_H
