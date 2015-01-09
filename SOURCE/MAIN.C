/*--------------------------------------------------------------------------*\

    MAIN.C							    1995 AVM

    This demo program shows how to use the CAPI 2.0 Development Kit.
    When the program is started, it prompts for a number which will be
    called from the CONNECT routine. After that there is the main loop where
    the program handles CAPI and keystrokes.
    Following is a list of available keys:
    'l'     Listen (CIP mask = 0x1FFF03FF) every service will be indicated
    'L'     Listen (CIP mask = 0x00000000) no incoming call will be indicated
	    Listen is sent to all available controllers
    'c'     Connect to specified number on slot 0
    'C'     Connect to specified number on slot 1
    'd'     Disconnect slot 0
    'D'     Disconnect slot 1
    's'     Send datablock with size "SendBlockSize" on slot 0
    'S'     Send datablock with size "SendBlockSize" on slot 1
    'f'     Transfer a file on slot 0
    'F'     Transfer a file on slot 1
    'v'     Receive a file on slot 0
    'V'     Receive a file on slot 1
    'a'     Accept incoming call on slot 0
    'A'     Accept incoming call on slot 1
    'i'     Ignore incoming call on slot 0
    'I'     Ignore incoming call on slot 1
    'r'     Reject incoming call on slot 0
    'R'     Reject incoming call on slot 1

    Next is an example how to connect the local 2 B-channels:
    !! This will cost the same as one telephone call !!
    After the start press 'l' so that every incoming call will be indicated.
    Press 'c' and the program asks for a number to call, then dials the number.
    There should be an incoming call indication on slot 1. Press 'A' and the
    program answers the call on the second slot. Now everytime you send data
    with 's' or 'S' there must be an data indication on the opposite slot.
    Disconnect the slot with 'd' or 'D'

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

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
static  char    testblock[2048];

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


#define B1PROTOCOL	    0
#define B2PROTOCOL	    0
#define B3PROTOCOL	    0
#define B3CONFIGURATION     NULL


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
    int     index;

    assert (Connection != INVALID_CONNECTION_ID);


#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** incoming call ,ID %d, caller: \"%s\" *****\n",Connection,CallingPartyNumber);
#   endif
    index = AllocSlot(Connection);

    if (index == INVALID_SLOT) {
#       ifdef DEBUG
        CAPI_PROTOCOL_TEXT("***** no free slot available, rejecting call... *****\n");
#       endif
	AnswerCall(Connection, REJECT,0,0,0,NULL);
	return;
    }
#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** call assigned to slot %d *****\n", index);
#   endif
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
    fflush(ProtocolFile);
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

    printf("Enter Number: ");
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
unsigned SendData_h(ConnectionID    Connection,
		    void __far	    *Data,
		    unsigned short  DataLength,
		    unsigned short  DataHandle) {

    int 	    index;
    ConnectionState State;

    if (Connection == INVALID_CONNECTION_ID) {
	printf("SendData_h: ConnectionID is invalid\n");
	return 0xFFFF;
    }
    State = GetState(Connection);
    if (State != Connected) {
	index = GetSlot(Connection);
	printf("SendData_h: slot %d ID %d is not connected\n",index, Connection);
	return 0xFFFF;
    }
    return SendData(Connection, Data, DataLength, DataHandle);
}

/*--------------------------------------------------------------------------*\
 * AnswerCall_h: high level AnswerCall
\*--------------------------------------------------------------------------*/
unsigned AnswerCall_h(ConnectionID	  Connection,
		      RejectValue	  Reject,
		      unsigned short	  B1Protocol,
		      unsigned short	  B2Protocol,
		      unsigned short	  B3Protocol,
		      unsigned char __far *B3Configuration) {

    int 	    index;
    ConnectionState State;

    if (Connection == INVALID_CONNECTION_ID) {
	printf("AnswerCall_h: ConnectionID is invalid\n");
	return 0xFFFF;
    }
    State = GetState(Connection);
    if (State != D_ConnectPending) {
	index = GetSlot(Connection);
	printf("AnswerCall_h: slot %d ID %d is the wrong state for answering\n",index, Connection);
	return 0xFFFF;
    }
    return AnswerCall(Connection,
		      Reject,
		      B1Protocol,
		      B2Protocol,
		      B3Protocol,
		      B3Configuration);
}

