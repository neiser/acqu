//--Author	JRM Annand   27th Jan 2004...Apapt A.Starostin code
//--Rev         JRM Annand   12th May 2004...TA2Calorimeter etc. added
//--Rev         JRM Annand   21st Nov 2004...TA2CosmicCal added
//--Rev         JRM Annand   15th Jul 2005...TA2CrystalBall, TA2TAPS
//--Update      JRM Annand   15th Jul 2005...Use TA2Analysis::LoadVariable
//--Description
//                *** Acqu++ <-> Root ***
// Online/Offline Analysis of Sub-Atomic Physics Experimental Data
//
// TA2UserAnalysis
//
// User analysis recognises user-written apparatus and physics classes
// New apparati and physics should be entered in Map_t kKnownChild
// and also in the switch statement of CreateChild() where the apparatus
// constructers are called

#include "TA2UserAnalysis.h"
#include "TA2Calorimeter.h"
#include "TA2Tagger.h"
#include "TA2CosmicCal.h"
#include "TA2CrystalBall.h"
#include "TA2CB.h"
#include "TA2Taps.h"
#include "TA2CentralApparatus.h"
#include "TA2GenericApp.h"
#include "TA2UserPhysics.h"
#include "TA2MesonPhysics.h"
#include "TA2AccessSQL.h"
#include "TA2GoAT.h"
#include "TA2BasePhysics.h"
#include "TA2TriggerPhysics.h"
#include "TA2Pi0Compton.h"
#include "TA2MyCrystalBall.h"
#include "TA2MyTAPS.h"
#include "TA2GenericApp.h"
#include "TA2BeamPolMon.h"
#include "TA2MyCalibration.h"
#include "TA2MyCaLib.h"
#include "TA2MyClusterCalib.h"
#include "TA2MyRateEstimation.h"
#include "TA2TAPSAnalysis.h"
#include "TA2GeomCalibPhysics.h"

// Recognised apparatus classes.
// The "standard" set is held in TA2Analysis
enum { 
  EA2Calorimeter,
  EA2Tagger,
  EA2CosmicCal,
  EA2CrystalBall,
  EA2CB,
  EA2Taps,
  EA2CentralApparatus,
  EA2GenericApp,
  EA2BeamPolMon,
  EA2Physics,
  EA2UserPhysics,
  EA2MesonPhysics,
  EA2AccessSQL,
  EA2GoAT,
  EA2BasePhysics,
  EA2TriggerPhysics,
  EA2Pi0Compton,
  EA2MyCrystalBall, 
  EA2MyTAPS,
  EA2MyAnalysis, 
  EA2MyCalibration, 
  EA2MyCaLib, 
  EA2MyClusterCalib, 
  EA2MyRateEstimation,
  EA2GeomCalibPhysics,
  EA2TAPSAnalysis };

static const Map_t kKnownChild[] =
{
  //Apparati
  {"TA2Calorimeter",      EA2Calorimeter},
  {"TA2Tagger",           EA2Tagger},
  {"TA2CosmicCal",        EA2CosmicCal},
  {"TA2CrystalBall",      EA2CrystalBall},
  {"TA2CB",               EA2CB},
  {"TA2Taps",             EA2Taps},
  {"TA2CentralApparatus", EA2CentralApparatus},
  {"TA2GenericApp",       EA2GenericApp},  
  {"TA2BeamPolMon",       EA2BeamPolMon},  
  {"TA2MyCrystalBall",    EA2MyCrystalBall},
  {"TA2MyTAPS",           EA2MyTAPS},
  //Physics
  {"TA2GeomCalibPhysics", EA2GeomCalibPhysics},
  {"TA2Pi0Compton",       EA2Pi0Compton},
  {"TA2Physics",          EA2Physics},
  {"TA2UserPhysics",      EA2UserPhysics},
  {"TA2MesonPhysics",     EA2MesonPhysics},
  {"TA2AccessSQL",		  EA2AccessSQL},
  {"TA2GoAT",			  EA2GoAT},
  {"TA2BasePhysics",      EA2BasePhysics},
  {"TA2TriggerPhysics",   EA2TriggerPhysics},
  {"TA2MyAnalysis",       EA2MyAnalysis},
  {"TA2MyCalibration",    EA2MyCalibration},
  {"TA2MyCaLib",          EA2MyCaLib},
  {"TA2MyClusterCalib",   EA2MyClusterCalib},
  {"TA2MyRateEstimation", EA2MyRateEstimation},
  {"TA2TAPSAnalysis",     EA2TAPSAnalysis},
  {NULL,                  -1}
};

