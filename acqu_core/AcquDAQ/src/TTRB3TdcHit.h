#ifndef TTRB3TDCHIT_H
#define TTRB3TDCHIT_H

#include <iostream>

// store raw TRB3 TDC hits within this class 
class TTRB3TdcHit {
public:
  TTRB3TdcHit() : Channel(-1), EpochCounter(0), CoarseTime(0), FineTime(0),
    IsRisingEdge(kFALSE), CalibratedTime(0) {}
  Int_t Channel;
  UInt_t EpochCounter;
  UInt_t CoarseTime;
  UInt_t FineTime;
  Bool_t IsRisingEdge;
  Double_t CalibratedTime;
  friend std::ostream& operator<< (std::ostream&, const TTRB3TdcHit&);
};

inline std::ostream& operator<< (std::ostream& out, const TTRB3TdcHit& t) {
  out << "TDC Hit: Ch " << t.Channel << ", "
      << t.EpochCounter << "/" << t.CoarseTime 
      << "/" << t.FineTime;   
  return out;
}

#endif // TTRB3TDCHIT_H
