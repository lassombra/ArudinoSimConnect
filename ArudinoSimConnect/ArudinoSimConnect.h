#pragma once

#include "resource.h"
#include "framework.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "ConnectionManager.h"

class ArduinoSimConnect : public wxApp
{
public:
    static ArduinoSimConnect& GetApp();
    virtual bool OnInit();
    ConnectionManager &getManager();
    void TimerEvent(wxTimerEvent&);
    void CleanUp() wxOVERRIDE;
private:
    static ArduinoSimConnect* app;
    ConnectionManager connManager;
    wxTimer* myTimer;
    DECLARE_EVENT_TABLE()
};