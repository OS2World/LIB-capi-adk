@rem
@rem     This is a demo makefile for making the CAPI 2.0 AVM Development Kit
@rem     library with the WATCOM CPP 10.5 C compiler for a 32 bit
@rem     NetWare environment.
@rem     We assume that the Environment variable WATCOM contains the Install
@rem     directory of your Watcom C Compiler (e.g. C:\wc105)
@rem
@rem     If as parameter dbg is given, the symbol CPROT is defined
@rem	 and the library will be build including the protocol functions
@
@    set COMP=%WATCOM%\BINW\wcc386
@
@if "%1" == "dbg" goto dbg
@    set wcc386=/d1 /s /3s /oi  /zq /wx /DB1 /DAVM /DTARGET_32BIT /DNDEBUG /DCPROT /Isource
@    set LIBNAME=cap2032.lib
@    goto compile
:dbg
@    set wcc386=/d2 /s /3s /oid /zq /wx /DB1 /DAVM /DTARGET_32BIT /DDEBUG  /DCPROT /Isource
@    set LIBNAME=cap2032d.lib
@    goto compile

:compile
     %COMP% /bt=netware /fonetware\func.obj     source\func.c
     %COMP% /bt=netware /fonetware\msg.obj      source\msg.c
     %COMP% /bt=netware /fonetware\gcmsg_nw.obj source\gcmsg_nw.c
     %COMP% /bt=netware /fonetware\c_nw.obj     source\c_nw.c

     del  netware\%LIBNAME%
     %WATCOM%\BINW\wlib netware\%LIBNAME% +netware\msg +netware\c_nw +netware\func +netware\gcmsg_nw
     del  netware\*.obj
@    goto ende
@
:ende
