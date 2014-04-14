// SVN Info: $Id: AnalysisPeriodMacro.C 1 2013-11-25 18:44:07Z mushkar $

//___________________________________________________________________________________________________
void AnalysisPeriodMacro()
{  
  Int_t nEvnt             = gAN->GetNEvent();
  Int_t nEvntAccepted     = gAN->GetNEvAnalysed();
  Int_t nDAQEvent         = gAN->GetNDAQEvent();

  cout << gUAN->GetFileName() << "\t#: " << nEvnt << "\tAccepted #: " << nEvntAccepted << "\t"
       << "DAQ#: " << nDAQEvent <<endl;
       
  PrintBenchmark();
}

//___________________________________________________________________________________________________
void AnalysisMCPeriodMacro()
{
  Int_t nEvnt             = gAN->GetNEvent();
  Int_t nEvntAccepted     = gAN->GetNEvAnalysed();
  Int_t nDAQEvent         = gAN->GetNDAQEvent();

  cout << strrchr(gAR->GetTreeFile()->GetName(),'/')+1 << "\t#: " << nEvnt << "\tAccepted #: " << nEvntAccepted << "\t"
       << "DAQ#: " << nDAQEvent <<endl;
}

//___________________________________________________________________________________________________
void PrintBenchmark()
{
   // Benchmark
   static TBenchmark benchmark;
   benchmark.Show("Periodic");
   benchmark.Reset();
   benchmark.Start("Periodic");
}
