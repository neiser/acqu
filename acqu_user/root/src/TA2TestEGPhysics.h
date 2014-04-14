// SVN info $Id$
#ifndef __TA2TestEGPhysics_h__
#define __TA2TestEGPhysics_h__

// AcquROOT includes
#include "TA2Physics.h"

// AcquROOT classes
class TA2Tagger;
class TA2Ladder;
// class TA2CB;
class TA2CentralApparatus;
class TA2Taps;

class TA2TestEGPhysics : public TA2Physics {
protected:
  // Apparatuses & Detectrors
  TA2Tagger         *fTAGG;		// Glasgow photon tagger
  TA2Ladder         *fFPD;		// Focal Plane Detectror
//   TA2CB             *fCB;		// Crystall Ball
  TA2CentralApparatus *fCB;
  TA2CalArray       *fNaI;      		// NaI array from CB
  TA2Taps           *fTAPS;		// TAPS
  TA2TAPS_BaF2      *fBaF2;    		// BaF2 array from TAPS

  //
  Double_t           fTShiftCB;		// A shift between CB and FPD times
  Double_t           fTShiftTAPS;	// A shift between TAPS and FPD times
  Double_t           fTCherMin;		// Cherenkov time min
  Double_t           fTCherMax;		// Cherenkov time max
  Double_t           fTPmtCBMin;	// CB-FPD prompt time min
  Double_t           fTPmtCBMax;	// CB-FPD prompt time max
  Double_t           fTRndCBMin[2];	// CB-FPD random time min
  Double_t           fTRndCBMax[2];	// CB-FPD random time max
  Double_t           fTPmtTAPSMin;	// TAPS-FPD prompt time min
  Double_t           fTPmtTAPSMax;	// TAPS-FPD prompt time max
  Double_t           fTRndTAPSMin[2];	// TAPS-FPD random time min
  Double_t           fTRndTAPSMax[2];	// TAPS-FPD random time max
  
  //Trigger
  Int_t              fHasTriggerHW;     // 1 - an event has a hardware trigger, 0 - hasn't
  Int_t              fHasTriggerSW;     // 1 - an event has a software trigger, 0 - hasn't
  Bool_t             fTestTriggerSW;    // 1 -test software trigger, 0 - do not
  Bool_t             fTestTriggerSWCB;  // 1 -test software trigger, 0 - do not
  Bool_t             fTestTriggerSWTAPS;// 1 -test software trigger, 0 - do not
  //CB
  Double_t           fESumThresCB;      // Threshold for energy sum in CB
  Double_t           fESumCB;           // L1 Energy sum in CB
  Double_t           fEDiscCB[46];      // Energy in every CB discriminator
  UInt_t             fMultThresCB;      // Threshold for multiplicity in CB
  UInt_t             fMultCB;           // L2 Multiplicity in CB
  UInt_t             fPrescaleCB;       // Prescale factor for CB triggers
  Double_t           fGainsNaI[720];    // Relative calibration of NaI crystals (used in energy sum calculation)
  Double_t           fThresNaI[720];    // Discriminator thresholds for each single CB NaI channel
  // TAPS
  UInt_t             fMultThresTAPS;    // Threshold for multiplicity in TAPS
  UInt_t             fMultTAPS;         // L2 Multiplicity in TAPS
  UInt_t             fPrescaleTAPS;     // Prescale factor for TAPS triggers
  UInt_t             fRingsTAPS;        //TAPS rings not contributing to trigger
  Double_t           fThresBaF2[510];   // Discriminator thresholds for each single TAPS BaF2 channel
  
  //
  Bool_t             fIsSim;		// 1 - simulation, 0 - experiment
  Bool_t             fCheckCher;	// 1 - check the coincidence with the cherenkov detector
  Bool_t             fIsMultihitOn;

