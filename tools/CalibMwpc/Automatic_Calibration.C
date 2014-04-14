//////////////////////////////////////////////////
// Author: S.Costanza - June 2013               //
// Methods from TA2CylMwpc.cc by A.Mushkarenkov //
//////////////////////////////////////////////////

#include <iostream>

const Int_t fNch = 2;
Double_t mwpc_params[2][14];
ifstream fin;
ofstream fout;


// ===============================================
void CalibEI(const Int_t ii)
{
  cout << "=== CalibEI === " << endl;
  // ii - # of the MWPC
  TH1D *h1[fNch];

  TString i;
  stringstream ss;
  ss << ii;
  ss >> i;  
  
  // dPhiEI
  h1[ii] = (TH1D*)gDirectory->Get("GeomCalib_DphiEI"+i);
  Double_t binmax = h1[ii]->GetMaximumBin();
  Double_t bincenter = h1[ii]->GetBinCenter(binmax);
  Double_t rms = h1[ii]->GetRMS();
  h1[ii]->Fit("gaus","","", bincenter-0.2*rms, bincenter+0.3*rms);
  Double_t parfitEI[fNch][3];
  gaus->GetParameters(parfitEI[ii]);
  if (parfitEI[ii][1]>parfitEI[ii][2])
    mwpc_params[ii][0] = parfitEI[ii][1];  
}


// ===============================================
void CalibMwpcNaI(const Int_t ii, const Int_t opt, Int_t niter)
{

  cout << "=== CalibMwpcNaI ===" << endl;
  TH1D *h1[fNch];
  TH2D *h2[fNch];
  TGraphErrors *grMean[fNch], *grSigma[fNch];

  TString i;
  stringstream ss;
  ss << ii;
  ss >> i;
  
  switch (opt)
    {
    case 1:
      // dPhi Mwpc-NaI
      h1[ii] = (TH1D*)gDirectory->Get("GeomCalib_DphiMwpc"+i+"NaI");
      Double_t binmax = h1[ii]->GetMaximumBin();
      Double_t bincenter = h1[ii]->GetBinCenter(binmax);
      Double_t rms = h1[ii]->GetRMS();
      h1[ii]->Fit("gaus","","",bincenter-0.5*rms, bincenter+0.5*rms);
      Double_t parfitMwpcNaI[fNch][3];
      gaus->GetParameters(parfitMwpcNaI[ii]);
      mwpc_params[ii][3] += parfitMwpcNaI[ii][1];
      break;
    case 2:
      // dZ vs Zinter
      h2[ii] = (TH2D*)gDirectory->Get("GeomCalib_DzInterMwpc"+i+"NaI_v_ZinterMwpc"+i+"NaI");
      SliceH2(h2[ii],"y",20,-50.,50.,1.,grMean[ii],grSigma[ii]);
      grMean[ii]->Fit("pol1","","",-280.,280.);
      Double_t parfitMwpcNaI2[fNch][2];
      pol1->GetParameters(parfitMwpcNaI2[ii]);
      if (niter==3) {
	mwpc_params[ii][6] = parfitMwpcNaI2[ii][0];
	mwpc_params[ii][7] = parfitMwpcNaI2[ii][1];
      }
      else if (niter==4) {
	mwpc_params[ii][8] = parfitMwpcNaI2[ii][0];
	mwpc_params[ii][9] = parfitMwpcNaI2[ii][1];
      }
      break;
  }
}


// ===============================================
void CalibMwpc(const Int_t opt) {

  cout << "=== CalibMwpc ===" << endl;
  TH1D *h1;
  TH2D *h2;
  TGraphErrors *grMean, *grSigma;

  switch (opt)
    {
    case 1:
      // dPhi MWPC1 - MWPC0
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DphiMwpc21");
      Double_t binmax = h1->GetMaximumBin();
      Double_t bincenter = h1->GetBinCenter(binmax);
      Double_t rms = h1->GetRMS();
      h1->Fit("gaus","","",bincenter-0.5*rms, bincenter+0.5*rms);
      Double_t parfitMwpc[3];
      gaus->GetParameters(parfitMwpc);
      mwpc_params[0][3] += parfitMwpc[1];
      break;
    case 2:
      // dZ vs Zinter
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DzInterMwpc1_v_ZinterMwpc1");
      SliceH2(h2,"y",20,-10.,10.,1.,grMean,grSigma);
      grMean->Fit("pol1");
      Double_t parfitMwpc2[2];
      pol1->GetParameters(parfitMwpc2);
      mwpc_params[0][10] = parfitMwpc2[0];
      mwpc_params[0][11] = parfitMwpc2[1];
      break;
    }
}


