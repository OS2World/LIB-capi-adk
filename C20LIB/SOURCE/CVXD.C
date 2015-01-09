/*---------------------------------------------------------------------------*\

    CWIN.C      Version 1.2                                         1997 AVM

    This file contains the source of the operating system specific
    CAPI 2.0 functions, here for the Win95 VxD interface. See CAPI 2.0 spec.

\*---------------------------------------------------------------------------*/
#include <memory.h>

#include "cvxd.h"

/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
static unsigned IsWin40 (void) {

    unsigned    Major;

    VMMCall (Get_VMM_Version);
    __asm {
        movzx   eax, ah
        mov     [Major], eax
    }
    return (Major >= 4);
}

/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
static unsigned CAPIVxDGetVersion (void) {

    unsigned    uRet = 0;

    VxDCall(CAPI20_Get_Version);
    __asm {
        jc      short   VarSet
        mov     [uRet], 1
    VarSet:
    }
    return uRet;
}

/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
static const void *GetDDB (unsigned DeviceId) {

    const void *DDB;

    __asm {
        mov     eax, [DeviceId]
        xor     edi, edi
    }
    VMMCall(Get_DDB);
    __asm {
        mov     [DDB], ecx
    }

    return DDB;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CAPI20_ISINSTALLED (void) {

    unsigned    Installed;

    Installed = CAPIVxDGetVersion ();

    if (!Installed && IsWin40 ()) {
        /*----- try GetDDB for dynamic loadable VxDs -----*/
        Installed = (GetDDB (CAPI20_DEVICE_ID) != 0);
    }
    return Installed;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
_cword CAPI20_REGISTER (unsigned MsgBufSize, unsigned MaxB3Connection, unsigned MaxB3Blks, unsigned MaxSizeB3, CAPI_REGISTER_ERROR *ErrorCode) {

    unsigned    ApplID;

    __asm {
        sub     ebx, ebx                    ; just use EBX compiler will save EBX
        mov     eax, (014h SHL 8) OR 01h
        mov     ecx, [MsgBufSize]
        mov     edx, [MaxB3Connection]
        mov     esi, [MaxB3Blks]
        mov     edi, [MaxSizeB3]
    }
    VxDCall(CAPI20_MessageOperations);
    __asm {
        movzx   eax, ax
        mov     [ApplID], eax
        mov     eax, [ErrorCode]
        mov     [eax], bx
    }
    return (_cword)ApplID;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_RELEASE (unsigned Appl_Id) {

    MESSAGE_EXCHANGE_ERROR Error;

    __asm {
        mov     eax, (014h SHL 8) OR 02h
        mov     edx, [Appl_Id]
    }
    VxDCall(CAPI20_MessageOperations);
    __asm {
        movzx   eax, ax
        mov     [Error],  eax
    }
    return Error;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_PUT_MESSAGE (CAPI_MESSAGE Msg, unsigned Appl_Id) {

    MESSAGE_EXCHANGE_ERROR Error;

    __asm {
        mov     eax, (014h SHL 8) OR 03h
        mov     ebx, [Msg]
        mov     edx, [Appl_Id]
    }
    VxDCall(CAPI20_MessageOperations);
    __asm {
        movzx   eax, ax
        mov     [Error], eax
    }

#if defined (CPROT)
    if (Error == CapiNoError)
        CAPI_PROTOCOL_MESSAGE (Msg);
    else
	CAPI_PROTOCOL_TEXT ("CAPI_PUT_MESSAGE error 0x%04x\n", Error);
#endif

    return Error;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_GET_MESSAGE (unsigned Appl_Id, CAPI_MESSAGE *ReturnMessage) {

    MESSAGE_EXCHANGE_ERROR Error;

    __asm {
        sub     ebx, ebx                    ; just use EBX compiler will save EBX
        mov     eax, (014h SHL 8) OR 04h
        mov     edx, [Appl_Id]
    }
    VxDCall(CAPI20_MessageOperations);
    __asm {
        movzx   eax, ax
        mov     [Error], eax
        mov     eax, [ReturnMessage]
        mov     [eax], ebx
    }

#if defined (CPROT)
    if (Error == CapiNoError)
        CAPI_PROTOCOL_MESSAGE (*ReturnMessage);
    else if (Error != CapiReceiveQueueEmpty)
	CAPI_PROTOCOL_TEXT ("CAPI_GET_MESSAGE error 0x%04x\n", Error);
#endif

    return Error;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_SET_SIGNAL(unsigned ApplId, void (*Callback) (void), unsigned Param) {

    MESSAGE_EXCHANGE_ERROR Error;

    __asm {
        mov     eax, (014h SHL 8) OR 05h
        mov     ebx, [Callback]
        mov     edx, [ApplId]
        mov     edi, [Param]
    }
    VxDCall(CAPI20_MessageOperations);
    __asm {
        movzx   eax, ax
        mov     [Error], eax
    }

#if defined (CPROT)
    if (Error != CapiNoError)
	CAPI_PROTOCOL_TEXT("CAPI_SET_SIGNAL error \n", Error);
#endif
    return Error;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE CAPI20_GET_MANUFACTURER (unsigned Controller, CAPI_MESSAGE LpBuffer) {

    __asm {
        mov     eax, (014h SHL 8) OR 0F0h
        mov     ecx, [Controller]
        mov     ebx, [LpBuffer]
    }
    VxDCall(CAPI20_MessageOperations);

    return LpBuffer;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE CAPI20_GET_VERSION (unsigned Controller, CAPI_MESSAGE version) {

    __asm {
        mov     eax, (014h SHL 8) OR 0F1h
        mov     ecx, [Controller]
    }
    VxDCall(CAPI20_MessageOperations);
    __asm {
        mov     eax, [version]
        mov     [eax+0], ah
        mov     [eax+1], al
        mov     [eax+2], dh
        mov     [eax+3], dl
    }

    return version;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE  CAPI20_GET_SERIAL_NUMBER (unsigned Controller, CAPI_MESSAGE LpBuffer) {

    __asm {
        mov     eax, (014h SHL 8) OR 0F2h
        mov     ecx, [Controller]
        mov     ebx, [LpBuffer]
    }
    VxDCall(CAPI20_MessageOperations)

    return LpBuffer;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_GET_PROFILE (unsigned Controller, CAPI_MESSAGE LpBuffer) {
    MESSAGE_EXCHANGE_ERROR  Error;

    __asm {
        mov     eax, (014h SHL 8) OR 0F3h
        mov     ecx, [Controller]
        mov     ebx, [LpBuffer]
    }
    VxDCall(CAPI20_MessageOperations);
    __asm {
        movzx   eax, ax
        mov     [Error], eax
    }

#if defined (CPROT)
    if (Error != CapiNoError)
	CAPI_PROTOCOL_TEXT("CAPI_GET_PROFILE error \n", Error);
#endif

    return Error;
}

/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
