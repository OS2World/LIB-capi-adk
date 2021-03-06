/*--------------------------------------------------------------------------*\

    DATA.C	Version 1.1					    1995 AVM

    Functions concerning data transmission

\*--------------------------------------------------------------------------*/
#include <assert.h>

#include "capi20.h"
#include "id.h"
#include "init.h"
#include "data.h"
#include "main.h"

/*--------------------------------------------------------------------------*\
 * SendData: Sends one block with data over the specified channel
\*--------------------------------------------------------------------------*/
unsigned SendData(ConnectionID	  Connection,
		  void __far	  *Data,
		  unsigned short  DataLength,
		  unsigned short  DataHandle) {

    _cmsg   CMSG;

    assert (Connection != INVALID_CONNECTION_ID);
    assert (GetState(Connection) == Connected);

    DATA_B3_REQ_HEADER(&CMSG, Appl_Id, 0, GetConnectionNCCI(Connection));
    DATA_B3_REQ_DATA(&CMSG) = (unsigned long)Data;
    DATA_B3_REQ_DATALENGTH(&CMSG) = DataLength;
    DATA_B3_REQ_DATAHANDLE(&CMSG) = DataHandle;

    return CAPI_PUT_CMSG(&CMSG);
}

/*--------------------------------------------------------------------------*\
 * DataConf: signals the successful sending of a datablock
 * This function is called after receiving a DATA_B3_CONFirmation. CAPI signals
 * that the datablock identified by DataHandle has been sent and the memory
 * area may be freed. The DataHandle is the same as specified in SendBlock.
 * This function is implemented in the main program.
\*--------------------------------------------------------------------------*/
void DataConf(ConnectionID    Connection,
	      unsigned short  DataHandle,
	      unsigned short  Info) {

    MainDataConf(Connection, DataHandle, Info);
}

/*--------------------------------------------------------------------------*\
 * DataAvailable: signals received data blocks
 * This function is called after a DATA_B3_INDication is received. The flag
 * DiscardData tells CAPI to free the memora area directly after the return
 * of this function when set to TRUE (1) which is the preset. When the flag
 * is set to FALSE (0) the data area MUST be freed later with ReleaseData.
 * The datahandle identifies the memory area. When reaching 7 unconfirmed
 * blocks, no more incoming data will be signaled until freeing at least
 * one block.
 * This function is implemented in the main program.
\*--------------------------------------------------------------------------*/
void DataAvailable(ConnectionID    Connection,
		   void __far	   *Data,
		   unsigned short  DataLength,
		   unsigned short  DataHandle,
		   int		   *DiscardData) {

    MainDataAvailable(Connection, Data, DataLength, DataHandle, DiscardData);
}

/*--------------------------------------------------------------------------*\
 * ReleaseData: allows CAPI to reuse the memory area of the specified block.
 * CAPI allows max. 7 unconfirmed Blocks. If the maximum of 7 is reached,
 * no more DATA_B3_INDications will come up.
\*--------------------------------------------------------------------------*/
unsigned ReleaseData(ConnectionID    Connection,
		     unsigned short  DataHandle) {

    _cmsg   CMSG;

    assert (Connection != INVALID_CONNECTION_ID);
    assert (GetState(Connection) == Connected);

    return DATA_B3_RESP(&CMSG, Appl_Id, 0, GetConnectionNCCI(Connection), DataHandle);
}
