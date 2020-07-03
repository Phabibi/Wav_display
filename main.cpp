#include <wx/wxprec.h>
#include <iostream>
#include <string>
#include <fstream>
#include <wx/wfstream.h>

#ifndef WX_PRECOMP
#	include <wx/wx.h>
#endif

#include "base.h"
#include "Canvas.cpp"

IMPLEMENT_APP(MainApp) // Initializes the MainApp class...

bool MainApp::OnInit()
{
	// Create an instance of our frame, or window
	MainFrame *MainWin = new MainFrame(_("WAV File Opener")
                                           , wxPoint(30 , 30), wxSize(600, 400));
	MainWin->Show(); // show the window
	SetTopWindow(MainWin); // and finally, set it as the main window

	return TRUE;
}


MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
: wxFrame(nullptr, -1, title, pos, size)
{
        Buttons = new wxButton*[2];
        wxGridSizer *grid  = new wxGridSizer(2,2,0,0);
        //Create our buttons
        
        Buttons[0] = new wxButton(this,BUTTON_open, "Open File"); 
        Buttons[1] = new wxButton(this,BUTTON_open+1, "Quit"); 

        //add it to the grid

        grid->Add(Buttons[0],1,wxEXPAND|wxALL);
        grid->Add(Buttons[1],1,wxEXPAND|wxALL);

        this->SetSizer(grid);
        grid->Layout();

        

        //draw


}

void MainFrame::OpenFile(wxCommandEvent& WXUNUSED(event))
{
        //opens a custom file exploarer that only opens *WAV files
	wxFileDialog *OpenDialog = new wxFileDialog(this, _("Choose a .WAV file to open"), wxEmptyString, wxEmptyString,("Text files (*.txt)|*.txt|WAV file (*.wav)|*.wav"),wxFD_OPEN, wxDefaultPosition);

            // if they pressed cancel, dont terminate the program
            if(OpenDialog->ShowModal() != wxID_OK)
            {
               return;
            }
            else
            {
              CurrentDocPath = OpenDialog->GetPath();

              Header head;
              WavReader wav;

              const char* path  = CurrentDocPath.mb_str();
              wav.Reader(head, const_cast<char*>(path));
              //[TODO] we jklshould interpret the data for wav and draw it.
               
              
              Canvas* canvas = new Canvas(reinterpret_cast<wxFrame*>(this));
              canvas->wav = wav;
              canvas->file_path = CurrentDocPath;
              canvas->Draw();
            }

}
void MainFrame::New_window()
{
        MainFrame *MainWin = new MainFrame(_("WAV File Opener")
                                           , wxPoint(30 , 30), wxSize(1200, 800));
	MainWin->Show(); // show the windo
};
void MainFrame::Quit(wxCommandEvent& WXUNUSED(event))
{
	Close(TRUE); // Close the window
}

