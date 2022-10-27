#$Id: vc6.mak,v 1.1 2003/06/25 19:26:59 amag97 Exp $
#$Author: amag97 $
CC=cl.exe
LL=link.exe
RM=rm

CFLAGS_COMP= -nologo -MDd -W3 -Gm -GX -Zi -Od -D "WIN32" -D "_DEBUG" -D "_CONSOLE" -D "_MBCS" -GZ 

#LFLAGS= -nologo c:/cygwin/usr/local/gmp/lib/libmpfr.a c:/cygwin/usr/local/gmp/lib/libgmp.a c:/cygwin/usr/local/gmp/lib/libgcc.a 

LFLAGS_COMP= -nologo 

