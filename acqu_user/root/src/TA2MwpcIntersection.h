#ifndef __TA2MwpcIntersection_h__
#define __TA2MwpcIntersection_h__

// ROOT
#include <TVector3.h>

// AcquRoot
#include <EnumConst.h>

class TA2MwpcIntersection : public TObject {
  public:
    // Constants for possible intersection types
    enum EType {
      k0,	// 000
      kI,	// 001
      kW,	// 010
      kWI,	// 011
      kE,	// 100
      kEI,	// 101
      kEW,	// 110
      kEWI	// 111
    };
  protected:
    TVector3	fPosition;	// Position of the intersection in 3D space
    Int_t	fIclI;		// Cluster index of internal strip layer
    Int_t	fIclW;		// Cluster index of wire layer
    Int_t	fIclE;		// Cluster index of external strip layer
    EType	fType;		// Type of intersection
    Double_t	fDphiWIE;	// dPhi between wire and internal-external strip clusters intersection
    Double_t	fAclIE;		// Average strip cluster pulse amplitudes associated with the intersection
  public:
    TA2MwpcIntersection() : fPosition(TVector3(0,0,0)), fIclI(ENullHit), fIclW(ENullHit), fIclE(ENullHit), fType(k0), fDphiWIE(ENullFloat), fAclIE(ENullFloat) {}
    virtual ~TA2MwpcIntersection() {}
    void Set(const Double_t&, const Double_t&, const Double_t&, const Int_t, const Int_t, const Int_t, const EType, const Double_t, const Double_t);
    const TVector3 *GetPosition() const { return &fPosition; }
    Double_t GetPhi() const { return fPosition.Phi(); }
    Double_t GetZ() const { return fPosition.Z(); }
    Int_t GetIclI() const { return fIclI; }
    Int_t GetIclW() const { return fIclW; }
    Int_t GetIclE() const { return fIclE; }
    EType GetType() const { return fType; }
    Double_t GetDphiWIE() const { return fDphiWIE; }
    Double_t GetAclIE() const { return fAclIE; }
//     Bool_t IsTrue() const { return fType == kEWI; }
    
    ClassDef(TA2MwpcIntersection,1) // MWPC wire-strip-strip intersection
};

//_______________________________________________________________________________________
inline void TA2MwpcIntersection::Set(const Double_t &x, const Double_t &y, const Double_t &z, const Int_t iClI, const Int_t iClW, const Int_t iClE, const EType type, const Double_t dPhiWIE, const Double_t a)
{
  // Set intersection
  
  fPosition.SetXYZ(x,y,z);
  fIclI = iClI;
  fIclW = iClW;
  fIclE = iClE;
  fType = type;
  fDphiWIE = dPhiWIE;
  fAclIE = a;
}

#endif
