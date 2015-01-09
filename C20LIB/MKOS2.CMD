@rem
@rem	 This is a demo makefile for making the CAPI 2.0 Development Kit
@rem     library with the IBM VisualAge C++ compiler for a 32 bit
@rem     OS/2 environment.
@rem
@rem     If as parameter dbg is given, the symbol CPROT is defined
@rem	 and the library will be build including the protocol functions
@
@
@if "%1" == "dbg" goto dbg
@
@    set icc=/C+ /Isource /DTARGET_32BIT
     icc source\func.c source\msg.c source\gcmsgos2.c source\cos2.c
     if exist os2\capi20.lib del os2\capi20.lib
     ilib /nobrowse os2\capi20.lib msg.obj cos2.obj func.obj gcmsgos2.obj os2\capi20i.lib;
     del *.obj
@    goto end
@
:dbg
@
@    set icc=/C+ /Isource /DTARGET_32BIT /DCPROT
     icc source\func.c source\msg.c source\gcmsgos2.c source\cos2.c
     if exist os2\capi20d.lib del os2\capi20d.lib
     ilib /nobrowse os2\capi20d.lib msg.obj cos2.obj func.obj gcmsgos2.obj os2\capi20i.lib;
     del *.obj
@    goto end
@
:end
