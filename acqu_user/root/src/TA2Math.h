#ifndef __TA2Math_h__
#define __TA2Math_h__

#include <TMath.h>
#include <TVector2.h>
#include <TLorentzVector.h>

// Global constants
static const Double_t kPi       = TMath::Pi();
static const Double_t kRadToDeg = TMath::RadToDeg();
static const Double_t kDegToRad = TMath::DegToRad();

namespace TA2Math {
  
  //_________________________________________________________________________________________________
  // Returns absolute phi in [0; Pi]
  inline Double_t AbsPhi(const Double_t &phi)
  {
    return TMath::Abs(TVector2::Phi_mpi_pi(phi));
  }
  
  //_________________________________________________________________________________________________
  // Calculate the shortest distance beetween 2 lines in 3D space
  // o1 and o2 are vectors to points belonging to the 1st and 2nd lines, respectively
  // d1 and d2 are direction cosines of the 1st and 2nd lines, respectively
  // r1 and r2 are vectors to the intersection with the common perpendicular
  inline Double_t DistanceBetweenTwoLines(const TVector3 &o1, const TVector3 &d1, const TVector3 &o2, const TVector3 &d2, TVector3 &r1, TVector3 &r2)
  { 
    // Parameters
    TVector3 oo  = o2 - o1;
    Double_t m1  = d1 * oo;
    Double_t m2  = d2 * oo;
    Double_t t12 = d1 * d2;
    Double_t t1  = d1.Mag2();
    Double_t t2  = d2.Mag2();
    //
    Double_t ttt = t2*t1 - t12*t12;
    Double_t t = ( m1*t2 - t12*m2)/ttt; // t
    Double_t s = (-m2*t1 + t12*m1)/ttt; // s
    
    // Intersection points with the common perpendicular
    r1 = o1 + d1 * t;
    r2 = o2 + d2 * s;
    
    // Return the shortest distance beetween the 2 lines
    return (r2-r1).Mag();
  }
  
  //_________________________________________________________________________________________________
  // Calculate vertex for 2 lines (see DistanceBetweenTwoLines(..))
  inline TVector3 Vertex(const TVector3 &o1, const TVector3 &d1, const TVector3 &o2, const TVector3 &d2, Double_t &distance)
  {
    // The shortest distance beetween the 2 lines
    TVector3 r1, r2;
    distance = DistanceBetweenTwoLines(o1, d1, o2, d2, r1, r2);
    
    // Return vertex
    return 0.5*(r1 + r2);
  }
  
  //_________________________________________________________________________________________________
  // Calculate a rotation angle of a q coordinate frame relative to a p coordinate frame.
  // Assumed that the points p1, p2 and q1, q2 lie on the same line
  inline Double_t CalcRotation2D(Double_t p1[2], Double_t p2[2], Double_t q1[2], Double_t q2[2])
  {
    // Vector in p frame
    TVector2 p = TVector2(p2) - TVector2(p1);
    
    // Vector in q frame
    TVector2 q = TVector2(q2) - TVector2(q1);
    
    // Rotation angle
    return q.DeltaPhi(p);
  }
  
  //_________________________________________________________________________________________________
  // Calculate a shift of a 2D q coordinate frame relative to a 2D p coordinate frame.
  // Assumed that the q frame is not rotated relative to the p frame.
  // Assumed that the points p1, p2 and q1, q2 lie on the same line
  inline void CalcShift2D(const Double_t p1[2], const Double_t p2[2], const Double_t q1[2], const Double_t q2[2], Double_t shift[2], Double_t p0[2], Double_t q0[2])
  {
    // Tangents of angle of inclinations for the lines q1q2 and p1p2
    Double_t tgQ = (q1[1]-q2[1])/(q1[0]-q2[0]);
    Double_t tgP = (p1[1]-p2[1])/(p1[0]-p2[0]);
    
    // q0 = {x0,y0},
    // where
    // x0 is the x coordinate of OX and q1q2 intersection,
    // y0 is the y coordinate of OY and q1q2 intersection.
//     Double_t q0[2];
    q0[1] =  q1[1] - tgQ*q1[0];
    q0[0] = -q0[1]/tgQ;
    
    // p0 = {x0,y0},
    // where
    // x0 is the x coordinate of OX and p1p2 intersection,
    // y0 is the y coordinate of OY and p1p2 intersection.
//     Double_t p0[2];
    p0[1] =  p1[1] - tgP*p1[0];
    p0[0] = -p0[1]/tgP;
    
    // Result
    shift[0] = q0[0] - p0[0];
    shift[1] = q0[1] - p0[1];
  }
  
  //_________________________________________________________________________________________________
  // Claculate 4-vector (TLorentzVector)
  // d - direction cosines
  // e - kinetic energy (must be >= 0)
  // m - mass (must be >= 0)
  inline TLorentzVector LorentzVector(const TVector3 &d, Double_t e, const Double_t &m)
  {
    e += m;
    return TLorentzVector(TMath::Sqrt(e*e-m*m)*d,e);
  }
  
  //_________________________________________________________________________________________________
  // Find intersection points r1 and r2 of a line formed by v1 and v2 with a cylinder having radius r
  // Returns number of solutions
  inline Int_t IntersectLineAndCylinder(const TVector3 &v1, const TVector3 &v2, const Double_t &r, TVector3 &r1, TVector3 &r2)
  {
    //
    TVector2  v1xy = v1.XYvector();
    TVector3     d = v2 - v1;
    TVector2   dxy = d.XYvector();
    
    //
    Double_t a = v1xy.Mod2() - r*r;
    Double_t b = dxy.Mod2();
    Double_t c = v1xy*dxy;
    Double_t dmt4 = c*c - b*a;
    
    // Check if there is a solution
    if ( dmt4 < 0. ) return 0;
    
    // Solutions
    Double_t t1 = (-c - TMath::Sqrt(dmt4))/b;
    Double_t t2 = (-c + TMath::Sqrt(dmt4))/b;
    
    // Result
    r1 = v1 + d*t1;
    r2 = v1 + d*t2;
    
    if ( t1 == t2 ) return 1;
    else            return 2;
  }
  
}

#endif
