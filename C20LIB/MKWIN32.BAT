@rem
@rem	 This is a demo makefile for making the CAPI 2.0 Development Kit
@rem     library with the Microsoft Visual C compiler for a 32 bit
@rem     Windows environment.
@rem
@rem     If as parameter dbg is given, the symbol CPROT is defined
@rem	 and the library will be build including the protocol functions
@
@
@if "%1" == "dbg" goto dbg
@
@    set cl=/c /Zl /W3 /Isource /nologo /DTARGET_32BIT
     cl  source\func.c source\msg.c source\gcmsgwin.c source\cwin32.c
     del win32\cap2032.lib
     lib /OUT:win32\cap2032.lib msg.obj cwin32.obj func.obj gcmsgwin.obj win32\capi2032.lib
     del *.obj
@    goto ende
@
:dbg
@
@    set cl=/c /Zl /W3 /Isource /nologo /DTARGET_32BIT /DCPROT
     cl source\func.c source\msg.c source\gcmsgwin.c source\cwin32.c
     del win32\cap2032d.lib
     lib /OUT:win32\cap2032d.lib msg.obj cwin32.obj func.obj gcmsgwin.obj win32\capi2032.lib
     del *.obj
@    goto ende
@
:ende
