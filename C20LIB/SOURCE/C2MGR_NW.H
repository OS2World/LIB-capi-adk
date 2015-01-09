/*---------------------------------------------------------------------------*\

    C2MGR_NW.H   Version 1.0                                         1998 AVM

    CAPI 2.0 Development Kit internal definitions for NetWare

\*---------------------------------------------------------------------------*/
#ifndef _C2MGR_NW_H_     /* avoid multiple inclusion */
#define _C2MGR_NW_H_

/* --------------------------------------------------
   signal type constants for application registration
   -------------------------------------------------- */
#define SIGNAL_TYPE_LOCAL_SEMAPHORE     0x0001
#define SIGNAL_TYPE_CALLBACK            0x0002

/* ---------------------------------------------------
   additional CAPI Manager specific CAPI info values
   --------------------------------------------------- */
#define CRE_INVALID_SIGNAL_TYPE         0x10F0
#define CRE_INVALID_FUNCTION_ARRAY      0x10F1
#define CRE_INVALID_CONTROLLER_INFO     0x10F2

#define CME_INVALID_CONTROLLER          0x11F0
#define CME_INVALID_CONTROLLER_INFO     0x11F1
#define CME_NO_MATCHING_CONTROLLER      0x11F2
#define CME_LOCK_FAILED                 0x11F3
#define CME_INVALID_LOCK_HANDLE         0x11F4

#define CSE_CTLR_NOT_RESPONDING         0x3F01

/* -----------------------------------------
   CAPI Manager specific messages extensions
   ----------------------------------------- */
#define MANUFACTURER_ID_NWCAPI          0x4D43574E  /* NWCM */

#define NW_CAPI_EXTENSION               0x00F0
#define BIND_APPL                       0x0010
#define UNBIND_APPL                     0x0011
#define CTLR_UNLOAD                     0x0012
#define CTLR_FAILURE                    0x0013

#define INFO_MASK_CTLR_UNLOAD           0x0080
#define INFO_MASK_CTLR_FAILURE          0x0100
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#endif /* _C2MGR_NW_H_ */
