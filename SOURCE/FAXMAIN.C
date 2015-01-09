/*--------------------------------------------------------------------------*\

    FAXMAIN.C	Version 1.1					    1995 AVM

    's'+'S' Send fax
    'r'+'R' Receive fax

\*--------------------------------------------------------------------------*/
#if !defined (NDEBUG)
#define DEBUG
#define CPROT
#endif

#include "os.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>

#include "capi20.h"
#include "c20msg.h"
#include "capi.h"
#include "connect.h"
#include "contr.h"
#include "data.h"
#include "id.h"
#include "init.h"
#include "fax.h"


/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
/*----- note: you may enter your own number here, but if you -----*/
/*-----       supply a wrong number, some PBXs may reject the  -----*/
/*-----       CAPI messages containing the wrong number -----*/
/*----- e.g.: static  char    *CallingPartyNumber = "1234567"; -----*/
static  char    *CallingPartyNumber = NULL;

static	char	CalledPartyNumberArr[40];

#ifdef DEBUG
static char	CAPI_PROT_BUF[CAPI_PROTOCOL_INIT_BUF_SIZE];
static char	ProtocolFileName[80];
static FILE	*ProtocolFile;
#endif

#define INVALID_SLOT	-1
#define maxSlots	2	/*----- this demo program handles max. -----*/
				/*----- two connections -----*/
static	ConnectionID	Slot[maxSlots];


#define B1PROTOCOL	    4
#define B2PROTOCOL	    4
#define B3PROTOCOL	    4


#define QueueSize   8

typedef struct __DataElement {
    char	    DATA[SendBlockSize];
    unsigned short  DATA_LENGTH;
    unsigned	    SENT;
} _DataElement;

typedef struct	__DataQueue {
    _DataElement    Element[QueueSize];
    unsigned	    Head;
    unsigned	    Tail;
    unsigned	    Fill;
} _DataQueue;

_DataQueue	    Queue;

static unsigned FileTransfer = FALSE; /*----- signals if filetransfer is in progress -----*/
static unsigned FileReceive  = FALSE; /*----- signals if filetransfer is in progress -----*/
static FILE	*File;

/*--------------------------------------------------------------------------*\
 * Press_Key:
\*--------------------------------------------------------------------------*/
int Press_Key(void) {
    int c;

    if ((c = getch()) == 0)
	c = getch()+256;
    return c;
}

/*--------------------------------------------------------------------------*\
 * GetSlot: returns the slotnumber of the ConnectionID or INVALID_SLOT
\*--------------------------------------------------------------------------*/
int GetSlot(ConnectionID Con) {
    int     x;

    for (x=0; x<maxSlots; x++)
	if (Slot[x] == Con) return x;
    return INVALID_SLOT;
}

/*--------------------------------------------------------------------------*\
 * AllocSlot: allocates one slot, if none free returns INVALID_SLOT
\*--------------------------------------------------------------------------*/
int AllocSlot(ConnectionID Con) {
    int     x;

    for (x=0; x<maxSlots; x++)
	if (Slot[x] == INVALID_CONNECTION_ID) {
	    Slot[x] = Con;
	    return x;
	}
    return INVALID_SLOT;
}

/*--------------------------------------------------------------------------*\
 * FreeSlot: clear one slot
\*--------------------------------------------------------------------------*/
void FreeSlot(int index) {
    Slot[index] = INVALID_CONNECTION_ID;
}

/*--------------------------------------------------------------------------*\
 * MainDataAvailable: signals received data blocks
 * This function is called after a DATA_B3_INDication is received. The flag
 * DiscardData tells CAPI to free the memora area directly after the return
 * of this function when set to TRUE (1) which is the preset. When the flag
 * is set to FALSE (0) the data area MUST be freed later with ReleaseData.
 * The datahandle identifies the memory area. When reaching 7 unconfirmed
 * blocks, no more incoming data will be signaled until freeing at least
 * one block.
\*--------------------------------------------------------------------------*/
void MainDataAvailable(ConnectionID    Connection,
		       void __far      *Data,
		       unsigned short  DataLength,
		       unsigned short  DataHandle,
		       int	       *DiscardData) {

    assert (Connection != INVALID_CONNECTION_ID);


#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** incoming data, slot %d ID %d, size %d , handle %u *****\n",
                     GetSlot(Connection), Connection, DataLength, DataHandle);
