/**
 * @file linuxguicontroller.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class LinuxGuiController
 *
 * @brief This is the controller class that works a linux GUI environment.
 */

#ifndef LINUXGUICONTROLLER_H
#define LINUXGUICONTROLLER_H

#include "guicontroller.h"

class LinuxGuiController : public GuiController
{
public:

    /**
     * @brief LinuxGuiController
     * @param view
     * @param parent
     */
    LinuxGuiController(QQuickWindow *view = Q_NULLPTR, QObject *parent = Q_NULLPTR);

    /**
     * @copydoc ContextualizationController::takeCaptureArea()
     */
    QString takeCaptureArea() override;
};

#endif // LINUXGUICONTROLLER_H
