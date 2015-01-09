/*--------------------------------------------------------------------------*\

    CAPI.C	Version 1.1					    1995 AVM

    Implementation of CAPI state machine

\*--------------------------------------------------------------------------*/
#include <stdio.h>

#include "capi20.h"
#include "connect.h"
#include "data.h"
#include "init.h"
#include "capi.h"
#include "id.h"

#define BC		NULL
#define LLC		NULL
#define HLC		NULL
#define B1Protokoll	1
#define B2Protokoll	1
#define B3Protokoll	0
#define B1Configuration NULL
#define B2Configuration NULL
#define B3Configuration NULL
#define Flags		0
#define NCPI		(_cstruct)NULL

static _cmsg		CMESSAGE;
static _cmsg __far	*CMSG = &CMESSAGE; /*----- used in all requests and responses -----*/


/*--------------------------------------------------------------------------*\
 *  SetState: Set the state internal and informs the user
\*--------------------------------------------------------------------------*/
static void ChangeState (ConnectionID	  Con,
			 ConnectionState  State) {

    SetState (Con, State);
    /*----- signal the status change to the user -----*/
    StateChange (Con, State);
}

/*--------------------------------------------------------------------------*\
 * Handle_Indication: CAPI logic for all indications
\*--------------------------------------------------------------------------*/
void Handle_Indication(void) {
    ConnectionID    Connection;

    switch (CMSG->Command) {

    case CAPI_CONNECT:
	Connection = GetConnectionByPLCI (CONNECT_IND_PLCI(CMSG));
	if (Connection == INVALID_CONNECTION_ID) {

	    /*----- incoming call -----*/
	    Connection = AllocConnection();
	    if (Connection == INVALID_CONNECTION_ID) {

		/*----- error no internal resources -----*/
		/*----- reject call -----*/
		CONNECT_RESP(CMSG, Appl_Id, CMSG->Messagenumber, CONNECT_IND_PLCI(CMSG), REJECT,
			     0, 0, 0, NULL, NULL, NULL,
			     NULL, NULL, NULL,
			     NULL, NULL, NULL, NULL);
		return;
	    }
	    SetConnectionPLCI(Connection, CONNECT_IND_PLCI(CMSG));
	}

	SetCallingPartyNumberStruct (Connection, CONNECT_IND_CALLINGPARTYNUMBER(CMSG));

	/*----- The ALERT_REQuest tells the caller that someone is listening -----*/
	/*----- for incoming calls on the line. A new timeout of 2 minutes is set -----*/
	/*----- Without the ALERT_REQuest a disconnect would be sent after -----*/
	/*----- 4 seconds with the cause "no user responding" on the caller side -----*/
	/*----- (Assumed that no CONNECT_RESPonse is sent in this time) -----*/
	/*----- If the application  -----*/
	ALERT_REQ (CMSG, Appl_Id, 0, CONNECT_IND_PLCI(CMSG),
						NULL, NULL, NULL, NULL);

	/*----- inform the user application -----*/
	SetState(Connection, D_ConnectPending);
	IncomingCall(Connection, GetCallingPartyNumber (Connection));

	ChangeState(Connection, D_ConnectPending);
	/*----- signal incoming call to the user -----*/
	return;

    case CAPI_CONNECT_ACTIVE:
	Connection = GetConnectionByPLCI (CONNECT_ACTIVE_IND_PLCI(CMSG));
	CONNECT_ACTIVE_RESP(CMSG, Appl_Id, CMSG->Messagenumber, CONNECT_ACTIVE_IND_PLCI(CMSG));
	ChangeState(Connection, D_Connected);
	if (GetConnectionInitiator (Connection)) {
	    CONNECT_B3_REQ(CMSG, Appl_Id, 0, CONNECT_ACTIVE_IND_PLCI(CMSG), NCPI);
	}
	return;

    case CAPI_CONNECT_B3:
	Connection = GetConnectionByPLCI (CONNECT_B3_IND_NCCI(CMSG) & 0x0000FFFF);
	SetConnectionNCCI (Connection, CONNECT_B3_IND_NCCI(CMSG));
	CONNECT_B3_RESP(CMSG, Appl_Id, CMSG->Messagenumber, CONNECT_B3_IND_NCCI(CMSG), 0, NCPI);
	ChangeState(Connection, B_ConnectPending);
	return;

    case CAPI_CONNECT_B3_ACTIVE:
	Connection = GetConnectionByNCCI (CONNECT_B3_ACTIVE_IND_NCCI(CMSG));
	SetConnectionInitiator (Connection, FALSE);
	CONNECT_B3_ACTIVE_RESP(CMSG, Appl_Id, CMSG->Messagenumber, CONNECT_B3_ACTIVE_IND_NCCI(CMSG));
	ChangeState(Connection, Connected);
	return;

    case CAPI_DISCONNECT_B3:
	Connection = GetConnectionByNCCI(DISCONNECT_B3_IND_NCCI(CMSG));
	SetConnectionNCCI (Connection, INVAL_NCCI);
	DISCONNECT_B3_RESP(CMSG, Appl_Id, CMSG->Messagenumber, DISCONNECT_B3_IND_NCCI(CMSG));
	ChangeState(Connection, D_Connected);
	if (GetConnectionInitiator(Connection)) {
	    DISCONNECT_REQ(CMSG, Appl_Id, 0, GetConnectionPLCI(Connection), NULL, NULL, NULL, NULL);
	}
	return;

    case CAPI_DISCONNECT:
	DISCONNECT_RESP(CMSG, Appl_Id, CMSG->Messagenumber, DISCONNECT_IND_PLCI(CMSG));
	Connection = GetConnectionByPLCI(DISCONNECT_IND_PLCI(CMSG));
        if (Connection != INVALID_CONNECTION_ID) {
            ChangeState(Connection, Disconnected);
            FreeConnection(Connection);
        }
	return;

    case CAPI_DATA_B3:
	Connection = GetConnectionByNCCI(DATA_B3_IND_NCCI(CMSG));
	if (CMSG->DataLength > 0) {
	    int     DiscardData = TRUE;

	    DataAvailable(Connection,
			  (void __far *)DATA_B3_IND_DATA(CMSG),
					DATA_B3_IND_DATALENGTH(CMSG),
					DATA_B3_IND_DATAHANDLE(CMSG),
					&DiscardData);

	    if (DiscardData)
		/*----- let CAPI free the data area immediately -----*/
		DATA_B3_RESP(CMSG, Appl_Id, CMSG->Messagenumber, DATA_B3_IND_NCCI(CMSG), DATA_B3_IND_DATAHANDLE(CMSG));
	}
	return;

    case CAPI_INFO:
	INFO_RESP(CMSG, Appl_Id, CMSG->Messagenumber, INFO_IND_PLCI(CMSG));
	return;

    default:
	puts("Error: Indication not handled in function Handle_Indication");
	return;
    }
}