#   endif

    if ((FileReceive) && (File != NULL)) {
	fwrite(Data, 1, DataLength, File);
    }
    *DiscardData = TRUE;
}

/*--------------------------------------------------------------------------*\
 * MainDataConf: signals the successful sending of a datablock
 * This function is called after receiving a DATA_B3_CONFirmation. CAPI signals
 * that the datablock identified by DataHandle has been sent and the memory
 * area may be freed. The DataHandle is the same as specified in SendBlock.
\*--------------------------------------------------------------------------*/
void MainDataConf(ConnectionID	  Connection,
		  unsigned short  DataHandle,
		  unsigned short  Info) {

    assert (Connection != INVALID_CONNECTION_ID);


    if (Info != 0) {
#       ifdef DEBUG
        CAPI_PROTOCOL_TEXT("***** datablock slot %d ID %d handle %d NOT sent , error: 0x%04X *****\n",
			    GetSlot(Connection), Connection, DataHandle, Info);
#       endif
	return;
    }
    if (FileTransfer) {

	assert (DataHandle == (unsigned short)Queue.Tail);

	Queue.Element[Queue.Tail].SENT = FALSE;
	if (++Queue.Tail >= QueueSize) Queue.Tail = 0;
	Queue.Fill--;
    }
#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** datablock slot %d ID %d handle %d has been sent *****\n",
                        GetSlot(Connection), Connection, DataHandle);
#   endif
}

/*--------------------------------------------------------------------------*\
 * MainStateChange: signals a state change on both B-channels (connected,
 * disconnected). Whenever a channel changes his state this function is called
\*--------------------------------------------------------------------------*/
void MainStateChange(ConnectionID     Connection,
		     ConnectionState  State) {
    int     index;

    assert (Connection != INVALID_CONNECTION_ID);


    index = GetSlot(Connection);
#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** state change slot %d ID %d: %s *****\n", index, Connection,
						ConnectionStateString[State]);
#   endif

    if (State == Disconnected) {
	FreeSlot(index);
    }
}

/*--------------------------------------------------------------------------*\
 * MainIncomingCall: signals an incoming call
 * This function will be executed if a CONNECT_INDication appears to
 * inform the user.
\*--------------------------------------------------------------------------*/
void MainIncomingCall(ConnectionID  Connection,
		      char	    *CallingPartyNumber) {

    int 	    index;
    B3_PROTO_FAXG3  B3conf;

    assert (Connection != INVALID_CONNECTION_ID);


#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** incoming call ,ID %d, caller: \"%s\" *****\n",Connection,CallingPartyNumber);
#   endif
    index = AllocSlot(Connection);

    SetupB3Config( &B3conf, FAX_SFF_FORMAT);

    if (index == INVALID_SLOT) {
#       ifdef DEBUG
        CAPI_PROTOCOL_TEXT("***** no free slot available, rejecting call... *****\n");
#       endif
	AnswerCall(Connection, REJECT, 4, 4, 4, (_cstruct)&B3conf);
	return;
    }
#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** call assigned to slot %d *****\n", index);
#   endif

    AnswerCall(Connection, ACCEPT, 4, 4, 4, (_cstruct)&B3conf);
}


#ifdef DEBUG

