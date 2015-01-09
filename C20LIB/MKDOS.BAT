@rem
@rem	 This is a demo makefile for making the CAPI 2.0 Development Kit
@rem     library with the Microsoft Visual C compiler for a DOS
@rem     environment.
@rem
@rem	 This makefile expects as first parameter a letter that specifies
@rem	 the memory model to compile with, for example L (large model),
@rem	 S (small model).
@rem	 If as second parameter dbg is given, the symbol CPROT is defined
@rem	 and the library will be build including the protocol functions
@
@
@if "%1" == "" goto NoMemoryModel
@if "%2" == "dbg" goto dbg
@
@    set cl=/c /Zl /A%1 /W4 /Isource /nologo /DTARGET_16BIT
     cl source\func.c source\msg.c source\gcmsgdos.c source\cdos.c
     del dos\capi20%1.lib
@    lib /NOLOGO dos\capi20%1.lib;
     lib /NOLOGO dos\capi20%1.lib +msg+cdos+func+gcmsgdos;
     del *.obj
     del dos\*.bak
@    goto ende
@
:dbg
@
@    set cl=/c /Zl /A%1 /W4 /Isource /nologo /DTARGET_16BIT /DCPROT
     cl  source\func.c source\msg.c source\gcmsgdos.c source\cdos.c
     del dos\capi20%1d.lib
@    lib /NOLOGO dos\capi20%1d.lib;
     lib /NOLOGO dos\capi20%1d.lib +msg+cdos+func+gcmsgdos;
     del *.obj
     del dos\*.bak
@    goto ende
@
:NoMemoryModel
@echo No memory model specified !!
@
:ende
