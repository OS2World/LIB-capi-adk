/*---------------------------------------------------------------------------*\

    CDOS.C      Version 1.2                                         1997 AVM

    This file contains the source of the operating system specific
    CAPI functions, here for DOS. See CAPI 2.0 spec.

\*---------------------------------------------------------------------------*/
#include <dos.h>
#include <memory.h>

#include "capi20.h"

#define CAPI_REGISTER_NR           0x01
#define CAPI_RELEASE_NR            0x02
#define CAPI_PUT_MESSAGE_NR        0x03
#define CAPI_GET_MESSAGE_NR        0x04
#define CAPI_SET_SIGNAL_NR         0x05
#define CAPI_DEINSTALL_NR          0x06
#define CAPI_GET_MANUFACTURER_NR   0xF0
#define CAPI_GET_VERSION_NR        0xF1
#define CAPI_GET_SERIAL_NUMBER_NR  0xF2
#define CAPI_GET_PROFILE_NR        0xF3
#define CAPI_MANUFACTURER_NR       0xFF

static int TrapNumber = 0xF1;

/*---------------------------------------------------------------------------*/
unsigned CAPI20_ISINSTALLED (void) {
    char __far *p = (char __far *) (long) _dos_getvect (TrapNumber);
    return 0 == _fmemcmp (p+11, "CAPI20", 6);
}

/*---------------------------------------------------------------------------*/
void CAPI20_SET_TRAP (unsigned char Number) {
    TrapNumber = Number;
}

/*---------------------------------------------------------------------------*/
_cword CAPI20_REGISTER (CAPI_MESSAGE Buffer, unsigned MsgBufSize, unsigned MaxB3Connection, unsigned MaxB3Blks, unsigned MaxSizeB3, CAPI_REGISTER_ERROR *ErrorCode) {
    struct _SREGS Segs;
    union _REGS Regs;

    Regs.h.ah = 20;
    Regs.h.al = CAPI_REGISTER_NR;

    Regs.x.cx = MsgBufSize;
    Regs.x.dx = MaxB3Connection;
    Regs.x.si = MaxB3Blks;
    Regs.x.di = MaxSizeB3;

    Regs.x.bx = _FP_OFF (Buffer);
    Segs.es =   _FP_SEG (Buffer);

    _int86x (TrapNumber, &Regs, &Regs, &Segs);

    *ErrorCode = (CAPI_REGISTER_ERROR) Regs.x.bx;

#if defined (CPROT)
    if (Regs.x.ax == 0x0000)
        CAPI_PROTOCOL_TEXT ("CAPI_REGISTER error \n", Regs.x.bx);
#endif

    return (_cword) Regs.x.ax;
}

