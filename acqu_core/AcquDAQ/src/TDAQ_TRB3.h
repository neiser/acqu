//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TDAQ_TRB3

#ifndef __TDAQ_TRB3_h__
#define __TDAQ_TRB3_h__

#include "TDAQmodule.h"

class TDAQ_TRB3;
#include "TTRB3module.h"
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
  // debugging of TRB3 specific stuff enabled?
  Bool_t fTRB3debug;
  // socket handle to listen for incoming UDP packets from TRB3
  Int_t fSocket;
  // buffer to store the incoming UDP packet
  static const size_t fPacketMaxSize = 65507;
  std::vector<UInt_t> fPacket;
  // sets up the socket
  void InitSocket();
  // stuff to handle the unpacking of the received UDP packet
  std::vector<UInt_t> fAddrHubs;
  UInt_t fAddrCTS;
  UInt_t fTdcHeaderRandomBits; // random code, generated individually for each event
  UInt_t fTdcHeaderErrorBits; // TDC errors are indicated here (0 in case of no errors)
  Int_t fTdcLastChannelNo;
  UInt_t fTdcEpochCounter;
  UInt_t fCTSExtTrigger;
  UInt_t fCTSExtTriggerStatus;
  void DoUnpacking();
  Bool_t CheckHubAddress(const UInt_t& nTrbAddress);
  Bool_t CheckTdcAddress(const UInt_t& nTrbAddress);
  Bool_t DecodeTdcHeader(UInt_t& DataWord);
  Bool_t DecodeTdcWord(UInt_t& DataWord, UInt_t& nUserTdcAddress);
  UInt_t DecodeCTSData(unsigned i0);
  // stuff to handle calibration and event reconstruction from the unpacked data
  Double_t fCTSExtTriggerTime;    // the time reconstructed

protected:
  Bool_t    fIsIRQEnabled;
  // fAddrEndpoints is protected so that TTRB3module
  // can add itself to this list
  std::map<UInt_t, TTRB3module*> fAddrEndpoints;
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