/*--------------------------------------------------------------------------*\
 * CAPI_PROT_HANDLE: This is a callback-function that has been specified
 * with CAPI_PROTOCOL_INIT. The first parameter is a pointer to the protocol-
 * message which is plain ASCII-text. The parameter t contains the type of
 * the message which can be CAPI_PROTOCOL_HEADER (appears only once when
 * calling CAPI_PROTOCOL_INIT), CAPI_PROTOCOL_MSG (the text contains a
 * decoded CAPI-message) and CAPI_PROTOCOL_TXT (the buffers contains a debug
 * message or a message sent with the function CAPI_PROTOCOL_TEXT).
 * If the type of the message is CAPI_PROTOCOL_MSG, the last parameter contains
 * a pointer to the decoded CAPI-message.
\*--------------------------------------------------------------------------*/
void CAPI_PROT_HANDLE(char		 *Message,
		      CAPI_PROTOCOL_TYP  t,
		      CAPI_MESSAGE	 m) {

    fprintf(ProtocolFile,"%s",Message);
    if (t != CAPI_PROTOCOL_MSG)
	puts(Message);

    if (t == CAPI_PROTOCOL_MSG) {
	_cmsg	CMSG;

	CAPI_MESSAGE_2_CMSG(&CMSG, m);
	if ((FileTransfer || FileReceive) &&
		(CMSG.Command == CAPI_DATA_B3) && (CMSG.Info == 0) &&
		(CMSG.Reason == 0) && (CMSG.Reason_B3 == 0)) {

	    return;
	}
	puts(Message);
	if (CMSG.Info != 0) {
	    printf("Info 0x%04X: %s\n",CMSG.Info,Decode_Info(CMSG.Info));
	    fprintf(ProtocolFile,"Info 0x%04X: %s\n",CMSG.Info,Decode_Info(CMSG.Info));
	}
	if (CMSG.Reason != 0) {
	    printf("Reason 0x%04X: %s\n",CMSG.Reason,Decode_Info(CMSG.Reason));
	    fprintf(ProtocolFile,"Reason 0x%04X: %s\n",CMSG.Reason,Decode_Info(CMSG.Reason));
	}
	if (CMSG.Reason_B3 != 0) {
	    printf("Reason_B3 0x%04X: %s\n",CMSG.Reason_B3,Decode_Info(CMSG.Reason_B3));
	    fprintf(ProtocolFile,"Reason_B3 0x%04X: %s\n",CMSG.Reason_B3,Decode_Info(CMSG.Reason_B3));
	}
    }

}

/*--------------------------------------------------------------------------*\
 * Prot_Init: Initialisation of the protocol
\*--------------------------------------------------------------------------*/
int Prot_Init(char *Filename) {
    char *p;

    strcpy(ProtocolFileName, Filename);
    p = strrchr(ProtocolFileName, '.');
    if (p) *p = '\0';
    strcat(ProtocolFileName, ".prt");

    if ((ProtocolFile=fopen(ProtocolFileName, "w"))==NULL) {
        printf("Can't open protocol-file !!\n");
	return FALSE;
    }
    CAPI_PROTOCOL_INIT(CAPI_PROT_BUF, CAPI_PROT_HANDLE);
    return TRUE;
}
#endif

/*--------------------------------------------------------------------------*\
 * The following _h functions are 'h'igh level functions for the ones
 * implemented in CONNECT.C . The _h functions perform some parameter tests
 * that would cause an assert on the low-level functions.
\*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*\
 * Connect_h: Asks for a number to call then executes 'Connect'
\*--------------------------------------------------------------------------*/
unsigned Connect_h(ConnectionID        *Connection,
		   char 	       *CallingPartyNumber,
		   unsigned long       Service,
		   unsigned short      B1Protocol,
		   unsigned short      B2Protocol,
		   unsigned short      B3Protocol,
		   unsigned char __far *B3Configuration) {

    if (*Connection != INVALID_CONNECTION_ID) {
	printf("Connect_h: Connection is already in use\n");
	return 0xFFFF;
    }

    printf("Enter Number to call: ");
    fflush (stdout);
    gets(CalledPartyNumberArr);

    return Connect(Connection,
		   CalledPartyNumberArr,
		   CallingPartyNumber,
		   Service,
		   B1Protocol,
		   B2Protocol,
		   B3Protocol,
		   B3Configuration);
}

