//                *** AcquDAQ++ <-> Root ***
// DAQ for Sub-Atomic Physics Experiments.
//
// TTRB3module
//

#include "TTRB3module.h"
#include "TDAQexperiment.h"

// make our life a bit more C++'ish
#include <iostream>
#include <iomanip>
using namespace std;

ClassImp(TTRB3module)

enum { ETRB3_Debug=100, ETRB3_TdcCalib };

static Map_t kTRB3moduleKeys[] = {
  {"TRB3-Debug:",    ETRB3_Debug},
  {"TRB3-TdcCalib:", ETRB3_TdcCalib},
  {NULL,                  -1}
};

//-----------------------------------------------------------------------------
TTRB3module::TTRB3module( Char_t* name, Char_t* file, FILE* log, Char_t* line ):
  TDAQmodule( name, file, log ),
  fTRB3debug(kFALSE),
  fTdcHits(),
  fRefHit(),
  fTdcCalib(),
  fTRBError(0)
{
  // Basic initialisation.
  // Read in base address, base readout index and # channels
  //
  Char_t type[32];
  AddCmdList(kTRB3moduleKeys);
  if( line ) {
    if( sscanf( line, "%*s%*s%*s%s%d%d%d%x",type,&fBaseIndex,&fNChannel,&fNBits,&fTRBAddress)
        < 5 ) {
      PrintError( line, "<TRB3-Module setup line parse>", EErrFatal );
    }
  }
  fType = GetModType(type);
}

//-----------------------------------------------------------------------------
TTRB3module::~TTRB3module( )
{
  // Clean up
}

//-----------------------------------------------------------------------------
void TTRB3module::SetConfig( Char_t* line, Int_t key )
{
  // Configuration from file
  switch( key ) {
  case ETRB3_Debug:
    // enable debug output (prints alot to stdout!)
    fTRB3debug = kTRUE;
    break;

  case ETRB3_TdcCalib:
    if(sscanf(line,"%lf%lf%lf", &fTdcCalib.StartBin,
              &fTdcCalib.EndBin, &fTdcCalib.ClockCycle) != 3)
      PrintError(line, "<TTRB3module TdcCalib setup line parse>", EErrFatal);
    fTdcCalib.IsValid = kTRUE;
    break;

  default:
    // default try commands of TDAQmodule
    TDAQmodule::SetConfig(line, key);
    break;
  }
}

//-----------------------------------------------------------------------------
void TTRB3module::PostInit( )
{
  // Post-configuration setup of hardware
  // Write stored data to hardware
  //
  if( fIsInit ) {
    return;
  }
  TDAQmodule::PostInit();
  if(!fCtrlMod->InheritsFrom("TDAQ_TRB3"))
    PrintError("TRB3module", "<TRB3module can only be controlled by TDAQ_TRB3>", EErrFatal );
  fTRBCtrl = (TDAQ_TRB3*)fCtrlMod;
  fTRBCtrl->fAddrEndpoints.insert(make_pair(fTRBAddress,this));
  if(fTRB3debug)
    cout << "TRB3 module init, TRB Address: "<< hex << fTRBAddress << dec << endl;
}

