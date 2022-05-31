// ArudinoSimConnect.cpp : Defines the entry point for the application.
//


#include "ArudinoSimConnect.h"
#include "MyFrame.h"
#include "constants.h"

wxIMPLEMENT_APP(ArduinoSimConnect);

bool ArduinoSimConnect::OnInit()
{
    if (!initSelectComPort()) {
        return false;
    }
    ArduinoSimConnect::app = this;
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    connManager.setFrame(frame);
    static const int INTERVAL = 100;
    myTimer = new wxTimer(this, ID_Timer);
    myTimer->Start(INTERVAL);
    return true;
}

ArduinoSimConnect& ArduinoSimConnect::GetApp() {
    return *ArduinoSimConnect::app;
}

ArduinoSimConnect* ArduinoSimConnect::app;

void ArduinoSimConnect::CleanUp() {
    myTimer->Stop();
    delete myTimer;
    myTimer = nullptr;
    wxApp::CleanUp();
}

ConnectionManager &ArduinoSimConnect::getManager()
{
    return connManager;
}

void ArduinoSimConnect::TimerEvent(wxTimerEvent&)
{
    getManager().processTimerEvent();
}

BEGIN_EVENT_TABLE(ArduinoSimConnect, wxApp)
    EVT_TIMER(ID_Timer, ArduinoSimConnect::TimerEvent)
END_EVENT_TABLE()