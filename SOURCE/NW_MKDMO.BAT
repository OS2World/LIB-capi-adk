@
@rem  Make utility for building the NetWare based demo.nlm example for
@rem  CAPI 2.0 calls. This example has been compiled with the 32Bit
@rem  Watcom C 10.5 compiler.
@rem  We assume that the Environment variable WATCOM contains the Install
@rem  directory of your Watcom C Compiler (e.g. C:\wc105)
@
@
@   set CAPI20LIB=..\c20lib\netware
@   set CAPI20H=..\c20lib\source
@   set COMP=%WATCOM%\BINW\WCC386
@
@if "%1"=="rel" goto NODEBUG
@   set WCC386=/d2 /s /3s /ox /zq /wx /I%CAPI20H% /DB1 /DAVM /DTARGET_32BIT /DTARGET_NW
goto compile

:NODEBUG
@   set WCC386=/d1 /s /3s /ox /zq /wx /I%CAPI20H% /DNDEBUG /DB1 /DAVM /DTARGET_32BIT /DTARGET_NW
goto compile

:compile
%COMP% /bt=netware /fonetware\main.obj    main.c    >  errors.nlm
%COMP% /bt=netware /fonetware\c20msg.obj  c20msg.c  >> errors.nlm
%COMP% /bt=netware /fonetware\capi.obj    capi.c    >> errors.nlm
%COMP% /bt=netware /fonetware\connect.obj connect.c >> errors.nlm
%COMP% /bt=netware /fonetware\contr.obj   contr.c   >> errors.nlm
%COMP% /bt=netware /fonetware\data.obj    data.c    >> errors.nlm
%COMP% /bt=netware /fonetware\id.obj      id.c      >> errors.nlm
%COMP% /bt=netware /fonetware\init.obj    init.c    >> errors.nlm

%WATCOM%\BINW\wlink @nw_dmo.rsp >> errors.nlm
del netware\*.obj
del *.map