//---------------------------------------------------------------------------
void TTRB3module::ReadIRQ(void** outBuffer)
{
  // do nothing if there are no hits
  size_t nHits = fTdcHits.size();
  if(nHits==0)
    return;

  // select calibration
  TTRB3TdcCalib* calib = fTdcCalib.IsValid ? &fTdcCalib : &(fTRBCtrl->fTdcCalib);

  // calculate ref time (hopefully there is one)
  Double_t refTimeEndpoint = calib->GetCalibratedTime(&fRefHit);
  Double_t refTimeCTS = calib->GetCalibratedTime(&(fTRBCtrl->fCTSRefHit));
  if(fTRB3debug) {
    cout << "Ref Time Phase to CTS: " << refTimeCTS-refTimeEndpoint << endl;
  }
  // actually one needs not only the hit in the CTS, but also its ref hit
  // (similar to peripheral ref time hit). But this was not read out, don't know why...
  // at the moment, refTimeCTS or refTimeEndpoint do not make a big difference
  Double_t refTime = refTimeCTS;

  // very simplistic hit identification
  // cannot handle multihits, but tries its best

  if(fTRB3debug) {
    for(size_t i=0;i<nHits;i++)
      cout << "Hit " << i << ": " << fTdcHits[i] << endl;
  }

  size_t leadingIdx = 0;
  size_t trailingIdx = 1;
  Int_t lastLeadingChannel = -1;
  while(leadingIdx<nHits-1 && trailingIdx<nHits) {
    // find the next leading edge hit (has a odd channel number)
    // note that for a correct leading hit there must be another one
    // also ensure that we have a change in the channel number,
    // this makes this algorithm robust against multi-hits
    TTRB3TdcHit* leadingHit;
    do {
      leadingHit = &(fTdcHits[leadingIdx]);
      leadingIdx++;
    }
    while(leadingIdx<nHits-1 && (leadingHit->Channel % 2 == 0 ||
          (lastLeadingChannel>=0 && leadingHit->Channel == lastLeadingChannel)));
    lastLeadingChannel = leadingHit->Channel;
    // best candidate for corresponding trailing edge
    // is next hit (note that leading has already been incremented)
    trailingIdx = leadingIdx;
    TTRB3TdcHit* trailingHit;
    do {
      trailingHit = &(fTdcHits[trailingIdx]);
      trailingIdx++;
    }
    while(trailingIdx<nHits && trailingHit->Channel % 2 == 1);

    if(fTRB3debug) {
      cout << "Found Leading  : " << *leadingHit << endl
           << "Found Trailing : " << *trailingHit << endl;
    }
    // check if this event makes sense
    if(leadingHit->Channel+1 != trailingHit->Channel) {
      if(fTRB3debug)
        cout << "Found non-matching Leading/Trailing event, skipping." << endl;
      continue;
    }
    Double_t leadingEdge = calib->GetCalibratedTime(leadingHit);
    Double_t trailingEdge = calib->GetCalibratedTime(trailingHit);
    Double_t timeOverThreshold = trailingEdge - leadingEdge;
    // the leading Edge should always have a lower value than the trailing edge
    // i.e. timeOverThreshold should be >0, if not, the input to the TDC was inverted
    // we correct that here, because only the "true" leading edge has a
    // precise time information!
    if(timeOverThreshold<0) {
      if(fTRB3debug)
        cout << "Setting the 'trailing' edge as the correct leading edge" << endl;
      timeOverThreshold = -timeOverThreshold;
      leadingEdge = trailingEdge;
      // trailing Edge is not needed anymore (information in ToT)
    }

    // correct by reference time
    leadingEdge = leadingEdge - refTime;
    // simple channel mapping, could be configured some day?
    UShort_t adcIndex = (leadingHit->Channel-1)/2 + fBaseIndex;

    // NOTE: Although ADCStore accepts an UShort_t as data, AcquRoot in MultiADC_t.h
    // typecasts this silently to a Short_t and uses the sign flag as an "undefined" ADC value

    // at least for the TDCs we send it as 0.1171 ns ticks (as the F1 CATCH TDCs do)
    // and center the value by an offset
    ADCStore(outBuffer, (UShort_t)(10000+leadingEdge/0.1171d), adcIndex);

    // for the QDC value (=charge information) we have no clue at the moment how to convert it
    ADCStore(outBuffer, (UShort_t)((10.0d+timeOverThreshold)*100.0d), adcIndex);

    if(fTRB3debug)
      cout << "EVENT: Idx " << adcIndex  << ", Edge " << leadingEdge << ", ToT " << timeOverThreshold << endl;
  }

  // clear the hits, so the next UDP unpacking can add new hits
  fTdcHits.clear();


  // Write something to output data buffer
  /*for( Int_t i=0; i<fNChannel; i++ ) {
    // provide a constant value of 512
    UInt_t chan = 512;
    if(fType == EDAQ_ADC) {
      UShort_t adcVal = chan & 0xffff;                 // ADC value
      UShort_t adcIndex = i + fBaseIndex;              // index offset
      ADCStore( outBuffer, adcVal, adcIndex );// store values
    }
    else if( fType == EDAQ_Scaler ) {
      UInt_t scalerVal = chan;
      UInt_t scalerIndex = i + fBaseIndex;
      ScalerStore( outBuffer, scalerVal, scalerIndex );
    }
  }*/
}


