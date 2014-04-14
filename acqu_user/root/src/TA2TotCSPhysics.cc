// SVN Info: $Id: TA2TotCSPhysics.cc 116 2013-02-06 02:32:22Z mushkar $
#include <iostream>
#include <set>

// AcquROOT includes
#include "TAcquRoot.h"
#include "TA2Analysis.h"
#include "TA2Tagger.h"
#include "TA2Taps.h"
#include "TA2CB.h"
#include "TAcquFile.h"

// My includes
#include "TA2TotCSPhysics.h"

//For software trigger keyword parsing
enum { ETShift = 10000, EMCData, ECheckCher, ETPmtCB, ETRndCB, ETPmtTAPS, ETRndTAPS,
       ETriggerCB, ETriggerTAPS};

static const Map_t kTotCSCfgKeys[] =
{
  {"TriggerCB:",	ETriggerCB},
  {"TriggerTAPS:",	ETriggerTAPS},
  {"TShift:",		ETShift},
  {"MCData:",		EMCData},
  {"CheckCher:",	ECheckCher},
  {"TPmtCB:",		ETPmtCB},
  {"TRndCB:",		ETRndCB},
  {"TPmtTAPS:",		ETPmtTAPS},
  {"TRndTAPS:",		ETRndTAPS},
  {NULL,		-1}
};

static const UInt_t kRing[2][12] = {{0,64,128,192,256,320}, {1,2,65,66,129,130,193,194,257,258,321,322}}; // the TAPS elements of inner ring 1, 2
static const UInt_t kRingSize[2] = {6,12}; //the TAPS size of the inner rings 1, 2

ClassImp(TA2TotCSPhysics)

//_____________________________________________________________________________________
TA2TotCSPhysics::TA2TotCSPhysics( const char* name, TA2Analysis* analysis ) : TA2Physics( name, analysis )
{
  // Initialise Physics variables here
  // Default null pointers, zeroed variables
      
  //Enables keyword recognition for SetConfig()
  AddCmdList(kTotCSCfgKeys);
  
  // Trigger
  fTestTriggerSW     = kFALSE;    // 1 -test software trigger, 0 - do not
  fTestTriggerSWCB   = kFALSE;    // 1 -test software trigger, 0 - do not
  fTestTriggerSWTAPS = kFALSE;    // 1 -test software trigger, 0 - do not
  //CB
  fESumThresCB = 0.;      // Threshold for energy sum in CB
  fMultThresCB = 0;      // Threshold for multiplicity in CB
  fPrescaleCB  = 0;       // Prescale factor for CB triggers
  for(Int_t i=0; i<720; ++i) {
    fGainsNaI[i] = 1.;    // Relative calibration of NaI crystals (used in energy sum calculation)
    fThresNaI[i] = 0.;    // Discriminator thresholds for each single CB NaI channel
  }
  // TAPS
  fMultThresTAPS = 0;    // Threshold for multiplicity in TAPS
  fPrescaleTAPS  = 0;     // Prescale factor for TAPS triggers
  fRingsTAPS     = 0;     //TAPS rings not contributing to trigger
  for(Int_t i=0; i<510; ++i) fThresBaF2[i] = 0.;   // Discriminator thresholds for each single TAPS BaF2 channel
  
  // Tagger
  fTAGG = NULL;
  fFPD  = NULL;
  //
  fCB   = NULL;
  fTAPS = NULL;
  //
  fIsSim = kFALSE;
  fCheckCher = kFALSE;
  fIsMultihitOn = kFALSE;
  //
  fTShiftCB = fTShiftTAPS = 0.;
  fTCherMin = fTCherMax = 0.;
  //
  fTPmtCBMin = -1000.;
  fTPmtCBMax = 1000.;
  fTRndCBMin[0] = fTRndCBMin[1] = 0.;
  fTRndCBMax[0] = fTRndCBMax[1] = 0.;
  fTPmtTAPSMin = -1000.;
  fTPmtTAPSMax = 1000.;
  fTRndTAPSMin[0] = fTRndTAPSMin[1] = 0.;
  fTRndTAPSMax[0] = fTRndTAPSMax[1] = 0.;
  //
  fDTCBTaggerAll = fDTTAPSTaggerAll = fDTCBTagger = fDTCBGoodTagger = fDTCBTAPS = fDTTAPSTagger = NULL;
  fHitsTaggerPmtCB = fHitsTaggerPmtCBGood = fHitsTaggerPmtTAPS = fHitsTaggerPmt = NULL;
  fHitsTaggerRndCB = fHitsTaggerRndCBGood = fHitsTaggerRndTAPS = fHitsTaggerRnd = NULL;
}

//_____________________________________________________________________________________
TA2TotCSPhysics::~TA2TotCSPhysics()
{
// Free up allocated memory...after checking its allocated
// detector and cuts lists

}

