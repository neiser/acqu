// SVN info: $Id: GeomCalibCentralApparatus.C 124 2013-05-28 02:49:45Z mushkar $

//
TFile *gFileAR = NULL;

//___________________________________________________________________________________
void CalibEI(const TString &i)
{
  //
  
  TControlBar* bar = new TControlBar("vertical", "Intrinsic MWPC-"+i+" calibration");
  bar->AddButton("dPhiEI", "CalibEI("+i+",1)", "dPhi shift E wrt I");
  bar->AddButton("dPhiEI(Z)", "CalibEI("+i+",2)", "dPhi shift E wrt I as a func of Z");
  bar->AddButton("dPhiEI(Phi)", "CalibEI("+i+",3)", "dPhi shift E wrt I as a func of Phi");
  bar->AddButton("dPhiEI(Phi,Z)", "CalibEI("+i+",23)", "dPhi shift E wrt I as a func of Phi and Z");
  bar->AddButton("dPhiEI(CGW)", "CalibEI("+i+",4)", "dPhi shift E wrt I as a func of CGW");
  bar->AddButton("dPhiEI(CGI)", "CalibEI("+i+",5)", "dPhi shift E wrt I as a func of CGI");
  bar->AddButton("dPhiEI(CGE)", "CalibEI("+i+",6)", "dPhi shift E wrt I as a func of CGE");
  bar->SetButtonWidth(150);
  bar->Show();
}

//___________________________________________________________________________________
void CalibMwpc()
{
  //
  
  TControlBar* bar = new TControlBar("vertical", "MWPC-1 wrt MWPC-0 calibration");
  bar->AddButton("dPhi",		"CalibMwpc(1, 0, 0, 0, 0)", "dPhi MWPC1 - MWPC0");
  bar->AddButton("dX",			"CalibMwpc(2, 0, 0, 0, 0)", "dX MWPC1 - MWPC0");
  bar->AddButton("dY",			"CalibMwpc(3, 0, 0, 0, 0)", "dY MWPC1 - MWPC0");
  bar->AddButton("dZ",			"CalibMwpc(4, 0, 0, 0, 0)", "dZ MWPC1 - MWPC0");
  bar->AddButton("dPhi tracks",		"CalibMwpc(5, 0, 0, 0, 0)", "dPhi tracks 1-2");
  bar->AddButton("dTheta tracks",	"CalibMwpc(6, 0, 0, 0, 0)", "dTheta tracks 1-2");
  bar->AddButton("dTheta2 tracks2",	"CalibMwpc(66, 0, 0, 0, 0)", "dTheta tracks 1-2");
  bar->AddButton("dPsi tracks",		"CalibMwpc(7, 0, 0, 0, 0)", "dPsi tracks 1-2");
  bar->AddButton("dZinter vs Zinter",	"CalibMwpc(8, 0, 0, 0, 0)", "dZ vs Zinter");
  bar->AddButton("dXinter vs Zinter",	"CalibMwpc(9, 0, 0, 0, 0)", "dX vs Zinter");
  bar->AddButton("dYinter vs Zinter",	"CalibMwpc(10, 0, 0, 0, 0)", "dY vs Zinter");
  bar->AddButton("dZinter vs PhiInter",	"CalibMwpc(11, 0, 0, 0, 0)", "dZinter vs PhiInter");
  bar->AddButton("dXinter vs PhiInter",	"CalibMwpc(12, 0, 0, 0, 0)", "dXinter vs PhiInter");
  bar->AddButton("dYinter vs PhiInter",	"CalibMwpc(13, 0, 0, 0, 0)", "dYinter vs PhiInter");
  bar->AddButton("dPhi vs PhiInter",	"CalibMwpc(14, 0, 0, 0, 0)", "dPhi vs PhiInter");
  bar->AddButton("dPhi vs ZInter",	"CalibMwpc(15, 0, 0, 0, 0)", "dPhi vs ZInter");
  bar->SetButtonWidth(150);
  bar->Show();
}

