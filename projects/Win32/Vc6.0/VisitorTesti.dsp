# Microsoft Developer Studio Project File - Name="VisitorTesti" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=VisitorTesti - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisitorTesti.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisitorTesti.mak" CFG="VisitorTesti - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisitorTesti - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "VisitorTesti - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisitorTesti - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debugi"
# PROP Intermediate_Dir "Debugi"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\toolkits\ektiff\inc" /I "..\..\toolkits\kies\inc" /I "..\..\toolkits\jpeg\inc" /I "..\..\src\Ek" /I "..\..\src\PmtUtils" /I "..\..\src\PmtCore" /I "..\..\src\PmtInterpreter" /I "..\..\src\PmtAccessor" /I "..\..\src\PmtTranslator" /I "..\..\..\toolkits\ektiff\inc" /I "..\..\..\toolkits\kies\inc" /I "..\..\..\toolkits\jpeg\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /D "PMT_USING_VISITOR" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ekid.lib jpegd.lib ektiffid.lib pmtcoreid.lib pmtaccessorid.lib ekexifio140d.lib ijl15.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /pdbtype:sept /libpath:"..\..\..\toolkits\kies\lib\debug" /libpath:"..\..\lib" /libpath:"..\..\..\toolkits\jpeg\lib" /libpath:"..\..\..\toolkits\ijl\lib" /libpath:"..\..\..\toolkits\ektiff\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "VisitorTesti - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VisitorTesti___Win32_Release"
# PROP BASE Intermediate_Dir "VisitorTesti___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Releasei"
# PROP Intermediate_Dir "Releasei"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".\\"  /I "..\..\src\Ek" /I "..\..\src\PmtUtils" /I "..\..\src\PmtCore" /I "..\..\src\PmtInterpreter" /I "..\..\src\PmtAccessor" /I "..\..\src\PmtTranslator" /I "..\..\toolkits\ektiff\inc" /I "..\..\toolkits\kies\inc" /I "..\..\toolkits\jpeg\inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GR /GX /ZI /Od /I "..\..\src\Ek" /I "..\..\src\PmtUtils" /I "..\..\src\PmtCore" /I "..\..\src\PmtInterpreter" /I "..\..\src\PmtAccessor" /I "..\..\src\PmtTranslator" /I "..\..\..\toolkits\ektiff\inc" /I "..\..\..\toolkits\kies\inc" /I "..\..\..\toolkits\jpeg\inc" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ekid.lib jpegd.lib ektiffid.lib pmtcoreid.lib pmtaccessorid.lib ijl11.lib ekexifio140d.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /pdbtype:sept /libpath:"..\..\toolkits\kies\lib\debug" /libpath:"..\..\lib" /libpath:"..\..\toolkits\jpeg\lib" /libpath:"..\..\toolkits\ijl\lib" /libpath:"..\..\toolkits\ektiff\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 eki.lib jpeg.lib ektiffi.lib pmtcorei.lib pmtaccessori.lib ekexifio140.lib ijl15.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /pdbtype:sept /libpath:"..\..\..\toolkits\kies\lib\release" /libpath:"..\..\lib" /libpath:"..\..\..\toolkits\jpeg\lib" /libpath:"..\..\..\toolkits\ijl\lib" /libpath:"..\..\..\toolkits\ektiff\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "VisitorTesti - Win32 Debug"
# Name "VisitorTesti - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GetMetadataValue.cpp
# End Source File
# Begin Source File

SOURCE=.\VisitorTest.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GetMetadataValue.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
