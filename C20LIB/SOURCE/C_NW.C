/*---------------------------------------------------------------------------*\

    C_NW.C      Version 1.0                                         1998 AVM

    CAPI 2.0 Development Kit NetWare

    This file contains the source of the operating system specific
    CAPI functions, here for Novell NetWare. See CAPI 2.0 spec.

\*---------------------------------------------------------------------------*/
#include <assert.h>
#include <nwsemaph.h>
#include <nwconio.h>

#include "c_nw.h"
#include "c2imp_nw.h"
#include "c2mgr_nw.h"
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
static LONG GetMessageSemaphore = -1;
/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CAPI20_ISINSTALLED (void) {
    return 1;       // loader refuses to load if it can not find the symbols
}

/*-----------------------------##########################*/
static void APPL_ReceiveNotify (unsigned long signalContext) {
    SignalLocalSemaphore (GetMessageSemaphore);  // enables CAPI_GetMessage
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
_cword CAPI20_REGISTER (unsigned MsgBufSize, unsigned MaxB3Connection, unsigned MaxB3Blks, unsigned MaxSizeB3, CAPI_REGISTER_ERROR *ErrorCode) {
    WORD    ApplId;

    assert (GetMessageSemaphore == -1);
    GetMessageSemaphore = OpenLocalSemaphore (0);

    *ErrorCode = (CAPI_REGISTER_ERROR)CAPI_Register(
                                            (WORD)MsgBufSize,
                                            (WORD)MaxB3Connection,
                                            (WORD)MaxB3Blks,
                                            (WORD)MaxSizeB3,
                                            &ApplId,
                                            SIGNAL_TYPE_CALLBACK,
                                            (DWORD) APPL_ReceiveNotify,
                                            0
                                            );

    if (*ErrorCode != CapiNoError) {
        CloseLocalSemaphore (GetMessageSemaphore);
        GetMessageSemaphore = -1;
        ApplId = 0;
    }

    return (_cword)ApplId;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_RELEASE (unsigned ApplId) {
    MESSAGE_EXCHANGE_ERROR Error;

    assert (GetMessageSemaphore != -1);

    Error = (MESSAGE_EXCHANGE_ERROR)CAPI_Release ((WORD)ApplId);

    if (Error == CapiNoError) {
        CloseLocalSemaphore (GetMessageSemaphore);
        GetMessageSemaphore = -1;
    }
    return Error;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_PUT_MESSAGE (CAPI_MESSAGE Msg, unsigned ApplId) {
    MESSAGE_EXCHANGE_ERROR Error;

    Error = (MESSAGE_EXCHANGE_ERROR)CAPI_PutMessage ((WORD)ApplId, (BYTE*)Msg);

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
MESSAGE_EXCHANGE_ERROR CAPI20_GET_MESSAGE (unsigned ApplId, CAPI_MESSAGE *ReturnMessage) {
    MESSAGE_EXCHANGE_ERROR Error;

    assert (GetMessageSemaphore != -1);

    if (ExamineLocalSemaphore (GetMessageSemaphore) <= 0) {
        Error = CapiReceiveQueueEmpty;
    } else {
        WaitOnLocalSemaphore (GetMessageSemaphore);

        Error = (MESSAGE_EXCHANGE_ERROR)CAPI_GetMessage((WORD)ApplId, (BYTE**)ReturnMessage);
    }

#if defined (CPROT)
    if (Error == 0x0000)
        CAPI_PROTOCOL_MESSAGE (*ReturnMessage);
    else if (Error != CapiReceiveQueueEmpty)
	CAPI_PROTOCOL_TEXT ("CAPI_GET_MESSAGE error 0x%04x\n", Error);
#endif

    return Error;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE CAPI20_GET_MANUFACTURER (CAPI_MESSAGE LpBuffer) {

    CAPI_GetManufacturer (0, LpBuffer);
    return LpBuffer;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE CAPI20_GET_VERSION (CAPI_MESSAGE version) {
    WORD CMajor;
    WORD CMinor;
    WORD MMajor;
    WORD MMinor;
    WORD MgrMajor;
    WORD MgrMinor;

    CAPI_GetVersion(0, (WORD *)&CMajor, (WORD *)&CMinor,
                       (WORD *)&MMajor, (WORD *)&MMinor,
                       (WORD *)&MgrMajor, (WORD *)&MgrMinor);

    version[0] = (BYTE)CMajor;
    version[1] = (BYTE)CMinor;
    version[2] = (BYTE)MMajor;
    version[3] = (BYTE)MMinor;
    return version;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE  CAPI20_GET_SERIAL_NUMBER (CAPI_MESSAGE LpBuffer) {

    CAPI_GetSerialNumber (0, LpBuffer);
    return LpBuffer;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_GET_PROFILE (unsigned Controller, CAPI_MESSAGE LpBuffer) {
    MESSAGE_EXCHANGE_ERROR Error;

    Error = (MESSAGE_EXCHANGE_ERROR)CAPI_GetProfile (LpBuffer, (DWORD)Controller);

#if defined (CPROT)
    if (Error != CapiNoError)
	CAPI_PROTOCOL_TEXT("CAPI_GET_PROFILE error \n", Error);
#endif

    return Error;
}

/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