// ===============================================
void SliceH2(
  // input
  const TH2 *h, const TString &axis, const Int_t n, Double_t fitMin, Double_t fitMax, const Double_t &factorSigma,
  // output
  TGraphErrors &*grMean, TGraphErrors &*grSigma)
{
  // Slice the h
  // Fit each slice to the gaus
  
  // Arrays for the output graphs
  Double_t x[n]    = {0.};
  Double_t errX[n] = {0.};
  Double_t sigma[n]    = {0.};
  Double_t errSigma[n] = {0.};
  Double_t mean[n]    = {0.};
  Double_t errMean[n] = {0.};
  
  // Get # of bin in the h
  Int_t nbins = 0;
  TAxis *axisPr, *axisSliced;
  if (axis != "y")
  {
    cout << "X-projections" << endl;
    nbins = h->GetNbinsY();
    axisSliced = h->GetYaxis();
    axisPr = h->GetXaxis();
  }
  else
  {
    cout << "Y-projections" << endl;
    nbins = h->GetNbinsX();
    axisSliced = h->GetXaxis();
    axisPr = h->GetYaxis();
  }
  
  // Slice step
  Int_t step = 0;
  if ( n != 0 ) step = ceil(Double_t(nbins)/n);
  
  // Bin counter. The first bin = 1
  Int_t i = 1;
  
  // # of points in the output graphs
  Int_t nPoints = 0;
  
  // Make the slices
  TH1D *hTmp = NULL;
  TCanvas *cTmp = new TCanvas();
  Char_t buf[10];
  Double_t xTmp;
  TString nameTmp;
  Int_t iUp = (step != 0) ? i+step-1 : nbins+1;
  while ( iUp <= nbins )
  {
    xTmp = (axisSliced->GetBinLowEdge(i) + axisSliced->GetBinUpEdge(iUp))/2.;
    sprintf(buf,"%d",xTmp);
    nameTmp = TString(axisSliced->GetTitle())+" = "+TString(buf);
    if (axis != "y") hTmp = h->ProjectionX(nameTmp, i, iUp, "e");
    else             hTmp = h->ProjectionY(nameTmp, i, iUp, "e");
    hTmp->Draw();
    if (fitMin != fitMax) hTmp->Fit("gaus","","",fitMin,fitMax);
//     cTmp->WaitPrimitive();
    if ( hTmp->GetFunction("gaus") )
    {
      x[nPoints] = xTmp;
      sigma[nPoints]    = hTmp->GetFunction("gaus")->GetParameter(2)/factorSigma;
      errSigma[nPoints] = hTmp->GetFunction("gaus")->GetParError(2)/factorSigma;
      mean[nPoints]     = hTmp->GetFunction("gaus")->GetParameter(1);
      errMean[nPoints]  = hTmp->GetFunction("gaus")->GetParError(1);
      if ( errSigma[nPoints] < 99999.9 ) ++nPoints;
    }
    i += step;
    iUp = i+step-1;
    delete hTmp;
  }
  
  // "Sigma" graph
  grSigma = new TGraphErrors(nPoints,x,sigma,errX,errSigma);
  
  // "Mean" graph
  grMean = new TGraphErrors(nPoints,x,mean,errX,errMean);
  
  delete cTmp;
}



// ===============================================
void Automatic_Calibration(Int_t niter) {

  cout << "Running Automatic_Calibration.C -- iteration " << niter << endl;
  for (int i=0; i<fNch; i++) 
    for (int ii=0; ii<14; ii++) 
      mwpc_params[i][ii] = 0.;

  TString filename = "nsummed.root";
  TFile *InputFile = new TFile(filename);

  TString input_pars = "mwpc_params.dat";
  fin.open(input_pars, ios::in);
  Double_t p0=0, p1=0, p2=0, p3=0, p4=0, p5=0, p6=0, p7=0,
    p8=0, p9=0, p10=0, p11=0, p12=0, p13=0;
  for (Int_t i=0; i<fNch; i++) {
    fin >> p0 >> p1 >> p2 >> p3 >> p4 >> p5 >> p6 >> p7
	>> p8 >> p9 >> p10 >> p11 >> p12 >> p13;
    mwpc_params[i][0]  = p0;
    mwpc_params[i][1]  = p1;
    mwpc_params[i][2]  = p2;
    mwpc_params[i][3]  = p3;
    mwpc_params[i][4]  = p4;
    mwpc_params[i][5]  = p5;
    mwpc_params[i][6]  = p6;
    mwpc_params[i][7]  = p7;
    mwpc_params[i][8]  = p8;
    mwpc_params[i][9]  = p9;
    mwpc_params[i][10] = p10;
    mwpc_params[i][11] = p11;
    mwpc_params[i][12] = p12;
    mwpc_params[i][13] = p13;
  }
  fin.close();

  TString input_pars = "mwpc_params.dat";
  fout.open(input_pars, ios::out);
  for (Int_t i=0; i<fNch; i++ ) 
    {
      switch (niter) {
      case 0:
	CalibEI(i);
	break;
      case 1:
	CalibMwpcNaI(i, 1, niter);
	break;
      case 2: 
	if (i==0) 
	  CalibMwpc(1);
	break;
      case 3:
	CalibMwpcNaI(i, 2, niter);
	break;
      case 4:
	CalibMwpcNaI(i, 2, niter);
	break;
      case 5:
	CalibMwpc(2);
      }
      
      for (Int_t npars = 0; npars < 14; npars++) {
	fout << mwpc_params[i][npars] << "\t" ;
	if (npars==13)
	  fout << endl;
	cout << mwpc_params[i][npars] << "\t" ;
	if (npars==13)
	  cout << endl;
      }
    }
  
  fout.close();
  InputFile->Close();

}
      
