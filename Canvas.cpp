#include <wx/wx.h>
#include <wx/sizer.h>
#include "Canvas.h"

Canvas::Canvas(wxFrame* parent) :
wxPanel(parent)
{
}

void Canvas::Draw()
{

      wxFrame* frame = new wxFrame(nullptr, -1,"WAV Fade_in _out", wxPoint(50,50), wxSize(2000,500));

      //sets up the canvas frame
      Canvas* canvas = new Canvas(reinterpret_cast<wxFrame*>(frame));
      canvas->file_path = this->file_path;
      canvas->wav = this->wav;
      
      // making the new scroll frame


      frame->Layout();
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
    dc.DrawText("the file is : " + this->file_path, 40, 60);
    dc.DrawText("Sample Count : " +  std::to_string(this->wav.sampleCount), 40, 120);

    int sample_count = this->wav.sampleCount;

    //how far we are stepping through
    float steps = 2000/(float)this->wav.sampleCount;

    float curr = 0;
    int k = 0;
     
    dc.SetBrush(*wxGREEN_BRUSH); // green filling
    dc.SetPen( wxPen( wxColor(0,0,0), 2 ) ); // 5-pixels-thick red outline

    //2000x300 window
    //
    //
    for(int i = 0; i < sample_count; i++)
    {
       float val = 0.0;

       if(this->wav.data[i] >= 0)
       {
         // scale this up by divindig it with max_int16
         float p_distance = (float) (this->wav.data[i]/32768.0);
         //check if we hit middle yet
         if(curr < 1000)
         {
            //scale up gradually (fade in)
            p_distance*= (curr/1000);
         }
         else
         {
           //if we are bigger fade out
           p_distance*= 2 - (curr/1000);
         }
       // scale our value with the offset for fade in and fadeout and put it in the middle
       val = (p_distance*150) + 150;
       
       }  
      
       if(this->wav.data[i] <0)
       {
          // these are values below the the x-axis(lowpitch)
          // since it signed the highest negative value in uint16 is 2^16-1 - 1
          float h_distance = (float) (this->wav.data[i] / -32767.0);
          if(curr < 1000)
          {
            h_distance*= (curr/1000);
          }
          else
          {
            h_distance*= 2 - (curr/1000);
          }

       //since we want the negative values, we minus 
       val = 150 - (h_distance*150);
       }

        dc.DrawLine(curr , val+100 , curr+steps, val+100);

        curr+= steps;


    }

}

