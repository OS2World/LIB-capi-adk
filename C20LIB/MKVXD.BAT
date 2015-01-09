@rem
@rem	 This is a demo makefile for making the CAPI 2.0 Development Kit
@rem     library with the Microsoft Visual C compiler for a 32 bit
@rem     VxD driver environment. You have to run this with a installed
@rem     Win95 DDK (%DDKROOT%).
@rem
@rem     If as parameter dbg is given, the symbol CPROT is defined
@rem	 and the library will be build including the protocol functions
@
@
@if "%1" == "dbg" goto dbg
@
@    set cl=/Zdp /Gs /DIS_32 /Zl /c /W3 /Isource /I%DDKROOT%\INC32 /nologo /DTARGET_32BIT
     cl  source\func.c source\msg.c source\gcmsgvxd.c source\cvxd.c
     del vxd\capi20.clb
     lib /OUT:vxd\capi20.clb msg.obj cvxd.obj func.obj gcmsgvxd.obj
     del *.obj
@    goto ende
@
:dbg
@
@    set cl=/Zdp /Gs /DIS_32 /Zl /c /W3 /Isource /I%DDKROOT%\INC32 /nologo /DTARGET_32BIT /DCPROT
     cl source\func.c source\msg.c source\gcmsgvxd.c source\cvxd.c
     del vxd\capi20d.clb
     lib /OUT:vxd\capi20d.clb msg.obj cvxd.obj func.obj gcmsgvxd.obj
     del *.obj
@    goto ende
@
:ende
