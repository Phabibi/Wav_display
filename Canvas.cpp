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

    
    //dc.SetBrush(*wxGREEN_BRUSH); // green filling
    //dc.SetPen( wxPen( wxColor(0,0,0), 1 ) ); // 5-pixels-thick red outline
    //int samples2=0;
    //
    // wxFile file(this->file_path);
    // char sample[128];

    // //12 bytes of RIF header
    // char RIFBuffer[12];
    // //24 bytes FMT 
    // char FMTBuffer[24];
    // //6 bytes of Data
    // char DATABuffer[6];

    // file.Read(RIFBuffer, 12);
    // file.Read(FMTBuffer, 24);
    // file.Read(DATABuffer, 6);
    // // seek the next 42 bytes since thats our actual data
    // file.Seek(42);
    // int i= 0;
    // int k =1;
    // std::vector<int> buffer;

    // while(!file.Eof())
    // {
    //    std::cout<< "k is : "<< k <<std::endl;
    //    std::cout<< "i is : "<< i <<std::endl;
    //    file.Read(sample,2); 
    //    int samples=reinterpret_cast<char>(sample[k]);
    //    k++;
    //    //scale
    //    buffer.push_back(samples*2);
    //    i+=1200;
    //    //dc.DrawLine(i/4,samples2*2+500,(i+1)/4,samples*2+500);
    //  }
    // std::cout <<"i made it out:";

    // for(int i = 0 ; i < buffer.size(); i += 1200)
    // {
    //    dc.DrawLine(i/4,buffer[i]+500,(i+1)/4,buffer[i]+500);
    // }

    // draw some text
    //dc.DrawText(this->file_path, 40, 60);

    //// draw a circle
    //dc.SetBrush(*wxGREEN_BRUSH); // green filling
    //dc.SetPen( wxPen( wxColor(255,0,0), 5 ) ); // 5-pixels-thick red outline
    //dc.DrawCircle( wxPoint(200,100), 25 /* radius */ );

    //// draw a rectangle
    //dc.SetBrush(*wxBLUE_BRUSH); // blue filling
    //dc.SetPen( wxPen( wxColor(255,175,175), 10 ) ); // 10-pixels-thick pink outline
    //dc.DrawRectangle( 300, 100, 400, 200 );

    //// draw a line
    //dc.SetPen( wxPen( wxColor(0,0,0), 3 ) ); // black line, 3 pixels thick
    //dc.DrawLine( 300, 100, 700, 300 ); // draw line across the rectangle

    // Look at the wxDC docs to learn how to draw other stuff
}

