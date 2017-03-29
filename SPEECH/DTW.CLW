; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=Cspinput
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "dtw.h"

ClassCount=4
Class1=CDtwApp
Class2=CDtwDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DTW_DIALOG
Class4=Cspinput
Resource4=IDD_SPEECH_INPUT

[CLS:CDtwApp]
Type=0
HeaderFile=dtw.h
ImplementationFile=dtw.cpp
Filter=N
LastObject=IDC_BUTTON_EXIT

[CLS:CDtwDlg]
Type=0
HeaderFile=dtwDlg.h
ImplementationFile=dtwDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CDtwDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=dtwDlg.h
ImplementationFile=dtwDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DTW_DIALOG]
Type=1
Class=CDtwDlg
ControlCount=8
Control1=IDC_BUTTON_TRAIN,button,1342242816
Control2=IDC_BUTTON_TEST,button,1342242816
Control3=IDC_LIST,listbox,1352728833
Control4=IDC_STATIC,button,1342178055
Control5=IDC_BUTTON_EXIT,button,1342242816
Control6=IDC_BUTTON_DEL,button,1342242816
Control7=IDC_EDIT_WORD,edit,1350631552
Control8=IDC_STATIC,button,1342178055

[DLG:IDD_SPEECH_INPUT]
Type=1
Class=Cspinput
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BUTTON_RECORD,button,1342242816
Control4=IDC_BUTTON_PLAY,button,1342242816
Control5=IDC_STATIC_WAVE,static,1342177284
Control6=IDC_SLIDER1,msctls_trackbar32,1342242840
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,button,1342177287

[CLS:Cspinput]
Type=0
HeaderFile=spinput.h
ImplementationFile=spinput.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=Cspinput

