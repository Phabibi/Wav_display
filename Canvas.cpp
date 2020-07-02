#include <wx/wx.h>
#include <wx/sizer.h>
#include "Canvas.h"

Canvas::Canvas(wxFrame* parent) :
wxPanel(parent)
{
}

void Canvas::Draw()
{
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    wxFrame* frame = new wxFrame(nullptr, -1,"WAV Fade_in _out", wxPoint(50,50), wxSize(800,600));
    Canvas* canvas = new Canvas(reinterpret_cast<wxFrame*>(frame));

    sizer->Add(canvas, 1, wxEXPAND);
	
    frame->SetSizer(sizer);
    frame->SetAutoLayout(true);
	
    frame->Show();
}
//painting event
BEGIN_EVENT_TABLE(Canvas, wxPanel)

      EVT_PAINT(Canvas::paintEvent)

END_EVENT_TABLE()

void Canvas::paintEvent(wxPaintEvent & evt)
{
    wxPaintDC dc(this);
    render(dc);
}
void Canvas::paintNow()
{
    wxClientDC dc(this);
    render(dc);
}


void Canvas::render(wxDC&  dc)
{
    // draw some text
    dc.DrawText(wxT("Testing"), 40, 60);

    // draw a circle
    dc.SetBrush(*wxGREEN_BRUSH); // green filling
    dc.SetPen( wxPen( wxColor(255,0,0), 5 ) ); // 5-pixels-thick red outline
    dc.DrawCircle( wxPoint(200,100), 25 /* radius */ );

    // draw a rectangle
    dc.SetBrush(*wxBLUE_BRUSH); // blue filling
    dc.SetPen( wxPen( wxColor(255,175,175), 10 ) ); // 10-pixels-thick pink outline
    dc.DrawRectangle( 300, 100, 400, 200 );

    // draw a line
    dc.SetPen( wxPen( wxColor(0,0,0), 3 ) ); // black line, 3 pixels thick
    dc.DrawLine( 300, 100, 700, 300 ); // draw line across the rectangle

    // Look at the wxDC docs to learn how to draw other stuff
}
