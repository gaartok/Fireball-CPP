# Microsoft Developer Studio Project File - Name="FireBall" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=FireBall - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FireBall.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FireBall.mak" CFG="FireBall - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FireBall - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "FireBall - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "FireBall"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FireBall - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ddraw.lib dsound.lib winmm.lib dinput.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"FireBall.exe"

!ELSEIF  "$(CFG)" == "FireBall - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib ddraw.lib dsound.lib winmm.lib dinput.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"FireBall_D.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FireBall - Win32 Release"
# Name "FireBall - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "*.h *.hpp"
# Begin Source File

SOURCE=.\AliveObject.hpp
# End Source File
# Begin Source File

SOURCE=.\App.hpp
# End Source File
# Begin Source File

SOURCE=.\Basket.hpp
# End Source File
# Begin Source File

SOURCE=.\Debug.h
# End Source File
# Begin Source File

SOURCE=.\DirectDraw.h
# End Source File
# Begin Source File

SOURCE=.\dsutil.h
# End Source File
# Begin Source File

SOURCE=.\Emitter.hpp
# End Source File
# Begin Source File

SOURCE=.\Global.h
# End Source File
# Begin Source File

SOURCE=.\Hero.hpp
# End Source File
# Begin Source File

SOURCE=.\House.hpp
# End Source File
# Begin Source File

SOURCE=.\input.hpp
# End Source File
# Begin Source File

SOURCE=.\MathStuff.h
# End Source File
# Begin Source File

SOURCE=.\Misc.hpp
# End Source File
# Begin Source File

SOURCE=.\Renderer.hpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\Rock.hpp
# End Source File
# Begin Source File

SOURCE=.\Types.h
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "*.c *.cpp"
# Begin Source File

SOURCE=.\AliveObject.cpp
# End Source File
# Begin Source File

SOURCE=.\App.cpp
# End Source File
# Begin Source File

SOURCE=.\AppUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\Basket.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\DirectDraw.cpp
# End Source File
# Begin Source File

SOURCE=.\dsutil.cpp
# End Source File
# Begin Source File

SOURCE=.\Emitter.cpp
# End Source File
# Begin Source File

SOURCE=.\global.cpp
# End Source File
# Begin Source File

SOURCE=.\Hero.cpp
# End Source File
# Begin Source File

SOURCE=.\House.cpp
# End Source File
# Begin Source File

SOURCE=.\input.cpp
# End Source File
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Misc.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Renderer.cpp
# End Source File
# Begin Source File

SOURCE=.\Rock.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "*.ico, *.bmp, *.wav, *.rc"
# Begin Group "Bitmaps"

# PROP Default_Filter "*.bmp"
# Begin Source File

SOURCE=.\Art\Basket.bmp
# End Source File
# Begin Source File

SOURCE=.\Art\Hero.bmp
# End Source File
# Begin Source File

SOURCE=.\Art\House.bmp
# End Source File
# Begin Source File

SOURCE=.\Art\Numbers.bmp
# End Source File
# Begin Source File

SOURCE=.\Art\Rocks.bmp
# End Source File
# Begin Source File

SOURCE=.\Art\Splash.bmp
# End Source File
# Begin Source File

SOURCE=.\Art\Volcano.bmp
# End Source File
# End Group
# Begin Group "Sounds"

# PROP Default_Filter "*.wav"
# Begin Source File

SOURCE=.\Art\089_argh.wav
# End Source File
# Begin Source File

SOURCE=.\Art\aaah.wav
# End Source File
# Begin Source File

SOURCE=.\Art\agony.wav
# End Source File
# Begin Source File

SOURCE=.\Art\ahhhhh.wav
# End Source File
# Begin Source File

SOURCE=.\Art\applaus1.wav
# End Source File
# Begin Source File

SOURCE=.\Art\applaus2.wav
# End Source File
# Begin Source File

SOURCE=.\Art\b_bang_2.wav
# End Source File
# Begin Source File

SOURCE=.\Art\baby1.wav
# End Source File
# Begin Source File

SOURCE=.\Art\bgsplash.wav
# End Source File
# Begin Source File

SOURCE=.\Art\bonk.wav
# End Source File
# Begin Source File

SOURCE=.\Art\boom.wav
# End Source File
# Begin Source File

SOURCE=.\Art\cheers2.wav
# End Source File
# Begin Source File

SOURCE=.\Art\cool.wav
# End Source File
# Begin Source File

SOURCE=.\Art\crying.wav
# End Source File
# Begin Source File

SOURCE=.\Art\excellnt.wav
# End Source File
# Begin Source File

SOURCE=.\Art\exploson.wav
# End Source File
# Begin Source File

SOURCE=.\Art\glass.wav
# End Source File
# Begin Source File

SOURCE=.\Art\glassbrk.wav
# End Source File
# Begin Source File

SOURCE=.\Art\grtballs.wav
# End Source File
# Begin Source File

SOURCE=.\Art\hallelu.wav
# End Source File
# Begin Source File

SOURCE=.\Art\jiberish.wav
# End Source File
# Begin Source File

SOURCE=.\Art\jokerlau.wav
# End Source File
# Begin Source File

SOURCE=.\Art\kinison.wav
# End Source File
# Begin Source File

SOURCE=.\Art\kook.wav
# End Source File
# Begin Source File

SOURCE=.\Art\laugh1.wav
# End Source File
# Begin Source File

SOURCE=.\Art\laugh2.wav
# End Source File
# Begin Source File

SOURCE=.\Art\master.wav
# End Source File
# Begin Source File

SOURCE=.\Art\mwpunch.wav
# End Source File
# Begin Source File

SOURCE=.\Art\neener.wav
# End Source File
# Begin Source File

SOURCE=.\Art\ni.wav
# End Source File
# Begin Source File

SOURCE=.\Art\ouch.wav
# End Source File
# Begin Source File

SOURCE=.\Art\outahere.wav
# End Source File
# Begin Source File

SOURCE=.\Art\pop.wav
# End Source File
# Begin Source File

SOURCE=.\Art\pop1.wav
# End Source File
# Begin Source File

SOURCE=.\Art\pop2.wav
# End Source File
# Begin Source File

SOURCE=.\Art\scream.wav
# End Source File
# Begin Source File

SOURCE=.\Art\splash.wav
# End Source File
# Begin Source File

SOURCE=.\Art\turnback.wav
# End Source File
# Begin Source File

SOURCE=.\Art\vtoadzil.wav
# End Source File
# Begin Source File

SOURCE=.\Art\water.wav
# End Source File
# Begin Source File

SOURCE=.\Art\wipeout.wav
# End Source File
# Begin Source File

SOURCE=.\Art\woow1.wav
# End Source File
# Begin Source File

SOURCE=.\Art\yelling.wav
# End Source File
# End Group
# Begin Source File

SOURCE=.\Art\FireBall.ico
# End Source File
# Begin Source File

SOURCE=.\FireBall.rc
# End Source File
# End Group
# End Target
# End Project
