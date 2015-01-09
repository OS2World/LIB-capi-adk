/*---------------------------------------------------------------------------*\

    GCMSGWIN.C	Version 1.1					    1995 AVM

    Operation system specific functions of the CAPI 2.0 Development Kit
    for Windows 16/32 bit

\*---------------------------------------------------------------------------*/
#include "capi20.h"

#if defined (TARGET_32BIT)
#include "cwin32.h"
#endif
#if defined (TARGET_16BIT)
#include "cwin16.h"
#endif

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
