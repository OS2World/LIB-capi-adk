/*--------------------------------------------------------------------------*\

    FUNC.C      Version 1.2                                         1997 AVM

    This file contains the source of the CAPI functions that correspond
    to the CAPI messages described in the CAPI 2.0 spec.

\*--------------------------------------------------------------------------*/
#include "capi20.h"

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned ALERT_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                    _cdword adr,
                    _cstruct BChannelinformation,
                    _cstruct Keypadfacility,
                    _cstruct Useruserdata,
                    _cstruct Facilitydataarray) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x01,0x80,Messagenumber,adr);
    cmsg->BChannelinformation = BChannelinformation;
    cmsg->Keypadfacility = Keypadfacility;
    cmsg->Useruserdata = Useruserdata;
    cmsg->Facilitydataarray = Facilitydataarray;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CONNECT_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                      _cdword adr,
                      _cword CIPValue,
                      _cstruct CalledPartyNumber,
                      _cstruct CallingPartyNumber,
                      _cstruct CalledPartySubaddress,
                      _cstruct CallingPartySubaddress,
                      _cword B1protocol,
                      _cword B2protocol,
                      _cword B3protocol,
                      _cstruct B1configuration,
                      _cstruct B2configuration,
                      _cstruct B3configuration,
                      _cstruct BC,
                      _cstruct LLC,
                      _cstruct HLC,
                      _cstruct BChannelinformation,
                      _cstruct Keypadfacility,
                      _cstruct Useruserdata,
                      _cstruct Facilitydataarray) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x02,0x80,Messagenumber,adr);
    cmsg->CIPValue = CIPValue;
    cmsg->CalledPartyNumber = CalledPartyNumber;
    cmsg->CallingPartyNumber = CallingPartyNumber;
    cmsg->CalledPartySubaddress = CalledPartySubaddress;
    cmsg->CallingPartySubaddress = CallingPartySubaddress;
    cmsg->B1protocol = B1protocol;
    cmsg->B2protocol = B2protocol;
    cmsg->B3protocol = B3protocol;
    cmsg->B1configuration = B1configuration;
    cmsg->B2configuration = B2configuration;
    cmsg->B3configuration = B3configuration;
    cmsg->BC = BC;
    cmsg->LLC = LLC;
    cmsg->HLC = HLC;
    cmsg->BChannelinformation = BChannelinformation;
    cmsg->Keypadfacility = Keypadfacility;
    cmsg->Useruserdata = Useruserdata;
    cmsg->Facilitydataarray = Facilitydataarray;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CONNECT_B3_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                         _cdword adr,
                         _cstruct NCPI) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x82,0x80,Messagenumber,adr);
    cmsg->NCPI = NCPI;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned DATA_B3_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                      _cdword adr,
                      _cdword Data,
                      _cword DataLength,
                      _cword DataHandle,
                      _cword Flags) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x86,0x80,Messagenumber,adr);
    cmsg->Data = Data;
    cmsg->DataLength = DataLength;
    cmsg->DataHandle = DataHandle;
    cmsg->Flags = Flags;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned DISCONNECT_B3_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                            _cdword adr,
                            _cstruct NCPI) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x84,0x80,Messagenumber,adr);
    cmsg->NCPI = NCPI;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned DISCONNECT_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                         _cdword adr,
                         _cstruct BChannelinformation,
                         _cstruct Keypadfacility,
                         _cstruct Useruserdata,
                         _cstruct Facilitydataarray) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x04,0x80,Messagenumber,adr);
    cmsg->BChannelinformation = BChannelinformation;
    cmsg->Keypadfacility = Keypadfacility;
    cmsg->Useruserdata = Useruserdata;
    cmsg->Facilitydataarray = Facilitydataarray;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned FACILITY_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                       _cdword adr,
                       _cword FacilitySelector,
                       _cstruct FacilityRequestParameter) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x80,0x80,Messagenumber,adr);
    cmsg->FacilitySelector = FacilitySelector;
    cmsg->FacilityRequestParameter = FacilityRequestParameter;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned INFO_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                   _cdword adr,
                   _cstruct CalledPartyNumber,
                   _cstruct BChannelinformation,
                   _cstruct Keypadfacility,
                   _cstruct Useruserdata,
                   _cstruct Facilitydataarray) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x08,0x80,Messagenumber,adr);
    cmsg->CalledPartyNumber = CalledPartyNumber;
    cmsg->BChannelinformation = BChannelinformation;
    cmsg->Keypadfacility = Keypadfacility;
    cmsg->Useruserdata = Useruserdata;
    cmsg->Facilitydataarray = Facilitydataarray;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned LISTEN_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                     _cdword adr,
                     _cdword InfoMask,
                     _cdword CIPmask,
                     _cdword CIPmask2,
                     _cstruct CallingPartyNumber,
                     _cstruct CallingPartySubaddress) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x05,0x80,Messagenumber,adr);
    cmsg->InfoMask = InfoMask;
    cmsg->CIPmask = CIPmask;
    cmsg->CIPmask2 = CIPmask2;
    cmsg->CallingPartyNumber = CallingPartyNumber;
    cmsg->CallingPartySubaddress = CallingPartySubaddress;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned MANUFACTURER_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                           _cdword adr,
                           _cdword ManuID,
                           _cdword Class,
                           _cdword Function,
                           _cstruct ManuData) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0xff,0x80,Messagenumber,adr);
    cmsg->ManuID = ManuID;
    cmsg->Class = Class;
    cmsg->Function = Function;
    cmsg->ManuData = ManuData;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned RESET_B3_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                       _cdword adr,
                       _cstruct NCPI) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x87,0x80,Messagenumber,adr);
    cmsg->NCPI = NCPI;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned SELECT_B_PROTOCOL_REQ (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                                _cdword adr,
                                _cword B1protocol,
                                _cword B2protocol,
                                _cword B3protocol,
                                _cstruct B1configuration,
                                _cstruct B2configuration,
                                _cstruct B3configuration) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x41,0x80,Messagenumber,adr);
    cmsg->B1protocol = B1protocol;
    cmsg->B2protocol = B2protocol;
    cmsg->B3protocol = B3protocol;
    cmsg->B1configuration = B1configuration;
    cmsg->B2configuration = B2configuration;
    cmsg->B3configuration = B3configuration;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CONNECT_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                       _cdword adr,
                       _cword Reject,
                       _cword B1protocol,
                       _cword B2protocol,
                       _cword B3protocol,
                       _cstruct B1configuration,
                       _cstruct B2configuration,
                       _cstruct B3configuration,
                       _cstruct ConnectedNumber,
                       _cstruct ConnectedSubaddress,
                       _cstruct LLC,
                       _cstruct BChannelinformation,
                       _cstruct Keypadfacility,
                       _cstruct Useruserdata,
                       _cstruct Facilitydataarray) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x02,0x83,Messagenumber,adr);
    cmsg->Reject = Reject;
    cmsg->B1protocol = B1protocol;
    cmsg->B2protocol = B2protocol;
    cmsg->B3protocol = B3protocol;
    cmsg->B1configuration = B1configuration;
    cmsg->B2configuration = B2configuration;
    cmsg->B3configuration = B3configuration;
    cmsg->ConnectedNumber = ConnectedNumber;
    cmsg->ConnectedSubaddress = ConnectedSubaddress;
    cmsg->LLC = LLC;
    cmsg->BChannelinformation = BChannelinformation;
    cmsg->Keypadfacility = Keypadfacility;
    cmsg->Useruserdata = Useruserdata;
    cmsg->Facilitydataarray = Facilitydataarray;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CONNECT_ACTIVE_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                              _cdword adr) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x03,0x83,Messagenumber,adr);
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CONNECT_B3_ACTIVE_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                                 _cdword adr) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x83,0x83,Messagenumber,adr);
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CONNECT_B3_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                          _cdword adr,
                          _cword Reject,
                          _cstruct NCPI) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x82,0x83,Messagenumber,adr);
    cmsg->Reject = Reject;
    cmsg->NCPI = NCPI;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned CONNECT_B3_T90_ACTIVE_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                                     _cdword adr) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x88,0x83,Messagenumber,adr);
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned DATA_B3_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                       _cdword adr,
                       _cword DataHandle) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x86,0x83,Messagenumber,adr);
    cmsg->DataHandle = DataHandle;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned DISCONNECT_B3_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                             _cdword adr) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x84,0x83,Messagenumber,adr);
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned DISCONNECT_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                          _cdword adr) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x04,0x83,Messagenumber,adr);
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned FACILITY_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                        _cdword adr,
                        _cword FacilitySelector,
                        _cstruct FacilityResponseParameters) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x80,0x83,Messagenumber,adr);
    cmsg->FacilitySelector = FacilitySelector;
    cmsg->FacilityResponseParameters = FacilityResponseParameters;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned INFO_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                    _cdword adr) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x08,0x83,Messagenumber,adr);
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned MANUFACTURER_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                            _cdword adr,
                            _cdword ManuID,
                            _cdword Class,
                            _cdword Function,
                            _cstruct ManuData) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0xff,0x83,Messagenumber,adr);
    cmsg->ManuID = ManuID;
    cmsg->Class = Class;
    cmsg->Function = Function;
    cmsg->ManuData = ManuData;
    return CAPI_PUT_CMSG (cmsg);
}

/*--------------------------------------------------------------------------*\
\*--------------------------------------------------------------------------*/
unsigned RESET_B3_RESP (_cmsg __far *cmsg, _cword ApplId, _cword Messagenumber,
                        _cdword adr) {
    CAPI_CMSG_HEADER (cmsg,ApplId,0x87,0x83,Messagenumber,adr);
    return CAPI_PUT_CMSG (cmsg);
}
