@
@rem  Make utility for building the DOS based demo.exe example for connection
@rem  handling and simple file transfer. This example has to be compiled
@rem  with the Microsoft Visual C compiler for DOS.
@
@   set CAPI20LIB=..\c20lib\dos
@   set CAPI20H=..\c20lib\source
@
@   set cl=/c /AL /W4 /I%CAPI20H% /nologo /DTARGET_16BIT
    cl main.c c20msg.c capi.c connect.c contr.c data.c id.c init.c
    link @makedemo.lnk
    del *.obj
    del *.map
