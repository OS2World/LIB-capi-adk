/*--------------------------------------------------------------------------*\

    CVxD.H      Version 1.2                                         1997 AVM

    Operation system specific CAPI 2.0 functions for the Win95 VxD interface

\*--------------------------------------------------------------------------*/
#ifndef _cvxd_h_
#define _cvxd_h_
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#include <basedef.h>    /*----- Win95 DDK see %DDKROOT%\INC32 -----*/
#include <vmm.h>        /*----- Win95 DDK see %DDKROOT%\INC32 -----*/
#include "capi20.h"
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#define CAPI20_DEVICE_ID        0x3215
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
Begin_Service_Table (CAPI20, VxD)
Declare_Service   (CAPI20_Get_Version, LOCAL)
Declare_Service   (CAPI20_MessageOperations, LOCAL)
End_Service_Table (CAPI20, VxD)
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
unsigned		CAPI20_ISINSTALLED (void);
_cword			CAPI20_REGISTER (unsigned MsgBufSize, unsigned MaxB3Connection, unsigned MaxB3Blks, unsigned MaxSizeB3, CAPI_REGISTER_ERROR *ErrorCode);
MESSAGE_EXCHANGE_ERROR	CAPI20_RELEASE (unsigned ApplId);
MESSAGE_EXCHANGE_ERROR	CAPI20_PUT_MESSAGE (CAPI_MESSAGE Msg, unsigned ApplId);
MESSAGE_EXCHANGE_ERROR  CAPI20_GET_MESSAGE (unsigned ApplId, CAPI_MESSAGE  *ReturnMessage);
MESSAGE_EXCHANGE_ERROR  CAPI20_SET_SIGNAL (unsigned ApplId, void (*CallBack)(void), unsigned Param);
CAPI_MESSAGE            CAPI20_GET_MANUFACTURER (unsigned Controller, CAPI_MESSAGE LpBuffer);
CAPI_MESSAGE            CAPI20_GET_VERSION (unsigned Controller, CAPI_MESSAGE version);
CAPI_MESSAGE            CAPI20_GET_SERIAL_NUMBER (unsigned Controller, CAPI_MESSAGE LpBuffer);
MESSAGE_EXCHANGE_ERROR	CAPI20_GET_PROFILE (unsigned Controller, CAPI_MESSAGE LpBuffer);
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#endif /*----- _cvxd_h_ -----*/
