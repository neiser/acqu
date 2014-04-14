//--Author	JRM Annand   30th Sep 2003
//--Update	JRM Annand...29th Nov 2007  Add libGui and libFoam
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
// ROOT script to make html'ised copy of the Acqu-Root Classes
//
{
  gROOT->Reset();
  gSystem->Load( "libPhysics.so" );    	          // Vectors etc.
  gSystem->Load( "libFoam.so" );    	          // Foam MC stuff
  gSystem->Load( "libGui.so" );    	          // GUI stuff
  gSystem->Load( "lib/libAcquRoot.so" );// AR lib
  gSystem->Load( "lib/libUserRoot.so" );    // User lib
  gSystem->Load( "lib/libAcquMC.so" );  // MC lib
  gSystem->Load( "lib/libMCUserRoot.so" );  // User MC
  gSystem->Load( "lib/libAcquDAQ.so" ); // DAQ library
  THtml h;
  h.SetProductName("AcquRoot");
  h.MakeAll();
  return;
}
