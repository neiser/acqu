//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TDAQ_TRB3


#include "TDAQ_TRB3.h"

// make our life a bit more C++'ish
#include <iostream>
#include <iomanip>
using namespace std;

ClassImp(TDAQ_TRB3)

enum { ETRB3_Debug=100, ETRB3_CtsAddr };
static Map_t kTRB3Keys[] = {
  {"TRB3-Debug:", ETRB3_Debug},
  {"TRB3-CtsAddr:", ETRB3_CtsAddr},
  {NULL,           -1}
};




TDAQ_TRB3::TDAQ_TRB3( Char_t* name, Char_t* file, FILE* log, Char_t* line ):
  TDAQmodule( name, file, log ),
  fTRB3debug(kFALSE),
  fPacket(fPacketMaxSize/sizeof(UInt_t),'0'),
  fAddrHubs(),
  fAddrCTS(0xffff),
  fAddrEndpoints()
{
  // Basic initialisation
  fCtrl = new TDAQcontrol(this);
  fType = EDAQ_Ctrl;                         // controller board (has slaves)
  AddCmdList( kTRB3Keys );                  // TRB3-specific cmds
}

//-----------------------------------------------------------------------------
TDAQ_TRB3::~TDAQ_TRB3( )
{
  // Disconnect
  close(fSocket);
}

