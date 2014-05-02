//--Author	JRM Annand	19th Nov 2007    For PCI-VMEbus AcquDAQ systems
//--Rev 	JRM Annand
//--Update	JRM Annand
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
//
// TA2LocalSource
// Specify local-node-read, e.g via a PCI-VME link, buffer for 
// AcquRoot analysis, ie make a link to TDAQperiment's TA2RingBuffer and
// make the link-TA2Ringbuffer available to TA2DataServer
// A wrapper for event saving in the RunIRQ procedure of TDAQexperiment
// which allows it to connect directly to the receiving procedure of
// Process/MultiProcess in TA2DataServer via a TA2RingBuffer. 
// TA2DataServer assumes that it connects to a data source via a derivative
// of TDataSource.
//
//---------------------------------------------------------------------------

#ifndef _TA2LocalSource_h_
#define _TA2LocalSource_h_

#include "TA2DataSource.h"

class TDAQexperiment;              

R__EXTERN TDAQexperiment* gDAQ;

class ARFile_t;

class TA2LocalSource : public TA2DataSource
{
 protected:
  Char_t** fFilename;			        // input file name list
 public:
  TA2LocalSource( Char_t*, Int_t, Int_t=EFalse, Int_t=1, Int_t=EFalse );
  virtual ~TA2LocalSource();
  void Process();			        // main data processor
  virtual void Initialise();                    // special for local
  void Start(){}                                // don't start anything
  ClassDef(TA2LocalSource,1)
};

#endif
