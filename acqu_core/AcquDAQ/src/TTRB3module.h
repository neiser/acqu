//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TTRB3module

#ifndef __TTRB3module_h__
#define __TTRB3module_h__

#include "TDAQmodule.h"

class TTRB3module;
#include "TDAQ_TRB3.h"

class TTRB3module : public TDAQmodule
{
private:
  // debugging of TRB3 specific stuff enabled?
  Bool_t fTRB3debug;
  // the address of the TRB endpoint which this module delivers to AcquDAQ
  UInt_t fTRBAddress;
  // pointer so that we can access the unpacked data in ReadIRQ
  TDAQ_TRB3 *fTRBCtrl;
protected:
public:
  TTRB3module( Char_t*, Char_t*, FILE*, Char_t* );
  virtual ~TTRB3module();
  virtual void SetConfig( Char_t*, Int_t );
  virtual void PostInit();
  virtual void ReadIRQ(void**);

  ClassDef(TTRB3module,1)
};

#endif
