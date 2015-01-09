/*---------------------------------------------------------------------------*\

    CWIN.C	Version 1.1					    1995 AVM

    This file contains the source of the operating system specific
    CAPI functions, here for Windows 16 bit. See CAPI 2.0 spec.

\*---------------------------------------------------------------------------*/
#include <windows.h>
#include <memory.h>

#include "capi20.h"

/*---------------------------------------------------------------------------*\
    defined in capi20 Runtime DLL
\*---------------------------------------------------------------------------*/
extern WORD FAR PASCAL CAPI_INSTALLED (void);
extern WORD FAR PASCAL CAPI_REGISTER (WORD MessageBufferSize, WORD maxLogicalConnection, WORD maxBDataBlocks, WORD maxBDataLen, LPWORD pApplID);
extern WORD FAR PASCAL CAPI_RELEASE (WORD ApplID);
extern WORD FAR PASCAL CAPI_PUT_MESSAGE (WORD ApplID, LPVOID pCAPIMessage);
extern WORD FAR PASCAL CAPI_GET_MESSAGE (WORD ApplID, LPVOID FAR *ppCAPIMessage);
extern WORD FAR PASCAL CAPI_SET_SIGNAL (WORD ApplID, VOID (FAR PASCAL *CAPI_Callback)(WORD ApplID, DWORD Param), DWORD Param);
extern WORD FAR PASCAL CAPI_GET_MANUFACTURER (LPBYTE SzBuffer);
extern WORD FAR PASCAL CAPI_GET_VERSION (LPWORD pCAPIMajor, LPWORD pCAPIMinor, LPWORD pManufacturerMajor, LPWORD pManufacturerMinor);
extern WORD FAR PASCAL CAPI_GET_SERIAL_NUMBER (LPBYTE SzBuffer);
extern WORD FAR PASCAL CAPI_GET_PROFILE (LPBYTE SzBuffer, WORD CtrlNr);
/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CAPI20_ISINSTALLED (void) {
    return (unsigned)CAPI_INSTALLED () == 0;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
_cword CAPI20_REGISTER (unsigned MsgBufSize, unsigned MaxB3Connection, unsigned MaxB3Blks, unsigned MaxSizeB3, CAPI_REGISTER_ERROR *ErrorCode) {
    WORD    ApplID;

    *ErrorCode = (CAPI_REGISTER_ERROR) CAPI_REGISTER((WORD)MsgBufSize, (WORD)MaxB3Connection, (WORD)MaxB3Blks, (WORD)MaxSizeB3, &ApplID);
    return (_cword)ApplID;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_RELEASE (unsigned Appl_Id) {

    return (MESSAGE_EXCHANGE_ERROR) CAPI_RELEASE ((WORD)Appl_Id);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_PUT_MESSAGE (CAPI_MESSAGE Msg, unsigned Appl_Id) {
    WORD    Error;

    Error = CAPI_PUT_MESSAGE((WORD)Appl_Id, (void __far *)Msg);

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
    WORD    Error;

    Error = CAPI_GET_MESSAGE((WORD)Appl_Id, (LPVOID FAR *)ReturnMessage);

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
MESSAGE_EXCHANGE_ERROR CAPI20_SET_SIGNAL(unsigned ApplId, void (FAR PASCAL *CallBack)(WORD,DWORD), DWORD Param) {
    WORD    Error;

    Error = CAPI_SET_SIGNAL((WORD)ApplId, CallBack, Param);

#if defined (CPROT)
    if (Error != 0x0000)
	CAPI_PROTOCOL_TEXT("CAPI_SET_SIGNAL error \n", Error);
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
    WORD CMajor;
    WORD CMinor;
    WORD MMajor;
    WORD MMinor;

    CAPI_GET_VERSION((WORD __far *)&CMajor, (WORD __far *)&CMinor,
		     (WORD __far *)&MMajor, (WORD __far *)&MMinor);

    version[0] = (BYTE)CMajor;
    version[1] = (BYTE)CMinor;
    version[2] = (BYTE)MMajor;
    version[3] = (BYTE)MMinor;

    return version;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
CAPI_MESSAGE  CAPI20_GET_SERIAL_NUMBER (CAPI_MESSAGE LpBuffer) {

    CAPI_GET_SERIAL_NUMBER(LpBuffer);

    return LpBuffer;
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
MESSAGE_EXCHANGE_ERROR CAPI20_GET_PROFILE (unsigned Controller, CAPI_MESSAGE LpBuffer) {
    WORD    Error;

    Error = CAPI_GET_PROFILE(LpBuffer, (WORD)Controller);

#if defined (CPROT)
    if (Error != 0x0000)
	CAPI_PROTOCOL_TEXT("CAPI_GET_PROFILE error \n", Error);
#endif

    return (MESSAGE_EXCHANGE_ERROR) Error;
}
