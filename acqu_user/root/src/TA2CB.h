#ifndef __TA2CB_h__
#define __TA2CB_h__

#include "TA2Apparatus.h"              // base apparatus class
#include "TA2ClusterDetector.h"        // calorimeter array

#include "TA2CylMwpc.h"
#include "TA2PlasticPID.h"
#include "TA2CalArray.h"
#include "TA2Particle.h"

#include "TCutG.h"
#include "TFile.h"

class TA2CB : public TA2Apparatus
{
 protected:
  TA2ClusterDetector* fCal; //Calorimeter
  TA2Detector* fPID;        //Charged-particle ID
  TA2Detector* fTracker;    //Charged-particle tracker
  TA2Particle* fParticles;  //Used to store particle information

  std::vector<TCutG*> fPIDCuts;     //Pointer to DeltaE-E cut
  Int_t* fPIDElement; //Indices of PID strips cuts are defined for
  Int_t* fPIDCode;    //PDG particle ID code to produce if inside cut
  Int_t nPIDCuts;     //Total number of defined cuts
  Int_t nPIDCount;    //Counter for current evaluated cut

  UInt_t nPIDDroopPoints;    //Number of points used for the droop correction, specified in CB.dat
  Double_t* fPIDLostCorrPos; //Positions for droop correction
  Double_t* fPIDLostCorrFac; //Factors used for the droop correction at positions in PIDLostCorrPos
  Double_t fPIDRadius;       //Radius of PID element 0
  Double_t fPIDLength;       //Length of PID element 0
  Double_t* fPIDPhiPlastic;  //Phi angle of PID element
  Double_t fPIDTimeShift;    //Shift parameter for PID timing
  Bool_t bPIDIsDroop;        //Droop correction on/off switch

  Double_t fPosShift[3];   //Shift parameter for NaI positions
  Double_t fMaxDPhiPID;    //Max phi difference between PID and NaI
  Double_t fMaxDPhiMWPC;   //Max phi difference between MWPC track and NaI
  Double_t fMaxDThetaMWPC; //Max theta difference between MWPC track and NaI
  Double_t fMaxDOmega2;    //MWPC tracks are assigned if within a ellipse in phi,theta plane
  Double_t fRatio2;        //Correction factor

  Int_t* fType;    //Field for numerical particle-ID. Correlated with fP4!
  Int_t* fDet;     //Bit field of used detector information. Correlated with fP4!
  Int_t* fSize;    //Particle cluster size. Correlated with fP4!
  Int_t* fCentral; //Central crystal of cluster. Correlated with fP4!
  Double_t* fTime; //Time of all detected particles. Correlated with fP4!

  Double_t* fEPlastic; //Energies of charged particles deposited in the PID (corrected for path length)
  Double_t* fECrystal; //Energies of charged particles deposited in the NaI array
  Int_t nPlastic;      //Number of PID strips

  //For histogramming DeltaE-E and phi correlation
  Double_t* fDeltaPhi;
  Double_t* fEPlasticOR;
  Double_t* fECrystalOR;
  Int_t nDeltaPhi;
  Int_t nCharged;

  Int_t nGamma;
  
 public:
  TA2CB(const char*, TA2System*);
  virtual ~TA2CB();
  virtual TA2DataManager* CreateChild(const char*, Int_t);
  virtual void PostInit();
  virtual void LoadVariable();
  virtual void Reconstruct();
  virtual void Cleanup();
  virtual void ParseMisc(char* line);
  virtual void PhotonReconstruct();
  virtual void DoParticleIdent();
  virtual TLorentzVector ParticleMass(TVector3, Double_t, Double_t);              //Recalculate 4-momentum with new mass and kinetic energy
  virtual TLorentzVector SetLorentzVector(Double_t, HitCluster_t*);               //Same as above, with other calling conventions...
  virtual Bool_t MakeTrack(TLorentzVector*);                                      //Replace direction information in 4-momentum with MWPC data
  virtual Bool_t MakeDEvE(TLorentzVector*, Int_t*, Double_t*, Double_t*, Int_t*); //Do DEvE analysis

  TA2ClusterDetector* GetCal(){ return fCal; }
  TA2Detector* GetPID(){ return fPID; }
  TA2Detector* GetPTracker(){ return fTracker; }

  TA2Particle* GetParticles() { return fParticles; }
  TA2Particle GetParticles(Int_t index){ return fParticles[index]; }

