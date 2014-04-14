//--Author	JRM Annand   30th Sep 2003
//--Update	JRM Annand... 1st Dec 2004 Update classes
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
// ROOT script to make html'ised copy of the Acqu-Root Classes
//
{
  gROOT->Reset();
  gSystem->Load( "libPhysics.so" );    	        // Vectors etc.
  gSystem->Load( "libFoam.so" );    	        // Foam MC stuff
  gSystem->Load( "libGui.so" );                 // GUI classes
  gSystem->Load( "$acqu_sys/lib/$ACQU_OS_NAME/libAcquRoot.so" ); // AR library
  gSystem->Load( "$acqu_sys/lib/$ACQU_OS_NAME/libAcquMC.so" );   // MC library
  gSystem->Load( "$acqu_sys/lib/$ACQU_OS_NAME/libAcquDAQ.so" );  // DAQ library
  THtml h;
  h.MakeAll();
  return;
}
