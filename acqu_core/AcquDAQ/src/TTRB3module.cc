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

enum { ETRB3_Debug=100, ETRB3_MyOption };

static Map_t kTRB3moduleKeys[] = {
  {"TRB3-Debug:",    ETRB3_Debug},
  {"TRB3-MyOption:", ETRB3_MyOption},
  {NULL,                  -1}
};

//-----------------------------------------------------------------------------
TTRB3module::TTRB3module( Char_t* name, Char_t* file, FILE* log, Char_t* line ):
  TDAQmodule( name, file, log )
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
  fTRB3debug = kFALSE; // can be enabled by .dat file config
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

  case ETRB3_MyOption:
    // do something meaningful with line here
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
void TTRB3module::ReadIRQ( void** outBuffer )
{
  // Write something to output data buffer
  for( Int_t i=0; i<fNChannel; i++ ) {
    // provide a constant value of 512
    UInt_t chan = 512;
    if(fType == EDAQ_ADC) {
      UShort_t adcVal = chan & 0xffff;                 // ADC value
      UShort_t adcIndex = i + fBaseIndex;              // index offset
      //for(Int_t j=0;j<4000; j++)
      ADCStore( outBuffer, adcVal, adcIndex );// store values
    }
    else if( fType == EDAQ_Scaler ) {
      UInt_t scalerVal = chan;
      UInt_t scalerIndex = i + fBaseIndex;
      ScalerStore( outBuffer, scalerVal, scalerIndex );
    }
  }
}