  Int_t* GetType(){ return fType; }       //Field for numerical particle-ID. Correlated with GetP4()!
  Int_t* GetDet(){ return fDet; }         //Bit field of used detector information. Correlated with GetP4()!
  Int_t* GetSize(){ return fSize; }       //Particle cluster size. Correlated with GetP4()!
  Int_t* GetCentral(){ return fCentral; } //Central crystal of cluster. Correlated with GetP4()!
  Double_t* GetTime(){ return fTime; }    //Time of all detected particles. Correlated with GetP4()!

  Double_t GetCBMeanTime();

  //Root needs this line for incorporation in dictionary
  ClassDef(TA2CB,1)
};

//-----------------------------------------------------------------------------

inline void TA2CB::Cleanup()
{
  //Clear any arrays holding variables
 TA2DataManager::Cleanup();

 if(!fNparticle) return;

 for(Int_t i=0; i<fNparticle; i++)
   fP4[i].SetXYZT(0.,0.,0.,0.);
 fNparticle = 0;
}

//-----------------------------------------------------------------------------

inline void TA2CB::PhotonReconstruct()
{
  //Get energies and "center of charge" vectors from calorimeter arrays epitomised by TA2ClusterDetector.
  //Create 4-vectors assuming cluster hits are photons

  UInt_t* id_clCal;     //Indices of hit clusters
  HitCluster_t** clCal; //Pointer to cluster structs
  HitCluster_t* cl;     //Cluster struct

  fNparticle = fCal->GetNCluster();
  id_clCal = fCal->GetClustHit();
  clCal = fCal->GetCluster();

  //For each cluster create 4 momentum from cluster energy and direction assuming it's a photon
  for (Int_t i=0; i<fNparticle; i++)
  {
    cl = clCal[id_clCal[i]]; //i-th hit cluster struct

    //Collect cluster size and crystal index
    fSize[i] = cl->GetNhits();
    fCentral[i] = cl->GetIndex();

    //Use central crystal timing information for whole cluster
    fTime[i] = cl->GetTime();
    fP4[i] = SetLorentzVector(0.0, cl);
    fType[i] = kGamma;
    fDet[i] = EDetNaI;

    fParticles[i].Reset();
    fParticles[i].SetP4(fP4[i]);
    fParticles[i].SetTime(fTime[i]);
    fParticles[i].SetClusterSize(fSize[i]);
    fParticles[i].SetCentralIndex(fCentral[i]);
    fParticles[i].SetApparatus(EAppCB);
    fParticles[i].SetDetector(EDetNaI);
    fParticles[i].SetSigmaTime(((TA2CalArray*)fCal)->GetSigmaTime());
    fParticles[i].SetSigmaE(((TA2CalArray*)fCal)->GetSigmaEnergy(fP4[i].E()));
    fParticles[i].SetSigmaPhi(((TA2CalArray*)fCal)->GetSigmaPhi(fP4[i].Theta()));
    fParticles[i].SetSigmaTheta(((TA2CalArray*)fCal)->GetSigmaTheta());
    fParticles[i].SetParticleIDA(kGamma);
  }
}

//-----------------------------------------------------------------------------

inline TLorentzVector TA2CB::SetLorentzVector(Double_t mass, HitCluster_t* cl)
{
  //Get cluster kinetic energy and particle direction and construct 4-momentum using input assumed mass

  Double_t E = cl->GetEnergy() + mass;
  Double_t p = TMath::Sqrt(E*E - mass*mass);
  TLorentzVector p4;
  TVector3 p_;

  p4.SetE(E);
  p_ = *(cl->GetMeanPosition());
  p_.SetX(p_.X() + fPosShift[0]);
  p_.SetY(p_.Y() + fPosShift[1]);
  p_.SetZ(p_.Z() + fPosShift[2]);
  p4.SetVect(p_.Unit() * p);

  return p4;
}

//-----------------------------------------------------------------------------

inline void TA2CB::Reconstruct()
{
  nGamma = 0;
  
  PhotonReconstruct();
  if(fPID) DoParticleIdent();
}

//-----------------------------------------------------------------------------

inline Double_t TA2CB::GetCBMeanTime()
{
  Double_t Time = 0.0;
  Int_t Count = 0;

  for(Int_t t=0; t<fNparticle; t++)
    if(fType[t]==kGamma)
    {
     Time+=fTime[t];
     Count++;
    }

  if(Count==0)
    return 0.0;
  else
    return (Time/Count);
}

//-----------------------------------------------------------------------------

#endif
