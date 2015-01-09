@
@rem  Make utility for building the Win32 based demo32.exe example for
@rem  connection handling and simple file transfer. This example has to be
@rem  compiled with the Microsoft Visual C for Win32.
@
@   set CAPI20LIB=..\c20lib\win32
@   set CAPI20H=..\c20lib\source
@
@   set cl=-DWIN32 -DCON -Di386 -D_X86_ -D_NTWIN -W3 -Gs -D_DEBUG -Zi
@   set cl=%cl% /c /I%CAPI20H% /nologo /DTARGET_32BIT
    cl main.c c20msg.c capi.c connect.c contr.c data.c id.c init.c
    link @mkdem32.lnk
    del *.obj
    del *.ilk
    del *.pdb