/*--------------------------------------------------------------------------*\
 * Disconnect_h: high level Disconnect
\*--------------------------------------------------------------------------*/
unsigned Disconnect_h(ConnectionID Connection) {

    int 	    index;
    ConnectionState State;

    if (Connection == INVALID_CONNECTION_ID) {
	printf("Disconnect_h: ConnectionID is invalid\n");
	return 0xFFFF;
    }
    State = GetState(Connection);
    if ((State == Disconnected) || (State == D_DisconnectPending)) {
	index = GetSlot(Connection);
	printf("Disconnect_h: slot %d ID %d is disconnected\n",index, Connection);
	return 0xFFFF;
    }
    return Disconnect(Connection);
}

/*--------------------------------------------------------------------------*\
 * SendData_h: high level SendData
\*--------------------------------------------------------------------------*/
void InitQueue(void) {
    unsigned	x;

    for (x=0; x<QueueSize; x++) {
	Queue.Element[x].SENT  = FALSE;
    }
    Queue.Head = 0;
    Queue.Tail = 0;
    Queue.Fill = 0;
}

/*--------------------------------------------------------------------------*\
 * AnswerCall_h: high level AnswerCall
\*--------------------------------------------------------------------------*/
void TransferData(int index) {
    MESSAGE_EXCHANGE_ERROR  error;
    unsigned		    t;

    if (Queue.Fill > 0) {
	t = Queue.Tail;
	do {
	    if (Queue.Element[t].SENT == FALSE) {
		error = SendData(index,
				 (void __far *)Queue.Element[t].DATA,
				 Queue.Element[t].DATA_LENGTH,
				 (unsigned short)t);

		if (error != 0) {
                    printf("Error transfering data: 0x%04X !!!\n",error);
		    break;
		}
		Queue.Element[t].SENT = TRUE;
	    }
	    if (++t >= QueueSize) t = 0;
	} while (t != Queue.Head);
    }
}

/*--------------------------------------------------------------------------*\
 * SendFax: Asks for a filename and sends it as a fax
\*--------------------------------------------------------------------------*/
unsigned SendFax(int SlotNr) {

    char	    Filename[80];
    unsigned	    count;
    int 	    FAX_Format;
    B3_PROTO_FAXG3  B3conf;

    printf("This demo program can send ASCII and FAX-format files (.sff). Filenames\n");
    printf("with extensions other than .sff will be interpreted as ASCII-files.\n");
    printf("The name of the demo-fax-file is \"TESTFAX.SFF\".\n");
    printf("Enter Filename: ");
    fflush (stdout);
    gets(Filename);
    FAX_Format = strstr( Filename, ".sff" ) ? FAX_SFF_FORMAT : FAX_ASCII_FORMAT;

    File = fopen(Filename, "rb");
    if (! File) {
#       ifdef DEBUG
        CAPI_PROTOCOL_TEXT("***** could not open file: \"%s\" *****\n",Filename);
#       endif
	return 1;
    }
#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** opening file: \"%s\" *****\n",Filename);
#   endif

    InitQueue();

    SetupB3Config( &B3conf, FAX_Format);
    Connect_h(&Slot[SlotNr],
	      CallingPartyNumber,
	      SPEECH,
	      B1PROTOCOL,
	      B2PROTOCOL,
	      B3PROTOCOL,
	      (_cstruct)&B3conf);

    do {
	Handle_CAPI_Msg();
	if (Slot[SlotNr] == INVALID_CONNECTION_ID) {
#           ifdef DEBUG
            CAPI_PROTOCOL_TEXT("***** disconnected, please try again *****\n\n");
#           endif
	    fclose(File);
	    return 2;
	}
    } while (GetState(Slot[SlotNr]) != Connected);


    FileTransfer = TRUE;

#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** Starting datatransfer, press any key to stop *****\n");
    CAPI_PROTOCOL_TEXT("***** there is no protocol output to the screen during transfer *****\n");
#   endif
    do {

	if ((! feof(File)) && (Queue.Fill < 7)) {  /*----- max. 7 outstanding blocks supported by CAPI -----*/
	    count = fread(&(Queue.Element[Queue.Head].DATA[0]), 1, SendBlockSize, File);
	    if (count > 0) {
		Queue.Element[Queue.Head].DATA_LENGTH = (unsigned short)count;
		if (++Queue.Head >= QueueSize) Queue.Head = 0;
		Queue.Fill++;
	    }
	}
	if (GetState(Slot[SlotNr]) != Connected) {
#           ifdef DEBUG
            CAPI_PROTOCOL_TEXT("***** connection broken *****\n");
#           endif
            break;
        }
        if (kbhit()) {
            getch();
#           ifdef DEBUG
            CAPI_PROTOCOL_TEXT("***** interrupted by user *****\n");
#           endif
            break;
	}
	TransferData(SlotNr);
	Handle_CAPI_Msg();

    } while (!feof(File));

    Disconnect_h(Slot[SlotNr]);
    while ((Slot[SlotNr] != INVALID_CONNECTION_ID) && (GetState(Slot[SlotNr]) != Disconnected)) {
	Handle_CAPI_Msg();
#       if defined (TARGET_NW)
        delay(50);     // the netware NLM has to be cooperative
#       endif
        if (kbhit()) {
            getch();
#           ifdef DEBUG
            CAPI_PROTOCOL_TEXT("***** interrupted by user *****\n");
#           endif
            return 4;
	}

    };

    FileTransfer = FALSE;
#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** End of transfer *****\n");
#   endif
    fclose(File);
    return 0;
}

