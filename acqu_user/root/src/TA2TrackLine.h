#ifndef __TA2TrackLine_h__
#define __TA2TrackLine_h__

// ROOT
#include <TVector3.h>

// AcquRoot
#include "TA2Math.h"

class TA2TrackLine : public TObject {
  protected:
    Int_t	 fType;		// Track type
    TVector3	 fOrigin;	// Track origin
    TVector3	 fDirCos;	// Direction cosines (NOT unit vector)
    TVector3	 fPsVertex;	// Pseudo vertex (closest point to the Z axis)
    
  public:
    TA2TrackLine() : fType(0) {}
    virtual ~TA2TrackLine() {}
    virtual void Reset();
    const Int_t &GetType() const { return fType; }
    const TVector3 &GetOrigin() const { return fOrigin; }
    const TVector3 &GetDirCos() const { return fDirCos; }
    const TVector3 &GetPsVertex() const { return fPsVertex; }
    const TVector3 *GetOriginPtr() const { return &fOrigin; }
    const TVector3 *GetDirCosPtr() const { return &fDirCos; }
    const TVector3 *GetPsVertexPtr() const { return &fPsVertex; }
    Double_t GetPhi() const { return fDirCos.Phi(); }
    Double_t GetTheta() const { return fDirCos.Theta(); }
    void SetType(const Int_t type) { fType = type; }
    void SetOrigin(const TVector3 &orig) { fOrigin = orig; }
    void SetDirCos(const TVector3 &dir) { fDirCos = dir; }
    void SetPsVertex(const TVector3 &vertex) { fPsVertex = vertex; }
    virtual Bool_t BuildTrack(const TVector3&, const TVector3&, const Double_t &maxDirCosMag = -1., const Double_t *limitsPsVertex = NULL);
    Double_t Angle(const TVector3 &v) const;
    Double_t Angle(const TA2TrackLine &tr) const;
    Double_t Angle(const TA2TrackLine *tr) const { return Angle(*tr); }
    Double_t Angle(const TVector2 &v) const;
    TVector3 Vertex(const TA2TrackLine&) const;
    TVector3 Vertex(const TA2TrackLine&, Double_t &dist) const;
    
    ClassDef(TA2TrackLine,1) // Particle track line
};

//_________________________________________________________________________________________
inline void TA2TrackLine::Reset()
{
  // Set default track info
  fType = 0;
  fOrigin.SetXYZ(0.,0.,0.);
  fDirCos.SetXYZ(0.,0.,0.);
  fPsVertex.SetXYZ(0.,0.,0.);
}

//_________________________________________________________________________________________
inline Double_t TA2TrackLine::Angle(const TVector3 &v) const
{
  // Spatial angle between the track and a 3D vector = v - fOrigin
  return fDirCos.Angle(v-fOrigin);
}

//_________________________________________________________________________________________
inline Double_t TA2TrackLine::Angle(const TA2TrackLine &tr) const
{
  // Spatial angle between this track and tr
  return fDirCos.Angle(tr.fDirCos);
}

//_________________________________________________________________________________________
inline Double_t TA2TrackLine::Angle(const TVector2 &v) const
{
  // Angle between XY projection of the track and a 2D vector vec = v - fOrigin.XYvector()
  // If vec crosses a circle having radius |v| then return angle-pi
  
  //
  Double_t cosMin = v.Mod()/fOrigin.Perp();
  Double_t cosPhiOrig = TMath::Cos( fOrigin.Phi() - v.Phi() );
  //
  Double_t dPhi = fDirCos.XYvector().DeltaPhi(v-fOrigin.XYvector());
  //
  return ( cosPhiOrig < cosMin ) ? dPhi : TVector2::Phi_mpi_pi( dPhi - kPi );
}

//_________________________________________________________________________________________
inline TVector3 TA2TrackLine::Vertex(const TA2TrackLine &tr) const
{
  //
  Double_t dist = -1000.;
  return TA2Math::Vertex(fOrigin, fDirCos, tr.GetOrigin(), tr.GetDirCos(), dist);
}

//_________________________________________________________________________________________
inline TVector3 TA2TrackLine::Vertex(const TA2TrackLine &tr, Double_t &dist) const
{
  //
  return TA2Math::Vertex(fOrigin, fDirCos, tr.GetOrigin(), tr.GetDirCos(), dist);
}

#endif
