/**
 * @file windowdguicontroller.h
 * @author Jorge Herrero Tardón (jorgeht@usal.es)
 * @date 20/02/2018
 * @version 1.0
 * @class WindowsGuiController
 *
 * @brief This is the controller class that works a GUI Windows environment.
 */

/////////////////////////////////////////////////////
//  NOT IMPLEMENTED CLASS YET.                  ////
////////////////////////////////////////////////////

#ifndef WINDOWSGUICONTROLLER_H
#define WINDOWSGUICONTROLLER_H

#include "guicontroller.h"

class WindowsGuiController : public GuiController
{
public:
    WindowsGuiController(QQuickWindow *view = Q_NULLPTR, QObject *parent = Q_NULLPTR);

    /**
     * @copydoc ContextualizationController::takeCaptureArea()
     */
    QString takeCaptureArea() override;

    /**
     * @copydoc ContextualizationController::generateDoneFpFile()
     */
    int generateDoneFpFile() override;
};

#endif // WINDOWSGUICONTROLLER_H