//___________________________________________________________________________________
void CalibMwpcNaI(const TString &i)
{
  //
  
  TControlBar* bar = new TControlBar("vertical", "MWPC-"+i+" wrt NaI");
  bar->AddButton("dPhi", "CalibMwpcNaI("+i+",1,0,0,0,0)", "dPhi Mwpc-NaI");
  bar->AddButton("dX", "CalibMwpcNaI("+i+",2,0,0,0,0)", "X shift");
  bar->AddButton("dY", "CalibMwpcNaI("+i+",3,0,0,0,0)", "Y shift");
  bar->AddButton("dZ", "CalibMwpcNaI("+i+",4,0,0,0,0)", "Z shift");
  bar->AddButton("dPhi tracks", "CalibMwpcNaI("+i+",5,0,0,0,0)", "dPhi between tracks");
  bar->AddButton("dTheta tracks", "CalibMwpcNaI("+i+",6,0,0,0,0)", "dTheta between tracks");
  bar->AddButton("dPsi tracks", "CalibMwpcNaI("+i+",7,0,0,0,0)", "dPsi between tracks");
  bar->AddButton("dZinter vs Zinter", "CalibMwpcNaI("+i+",8,0,0,0,0)", "");
  bar->AddButton("dXinter vs Zinter", "CalibMwpcNaI("+i+",9,0,0,0,0)", "");
  bar->AddButton("dYinter vs Zinter", "CalibMwpcNaI("+i+",10,0,0,0,0)", "");
  bar->AddButton("dZinter vs PhiInter", "CalibMwpcNaI("+i+",11,0,0,0,0)", "");
  bar->AddButton("dXinter vs PhiInter", "CalibMwpcNaI("+i+",12,0,0,0,0)", "");
  bar->AddButton("dYinter vs PhiInter", "CalibMwpcNaI("+i+",13,0,0,0,0)", "");
  bar->AddButton("dPhiInter vs PhiInter", "CalibMwpcNaI("+i+",14,0,0,0,0)", "");
  bar->AddButton("dPhiInter vs Zinter", "CalibMwpcNaI("+i+",15,0,0,0,0)", "");
  bar->SetButtonWidth(150);
  bar->Show();
}

