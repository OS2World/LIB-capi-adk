


The COMMON ISDN API 2.0 Application Development Kit  V1.6
---------------------------------------------------------

This COMMON ISDN API Application Development Kit is designed for programmers
and other people who are interested in the operation of the COMMON ISDN API.

The COMMON ISDN API 2.0 Application Development Kit supports you in
developing ISDN-based applications. It offers a good introduction into
programming for COMMON ISDN API 2.0 and demonstrates how applications and
devices communicate through a uniform, standardized software interface. The
sample code is largely operating system independent, and should be easy to
port to other platforms. This time the SDK includes complete code for the
usage of the libraries for the operation systems Dos, Windows 16 bit,
Windows 32 bit and the Windows VxD interface.


The COMMON ISDN API (CAPI) enables applications to operate ISDN controller
boards in a simple, consistent manner. It is designed as a standard interface
allowing unrestricted use of all functions of ISDN boards.

Applications using CAPI will not be affected by details specific to hardware
environments or network operators, such as different manufacturers' adapters
or national D-channel protocols. Furthermore, future technical developments
in ISDN hardware will be transparent to applications using the
COMMON ISDN API. The interface therefore provides a solid basis for all
ISDN-related techniques.

Thus, ISDN-specific applications can be built or other protocols linked to
ISDN over a defined, structured interface, which has been supported in Europe
as an ETSI standard since 1994.

The initial intention behind COMMON ISDN API was to create a uniform, open
software interface for using ISDN with the PC. This open interface is the
hardware-independent platform for ISDN applications. A second objective
was to design the interface at a high level of abstraction in order to allow
its unrestricted use at any BRI or PRI.


Functional Characteristics

The COMMON ISDN API connects any number of applications to any number of
controllers. It supports layer 1 to 3 of the OSI protocol stack. The API
rests above layer 3 as the point of access for applications and higher-layer
protocols.

Applications and COMMON ISDN API communicate with each other by exchanging
command and data messages in message queues. When an application issues a
command to an ISDN device (driver and/or controller), for example, the
command is posted to the COMMON ISDN API's corresponding message queue. In
the opposite direction, events from an ISDN device (driver and/or controller)
are posted to the message queue of the appropriate application.

The COMMON ISDN API Application Developing Kit gives you support in
developing ISDN based applications. When you access CAPI direct from the
application, you have to assemble correct CAPI 2.0 messages yourself and
make your own trace functions. Or you can use the Development Kit Library,
which contains functions that build the CAPI 2.0 messages for you
(CAPI20_CMSG_2_MESSAGE) out of a C-structure (_cmsg) and protocol functions
with detailed output.


    +----------------------------------------+
    |		  application		     |
    +----------------------------------------+
	   |			|
	   |			V
	   |	     +-----------------------+
	   |	     | CAPI 2.0 Development  |--------> Protocol function
	   |	     |	   Kit library	     |		easier handling
	   |	     +-----------------------+		...
	   |			|
	   V			V
    +--------------------------------------+
    |              CAPI 2.0                |
    | operation system specific functions  |
    +--------------------------------------+



Contents

The COMMON ISDN API 2.0 Application Development Kit provides all CAPI 2.0
functions for MS-DOS as defined in the CAPI 2.0 specification of February,
1994. Access to all parameters in CAPI messages is facilitated through
C source, library and header files.


The COMMON ISDN API 2.0 Application Development Kit consists of the
following components:

1. This description.
2. The COMMON ISDN API 2.0 specification and the addenda
3. A complete development environment, including the required libraries and
   header files to support all COMMON ISDN API functions.
4. Sample source codes (C and header files) including explanations of the
   COMMON ISDN API functions and their use, as well as a complete sample
   application.
5. The compiled sample application.

The kit includes the following files:

<Basedirectory> \

   README.TXT   This text.

   WHATS.NEW    New features of the CAPI 2.0 Development Kit V1.4

\CAPISPEC\

   CAPI20.DOC   Word document containing the CAPI 2.0 specification

   ADDENDUM.DOC Word document containing the addendum to the CAPI 2.0
                specification including specifications for Windows 95
		and Windows NT