//-----------------------------------------------------------------------------
void TDAQ_TRB3::InitSocket()
{
  fSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (fSocket==-1)
    PrintError("TDAQ_TRB3","socket()",EErrFatal);
  else if(fTRB3debug)
    printf("TDAQ_TRB3: socket() successful\n");

  // don't know if increasing the UDP receive buffer is really necessary
  // especially newer kernels have a really smart autotuning feature for this...
  /*Int_t recvBuf = 1024*1024*10;
  if(setsockopt(fSocket, SOL_SOCKET, SO_RCVBUF, &recvBuf, sizeof(recvBuf))==-1)
    PrintError("TDAQ_TRB3","Socket Buffer Size",EErrFatal);*/

  struct sockaddr_in my_addr;
  bzero(&my_addr, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(50000);
  inet_pton(my_addr.sin_family, "192.168.1.1", &(my_addr.sin_addr));

  if (bind(fSocket, (struct sockaddr* ) &my_addr, sizeof(my_addr))==-1)
    PrintError("TDAQ_TRB3","bind()",EErrFatal);
  else if(fTRB3debug)
    printf("TDAQ_TRB3: bind() successful\n");
}

//-----------------------------------------------------------------------------
void TDAQ_TRB3::SetConfig( Char_t* line, Int_t key )
{
  // Configuration from file
  switch( key ) {
  case ETRB3_Debug:
    // enable debug output (prints alot to stdout!)
    fTRB3debug = kTRUE;
    break;

  case ETRB3_CtsAddr:
    // get the CTS address (needed for unpacking the UDP packet)
    if(sscanf(line,"%x", &fAddrCTS) != 1)
      PrintError(line, "<TDAQ_TRB3 CtsAddr setup line parse>", EErrFatal);
    break;

  default:
    // try general module setup commands
    TDAQmodule::SetConfig(line, key);
    break;
  }
}

//-----------------------------------------------------------------------------
void TDAQ_TRB3::PostInit()
{
  if( fIsInit ) {
    // we are already initialized
    return;
  }
  TDAQmodule::PostInit();
  if(fAddrCTS==0xffff)
    PrintError("TDAQ_TRB3", "No Cts Addr has been provided", EErrFatal);
  InitSocket();
}

//---------------------------------------------------------------------------
void TDAQ_TRB3::WaitIRQ( )
{
  for(;;) {
    if(fIsIRQEnabled) {      // "interrupt" enabled?
      struct sockaddr_in cli_addr;
      socklen_t slen=sizeof(cli_addr);
      // recvfrom expects a pointer to where it should store the data
      Int_t recvBytes = recvfrom(fSocket, &(fPacket)[0], fPacketMaxSize, 0,
                                 (struct sockaddr*)&cli_addr, &slen);
      if(recvBytes==-1)
        PrintError("TDAQ_TRB3","recvfrom()",EErrFatal);
      // TRB3 packets should have 32bit words => number of bytes is multiple of 4
      Int_t rem = recvBytes % sizeof(UInt_t);
      if(rem != 0) {
        PrintError("TDAQ_TRB3","recvfrom(): #Bytes not multiple of UInt_t size",EErrNonFatal);
        recvBytes += sizeof(UInt_t)-rem;
      }
      fPacket.resize(recvBytes/4);

      // transform from network byte order to host byte order
      transform(fPacket.begin(),fPacket.end(),fPacket.begin(),ntohl);

      if(fTRB3debug) {
        printf("\nReceived packet from %s:%d Data:",
               inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        cout <<  setfill('0') << hex;
        for(vector<UInt_t>::size_type i=0; i<fPacket.size(); i++) {
          if(i % 8 == 0)
            cout << endl;
          cout << setw(8) << fPacket[i] << " ";
        }
        cout << dec << endl;
      }
      DoUnpacking();
      break;
    } else {
      sleep(1);
    }
  }
}

//---------------------------------------------------------------------------
void TDAQ_TRB3::ResetIRQ()
{
  // Dummy reset interrupt
  usleep(10);
}


void TDAQ_TRB3::DoUnpacking()
{
  /* The following unpacking algorithm is an adapted version from
   * https://github.com/neiser/mz-unpacker/blob/a2/THldSubevent.cpp
   *
   * If the comments or variable names refer to "TDC", the implementation of
   * a TDC in the FPGAs of the TRB3 are meant!
   */

  // state machine for TDC data decoding needed
  UInt_t nTrbAddress	= 0;
  size_t nTrbWords	= 0;
  UInt_t nTdcAddress	= 0;
  size_t nTdcWords	= 0;
  Bool_t bFoundCtsPacket = kFALSE;

  // declare i outside loop to check in the end
  // if we processed everything correctly
  vector<UInt_t>::size_type i=0;
  // the "real" trb data words start after 6 HLD header words
  // and are appended by 11 HLD trailing words
  for(i=6;i<fPacket.size()-11;i++){ // loop over all TRB data and decode TDC hits

    UInt_t CurrentDataWord = fPacket[i];
    if(fTRB3debug)
      cout << "i=" <<i<<" Data: "<<setfill('0') << setw(8)<<hex<<CurrentDataWord<<dec<<endl;
    if(nTdcWords==0) {
      // look for DataWord with matching TrbAddress
      nTrbAddress = CurrentDataWord & 0xFFFF;
      nTrbWords	= CurrentDataWord>>16;

      if(nTrbAddress==fAddrCTS) {
        // Check first if it's CTS, because it might contain a TDC
        // except for the integrated TDC and external trigger id
        bFoundCtsPacket = kTRUE;
        if(fTRB3debug)
          cout << "Found CTS readout packet, decoding it (" << nTrbWords << " words)" << endl;

        // the CTS address is also mentioned in the TDC list,
        // so we expect to find some TDC information
        // we also save the external trigger id (if we find one)
        UInt_t nCTSwords = DecodeCTSData(i+1);

        if(nCTSwords==0 || nCTSwords>nTrbWords) {
          if(fTRB3debug)
            cout << "ERROR in decoding CTS word, skipping it." << endl;
          i += nTrbWords;
        }
        else {
          // TDC words of the CTS are at the very end of this data block
          // so we fake at this point to have found a "normal" TDC data block now
          // skip the already parsed CTS words
          i += nCTSwords;
          // but not the appended TDC words
          nTdcWords = nTrbWords-nCTSwords;
          nTrbWords = nTdcWords;
          nTdcAddress = nTrbAddress;
          if(fTRB3debug)
            cout << "TDC in CTS Endpoint found at 0x" << setfill('0') << setw(4)
                 << hex << nTdcAddress << dec << ", Payload " << nTdcWords << endl;

          }


        // skip rest of loop, but we might decode TDC stuff from the CTS now
        continue;
      }
      else if(CheckHubAddress(nTrbAddress)) {
        // we found a interesting hub, we simply do nothing but
        // to go one level deeper in the topology
        // a consistency check could verify if nTrbWords extracted now
        // will fit to the words of the following subsubevent
        // (but it's complicated since you don't know the depth of the topology tree)
        if(fTRB3debug)
          cout << "Known HUB found at 0x" << setfill('0') << setw(4)
               << hex << nTrbAddress << dec << ", Subsubevent size " << nTrbWords << endl;

      }
      else if(CheckTdcAddress(nTrbAddress)) {
        // we recognized it as an TDC endpoint
        nTdcWords	= nTrbWords;
        nTdcAddress = nTrbAddress;
        if(fTRB3debug)
          cout << "TDC Endpoint found at 0x" << setfill('0') << setw(4)
               << hex << nTdcAddress << dec << ", Payload " << nTdcWords << endl;
      }
      else {
        // this doesn't seem to be interesting data, so skip it
        if(fTRB3debug)
          cout << "Found uninteresting data at 0x" << setfill('0') << setw(4)
               << hex << nTrbAddress << dec << " (not HUB, not TDC, not CTS), skipping it ("  << nTrbWords << " words)" << endl;
        i += nTrbWords;
        continue;
      }

    }
    else if(nTdcWords==nTrbWords) {
      // At beginning, we expect a TDC Header
      if(!DecodeTdcHeader(CurrentDataWord)) {
        cerr << "ERROR in UDP Packet: TDC Header "
             << CurrentDataWord << dec << " invalid, skipping payload ("<< nTdcWords << " words)" << endl;
        i += nTdcWords-1;
        nTdcWords = 0;
        continue;
      }
      // successfully parsed Tdc Header
      nTdcWords--;
    }
    else {
      DecodeTdcWord(CurrentDataWord, nTdcAddress);
      nTdcWords--;
      if(nTdcWords==0) {
        if(fTRB3debug)
          cout << "Successfully unpacked TDC event" << endl;
      }
    }

  } // end of loop over all TRB data



  if(i != fPacket.size()-11) {
    cerr << "ERROR in UDP Packet: Skipped too many words " << endl;
  }

  if(!bFoundCtsPacket) {
    cerr << "ERROR in UDP Packet: No CTS packet found " << endl;
  }

}

Bool_t TDAQ_TRB3::CheckHubAddress(const UInt_t& nTrbAddress) {
  return find(fAddrHubs.begin(),fAddrHubs.end(),nTrbAddress) != fAddrHubs.end();
}

Bool_t TDAQ_TRB3::CheckTdcAddress(const UInt_t& nTrbAddress) {
  return fAddrEndpoints.find(nTrbAddress) != fAddrEndpoints.end();
}

#define TDC_HEADER_MARKER  0b001 // |MH> binary definitions work with gcc but not with VC++
#define TDC_EPOCH_MARKER   0b011
#define TDC_DEBUG_MARKER   0b010

Bool_t TDAQ_TRB3::DecodeTdcHeader(UInt_t& DataWord) {

  if(((DataWord>>29) & 0x7) != TDC_HEADER_MARKER) { // check 3 bits reserved for TDC header marker
    return (kFALSE);
  }
  fTdcHeaderRandomBits	= (DataWord>>16) & 0xFF;
  fTdcHeaderErrorBits	= DataWord & 0xFFFF;
  fTdcLastChannelNo = -1; // reset the last channel number...
  if(fTRB3debug){
    cout << "TDC Header word found!" << endl;
    cout << "TDC Error Code is " << hex << fTdcHeaderErrorBits << dec << endl;
    cout << "TDC Random Bits " << hex << fTdcHeaderRandomBits << dec << endl;
  }
  return (kTRUE);
}

Bool_t TDAQ_TRB3::DecodeTdcWord(UInt_t& DataWord, UInt_t& nUserTdcAddress) { // decode TDC data word

  // first check if word is EPOCH or DEBUG
  UInt_t FirstThreeBits =  (DataWord>>29) & 0x7;

  // put the EPOCH number into the SubEvent's member
  if(FirstThreeBits == TDC_EPOCH_MARKER) {
    if(fTRB3debug)
      cout << "Found EPOCH word:  " << hex << DataWord << ", " << nUserTdcAddress << dec << endl;
    // lowest 28bits represent epoch counter
    fTdcEpochCounter = DataWord & 0x0FFFFFFF;
    // this indicates that we found an EPOCH counter
    fTdcLastChannelNo = -2;
    return kFALSE;
  }

  // check for DEBUG, we don't use this info at the moment
  if(FirstThreeBits == TDC_DEBUG_MARKER) {
    if(fTRB3debug)
      cout << "Found DEBUG word:  " << hex << DataWord << ", " << nUserTdcAddress << dec << endl;
    return kFALSE;
  }

  // now we expect a TIMEDATA word...if not, we don't know :)
  if((DataWord>>31) != 1) { // check time data marker i.e. MSB==1
    if(fTRB3debug)
      cout << "Found ??UNKNOWN?? word (maybe spurious header): " << hex << DataWord << ", "
           << nUserTdcAddress << dec  << endl;
    return kFALSE;
  }

  UInt_t nTdcChannelNo	= (DataWord>>22) & 0x7F; // TDC channel number is represented by 7 bits
  // check here if we need to reset the EPOCH counter
  if(fTdcLastChannelNo>=0 && (Int_t)nTdcChannelNo != fTdcLastChannelNo) {
    if(fTRB3debug)
      cout << "Epoch Counter reset since channel has changed" << endl;
    fTdcEpochCounter = 0;
  }

  Bool_t bIsRefChannel	= (nTdcChannelNo==0 || nUserTdcAddress==fAddrCTS) ? kTRUE : kFALSE;
  UInt_t nTdcFineTime		= (DataWord>>12) & 0x3FF; // TDC fine time is represented by 10 bits
  UInt_t nTdcEdge			= (DataWord>>11) & 0x1; // TDC edge indicator: 1->rising edge, 0->falling edge
  UInt_t nTdcCoarseTime	= DataWord & 0x7FF; // TDC coarse time is represented by 11 bits

  if(fTRB3debug)
    cout << "Found TIMEDATA word:  " <<hex << DataWord << dec << ", channel " << nTdcChannelNo
         << " Epoch:" << fTdcEpochCounter  << " last: " << fTdcLastChannelNo << endl;
  fTdcLastChannelNo = (Int_t)nTdcChannelNo;
  return kTRUE;
}

UInt_t TDAQ_TRB3::DecodeCTSData(unsigned i0) {
  // i0 should point the CTS header
  UInt_t header = fPacket[i0];
  // we extract only stuff which is interesting for now...
  UInt_t nInputs = header >> 16 & 0xf;
  UInt_t nTrigChannels = header >> 20 & 0xf;
  UInt_t bIncludeLastIdle = header >> 25 & 0x1;
  UInt_t bIncludeCounters = header >> 26 & 0x1;
  UInt_t bIncludeTimestamp = header >> 27 & 0x1;
  UInt_t nExtTrigFlag = header >> 28 & 0x3;

  // how many words each data needs is documented in trb3docu.pdf
  // don't forget the header, which is one word
  UInt_t nCTSwords = 1 + nInputs*2 + nTrigChannels*2 +
    bIncludeLastIdle*2 + bIncludeCounters*3 + bIncludeTimestamp*1;

  // now, the external trigger module (ETM) is missing
  // i should point to the first ETM word
  unsigned i = i0+nCTSwords;

  if(nExtTrigFlag==0x1) {
    // ETM sends one word, is probably MBS Vulom Recv
    // this is not yet tested
    nCTSwords+=1;
    fCTSExtTrigger = fPacket[i] & 0x00ffffff; // lower 24 bits are trigger number
    fCTSExtTriggerStatus = fPacket[i] & 0xff000000; // upper 8 bits are status/error
  }
  else if(nExtTrigFlag==0x2) {
    // ETM sends four words, is probably a Mainz A2 recv
    nCTSwords+=4;
    fCTSExtTrigger = fPacket[i];
    fCTSExtTriggerStatus = fPacket[i+1];
    // word 3+4 are 0xdeadbeef i.e. not used at the moment...
  }
  else if(nExtTrigFlag==0x3) {
    if(fTRB3debug)
      cout << "ERROR: Unknown ETM found" << endl;
    // return 0, which skips the CTS stuff
    nCTSwords = 0;
  }

  if(fTRB3debug)
    cout << "External Trigger ID " << hex << fCTSExtTrigger << ", Status: " << fCTSExtTriggerStatus <<endl;


  return nCTSwords;
}

