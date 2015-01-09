/*---------------------------------------------------------------------------*\

    GCMSG_NW.H   Version 1.0                                         1998 AVM

    CAPI 2.0 Development Kit NetWare

    This file contains NetWare specific functions.
\*---------------------------------------------------------------------------*/
#include "capi20.h"
#include "c_nw.h"

/*-------------------------------------------------------*/
unsigned __far CAPI_PUT_CMSG (_cmsg __far *cmsg) {
    static unsigned char msg[2048];

    CAPI_CMSG_2_MESSAGE (cmsg, (CAPI_MESSAGE)msg);
    return CAPI20_PUT_MESSAGE ((CAPI_MESSAGE)msg, cmsg->ApplId);
}

/*-------------------------------------------------------*/
unsigned __far CAPI_GET_CMSG (_cmsg __far *cmsg, unsigned applid) {
    MESSAGE_EXCHANGE_ERROR rtn;
    CAPI_MESSAGE msg;

    rtn = CAPI20_GET_MESSAGE (applid, &msg);
    if (rtn == 0x0000)
        CAPI_MESSAGE_2_CMSG (cmsg, msg);
    return rtn;
}
