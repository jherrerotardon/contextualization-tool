/**
 * @file guicontroller.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class GuiController
 *
 * @brief This is the controller class that works a GUI environment.
 */

#ifndef GUICONTROLLER_H
#define GUICONTROLLER_H

#include <QInputDialog>
#include <QFileDialog>
#include <QApplication>
#include <QQuickWindow>
#include "contextualization/controller/contextualizationcontroller.h"

class GuiController : public ContextualizationController
{
    Q_OBJECT

    Q_PROPERTY(QString image READ getImageOfModel NOTIFY imageChanged)
    Q_PROPERTY(QList<QObject *> tableModel READ getTableModel NOTIFY stringsListChanged)
    Q_PROPERTY(QQuickWindow *view READ getView WRITE setView NOTIFY viewChanged)
    Q_PROPERTY(bool onlyDoneStrings MEMBER onlyDoneStrings_)
    Q_PROPERTY(bool caseSensitive MEMBER caseSensitive_)

public:

    /**
     * @brief Creates an instance of a GuiController.
     * @param view Visual part of MVC.
     * @param parent Parent object.
     */
    GuiController(QQuickWindow *view = Q_NULLPTR, QObject *parent = Q_NULLPTR);

    /**
     * @brief Destroys the GuiController.
     */
    ~GuiController();

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
    void capture(bool detectStringsOnLoad) override;

    /**
     * @copydoc ContextualizationController::load();
     */
    void load(bool detectStringsOnLoad) override;

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
     * @brief Allows user select an area of image loaded where strings will be detected.
     */
    void detectsStringOnInterestingArea(
        int startX,
        int startY,
        int endX,
        int endY,
        int paintedWidth,
        int paintedHeight
    );

    /**
     * @brief Change a flag to indicate that the model has changed.
     */
    void indicateProjectChanges();

    /**
     * @brief Change a flag to indicate that the model has not changes.
     */
    void indicateProjectSaved();

    /**
     * @brief Opens a dialog where the user can configure the path of fp file.
     */
    void configFpFile();

    /**
     * @brief Opens a dialog where the user can configure the host where the contextualization will be sent.
     */
    void configRemoteHost();

    /**
     * @brief Opens a dialog where the user can configure all valid states that string can be.
     */
    void configValidStates();

private:
    QQuickWindow *view_;            ///< Pointer to view part of the MVC.
    QString currentProjectPath_;    ///< Path of the active project in disk.
    bool projectHasChanges_;        ///< Flag to know if current project has changes.

    /**
     * @brief Connects signals emited in the view with the slots in the controller.
     */
    void connectGuiSignalsAndSlots();

    /**
     * @brief Connects signals emited in the model with the slots in the controller.
     */
    void connectModelSignalsAndSlots();

    /**
     * @brief Returns a pointer to the view object.
     * @return Pointer.
     */
    QQuickWindow *getView();

    /**
     * @brief Sets the ponter to the view.
     * @param view Pointer to the view.
     */
    void setView(QQuickWindow *view);

    /**
     * @brief Opens a dialog where the user can be introduce an usename.
     * @return User input.
     */
    QString requestUsername();

    /**
     * @brief Opens a dialog where the user can be introduce a password without the characters being seen on the screen.
     * @return User input.
     */
    QString requestPassword();

signals:

    /**
     * @brief The singal is emitted when the pointer to the view changed.
     */
    void viewChanged();

    /**
     * @brief The signal is emitted when a current project is saved.
     */
    void unchangedProject();
};

#endif // GUICONTROLLER_H
