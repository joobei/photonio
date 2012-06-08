# Microsoft Developer Studio Project File - Name="libspuc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libspuc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libspuc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libspuc.mak" CFG="libspuc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libspuc - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libspuc - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libspuc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libspuc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libspuc - Win32 Release"
# Name "libspuc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\comm\baud_eq_env.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\binary.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\bool_nco.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\bped.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\bpsk_ber_test.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\butterworth_fir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\carrier_nco.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\cholesky.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\cic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\commfunc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\convcode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\cordic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\crc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\data_conv_encoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\det.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\dvb_conv_encoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\egolay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\eigen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\fading_channel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\fastmath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\fft.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\fir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\gaussian_fir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\gf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\gfx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\hammcode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\ifft.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\ifft_q.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\inv.cpp
# End Source File
# Begin Source File

SOURCE=..\..\equalizers\lms_dfe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\ls_solve.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\lu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\max_pn.cpp
# End Source File
# Begin Source File

SOURCE=..\..\functions\misc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\equalizers\mlsd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\modulator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\nco.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\noise.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\ofdm_data_encoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\punct_convcode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\qam_mod.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\qam_soft_decision.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\qam_tx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\qpsk.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\qpsk_ber_test.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\qpsk_discriminators.cpp
# End Source File
# Begin Source File

SOURCE=..\..\qpsk\qpsk_variable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\quad_data.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\raised_cosine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\random.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\rec_syst_conv_code.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\reed_solomon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\remez_fir.cpp
# End Source File
# Begin Source File

SOURCE=..\..\equalizers\rls.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\sequence.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\sigdel.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\sim_qam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\sim_qpsk.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\specmat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\functions\spuc_math.cpp
# End Source File
# Begin Source File

SOURCE=..\..\utility\spucassert.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\sum_and_dump.cpp
# End Source File
# Begin Source File

SOURCE=..\..\matrix\svd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\turbo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\generic\vco.cpp
# End Source File
# Begin Source File

SOURCE=..\..\comm\viterbi.cpp
# End Source File
# Begin Source File

SOURCE=..\..\functions\window.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\equalizers\rls.h
# End Source File
# End Group
# End Target
# End Project
