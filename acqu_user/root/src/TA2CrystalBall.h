//--Author	JRM Annand   20th Dec 2002
//--Rev 	JRM Annand...26th Feb 2003...1st production version
//--Rev 	JRM Annand... 1st Oct 2003...Incorp. TA2DataManager
//--Rev 	JRM Annand...12th Dec 2003...fMulti 2D plots
//--Rev 	JRM Annand...19th Feb 2004...User code
//--Rev  	E Downie......1st Jan 2005...Clean up PID mods
//--Rev 	E Downie.....20th Jan 2005...Add MWPC track getting
//--Rev 	E Downie.....20th Feb 2005...Energy sum over PID joins,clean up
//--Rev 	JRM Annand...10th Nov 2006...Check MWPC defined in reconstruct
//--Update	JRM Annand....1st Feb 2007...OR dE-E variable for common cut
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data
//
// TA2CrystalBall
//
// User coded version of Crystal Ball apparatus.  Includes identification
// of charged particles, but no method for differentiation between g and n

#ifndef __TA2CrystalBall_h__
#define __TA2CrystalBall_h__

#include "TA2Apparatus.h"            // Acqu-Root histogrammer
#include "TA2Particle.h"
#include "TFile.h"
#include "TCutG.h"

#include "TA2PlasticPID.h"
#include "TA2CalArray.h"
#include "TA2CylMwpc.h"

class TA2CylMwpc;

enum { ECBNullPartner = -999999 };

class TA2CrystalBall : public TA2Apparatus
{
 protected:
  TA2ClusterDetector* fNaI;    // Pointer to NaI array
  TA2Particle* fParticle;      // Pointer to TA2Particle array
  TA2Detector* fPID;           // Pointer to PID
  TA2CylMwpc*  fMWPC;          // Pointer to MWPC
  Double_t*    fPIDdPhi;       // PID-CB Phi differences
  Double_t     fMaxPIDdPhi;    // Max PID-CB Phi difference
  Double_t*    fMWPCdTheta;    // PID-CB Phi differences
  Double_t     fMaxMWPCdTheta; // Max MWPC-CB angle difference
  Int_t*       fAngDiffI;      // for general sorting of angle differences
  Int_t*       fPIDij;         // back reference PID j, to CB i
  Int_t*       fPIDindex;      // Delta-E index associated with cluster
  Int_t*       fPIDdouble;     // Delta-E 2nd index associated with cluster
  Int_t*       fMWPCij;        // back reference MWPC track j, to CB i
  Int_t*       fMWPCindex;     // MWPC track index associated with cluster
  Double_t*    fDeltaE;        // Cluster correlated PID energy deposit
  Double_t*    fDeltaEdouble;  // Cluster correlated PID energy deposit
  Double_t*    fECentral;      // Fraction total energy in central clust elem.
  Double_t*    fEch;           // PID correlated Cluster energy deposit
  Double_t*    fDeltaPhi;      // Difference in PiD and cluster phi
  Double_t*    fDeltaTheta;    // Difference in PiD and cluster phi
  Double_t     fDeltaEOR;      // OR of Delta-E (for common dE-E cuts)
  Double_t     fEchOR;         // OR correlated cluster-E (common dE-E cuts)
  Int_t        fNCharged;      // Number of CB clusters identified as charged
  Double_t     fPIDSigmaTime;  // PID Time resolution;
  Double_t     fNaISigmaTime;  // CB Time Resolution;

 public:
  TA2CrystalBall( const char*, TA2System* );  // pass ptr to analyser
  virtual ~TA2CrystalBall();                  // destructor
  virtual void PostInit();                    // some setup after parms read in
  virtual TA2DataManager* CreateChild( const char*, Int_t );
  virtual void LoadVariable(  );              // display setup
  virtual void Reconstruct( );                // event by event analysis
  virtual void Cleanup();                     // reset at end of event
  virtual void SetConfig( Char_t*, Int_t );   // setup decode in implement

  TA2ClusterDetector* GetNaI()      { return fNaI; }
  TA2Detector*        GetPID()      { return fPID; }
  TA2CylMwpc*         GetMWPC()     { return fMWPC; }
  Int_t               GetNCharged() { return fNCharged;}
  Double_t*           GetDeltaE()   { return fDeltaE; }
  Double_t            GetCBMeanTime();
  TA2Particle*        GetParticles() { return fParticle; }
  TA2Particle         GetParticles(Int_t index){ return fParticle[index]; }
  Int_t GetNparticle(){ return fNparticle; }

  // Root needs this line for incorporation in dictionary
  ClassDef(TA2CrystalBall,1)
};

//-----------------------------------------------------------------------------

inline Double_t TA2CrystalBall::GetCBMeanTime()
{
  Double_t time = 0.0;
  Int_t counter = 0;
  UInt_t* id_clCal = fNaI->GetClustHit();    //Indices of hit clusters
  HitCluster_t** clCal = fNaI->GetCluster(); //Pointer to cluster structs
  HitCluster_t* cl;                          //Cluster struct

  for(Int_t i = 0; i<fNparticle; i++)
  {
    if(fPDG_ID[i]==kGamma)
    {
     cl = clCal[id_clCal[i]];
     time = time + cl->GetTime();
     counter++;
    }
  }
  if(counter==0)
    return 0.0;
  else
    return (time/counter);
}

//-----------------------------------------------------------------------------

#endif
