; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEFisDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "efis.h"
LastPage=0

ClassCount=6
Class1=CCom
Class2=CEFisApp
Class3=CAboutDlg
Class4=CEFisDlg
Class5=CResultDlg

ResourceCount=4
Resource1=IDD_RESULT_DIALOG
Resource2=IDD_ABOUTBOX
Resource3=IDD_PHOTOBOOK_DIALOG
Class6=CPhotoBook
Resource4=IDD_EFIS_DIALOG

[CLS:CCom]
Type=0
BaseClass=CSocket
HeaderFile=Com.h
ImplementationFile=Com.cpp

[CLS:CEFisApp]
Type=0
BaseClass=CWinApp
HeaderFile=eFis.h
ImplementationFile=eFis.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=eFisDlg.cpp
ImplementationFile=eFisDlg.cpp
LastObject=CAboutDlg

[CLS:CEFisDlg]
Type=0
BaseClass=CDialog
HeaderFile=eFisDlg.h
ImplementationFile=eFisDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_REAL_TIME_RECOGN

[CLS:CResultDlg]
Type=0
BaseClass=CDialog
HeaderFile=ResultDlg.h
ImplementationFile=ResultDlg.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_EFIS_DIALOG]
Type=1
Class=CEFisDlg
ControlCount=27
Control1=IDC_PIC_SMALL1,static,1342177289
Control2=IDC_PIC_SMALL2,static,1342177289
Control3=IDC_PIC_SMALL3,static,1342177289
Control4=IDC_PIC_SMALL4,static,1342177289
Control5=IDC_PIC_SMALL5,static,1342177289
Control6=IDC_PIC_MAIN,static,1342177288
Control7=IDC_ID,edit,1342242945
Control8=IDC_INITIALIZE,button,1342242816
Control9=IDC_DETECT_FACE,button,1342242816
Control10=IDC_ENROLLMENT,button,1342242816
Control11=IDC_VERIFY,button,1342242816
Control12=IDC_SAVE_STILL_IMAGE,button,1342242816
Control13=IDC_PROGRESS,msctls_progress32,1350565889
Control14=IDC_PIC_VERIFY_FACE,static,1342181394
Control15=IDC_LB_PERSON,listbox,1352728835
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC2,static,1342308865
Control18=IDC_PERSON_LIST_CNT,edit,1342244993
Control19=IDC_EDIT_OUTPUT,edit,1352735172
Control20=IDC_LB_IMAGE_TITLE,listbox,1352728835
Control21=IDC_STATIC1,static,1342308865
Control22=IDC_TRACKING_EYES,button,1342246915
Control23=IDC_PHOTO_CHECK,button,1342246915
Control24=IDC_STATIC,static,1342177298
Control25=IDC_RECOGNIZE,button,1342242816
Control26=IDC_REAL_TIME_RECOGN,button,1342246915
Control27=IDC_EDIT_DEBUG,edit,1352728708

[DLG:IDD_RESULT_DIALOG]
Type=1
Class=CResultDlg
ControlCount=0

[DLG:IDD_PHOTOBOOK_DIALOG]
Type=1
Class=CPhotoBook
ControlCount=6
Control1=IDC_STATIC,static,1342308352
Control2=IDC_PIC_QUERY,static,1342177298
Control3=IDC_STATIC,static,1342308352
Control4=IDC_PIC_RESULT,static,1342177298
Control5=IDC_RESULT_ID,static,1342308353
Control6=IDC_IMAGELISTCTRL1,{FAF056D6-A683-11D1-BB57-00C04FCCB6BB},1342242816

[CLS:CPhotoBook]
Type=0
HeaderFile=PhotoBook.h
ImplementationFile=PhotoBook.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPhotoBook

