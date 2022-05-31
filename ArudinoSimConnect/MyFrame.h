#pragma once
#include <wx/wx.h>
#include "constants.h"
#include "Serial.h"
#include <memory>
#include <locale>
#include <codecvt>
#include <string>
class ArduinoSimConnect;
class MyFrame : public wxFrame
{
public:
    MyFrame();
    void setLabel(wxString);
    DECLARE_EVENT_TABLE()
private:
    wxStaticText* myText;
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCloseEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnConnectClick(wxCommandEvent& event);
    void OnDisconnectClick(wxCommandEvent& event);
    void OnButtonClick(wxCommandEvent& event);
};
