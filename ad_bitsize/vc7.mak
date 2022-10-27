#$Id: vc7.mak,v 1.4 2003/07/24 21:07:23 amag97 Exp $
#$Author: amag97 $

CC=cl.exe
LL=link.exe
RM=rm

#MSINCDIR= -Ic:/progra~1/micros~1.net/vc7/INCLUDE

#MSLIBDIR= c:/progra~1/micros~1.net/vc7/LIB

CFLAGS_COMP= -nologo -MDd -W3 -Gm -GX -ZI -Od -D "WIN32" -D "_DEBUG" -D "_CONSOLE" -D "_MBCS" -GZ  $(MSINCDIR)
#CFLAGS= /Od /I "C:\cygwin\usr\local\gmp\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /EHsc /RTC1 /MDd /YX"stdafx.h" /Fp".\Debug/multiprec.pch" /Fo".\Debug/" /Fd".\Debug/" /W3 /nologo /c /ZI /TP

#LFLAGS= -nologo c:/cygwin/usr/local/gmp/lib/libmpfr.a c:/cygwin/usr/local/gmp/lib/libgmp.a c:/cygwin/usr/local/gmp/lib/libgcc.a 

LFLAGS_COMP= -nologo 

#-LIBPATH:$(MSLIBDIR)  