/*--------------------------------------------------------------------------*\
 * ReceiveFax: Waits for incoming data and stores it to disk
\*--------------------------------------------------------------------------*/
unsigned ReceiveFax(int SlotNr) {

    char    Filename[80];
    char    *p;

    printf("The default extension for the FAX-data is .sff\n");
    printf("Enter Filename where incoming data shall be saved: ");
    fflush (stdout);
    gets(Filename);

    p = strchr(Filename, '.');
    if (p) *p = '\0';
    strcat(Filename, ".sff");


    File = fopen(Filename, "wb");
    if (! File) {
#       ifdef DEBUG
        CAPI_PROTOCOL_TEXT("***** could not open file: \"%s\" *****\n",Filename);
#       endif
	return 3;
    }
    printf("opening file: \"%s\"\n\n",Filename);

    InitQueue();

    FileReceive = TRUE;

#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** Waiting for data , press any key to stop *****\n");
    CAPI_PROTOCOL_TEXT("***** there is no protocol output to the screen during transfer *****\n");
#   endif

    Listen(0x1FFF03FF);

    while (GetState(Slot[SlotNr]) != Connected) {
	Handle_CAPI_Msg();
	if (kbhit()) {
#           if defined (TARGET_NW)
            delay(50);     // the netware NLM has to be cooperative
#           endif
	    getch();
#           ifdef DEBUG
            CAPI_PROTOCOL_TEXT("***** interrupted by user *****\n");
#           endif
	    fclose(File);
	    FileReceive = FALSE;
	    return 0;
	}
    }
    while (GetState(Slot[SlotNr]) != INVAL_STATE) {
	Handle_CAPI_Msg();
	if (kbhit()) {
#           if defined (TARGET_NW)
            delay(50);     // the netware NLM has to be cooperative
#           endif
	    getch();
#           ifdef DEBUG
            CAPI_PROTOCOL_TEXT("***** interrupted by user *****\n");
#           endif
	    FileReceive = FALSE;
	    fclose(File);
	    return 0;
	}
    }

    FileReceive = FALSE;

#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** End of FAX-receive *****\n");
#   endif

    fclose(File);
    return 0;
}

