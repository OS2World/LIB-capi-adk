/*--------------------------------------------------------------------------*\

    OS.H       Version 1.1                                         1997 AVM

    Header file for OS.H

\*--------------------------------------------------------------------------*/
#ifndef _os_h_
#define _os_h_
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#if defined (TARGET_32BIT)
#   ifdef __linux__
#       include "clinux.h"
#       include "linuti.h"
#   elif defined (TARGET_OS2)
#       include "cos2.h"
#       include <conio.h>
#   elif defined (TARGET_NW)
#       include "c_nw.h"
#       include <nwconio.h>
#       include <nwthread.h>
#   else
#       include "cwin32.h"
#       include <conio.h>
#   endif
#else
#   include "cdos.h"
#   include <conio.h>
#endif
/*---------------------------------------------------------------------------*\
\*---------------------------------------------------------------------------*/
#endif
