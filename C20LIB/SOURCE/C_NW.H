/*---------------------------------------------------------------------------*\

    C_NW.H      Version 1.0                                         1998 AVM

    CAPI 2.0 Development Kit internal functions for NetWare

\*---------------------------------------------------------------------------*/
#ifndef _c_nw_h
#define _c_nw_h
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#include "capi20.h"     // NetWare independant
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
unsigned                CAPI20_ISINSTALLED (void);
_cword                  CAPI20_REGISTER (unsigned MsgBufSize, unsigned MaxB3Connection, unsigned MaxB3Blks, unsigned MaxSizeB3, CAPI_REGISTER_ERROR *ErrorCode);
MESSAGE_EXCHANGE_ERROR  CAPI20_RELEASE (unsigned Appl_Id);
MESSAGE_EXCHANGE_ERROR  CAPI20_PUT_MESSAGE (CAPI_MESSAGE Msg, unsigned Appl_Id);
MESSAGE_EXCHANGE_ERROR  CAPI20_GET_MESSAGE (unsigned Appl_Id, CAPI_MESSAGE *ReturnMessage);
CAPI_MESSAGE            CAPI20_GET_VERSION (CAPI_MESSAGE version);
CAPI_MESSAGE            CAPI20_GET_MANUFACTURER (CAPI_MESSAGE LpBuffer);
CAPI_MESSAGE            CAPI20_GET_SERIAL_NUMBER (CAPI_MESSAGE LpBuffer);
MESSAGE_EXCHANGE_ERROR  CAPI20_GET_PROFILE (unsigned Controller, CAPI_MESSAGE LpBuffer);
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#endif