//-----------------------------------------------------------------------------

TA2UserAnalysis::TA2UserAnalysis(const char* name)
                :TA2Analysis(name)
{
  kValidChild = kKnownChild; //Valid apparatus list
  PrivateInit();
}

//---------------------------------------------------------------------------

TA2UserAnalysis::~TA2UserAnalysis()
{
  //Free up allocated memory
}

//---------------------------------------------------------------------------

TA2DataManager* TA2UserAnalysis::CreateChild(const char* name, Int_t a)
{
  //Add creation of user-defined apparati here
  switch(a)
  {
   //Apparatus stuff:
   case EA2Tagger:
    //Standard tagger
    return new TA2Tagger(name, this);
   case EA2Calorimeter:
    //General calorimeter... TAPS, CB etc.
    return new TA2Calorimeter(name, this);
   case EA2CosmicCal:
    //Cosmic calibration array of long bars
    return new TA2CosmicCal(name, this);
   case EA2CrystalBall:
    //Modded CB stuff
    return new TA2CrystalBall(name, this);
   case EA2CB:
    //Other modded CB stuff
    return new TA2CB(name, this);
   case EA2Taps:
    //Henning's TAPS class
    return new TA2Taps(name, this);
   case EA2CentralApparatus:
    //Alexander Mushkarenkov's MWPC/Tracking systems
    return new TA2CentralApparatus(name, this);
   case EA2GenericApp:
    //Generic
    return new TA2GenericApp(name, this);
  case EA2BeamPolMon:
    // BeamPolMon
    return new TA2BeamPolMon(name, this);
  case EA2MyCrystalBall:
    // My Moded CB stuff
    return new TA2MyCrystalBall( name, this );
  case EA2MyTAPS:
    // My TAPS stuff
    return new TA2MyTAPS( name, this );

   //Physics stuff:
   case EA2Pi0Compton:
    // Dave's Physics class
    return new TA2Pi0Compton( name, this );
   case EA2Physics:
    //Default (dummy physics)
    return new TA2Physics(name, this);
   case EA2UserPhysics:
    //User defined Physics
    return new TA2UserPhysics(name, this);
   case EA2MesonPhysics:
    //Demonstration physics class for pi0/eta -> 2gamma
    return new TA2MesonPhysics(name, this);
   case EA2AccessSQL:
    //Base physics class with particle collection and trigger
    return new TA2AccessSQL(name, this);
   case EA2GoAT:
    //Base physics class with particle collection and trigger
    return new TA2GoAT(name, this);
   case EA2BasePhysics:
    //Base physics class with particle collection and trigger
    return new TA2BasePhysics(name, this);
   case EA2TriggerPhysics:
    //Trigger threshold physics class
    return new TA2TriggerPhysics(name, this);
   case EA2MyCalibration:
    // calibration
    return new TA2MyCalibration( name, this );
  case EA2MyCaLib:
    // CaLib
    return new TA2MyCaLib( name, this );
  case EA2MyClusterCalib:
    // cluster calibration
    return new TA2MyClusterCalib( name, this );
  case EA2MyRateEstimation:
    // rate estimation
    return new TA2MyRateEstimation( name, this );
  case EA2GeomCalibPhysics:
    // Geometrical calibration for the TA2Mwpc and TA2CentralApparatus
    return new TA2GeomCalibPhysics( name, this );
  case EA2TAPSAnalysis:
    // TAPS analysis
    return new TA2TAPSAnalysis( name, this );

   default:
    PrintError((char*)name, "<Unknown apparatus..cannot continue>", EErrFatal);
  }
  return NULL;
}
