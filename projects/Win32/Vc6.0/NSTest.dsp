# Microsoft Developer Studio Project File - Name="NSTest" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=NSTest - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NSTest.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NSTest.mak" CFG="NSTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NSTest - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "NSTest - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NSTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "NSTestRelease"
# PROP Intermediate_Dir "NSTestRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "$(TIFFROOT)\inc" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "..\..\..\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 Ek.lib PmtCore.lib PmtAccessor.lib jpeg.lib ektiff.lib openexif.lib /nologo /subsystem:console /machine:I386 /out:"..\..\..\test\NSTest\Release\NSTest.exe" /libpath:"$(EXIFROOT)\lib\debug" /libpath:"..\..\..\lib" /libpath:"$(EXIFROOT)\lib" /libpath:"$(TIFFROOT)\lib" /libpath:"$(JPEGROOT)\lib"

!ELSEIF  "$(CFG)" == "NSTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "NSTestDebug"
# PROP Intermediate_Dir "NSTestDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "$(TIFFROOT)\inc" /I "..\..\..\src\Ek" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "..\..\..\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 EkD.lib PmtCoreD.lib PmtAccessorD.lib jpeg.lib ektiffd.lib openexifd.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\test\NSTest\Debug\NSTest.exe" /pdbtype:sept /libpath:"$(EXIFROOT)\lib\debug" /libpath:"..\..\..\lib" /libpath:"$(EXIFROOT)\lib" /libpath:"$(TIFFROOT)\lib" /libpath:"$(JPEGROOT)\lib"

!ENDIF 

# Begin Target

# Name "NSTest - Win32 Release"
# Name "NSTest - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\test\NSTest\NSTest.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
