object main: Tmain
  Left = 236
  Top = 141
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Neuron Networks'
  ClientHeight = 613
  ClientWidth = 862
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 376
    Top = 16
    Width = 457
    Height = 537
    Style = bsRaised
  end
  object map: TPaintBox
    Left = 384
    Top = 24
    Width = 441
    Height = 521
    Cursor = crCross
    Color = clBlack
    ParentColor = False
    OnMouseDown = mapMouseDown
    OnMouseMove = mapMouseMove
    OnMouseUp = mapMouseUp
    OnPaint = mapPaint
  end
  object Bevel2: TBevel
    Left = 96
    Top = 16
    Width = 169
    Height = 153
  end
  object road: TImage
    Left = 100
    Top = 20
    Width = 161
    Height = 145
  end
  object labelSpeed: TLabel
    Left = 160
    Top = 360
    Width = 40
    Height = 13
    Caption = 'Speed 1'
  end
  object train_mode: TLabel
    Left = 152
    Top = 272
    Width = 57
    Height = 13
    Caption = 'Mode: Train'
  end
  object status: TLabel
    Left = 67
    Top = 544
    Width = 3
    Height = 13
  end
  object Label1: TLabel
    Left = 136
    Top = 448
    Width = 73
    Height = 20
    Caption = 'Examples:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object examples: TLabel
    Left = 216
    Top = 448
    Width = 11
    Height = 20
    Caption = '0'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object labelWidth: TLabel
    Left = 432
    Top = 576
    Width = 6
    Height = 13
    Caption = '1'
  end
  object left: TButton
    Left = 128
    Top = 184
    Width = 33
    Height = 25
    Caption = '<'
    TabOrder = 0
    OnClick = leftClick
  end
  object strongLeft: TButton
    Left = 88
    Top = 184
    Width = 33
    Height = 25
    Caption = '<<'
    TabOrder = 1
    OnClick = strongLeftClick
  end
  object strongRight: TButton
    Left = 240
    Top = 184
    Width = 33
    Height = 25
    Caption = '>>'
    TabOrder = 2
    OnClick = strongRightClick
  end
  object right: TButton
    Left = 200
    Top = 184
    Width = 33
    Height = 25
    Caption = '>'
    TabOrder = 3
    OnClick = rightClick
  end
  object Load: TButton
    Left = 464
    Top = 568
    Width = 65
    Height = 25
    Caption = 'Load'
    TabOrder = 4
    OnClick = LoadClick
  end
  object Clear: TButton
    Left = 620
    Top = 568
    Width = 69
    Height = 25
    Caption = 'Clear'
    TabOrder = 5
    OnClick = ClearClick
  end
  object Save: TButton
    Left = 540
    Top = 568
    Width = 69
    Height = 25
    Caption = 'Save'
    TabOrder = 6
    OnClick = SaveClick
  end
  object Run: TButton
    Left = 168
    Top = 568
    Width = 75
    Height = 25
    Caption = 'Start'
    TabOrder = 7
    OnClick = RunClick
  end
  object UpDown1: TUpDown
    Left = 160
    Top = 312
    Width = 41
    Height = 41
    Min = 0
    Position = 0
    TabOrder = 8
    Wrap = False
    OnClick = UpDown1Click
  end
  object SetCar: TButton
    Left = 700
    Top = 568
    Width = 69
    Height = 25
    Caption = 'Set Car'
    TabOrder = 9
    OnClick = SetCarClick
  end
  object forward: TButton
    Left = 168
    Top = 184
    Width = 25
    Height = 25
    Caption = '|'
    TabOrder = 10
    OnClick = forwardClick
  end
  object train: TButton
    Left = 144
    Top = 232
    Width = 75
    Height = 25
    Caption = 'Auto'
    TabOrder = 11
    OnClick = trainClick
  end
  object doTrain: TButton
    Left = 88
    Top = 568
    Width = 75
    Height = 25
    Caption = 'Do Train'
    TabOrder = 12
    OnClick = doTrainClick
  end
  object TrackBarWidth: TTrackBar
    Left = 272
    Top = 568
    Width = 153
    Height = 33
    Min = 1
    Orientation = trHorizontal
    Frequency = 1
    Position = 1
    SelEnd = 0
    SelStart = 0
    TabOrder = 13
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = TrackBarWidthChange
  end
  object Button1: TButton
    Left = 8
    Top = 568
    Width = 75
    Height = 25
    Caption = 'Reset'
    TabOrder = 14
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 780
    Top = 568
    Width = 69
    Height = 25
    Caption = 'Exit'
    TabOrder = 15
    OnClick = Button2Click
  end
  object timer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = timerTimer
    Left = 280
  end
  object SaveDialog: TSaveDialog
    Left = 312
  end
  object OpenDialog: TOpenDialog
    Left = 344
  end
end
