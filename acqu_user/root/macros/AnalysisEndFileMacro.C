// SVN Info: $Id: AnalysisEndFileMacro.C 1 2013-11-25 18:44:07Z mushkar $

//_________________________________________________________________________________
// void AnalysisMCEndFileMacro()
// {
//   // Stuff to do at the end of a data file
//   // Here all spectra are saved to disk
//   
//   printf("End-of-File macro executing\n");
// 
//   // Save histograms
//   Char_t *n = strrchr(gAR->GetTreeFile()->GetName(), '/') + 1;
//   if(!n) {
//     cout << "Too short file. File name can be wrong. Nothing to save." << endl;
//     ResetROOTMem();
//     return;
//   }
//   Char_t name[128];
//   strcpy(name, n);
//   Char_t* n1 = strrchr(name, '.');
//   *n1 = '\0';
//   strcat(name, "_hist.root");
//   TFile f(name,"recreate");
//   gROOT->GetList()->Write();
//   ResetROOTMem();
//   f.Close();
//   printf("All histograms saved to %s and zero'ed\n\n", name);
// }

//_________________________________________________________________________________
void AnalysisEndFileMacro()
{
  // Stuff to do at the end of a data file
  // Here all spectra are saved to disk
  
  printf("End-of-File macro executing\n");

  // Save histograms
  Char_t *n = gUAN->GetFileName();
  if(!n) {
    cout << "Too short file. File name can be wrong. Nothing to save." << endl;
    ResetROOTMem();
    return;
  }
  Char_t name[128];
  strcpy(name, n);
  Char_t* n1 = strrchr(name, '.');
  *n1 = '\0';
  strcat(name, "_hist.root");
  TFile f(name,"recreate");
  gROOT->GetList()->Write();
  ResetROOTMem();
  f.Close();
  printf("All histograms saved to %s and zero'ed\n\n", name);
}

//_________________________________________________________________________________
void ResetROOTMem()
{
  // Reset all histograms in ROOT memory and zero FPD ScalerAccB

  TObject *obj;
  TIter next(gROOT->GetList());
  while((obj = next()))
    if(obj->InheritsFrom("TH1"))
      ((TH1*)obj)->Reset();
      
  gAN->ZeroSumScalers();
  gUAN->ZeroAll();
//   ((TA2KensLadder*)gAN->GetGrandChild("FPD"))->ClearScalerAcc();
}
