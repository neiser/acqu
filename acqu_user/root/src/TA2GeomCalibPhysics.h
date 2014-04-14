#ifndef __TA2GeomCalibPhysics_h__
#define __TA2GeomCalibPhysics_h__

// AcquROOT includes
#include "TA2Physics.h"
#include "TA2CalArray.h"
#include "TA2PlasticPID.h"

// AcquROOT classes
class TA2CentralApparatus;
class TA2CylMwpc;

class TA2GeomCalibPhysics : public TA2Physics {
protected:
  
  static const Int_t kNch = 2;
  
  ////////////////////////////
  // Apparatuses & Detectrors
  ////////////////////////////
  
  TA2CentralApparatus	*fCB;	// Central apparatus
  //
  TA2CalArray		*fNaI;	// NaI array from CB
  //
  TA2CylMwpc		*fMwpc;		// Mwpc
  const Int_t		*fNintersTrue;
  //
  TA2PlasticPID		*fPid;	// Pid
  
  ////////////////////////////
  // Histograms
  ////////////////////////////
  
  // E-IW
  Double_t	*fPhiInters[kNch];
  Double_t	*fZinters[kNch];
  Double_t	*fDphiEI[kNch];
  Double_t	*fCGclI[kNch];
  Double_t	*fCGclW[kNch];
  Double_t	*fCGclE[kNch];
  Double_t	*fPhiEI[kNch];
  TH3D		*fH3EI_dPhiEI_PhiEI_Zinters[kNch];
//   TH2D		*fH2D_DphiEI_PhiEI_Zinters[kNch];
//   TH2D		*fH2D_Nevent_PhiEI_Zinters[kNch];
  
  // Mwpc_i -> Mwpc_0
  // intersection with XYZ
  Double_t	fDphiMwpc21;
  Double_t	fDxMwpc21;
  Double_t	fDyMwpc21;
  Double_t	fDzMwpc21;
  Double_t	fZ0Mwpc21;
  // intersection with MWPC-1
  Double_t	fDxInterMwpc[3];
  Double_t	fDyInterMwpc[3];
  Double_t	fDzInterMwpc[3];
  Double_t	fDphiInterMwpc[3];
  Double_t	fZinterMwpc[3];
  Double_t	fPhiInterMwpc[3];
  TH3D	       *fH3Mwpc_dPhi_vs_Phi_vs_Z;
  TH3D	       *fH3Mwpc_dZ_vs_Phi_vs_Z;
  //
  Double_t	fDphiIntersMwpc0;
  Double_t	fDpsiTracks; // spatial angle beetween 2 tracks
  Double_t	fDphiTracks;
  Double_t	fDtetTracks;
  Double_t	fDtetTracks2[3]; // to compare with Susanna
  Double_t	fDzPsVertexTracks;
  Double_t	fThetaTracks;
  Double_t	fThetaTracks2[3];
  Double_t	fPhiTracks;
   
  // Mwpc_i -> NaI
  // intersection with XYZ
  Double_t	fDphiMwpcNaI[kNch];
  Double_t	fDxMwpcNaI[kNch];
  Double_t	fDyMwpcNaI[kNch];
  Double_t	fDzMwpcNaI[kNch];
  Double_t	fZ0MwpcNaI[kNch];
  // intersection with MWPCs
  Double_t	fDxInterMwpcNaI[kNch][3];
  Double_t	fDyInterMwpcNaI[kNch][3];
  Double_t	fDzInterMwpcNaI[kNch][3];
  Double_t	fDphiInterMwpcNaI[kNch][3];
  Double_t	fZinterMwpcNaI[kNch][3];
  Double_t	fPhiInterMwpcNaI[kNch][3];
  TH3D	       *fH3MwpcNaI_dPhi_vs_Phi_vs_Z[kNch];
  TH3D	       *fH3MwpcNaI_dZ_vs_Phi_vs_Z[kNch];
  //
  Double_t	fDphiClNaI[kNch];
  Double_t	fDpsiTracksMwpcNaI[kNch]; // spatial angle beetween 2 tracks
  Double_t	fDpsiTracksSingleMwpcNaI[kNch]; // spatial angle beetween 2 tracks
  Double_t	fDphiTracksMwpcNaI[kNch];
  Double_t	fDphiTracksSingleMwpcNaI[kNch];
  Double_t	fDtetTracksMwpcNaI[kNch];
  Double_t	fDzPsVertexTracksMwpcNaI[kNch];
  Double_t	fThetaTracksMwpcNaI[kNch];
  Double_t	fPhiTracksMwpcNaI[kNch];
  Double_t	fDtetTracksSingleMwpcNaI[kNch];
  Double_t	fThetaCosmicNaI[kNch]; // TODO
  
  // Pid -> Mwpc_i
  Double_t	fDphiMwpcPid[kNch];
  Double_t	fDxMwpcPid[kNch];
  Double_t	fDyMwpcPid[kNch];
  //
  Double_t	fDphiHitsPid[kNch];
  Double_t	fDpsiTracksMwpcPid[kNch]; // spatial angle beetween 2 tracks
  Double_t	fDphiTracksMwpcPid[kNch];
  Double_t	fZinterMwpcPid[kNch][3];
  Double_t	fDphiInterMwpcHitPid[kNch][3];
  Int_t		fIhitPid[kNch][3];
  
  // Pid -> NaI
  Double_t	fDphiPidNaI;
  Double_t	fDxPidNaI;
  Double_t	fDyPidNaI;
  //
  Double_t	fDphiTracksPidNaI;
  Double_t	fDpsiTracksPidNaI;
  Double_t	fDphiClNaIwithPid;
  
public:
  TA2GeomCalibPhysics( const char*, TA2Analysis* );
  virtual ~TA2GeomCalibPhysics();
//   virtual void 		  SetConfig(Char_t*, Int_t);
  virtual void 		  LoadVariable();            // variables for display/cuts
  virtual void 		  PostInit();                // init after parameter input
  virtual void 		  Reconstruct();             // reconstruct detector info
  virtual TA2DataManager *CreateChild( const char*, Int_t ) { return NULL; }
  virtual void		  MarkEndBuffers();		// Mark EndBuffer for the output arrays
  
  virtual void	ResetEvent();
  //
  virtual void	CalibEIW(const Int_t);
  virtual void	CalibMwpc(const Int_t);
  virtual void	CalibMwpcNaI(const Int_t);
  virtual void	CalibPidMwpc(const Int_t);
  virtual void	CalibPidNaI();

  ClassDef(TA2GeomCalibPhysics,1)
};

#endif