//_____________________________________________________________________________________
void TA2TotCSPhysics::SetConfig(Char_t* line, Int_t key)
{
  //  Reading the setup file
  
  FILE* file = NULL;
  Int_t dummy;
  Char_t nameFileThres[255], nameFileGain[255];

  switch (key)
  {
    case ETriggerCB:
      if(sscanf(line, "%lf %u %u %s %s\n", &fESumThresCB, &fMultThresCB, &fPrescaleCB, nameFileThres, nameFileGain)==5) {
	std::cout << "Software CB trigger (Esum>=" << fESumThresCB << "MeV && M" << fMultThresCB << "+)/" << fPrescaleCB << " is enabled." << std::endl;
	file = fopen(nameFileGain, "r");
	if(file) {
          for(Int_t t=0; t<720; t++) fscanf(file, "%lf", &fGainsNaI[t]);
          fclose(file);
	} else PrintError( "", "<File with CB gains was not found>", EErrFatal );
	file = fopen(nameFileThres, "r");
	if(file) {
          for(Int_t t=0; t<720; t++) fscanf(file, "%d %lf", &dummy, &fThresNaI[t]);
          fclose(file);
	} else PrintError( "", "<File with CB thresholds was not found>", EErrFatal );
	fTestTriggerSW = kTRUE;
	fTestTriggerSWCB = kTRUE;
      } else PrintError( line, "<Problem with software TriggerCB!>" );
      break;
    case ETriggerTAPS:
      if(sscanf(line, "%u %u %u %s\n", &fMultThresTAPS, &fPrescaleTAPS, &fRingsTAPS, nameFileThres)==4) {
        std::cout << "Software TAPS trigger M" << fMultThresTAPS << "+/" << fPrescaleTAPS << " is enabled.";
        if(fRingsTAPS>0) std::cout << " Disabled TAPS rings: " << fRingsTAPS << std::endl;
        else             std::cout << " All TAPS rings are enabled." << std::endl;
	file = fopen(nameFileThres, "r");
	if(file) {
	  for(Int_t t=0; t<510; t++) fscanf(file, "%d %lf", &dummy, &fThresBaF2[t]);
	  fclose(file);
	} else PrintError( "", "<File with TAPS thresholds was not found>", EErrFatal );
	fTestTriggerSW = kTRUE;
	fTestTriggerSWTAPS = kTRUE;
      } else PrintError( line, "<Problem with software TriggerTAPS!>" );
      break;
    //
    case ETShift:
      if(sscanf(line, "%lf %lf\n", &fTShiftCB, &fTShiftTAPS)!=2) PrintError( line, "<CB-Tagger and TAPS-Tagger time shift>" );
      break;
    case EMCData:
      fIsSim = kTRUE;
      break;
    case ECheckCher:
      if(sscanf(line, "%lf %lf\n", &fTCherMin, &fTCherMax)!=2) PrintError( line, "<Cherenkov ADC1411M0 Tmax and Tmin>" );
      else fCheckCher = kTRUE;
      break;
    case ETPmtCB:
      if(sscanf(line, "%lf %lf\n", &fTPmtCBMin, &fTPmtCBMax)!=2) PrintError( line, "<CB prompt time window>" );
      break;
    case ETRndCB:
      if(sscanf(line, "%lf %lf %lf %lf\n", &fTRndCBMin[0], &fTRndCBMax[0], &fTRndCBMin[1], &fTRndCBMax[1])!=4) PrintError( line, "<CB random time windows>" );
      break;
    case ETPmtTAPS:
      if(sscanf(line, "%lf %lf\n", &fTPmtTAPSMin, &fTPmtTAPSMax)!=2) PrintError( line, "<TAPS prompt time window>" );
      break;
    case ETRndTAPS:
      if(sscanf(line, "%lf %lf %lf %lf\n", &fTRndTAPSMin[0], &fTRndTAPSMax[0], &fTRndTAPSMin[1], &fTRndTAPSMax[1])!=4) PrintError( line, "<TAPS random time windows>" );
      break;
    default:
      //Call default SetConfig()
      TA2Physics::SetConfig(line, key);
      break;
  }
}

//_____________________________________________________________________________________
void TA2TotCSPhysics::PostInit()
{
  // Initialise arrays to contain 4 momenta and plotable scaler variables
  // Missing mass, missing energy, cm momentum, energies, angles
  // Initialisation will abort if CB or Tagger not initialised
  // TAPS is optional
  
  // Tagger
  fTAGG = (TA2Tagger*)((TA2Analysis*)fParent)->GetChild("TAGG");
  if(!fTAGG) PrintError("","<No Tagger class found in annalysis>",EErrFatal);
  else {
    // FPD
    fFPD = (TA2Ladder*)((TA2Analysis*)fParent)->GetGrandChild("FPD");
    if(!fFPD) PrintError("","<No Ladder class found in annalysis>",EErrFatal);
  }

  // CB
  fCB = (TA2CB*)((TA2Analysis*)fParent)->GetChild("CB");
  if(!fCB) PrintError("","<No CB class found in annalysis>",EErrFatal);
  else {
    // NaI
    fNaI = (TA2CalArray*)((TA2Analysis*)fParent)->GetGrandChild("NaI");
    if(!fNaI) PrintError("","<No NaI class found in annalysis>",EErrFatal);
  }

  // TAPS
  fTAPS = (TA2Taps*)((TA2Analysis*)fParent)->GetChild("TAPS");
  if(!fTAPS) PrintError("","<No TAPS class found in annalysis>",EErrFatal);
  else {
    // BaF2
    fBaF2 = (TA2TAPS_BaF2*)((TA2Analysis*)fParent)->GetGrandChild("BaF2");
    if(!fBaF2) PrintError("","<No BaF2 class found in annalysis>",EErrFatal);
  }
  
  //
  if(fFPD->GetNMultihit()>0) fIsMultihitOn = kTRUE;
  Int_t nMaxTAGG = (fIsMultihitOn) ? fTAGG->GetMaxParticle()*fFPD->GetNMultihit() : fTAGG->GetMaxParticle();
  fFPDTimeOR  = new Double_t[nMaxTAGG+1];
  fFPDHits    = new Int_t[nMaxTAGG+1];
  fFPDEg      = new Double_t[nMaxTAGG+1];
  fFPDHitsPmt = new Int_t[nMaxTAGG+1];
  fFPDHitsRnd = new Int_t[nMaxTAGG+1];
  //
  Int_t nMaxCB   = fCB ? fCB->GetMaxParticle() : 0;
  Int_t nMaxTAPS = fTAPS ? fTAPS->GetMaxParticle() : 0;
  fDTCBTagger     = new Double_t[nMaxTAGG+1];
  fDTCBTaggerAll  = new Double_t[nMaxTAGG*nMaxCB+1];
  fDTCBGoodTagger = new Double_t[nMaxTAGG+1];
  fDTCBTAPS       = new Double_t[nMaxCB*nMaxTAPS+1];
  fDTTAPSTagger   = new Double_t[nMaxTAGG+1];
  fDTTAPSTaggerAll= new Double_t[nMaxTAGG*nMaxTAPS+1];
  fHitsTaggerPmtCB     = new Int_t[nMaxTAGG+1];
  fHitsTaggerPmtCBGood = new Int_t[nMaxTAGG+1];
  fHitsTaggerPmtTAPS   = new Int_t[nMaxTAGG+1];
  fHitsTaggerPmt       = new Int_t[nMaxTAGG+1];
  fHitsTaggerPmtGood   = new Int_t[nMaxTAGG+1];
  fHitsTaggerRndCB     = new Int_t[nMaxTAGG+1];
  fHitsTaggerRndCBGood = new Int_t[nMaxTAGG+1];
  fHitsTaggerRndTAPS   = new Int_t[nMaxTAGG+1];
  fHitsTaggerRnd       = new Int_t[nMaxTAGG+1];
  fHitsTaggerRndGood   = new Int_t[nMaxTAGG+1];
  
  // Default physics initialisation
  TA2Physics::PostInit();
}

