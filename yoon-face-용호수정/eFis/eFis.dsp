# Microsoft Developer Studio Project File - Name="eFis" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=eFis - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "eFis.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "eFis.mak" CFG="eFis - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "eFis - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "eFis - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "eFis - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Exe"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib"

!ELSEIF  "$(CFG)" == "eFis - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Exe"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "eFis - Win32 Release"
# Name "eFis - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Com.cpp
# End Source File
# Begin Source File

SOURCE=.\eFis.cpp
# End Source File
# Begin Source File

SOURCE=.\eFis.rc
# End Source File
# Begin Source File

SOURCE=.\eFisDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\imagelist.cpp
# End Source File
# Begin Source File

SOURCE=.\PhotoBook.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Com.h
# End Source File
# Begin Source File

SOURCE=.\eFis.h
# End Source File
# Begin Source File

SOURCE=.\eFisDlg.h
# End Source File
# Begin Source File

SOURCE=.\imagelist.h
# End Source File
# Begin Source File

SOURCE=.\PhotoBook.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Accept.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BG.bmp
# End Source File
# Begin Source File

SOURCE=.\res\eFis.ico
# End Source File
# Begin Source File

SOURCE=.\res\eFis.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\Line.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Reject.bmp
# End Source File
# End Group
# Begin Group "ToolBox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\ToolBox\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBox\BtnST.h
# End Source File
# Begin Source File

SOURCE=..\ToolBox\DirTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBox\DirTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToolBox\GradProgress.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBox\GradProgress.h
# End Source File
# Begin Source File

SOURCE=..\ToolBox\Label.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBox\Label.h
# End Source File
# Begin Source File

SOURCE=..\ToolBox\ListCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBox\ListCtrlEx.h
# End Source File
# Begin Source File

SOURCE=..\ToolBox\MacButtons.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBox\MacButtons.h
# End Source File
# Begin Source File

SOURCE=..\ToolBox\MyToolTipCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBox\MyToolTipCtrl.h
# End Source File
# Begin Source File

SOURCE=..\ToolBox\WinXPButtonST.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBox\WinXPButtonST.h
# End Source File
# Begin Source File

SOURCE=..\ToolBox\XTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolBox\XTabCtrl.h
# End Source File
# End Group
# Begin Group "Dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ResultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ResultDlg.h
# End Source File
# End Group
# Begin Group "Src"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\STDEF.EFD
# End Source File
# End Target
# End Project
# Section eFis : {FAF056D6-A683-11D1-BB57-00C04FCCB6BB}
# 	2:21:DefaultSinkHeaderFile:imagelist.h
# 	2:16:DefaultSinkClass:CImageList1
# End Section
# Section eFis : {FAF056D4-A683-11D1-BB57-00C04FCCB6BB}
# 	2:5:Class:CImageList1
# 	2:10:HeaderFile:imagelist.h
# 	2:8:ImplFile:imagelist.cpp
# End Section
