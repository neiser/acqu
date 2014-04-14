#ifndef __TA2CylMwpcStrip_h__
#define __TA2CylMwpcStrip_h__

#include "TA2CylMwpcLayer.h"

class TA2CylMwpcStrip : public TA2CylMwpcLayer {
  protected:
    Double_t  fRadius;		// radius mm
    Double_t  fTgWC;		// tan helix angle
    Double_t  fPitch;		// strip pitch mm
    Double_t  fLength;		// length mm
    Double_t  fEffLength;	// effective 1/2 length mm
    Double_t *fClustEn;		// Summed amplitude of strips contributing to a cluster
  public:
    TA2CylMwpcStrip(const char*, Int_t, Int_t, Int_t, void*, Double_t*);
    virtual void CGCluster(const Int_t);
    virtual void CGClusters();
    virtual Bool_t IsInside(const Double_t) const;
    virtual Double_t  GetRadius() const { return fRadius; }
    virtual Double_t  GetTgWC() const { return fTgWC; }
    virtual Double_t  GetPitch() const { return fPitch; }
    virtual Double_t  GetLength() const { return fLength; }
    virtual Double_t  GetEffLength() const { return fEffLength; }
    virtual Double_t *GetClustEn() const { return fClustEn; }
    virtual Double_t  GetClustEn(const Int_t i) const { return fClustEn[i]; }
    enum EHelicity {
      kPlus  = 1,
      kMinus = -1
    };
    virtual Double_t CGClusterFromPhiZ(const Double_t&, const Double_t&, const EHelicity); // Find CG from known Phi and Z
    
    ClassDef(TA2CylMwpcStrip,1)	// Strip layer for cylindrical MWPC
};

//__________________________________________________________________________________
inline void TA2CylMwpcStrip::CGCluster(const Int_t ic)
{
  // Amplitude-weighted Center of gravity of a cluster of adjacent hit strips
  // where fEnergy records the induced amplitude on each strip
  
  Double_t esum = 0.;
  Double_t wesum = 0.;
  Int_t istart = fClust[ic];                    // start cluster index
  Int_t iend = istart + fLenClust[ic];          // end cluster index
  Int_t j; 
  
  for (Int_t i=istart; i<iend; ++i)
  {
    Int_t ix = i; //added pedro
    if (i >= fNElement) ix = i - fNElement ; //added pedro
    j = ix + fIstart;
    //
    if (fEnergy[j] == static_cast<Double_t>(ENullHit)) continue;
    esum  +=   fEnergy[j];
    wesum += i*fEnergy[j];
  }
  
  // Energy weighted "index"....check its within bounds and save
  wesum = wesum/esum;
  if ( wesum >= fNElement ) wesum -= fNElement;
  if ( wesum < 0 ) wesum += fNElement;
  fCGClust[ic] = wesum/fNElement;
  fClustEn[ic] = esum;
}

//__________________________________________________________________________________
inline void TA2CylMwpcStrip::CGClusters()
{
  // Center of gravity of all clusters
  // For multi-value variables add end-marker to storage buffer
  
  for (Int_t i=0; i<fNClust; ++i)
  {
    CGCluster(i);
  }
  
  //
  fClust[fNClust] = EBufferEnd;
  fLenClust[fNClust] = EBufferEnd;
  fCGClust[fNClust] = EBufferEnd;
  fClustEn[fNClust] = EBufferEnd;
}

//__________________________________________________________________________________
inline Bool_t TA2CylMwpcStrip::IsInside(const Double_t z) const
{
  // Is Z coordinate sensible for this layer
  
  if ( z < -fEffLength ) return kFALSE;
  if ( z >  fEffLength ) return kFALSE;
  return kTRUE;
}

#endif
