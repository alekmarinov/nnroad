//---------------------------------------------------------------------------

#ifndef RoadH
#define RoadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class Car;
class Tmain : public TForm
{
__published:	// IDE-managed Components
        TTimer *timer;
        TImage *road;
        TButton *left;
        TButton *strongLeft;
        TButton *strongRight;
        TButton *right;
        TButton *Load;
        TButton *Clear;
        TButton *Save;
        TPaintBox *map;
        TBevel *Bevel1;
        TBevel *Bevel2;
        TButton *Run;
        TUpDown *UpDown1;
        TLabel *labelSpeed;
        TButton *SetCar;
        TSaveDialog *SaveDialog;
        TOpenDialog *OpenDialog;
        TButton *forward;
        TButton *train;
        TLabel *train_mode;
        TButton *doTrain;
        TLabel *status;
        TLabel *Label1;
        TLabel *examples;
        TTrackBar *TrackBarWidth;
        TLabel *labelWidth;
        TButton *Button1;
        TButton *Button2;
        void __fastcall mapMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall mapMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
        void __fastcall mapMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall ClearClick(TObject *Sender);
        void __fastcall mapPaint(TObject *Sender);
        void __fastcall RepaintClick(TObject *Sender);
        void __fastcall leftClick(TObject *Sender);
        void __fastcall rightClick(TObject *Sender);
        void __fastcall strongLeftClick(TObject *Sender);
        void __fastcall strongRightClick(TObject *Sender);
        void __fastcall RunClick(TObject *Sender);
        void __fastcall timerTimer(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall UpDown1Click(TObject *Sender, TUDBtnType Button);
        void __fastcall SetCarClick(TObject *Sender);
        void __fastcall SaveClick(TObject *Sender);
        void __fastcall LoadClick(TObject *Sender);
        void __fastcall forwardClick(TObject *Sender);
        void __fastcall trainClick(TObject *Sender);
        void __fastcall doTrainClick(TObject *Sender);
        void __fastcall TrackBarWidthChange(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
        bool drawing;
        bool set_car;
        bool trainMode;
        bool autoTrainMode;
        int mouseX,mouseY;
        int roadLinesX[10000];
        int roadLinesY[10000];
        int lines_count;
        void repaintMap(int dir=-1);
        void shotCamera(int direction=-1);
        Car *car;
        int camera_width, camera_width2;
        int camera_height, camera_height2;
        int cameraLineWidth;

public:		// User declarations
        __fastcall Tmain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Tmain *main;
//---------------------------------------------------------------------------
#endif