\DEBUG.DLL\

   CAPI11.DLL   16-bit CAPI 1.1 protocol DLL

   CAPI20.DLL   16-bit CAPI 2.0 protocol DLL

   CAPI2032.DLL 32-bit CAPI 2.0 protocol DLL

   README.TXT   Installation instructions for protocol DLLs


\SOURCE\

   CAPI.C	CAPI 2.0 state machine, which handles all incoming messages.
   CAPI.H	Header file.

   MAIN.C	The sample program (DEMO.EXE) built using the development
		kit. Allows you to connect and send data packets and
		complete files to another B-channel: writes a log file to
		disk. See the source file for further information on how to
		use the sample program.
   FAXMAIN.C	The sample program for sending and receiving FAX.

   MAIN.H	The callback-functions that are used by CAPI.C are defined
		here. This file is used by both MAIN.C and FAXMAIN.C.

   C20MSG.C     CAPI 2.0 error messages and message names
   C20MSG.H	Header file.

   CONNECT.C	Functions to connect and disconnect.
   CONNECT.H	Header file.

   CONTR.C	Functions concerning information about controllers.
   CONTR.H	Header file.

   DATA.C	Functions to transfer data over B-channels.
   DATA.H	Header file.

   FAX.C	Functions and constants used for sending and receiving FAX
   FAX.H	Header file.

   ID.C 	Functions handling connections.
   ID.H 	Header file.

   INIT.C	Functions to initialize CAPI.
   INIT.H	Header file.

   OS.H         Header file for operating system dependent files.

   TESTFAX.SFF	Sample FAX.

   LINUTI.C     Funktions that are not available with Linux.
   LINUTI.H     Header file.

   LINUX.MAK    Make routine for the sample Linux program.

   MAKEDEMO.BAT Make routine for the sample DOS program.
   MAKEDEMO.LNK Linker input file for sample DOS program

   MAKEFAX.BAT  Make routine for the sample DOS program.
   MAKEFAX.LNK  Linker input file for sample DOS program

   MKDEM32.BAT  Make routine for the sample DOS program.
   MKDEM32.LNK  Linker input file for sample DOS program

   MKDEMO2.BAT  Make routine for the sample DOS program.
   MKFAX2.BAT   Make routine for the sample DOS program.

   NW_MKDMO.BAT Make routine for the sample NetWare program.
   NW_DMO.RSP   Response file for sample NetWare program.
   NW_MKFAX.BAT Make routine for the sample NetWare program.
   NW_FAX.RSP   Response file for sample NetWare program.


\SOURCE\DOS

   DEMO.EXE     DOS executable of the demo program
   FAX.EXE      DOS executable of the Fax-demo program

\SOURCE\LINUX

   DEMO         Linux executable of the demo program
   FAXMAIN      Linux executable of the Fax-demo program

\SOURCE\OS2

   DEMO2.EXE    OS/2 executable of the demo program
   FAX2.EXE     OS/2 executable of the Fax-demo program

\SOURCE\WIN32

   DEMO32.EXE   Windows 32 bit executable of the demo program

\SOURCE\NETWARE

   DEMO.NLM     NetWare NLM executable of the demo program
   FAXDEMO.NLM  NetWare NLM executable of the demo fax program


\C20LIB

   MKDOS.BAT    Make routine for DOS libraries (see description in the file).
   MKWIN16.BAT  Make routine for Windows 16 bit libraries (see description in
                the file).
   MKWIN32.BAT  Make routine for Windows 32 bit libraries (see description in
                the file).
   MKVXD.BAT    Make routine for Windows 32 bit VxD libraries (see description
                in the file).
   MKOS2.CMD    Make routine for OS/2 libraries
   LINUX.MAK    Make routine for Linux libraries

   MKNW.BAT     Make routine for NetWare libraries


