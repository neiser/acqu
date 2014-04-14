#ifndef __TA2CylMwpcWire_h__
#define __TA2CylMwpcWire_h__

#include "TA2CylMwpcLayer.h"
#include "TA2Math.h"

class TA2CylMwpcWire : public TA2CylMwpcLayer {
  protected:
    Double_t fRadius;		// radius mm
    Double_t fLength;		// length mm
    Double_t fPhiSpace;		// phi spacing of wires
  public:
    TA2CylMwpcWire(const char*, Int_t, Int_t, Int_t, void*, const Double_t*);
    virtual void CGCluster(const Int_t);
    virtual Double_t GetRadius() const { return fRadius; }
    virtual Double_t GetLength() const { return fLength; }
    
    ClassDef(TA2CylMwpcWire,1) // Wire layer for cylindrical MWPC
};

//---------------------------------------------------------------------------
inline void TA2CylMwpcWire::CGCluster(const Int_t ic)
{
  // Center of gravity of cluster (index ic) of adjacent hit wires
  // No pulse height to weight the distribution.
  // Convert wire # to phi
  
  fCGClust[ic] = fPhiSpace * ( fClust[ic] + 0.5*(fLenClust[ic] - 1) );
  if ( fCGClust[ic] >= 2.*kPi ) fCGClust[ic] -= 2.*kPi; // TODO May not need!
  else if ( fCGClust[ic] < 0. ) fCGClust[ic] += 2.*kPi; // TODO May not need!
}

#endif
