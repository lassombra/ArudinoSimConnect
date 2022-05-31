#include "MyFrame.h"
#include "ArudinoSimConnect.h"

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Arduino Command", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX))
{
    CreateStatusBar();
    SetStatusText("Disconnected");
    wxBoxSizer* buttonSizer1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* buttonSizer2 = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer1->Add(new wxButton(this, ID_Button_Off, "Off"), 1, wxALIGN_CENTER | wxRIGHT, 5);
    buttonSizer1->Add(new wxButton(this, ID_Button_On, "On"), 1, wxALIGN_CENTER | wxLEFT, 5);
    buttonSizer2->Add(new wxButton(this, ID_Button_Connect, "Connect"), 1, wxALIGN_CENTER | wxRIGHT, 5);
    buttonSizer2->Add(new wxButton(this, ID_Button_Disconnect, "Disconnect"), 1, wxALIGN_CENTER | wxLEFT, 5);
    wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);
    myText = new wxStaticText(this, -1, "Hellow World");
    sizer2->Add(myText, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxTOP, 20);
    sizer2->Add(buttonSizer1, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT, 20);
    sizer2->Add(buttonSizer2, 1, wxALIGN_CENTER | wxLEFT | wxRIGHT | wxBOTTOM, 20);
    sizer2->SetSizeHints(this);
    SetSizer(sizer2);-
}
void MyFrame::setLabel(wxString label)
{
    myText->SetLabelText(label);
    Fit();
    Layout();
}
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(ID_Button_Connect, MyFrame::OnConnectClick)
    EVT_BUTTON(ID_Button_Disconnect, MyFrame::OnDisconnectClick)
    EVT_BUTTON(ID_Button_On, MyFrame::OnButtonClick)
    EVT_BUTTON(ID_Button_Off, MyFrame::OnButtonClick)
    EVT_CLOSE(MyFrame::OnExit)
END_EVENT_TABLE()

void MyFrame::OnExit(wxCloseEvent& event)
{
    if (event.CanVeto()) {
        event.Veto();
        Hide();
    }
}

void MyFrame::OnButtonClick(wxCommandEvent& event) {
    switch (event.GetId()) {
    case ID_Button_Off:
        ArduinoSimConnect::GetApp().getManager().sendMessage("off");
        break;
    case ID_Button_On:
        ArduinoSimConnect::GetApp().getManager().sendMessage("on");
        break;
    }
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnConnectClick(wxCommandEvent& event)
{
    ArduinoSimConnect::GetApp().getManager().startConnection();
}

void MyFrame::OnDisconnectClick(wxCommandEvent& event) 
{
    ArduinoSimConnect::GetApp().getManager().endConnection();
}