\C20LIB\SOURCE

   CAPI20.H     Header file for all CAPI20xx librarys.

   CDOS.C	C file for building the CAPI20xx libraries for DOS.
   CDOS.H	Header file for the DOS-specific CAPI 2.0 functions.

   CWIN16.C     C-file for building the CAPI20xx libraries for Windows 16 bit.
   CWIN16.H     Header file for the Windows(16)-specific CAPI 2.0 functions.

   CWIN32.C     C-file for building the CAPI20xx libraries for Windows 32 bit.
   CWIN32.H     Header file for the Windows(32)-specific CAPI 2.0 functions.

   CVXD.C       C-file for building the CAPI20xx libraries for Windows VxDs.
   CVXD.H       Header file for the VXD-specific CAPI 2.0 functions.

   COS2.C       C file for building the CAPI20xx libraries for OS/2.
   COS2.H       Header file for the OS/2-specific CAPI 2.0 functions.

   CLINUX.C     C file for building the CAPI20xx libraries for Linux.
   CLINUX.H     Header file for the Linux-specific CAPI 2.0 functions.

   C_NW.C       C file for building the CAPI20xx libraries for NetWare.
   C_NW.H       Header files for the NetWare-specific CAPI 2.0 functions
   C2IMP_NW.H
   C2MGR_NW.H

   FUNC.C	CAPI 2.0 functions.
   GCMSGDOS.C	Functions specific to DOS.
   GCMSGWIN.C   Functions specific to Windows 16 and 32 bit.
   GCMSGVXD.C   Functions specific to Windows VxDs.
   GCMSGOS2.C   Functions specific to OS/2.
   GCMSGLIN.C   Functions specific to Linux.
   GCMSG_NW.C   Functions specific to NetWare
   MSG.C        Internal CAPI 2.0 Application Development Kit functions.


\C20LIB\DOS
   This directory contains the DOS-specific builds.

   CAPI20S.LIB	CAPI 2.0 development kit library for the small memory model.
   CAPI20L.LIB	CAPI 2.0 development kit library for the large memory model.
   CAPI20SD.LIB CAPI 2.0 development kit library for the small memory model,
		including protocol functions.
   CAPI20LD.LIB CAPI 2.0 development kit library for the large memory model,
		including protocol functions.

\C20LIB\WIN16
   This directory contains the Windows 16 bit-specific builds.

   CAPI20S.LIB	CAPI 2.0 development kit library for the small memory model.
   CAPI20L.LIB	CAPI 2.0 development kit library for the large memory model.
   CAPI20SD.LIB CAPI 2.0 development kit library for the small memory model,
		including protocol functions.
   CAPI20LD.LIB CAPI 2.0 development kit library for the large memory model,
		including protocol functions.
   CAPI20W.LIB	Windows link library for dynamically linking a manufacturer-
		specific CAPI20.DLL.

\C20LIB\WIN32
   This directory contains the Windows 32 bit-specific builds.

   CAP2032.LIB  CAPI 2.0 development kit library.
   CAP2032D.LIB CAPI 2.0 development kit library including protocol functions.
   CAPI2032.LIB Windows link library for dynamically linking a manufacturer-
                specific CAPI2032.DLL.

\C20LIB\VXD
   This directory contains the Windows 32 bit VxD-specific builds.

   CAPI20.CLB   CAPI 2.0 development kit library.
   CAPI20D.CLB  CAPI 2.0 development kit library including protocol functions.

\C20LIB\OS2
   This directory contains the OS/2-specific builds.

   CAPI20.LIB   CAPI 2.0 development kit library.
   CAPI20D.LIB  CAPI 2.0 development kit library including protocol functions.
   CAPI20I.LIB  OS/2 link library for dynamically linking a manufacturer-
                specific CAPI2032.DLL.

\C20LIB\LINUX
   This directory contains the Linux-specific builds.

   LIBC20.a     CAPI 2.0 development kit library.


\C20LIB\NETWARE
   This directory contains the NetWare-specific builds.

   CAP2032.LIB   CAPI 2.0 development kit library.
   CAP2032D.LIB  CAPI 2.0 development kit library including protocol functions.