/*--------------------------------------------------------------------------*\
 * HandleKeyStroke: Checks the keyboard
\*--------------------------------------------------------------------------*/
int HandleKeyStroke(void) {
    int  i;

    if (kbhit()) {
	i = Press_Key();
	switch (i) {
	    case 27: {	  /*----- ESCAPE -----*/
                printf("Exit program ?  y/n ");
                fflush (stdout);
		i = Press_Key();
		if ((i == 'y') || (i =='Y')) {
		    puts("Y");
		    return FALSE;
		}
		puts("N");
		return TRUE;
	    }
	    case 's':
	    case 'S':
		SendFax(0);
		break;
	    case 'r':
	    case 'R':
		ReceiveFax(0);
		break;
	}
    }
    return TRUE;
}

/*--------------------------------------------------------------------------*\
 * Interact: main loop, checks keystrokes and CAPI-messages
\*--------------------------------------------------------------------------*/
void Interact(void) {
    int     numController;
    int     BChannels, Contr;


    numController = GetNumController ();
    BChannels = 0;
    for (Contr=1; Contr<=numController; Contr++)
	BChannels += GetNumOfSupportedBChannels(Contr);

    printf("Detected %i controllers with %i B-channels overall.\n\n",
						    numController, BChannels);

    puts("'s'+'S' Send fax");
    puts("'r'+'R' Receive fax");

    do {
#       if defined (TARGET_NW)
        delay(50);     // the netware NLM has to be cooperative
#       endif
	Handle_CAPI_Msg();
    } while (HandleKeyStroke());

    puts("\nProgram terminated\n");
}

/*--------------------------------------------------------------------------*\
 * Hangup: Disconnect both channels
\*--------------------------------------------------------------------------*/
void Hangup(void) {
    int i;


    if ((Slot[0] != INVALID_CONNECTION_ID) &&
		(GetState(Slot[0]) != Disconnected) &&
		(GetState(Slot[0]) != D_DisconnectPending))
	Disconnect(Slot[0]);

    if ((Slot[1] != INVALID_CONNECTION_ID) &&
		(GetState(Slot[1]) != Disconnected) &&
		(GetState(Slot[1]) != D_DisconnectPending))
	Disconnect(Slot[1]);

    do {
	Handle_CAPI_Msg();
#       if defined (TARGET_NW)
        delay(50);     // the netware NLM has to be cooperative
#       endif
	if (kbhit()) {
	    while (kbhit()) {
		getch();
	    }
	    printf("Exit program ?  y/n ");
            fflush (stdout);
	    i = Press_Key();
	    if ((i == 'y') || (i =='Y')) {
		puts("Y");
		return;
	    }
	    puts("N");
	}
    }
    while ((Slot[0] != INVALID_CONNECTION_ID) || (Slot[1] != INVALID_CONNECTION_ID));
}

/*--------------------------------------------------------------------------*\
 * ctrlchandler: exits on CTRL-C and CTRL-BREAK
\*--------------------------------------------------------------------------*/
void ctrlchandler(int sig)
{
    signal( SIGINT, ctrlchandler );
    exit(0);
    sig = 0;	/*----- suppress warning -----*/
}

/*--------------------------------------------------------------------------*\
 * main: Init & exit functions
\*--------------------------------------------------------------------------*/
#ifdef DEBUG
int main(int ac, char *av[]) {
#else
int main(void) {
#endif

    Slot[0] = INVALID_CONNECTION_ID;
    Slot[1] = INVALID_CONNECTION_ID;

    if (! RegisterCAPI ()) return 1;
    atexit (ReleaseCAPI);

    signal(SIGINT, ctrlchandler);

    InitConnectionIDHandling ();


#ifdef DEBUG
    if (! Prot_Init(av[ac-1])) return 2;
#endif

#ifdef __linux__
    init_tty();
    atexit (restore_tty);
#endif

    Interact();

    Hangup();

#ifdef DEBUG
    fclose(ProtocolFile);
#endif
    return 0;
}