  // Output
  static const Int_t kBeamPolConst = 15; // 0b0000000000001111 (only the first 4 bit are important for pol)
  Int_t              fBeamPol;		// the photon beam polarization
  Int_t              fBeamPol10;	// 1 - fBeamPol = 10
  Int_t              fBeamPol20;	// 1 - fBeamPol = 20
  Int_t              fADC1411M0;	// Cherenkov TDC
  Int_t              fHasCherCoinc;	// 1 - fTCherMin < fADC1411M0 < fTCherMax
  //
  Double_t          *fFPDTimeOR;	// FPD TimeOR for all the hits multiplicities
  Int_t              fFPDNHits;		// # FPD hits for the hits multiplicities
  Int_t             *fFPDHits;		// FPD hits for all the hits multiplicities
  Double_t          *fFPDEg;		// Impinging photon energy for all the hits multiplicities
  Int_t              fFPDNHitsPmt;	// # FPD prompt hits for all the hits multiplicities
  Int_t             *fFPDHitsPmt;	// FPD prompt hits for all the hits multiplicities
  Int_t              fFPDNHitsRnd;	// # FPD random hits for all the hits multiplicities
  Int_t             *fFPDHitsRnd;	// FPD random hits for all the hits multiplicities
  //
  Int_t              fCBTrig;		// 1 - Nparticles in CB>0
  Int_t              fTAPSTrig;		// 1 - Nparticles in CB=0 && Nparticles in TAPS>0
  Int_t              fNoneTrig;		// 1 - Nparticles in CB=0 && Nparticles in TAPS=0
  Int_t              fCBTrigPol10;	// 1 - fCBTrig=1 && fBeamPol10=1
  Int_t              fTAPSTrigPol10;	// 1 - fTAPSTrig=1 && fBeamPol10=1
  Int_t              fNoneTrigPol10;	// 1 - fNoneTrig=1 && fBeamPol10=1
  Int_t              fCBTrigPol20;	// 1 - fCBTrig=1 && fBeamPol20=1
  Int_t              fTAPSTrigPol20;	// 1 - fTAPSTrig=1 && fBeamPol20=1
  Int_t              fNoneTrigPol20;	// 1 - fNoneTrig=1 && fBeamPol20=1
  //
  Int_t              fCBSumTrig;	// 1 - CBSum trigger
  Int_t              fTAPSOrTrig;	// 1 - TAPSOr trigger
  Int_t              fNoTrig;		// 1 - No trigger
  Int_t              fCBSumTrigPol10;	// 1 - CBSum trigger && fBeamPol10=1
  Int_t              fTAPSOrTrigPol10;	// 1 - TAPSOr trigger && fBeamPol10=1
  Int_t              fNoTrigPol10;	// 1 - No trigger && fBeamPol10=1
  Int_t              fCBSumTrigPol20;	// 1 - CBSum trigger && fBeamPol20=1
  Int_t              fTAPSOrTrigPol20;	// 1 - TAPSOr trigger && fBeamPol20=1
  Int_t              fNoTrigPol20;	// 1 - No trigger && fBeamPol20=1
  //
  Int_t              fCBBgd;		// 1 - No good CB particle
  Int_t              fCBBgdPol10;	// 1 - fCBBgd=1 && fBeamPol10=1
  Int_t              fCBBgdPol20;	// 1 - fCBBgd=1 && fBeamPol20=1
  Int_t              fCBGood;		// 1 - There is at least 1 good CB particle
  Int_t              fCBGoodPol10;	// 1 - fCBGood=1 && fBeamPol10=1
  Int_t              fCBGoodPol20;	// 1 - fCBGood=1 && fBeamPol20=1
  //
  Int_t              fTAPSBgd;		// 1 - No good TAPS particle
  Int_t              fTAPSBgdPol10;	// 1 - fTAPSBgd=1 && fBeamPol10=1
  Int_t              fTAPSBgdPol20;	// 1 - fTAPSBgd=1 && fBeamPol20=1
  Int_t              fTAPSGood;		// 1 - There is at least 1 good TAPS particle && No good CB particle
  Int_t              fTAPSGoodPol10;	// 1 - fTAPSGood=1 && fBeamPol10=1
  Int_t              fTAPSGoodPol20;	// 1 - fTAPSGood=1 && fBeamPol20=1
  //
  Int_t              fBgd;		// 1 - fCBBgd=1 && fTAPSBgd=1
  Int_t              fBgdPol10;		// 1 - fBgd=1 && fBeamPol10=1
  Int_t              fBgdPol20;		// 1 - fBgd=1 && fBeamPol20=1
  Int_t              fGood;		// 1 - fCBGood=1 || fTAPSGood=1
  Int_t              fGoodPol10;	// 1 - fGood=1 && fBeamPol10=1
  Int_t              fGoodPol20;	// 1 -  fGood=1 && fBeamPol20=1
  //
  Double_t          *fDTCBTaggerAll;		// dT for all the CB-particles FPD-hits pairs
  Double_t          *fDTTAPSTaggerAll;		// dT for all the TAPS particles FPD-hits pairs
  Double_t          *fDTCBTagger;		// dT CB-FPD closest to 0
  Double_t          *fDTCBGoodTagger;		// dT CBGood-FPD closest to 0
  Double_t          *fDTCBTAPS;			// dT for all the CB-FPD particles pairs
  Double_t          *fDTTAPSTagger;		// dT TAPS-FPD closest to 0
  Int_t              fNTaggCB;			// fDTCBTagger array length
  Int_t              fNTaggCBGood;		// fDTCBGoodTagger array length
  Int_t              fNCBTAPS;			// fDTCBTAPS array length
  Int_t              fNTaggTAPS;		// fDTTAPSTagger array length
  Int_t             *fHitsTaggerPmtCB;		// FPD hits giving prompt FPD-CB dT
  Int_t             *fHitsTaggerPmtCBGood;	// FPD hits giving prompt FPD-CBGood dT
  Int_t             *fHitsTaggerPmtTAPS;	// FPD hits giving prompt FPD-TAPS dT
  Int_t             *fHitsTaggerPmt;		// FPD hits giving prompt FPD-CB or FPD-TAPS dT
  Int_t             *fHitsTaggerPmtGood;	// FPD hits giving prompt FPD-CBGood or FPD-TAPS dT
  Int_t             *fHitsTaggerRndCB;		// FPD hits giving random FPD-CB dT
  Int_t             *fHitsTaggerRndCBGood;	// FPD hits giving random FPD-CBGood dT
  Int_t             *fHitsTaggerRndTAPS;	// FPD hits giving random FPD-TAPS dT
  Int_t             *fHitsTaggerRnd;		// FPD hits giving random FPD-CB or FPD-TAPS dT
  Int_t             *fHitsTaggerRndGood;	// FPD hits giving random FPD-CBGood or FPD-TAPS dT
  Int_t              fNPmtCB;			// # FPD-CB prompt hits
  Int_t              fNPmtCBGood;		// # FPD-CBGood prompt hits
  Int_t              fNPmtTAPS;			// # FPD-TAPS prompt hits
  Int_t              fNPmt;			// # (FPD-CB or FPD-TAPS) prompt hits
  Int_t              fNPmtGood;			// # (FPD-CBGood or FPD-TAPS) prompt hits
  Int_t              fNRndCB;			// # FPD-CB random hits
  Int_t              fNRndCBGood;		// # FPD-CBGood random hits
  Int_t              fNRndTAPS;			// # FPD-TAPS random hits
  Int_t              fNRnd;			// # (FPD-CB or FPD-TAPS) random hits
  Int_t              fNRndGood;			// # (FPD-CBGood or FPD-TAPS) random hits
  //
  Double_t           fTheta[2];			// simulated theta
  Double_t           fPhi[2];			// simulated phi
  Double_t           fEg;			// incident photo energy
  
