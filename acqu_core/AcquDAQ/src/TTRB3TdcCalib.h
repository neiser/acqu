#ifndef TTRB3TDCCALIB_H
#define TTRB3TDCCALIB_H

#include <Rtypes.h>
#include <TTRB3TdcHit.h>

// store calibration information and provide routine
// to obtain a "calibrated" time from the raw data
// based on the stored information

/**
 * @brief The TTRB3TdcCalib class
 */
class TTRB3TdcCalib {
public:
  TTRB3TdcCalib() : IsValid(kFALSE) {}
  Bool_t IsValid;
  Double_t StartBin;
  Double_t EndBin;
  Double_t ClockCycle;
  /**
   * @brief Very simple linear calibration of fine time hits
   * @param h the single TDC Hit to be calibrated
   * @return Calibrated time in ns
   */
  Double_t GetCalibratedTime(const TTRB3TdcHit* h) {
    return ClockCycle*(
          // we assume 11 bits as length of coarse time
          // don't use bit shift due to overflow
          (Double_t)(h->EpochCounter)*pow(2.0,11) +
          (Double_t)(h->CoarseTime)
          // note we need to SUBTRACT the fine time (see TRB3 docu)
          // here in units of fClockCycle the fine time is between 0 and 1
          - ((Double_t)(h->FineTime) - StartBin)/(EndBin-StartBin)
          );
  }
};

#endif // TTRB3TDCCALIB_H
