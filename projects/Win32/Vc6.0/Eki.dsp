# Microsoft Developer Studio Project File - Name="Eki" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Eki - Win32 Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Eki.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Eki.mak" CFG="Eki - Win32 Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Eki - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Eki - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Eki - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Eki - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Eki - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Eki___W"
# PROP BASE Intermediate_Dir "Eki___W"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EkReleasei"
# PROP Intermediate_Dir "EkReleasei"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MD /W2 /GR /GX /O2 /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "EK_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x62300000" /subsystem:windows /dll /machine:I386 /out:"..\..\..\lib\Eki.dll" /implib:"..\..\..\lib\Eki.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Eki - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Eki___0"
# PROP BASE Intermediate_Dir "Eki___0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "EkDebugi"
# PROP Intermediate_Dir "EkDebugi"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MDd /W2 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "EK_BUILD_DLL" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x62300000" /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\lib\Ekid.dll" /implib:"..\..\..\lib\Ekid.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Eki - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Eki___Win32_Unicode_Release"
# PROP BASE Intermediate_Dir "Eki___Win32_Unicode_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EkReleaseiu"
# PROP Intermediate_Dir "EkReleaseiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W2 /GR /GX /O2 /I "..\..\..\src\Ek" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "EK_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W2 /GR /GX /O2 /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "EK_BUILD_DLL" /D "_UNICODE" /D "UNICODE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x62300000" /subsystem:windows /dll /machine:I386 /out:"..\..\..\lib\Eki.dll" /implib:"..\..\..\lib\Eki.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x62300000" /subsystem:windows /dll /machine:I386 /out:"..\..\..\lib\Ekiu.dll" /implib:"..\..\..\lib\Ekiu.lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Eki - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Eki___Win32_Unicode_Debug"
# PROP BASE Intermediate_Dir "Eki___Win32_Unicode_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "EkDebugiu"
# PROP Intermediate_Dir "EkDebugiu"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W2 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "EK_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MDd /W2 /Gm /GR /GX /ZI /Od /I "..\..\..\src\Ek" /I "..\..\..\src\PmtCore" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "EK_BUILD_DLL" /D "_UNICODE" /D "UNICODE" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x62300000" /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\lib\Ekid.dll" /implib:"..\..\..\lib\Ekid.lib" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /base:"0x62300000" /subsystem:windows /dll /debug /machine:I386 /out:"..\..\..\lib\Ekiud.dll" /implib:"..\..\..\lib\Ekiud.lib" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Eki - Win32 Release"
# Name "Eki - Win32 Debug"
# Name "Eki - Win32 Unicode Release"
# Name "Eki - Win32 Unicode Debug"
# Begin Group "Source Files"

# PROP Default_Filter ".cpp"
# Begin Source File

SOURCE=..\..\..\src\Ek\EkCompiler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkNullMutex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkPrecompiled.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkRefCount.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkStrings.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkThread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\ptstring.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ".h"
# Begin Source File

SOURCE=..\..\..\src\Ek\EkBool.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkCompiler.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkCompSmartPtr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkError.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkFactoryT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkGPtr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkGuard.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkLocking.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkMapT.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkNullMutex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkPrecompiled.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkRational.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkRefCount.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkSmartPtr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkStrings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkTemplate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\EkThread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\Ek\ptstring.h
# End Source File
# End Group
# End Target
# End Project
