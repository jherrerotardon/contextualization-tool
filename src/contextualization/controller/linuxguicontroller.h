#ifndef LINUXGUICONTROLLER_H
#define LINUXGUICONTROLLER_H

#include "guicontroller.h"

class LinuxGuiController : public GuiController
{
    Q_OBJECT

public:
    LinuxGuiController(QQuickWindow *view = Q_NULLPTR, QObject *parent = Q_NULLPTR);
};

#endif // LINUXGUICONTROLLER_H