/*--------------------------------------------------------------------------*\
 * InitQueue: resets the data queue
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
 * TransferData: sends datablocks from the queue to CAPI until a) all Blocks
 * from the queue are sent or b) an error occurs. This functions trys to send
 * the maximum of 7 unconfirmed datablocks to CAPI for maximum throughput.
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
		    printf("Error transfering data: 0x%04X !!!",error);
		    break;
		}
		Queue.Element[t].SENT = TRUE;
	    }
	    if (++t >= QueueSize) t = 0;
	} while (t != Queue.Head);
    }
}

/*--------------------------------------------------------------------------*\
 * SendFile: Sends a file
\*--------------------------------------------------------------------------*/
unsigned SendFile(int index) {

    char	    Filename[80];
    unsigned	    count;

    if (Slot[index] == INVALID_CONNECTION_ID) {
	printf("SendFile: ConnectionID is invalid\n");
	return 1;
    }
    if (GetState(Slot[index]) != Connected) {
	printf("SendFile: slot %d ID %d is not connected\n",index, Slot[index]);
	return 2;
    }
    printf("Enter Filename: ");
    fflush (stdout);
    gets(Filename);

    File = fopen(Filename, "rb");
    if (! File) {
#       ifdef DEBUG
        CAPI_PROTOCOL_TEXT("***** file not found *****\n");
#       endif
	return 3;
    }
    InitQueue();

    FileTransfer = TRUE;

#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** Starting datatransfer on slot %d, press any key to stop *****\n",index);
    CAPI_PROTOCOL_TEXT("***** there is no protocol output to the screen during transfer *****\n\n");
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
	if (GetState(Slot[index]) != Connected) {
#           ifdef DEBUG
            CAPI_PROTOCOL_TEXT("***** connection broken *****\n");
#           endif
            break;
        }
        if (kbhit()) {
#           ifdef DEBUG
            CAPI_PROTOCOL_TEXT("***** interrupted by user *****\n");
#           endif
            break;
	}
	TransferData(index);
	Handle_CAPI_Msg();
    } while (Queue.Fill > 0);

    FileTransfer = FALSE;
#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** End of filetransfer *****\n");
#   endif
    fclose(File);
    return 0;
}

/*--------------------------------------------------------------------------*\
 * ReceiveFile: Receives a file and writes it to disk
\*--------------------------------------------------------------------------*/
unsigned ReceiveFile(int index) {

    char	    Filename[80];

    if (Slot[index] == INVALID_CONNECTION_ID) {
	printf("ReceiveFile: ConnectionID is invalid\n");
	return 1;
    }
    if (GetState(Slot[index]) != Connected) {
	printf("ReceiveFile: slot %d ID %d is not connected\n",index, Slot[index]);
	return 2;
    }
    printf("Enter Filename where incoming data shall be saved: ");
    fflush (stdout);
    gets(Filename);

    File = fopen(Filename, "wb");
    if (! File) {
#       ifdef DEBUG
        CAPI_PROTOCOL_TEXT("***** could not open file *****\n");
#       endif
	return 3;
    }
    InitQueue();

    FileReceive = TRUE;

#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** Waiting for data on slot %d, press any key to stop *****\n",index);
    CAPI_PROTOCOL_TEXT("***** there is no protocol output to the screen during transfer *****\n");
#   endif
    do {
	if (GetState(Slot[index]) != Connected) {
#           ifdef DEBUG
            CAPI_PROTOCOL_TEXT("***** connection broken *****\n");
#   endif
            break;
        }
	Handle_CAPI_Msg();
    } while (! kbhit());

    getch();
    FileReceive = FALSE;
#   ifdef DEBUG
    CAPI_PROTOCOL_TEXT("***** End of filetransfer *****\n");
#   endif
    fclose(File);
    return 0;
}

