#ifndef WINDOWSGUICONTROLLER_H
#define WINDOWSGUICONTROLLER_H

#include "guicontroller.h"

class WindowsGuiController : public GuiController
{
    Q_OBJECT

public:
    WindowsGuiController(QQuickWindow *view = Q_NULLPTR, QObject *parent = Q_NULLPTR);
};

#endif // WINDOWSGUICONTROLLER_H