//_____________________________________________________________________________________
void TA2TotCSPhysics::LoadVariable()
{

  // Input name - variable pointer associations for any subsequent
  // cut or histogram setup
  // LoadVariable( "name", pointer-to-variable, type-spec );
  // NB scaler variable pointers need the preceeding &
  //    array variable pointers do not.
  // type-spec ED prefix for a Double_t variable
  //           EI prefix for an Int_t variable
  // type-spec SingleX for a single-valued variable
  //           MultiX  for a multi-valued variable

  TA2Physics::LoadVariable();
  
  // Trigger
  TA2DataManager::LoadVariable("HasTriggerHW",		&fHasTriggerHW,		 EISingleX);
  TA2DataManager::LoadVariable("HasTriggerSW",		&fHasTriggerSW,		 EISingleX);
  TA2DataManager::LoadVariable("ESumCB",		&fESumCB,		 EDSingleX);
  TA2DataManager::LoadVariable("EDiscCB",		 fEDiscCB,		 EDMultiX);
  TA2DataManager::LoadVariable("MultCB",		&fMultCB,		 EISingleX);
  TA2DataManager::LoadVariable("MultTAPS",		&fMultTAPS,		 EISingleX);
  // Beam polarization
  TA2DataManager::LoadVariable("BeamPol",	 	&fBeamPol,		 EISingleX);
  TA2DataManager::LoadVariable("BeamPol10",	 	&fBeamPol10,		 EISingleX);
  TA2DataManager::LoadVariable("BeamPol20",	 	&fBeamPol20,		 EISingleX);
  //
  TA2DataManager::LoadVariable("CBTrig",	 	&fCBTrig,		 EISingleX);
  TA2DataManager::LoadVariable("TAPSTrig",	 	&fTAPSTrig,		 EISingleX);
  TA2DataManager::LoadVariable("NoneTrig",	 	&fNoneTrig,		 EISingleX);
  TA2DataManager::LoadVariable("CBTrigPol10",	 	&fCBTrigPol10,		 EISingleX);
  TA2DataManager::LoadVariable("TAPSTrigPol10",	 	&fTAPSTrigPol10,	 EISingleX);
  TA2DataManager::LoadVariable("NoneTrigPol10",	 	&fNoneTrigPol10,	 EISingleX);
  TA2DataManager::LoadVariable("CBTrigPol20",	 	&fCBTrigPol20,		 EISingleX);
  TA2DataManager::LoadVariable("TAPSTrigPol20",	 	&fTAPSTrigPol20,	 EISingleX);
  TA2DataManager::LoadVariable("NoneTrigPol20",	 	&fNoneTrigPol20,	 EISingleX);
  // 
  TA2DataManager::LoadVariable("CBSumTrig",	 	&fCBSumTrig,		 EISingleX);
  TA2DataManager::LoadVariable("TAPSOrTrig",	 	&fTAPSOrTrig,		 EISingleX);
  TA2DataManager::LoadVariable("NoTrig",	 	&fNoTrig,		 EISingleX);
  TA2DataManager::LoadVariable("CBSumTrigPol10", 	&fCBSumTrigPol10,	 EISingleX);
  TA2DataManager::LoadVariable("TAPSOrTrigPol10",	&fTAPSOrTrigPol10,	 EISingleX);
  TA2DataManager::LoadVariable("NoTrigPol10",	 	&fNoTrigPol10,		 EISingleX);
  TA2DataManager::LoadVariable("CBSumTrigPol20", 	&fCBSumTrigPol20,	 EISingleX);
  TA2DataManager::LoadVariable("TAPSOrTrigPol20",	&fTAPSOrTrigPol20,	 EISingleX);
  TA2DataManager::LoadVariable("NoTrigPol20",	 	&fNoTrigPol20,		 EISingleX);
  //   
  TA2DataManager::LoadVariable("CBGood", 	 	&fCBGood,		 EISingleX);
  TA2DataManager::LoadVariable("CBGoodPol10",	 	&fCBGoodPol10,		 EISingleX);
  TA2DataManager::LoadVariable("CBGoodPol20",	 	&fCBGoodPol20,		 EISingleX);
  TA2DataManager::LoadVariable("CBBgd", 	 	&fCBBgd,		 EISingleX);
  TA2DataManager::LoadVariable("CBBgdPol10",	 	&fCBBgdPol10,		 EISingleX);
  TA2DataManager::LoadVariable("CBBgdPol20",	 	&fCBBgdPol20,		 EISingleX);
  //
  TA2DataManager::LoadVariable("TAPSGood", 	 	&fTAPSGood,		 EISingleX);
  TA2DataManager::LoadVariable("TAPSGoodPol10",	 	&fTAPSGoodPol10,	 EISingleX);
  TA2DataManager::LoadVariable("TAPSGoodPol20",	 	&fTAPSGoodPol20,	 EISingleX);
  TA2DataManager::LoadVariable("TAPSBgd", 	 	&fTAPSBgd,		 EISingleX);
  TA2DataManager::LoadVariable("TAPSBgdPol10",	 	&fTAPSBgdPol10,		 EISingleX);
  TA2DataManager::LoadVariable("TAPSBgdPol20",	 	&fTAPSBgdPol20,		 EISingleX);
  //
  TA2DataManager::LoadVariable("Good", 	 	 	&fGood,		 	 EISingleX);
  TA2DataManager::LoadVariable("GoodPol10",	 	&fGoodPol10,		 EISingleX);
  TA2DataManager::LoadVariable("GoodPol20",	 	&fGoodPol20,		 EISingleX);
  TA2DataManager::LoadVariable("Bgd", 		 	&fBgd,			 EISingleX);
  TA2DataManager::LoadVariable("BgdPol10",	 	&fBgdPol10,		 EISingleX);
  TA2DataManager::LoadVariable("BgdPol20",	 	&fBgdPol20,		 EISingleX);
  // 
  TA2DataManager::LoadVariable("FPDNHits",		&fFPDNHits,		 EISingleX);
  TA2DataManager::LoadVariable("FPDHits",      	 	 fFPDHits,		 EIMultiX);
  TA2DataManager::LoadVariable("FPDEg",      	 	 fFPDEg,		 EDMultiX);
  TA2DataManager::LoadVariable("FPDNHitsPmt",		&fFPDNHitsPmt,		 EISingleX);
  TA2DataManager::LoadVariable("FPDHitsPmt",      	 fFPDHitsPmt,		 EIMultiX);
  TA2DataManager::LoadVariable("FPDNHitsRnd",		&fFPDNHitsRnd,		 EISingleX);
  TA2DataManager::LoadVariable("FPDHitsRnd",      	 fFPDHitsRnd,		 EIMultiX);
  TA2DataManager::LoadVariable("FPDTimeOR",	  	 fFPDTimeOR,		 EDMultiX);
  //
  TA2DataManager::LoadVariable("DTCBTagger",	  	 fDTCBTagger,		 EDMultiX);
  TA2DataManager::LoadVariable("DTCBTaggerAll",	  	 fDTCBTaggerAll,	 EDMultiX);
  TA2DataManager::LoadVariable("DTCBGoodTagger",  	 fDTCBGoodTagger,	 EDMultiX);
  TA2DataManager::LoadVariable("DTCBTAPS",	  	 fDTCBTAPS,		 EDMultiX);
  TA2DataManager::LoadVariable("DTTAPSTagger",    	 fDTTAPSTagger,	 	 EDMultiX);
  TA2DataManager::LoadVariable("DTTAPSTaggerAll", 	 fDTTAPSTaggerAll,	 EDMultiX);
  //
  TA2DataManager::LoadVariable("FPDHitsPmtCB",		 fHitsTaggerPmtCB,	 EIMultiX);
  TA2DataManager::LoadVariable("FPDHitsPmtCBGood",	 fHitsTaggerPmtCBGood,	 EIMultiX);
  TA2DataManager::LoadVariable("FPDHitsPmtTAPS",	 fHitsTaggerPmtTAPS,	 EIMultiX);
  TA2DataManager::LoadVariable("FPDHitsPmtTot",		 fHitsTaggerPmt,	 EIMultiX);
  TA2DataManager::LoadVariable("FPDHitsPmtTotGood",	 fHitsTaggerPmtGood,	 EIMultiX);
  TA2DataManager::LoadVariable("FPDHitsRndCB",   	 fHitsTaggerRndCB,	 EIMultiX);
  TA2DataManager::LoadVariable("FPDHitsRndCBGood",	 fHitsTaggerRndCBGood,	 EIMultiX);
  TA2DataManager::LoadVariable("FPDHitsRndTAPS",	 fHitsTaggerRndTAPS,	 EIMultiX);
  TA2DataManager::LoadVariable("FPDHitsRndTot",		 fHitsTaggerRnd,	 EIMultiX);
  TA2DataManager::LoadVariable("FPDHitsRndTotGood",	 fHitsTaggerRndGood,	 EIMultiX);
  //
  TA2DataManager::LoadVariable("NPmtCB",		&fNPmtCB,		 EISingleX);
  TA2DataManager::LoadVariable("NPmtCBGood",		&fNPmtCBGood,		 EISingleX);
  TA2DataManager::LoadVariable("NPmtTAPS",		&fNPmtTAPS,		 EISingleX);
  TA2DataManager::LoadVariable("NPmtTot",		&fNPmt,			 EISingleX);
  TA2DataManager::LoadVariable("NPmtTot0",		&fNPmt,			 EISingleX); // it's needed for appling as a cut
  TA2DataManager::LoadVariable("NPmtTotGood",		&fNPmtGood,		 EISingleX);
  TA2DataManager::LoadVariable("NRndCB",   		&fNRndCB,		 EISingleX);
  TA2DataManager::LoadVariable("NRndCBGood",    	&fNRndCBGood,		 EISingleX);
  TA2DataManager::LoadVariable("NRndTAPS",		&fNRndTAPS,		 EISingleX);
  TA2DataManager::LoadVariable("NRndTot",		&fNRnd,			 EISingleX);
  TA2DataManager::LoadVariable("NRndTotGood",		&fNRndGood,		 EISingleX);
  //
  TA2DataManager::LoadVariable("NTaggCB",		&fNTaggCB,		 EISingleX);
  TA2DataManager::LoadVariable("NTaggCBGood",		&fNTaggCBGood,		 EISingleX);
  TA2DataManager::LoadVariable("NCBTAPS",   		&fNCBTAPS,		 EISingleX);
  TA2DataManager::LoadVariable("NTaggTAPS",		&fNTaggTAPS,		 EISingleX);
  //
  TA2DataManager::LoadVariable("ADC1411M0",		&fADC1411M0,		 EISingleX);
  TA2DataManager::LoadVariable("CherCoinc",		&fHasCherCoinc,		 EISingleX);
  // Sim
  TA2DataManager::LoadVariable("Theta1",	 	&fTheta[0],		 EDSingleX);
  TA2DataManager::LoadVariable("Phi1",    	 	&fPhi[0],		 EDSingleX);
  TA2DataManager::LoadVariable("Theta2",	 	&fTheta[1],		 EDSingleX);
  TA2DataManager::LoadVariable("Phi2",    	 	&fPhi[1],		 EDSingleX);
  TA2DataManager::LoadVariable("Eg",    	 	&fEg,		         EDSingleX);
}