/*---------------------------------------------------------------------------*\
 * PrintHelp: tell the user the options
\*---------------------------------------------------------------------------*/
static void PrintHelp (void) {

    puts("\n\n");
    puts("'ESC'   Exit program");
    puts("'l'     Listen (CIP mask = 0x1FFF03FF) every service will be indicated");
    puts("'L'     Listen (CIP mask = 0x00000000) no incoming call will be indicated");
    puts("'c'+'C' Connect, the specified number will be called");
    puts("'d'     Disconnect slot 0");
    puts("'D'     Disconnect slot 1");
    puts("'s'     Send datablock with size \"SendBlockSize\" on slot 0");
    puts("'S'     Send datablock with size \"SendBlockSize\" on slot 1");
    puts("'f'     Transfer a file on slot 0");
    puts("'F'     Transfer a file on slot 1");
    puts("'v'     Receive a file on slot 0");
    puts("'V'     Receive a file on slot 1");
    puts("'a'     Accept incoming call on slot 0");
    puts("'A'     Accept incoming call on slot 1");
    puts("'i'     Ignore incoming call on slot 0");
    puts("'I'     Ignore incoming call on slot 1");
    puts("'r'     Reject incoming call on slot 0");
    puts("'R'     Reject incoming call on slot 1");
    puts("'?'     Print this help screen");
}

/*--------------------------------------------------------------------------*\
 * HandleKeyStroke: Checks the keyboard
\*--------------------------------------------------------------------------*/
int HandleKeyStroke(void) {
    int  i;

#if defined (TARGET_NW)
    delay(50);     // the netware NLM has to be cooperative
#endif
    if (kbhit()) {
	i = Press_Key();
	switch (i) {
	    case 'q':
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
	    case 'l':
                Listen(ALL_SERVICES);
		break;
	    case 'L':
                Listen(NO_SERVICES);
		break;
	    case 'c':
		Connect_h(&Slot[0],
			  CallingPartyNumber,
			  DATA_TRANSFER,
			  B1PROTOCOL,
			  B2PROTOCOL,
			  B3PROTOCOL,
			  B3CONFIGURATION);
		break;
	    case 'C':
		Connect_h(&Slot[1],
			  CallingPartyNumber,
			  DATA_TRANSFER,
			  B1PROTOCOL,
			  B2PROTOCOL,
			  B3PROTOCOL,
			  B3CONFIGURATION);
		break;
	    case 'd':
		Disconnect_h(Slot[0]);
		break;
	    case 'D':
		Disconnect_h(Slot[1]);
		break;
	    case 's':
                SendData_h(Slot[0], (void __far *)&testblock, SendBlockSize, 1);
		break;
	    case 'S':
                SendData_h(Slot[1], (void __far *)&testblock, SendBlockSize, 1);
		break;
	    case 'f':
		SendFile(Slot[0]);
		break;
	    case 'F':
		SendFile(Slot[1]);
		break;
	    case 'v':
		ReceiveFile(Slot[0]);
		break;
	    case 'V':
		ReceiveFile(Slot[1]);
		break;
	    case 'a':
                AnswerCall_h(Slot[0],ACCEPT,B1PROTOCOL,B2PROTOCOL,B3PROTOCOL,NULL);
		break;
	    case 'A':
                AnswerCall_h(Slot[1],ACCEPT,B1PROTOCOL,B2PROTOCOL,B3PROTOCOL,NULL);
		break;
	    case 'i':
		AnswerCall_h(Slot[0],IGNORE,0,0,0,NULL);
		break;
	    case 'I':
		AnswerCall_h(Slot[1],IGNORE,0,0,0,NULL);
		break;
	    case 'r':
		AnswerCall_h(Slot[0],REJECT,0,0,0,NULL);
		break;
	    case 'R':
		AnswerCall_h(Slot[1],REJECT,0,0,0,NULL);
		break;
            case 'h':
            case '?':
                PrintHelp();
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

    printf("Detected %i controllers with %i B-channels overall.\n",
						    numController, BChannels);
    PrintHelp ();

    do {
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
