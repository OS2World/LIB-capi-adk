/*--------------------------------------------------------------------------*\

    INIT.C	Version 1.1					    1995 AVM

    The init function must be called before any other function that uses
    CAPI messages

\*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <malloc.h>

#include "capi20.h"
#include "init.h"
#include "contr.h"
#include "capi.h"

#include "os.h"

/*--------------------------------------------------------------------------*\
 * defines needed by InitISDN
\*--------------------------------------------------------------------------*/
unsigned short Appl_Id = 0;


#define MaxNumBChan	    2  /*----- max. number of B-channels -----*/
#define MaxNumB3DataBlocks  7  /*----- max. number of unconfirmed B3-datablocks -----*/
			       /*----- 7 is the maximal number supported by CAPI -----*/
#define MaxB3DataBlockSize  2048   /*- max. B3-Datablocksize -----*/
				   /*- 2048 is the maximum supported by CAPI -----*/
#define MsgBufSize  (1024 + (1024 * MaxNumBChan)) /*-- size of messagebuffer -----*/

static CAPI_MESSAGE CAPI_BUFFER = NULL;

/*--------------------------------------------------------------------------*\
 * RegisterCAPI: Check for CAPI, allocate memory for CAPI-buffer and
 * register application. This function has to be called before using any
 * other CAPI functions.
\*--------------------------------------------------------------------------*/
unsigned RegisterCAPI (void) {
    CAPI_REGISTER_ERROR ErrorCode;
    unsigned		numController;

    if (!CAPI20_ISINSTALLED()) {
	printf ("\nError: ISDN-driver not installed\n");
	return 0;
    }
    numController = GetNumController();
    if (numController == 0) {
	printf("\nError: No ISDN-controller installed\n");
	return 0;
    }
#if defined (TARGET_16BIT)
    CAPI_BUFFER = malloc(MsgBufSize + (MaxNumBChan * MaxNumB3DataBlocks * MaxB3DataBlockSize));
    if (!CAPI_BUFFER)  {
	printf ("\nError: not enough memory in function RegisterCAPI\n");
	return 0;
    }
    Appl_Id = CAPI20_REGISTER(CAPI_BUFFER, MsgBufSize, MaxNumBChan,
                          MaxNumB3DataBlocks, MaxB3DataBlockSize, &ErrorCode);
#endif
#if defined (TARGET_32BIT)
#ifdef __linux__
    Appl_Id = CAPI20_REGISTER(MaxNumBChan,
                          MaxNumB3DataBlocks, MaxB3DataBlockSize, &ErrorCode);
#else
    Appl_Id = CAPI20_REGISTER(MsgBufSize, MaxNumBChan,
                          MaxNumB3DataBlocks, MaxB3DataBlockSize, &ErrorCode);
#endif
#endif

    if (Appl_Id == 0) {
	printf ("\nError: registering the application in function RegisterCAPI, ");
	printf ("code: %04X\n",ErrorCode);
	return 0;
    }
    return numController;
}

/*--------------------------------------------------------------------------*\
 * ReleaseCAPI: deregister application
\*--------------------------------------------------------------------------*/
void ReleaseCAPI (void) {
    MESSAGE_EXCHANGE_ERROR ErrorCode;

    ErrorCode = CAPI20_RELEASE(Appl_Id);
    if (ErrorCode != 0) {
	printf("Error: deregistering the application in function ReleaseCAPI, ");
	printf("code: 0x%04X\n",ErrorCode);
    }
    if (CAPI_BUFFER != NULL) {
	free (CAPI_BUFFER);
	CAPI_BUFFER = NULL;
    }
}
