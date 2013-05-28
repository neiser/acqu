//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TTRB3module

#ifndef __TTRB3module_h__
#define __TTRB3module_h__

#include "TDAQmodule.h"

class TTRB3module;
#include "TDAQ_TRB3.h"
#include "TTRB3TdcHit.h"
#include "TTRB3TdcCalib.h"

class TTRB3module : public TDAQmodule
{
private:
  // debugging of TRB3 specific stuff enabled?
  Bool_t fTRB3debug;
  // the address of the TRB endpoint which this module delivers to AcquDAQ
  UInt_t fTRBAddress;
  // pointer so that we can access the calibration/reference time (if needed)
  // and introduce ourselve to the controller
  TDAQ_TRB3 *fTRBCtrl;
  // hit information indexed by TDC channel
  // we assume that there's only one hit per channel per event!
  std::vector<TTRB3TdcHit> fTdcHits;
  // reference hit is stored extra
  TTRB3TdcHit fRefHit;
  // store calibration valid only for this module
  TTRB3TdcCalib fTdcCalib;
  // if something went wrong with the unpacking,
  // here are some details about it
  UInt_t fTRBError;
protected:
public:
  friend class TDAQ_TRB3;
  TTRB3module( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TTRB3module();
  virtual void SetConfig( Char_t*, Int_t );
  virtual void PostInit();
  virtual void ReadIRQ(void**);

  ClassDef(TTRB3module,1)
};

#endif
