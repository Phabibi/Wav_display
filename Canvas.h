#include <wx/wx.h>

// Our canvas class

class Canvas: public wxPanel
{
  public:
        Canvas(wxFrame* parent);
        //this is to draw our waves
        wxFrame *frame;

        void Draw();

        void paintEvent(wxPaintEvent& event);

        //event to actually draw
        void paintNow();
        
        //render the drawings
        void render(wxDC& dc);

        DECLARE_EVENT_TABLE()


};

