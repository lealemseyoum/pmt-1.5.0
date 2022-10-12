# Microsoft Developer Studio Project File - Name="PmtInterpreterTesti" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=PmtInterpreterTesti - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PmtInterpreterTesti.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PmtInterpreterTesti.mak" CFG="PmtInterpreterTesti - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PmtInterpreterTesti - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "PmtInterpreterTesti - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "PmtInterpreterTesti - Win32 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "PmtInterpreterTesti - Win32 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PmtInterpreterTesti - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtInterpreterTestiDebugi"
# PROP Intermediate_Dir "PmtInterpreterTestiDebugi"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtUtils" /I "..\..\..\inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /FR /YX /FD /Zm200 /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Ekid.lib PmtCoreid.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\..\..\test\PmtInterpreterTest\Debugi\PmtInterpreterTesti.exe" /pdbtype:sept /libpath:"..\..\..\lib" 
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PmtInterpreterTesti - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PmtInterpreterTesti___Win32_Release"
# PROP BASE Intermediate_Dir "PmtInterpreterTesti___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtInterpreterTestiReleasei"
# PROP Intermediate_Dir "PmtInterpreterTestiReleasei"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\src\Ek" /I "..\..\src\PmtInterpreter" /I "..\..\src\PmtCore" /I "..\..\src\PmtUtils" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtUtils" /I "..\..\..\inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /FR /YX /FD /Zm200 /Zm200 /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib Ekid.lib PmtCoreid.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /pdbtype:sept /libpath:"..\..\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Eki.lib PmtCorei.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\..\..\test\PmtInterpreterTest\Releasei\PmtInterpreterTesti.exe" /pdbtype:sept /libpath:"..\..\..\lib" 
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PmtInterpreterTesti - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PmtInterpreterTesti___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "PmtInterpreterTesti___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtInterpreterTestiReleaseiu"
# PROP Intermediate_Dir "PmtInterpreterTestiReleaseiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /Gm /GR /GX /ZI /Od /I "..\..\src\Ek" /I "..\..\src\PmtInterpreter" /I "..\..\src\PmtCore" /I "..\..\src\PmtUtils" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtUtils" /I "..\..\..\inc" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /D "_UNICODE" /D "UNICODE" /YX /FD /Zm200 /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib Eki.lib PmtCorei.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /pdbtype:sept /libpath:"..\..\lib" 
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Ekiu.lib PmtCoreiu.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\..\..\test\PmtInterpreterTest\Releaseiu\PmtInterpreterTesti.exe" /pdbtype:sept /libpath:"..\..\..\lib" 
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PmtInterpreterTesti - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PmtInterpreterTesti___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "PmtInterpreterTesti___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "PmtInterpreterTestiDebugiu"
# PROP Intermediate_Dir "PmtInterpreterTestiDebugiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\src\Ek" /I "..\..\src\PmtInterpreter" /I "..\..\src\PmtCore" /I "..\..\src\PmtUtils" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtUtils" /I "..\..\..\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /D "_UNICODE" /D "UNICODE" /YX /FD /Zm200 /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib Ekid.lib PmtCoreid.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /pdbtype:sept /libpath:"..\..\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 Ekiud.lib PmtCoreiud.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\..\..\test\PmtInterpreterTest\Debugiu\PmtInterpreterTesti.exe" /pdbtype:sept /libpath:"..\..\..\lib" 
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PmtInterpreterTesti - Win32 Debug"
# Name "PmtInterpreterTesti - Win32 Release"
# Name "PmtInterpreterTesti - Win32 Unicode Release"
# Name "PmtInterpreterTesti - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\test\PmtInterpreterTest\PmtAPITestPmtCompMetadata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\PmtInterpreterTest\PmtAPITestPmtMetadata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\PmtInterpreterTest\PmtAPITestPmtMetadataT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\test\PmtInterpreterTest\PmtInterpreterTest.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\test\PmtInterpreterTest\PmtAPITestPmtCompMetadata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\test\PmtInterpreterTest\PmtAPITestPmtMetadata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\test\PmtInterpreterTest\PmtAPITestPmtMetadataT.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
