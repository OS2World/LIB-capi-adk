/*--------------------------------------------------------------------------*\

    COS2.H      Version 1.0                                         1997 AVM

    Operation system specific CAPI 2.0 functions for OS/2

\*--------------------------------------------------------------------------*/
#ifndef _cos2_h_
#define _cos2_h_
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#include <os2.h>

#include "capi20.h"
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
unsigned		CAPI20_ISINSTALLED (void);
_cword			CAPI20_REGISTER (unsigned MsgBufSize, unsigned MaxB3Connection, unsigned MaxB3Blks, unsigned MaxSizeB3, CAPI_REGISTER_ERROR *ErrorCode);
MESSAGE_EXCHANGE_ERROR	CAPI20_RELEASE (unsigned ApplId);
MESSAGE_EXCHANGE_ERROR	CAPI20_PUT_MESSAGE (CAPI_MESSAGE Msg, unsigned ApplId);
MESSAGE_EXCHANGE_ERROR	CAPI20_GET_MESSAGE (unsigned ApplId, CAPI_MESSAGE  __far * ReturnMessage);
MESSAGE_EXCHANGE_ERROR  CAPI20_SET_SIGNAL (unsigned ApplId, HEV hEventSem);
CAPI_MESSAGE		CAPI20_GET_MANUFACTURER (CAPI_MESSAGE LpBuffer);
CAPI_MESSAGE		CAPI20_GET_VERSION (CAPI_MESSAGE version);
CAPI_MESSAGE		CAPI20_GET_SERIAL_NUMBER (CAPI_MESSAGE LpBuffer);
MESSAGE_EXCHANGE_ERROR	CAPI20_GET_PROFILE (unsigned Controller, CAPI_MESSAGE LpBuffer);
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#endif /*----- _cos2_h_ -----*/
