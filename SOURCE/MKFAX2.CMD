@
@rem Make utility for building the OS/2 based fax2.exe example for simple
@rem fax handling. This example has to be compiled with the IBM VisualAge C++
@rem compiler.
@
@   set CAPI20LIB=..\c20lib\os2
@   set CAPI20H=..\c20lib\source
@
@   set icc=/I%CAPI20H% /DTARGET_32BIT /DTARGET_OS2
    icc /Fefax2.exe faxmain.c c20msg.c capi.c connect.c contr.c data.c id.c init.c fax.c %CAPI20LIB%\capi20d.lib
    del *.obj
