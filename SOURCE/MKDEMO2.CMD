@
@rem Make utility for building the OS/2 based demo2.exe example for connection
@rem handling and simple file transfer. This example has to be compiled
@rem with the IBM VisualAge C++ compiler.
@
@   set CAPI20LIB=..\c20lib\os2
@   set CAPI20H=..\c20lib\source
@
@   set icc=/I%CAPI20H% /DTARGET_32BIT /DTARGET_OS2
    icc /Fedemo2.exe main.c c20msg.c capi.c connect.c contr.c data.c id.c init.c %CAPI20LIB%\capi20d.lib
    del *.obj
