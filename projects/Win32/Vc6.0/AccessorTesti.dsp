# Microsoft Developer Studio Project File - Name="AccessorTesti" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=AccessorTesti - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AccessorTesti.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AccessorTesti.mak" CFG="AccessorTesti - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AccessorTesti - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "AccessorTesti - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AccessorTesti - Win32 Unicode Release" (based on "Win32 (x86) Console Application")
!MESSAGE "AccessorTesti - Win32 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AccessorTesti - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AccessorTestiDebugi"
# PROP Intermediate_Dir "AccessorTestiDebugi"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "..\..\..\inc" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /D "EXIF_DLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ekid.lib ektiffid.lib pmtcoreid.lib pmtaccessorid.lib openexifid.lib jpeg.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\..\..\test\AccessorTest\Debugi\AccessorTesti.exe" /pdbtype:sept /libpath:"$(EXIFROOT)\lib\debug" /libpath:"..\..\..\lib" /libpath:"$(JPEGROOT)\lib" /libpath:"$(EXIFROOT)\lib" /libpath:"$(TIFFROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AccessorTesti - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AccessorTesti___Win32_Release"
# PROP BASE Intermediate_Dir "AccessorTesti___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AccessorTestiReleasei"
# PROP Intermediate_Dir "AccessorTestiReleasei"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".\\" /I "..\..\src\Ek" /I "..\..\src\PmtUtils" /I "..\..\src\PmtCore" /I "..\..\src\PmtInterpreter" /I "..\..\src\PmtAccessor" /I "..\..\src\PmtTranslator" /I "..\..\toolkits\ektiff\inc" /I "..\..\toolkits\kies\inc" /I "..\..\toolkits\jpeg\inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /GR /GX /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "..\..\..\inc" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /D "EXIF_DLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib ekid.lib jpegd.lib ektiffid.lib pmtcoreid.lib pmtaccessorid.lib ijl11.lib ekexifio140d.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /pdbtype:sept /libpath:"..\..\toolkits\kies\lib\debug" /libpath:"..\..\lib" /libpath:"..\..\toolkits\jpeg\lib" /libpath:"..\..\toolkits\ijl\lib" /libpath:"..\..\toolkits\ektiff\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 eki.lib ektiffi.lib pmtcorei.lib pmtaccessori.lib openexifi.lib jpeg.lib /nologo /subsystem:console /pdb:none /machine:I386 /out:"..\..\..\test\AccessorTest\Releasei\AccessorTesti.exe" /libpath:"$(EXIFROOT)\lib\release" /libpath:"..\..\..\lib" /libpath:"$(JPEGROOT)\lib" /libpath:"$(EXIFROOT)\lib" /libpath:"$(TIFFROOT)\lib"

!ELSEIF  "$(CFG)" == "AccessorTesti - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AccessorTesti___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "AccessorTesti___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AccessorTestiReleaseiu"
# PROP Intermediate_Dir "AccessorTestiReleaseiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GR /GX /O2 /I "..\..\src\Ek" /I "..\..\src\PmtUtils" /I "..\..\src\PmtCore" /I "..\..\src\PmtInterpreter" /I "..\..\src\PmtAccessor" /I "..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(KIESEXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /D "HAVE_OPENEXIF" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "..\..\..\inc" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /D "EXIF_DLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib eki.lib jpeg.lib ektiffi.lib pmtcorei.lib pmtaccessori.lib ekexifio140.lib ijl15.lib /nologo /subsystem:console /pdb:none /machine:I386 /libpath:"$(KIESEXIFROOT)\lib\release" /libpath:"..\..\lib" /libpath:"$(JPEGROOT)\lib" /libpath:"..\..\toolkits\ijl\lib" /libpath:"$(TIFFROOT)\lib"
# SUBTRACT BASE LINK32 /map /debug
# ADD LINK32 ekiu.lib ektiffi.lib pmtcoreiu.lib pmtaccessoriu.lib openexifi.lib jpeg.lib /nologo /subsystem:console /pdb:none /machine:I386 /out:"..\..\..\test\AccessorTest\Releaseiu\AccessorTesti.exe" /libpath:"$(EXIFROOT)\lib\release" /libpath:"..\..\..\lib" /libpath:"$(JPEGROOT)\lib" /libpath:"$(EXIFROOT)\lib" /libpath:"$(TIFFROOT)\lib"

!ELSEIF  "$(CFG)" == "AccessorTesti - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "AccessorTesti___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "AccessorTesti___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "AccessorTestiDebugiu"
# PROP Intermediate_Dir "AccessorTestiDebugiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\src\Ek" /I "..\..\src\PmtUtils" /I "..\..\src\PmtCore" /I "..\..\src\PmtInterpreter" /I "..\..\src\PmtAccessor" /I "..\..\src\PmtTranslator" /I "$(TIFFROOT)\inc" /I "$(KIESEXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /D "HAVE_OPENEXIF" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtUtils" /I "..\..\..\src\PmtCore" /I "..\..\..\src\PmtInterpreter" /I "..\..\..\src\PmtAccessor" /I "..\..\..\src\PmtTranslator" /I "..\..\..\inc" /I "$(TIFFROOT)\inc" /I "$(EXIFROOT)\inc" /I "$(JPEGROOT)\inc" /D "_DEBUG" /D "_UNICODE" /D "UNICODE" /D "WIN32" /D "_CONSOLE" /D "EK_DLL" /D "PMT_DLL" /D "EXIF_DLL" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ole32.lib ekid.lib jpegd.lib ektiffid.lib pmtcoreid.lib pmtaccessorid.lib ekexifio140d.lib ijl15.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /pdbtype:sept /libpath:"$(KIESEXIFROOT)\lib\debug" /libpath:"..\..\lib" /libpath:"$(JPEGROOT)\lib" /libpath:"..\..\toolkits\ijl\lib" /libpath:"$(TIFFROOT)\lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ekiud.lib ektiffid.lib pmtcoreiud.lib pmtaccessoriud.lib openexifid.lib jpeg.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"LIBCD" /out:"..\..\..\test\AccessorTest\Debugiu\AccessorTesti.exe" /pdbtype:sept /libpath:"$(EXIFROOT)\lib\debug" /libpath:"..\..\..\lib" /libpath:"$(JPEGROOT)\lib" /libpath:"$(EXIFROOT)\lib" /libpath:"$(TIFFROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "AccessorTesti - Win32 Debug"
# Name "AccessorTesti - Win32 Release"
# Name "AccessorTesti - Win32 Unicode Release"
# Name "AccessorTesti - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\test\AccessorTest\AccessorTest.cpp
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
