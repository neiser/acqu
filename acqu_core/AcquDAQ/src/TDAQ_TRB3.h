//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TDAQ_TRB3

#ifndef __TDAQ_TRB3_h__
#define __TDAQ_TRB3_h__

#include "TDAQmodule.h"

class TDAQ_TRB3;
#include "TTRB3module.h"
#include "TTRB3TdcHit.h"
#include "TTRB3TdcCalib.h"
#include <map>

#ifndef __CINT__
extern "C" {
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
}
#endif // __CINT__

class TDAQ_TRB3 : public TDAQmodule
{
private:
  Bool_t    fIsIRQEnabled;
  // debugging of TRB3 specific stuff enabled?
  Bool_t fTRB3debug;
  // socket handle to listen for incoming UDP packets from TRB3
  Int_t fSocket;
  // buffer to store the incoming UDP packet
  static const size_t fPacketMaxSize = 65507;
  std::vector<UInt_t> fPacket;
  // sets up the socket
  void InitSocket();
  std::string fListenAddress;
  UInt_t fListenPort;
  // stuff to handle the unpacking of the received UDP packet
  std::map<UInt_t, TTRB3module*> fAddrEndpoints;
  // the hub list is to be filled via config file
  std::vector<UInt_t> fAddrHubs;
  // and the CTS address is also set by config file
  UInt_t fAddrCTS;
  Int_t fTdcLastChannelNo;
  UInt_t fTdcEpochCounter;
  UInt_t fCTSExtTrigger;
  UInt_t fCTSExtTriggerStatus;
  Bool_t bUnpackingOkay;
  void DoUnpacking();
  Bool_t CheckHubAddress(const UInt_t& nTrbAddress);
  TTRB3module* CheckTdcAddress(const UInt_t& nTrbAddress);
  Bool_t DecodeTdcHeader(TTRB3module* currMod, UInt_t& DataWord);
  Bool_t DecodeTdcWord(TTRB3module* currMod, UInt_t& DataWord);
  UInt_t DecodeCTSData(unsigned i0);
  // stuff to handle calibration and event reconstruction from the unpacked data
  TTRB3TdcHit fCTSRefHit;    // reference TDC hit from CTS
  TTRB3TdcCalib fTdcCalib;   // calibration information
protected:

public:
  friend class TTRB3module;
  TDAQ_TRB3( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TDAQ_TRB3();
  void SetConfig( Char_t*, Int_t );         // configure TRB3
  virtual void PostInit();

  virtual void WaitIRQ();
  virtual void ResetIRQ();
  virtual void EnableIRQ() {
    fIsIRQEnabled = kTRUE;
  }
  virtual void DisableIRQ() {
    fIsIRQEnabled = kFALSE;
  }
  Bool_t IsIRQEnabled() {
    return fIsIRQEnabled;
  }

  ClassDef(TDAQ_TRB3,1)
};


#endif
