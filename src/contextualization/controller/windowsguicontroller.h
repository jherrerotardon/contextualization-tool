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
};

#endif // WINDOWSGUICONTROLLER_H
