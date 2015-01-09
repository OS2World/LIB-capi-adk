/*--------------------------------------------------------------------------*\

    FAX.C	Version 1.1					    1995 AVM

    Functions and constants used to FAX

\*--------------------------------------------------------------------------*/
#include <string.h>

#include "fax.h"

char	stationID[30] = "123456789012";
char	headLine [30] = "FAX with CAPI 2.0";

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
void SetupB3Config(B3_PROTO_FAXG3  *B3conf,
		   int		   FAX_Format ) {

    int      len1, len2;

    B3conf->resolution = 0;
    B3conf->format     = (unsigned short)FAX_Format;

    len1 = strlen( stationID );
    B3conf->Infos[0] = (unsigned char)len1;
    strcpy( (char *)&B3conf->Infos[1], stationID );

    len2 = strlen( headLine );
    B3conf->Infos[len1 + 1] = (unsigned char)len2;
    strcpy( (char *)&B3conf->Infos[len1 + 2], headLine );
    B3conf->len = (unsigned char)(2 * sizeof( unsigned short ) + len1 + len2 + 2);
}