/*---------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_RELEASE (unsigned Appl_Id) {
    struct _SREGS Segs;
    union _REGS  Regs;

    Regs.h.ah = 20;
    Regs.h.al = CAPI_RELEASE_NR;

    Regs.x.dx = Appl_Id;

    _int86x(TrapNumber, &Regs, &Regs, &Segs);

#if defined (CPROT)
    if (Regs.x.ax != 0x0000)
        CAPI_PROTOCOL_TEXT ("CAPI_RELEASE error 0x%04x\n", Regs.x.ax);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Regs.x.ax;
}

/*---------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_PUT_MESSAGE (CAPI_MESSAGE Msg, unsigned Appl_Id) {
    struct _SREGS Segs;
    union _REGS  Regs;

    Regs.h.ah = 20;
    Regs.h.al = CAPI_PUT_MESSAGE_NR;

    Regs.x.dx = Appl_Id;
    Regs.x.bx = _FP_OFF (Msg);
    Segs.es   = _FP_SEG (Msg);

    _int86x(TrapNumber, &Regs, &Regs, &Segs);

#if defined (CPROT)
    if (Regs.x.ax == 0x0000)
        CAPI_PROTOCOL_MESSAGE (Msg);
    else
        CAPI_PROTOCOL_TEXT ("CAPI_PUT_MESSAGE error 0x%04x\n", Regs.x.ax);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Regs.x.ax;
}

/*---------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_GET_MESSAGE (unsigned Appl_Id, CAPI_MESSAGE  __far * ReturnMessage) {
    struct _SREGS Segs;
    union _REGS  Regs;

    Regs.h.ah = 20;
    Regs.h.al = CAPI_GET_MESSAGE_NR;

    Regs.x.dx = Appl_Id;

    _int86x(TrapNumber, &Regs, &Regs, &Segs);

    (*((unsigned __far *)&(*ReturnMessage))) = Regs.x.bx;
    (*((unsigned __far *)&(*ReturnMessage) + 1)) = Segs.es;

#if defined (CPROT)
    if (Regs.x.ax == 0x0000)
        CAPI_PROTOCOL_MESSAGE (*ReturnMessage);
    else if (Regs.x.ax != CapiReceiveQueueEmpty)
        CAPI_PROTOCOL_TEXT ("CAPI_GET_MESSAGE error 0x%04x\n", Regs.x.ax);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Regs.x.ax;
}
/*---------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_SET_SIGNAL (unsigned ApplId, void (__far * CallBack) (void), void __far *CallBackPar) {
    struct _SREGS Segs;
    union _REGS  Regs;

    Regs.h.ah = 20;
    Regs.h.al = CAPI_SET_SIGNAL_NR;

    Regs.x.dx = ApplId;
    Regs.x.di = _FP_OFF (CallBackPar);
    Regs.x.si = _FP_SEG (CallBackPar);

    Regs.x.bx = _FP_OFF (CallBack);
    Segs.es   = _FP_SEG (CallBack);

    _int86x (TrapNumber, &Regs, &Regs, &Segs);

#if defined (CPROT)
    if (Regs.x.ax != 0x0000)
        CAPI_PROTOCOL_TEXT ("CAPI_SET_SIGNAL error \n", Regs.x.ax);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Regs.x.ax;
}

/*---------------------------------------------------------------------------*/
CAPI_MESSAGE CAPI20_GET_MANUFACTURER (CAPI_MESSAGE LpBuffer) {
    struct _SREGS Segs;
    union _REGS  Regs;

    Regs.h.ah = 20;
    Regs.h.al = CAPI_GET_MANUFACTURER_NR;

    Regs.x.bx = _FP_OFF (LpBuffer);
    Segs.es   = _FP_SEG (LpBuffer);

    _int86x(TrapNumber, &Regs, &Regs, &Segs);
    return LpBuffer;
}

/*---------------------------------------------------------------------------*/
CAPI_MESSAGE CAPI20_GET_VERSION (CAPI_MESSAGE version) {
    struct _SREGS Segs;
    union _REGS  Regs;

    Regs.h.ah = 20;
    Regs.h.al = CAPI_GET_VERSION_NR;

    _int86x(TrapNumber, &Regs, &Regs, &Segs);

    version[0] = Regs.h.ah;
    version[1] = Regs.h.al;
    version[2] = Regs.h.dh;
    version[3] = Regs.h.dl;
    return version;
}

/*---------------------------------------------------------------------------*/
CAPI_MESSAGE  CAPI20_GET_SERIAL_NUMBER (CAPI_MESSAGE LpBuffer) {
    struct _SREGS Segs;
    union _REGS  Regs;

    Regs.h.ah = 20;
    Regs.h.al = CAPI_GET_SERIAL_NUMBER_NR;

    Regs.x.bx = _FP_OFF (LpBuffer);
    Segs.es   = _FP_SEG (LpBuffer);

    _int86x(TrapNumber, &Regs, &Regs, &Segs);
    return LpBuffer;
}

/*---------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_GET_PROFILE (unsigned Controller, CAPI_MESSAGE LpBuffer) {
    struct _SREGS Segs;
    union _REGS  Regs;

    Regs.h.ah = 20;
    Regs.h.al = CAPI_GET_PROFILE_NR;

    Regs.x.cx = Controller;
    Regs.x.bx = _FP_OFF (LpBuffer);
    Segs.es   = _FP_SEG (LpBuffer);

    _int86x(TrapNumber, &Regs, &Regs, &Segs);

#if defined (CPROT)
    if (Regs.x.ax != 0x0000)
	CAPI_PROTOCOL_TEXT ("CAPI_GET_PROFILE error \n", Regs.x.ax);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Regs.x.ax;
}
