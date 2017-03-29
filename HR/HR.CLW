; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHRView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "HR.h"
LastPage=0

ClassCount=7
Class1=CHRApp
Class2=CHRDoc
Class3=CHRView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_GRAB
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_CONTROLBOX
Class6=Grab
Resource6=IDD_YESNO_DLG
Class7=CYesNoDlg
Resource7=IDR_MAINFRAME (English (U.S.))

[CLS:CHRApp]
Type=0
HeaderFile=HR.h
ImplementationFile=HR.cpp
Filter=N
LastObject=CHRApp

[CLS:CHRDoc]
Type=0
HeaderFile=HRDoc.h
ImplementationFile=HRDoc.cpp
Filter=N
LastObject=CHRDoc

[CLS:CHRView]
Type=0
HeaderFile=HRView.h
ImplementationFile=HRView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CHRView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC



[CLS:CAboutDlg]
Type=0
HeaderFile=HR.cpp
ImplementationFile=HR.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
CommandCount=16
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command16=ID_APP_ABOUT

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
CommandCount=14
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CONTROLBOX]
Type=1
Class=CHRView
ControlCount=37
Control1=IDC_UP,button,1342242816
Control2=IDC_LEFT,button,1342242816
Control3=IDC_RIGHT,button,1342242816
Control4=IDC_DOWN,button,1342242816
Control5=IDC_INIT,button,1342242816
Control6=IDC_STOP,button,1342242816
Control7=IDC_TEST,button,1342242816
Control8=IDC_COMBO1,combobox,1344339970
Control9=IDC_STATIC,button,1342177287
Control10=IDC_ALLSTOP,button,1342242816
Control11=IDC_DEMO,button,1342242816
Control12=IDC_JOYSTICK,button,1342242816
Control13=IDC_TEST2,button,1342242816
Control14=IDC_READY,button,1342242816
Control15=IDC_GRAB,button,1342242816
Control16=IDC_MOVELEFT,button,1342242816
Control17=IDC_MOVERIGHT,button,1342242816
Control18=IDC_RESETARM,button,1342242816
Control19=IDC_HANDRST,button,1342242816
Control20=IDC_TEST4,button,1342242816
Control21=IDC_SENSOR,button,1342242816
Control22=IDC_WALLY,button,1342242816
Control23=IDC_HANDSTP,button,1342242816
Control24=IDC_TEST3,button,1342242816
Control25=IDC_HANDCLS,button,1342242816
Control26=IDC_WALKING,button,1342242816
Control27=IDC_STOPWALK,button,1342242816
Control28=IDC_IDLEENABLE,button,1342242816
Control29=IDC_IDLEDISABLE,button,1342242816
Control30=IDC_CLEAN,button,1342242816
Control31=IDC_HANDINIT,button,1342242816
Control32=IDC_NICEMOTION,button,1342242816
Control33=IDC_NICEMOTION2,button,1342242816
Control34=IDC_NICEMOTION3,button,1342242816
Control35=IDC_PLAYALONE,button,1342242816
Control36=IDC_READREADY,button,1342242816
Control37=IDC_READING,button,1342242816

[DLG:IDD_GRAB]
Type=1
Class=Grab
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_OBJX,edit,1350631552
Control4=IDC_OBJY,edit,1350631552
Control5=IDC_OBJZ,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352

[CLS:Grab]
Type=0
HeaderFile=Grab.h
ImplementationFile=Grab.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=Grab

[DLG:IDD_YESNO_DLG]
Type=1
Class=CYesNoDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352

[CLS:CYesNoDlg]
Type=0
HeaderFile=YesNoDlg.h
ImplementationFile=YesNoDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CYesNoDlg