//_____________________________________________________________________________________
void TA2TotCSPhysics::Reconstruct()
{
  // General reconstruction of reaction kinematics in Mainz tagged-photon
  // meson production experiments.
  // Use 4-momenta and PDG-index information from apparati to reconstruct
  // reaction kinematics. The PDG index (and 4-momentum) assigned by the
  // apparatus is not considered binding, e.g. in cases where n/gamma
  // discrimination by an apparatus is not possible, in which case it
  // defaults to kGamma. The method TA2ParticleID->SetMassP4( *p4, ipdg )
  // may be used to reset the rest-mass of an existing 4 momentum *p4 to that
  // corresponding to PDG index ipdg.
  // This one deals with pion and eta photoproduction on the nucleon.
  
  ResetEvent();
  
  // Evaluate trigger
  if(!fIsSim)        fHasTriggerHW = (TriggerProcessHW() && !gAR->IsScalerRead());  // Collect hardware trigger information (if available)
  if(fTestTriggerSW) fHasTriggerSW = (TriggerProcessSW() && !gAR->IsScalerRead());  // Test trigger conditions in software (if requested)
  
  // Simulated observables reconstruction
  if(fIsSim) {
    fEg = *((Float_t*)(fEvent[EI_beam])+3)*1000.;
    Float_t *dircos1 = (Float_t*)(fEvent[EI_dircos]);
    Float_t *dircos2 = (Float_t*)(fEvent[EI_dircos])+3;
    fTheta[0] = TMath::ACos(*(dircos1+2))*TMath::RadToDeg();
    fTheta[1] = TMath::ACos(*(dircos2+2))*TMath::RadToDeg();
    fPhi[0]   = TMath::ATan2(*(dircos1),*(dircos1+1))*TMath::RadToDeg();
    fPhi[1]   = TMath::ATan2(*(dircos2),*(dircos2+1))*TMath::RadToDeg();
  }
  
  // Cherenkov TDC
  if(!fIsSim) {
    fADC1411M0 = fMulti[1411]->GetHit(0);
    if(fCheckCher) fHasCherCoinc = HasCherCoinc(fADC1411M0);
  }
  
  // Beam polarization
  fBeamPol = (Int_t)fADC[6]&kBeamPolConst;
  if(fBeamPol==10) fBeamPol10 = 1;
  if(fBeamPol== 4) fBeamPol20 = 1;
  
  // FPD hits
  Int_t nFPDHits = (fIsSim) ? 1 : fFPD->GetNhits();
  if(!fIsMultihitOn||fIsSim) {
    for(Int_t i=0; i<nFPDHits; ++i) {
      fFPDTimeOR[fFPDNHits] = fFPD->GetTimeOR()[i];
      fFPDHits[fFPDNHits]   = fFPD->GetHits()[i];
      fFPDEg[fFPDNHits] = fTAGG->GetBeamEnergy() - fFPD->GetECalibration()[fFPDHits[fFPDNHits]];
      if(IsPmtTrig(fFPDTimeOR[fFPDNHits])) fFPDHitsPmt[fFPDNHitsPmt++] = fFPDHits[fFPDNHits];
      if(IsRndTrig(fFPDTimeOR[fFPDNHits])) fFPDHitsRnd[fFPDNHitsRnd++] = fFPDHits[fFPDNHits];
      ++fFPDNHits;
    }
  } else {
    // FPD hits for all multiple hits
    for(UInt_t m=0; m<fFPD->GetNMultihit(); ++m) {
      for(UInt_t i=0; i<fFPD->GetNhitsM(m); ++i) {
	fFPDTimeOR[fFPDNHits] = fFPD->GetTimeORM(m)[i];
	fFPDHits[fFPDNHits]   = fFPD->GetHitsM(m)[i];
	fFPDEg[fFPDNHits] = fTAGG->GetBeamEnergy() - fFPD->GetECalibration()[fFPDHits[fFPDNHits]];
	if(fFPD->GetECalibration()[fFPDHits[fFPDNHits]] == 0) std::cout << "0" << std::endl;
	if(IsPmtTrig(fFPDTimeOR[fFPDNHits])) fFPDHitsPmt[fFPDNHitsPmt++] = fFPDHits[fFPDNHits];
	if(IsRndTrig(fFPDTimeOR[fFPDNHits])) fFPDHitsRnd[fFPDNHitsRnd++] = fFPDHits[fFPDNHits];
	++fFPDNHits;
      }
    }
  }
  
  //////////////////////////////////////////////////////////
  // Trigger selection (using NParticles)
  Int_t nCB   = 0;
  Int_t nTAPS = 0;
  if(fCB)   nCB   = fCB->GetNParticle();
  if(fTAPS) nTAPS = fTAPS->GetNParticle();
  if(nCB>=1)        fCBTrig   = 1;
  else if(nTAPS>=1) fTAPSTrig = 1;
  else              fNoneTrig = 1;
  
  fCBTrigPol10   = fCBTrig   && fBeamPol10;
  fCBTrigPol20   = fCBTrig   && fBeamPol20;
  fTAPSTrigPol10 = fTAPSTrig && fBeamPol10;
  fTAPSTrigPol20 = fTAPSTrig && fBeamPol20;
  fNoneTrigPol10 = fNoneTrig && fBeamPol10;
  fNoneTrigPol20 = fNoneTrig && fBeamPol20;

  // CB bgd
  TA2Particle *pCB = fCB->GetParticles();
  for(Int_t i=0; i<nCB; ++i) {
    fCBBgd = IsBgdCB(pCB+i);
    if(!fCBBgd) break;
  }
  fCBBgdPol10 = fCBBgd && fBeamPol10;
  fCBBgdPol20 = fCBBgd && fBeamPol20;
  
  // TAPS bgd
  TA2Particle *pTAPS = fTAPS->GetParticles();
  for(Int_t i=0; i<nTAPS; ++i) {
    fTAPSBgd = IsBgdTAPS(pTAPS+i);
    if(!fTAPSBgd) break;
  }
  fTAPSBgdPol10 = fTAPSBgd && fBeamPol10;
  fTAPSBgdPol20 = fTAPSBgd && fBeamPol20;
  
  // CB good / TAPS good / Bgd
  if(!fCBBgd&&nCB>=1)          fCBGood   = 1;
  else if(!fTAPSBgd&&nTAPS>=1) fTAPSGood = 1;
  else if(nCB>=1||nTAPS>=1)    fBgd      = 1;
  
  fCBGoodPol10   = fCBGood   && fBeamPol10;
  fCBGoodPol20   = fCBGood   && fBeamPol20;
  fTAPSGoodPol10 = fTAPSGood && fBeamPol10;
  fTAPSGoodPol20 = fTAPSGood && fBeamPol20;
  fBgdPol10      = fBgd      && fBeamPol10;
  fBgdPol20      = fBgd      && fBeamPol20;
  
  // Good
  fGood      = fCBGood || fTAPSGood;
  fGoodPol10 = fGood   && fBeamPol10;
  fGoodPol20 = fGood   && fBeamPol20;
  
  
  /////////////////////////////////////////////////////////////////
  // Trigger selection (using trigger pattern)
  //
  Int_t iTrig = fParent->GetBitPattern()->GetHits(0,0);
  switch(iTrig) {
    case 0:  fCBSumTrig  = 1; break;
    case 3:  fTAPSOrTrig = 1; break;
    default: fNoTrig     = 1;
  }

  fCBSumTrigPol10  = fCBSumTrig  && fBeamPol10;
  fCBSumTrigPol20  = fCBSumTrig  && fBeamPol20;
  fTAPSOrTrigPol10 = fTAPSOrTrig && fBeamPol10;
  fTAPSOrTrigPol20 = fTAPSOrTrig && fBeamPol20;
  fNoTrigPol10     = fNoTrig     && fBeamPol10;
  fNoTrigPol20     = fNoTrig     && fBeamPol20;
  

  /////////////////////////////////////////////////////////////////
  // Selection based on dTime CB-Tagg and TAPS-Tagg correlation
  // 
  Bool_t isPmtCB, isRndCB, isPmtCBGood, isRndCBGood, isPmtTAPS, isRndTAPS;
  Int_t nCBTaggerAll = 0, nTAPSTaggerAll = 0;
  std::map<Double_t,Int_t> mapDTCBTagg, mapDTCBGoodTagg, mapDTTAPSTagg;
  for(Int_t iTagg=0; iTagg<fFPDNHits; ++iTagg) {
    //CB-Tagger
    mapDTCBTagg.clear();
    mapDTCBGoodTagg.clear();
    for(Int_t iCB=0; iCB<nCB; ++iCB) {
      fDTCBTaggerAll[nCBTaggerAll] = (pCB+iCB)->GetTime() - fFPDTimeOR[iTagg] + fTShiftCB;
      mapDTCBTagg[TMath::Abs(fDTCBTaggerAll[nCBTaggerAll])] = nCBTaggerAll;
      if(!IsBgdCB(pCB+iCB)) mapDTCBGoodTagg[TMath::Abs(fDTCBTaggerAll[nCBTaggerAll])] = nCBTaggerAll;
      ++nCBTaggerAll;
    }
    // all CB
    isPmtCB = isRndCB = kFALSE;
    if(!mapDTCBTagg.empty()) {
      fDTCBTagger[fNTaggCB] = fDTCBTaggerAll[mapDTCBTagg.begin()->second];
      isPmtCB = IsPmtCB(fDTCBTagger[fNTaggCB]);
      isRndCB = IsRndCB(fDTCBTagger[fNTaggCB]);
      ++fNTaggCB;
      if(isPmtCB)      fHitsTaggerPmtCB[fNPmtCB++] = fFPDHits[iTagg];
      else if(isRndCB) fHitsTaggerRndCB[fNRndCB++] = fFPDHits[iTagg];
    }
    // good CB
    isPmtCBGood = isRndCBGood = kFALSE;
    if(!mapDTCBGoodTagg.empty()) {
      fDTCBGoodTagger[fNTaggCBGood] = fDTCBTaggerAll[mapDTCBGoodTagg.begin()->second];;
      isPmtCBGood = IsPmtCB(fDTCBGoodTagger[fNTaggCBGood]);
      isRndCBGood = IsRndCB(fDTCBGoodTagger[fNTaggCBGood]);
      ++fNTaggCBGood;
      if(isPmtCBGood)      fHitsTaggerPmtCBGood[fNPmtCBGood++] = fFPDHits[iTagg];
      else if(isRndCBGood) fHitsTaggerRndCBGood[fNRndCBGood++] = fFPDHits[iTagg];
    }
    
    //TAPS-Tagger
    mapDTTAPSTagg.clear();
    isPmtTAPS = isRndTAPS = kFALSE;
    for(Int_t iTAPS=0; iTAPS<nTAPS; ++iTAPS) {
      fDTTAPSTaggerAll[nTAPSTaggerAll] = (pTAPS+iTAPS)->GetTime() + fFPDTimeOR[iTagg] + fTShiftTAPS;
      mapDTTAPSTagg[TMath::Abs(fDTTAPSTaggerAll[nTAPSTaggerAll])] = nTAPSTaggerAll;
      ++nTAPSTaggerAll;
    }
    // all TAPS
    if(!mapDTTAPSTagg.empty()) {
      fDTTAPSTagger[fNTaggTAPS] = fDTTAPSTaggerAll[mapDTTAPSTagg.begin()->second];
      isPmtTAPS = IsPmtTAPS(fDTTAPSTagger[fNTaggTAPS]);
      isRndTAPS = IsRndTAPS(fDTTAPSTagger[fNTaggTAPS]);
      ++fNTaggTAPS;
      if(isPmtTAPS)      fHitsTaggerPmtTAPS[fNPmtTAPS++] = fFPDHits[iTagg];
      else if(isRndTAPS) fHitsTaggerRndTAPS[fNRndTAPS++] = fFPDHits[iTagg];
    }
    // CB || TAPS
    if(isPmtCB||isPmtTAPS) fHitsTaggerPmt[fNPmt++] = fFPDHits[iTagg];
    if(isRndCB||isRndTAPS) fHitsTaggerRnd[fNRnd++] = fFPDHits[iTagg];
    // good CB || TAPS
    if(isPmtCBGood||isPmtTAPS) fHitsTaggerPmtGood[fNPmtGood++] = fFPDHits[iTagg];
    if(isRndCBGood||isRndTAPS) fHitsTaggerRndGood[fNRndGood++] = fFPDHits[iTagg];
  }
  
//   // CB-TAPS
//   for(Int_t iCB=0; iCB<nCB; ++iCB) {
//     for(Int_t iTAPS=0; iTAPS<nTAPS; ++iTAPS) {
//       fDTCBTAPS[fNCBTAPS++] = (pCB+iCB)->GetTime() - (pTAPS+iTAPS)->GetTime();
//     }
//   }
  
  MarkEndBuffer();
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::TriggerProcessSW()
{
  // Software trigger emulation (for the MC)
  
  Bool_t trigCB = kFALSE, trigTAPS = kFALSE;
  static UInt_t latchCB   = 1;  // Prescaled CB trigger counter
  static UInt_t latchTAPS = 1;  // Prescaled TAPS trigger counter
  
  // CB trigger
  if(fTestTriggerSWCB) {
    Double_t eNaI = 0.;          // Energy of a single NaI channel
    Bool_t   flagDisc = kFALSE;  // Flag whether a discriminator contributed to multiplicity
    // Loop over all 45 CB discriminators
    for(UInt_t i=0; i<45; i++) {
      flagDisc = kFALSE;
      // Loop over all 16 discriminator channels
      for(UInt_t j=0; j<16; j++) {
	eNaI = fNaI->GetEnergyAll(i*16+j);        // Get energy of crystal
	if(eNaI > fThresNaI[i*16+j]) flagDisc = kTRUE;  // Check if crystal above threshold (block marked as hit)
	  eNaI /= fGainsNaI[i*16+j];                 // Reconstruct original analogue signal ('de-calibrate')...
	  fEDiscCB[i] += eNaI;                              // ...and sum up analogue signals in block
      }
      fESumCB += fEDiscCB[i];       // Add analoge block sum to total CB energy sum,
      if(flagDisc) fMultCB++;       // If block marked as hit, contribute to multiplicity
    }
    // Evaluate CB trigger
    Bool_t trigESumCB  = (fESumCB >= fESumThresCB); // 1st level trigger (CB energy sum)
    Bool_t trigMultiCB = (fMultCB >= fMultThresCB); // 2nd level trigger (CB Multiplicity)
    if(trigESumCB && trigMultiCB) {
      trigCB = (latchCB == fPrescaleCB);
      latchCB = (trigCB) ? 1 : latchCB+1;
    }
  }

  // TAPS trigger
  if(fTestTriggerSWTAPS) {
    Double_t eBaF2 = 0.;           // Energy of a single BaF2 channel
    UInt_t   elements = 0;         // Number of already processed TAPS channels
    Bool_t   flagBlock[6] = {0};   // Flag, wether a TAPS block contributed to multiplicity
    // Loop over the 6 TAPS blocks
    for(UInt_t i=0; i<6; i++) {
      // Loop over all channels in one block
      for(UInt_t j=0; j<64; j++) {
	if(BaF2Contrib(elements+j)) {
	  // If inner rings are disabled, these channels do not contribute
	  eBaF2 = fBaF2->GetEnergyAll(elements+j);                 // Get energy of crystal
	  if(eBaF2 > fThresBaF2[elements+j]) flagBlock[i] = kTRUE; // Check if crystal above threshold (block marked as hit)
	}
      }
      elements += 64;            // Count how many channels have been processed so far
      if(flagBlock[i]) fMultTAPS++;  // If block marked as hit, contribute to multiplicity
    }
    // Evaluate TAPS trigger
    Bool_t trigMultiTAPS = (fMultTAPS >= fMultThresTAPS); // 2nd level trigger (TAPS Multiplicity)
    if(trigMultiTAPS) {
      trigTAPS = (latchTAPS == fPrescaleTAPS);
      latchTAPS = (trigTAPS) ? 1 : latchTAPS+1;
    }
  }

  //
  return (trigCB || trigTAPS);
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::BaF2Contrib(UInt_t element)
{
  // Is TAPS channel part of the inner rings that do not contribute to the trigger?
  
  if(fRingsTAPS > 2) return kFALSE; //TAPS trigger disabled

  for(UInt_t i=0; i<fRingsTAPS; i++)          // Loop over the rings turned off for the trigger
    for(UInt_t j=0; j<kRingSize[i]; j++)      // Loop over the channels in the inner ring i
      if(element==kRing[i][j]) return kFALSE; // If channel in ring, no contribution to trigger

  return kTRUE; //Channel is not in the inner rings
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::TriggerProcessHW()
{
  // Get the trigger from hardware information
  // NOTE: need to check
  
  UInt_t patternL1 = 0x00;
  UInt_t patternL2 = 0x00;
  
  if(fADC)
  {
    patternL1 = fADC[0] & 0xFF;
    patternL2 = fADC[1] & 0xFF;
  }
  
//   Bool_t hasEnergy   = (patternL1 & 0x01); //Accept events with energy sum
//   Bool_t hasPrescale = (patternL2 & 0x10); //Accept events with prescaled multiplicity
//   Bool_t hasMulti    = (patternL2 & 0x20); //Accept events with normal multiplicity

//   Bool_t hasTriggerA = hasEnergy && (hasPrescale && fUsePrescale);
//   Bool_t hasTriggerB = hasEnergy && (hasMulti    && fUseMulti);
  
//   if(fL2Pattern!=135) std::cout << fL2Pattern << std::endl;
//   std::cout << hasEnergy << "\t" << hasTriggerA << "\t" << hasTriggerB << std::endl;

//   return (hasTriggerA || hasTriggerB);
  return kTRUE;
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::HasCherCoinc(const Double_t &t)
{
  // Return true if t belongs to [fTCherMin, TCherMax]
  
  return t >= fTCherMin && t <= fTCherMax;
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::IsBgdCB(TA2Particle* p)
{
  // Check if the particle is CB bgd
  
//   return p->GetE()<=50. && ( p->GetThetaDg()<=40. || p->GetThetaDg()>=140. );
  return p->GetThetaDg()<=40. || p->GetThetaDg()>=140.;
}

//_____________________________________________________________________________________
Bool_t  TA2TotCSPhysics::IsBgdTAPS(TA2Particle* p)
{
  // Check if the particle is TAPS bgd
  
//   return (p+10)->GetE()<=70.;
  return kFALSE;
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::IsPmtTrig(const Double_t &dT)
{
  // Check if dT is inside of the FPD TimeOR prompt window
  
  return dT > fFPD->GetPromptMin() && dT < fFPD->GetPromptMax();
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::IsRndTrig(const Double_t &dT)
{
  // Check if dT is inside of the FPD TimeOR random windows
  
  for(UInt_t n=0; n<fFPD->GetNRandWindows(); n++) {

    if(dT>fFPD->GetRandMin()[n]&&dT<fFPD->GetRandMax()[n]) return kTRUE;
  }
  return kFALSE;
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::IsPmtCB(const Double_t &dT)
{
  // Check if dT is inside of the CB prompt window
  
  return dT > fTPmtCBMin && dT < fTPmtCBMax;
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::IsRndCB(const Double_t &dT)
{
  // Check if dT is inside of at list one the CB random windows
  
  Bool_t isRnd = kFALSE;
  for(Int_t i=0; i<2; ++i) {
    isRnd |= dT > fTRndCBMin[i] && dT < fTRndCBMax[i];
    if(isRnd) break;
  }
  
  return isRnd;
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::IsPmtTAPS(const Double_t &dT)
{
  // Check if dT is inside of the CB prompt window
  
  return dT > fTPmtTAPSMin && dT < fTPmtTAPSMax;
}

//_____________________________________________________________________________________
Bool_t TA2TotCSPhysics::IsRndTAPS(const Double_t &dT)
{
  // Check if dT is inside of at list one the CB random windows
  
  Bool_t isRnd = kFALSE;
  for(Int_t i=0; i<2; ++i) {
    isRnd |= dT > fTRndTAPSMin[i] && dT < fTRndTAPSMax[i];
    if(isRnd) break;
  }
  
  return isRnd; 
}

//_____________________________________________________________________________________
void TA2TotCSPhysics::ResetEvent()
{
  // Set default value for the variables being chenged during an event
  
  // Trigger
  fHasTriggerHW = 1;
  fHasTriggerSW = 1;
  fESumCB = 0.;
  for(UInt_t i=0; i<45; i++) fEDiscCB[i] = 0.; fEDiscCB[45] = EBufferEnd;
  fMultCB   = 0;
  fMultTAPS = 0;
  
  //
  fADC1411M0 = -10000;
  fHasCherCoinc = 0;
  
  //
  fBeamPol = fBeamPol10 = fBeamPol20 = 0;
  
  //
  fFPDTimeOR[0] = EBufferEnd;
  fFPDNHits = fFPDNHitsPmt = fFPDNHitsRnd = 0;
  fFPDHits[0] = fFPDHitsPmt[0] = fFPDHitsRnd[0] = EBufferEnd;
  fFPDEg[0] = EBufferEnd;
  
  fCBTrig      = fTAPSTrig      = fNoneTrig      = 0;
  fCBTrigPol10 = fTAPSTrigPol10 = fNoneTrigPol10 = 0;
  fCBTrigPol20 = fTAPSTrigPol20 = fNoneTrigPol20 = 0;

  fCBSumTrig      = fTAPSOrTrig      = fNoTrig      = 0;
  fCBSumTrigPol10 = fTAPSOrTrigPol10 = fNoTrigPol10 = 0;
  fCBSumTrigPol20 = fTAPSOrTrigPol20 = fNoTrigPol20 = 0;
  
  fCBBgd   = fCBBgdPol10   = fCBBgdPol20   = 0;
  fTAPSBgd = fTAPSBgdPol10 = fTAPSBgdPol20 = 0;
  fBgd     = fBgdPol10     = fBgdPol20     = 0;
  
  fCBGood   = fCBGoodPol10   = fCBGoodPol20   = 0;
  fTAPSGood = fTAPSGoodPol10 = fTAPSGoodPol20 = 0;
  fGood     = fGoodPol10     = fGoodPol20     = 0;
  
  //
  fNTaggCB = fNTaggCBGood = fNCBTAPS = fNTaggTAPS = 0;
  fNPmtCB = fNPmtCBGood = fNPmtTAPS = fNPmt = fNPmtGood = 0;
  fNRndCB = fNRndCBGood = fNRndTAPS = fNRnd = fNRndGood = 0;
  
  fDTCBTagger[0]     = EBufferEnd;
  fDTCBTaggerAll[0]  = EBufferEnd;
  fDTCBGoodTagger[0] = EBufferEnd;
  fDTCBTAPS[0]       = EBufferEnd;
  fDTTAPSTagger[0]   = EBufferEnd;
  fDTTAPSTaggerAll[0]= EBufferEnd;
  
  fHitsTaggerPmtCB[0]     = EBufferEnd;
  fHitsTaggerPmtCBGood[0] = EBufferEnd;
  fHitsTaggerPmtTAPS[0]   = EBufferEnd;
  fHitsTaggerPmt[0]       = EBufferEnd;
  fHitsTaggerPmtGood[0]   = EBufferEnd;
  fHitsTaggerRndCB[0]     = EBufferEnd;
  fHitsTaggerRndCBGood[0] = EBufferEnd;
  fHitsTaggerRndTAPS[0]   = EBufferEnd;
  fHitsTaggerRnd[0]       = EBufferEnd;
  fHitsTaggerRndGood[0]   = EBufferEnd;
  
  // Sim
  fEg = -1000.;
  fTheta[0] = fTheta[1] = -1000.;
  fPhi[0] = fPhi[1] = -1000.;
}

//_____________________________________________________________________________________
void TA2TotCSPhysics::MarkEndBuffer()
{
  // Ensure the multi-data buffers are marked as ended
  
  fFPDTimeOR[fFPDNHits]     = EBufferEnd;
  fFPDHits[fFPDNHits]       = EBufferEnd;
  fFPDEg[fFPDNHits]         = EBufferEnd;
  fFPDHitsPmt[fFPDNHitsPmt] = EBufferEnd;
  fFPDHitsRnd[fFPDNHitsRnd] = EBufferEnd;
  //
  fDTCBTagger[fNTaggCB]         = EBufferEnd;
  fDTCBTaggerAll[fFPDNHits*fCB->GetNParticle()]      = EBufferEnd;
  fDTCBGoodTagger[fNTaggCBGood] = EBufferEnd;
  fDTCBTAPS[fNCBTAPS]           = EBufferEnd;
  fDTTAPSTagger[fNTaggTAPS]     = EBufferEnd;
  fDTTAPSTaggerAll[fFPDNHits*fTAPS->GetNparticle()]  = EBufferEnd;
  //
  fHitsTaggerPmtCB[fNPmtCB]         = EBufferEnd;
  fHitsTaggerPmtCBGood[fNPmtCBGood] = EBufferEnd;
  fHitsTaggerPmtTAPS[fNPmtTAPS]     = EBufferEnd;
  fHitsTaggerPmt[fNPmt]             = EBufferEnd;
  fHitsTaggerPmtGood[fNPmtGood]     = EBufferEnd;
  fHitsTaggerRndCB[fNRndCB]         = EBufferEnd;
  fHitsTaggerRndCBGood[fNRndCBGood] = EBufferEnd;
  fHitsTaggerRndTAPS[fNRndTAPS]     = EBufferEnd;
  fHitsTaggerRnd[fNRnd]             = EBufferEnd;
  fHitsTaggerRndGood[fNRndGood]     = EBufferEnd;
}
