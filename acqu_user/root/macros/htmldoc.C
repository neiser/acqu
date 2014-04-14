//--Author	JRM Annand   30th Sep 2003
//--Update	JRM Annand...
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data 
// ROOT script to make html'ised copy of the Acqu-Root Classes
//
{
gROOT->Reset();
gSystem->Load( "libPhysics.so" );    	        // Vectors etc.
gSystem->Load( "$acqu_sys/lib/libAcquRoot.so" );// Acqu-Root library
gSystem->Load( "lib/libUserRoot.so" );    	// User class library
THtml h;
// Main control
h.MakeClass("TA2UserControl",kTRUE);
h.MakeClass("TA2UserAnalysis",kTRUE);
// Physics
h.MakeClass("TA2UserPhysics",kTRUE);
h.MakeClass("TA2PhotoPhysics",kTRUE);
// Apparatus
h.MakeClass("TA2Calorimeter",kTRUE);
h.MakeClass("TA2KensTagger",kTRUE);
h.MakeClass("TA2LinearPol",kTRUE);
h.MakeClass("TA2CosmicCal",kTRUE);
// Detector
h.MakeClass("TA2PlasticPID",kTRUE);
h.MakeClass("TA2CalArray",kTRUE);
h.MakeClass("TA2KensLadder",kTRUE);
h.MakeClass("TA2TAPS_BaF2",kTRUE);
h.MakeClass("TA2CylMWPC",kTRUE);
}