/*--------------------------------------------------------------------------*\
 * Handle_Confirmation: CAPI logic for all confirmations
\*--------------------------------------------------------------------------*/
static void Handle_Confirmation(void) {
    ConnectionID    Connection;

    if (CMSG->Info > 0x00FF) { /*----- Info's with value 0x00xx are only -----*/
			       /*----- warnings, the corresponding requests -----*/
			       /*----- have been processed -----*/
	printf("Error: Info value 0x%x indicates error, function Handle_Confirmation\n", CMSG->Info);

    /*----- This branch is executed if an error has occurred -----*/

	switch (CMSG->Command) {
	case CAPI_CONNECT:
	    Connection = CMSG->Messagenumber;
	    ChangeState (Connection, D_ConnectPending);
	    ChangeState (Connection, Disconnected);
	    FreeConnection(Connection);
	    break;

	case CAPI_DATA_B3:
	    /*----- return the error value -----*/
	    Connection = GetConnectionByNCCI(DATA_B3_CONF_NCCI(CMSG));
	    DataConf(Connection, DATA_B3_CONF_DATAHANDLE(CMSG),
						 DATA_B3_CONF_INFO(CMSG));
	    break;

	case CAPI_CONNECT_B3:
	    /*----- disconnect line -----*/
	    Connection = GetConnectionByPLCI(CONNECT_B3_CONF_NCCI(CMSG) & 0x0000FFFF);
	    if (Connection == INVALID_CONNECTION_ID)
		puts("Error: invalid PLCI in CONNECT_B3_CONF in function Handle_Confirmation");
	    else
		DISCONNECT_REQ(CMSG, Appl_Id, 0, GetConnectionPLCI(Connection), NULL, NULL, NULL, NULL);
	    break;

	case CAPI_DISCONNECT:
	    Connection = GetConnectionByPLCI(DISCONNECT_CONF_PLCI(CMSG));
	    if (Connection == INVALID_CONNECTION_ID)
		puts("Error: invalid PLCI in DISCONNECT_CONF in function Handle_Confirmation");
	    break;

	case CAPI_DISCONNECT_B3:
	    Connection = GetConnectionByNCCI(DISCONNECT_B3_CONF_NCCI(CMSG));
	    if (Connection == INVALID_CONNECTION_ID)
		puts("Error: invalid NCCI in DISCONNECT_B3_CONF in function Handle_Confirmation");
	    break;
	case CAPI_LISTEN:
	    puts("Error: Info != 0 in LISTEN_CONF in function Handle_Confirmation");
	    break;
	case CAPI_INFO:
	    puts("Error: Info != 0 in INFO_CONF in function Handle_Confirmation");
	    break;
	case CAPI_ALERT:
	    puts("Error: Info != 0 in ALERT_CONF in function Handle_Confirmation");
	    break;
	}

    } else {	 /*----- no error -----*/

	switch (CMSG->Command) {
	case CAPI_CONNECT:
	    Connection = CMSG->Messagenumber;
	    SetConnectionPLCI(Connection, CONNECT_CONF_PLCI(CMSG));
	    SetConnectionInitiator(Connection, TRUE);
	    ChangeState(Connection, D_ConnectPending);
	    return;

	case CAPI_CONNECT_B3:
	    Connection = GetConnectionByPLCI(CONNECT_B3_CONF_NCCI(CMSG) & 0x0000FFFF);
	    SetConnectionNCCI(Connection, CONNECT_B3_CONF_NCCI(CMSG));
	    ChangeState(Connection, B_ConnectPending);
	    return;

	case CAPI_DISCONNECT:
	    Connection = GetConnectionByPLCI(DISCONNECT_CONF_PLCI(CMSG));
            if (Connection != INVALID_CONNECTION_ID) {
                ChangeState(Connection, D_DisconnectPending);
            }
	    return;

	case CAPI_DISCONNECT_B3:
	    Connection = GetConnectionByNCCI(DISCONNECT_B3_CONF_NCCI(CMSG));
	    SetConnectionInitiator(Connection, TRUE);
	    ChangeState(Connection, B_DisconnectPending);
	    return;

	case CAPI_DATA_B3:
	    Connection = GetConnectionByNCCI(DATA_B3_CONF_NCCI(CMSG));
	    DataConf(Connection, DATA_B3_CONF_DATAHANDLE(CMSG),
						 DATA_B3_CONF_INFO(CMSG));
	    return;
	case CAPI_LISTEN:
	    return;
	case CAPI_INFO:
	    return;
	case CAPI_ALERT:
	    return;
	default:
	    puts("Error: Confirmation not handled in function Handle_Confirmation");
	    return;
	}
    }
}

/*--------------------------------------------------------------------------*\
 * Handle_CAPI_Msg: the main routine, checks for messages and handles them
\*--------------------------------------------------------------------------*/
void Handle_CAPI_Msg(void) {
    MESSAGE_EXCHANGE_ERROR  Info;
    int 		    count;

    count=0;
    do {
	switch (Info = CAPI_GET_CMSG(CMSG, Appl_Id)) {
	case 0x0000:	       /*----- a message has been read -----*/
	    switch (CMSG->Subcommand) {
	    case CAPI_CONF:
		Handle_Confirmation();
		break;

	    case CAPI_IND:
		Handle_Indication();
		break;

	    default:	   /*----- neither indication nor confirmation ???? -----*/
		puts("Error: Unknown subcommand in function Handle_CAPI_Msg");
		return;
	    }
	    break;

	case 0x1104:
	    return;	/*----- messagequeue is empty -----*/

	default:
	    puts("Error: CAPI_GET_CMSG returns Info != 0 in function Handle_CAPI_Msg");
	    return;
	}
	count++;
    } while (count < 6);    /*----- CAPI is checked maximal 5 times before returning -----*/
}
