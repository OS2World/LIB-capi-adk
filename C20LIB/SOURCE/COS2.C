/*---------------------------------------------------------------------------*\

    COS2.C      Version 1.0                                        1997 AVM

    This file contains the source of the operating system specific
    CAPI functions, here for OS/2 applications. See CAPI 2.0 spec.

\*---------------------------------------------------------------------------*/
#include "cos2.h"
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
typedef unsigned short WORD;
typedef unsigned long  DWORD;
/*---------------------------------------------------------------------------*\
    defined in capi20 runtime DLL
\*---------------------------------------------------------------------------*/
extern DWORD APIENTRY CAPI_REGISTER (DWORD MessageBufferSize, DWORD maxLogicalConnection, DWORD maxBDataBlocks, DWORD maxBDataLen, DWORD *pApplID);
extern DWORD APIENTRY CAPI_RELEASE (DWORD ApplID);
extern DWORD APIENTRY CAPI_PUT_MESSAGE (DWORD ApplID, PVOID pCAPIMessage);
extern DWORD APIENTRY CAPI_GET_MESSAGE (DWORD ApplID, PVOID *ppCAPIMessage);
extern DWORD APIENTRY CAPI_SET_SIGNAL (DWORD ApplID, DWORD hEventSem);
extern void  APIENTRY CAPI_GET_MANUFACTURER (PVOID SzBuffer);
extern DWORD APIENTRY CAPI_GET_VERSION (DWORD *pCAPIMajor, DWORD *pCAPIMinor, DWORD *pManufacturerMajor, DWORD *pManufacturerMinor);
extern DWORD APIENTRY CAPI_GET_SERIAL_NUMBER (PVOID SzBuffer);
extern DWORD APIENTRY CAPI_GET_PROFILE (PBYTE SzBuffer, WORD CtrlNr);
extern DWORD APIENTRY CAPI_INSTALLED (void);
/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CAPI20_ISINSTALLED (void) {
    return (unsigned)CAPI_INSTALLED () == 0;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
_cword CAPI20_REGISTER (unsigned MsgBufSize, unsigned MaxB3Connection, unsigned MaxB3Blks, unsigned MaxSizeB3, CAPI_REGISTER_ERROR *ErrorCode) {
    DWORD    ApplID;

    *ErrorCode = (CAPI_REGISTER_ERROR) CAPI_REGISTER(MsgBufSize, MaxB3Connection, MaxB3Blks, MaxSizeB3, &ApplID);

    return (_cword)ApplID;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_RELEASE (unsigned Appl_Id) {

    return (MESSAGE_EXCHANGE_ERROR) CAPI_RELEASE (Appl_Id);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_PUT_MESSAGE (CAPI_MESSAGE Msg, unsigned Appl_Id) {
    DWORD    Error;

    Error = CAPI_PUT_MESSAGE(Appl_Id, (void *)Msg);

#if defined (CPROT)
    if (Error == 0x0000)
        CAPI_PROTOCOL_MESSAGE (Msg);
    else
	CAPI_PROTOCOL_TEXT ("CAPI_PUT_MESSAGE error 0x%04x\n", Error);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Error;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_GET_MESSAGE (unsigned Appl_Id, CAPI_MESSAGE __far *ReturnMessage) {
    DWORD    Error;

    Error = CAPI_GET_MESSAGE(Appl_Id, (PVOID *)ReturnMessage);

#if defined (CPROT)
    if (Error == 0x0000)
        CAPI_PROTOCOL_MESSAGE (*ReturnMessage);
    else if (Error != CapiReceiveQueueEmpty)
	CAPI_PROTOCOL_TEXT ("CAPI_GET_MESSAGE error 0x%04x\n", Error);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Error;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_SET_SIGNAL (unsigned ApplId, HEV hEventSem) {
    DWORD    Error;

    Error = CAPI_SET_SIGNAL (ApplId, (DWORD) hEventSem);

#if defined (CPROT)
    if (Error != 0x0000)
        CAPI_PROTOCOL_TEXT("CAPI_SET_SIGNAL error 0x%04x\n", Error);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Error;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE CAPI20_GET_MANUFACTURER (CAPI_MESSAGE LpBuffer) {
    CAPI_GET_MANUFACTURER(LpBuffer);

    return LpBuffer;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE CAPI20_GET_VERSION (CAPI_MESSAGE version) {
    DWORD CMajor;
    DWORD CMinor;
    DWORD MMajor;
    DWORD MMinor;

    CAPI_GET_VERSION(&CMajor, &CMinor, &MMajor, &MMinor);

    version[0] = (BYTE)CMajor;
    version[1] = (BYTE)CMinor;
    version[2] = (BYTE)MMajor;
    version[3] = (BYTE)MMinor;

    return version;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE CAPI20_GET_SERIAL_NUMBER (CAPI_MESSAGE LpBuffer) {

    CAPI_GET_SERIAL_NUMBER(LpBuffer);

    return LpBuffer;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_GET_PROFILE (unsigned Controller, CAPI_MESSAGE LpBuffer) {
    DWORD    Error;

    Error = CAPI_GET_PROFILE (LpBuffer, Controller);

#if defined (CPROT)
    if (Error != 0x0000)
	CAPI_PROTOCOL_TEXT("CAPI_GET_PROFILE error \n", Error);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Error;
}

/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
