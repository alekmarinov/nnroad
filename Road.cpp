//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Road.h"
#include "Car.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tmain *main;
//---------------------------------------------------------------------------
__fastcall Tmain::Tmain(TComponent* Owner)
        : TForm(Owner)
{
        set_car=drawing=false;
        autoTrainMode=false;
        trainMode=true;
        mouseX=mouseY=0;
        lines_count=0;
        car=new Car(0, -1, 0, map->Height);
        camera_width=camera_height=20;
        camera_width2=camera_width/2;
        camera_height2=camera_height/2;
        cameraLineWidth=1;
}
//---------------------------------------------------------------------------


void __fastcall Tmain::mapMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
        if (set_car)
        {
                car->_posX=X;
                car->_posY=Y;
                set_car=false;
                repaintMap();
        }
        else
        {
                drawing=true;
        }
}
//---------------------------------------------------------------------------
void __fastcall Tmain::mapMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
        drawing=false;
        mouseX=mouseY=0;
}
//---------------------------------------------------------------------------
void __fastcall Tmain::mapMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
        if (drawing)
        {
            TCanvas* canvas=map->Canvas;
            if (mouseX || mouseY)
            {
                COLORREF oldColor=canvas->Pen->Color;
                canvas->Pen->Color = RGB(255, 255, 255);
                roadLinesX[lines_count]=mouseX;
                roadLinesY[lines_count]=mouseY;
                roadLinesX[lines_count+1]=X;
                roadLinesY[lines_count+1]=Y;
                canvas->LineTo(X, Y);
                canvas->Pen->Color=oldColor;
                lines_count+=2;
            } else
            {
                canvas->MoveTo(X, Y);
            }
            mouseX=X;
            mouseY=Y;
        } else if (set_car)
        {
                car->_posX=X;
                car->_posY=Y;
                repaintMap();
        }
}
//---------------------------------------------------------------------------
void __fastcall Tmain::ClearClick(TObject *Sender)
{
        TCanvas* canvas=map->Canvas;
        COLORREF oldColor=canvas->Brush->Color;
        canvas->Brush->Color = RGB(0, 0, 0);
        canvas->Rectangle(0, 0, map->Width, map->Height);
        canvas->Brush->Color=oldColor;
        lines_count=0;
}
//---------------------------------------------------------------------------
void __fastcall Tmain::mapPaint(TObject *Sender)
{
        repaintMap();
}
//---------------------------------------------------------------------------
void Tmain::repaintMap(int dir)
{
        TCanvas* canvas=map->Canvas;
        COLORREF oldColor=canvas->Brush->Color;
        canvas->Brush->Color = RGB(0, 0, 0);
        canvas->Rectangle(0, 0, map->Width, map->Height);
        canvas->Brush->Color=oldColor;


        oldColor=canvas->Pen->Color;
        canvas->Pen->Color = RGB(255, 255, 255);
        for (int i=0; i<lines_count; i+=2)
        {
                double angle=atan2((roadLinesY[i+1]-roadLinesY[i]),(roadLinesX[i+1]-roadLinesX[i]));
                double normalX=cos(angle+M_PI/2);
                double normalY=sin(angle+M_PI/2);

                for (int j=0; j<TrackBarWidth->Position; j++)
                {
                    canvas->Pen->Color = RGB(255-j*5, 255-j*5, 255-j*5);
                    canvas->MoveTo((int)(roadLinesX[i]+j*normalX), (int)(roadLinesY[i]+j*normalY));
                    canvas->LineTo((int)(roadLinesX[i+1]+j*normalX), (int)(roadLinesY[i+1]+j*normalY));
                    canvas->MoveTo((int)(roadLinesX[i]-j*normalX), (int)(roadLinesY[i]-j*normalY));
                    canvas->LineTo((int)(roadLinesX[i+1]-j*normalX), (int)(roadLinesY[i+1]-j*normalY));
                }
        }

        shotCamera(dir);

        // draw car
        canvas->Pen->Color = RGB(255, 0, 0);
        canvas->Brush->Color = RGB(255, 0, 0);
        canvas->Ellipse(car->_posX-camera_width2, car->_posY-camera_height2,
                car->_posX+camera_width2, car->_posY+camera_height2);

        canvas->MoveTo(car->_posX, car->_posY);
        canvas->Pen->Color = RGB(255, 255, 0);
        canvas->LineTo(car->_posX+car->_dirX*camera_width2, car->_posY+car->_dirY*camera_height2);
        canvas->MoveTo(mouseX, mouseY);
        canvas->Pen->Color = oldColor;

}                               
//---------------------------------------------------------------------------
void Tmain::shotCamera(int direction)
{
    TCanvas* canvas=map->Canvas;

    // draw camera
    double angle=atan2(car->_dirY, car->_dirX);
    angle+=(90*M_PI/(double)180);
    double dirY=sin(angle);
    double dirX=cos(angle);
    double camera_pixel_width=(double)road->Width / camera_width;
    double camera_pixel_height=(double)road->Height / camera_height;
                
    double *data=(double *)malloc(camera_width * sizeof(double));
    for (int j=-camera_height2; j<camera_height2; j++)
    {
      for (int i=-camera_width2; i<camera_width2; i++)
      {
        road->Canvas->Pen->Color = RGB(0,0,0);
        road->Canvas->Brush->Color=canvas->Pixels[car->_posX+j*car->_dirX + i*dirX][car->_posY+j*car->_dirY + i*dirY];
        //canvas->Pixels[car->_posX+j*car->_dirX + i*dirX][car->_posY+j*car->_dirY + i*dirY];
        road->Canvas->FillRect(TRect(
                camera_pixel_width*(camera_width2-i),
                camera_pixel_height*(camera_height2-j),
                camera_pixel_width*(camera_width2-i+1)-1,
                camera_pixel_height*(camera_height2-j+1)-1
                ));

        if (j==camera_height2-1)
        {
                unsigned int color=(unsigned int)road->Canvas->Brush->Color;
                double value = (double)((color&0xFF+(color>>8)&0xFF+(color>>16)&0xFF))/(3*255);
                data[camera_width2+i]= value;
        }
      }
      if (j==camera_height2-1)
      {
        if (direction == -1)
        {
            //pos->Caption=p;
            if (!trainMode)
            {
                    car->camera(data, camera_width);
            }
        }
        else
        {
              car->add_example(data, camera_width, direction);
              examples->Caption = car->examples_count;
              examples->Update();
        }
      }
    }
    free(data);
}
//---------------------------------------------------------------------------
void __fastcall Tmain::RepaintClick(TObject *Sender)
{
        repaintMap();
}
//---------------------------------------------------------------------------
void __fastcall Tmain::leftClick(TObject *Sender)
{
        repaintMap((trainMode?DIR_RR:-1));
        car->left();
}
//---------------------------------------------------------------------------
void __fastcall Tmain::rightClick(TObject *Sender)
{                    
        repaintMap((trainMode?DIR_LL:-1));
        car->right();
}
//---------------------------------------------------------------------------
void __fastcall Tmain::strongLeftClick(TObject *Sender)
{
        repaintMap((trainMode?DIR_SR:-1));
        car->strong_left();
}
//---------------------------------------------------------------------------
void __fastcall Tmain::strongRightClick(TObject *Sender)
{
        repaintMap((trainMode?DIR_SL:-1));
        car->strong_right();
}
//---------------------------------------------------------------------------
void __fastcall Tmain::RunClick(TObject *Sender)
{             
        status->Caption="";
        timer->Enabled=!timer->Enabled;
        if (timer->Enabled)
        {
                Run->Caption="Stop";
        } else
        {
                Run->Caption="Start";
        }
}
//---------------------------------------------------------------------------
void __fastcall Tmain::timerTimer(TObject *Sender)
{
        car->move();
        repaintMap();
}
//---------------------------------------------------------------------------
void __fastcall Tmain::FormCreate(TObject *Sender)
{
        //labelSpeed->Caption = Ansicar->_speed;
        //car->autotrain();        
}
//---------------------------------------------------------------------------
void __fastcall Tmain::UpDown1Click(TObject *Sender, TUDBtnType Button)
{
        if (Button == btNext)
        {
                car->accellerate();
        } else
        {
                car->decrease();
        }
        labelSpeed->Caption = AnsiString("Speed ") + car->_speed;
}
//---------------------------------------------------------------------------
void __fastcall Tmain::SetCarClick(TObject *Sender)
{
        set_car=true;
}
//---------------------------------------------------------------------------
void __fastcall Tmain::SaveClick(TObject *Sender)
{
        if (SaveDialog->Execute())
        {
                TFileStream *file = new TFileStream(SaveDialog->FileName, fmCreate);
                file->Write(&lines_count, sizeof(int));
                for (int i=0; i<lines_count; i++)
                {
                  file->Write(&roadLinesX[i], sizeof(int));
                  file->Write(&roadLinesY[i], sizeof(int));
                }
                car->save(*file);
                car->save_examples(*file);
                delete file;
        }
}
//---------------------------------------------------------------------------
void __fastcall Tmain::LoadClick(TObject *Sender)
{
        if (OpenDialog->Execute())
        {
                TFileStream *file = new TFileStream(OpenDialog->FileName, fmOpenRead);
                file->Read(&lines_count, sizeof(int));
                for (int i=0; i<lines_count; i++)
                {
                  file->Read(&roadLinesX[i], sizeof(int));
                  file->Read(&roadLinesY[i], sizeof(int));
                }
                car->load(*file);
                car->load_examples(*file);
                examples->Caption = car->examples_count;
                delete file;
        }          
        repaintMap();
}
//---------------------------------------------------------------------------
void __fastcall Tmain::forwardClick(TObject *Sender)
{
        car->move();
        repaintMap((trainMode?DIR_FW:-1));
}
//---------------------------------------------------------------------------

void __fastcall Tmain::trainClick(TObject *Sender)
{
        trainMode=!trainMode;
        if (trainMode)
        {
                train->Caption = "Auto";
                train_mode->Caption = "Mode: Train";
        }
        else
        {
                train->Caption = "Train";
                train_mode->Caption = "Mode: Auto";
        }
}
//---------------------------------------------------------------------------



void __fastcall Tmain::doTrainClick(TObject *Sender)
{
        status->Caption=AnsiString("Training ") + car->examples_count + " examples. Please wait...";
        status->Update();
        car->train();
        status->Caption=status->Caption + "done!";
}
//---------------------------------------------------------------------------

void __fastcall Tmain::TrackBarWidthChange(TObject *Sender)
{
        labelWidth->Caption = cameraLineWidth = TrackBarWidth->Position;
        repaintMap();
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Button1Click(TObject *Sender)
{
        car->resetMind();
        examples->Caption = "0";
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Button2Click(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------

