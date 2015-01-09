@
@rem  Make utility for building the DOS based faxdemo.exe example for simple
@rem  fax handling. This example has to be compiled with the Microsoft Visual C
@rem  compiler for a DOS.
@
@   set CAPI20LIB=..\c20lib\dos
@   set CAPI20H=..\c20lib\source
@
@   set cl=/c /AL /W4 /I%CAPI20H% /nologo /DTARGET_16BIT
    cl faxmain.c c20msg.c capi.c connect.c contr.c data.c id.c init.c fax.c
    link @makefax.lnk
    del *.obj
    del *.map
