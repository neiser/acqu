#ifndef __TA2MwpcTrack_h__
#define __TA2MwpcTrack_h__

// AcquRoot
#include "EnumConst.h"
#include "TA2TrackLine.h"

class TA2MwpcTrack : public TA2TrackLine {
  protected:
    static const Int_t kNullHit = static_cast<Int_t>(ENullHit);
    Int_t fIinters[2];	// Indexes of intersections in the internal (0) and external (1) MWPC
  public:
    TA2MwpcTrack();
    virtual ~TA2MwpcTrack() {}
    virtual void Reset();
    const Int_t &GetIinter(const Int_t) const;
    const Int_t *GetIinters() const;
    static Int_t CalcTrackType(const Int_t, const Int_t);
    void SetType(const Int_t, const Int_t);
    void SetIinters(const Int_t, const Int_t);
    void SetIinters(const Int_t*);
    void SetIinter(const UChar_t, const Int_t);
    virtual Bool_t BuildTrack(const TVector3&, const TVector3&, const Double_t&, const Double_t*, const Int_t*, const Int_t*);
    
    ClassDef(TA2MwpcTrack,1) // MWPC track
};

//_________________________________________________________________________________________
inline void TA2MwpcTrack::Reset()
{
  // Set default track info
  
  // Reset inherited data-members
  TA2TrackLine::Reset();
  
  //
  fIinters[0] = fIinters[1] = kNullHit;
}

//_________________________________________________________________________________________
inline Bool_t TA2MwpcTrack::BuildTrack(const TVector3 &r0, const TVector3 &r1, const Double_t &maxDirCosMag, const Double_t *limitsPsVertex, const Int_t *i, const Int_t *type)
{
  // Try to build a MWPC track and if it fails return FALSE
  if (TA2TrackLine::BuildTrack(r0,r1,maxDirCosMag,limitsPsVertex))
  {
    SetIinters(i);
    SetType(type[0],type[1]);
    return kTRUE;
  }
  else
    return kFALSE;
}

//_________________________________________________________________________________________
inline void TA2MwpcTrack::SetIinters(const Int_t i0, const Int_t i1)
{
  fIinters[0] = i0;
  fIinters[1] = i1;
}

//_________________________________________________________________________________________
inline void TA2MwpcTrack::SetIinters(const Int_t* i)
{
  fIinters[0] = i[0];
  fIinters[1] = i[1];
}

//_________________________________________________________________________________________
inline void TA2MwpcTrack::SetIinter(const UChar_t iCh, const Int_t i)
{
  fIinters[iCh] = i;
}

//_________________________________________________________________________________________
inline const Int_t &TA2MwpcTrack::GetIinter(const Int_t iCh) const
{
  return fIinters[iCh];
}

//_________________________________________________________________________________________
inline const Int_t *TA2MwpcTrack::GetIinters() const
{
  return fIinters;
}

//_________________________________________________________________________________________
inline Int_t TA2MwpcTrack::CalcTrackType(const Int_t typeInter0, const Int_t typeInter1)
{
  // Calculate track type
  // bit = 0(no), = 1(yes)
  //
  // bit#	detector			param
  // 0		MWPC0 internal strip layer
  // 1		MWPC0 wires layer		typeInterMwpc0 (for 0 to 2)
  // 2		MWPC0 external strip layer
  // 3		MWPC1 internal strip layer
  // 4		MWPC1 wires layer		typeInterMwpc1 (for 3 to 5)
  // 5		MWPC1 external strip layer
  
  return typeInter1 << 3 | typeInter0;
}

//_________________________________________________________________________________________
inline void TA2MwpcTrack::SetType(const Int_t typeInter0, const Int_t typeInter1)
{
  // Set track type
  fType = CalcTrackType(typeInter0,typeInter1);
}

#endif
