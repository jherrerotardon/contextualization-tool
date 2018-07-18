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