//___________________________________________________________________________________
void CalibEI(const Int_t ii, const Int_t opt)
{
  // i - # of the MWPC
  // opt - option to plot
  
  
  TGraphErrors *grMean, *grSigma;
  TCanvas *cnv;
  TH1D *h1;
  TH2D *h2;
  TH3D *h3;
  TLine *lZ = new TLine(-300.,0.,300.,0.);
  lZ->SetLineColor(kBlue);
  lZ->SetLineWidth(2.);
  TLine *lPhi = new TLine(0.,0.,360.,0.);
  lPhi->SetLineColor(kBlue);
  lPhi->SetLineWidth(2.);
  
  //
  TString i;
  stringstream ss;
  ss << ii;
  ss >> i;
  
  //
  switch (opt)
  {
    case 1:
      // dPhiEI
      cnv = new TCanvas("cEI"+i);
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DphiEI"+i);
      h1->Draw();
      h1->Fit("gaus","","",-.6,.6);
      //   hEI->Fit("gaus");
      break;
      
    case 2:
      // dPhiEI(Z)
      cnv = new TCanvas("cEIZ"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiEI"+i+"_v_ZInters"+i);
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-1.,1.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("cEIZ_Sigma"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,0.,300.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("dPhiEI(Z)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("cEIZ_Mean"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,-30.,300.,30.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("dPhiEI(Z)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      break;
      
    case 3:
      // dPhiEI(Phi)
      cnv = new TCanvas("cEIPhi"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiEI"+i+"_v_PhiEI"+i);
      h2->Draw("color");
      lPhi->Draw("same");
      //
      SliceH2(h2,"y",20,-1.,1.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("cEIPhi_Sigma"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("dPhiEI(Phi)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("cEIPhi_Mean"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("dPhiEI(Phi)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      break;
    
    case 23:
      cnv = new TCanvas("cEIPhi_v_Phi_v_Z"+i);
      h3 = (TH3D*)gDirectory->Get("dPhiEI_PhiEI_Zinters_"+i);
      TF2 *f2 = new TF2("f1", "[0]+x*[1]+y*[2]",0.,360.,-250,250.);
      h3->Fit(f2);
      h3->Draw("same");
      break;
      
    case 4:
      // dPhiEI(CGW)
      cnv = new TCanvas("cEIPhi_v_CGW"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiEI"+i+"_v_CGclW"+i);
      h2->Draw("color");
      lPhi->Draw("same");
      break;
      
    case 5:
      // dPhiEI(CGI)
      cnv = new TCanvas("cEIPhi_v_CGI"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiEI"+i+"_v_CGclI"+i);
      h2->Draw("color");
      lPhi->Draw("same");
      break;
      
    case 6:
      // dPhiEI(CGE)
      cnv = new TCanvas("cEIPhi_v_CGE"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiEI"+i+"_v_CGclE"+i);
      h2->Draw("color");
      lPhi->Draw("same");
      break;
      
    default:
      for (Int_t j=1; j<=6; ++j) CalibEI(i,j);
  }
}

//___________________________________________________________________________________
void CalibMwpc(const Int_t opt, TH1D &*h1, TH2D &*h2, TGraphErrors &*grMean, TGraphErrors &*grSigma)
{
  // General style
//   gROOT->ProcessLine(".x Style_Default_2.C");
//   gROOT->SetStyle("Default_2");
//   gROOT->ForceStyle();
  
  //
  TCanvas *cnv;
  TLine *lZ = new TLine(-300.,0.,300.,0.);
  lZ->SetLineColor(kBlue);
  lZ->SetLineWidth(2.);
  TLine *lTheta = new TLine(0.,0.,180.,0.);
  lTheta->SetLineColor(kBlue);
  lTheta->SetLineWidth(2.);
  TLine *lPhi = new TLine(0.,0.,360.,0.);
  lPhi->SetLineColor(kBlue);
  lPhi->SetLineWidth(2.);
  
  //
  switch (opt)
  {
    case 1:
      // dPhi MWPC1 - MWPC0
      cnv = new TCanvas("dPhi");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DphiMwpc21");
      h1->Draw();
      h1->Fit("gaus","","",-1.,1.);
      // dPhi(Z)
      cnv = new TCanvas("dPhi_VS_Z");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiMwpc21_v_Z0Mwpc21");
      h2->Draw("color");
      lZ->Draw("same");
      break;
      
    case 2:
      // dX
      cnv = new TCanvas("dX");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DxMwpc21");
      h1->Draw();
      h1->Fit("gaus","","",-2.,2.);
      // dX(Z)
      cnv = new TCanvas("dX_VS_Z");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DxMwpc21_v_Z0Mwpc21");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-5.,5.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDx_vs_Z");
      h1 = (TH1D*)cnv->DrawFrame(-300.,0.,300.,5.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dX(Z)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDx_vs_Z");
      h1 = (TH1D*)cnv->DrawFrame(-300.,-3.,300.,3.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dX(Z)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 3:
      // dY
      cnv = new TCanvas("dY");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DyMwpc21");
      h1->Draw();
      h1->Fit("gaus","","",-10.,10.);
      // dY(Z)
      cnv = new TCanvas("dY_VS_Z");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DyMwpc21_v_Z0Mwpc21");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-10.,10.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDy_vs_Z");
      h1 = (TH1D*)cnv->DrawFrame(-300.,0.,300.,15.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dY(Z)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDy_vs_Z");
      h1 = (TH1D*)cnv->DrawFrame(-300.,-5.,300.,5.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dY(Z)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 4:
      // dZ
      cnv = new TCanvas("dZ");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DzMwpc21");
      h1->Draw();
      h1->Fit("gaus","","",-3.,3.);
      // dZ(Z)
      cnv = new TCanvas("dZ_VS_Z");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DzMwpc21_v_Z0Mwpc21");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-5.,5.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDz_vs_Z");
      h1 = (TH1D*)cnv->DrawFrame(-300.,0.,300.,5.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dZ(Z)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDz_vs_Z");
      h1 = (TH1D*)cnv->DrawFrame(-300.,-3.,300.,3.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dZ(Z)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      // dZ(Theta)
      cnv = new TCanvas("dZ_VS_Theta");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DzMwpc21_v_ThetaTracks");
      h2->Draw("color");
      lTheta->Draw("same");
      //
      SliceH2(h2,"y",20,-10.,10.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDz_vs_Theta");
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,180.,5.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dZ(Theta)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDz_vs_Theta");
      h1 = (TH1D*)cnv->DrawFrame(0.,-3.,180.,3.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("Theta, degrees");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dZ(Theta)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      break;
      
    case 5:
      // dPhi tracks
      cnv = new TCanvas("dPhiTracks");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DPhiTracks");
      h1->Draw();
      h1->Fit("gaus","","",-10.,10.);
      // dPhi tracks (Theta)
      cnv = new TCanvas("dPhiTracks_v_Theta");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DPhiTracks_v_ThetaTracks");
      h2->Draw("color");
      lTheta->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDphiTracks_v_Theta");
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,180.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("Standard deviation of #Delta#Phi as a function #theta");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDphiTracks_v_Theta");
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,180.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("#Delta#Phi(#theta)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 6:
      // dTheta tracks
      cnv = new TCanvas("dTetTracks");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DTetTracks");
      h1->Draw();
      h1->Fit("gaus","","",-7.,7.);
      // dTheta tracks (Theta)
      cnv = new TCanvas("dTetTracks_v_Theta");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DTetTracks_v_ThetaTracks");
      h2->Draw("color");
      lTheta->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDthetaTracks_v_Theta");
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,180.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("Standard deviation of #Delta#theta as a function of #theta");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDthetaTracks_v_Theta");
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,180.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("#Delta#theta(#theta)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
    
    case 66:
      // dTheta2 tracks2
      cnv = new TCanvas("dTetTracks2");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DTetTracks2");
      h1->Draw();
      h1->Fit("gaus","","",-7.,7.);
      // dTheta tracks (Theta)
      cnv = new TCanvas("dTetTracks2_v_Theta2");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DTetTracks2_v_ThetaTracks2");
      h2->Draw("color");
      lTheta->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDthetaTracks2_v_Theta2");
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,180.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("Standard deviation of #Delta#theta as a function of #theta");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDthetaTracks2_v_Theta2");
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,180.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("#Delta#theta(#theta)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 7:
      // dPsi tracks
      cnv = new TCanvas("dPsiTracks");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DPsiTracks");
      h1->Draw();
      break;
      
    case 8:
      // dZ vs Zinter
      cnv = new TCanvas("DzInterMwpc1");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DzInterMwpc1");
      h1->Draw();
      h1->Fit("gaus","","",-3.,3.);
      //
      cnv = new TCanvas("Dz_vs_Zinter");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DzInterMwpc1_v_ZinterMwpc1");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-10.,10.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDz_vs_Zinter");
      h1 = (TH1D*)cnv->DrawFrame(-400.,0.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dZ(Zinter)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDz_vs_Zinter");
      h1 = (TH1D*)cnv->DrawFrame(-400.,-10.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dZ(Zinter)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      break;
      
    case 9:
      // dX vs Zinter
      cnv = new TCanvas("DxInterMwpc1");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DxInterMwpc1");
      h1->Draw();
      h1->Fit("gaus","","",-2.5,2.5);
      //
      cnv = new TCanvas("Dx_vs_Zinter");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DxInterMwpc1_v_ZinterMwpc1");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-10.,10.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDx_vs_Zinter");
      h1 = (TH1D*)cnv->DrawFrame(-400.,0.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dX(Zinter)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDx_vs_Zinter");
      h1 = (TH1D*)cnv->DrawFrame(-400.,-10.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dX(Zinter)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 10:
      // dY vs Zinter
      cnv = new TCanvas("DyInterMwpc1");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DyInterMwpc1");
      h1->Draw();
      h1->Fit("gaus","","",-.2,.2);
      //
      cnv = new TCanvas("Dy_vs_Zinter");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DyInterMwpc1_v_ZinterMwpc1");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-5.,5.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDy_vs_Zinter");
      h1 = (TH1D*)cnv->DrawFrame(-400.,0.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dY(Zinter)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDy_vs_Zinter");
      h1 = (TH1D*)cnv->DrawFrame(-400.,-10.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dY(Zinter)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 11:
      // dZ vs PhiInter
      cnv = new TCanvas("Dz_vs_PhiInter");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DzInterMwpc1_v_PhiInterMwpc1");
      h2->Draw("color");
      lPhi->Draw("same");
      //
      SliceH2(h2,"y",20,-5.,5.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDz_vs_PhiInter");
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dZ(PhiInter)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDz_vs_PhiInter");
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dZ(PhiInter)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 12:
      // dX vs PhiInter
      cnv = new TCanvas("Dx_vs_PhiInter");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DxInterMwpc1_v_PhiInterMwpc1");
      h2->Draw("color");
      lPhi->Draw("same");
      //
      SliceH2(h2,"y",20,-10.,10.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDx_vs_PhiInter");
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dX(PhiInter)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDx_vs_PhiInter");
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dX(PhiInter)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 13:
      // dY vs PhiInter
      cnv = new TCanvas("Dy_vs_PhiInter");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DyInterMwpc1_v_PhiInterMwpc1");
      h2->Draw("color");
      lPhi->Draw("same");
      //
      SliceH2(h2,"y",20,-5.,5.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDy_vs_PhiInter");
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dY(PhiInter)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDy_vs_PhiInter");
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dY(PhiInter)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 14:
      // dPhi vs PhiInter
      cnv = new TCanvas("dPhiInter");
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DphiInterMwpc1");
      h1->Draw();
      h1->Fit("gaus","","",-1.,1.);
      //
      cnv = new TCanvas("Dphi_vs_PhiInter");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiInterMwpc1_v_PhiInterMwpc1");
      h2->Draw("color");
      lPhi->Draw("same");
      //
      SliceH2(h2,"y",20,-5.,5.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDphi_vs_PhiInter");
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("dPhi(PhiInter)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDphi_vs_PhiInter");
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("dPhi(PhiInter)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 15:
      // dPhi vs Zinter
      cnv = new TCanvas("Dphi_vs_Zinter");
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiInterMwpc1_v_ZinterMwpc1");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-5.,5.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDphi_vs_Zinter");
      h1 = (TH1D*)cnv->DrawFrame(-400.,0.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("dPhi(Zinter)");
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDphi_vs_Zinter");
      h1 = (TH1D*)cnv->DrawFrame(-400.,-10.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("dPhi(Zinter)");
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    default:
      // Plot all
      for (Int_t i=1; i<=20; ++i) CalibMwpc(i,0,0,0,0);
  }
  
}

//___________________________________________________________________________________
void CalibMwpcNaI(const Int_t ii, const Int_t opt, TH1D &*h1, TH2D &*h2, TGraphErrors &*grMean, TGraphErrors &*grSigma)
{
  // General style
//   gROOT->ProcessLine(".x Style_Default_2.C");
//   gROOT->SetStyle("Default_2");
//   gROOT->ForceStyle();
  
  //
  TGraphErrors *grMean, *grSigma;
  TCanvas *cnv;
  TH1D *h1;
  TH2D *h2;
  TLine *lZ = new TLine(-300.,0.,300.,0.);
  lZ->SetLineColor(kBlue);
  lZ->SetLineWidth(2.);
  TLine *lTheta = new TLine(0.,0.,180.,0.);
  lTheta->SetLineColor(kBlue);
  lTheta->SetLineWidth(2.);
  TLine *lPhi = new TLine(0.,0.,360.,0.);
  lPhi->SetLineColor(kBlue);
  lPhi->SetLineWidth(2.);
  
  //
  TString i;
  stringstream ss;
  ss << ii;
  ss >> i;
  
  //
  switch (opt)
  {
    case 1:
      // dPhi Mwpc-NaI
      cnv = new TCanvas("dPhi"+i);
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DphiMwpc"+i+"NaI");
      h1->Draw();
      h1->Fit("gaus","","",-1.8,1.8);
      //   hDphi->Fit("gaus");
      // dPhi(Z)
      cnv = new TCanvas("dPhi_VS_Z"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiMwpc"+i+"NaI_v_Z0Mwpc"+i+"NaI");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-10.,10.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("dPhi_Sigma_vs_Z"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,0.,300.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma_{#Delta#varphi}, degrees");
      h1->SetTitle("dPhi(Z), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("dPhi_Mean_vs_Z"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,-30.,300.,30.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("dPhi(Z), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      break;
      
    case 2:
      // dX
      cnv = new TCanvas("dX"+i);
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DxMwpc"+i+"NaI");
      h1->Draw();
      h1->Fit("gaus","","",-20.,20.);
      // dX(Z)
      cnv = new TCanvas("dX_VS_Z"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DxMwpc"+i+"NaI_v_Z0Mwpc"+i+"NaI");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("dX_Sigma_vs_Z"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,0.,300.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma_{#DeltaX}, mm");
      h1->SetTitle("dX(Z), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("dX_Mean_vs_Z"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,-30.,300.,30.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dX(Z), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 3:
      // dY
      cnv = new TCanvas("dY"+i);
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DyMwpc"+i+"NaI");
      h1->Draw();
      h1->Fit("gaus","","",-60.,60.);
      // dY(Z)
      cnv = new TCanvas("dY_VS_Z"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DyMwpc"+i+"NaI_v_Z0Mwpc"+i+"NaI");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-70.,70.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("dY_Sigma_vs_Z"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,0.,300.,40.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma_{#DeltaY}, mm");
      h1->SetTitle("dY(Z), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("dY_Mean_vs_Z"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,-30.,300.,30.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dY(Z), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 4:
      // dZ
      cnv = new TCanvas("dZ"+i);
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DzMwpc"+i+"NaI");
      h1->Draw();
      h1->Fit("gaus","","",-10.,10.);
      // dZ(Z)
      cnv = new TCanvas("dZ_VS_Z"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DzMwpc"+i+"NaI_v_Z0Mwpc"+i+"NaI");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("dZ_Sigma_vs_Z"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,0.,300.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma_{#DeltaZ}, mm");
      h1->SetTitle("dZ(Z), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("dZ_Mean_vs_Z"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,-30.,300.,30.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dZ(Z), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      // dZ(Theta)
      cnv = new TCanvas("dZ_VS_Theta"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DzMwpc"+i+"NaI_v_ThetaTracksMwpc"+i+"NaI");
      h2->Draw("color");
      lTheta->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("dZ_Sigma_vs_Theta"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,180.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dZ(#theta), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("dZ_Mean_vs_Theta"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,-30.,180.,30.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dZ(#theta), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      break;
      
    case 5:
      // dPhi tracks
      cnv = new TCanvas("dPhiTracks"+i);
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DPhiTracksMwpc"+i+"NaI");
      h1->Draw();
      h1->Fit("gaus","","",-5.,5.);
      // dPhi tracks (Theta)
      cnv = new TCanvas("dPhiTracks_v_Theta"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DPhiTracksMwpc"+i+"NaI_v_ThetaTracksMwpc"+i+"NaI");
      h2->Draw("color");
      lTheta->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("Sigma_dPhiTracks_v_Theta"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,180.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("Standard deviation of #Delta#Phi as a function of #theta, MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("Mean_dPhi_vs_Theta"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,-30.,180.,30.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("#Delta#Phi(#theta), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 6:
      // dTheta tracks
      cnv = new TCanvas("dTetTracks"+i);
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DTetTracksMwpc"+i+"NaI");
      h1->Draw();
      h1->Fit("gaus","","",-3.,3.);
      // dTheta tracks (Theta)
      cnv = new TCanvas("dTetTracks_v_Theta"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DTetTracksMwpc"+i+"NaI_v_ThetaTracksMwpc"+i+"NaI");
      h2->Draw("color");
      lTheta->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,TMath::Sqrt(2.),grMean,grSigma);
      // Sigma
      cnv = new TCanvas("Sigma_dThetaTracks_v_Theta"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,180.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("Standard deviation of #Delta#theta as a function #theta, MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("Mean_dTheta_vs_Theta"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,-30.,180.,30.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#theta, degrees");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("#Delta#theta(#theta), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 7:
      // dPsi tracks
      cnv = new TCanvas("dPsiTracks"+i);
      h1 = (TH1D*)gDirectory->Get("GeomCalib_DPsiTracksMwpc"+i+"NaI");
      h1->Draw();
      break;
      
    case 8:
      // dZ vs Zinter
      cnv = new TCanvas("Dz_vs_Zinter"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DzInterMwpc"+i+"NaI_v_ZinterMwpc"+i+"NaI");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("Sigma_dZ_vs_Zinter"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,0.,300.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("Dz(Zinter), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("Mean_dZ_vs_Zinter"+i);
      h1 = (TH1D*)cnv->DrawFrame(-300.,-30.,300.,30.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("Dz(Zinter), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1","","",-280.,280.);
      grMean->Draw("P");
      break;
      
    case 9:
      // dX vs Zinter
      cnv = new TCanvas("Dx_vs_Zinter"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DxInterMwpc"+i+"NaI_v_ZinterMwpc"+i+"NaI");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-40.,40.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDx_vs_Zinter"+i);
      h1 = (TH1D*)cnv->DrawFrame(-400.,0.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dX(Zinter), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDx_vs_Zinter"+i);
      h1 = (TH1D*)cnv->DrawFrame(-400.,-10.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dX(Zinter), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 10:
      // dY vs Zinter
      cnv = new TCanvas("Dy_vs_Zinter"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DyInterMwpc"+i+"NaI_v_ZinterMwpc"+i+"NaI");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-10.,10.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDy_vs_Zinter"+i);
      h1 = (TH1D*)cnv->DrawFrame(-400.,0.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dY(Zinter), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDy_vs_Zinter"+i);
      h1 = (TH1D*)cnv->DrawFrame(-400.,-10.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dY(Zinter), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 11:
      // dZ vs PhiInter
      cnv = new TCanvas("Dz_vs_PhiInter"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DzInterMwpc"+i+"NaI_v_PhiInterMwpc"+i+"NaI");
      h2->Draw("color");
      lPhi->Draw("same");
      //
      SliceH2(h2,"y",20,-40.,40.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDz_vs_PhiInter"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,360.,40.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dZ(PhiInter), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDz_vs_PhiInter"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dZ(PhiInter), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      break;
      
    case 12:
      // dX vs PhiInter
      cnv = new TCanvas("Dx_vs_PhiInter"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DxInterMwpc"+i+"NaI_v_PhiInterMwpc"+i+"NaI");
      h2->Draw("color");
      lPhi->Draw("same");
      //
      SliceH2(h2,"y",20,-40.,40.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDx_vs_PhiInter"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dX(PhiInter), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDx_vs_PhiInter"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dX(PhiInter), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 13:
      // dY vs PhiInter
      cnv = new TCanvas("Dy_vs_PhiInter"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DyInterMwpc"+i+"NaI_v_PhiInterMwpc"+i+"NaI");
      h2->Draw("color");
      lPhi->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDy_vs_PhiInter"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("#sigma, mm");
      h1->SetTitle("dY(PhiInter), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDy_vs_PhiInter"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,-20.,360.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("Mean, mm");
      h1->SetTitle("dY(PhiInter), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol0");
      grMean->Draw("P");
      break;
      
    case 14:
      // dPhi vs PhiInter
      cnv = new TCanvas("Dphi_vs_PhiInter"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiInterMwpc"+i+"NaI_v_PhiInterMwpc"+i+"NaI");
      h2->Draw("color");
      lPhi->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDphi_vs_PhiInter"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,0.,360.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("dPhi(PhiInter), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDphi_vs_PhiInter"+i);
      h1 = (TH1D*)cnv->DrawFrame(0.,-10.,360.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("#varphi, degrees");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("dPhi(PhiInter), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      break;
      
    case 15:
      // dPhi vs Zinter
      cnv = new TCanvas("Dphi_vs_Zinter"+i);
      h2 = (TH2D*)gDirectory->Get("GeomCalib_DphiInterMwpc"+i+"NaI_v_ZinterMwpc"+i+"NaI");
      h2->Draw("color");
      lZ->Draw("same");
      //
      SliceH2(h2,"y",20,-50.,50.,1.,grMean,grSigma);
      // Sigma
      cnv = new TCanvas("SigmaDphi_vs_Zinter"+i);
      h1 = (TH1D*)cnv->DrawFrame(-400.,0.,400.,20.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("#sigma, degrees");
      h1->SetTitle("dPhi(Zinter), MWPC-"+i);
      grSigma->SetMarkerStyle(20);
      grSigma->Draw("P");
      // Mean
      cnv = new TCanvas("MeanDphi_vs_Zinter"+i);
      h1 = (TH1D*)cnv->DrawFrame(-400.,-10.,400.,10.);
      cnv->GetFrame()->SetFillColor(0);
      cnv->GetFrame()->SetBorderSize(0);
      h1->SetXTitle("z, mm");
      h1->SetYTitle("Mean, degrees");
      h1->SetTitle("dPhi(Zinter), MWPC-"+i);
      grMean->SetMarkerStyle(20);
      grMean->Fit("pol1");
      grMean->Draw("P");
      break;
      
    default:
      for (Int_t j=1; j<=20; ++j) CalibMwpcNaI(i,j,0,0,0,0);
  }
}

//___________________________________________________________________________________
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


//___________________________________________________________________________________
void Plot()
{
  // General style
  gROOT->ProcessLine(".x Style_Default_2.C");
  gROOT->SetStyle("Default_2");
  gROOT->ForceStyle();
  
  // Sigma Phi (theta)
  TGraphErrors *grSigma1;
  CalibMwpc(9,0,0,0,grSigma1);
  //
  TGraphErrors *grSigma2;
  CalibMwpcNaI("0",9,0,0,0,grSigma2);
  //
  cnv = new TCanvas("Sigma_1");
  h = (TH1D*)cnv->DrawFrame(0.,0.,180.,5.);
  cnv->GetFrame()->SetFillColor(0);
  cnv->GetFrame()->SetBorderSize(0);
  h->SetXTitle("#theta, degrees");
  h->SetYTitle("#sigma, degrees");
  h->SetTitle("Standard deviation of #Delta#phi as a function of #theta");
  grSigma2->SetMarkerStyle(24);
  grSigma1->Draw("P");
  grSigma2->Draw("P, same");
  
//   // Sigma theta (theta)
//   TGraphErrors *grSigma1;
//   CalibMwpc(11,0,0,0,grSigma1);
//   //
//   TGraphErrors *grSigma2;
//   CalibMwpcNaI("0",11,0,0,0,grSigma2);
//   //
//   cnv = new TCanvas("Sigma_2");
//   h = (TH1D*)cnv->DrawFrame(0.,0.,180.,4.);
//   cnv->GetFrame()->SetFillColor(0);
//   cnv->GetFrame()->SetBorderSize(0);
//   h->SetXTitle("#theta, degrees");
//   h->SetYTitle("#sigma, degrees");
//   h->SetTitle("Standard deviation of #Delta#theta as a function of #theta");
//   grSigma2->SetMarkerStyle(24);
//   grSigma1->Draw("P");
//   grSigma2->Draw("P, same");
  
  // Legends
  TLegend *lg = new TLegend(0.7,0.7,0.85,0.88,"","brNDC");
  lg->SetLineColor(0);
  lg->SetTextSize(0.03);
  lg->AddEntry(grSigma1,"MWPC1-MWPC2","p");
  lg->AddEntry(grSigma2,"MWPC1-CB","p");
  lg->Draw();
}

//___________________________________________________________________________________
void GeomCalibCentralApparatus(const Bool_t readFile)
{
  //
  
  if (readFile)
  {
    gFileAR = new TFile("ARHistograms.root");
    if (!gFileAR)
    {
      cout << "File " << nameFile << " not found!" << endl;
      return;
    }
  }
  
  //
  CalibEI("0");
  CalibEI("1");
  
  //
//   CalibMwpc();
  
  //
//   CalibMwpcNaI("0");
//   CalibMwpcNaI("1");
}
