/*---------------------------------------------------------------------------*\

    C2IMP_NW.H   Version 1.0                                         1998 AVM

    CAPI 2.0 Development Kit NetWare

    This file contains the NetWare specific import symbols for CAPI 2.0.
\*---------------------------------------------------------------------------*/
#ifndef c2imp_nw_inc
#define c2imp_nw_inc
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#include <nwdstype.h>
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#define IMPORT _cdecl
/*---------------------------------------------------------------------------*\
    external symbols for CAPI20 functions
\*---------------------------------------------------------------------------*/
extern DWORD IMPORT CAPI_Register         (WORD, WORD, WORD, WORD, WORD*, WORD, DWORD, DWORD);
extern DWORD IMPORT CAPI_Release          (WORD );
extern DWORD IMPORT CAPI_PutMessage       (WORD, BYTE* );
extern DWORD IMPORT CAPI_GetMessage       (WORD, BYTE** );
extern DWORD IMPORT CAPI_GetManufacturer  (DWORD, BYTE* );
extern DWORD IMPORT CAPI_GetVersion       (DWORD, WORD*, WORD*, WORD*, WORD*, WORD*, WORD*);
extern DWORD IMPORT CAPI_GetSerialNumber  (DWORD, BYTE* );
extern DWORD IMPORT CAPI_GetProfile       (void*, DWORD );
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#endif