  // Pi0 CB
  static const Int_t fNggMax = 100;
  Int_t fNggCB;
  Double_t *fMggCB;
  Double_t  fMggBestCB;
  Int_t fHasPi0CB; // 1 - >=1 pi0-detected in CB, 0 - no pi0 in CB
  Int_t fHasPi0CBPol10;
  Int_t fHasPi0CBPol20;
  // Pi0 CB-TAPS
  Int_t fNggCBTAPS;
  Double_t *fMggCBTAPS;
  Double_t  fMggBestCBTAPS;
  Int_t fHasPi0CBTAPS; // 1 - >=1 pi0-detected in CB-TAPS, 0 - no pi0 in CB-TAPS
  Int_t fHasPi0CBTAPSPol10;
  Int_t fHasPi0CBTAPSPol20;
  // Pi0 in TAPS
  Int_t fNggTAPS;
  Double_t *fMggTAPS;
  Double_t  fMggBestTAPS;
  Int_t fHasPi0TAPS; // 1 - >=1 pi0-detected in TAPS, 0 - no pi0 in TAPS
  Int_t fHasPi0TAPSPol10;
  Int_t fHasPi0TAPSPol20;
  //
  Int_t fHasPi0;
  Int_t fHasPi0Pol10;
  Int_t fHasPi0Pol20;
  
  // Charged
  Int_t fNch;
  Double_t *fEpid;
  Double_t *fEcb;
  

public:
  TA2TestEGPhysics( const char*, TA2Analysis* );
  virtual ~TA2TestEGPhysics();
  virtual void 		  SetConfig(Char_t*, Int_t);
  virtual void 		  LoadVariable();            // variables for display/cuts
  virtual void 		  PostInit();                // init after parameter input
  virtual void 		  Reconstruct();             // reconstruct detector info
  virtual TA2DataManager *CreateChild( const char*, Int_t ) { return NULL; }
  virtual void            MarkEndBuffer();
  
  virtual void            ResetEvent();			// Reset all event dependent variables
  virtual Bool_t          IsBgdCB(TA2Particle*);
  virtual Bool_t          IsBgdTAPS(TA2Particle*);
  virtual Bool_t          IsPmtTrig(const Double_t&);
  virtual Bool_t          IsRndTrig(const Double_t&);
  virtual Bool_t          IsPmtCB(const Double_t&);
  virtual Bool_t          IsRndCB(const Double_t&);
  virtual Bool_t          IsPmtTAPS(const Double_t&);
  virtual Bool_t          IsRndTAPS(const Double_t&);
  virtual Bool_t          HasCherCoinc(const Double_t&);
  
  // Trigger
  virtual Bool_t	  TriggerProcessSW();  //Generates the trigger for the MC
  virtual Bool_t	  TriggerProcessHW();  //Generates the trigger from hardware information
  virtual Bool_t	  BaF2Contrib(UInt_t); //Is a BaF2 crystal part of the inner TAPS rings?
  
  // Pi0
  virtual void ReconstructPi0();

  ClassDef(TA2TestEGPhysics,1)
};

#endif
