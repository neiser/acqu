// SVN Info: $Id: TA2MyPhysics.cc 780 2011-02-03 13:38:26Z werthm $

/*************************************************************************
 * Author: Dominik Werthmueller, 2008-2011
 *************************************************************************/

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TA2MyPhysics                                                         //
//                                                                      //
// This is an abstract base physics analysis class.                     //
// Classes inheriting from this class must implement the LoadVariable() //
// and the ReconstructPhysics() method.                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#include "TA2MyPhysics.h"

ClassImp(TA2MyPhysics)


//______________________________________________________________________________
TA2MyPhysics::TA2MyPhysics(const char* name, TA2Analysis* analysis) 
    : TA2Physics(name, analysis)
{
    // Constructor.
    
    Char_t tmp[256];
   
    // command-line recognition for SetConfig()
    AddCmdList(myPhysicsConfigKeys);
    
    // init members 
    fAnalysisMode = gAR->GetProcessType();
    
    if (fAnalysisMode == EMCProcess) fIsMC = kTRUE;
    else fIsMC = kFALSE;

    fMCVertX = 0;
    fMCVertY = 0;
    fMCVertX = 0;
    
    fRunNumber          = 0;
    ExtrtactRunNumber();
    fEventCounter       = 0;
    fEventOffset        = 0;
    fSaveEvent          = 0;
    
    fTagger             = 0;                 
    fLadder             = 0;
    fTaggerPhotonNhits  = 0;                 
    fTaggerPhotonHits   = 0;                 
    fTaggerPhotonEnergy = 0;                 
    fTaggerPhotonTime   = 0;                 

    fCB               = 0;  
    fNaI              = 0;
    fNaINhits         = 0;
    fNaIHits          = 0;
    fNaIEnergy        = 0;
    fNaITime          = 0;
    fCBClTime         = 0;       
    fCBPID_Index      = 0;
    fCBPID_dE         = 0;
    fCBClusterHit     = 0;         
    fCBNCluster       = 0;          
    fCBCluster        = 0;  
    
    fPID              = 0;         
    fPIDNhits         = 0;               
    fPIDHits          = 0;               
    fPIDHitPos        = 0;            
    fPIDEnergy        = 0;
    fPIDTime          = 0;

    fTAPS             = 0;        
    fBaF2PWO          = 0;   
    fBaF2PWONhits     = 0;            
    fBaF2PWOHits      = 0;            
    fBaF2PWOEnergy    = 0;
    fBaF2PWOTime      = 0;         

    fBaF2Pattern      = 0; 
    fBaF2CFDPattern   = -1;
    fBaF2LED1Pattern  = -1;
    fBaF2LED2Pattern  = -1;
    fBaF2CFDNhits     = 0;         
    fBaF2CFDHits      = 0;           
    fBaF2LED1Nhits    = 0;           
    fBaF2LED1Hits     = 0;           
    fBaF2LED2Nhits    = 0;           
    fBaF2LED2Hits     = 0;            
    
    fTAPSClusterHit   = 0;          
    fTAPSNCluster     = 0;         
    fTAPSCluster      = 0;  
    
    fVeto             = 0;    
    fVetoNhits        = 0;          
    fVetoHits         = 0;          
    fVetoEnergy       = 0;
    fVetoTime         = 0;

    fPbWO4            = 0;
    fPbWO4Nhits       = 0;      
    fPbWO4Hits        = 0;
    fPbWO4Energy      = 0;
    fPbWO4Time        = 0;

    fCP               =  0;     
    fEBeamBitPos      = 10;
    fEBeamBitNeg      =  4;
    fEBeamHelState    =  0;     
    
    fTOF              = 0;   
    fTOFBarNhits      = 0;          
    fTOFBarHits       = 0;          
    fTOFBarMeanEnergy = 0;      
    fTOFBarMeanTime   = 0;      
  
    fPbGlassApp       = 0;
    fPbGlass          = 0;  
    fPbGlassNhits     = 0;        
    fPbGlassTime      = 0;    

    fNNeutral = 0;
    fNCharged = 0;
    fPartCB   = 0;
    fPartTAPS = 0;
    fPartNeutral = 0;
    fPartCharged = 0;

    fNMC = 0;
    for (Int_t i = 0; i < gMyPhysics_MaxMCPart; i++) fPartMC[i] = new TOMCParticle(kRootino, kFALSE);
    
    fL1CB          = kFALSE;
    fL1TAPS_OR     = kFALSE;
    fL1TAPS_M2     = kFALSE;
    fL1Pulser      = kFALSE;
    fL1TAPS_Pulser = kFALSE;
    fL2M1          = kFALSE;
    fL2M2          = kFALSE;
    fL2M3          = kFALSE;
    fL2M4          = kFALSE;

    fTrigTotalMult   = 0;
    fTrigTAPSMult    = 0;
    fTrigCBSum       = 0;
    fTrigCBSegThr    = 0;
    fTrigTAPSLED1Thr = 0;  
    fTrigTAPSLED2Thr = 0; 

    fOldScalerSumValue = new Double_t[gMyPhysics_MaxScalers];
    for (Int_t i = 0; i < gMyPhysics_MaxScalers; i++) fOldScalerSumValue[i] = 0.;
    fOldP2ScalerSum = 0.;
    fNBadTaggerChannels = 0;
    fBadTaggerChannels  = new Int_t[gMyPhysics_MaxTaggerCh];
    
    fNt_P2TaggerRatio = 0;

    fTargetPosition = 0;

    fCBTimeOffset   = 0.;
    fTAPSTimeOffset = 0.;
    
    fH_CB_PID_Coinc_Hits = new TH1*[gMyPhysics_MaxPID];
    fH_CB_PID_dE_E = new TH2*[gMyPhysics_MaxPID];
    for (Int_t i = 0; i < gMyPhysics_MaxPID; i++) 
    {
        // CB-PID coincidence plots
        sprintf(tmp, "CB_PID_Coinc_Hits_%d", i);
        //fH_CB_PID_Coinc_Hits[i] = new TH1F(tmp, tmp, 360, 0, 180);
        
        // CB-PID dE vs. E plots
        sprintf(tmp, "CB_PID_dE_E_%d", i);
        //fH_CB_PID_dE_E[i] = new TH2F(tmp, tmp, 1000, 0, 1000, 200, 0, 10);
    }

    fCBPIDPhiLimit = 15.;

    // CaLib
    fCaLibReader = 0;
}

//______________________________________________________________________________
TA2MyPhysics::~TA2MyPhysics()
{
    // Destructor.
    
    if (fBadTaggerChannels) delete [] fBadTaggerChannels;
    if (fNt_P2TaggerRatio)
    {
        for (Int_t i = 0; i < 352; i++) delete fNt_P2TaggerRatio[i];
        delete [] fNt_P2TaggerRatio;
    }
}

//______________________________________________________________________________
void TA2MyPhysics::SetConfig(Char_t* line, Int_t key)
{
    // Read in analysis configuration parameters and configure the class. 

    switch (key)
    {
        case EMP_USE_CALIB:
        {   
            Char_t tmp[5][256];

            // read CaLib parameters
            if (sscanf(line, "%s%s%s%s%s", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4]) == 5) 
            {
                // create the CaLib reader
                fCaLibReader = new CaLibReader_t(tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], fRunNumber);
                
                // check database connection
                if (fCaLibReader->IsConnected())
                    Info("SetConfig", "Using calibration '%s' in the CaLib database '%s' on '%s@%s'"
                                      , tmp[4], tmp[1], tmp[2], tmp[0]);
                else
                    Error("SetConfig", "Could not connect to CaLib database server!");
            }
            else Error("SetConfig", "CaLib could not be configured and is hence disabled!");
            break;
        }
        case EMP_CALIB_MISC:
        {   
            Int_t target_pos;

            // read CaLib misc parameters
            if (sscanf(line, "%d", &target_pos) == 1)
            {
                // check if CaLib reader exists
                if (fCaLibReader) 
                {
                    fCaLibReader->SetTargetPosition((Bool_t) target_pos);
                }
                else
                {
                    Error("SetConfig", "Misc calibration cannot be configured because CaLib was not configured!");
                    break;
                }
            }
            else Error("SetConfig", "CaLib misc calibration could not be configured!");
            break;
        }
        case EMP_CALIB_TAGG:
        {   
            Int_t time;

            // read CaLib TAGG parameters
            if (sscanf(line, "%d", &time) == 1)
            {
                // check if CaLib reader exists
                if (fCaLibReader) 
                {
                    fCaLibReader->SetTAGGtime((Bool_t) time);
                }
                else
                {
                    Error("SetConfig", "TAGG cannot be configured because CaLib was not configured!");
                    break;
                }
            }
            else Error("SetConfig", "CaLib TAGG calibration could not be configured!");
            break;
        }
        case EMP_CALIB_CB:
        {   
            Int_t energy;
            Int_t time;
            Int_t timewalk;
            Int_t quadEnergy;

            // read CaLib CB parameters
            if (sscanf(line, "%d%d%d%d", &energy, &time, &timewalk, &quadEnergy) == 4)
            {
                // check if CaLib reader exists
                if (fCaLibReader)
                {
                    fCaLibReader->SetCBenergy((Bool_t) energy);
                    fCaLibReader->SetCBtime((Bool_t) time);
                    fCaLibReader->SetCBwalk((Bool_t) timewalk);
                    fCaLibReader->SetCBquadEnergy((Bool_t) quadEnergy);
                }
                else
                {
                    Error("SetConfig", "CB cannot be configured because CaLib was not configured!");
                    break;
                }
            }
            else Error("SetConfig", "CaLib CB calibration could not be configured!");
            break;
        }
        case EMP_CALIB_TAPS:
        {   
            Int_t energy;  
            Int_t time;  
            Int_t quadEnergy;
            Int_t led;

            // read CaLib TAPS parameters
            if (sscanf(line, "%d%d%d%d", &energy, &time, &quadEnergy, &led) == 4)
            {
                // check if CaLib reader exists
                if (fCaLibReader)
                {
                    fCaLibReader->SetTAPSenergy((Bool_t) energy);
                    fCaLibReader->SetTAPStime((Bool_t) time);
                    fCaLibReader->SetTAPSquadEnergy((Bool_t) quadEnergy);
                    fCaLibReader->SetTAPSled((Bool_t) led);
                }
                else
                {
                    Error("SetConfig", "TAPS cannot be configured because CaLib was not configured!");
                    break;
                }
            }
            else Error("SetConfig", "CaLib TAPS calibration could not be configured!");
            break;
        }
        case EMP_CALIB_PID:
        {   
            Int_t phi;
            Int_t droop;
            Int_t energy;
            Int_t time;

            // read CaLib PID parameters
            if (sscanf(line, "%d%d%d%d", &phi, &droop, &energy, &time) == 4)
            {
                // check if CaLib reader exists
                if (fCaLibReader)
                {
                    fCaLibReader->SetPIDphi((Bool_t) phi);
                    fCaLibReader->SetPIDdroop((Bool_t) droop);
                    fCaLibReader->SetPIDenergy((Bool_t) energy);
                    fCaLibReader->SetPIDtime((Bool_t) time);
                }
                else
                {
                    Error("SetConfig", "PID cannot be configured because CaLib was not configured!");
                    break;
                }
            }
            else Error("SetConfig", "CaLib PID calibration could not be configured!");
            break;
        }
        case EMP_CALIB_VETO:
        {   
            Int_t energy;
            Int_t time;

            // read CaLib Veto parameters
            if (sscanf(line, "%d%d", &energy, &time) == 2)
            {
                // check if CaLib reader exists
                if (fCaLibReader)
                {
                    fCaLibReader->SetVetoenergy((Bool_t) energy);
                    fCaLibReader->SetVetotime((Bool_t) time);
                }
                else
                {
                    Error("SetConfig", "Veto cannot be configured because CaLib was not configured!");
                    break;
                }
            }
            else Error("SetConfig", "CaLib Veto calibration could not be configured!");
            break;
        }
        case EMP_BAD_TAGG_CH:
        {   
            /*Char_t fileName[256];

            // read number of bad tagger channels
            if (sscanf(line, "%s", fileName))
                fNBadTaggerChannels = TOLoader::LoadBadTaggerChannels(gSystem->ExpandPathName(fileName), fBadTaggerChannels);*/
            break;
        }
        case EMP_P2_TAGGER_RATIO:
        {
            // activate p2-tagger-ratio output
            
            Char_t tname[256];
            fNt_P2TaggerRatio = new TNtupleD*[352];
            for (Int_t i = 0; i < 352; i++) 
            {
                sprintf(tname, "P2TaggerRatio_%d", i);
                fNt_P2TaggerRatio[i] = new TNtupleD(tname, tname, "ratio:ratio_err");
            }
            break;
        }
        case EMP_TRIGGER_TOTAL_MULT:
        {
            if (sscanf(line, "%d", &fTrigTotalMult) != 1)
                Error("SetConfig", "Bad syntax in trigger total multiplicity definition!");
            break;
        }
        case EMP_TRIGGER_TAPS_MULT:
        {
            if (sscanf(line, "%d", &fTrigTAPSMult) != 1)
                Error("SetConfig", "Bad syntax in trigger TAPS multiplicity definition!");
            break;
        }
        case EMP_TRIGGER_CB_THRESHOLDS:
        {
            if (sscanf(line, "%lf%lf", &fTrigCBSum, &fTrigCBSegThr) != 2)
                Error("SetConfig", "Bad syntax in trigger CB thresholds definition!");
            break;
        }
        case EMP_TRIGGER_TAPS_LED1_THRESHOLD:
        {
            if (sscanf(line, "%lf", &fTrigTAPSLED1Thr) != 1)
                Error("SetConfig", "Bad syntax in trigger TAPS LED1 thresholds definition!");
            break;
        }
        case EMP_TRIGGER_TAPS_LED2_THRESHOLD:
        {
            if (sscanf(line, "%lf", &fTrigTAPSLED2Thr) != 1)
                Error("SetConfig", "Bad syntax in trigger TAPS LED2 thresholds definition!");
            break;
        }
        case EMP_CB_TIME_OFFSET:
        {
            if (sscanf(line, "%lf", &fCBTimeOffset) != 1)
                Error("SetConfig", "Bad syntax in CB time offset definition!");
            else Info("SetConfig", "Using a global CB time offset of %f ns", fCBTimeOffset);
            break;
        }
        case EMP_TAPS_TIME_OFFSET:
        {
            if (sscanf(line, "%lf", &fTAPSTimeOffset) != 1)
                Error("SetConfig", "Bad syntax in TAPS time offset definition!");
            else Info("SetConfig", "Using a global TAPS time offset of %f ns", fTAPSTimeOffset);
            break;
        }
        case EMP_CB_PID_PHI_LIMIT:
        {
            if (sscanf(line, "%lf", &fCBPIDPhiLimit) != 1)
                Error("SetConfig", "Bad syntax in the CB-PID coincidence phi limit!");
            else Info("SetConfig", "Using a CB-PID coincidence phi limit of %.2f degrees", fCBPIDPhiLimit);
            break;
        }
        case EMP_BEAM_POL_BITS:
        {
            if (sscanf(line, "%d%d", &fEBeamBitPos, &fEBeamBitNeg) != 2)
                Error("SetConfig", "Bad syntax in beam helicity bit definition!");
            else Info("SetConfig", "Using beam helicity bit: %d (+) and %d (-)", fEBeamBitPos, fEBeamBitNeg);
            break;
        }
		case EMP_CALIB_CBENERGY_PER_RUN:
        {  
			Char_t tmp[128];
			
			printf("try to enable CBEnergy correction per Run\n");
			
			if (sscanf(line, "%s", tmp) == 1) 
            {
				GetRunNumber();
				FILE*	f = fopen(tmp,"r");
				Int_t 		num;
				Double_t 	val[4];
				while(!feof(f))
				{
					if (fscanf(f, "%d %lf %lf %lf %lf\n", &num, &val[0], &val[1], &val[2], &val[3]) == 5) 
					{
						if(num == fRunNumber)
						{
							CBEnergyPerRunCorrection		= true;
							CBEnergyPerRunCorrectionFactor	= val[0];
							printf("CBEnergy correction factor for run %d is %lf with error %lf\n", fRunNumber, CBEnergyPerRunCorrectionFactor, val[1]);
							break;
						}
					}
				}
				fclose(f);
			}
		}
        default:
        {
            // default main apparatus SetConfig()
            TA2Physics::SetConfig(line, key);
            break;
        }
    }
}

//______________________________________________________________________________

Int_t	TA2MyPhysics::ExtrtactRunNumber()
{
	char*	str = new char[64];
	strcpy(str,gAR->GetFileName());
	while(strpbrk(str+1,"."))
		str	= strpbrk(str+1,".");
	
	*str	= '\0';
	str	= str-1;
	
	while(*str == '0' || *str == '1' || *str == '2' || *str == '3' || *str == '4' || *str == '5' || *str == '6' || *str == '7' || *str == '8' || *str == '9')
		str	= str-1;
	str	= str+1;
	
	if(fRunNumber != atoi(str));
	{
		fRunNumber = atoi(str);
		printf("RunNumber : %d\n",fRunNumber);
	}
	
	return fRunNumber;
}

//______________________________________________________________________________ 
void TA2MyPhysics::LoadDetectors(TA2DataManager* parent, Int_t depth)
{
    // Load recursively all detector components.
    
    Bool_t added = kFALSE;

    // get the children, leave if none found
    TList* children = parent->GetChildren();
    if (!children) return;
    
    // loop over children
    TObjLink *lnk = children->FirstLink();
    while (lnk)
    {
        TObject* obj = lnk->GetObject();
     
        // look for detectors
        if (!strcmp(obj->GetName(), "TAGG"))
        {
            fTagger = (TA2Tagger*) obj;
            added = kTRUE;
        }
        if (!strcmp(obj->GetName(), "FPD"))
        {
            fLadder = (TA2Ladder*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "CB"))
        {
            fCB = (TA2MyCrystalBall*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "NaI"))
        {
            fNaI = (TA2ClusterDetector*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "PID"))
        {
            fPID = (TA2Detector*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "TAPS"))
        {
            fTAPS = (TA2MyTAPS*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "BaF2PWO"))
        {
            fBaF2PWO = (TA2MyTAPS_BaF2PWO*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "VETO"))
        {
            fVeto = (TA2Detector*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "PbWO4"))
        {
            fPbWO4 = (TA2GenericDetector*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "TOF"))
        {
            fTOF = (TA2LongScint*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "PbGlassApp"))
        {
            fPbGlassApp = (TA2GenericApparatus*) obj;
            added = kTRUE;
        }
        else if (!strcmp(obj->GetName(), "PbGlassDet"))
        {
            fPbGlass = (TA2GenericDetector*) obj;
            added = kTRUE;
        }

        // print information if a detector was added
        if (added)
        {
            for (Int_t i = 0; i < depth; i++) printf("  ");
            printf( "   %s (%s)\n", obj->GetName(), obj->ClassName());
            added = kFALSE;
        }

        // add children of child
        LoadDetectors((TA2DataManager*) obj, depth + 1);
        lnk = lnk->Next();
    }
}

//______________________________________________________________________________ 
Int_t TA2MyPhysics::GetPatternIndex(const Char_t* patternName, TA2BitPattern* pattern)
{
    // Returns the index of the pattern called 'patternName' in the bit pattern 'pattern'.
    // If no such pattern is found -1 is returned.

    // check if pattern exists
    if (!pattern) return -1;

    // loop over pattern
    for (Int_t i = 0; i < pattern->GetNPattern(); i++)
    {
        if (strcmp(patternName, pattern->GetPatternName(i)) == 0) return i;
    }

    return -1;
}

//______________________________________________________________________________ 
void TA2MyPhysics::ApplyCaLib()
{
    // Update the detector calibration parameters using CaLib.

    // target position
    if (!fCaLibReader->ApplyTargetPositioncalib(&fTargetPosition))
        Error("ApplyCaLib", "An error occured during target position calibration!");

    // calibrate TAGG
    if (fLadder)
    {
        if (!fCaLibReader->ApplyTAGGcalib(fLadder))
            Error("ApplyCaLib", "An error occured during tagger calibration!");
    }
    else Error("ApplyCaLib", "Tagger not found - could not calibrate the tagger using CaLib!");

    // calibrate CB
    if (fNaI)
    {
        if (!fCaLibReader->ApplyCBcalib(fNaI))
            Error("ApplyCaLib", "An error occured during CB calibration!");
    }
    else Error("ApplyCaLib", "CB not found - could not calibrate CB using CaLib!");

    // calibrate TAPS
    if (fBaF2PWO)
    {
        if (!fCaLibReader->ApplyTAPScalib(fBaF2PWO))
            Error("ApplyCaLib", "An error occured during TAPS calibration!");
    }
    else Error("ApplyCaLib", "TAPS not found - could not calibrate TAPS using CaLib!");
    
    // calibrate PID
    if (fPID)
    {
        if (!fCaLibReader->ApplyPIDcalib(fPID))
            Error("ApplyCaLib", "An error occured during PID calibration!");
    }
    else Error("ApplyCaLib", "PID not found - could not calibrate PID using CaLib!");
    
    // calibrate Veto 
    if (fVeto)
    {
        if (!fCaLibReader->ApplyVetocalib(fVeto))
            Error("ApplyCaLib", "An error occured during Veto calibration!");
    }
    else Error("ApplyCaLib", "Veto not found - could not calibrate Veto using CaLib!");
}

//______________________________________________________________________________ 
void TA2MyPhysics::PostInit()
{
    // Do some further initialisation after all setup parameters have been read in.
 
    // call PostInit() of parent class
    TA2Physics::PostInit();

    // Show some general information
    printf("\n\n\n  ****** GENERAL INFORMATION ******\n\n");
    
    if (gAR->IsOnline()) 
    {
        printf("   ACQU data name      : %s\n", gAR->GetFileName());
        printf("   Analysis type       : Online\n");
    }
    else
    {
        printf("   ROOT input file #1  : %s\n", gAR->GetTreeFileList(0));
        printf("   Analysis type       : Offline\n");
    }

    if (fAnalysisMode == EMk1Process) printf("   Analysis input      : Raw data\n");
    else if (fAnalysisMode == EMCProcess) printf("   Analysis input      : MC data\n");
    else if (fAnalysisMode == EPhysicsProcess) printf("   Analysis input      : Physics\n");
    else printf("   Analysis input      : Unknown\n");
    printf("   Analysis class      : %s\n", gAR->GetAnalysisType());
    printf("   Analysis setup      : %s\n", gAR->GetAnalysisSetup());
    printf("   Physics class       : %s\n", gAR->GetAnalysis()->GetPhysics()->ClassName());
    printf("   Physics name        : %s\n", gAR->GetAnalysis()->GetPhysics()->GetName());
    printf("   Max ADC             : %d\n", gAR->GetMaxADC());
    printf("   Max Scaler          : %d\n", gAR->GetMaxScaler());
    printf("\n");

    // show information about the selected analysis class
    printf("\n\n\n  ****** PHYSICS ANALYSIS ******\n\n");
    printf("   Name       : %s\n", GetName());
    printf("   Class Name : %s\n", ClassName());
    printf("\n");

    // load the detectors and their components
    printf("\n\n\n  ****** DETECTOR LIST ******\n\n");
    printf("   Loaded Detectors:\n\n");
    LoadDetectors(fParent, 0);
    printf("\n");
    
    // show the software trigger settings
    printf("\n\n\n  ****** SOFTWARE TRIGGER ******\n\n");
    printf("   Total multiplicity      : %d or more\n", fTrigTotalMult);
    printf("   TAPS alone multiplicity : %d or more\n", fTrigTAPSMult);
    printf("   CB sum                  : %6.2f MeV\n", fTrigCBSum);
    printf("   CB segment threshold    : %6.2f MeV\n", fTrigCBSegThr);
    printf("   TAPS LED1 threshold     : %6.2f MeV\n", fTrigTAPSLED1Thr);
    printf("   TAPS LED2 threshold     : %6.2f MeV\n", fTrigTAPSLED2Thr);
    printf("\n");

    printf("\n\n");
     
    // print bad tagger list
    if (fBadTaggerChannels)
    {
        printf("  %d Bad tagger channels: %s", fNBadTaggerChannels, 
                                               TOSUtils::FormatArrayList(fNBadTaggerChannels, fBadTaggerChannels));
        printf("\n\n");
    }

    // p2-tagger-ratio user output
    if (fNt_P2TaggerRatio) printf("  Writing P2/Tagger ntuples\n");
    
    // assign the TAPS pointers
    if (fTAPS && fBaF2PWO)
    {
        fBaF2PWOHits     = fBaF2PWO->GetHits();
        fBaF2PWOEnergy   = fBaF2PWO->GetEnergy();
        fBaF2PWOTime     = fBaF2PWO->GetTime();

        fBaF2Pattern     = fBaF2PWO->GetBitPattern();
        fBaF2CFDPattern  = GetPatternIndex("TAPSCFDPattern", fBaF2Pattern);
        fBaF2LED1Pattern = GetPatternIndex("TAPSLED1Pattern", fBaF2Pattern);
        fBaF2LED2Pattern = GetPatternIndex("TAPSLED2Pattern", fBaF2Pattern);
        
        if (fBaF2CFDPattern >= 0)
        {
            fBaF2CFDHits = fBaF2Pattern->GetHits(fBaF2CFDPattern);
            printf("  BaF2 CFD Pattern '%s' found at index %d\n", fBaF2Pattern->GetPatternName(fBaF2CFDPattern), fBaF2CFDPattern);
        }

        if (fBaF2LED1Pattern >= 0)
        {
            fBaF2LED1Hits = fBaF2Pattern->GetHits(fBaF2LED1Pattern);
            printf("  BaF2 LED1 Pattern '%s' found at index %d\n", fBaF2Pattern->GetPatternName(fBaF2LED1Pattern), fBaF2LED1Pattern);
        }

        if (fBaF2LED2Pattern >= 0)
        {
            fBaF2LED2Hits = fBaF2Pattern->GetHits(fBaF2LED2Pattern);
            printf("  BaF2 LED2 Pattern '%s' found at index %d\n", fBaF2Pattern->GetPatternName(fBaF2LED2Pattern), fBaF2LED2Pattern);
        }

        fTAPSClusterHit  = fBaF2PWO->GetClustHit();
        fTAPSCluster     = fBaF2PWO->GetCluster();
    }
    
    // assign the CB pointers
    if (fCB && fNaI)
    {
        fCBClTime       = fNaI->GetClTimeOR();
        fNaIHits        = fNaI->GetHits();
        fNaIEnergy      = fNaI->GetEnergy();
        fNaITime        = fNaI->GetTime();

        fCBPID_Index    = fCB->GetPIDElementIndex();
        fCBPID_dE       = fCB->GetPIDEnergyDepo();
        fCBClusterHit   = fNaI->GetClustHit();
        fCBCluster      = fNaI->GetCluster();
    }
    
    // assign the PID pointers
    if (fPID)
    {
        fPIDHits    = fPID->GetHits();
        fPIDHitPos  = fPID->GetPosition();
        fPIDEnergy  = fPID->GetEnergy();
        fPIDTime    = fPID->GetTime();
    }

    // assign the Veto pointers
    if (fVeto)
    {
        fVetoHits    = fVeto->GetHits();
        fVetoEnergy  = fVeto->GetEnergy();
        fVetoTime    = fVeto->GetTime();
    }
    
    // assign the PbWO4 pointers
    if (fPbWO4)
    {
        fPbWO4Hits   = fPbWO4->GetHits();
        fPbWO4Energy = fPbWO4->GetEnergy();
        fPbWO4Time   = fPbWO4->GetTime();
    }

    // assign the TOF pointers
    if (fTOF)
    {
        fTOFBarHits       = fTOF->GetBarHits();
        fTOFBarMeanEnergy = fTOF->GetMeanEnergy();
        fTOFBarMeanTime   = fTOF->GetMeanTime();
    }

    // assign the Pb glass pointers
    if (fPbGlassApp && fPbGlass)
    {
        fPbGlassTime = fPbGlass->GetTime();
    }

    // electron beam helicity state pointer
    fCP = &fADC[6]; 

    // create histograms
    fH_EventInfo = new TH1F("EventInfo", "EventInfo", 10, 0, 10);
    fH_Corrected_Scalers = new TH1F("CorrectedScalers", "CorrectedScalers", gMyPhysics_MaxScalers, 0, gMyPhysics_MaxScalers);
    fH_Corrected_SumScalers = new TH1F("CorrectedSumScalers", "CorrectedSumScalers", gMyPhysics_MaxScalers, 0, gMyPhysics_MaxScalers);
    if (fIsMC)
    {
        fH_MCVertX = new TH1F("MCVertexX", "MCVertexX", 400, -10, 10);
        fH_MCVertY = new TH1F("MCVertexY", "MCVertexY", 400, -10, 10);
        fH_MCVertZ = new TH1F("MCVertexZ", "MCVertexZ", 400, -10, 10);
    }

    // CaLib
    if (fCaLibReader)
    {
        ApplyCaLib();
        fCaLibReader->Deconnect();
    }
    
    //CBEnergy Correction per run
    if(CBEnergyPerRunCorrection)
	{
		for(int i=0; i<fNaI->GetNelement(); i++)
		{
			fNaI->GetElement(i)->SetA1(CBEnergyPerRunCorrectionFactor * (fNaI->GetElement(i)->GetA1()));
		}
	}
}

//______________________________________________________________________________ 
void TA2MyPhysics::LoadVariable()
{
    // Register class variables within AcquRoot.

    // variable for event saving into the reduced AcquRoot ROOT file 
    TA2DataManager::LoadVariable("SaveEvent", &fSaveEvent, EISingleX);
}

//______________________________________________________________________________ 
void TA2MyPhysics::Reconstruct()
{
    // AcquRoot Reconstruct() method.
 

    ////////////////////////////////////////////////////////////////////////////
    // Set detector event-dependent variables                                 //
    ////////////////////////////////////////////////////////////////////////////
 
    // do not save current event in reduced AcquRoot ROOT file by default
    fSaveEvent = 0;
    

    ////////////////////////////////////////////////////////////////////////////
    // Set detector event-dependent variables                                 //
    ////////////////////////////////////////////////////////////////////////////
    
    if (fNaI)
    {
        fCBNCluster = fNaI->GetNCluster();
        fNaINhits   = fNaI->GetNhits();
    }

    if (fBaF2PWO)
    {
        fBaF2PWONhits = fBaF2PWO->GetNhits();
        fTAPSNCluster = fBaF2PWO->GetNCluster();
    }

    if (fBaF2CFDHits) fBaF2CFDNhits = fBaF2Pattern->GetNHits()[fBaF2CFDPattern];
    if (fBaF2LED1Hits) fBaF2LED1Nhits = fBaF2Pattern->GetNHits()[fBaF2LED1Pattern];
    if (fBaF2LED2Hits) fBaF2LED2Nhits = fBaF2Pattern->GetNHits()[fBaF2LED2Pattern];

    if (fPID)
    {
        fPIDNhits = fPID->GetNhits();
    }

    if (fVeto)
    {
        fVetoNhits = fVeto->GetNhits();
    }

    if (fPbWO4)
    {
        fPbWO4Nhits = fPbWO4->GetNhits();
    }

    if (fTOF)
    {
        fTOFBarNhits = fTOF->GetNBarHits();
    }

    if (fPbGlass)
    {
        fPbGlassNhits = fPbGlass->GetNhits();
    }

    if (fCP)
    {
        UShort_t eHS = *fCP;
        if (eHS == fEBeamBitNeg) fEBeamHelState = -1;
        else if (eHS == fEBeamBitPos) fEBeamHelState = 1;
        else fEBeamHelState = 0;
    }
    else fEBeamHelState = 0;
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Read-in hardware trigger                                               //
    ////////////////////////////////////////////////////////////////////////////
  
    // Level 1 trigger
    (fADC[0] & 0x01) ? fL1CB = kTRUE : fL1CB = kFALSE;
    (fADC[0] & 0x08) ? fL1TAPS_OR = kTRUE : fL1TAPS_OR = kFALSE;
    (fADC[0] & 0x10) ? fL1TAPS_Pulser = kTRUE : fL1TAPS_Pulser = kFALSE;
    (fADC[0] & 0x40) ? fL1TAPS_M2 = kTRUE : fL1TAPS_M2 = kFALSE;
    (fADC[0] & 0x80) ? fL1Pulser = kTRUE : fL1Pulser = kFALSE;
    
    // Level 2 trigger ("Normal" ordering, Feb 2009, Nov 2010)
    //(fADC[1] & 0x10) ? fL2M1 = kTRUE : fL2M1 = kFALSE;
    //(fADC[1] & 0x20) ? fL2M2 = kTRUE : fL2M2 = kFALSE;
    //(fADC[1] & 0x40) ? fL2M3 = kTRUE : fL2M3 = kFALSE;
    //(fADC[1] & 0x80) ? fL2M4 = kTRUE : fL2M4 = kFALSE;

    // Level 2 trigger ("Strange" ordering, May 2009)
    (fADC[1] & 0x80) ? fL2M1 = kTRUE : fL2M1 = kFALSE;
    (fADC[1] & 0x10) ? fL2M2 = kTRUE : fL2M2 = kFALSE;
    (fADC[1] & 0x20) ? fL2M3 = kTRUE : fL2M3 = kFALSE;
    (fADC[1] & 0x40) ? fL2M4 = kTRUE : fL2M4 = kFALSE;


    ////////////////////////////////////////////////////////////////////////////
    // Read-in MC information                                                 //
    ////////////////////////////////////////////////////////////////////////////
    
    if (fIsMC)
    {
        // get vertex
        Float_t* vert = (Float_t*) (fEvent[EI_vertex]);
    
        // set vertex
        fMCVertX = vert[0];
        fMCVertY = vert[1];
        fMCVertZ = vert[2];
    
        // fill histograms
        fH_MCVertX->Fill(fMCVertX);
        fH_MCVertY->Fill(fMCVertY);
        fH_MCVertZ->Fill(fMCVertZ);
    }

 
    ////////////////////////////////////////////////////////////////////////////
    // Read-in tagger hits                                                    //
    ////////////////////////////////////////////////////////////////////////////
     
    if (fTagger && fLadder)
    {
        // get the electron beam energy
        Double_t eBeamEnergy = fTagger->GetBeamEnergy();
        
        // get the channel electron energies
        const Double_t* fpdEnergy = fLadder->GetECalibration();

        // data or MC handling
        if (fIsMC)
        {
            // get tagger hits and the time array
            fTaggerPhotonNhits = fLadder->GetNhits();
            Int_t* hits = fLadder->GetHits();
            Double_t* time = fLadder->GetTimeOR();
            
            // create hit, energy and time arrays
            fTaggerPhotonHits = new Int_t[fTaggerPhotonNhits];
            fTaggerPhotonEnergy = new Double_t[fTaggerPhotonNhits];
            fTaggerPhotonTime = new Double_t[fTaggerPhotonNhits];
            
            // loop over tagger hits
            for (UInt_t i = 0; i < fTaggerPhotonNhits; i++)
            {
                fTaggerPhotonHits[i] = hits[i];
                fTaggerPhotonEnergy[i] = eBeamEnergy - fpdEnergy[hits[i]];
                fTaggerPhotonTime[i] = time[i];
            }
        }
        else
        {
            // get the tagger hit multiplicity
            Int_t m = fLadder->GetNMultihit();

            // count total number of hits including all hit multiplicities
            fTaggerPhotonNhits = 0;
            for (Int_t i = 0; i < m; i++) fTaggerPhotonNhits += fLadder->GetNhitsM(i);
            
            // create hit, energy and time arrays
            fTaggerPhotonHits = new Int_t[fTaggerPhotonNhits];
            fTaggerPhotonEnergy = new Double_t[fTaggerPhotonNhits];
            fTaggerPhotonTime = new Double_t[fTaggerPhotonNhits];
            
            // read-in all the hits
            // loop over hit multiplicity
            Int_t currHit = 0;
            for (Int_t i = 0; i < m; i++)
            {   
                // number of hits of current multiplicity
                Int_t nhits = fLadder->GetNhitsM(i);

                // hit array of current multiplicity
                Int_t* hits = fLadder->GetHitsM(i);

                // time array of current multiplicity
                Double_t* time = fLadder->GetTimeORM(i);

                // loop over hits of current multiplicity
                for (Int_t j = 0; j < nhits; j++)
                {
                    // set hit element, time and energy
                    fTaggerPhotonHits[currHit] = hits[j];
                    fTaggerPhotonEnergy[currHit] = eBeamEnergy - fpdEnergy[hits[j]];
                    fTaggerPhotonTime[currHit] = time[j];
                    currHit++;
                }
            } // for: hit multiplicity loop
        } // if: data handling
    }


    ////////////////////////////////////////////////////////////////////////////
    // Prepare particle collection                                            //
    ////////////////////////////////////////////////////////////////////////////
    
    // detector particle arrays
    fPartCB = new TOA2DetParticle*[fCBNCluster];
    fPartTAPS = new TOA2DetParticle*[fTAPSNCluster];
    for (UInt_t i = 0; i < fCBNCluster; i++) fPartCB[i] = new TOA2DetParticle();
    for (UInt_t i = 0; i < fTAPSNCluster; i++) fPartTAPS[i] = new TOA2DetParticle();
    
    // neutral/charged particle arrays
    fPartNeutral = new TOA2DetParticle*[fCBNCluster + fTAPSNCluster];
    fPartCharged = new TOA2DetParticle*[fCBNCluster + fTAPSNCluster];
    
    // number of neutral/charged particles
    fNNeutral = 0;
    fNCharged = 0;


    ////////////////////////////////////////////////////////////////////////////
    // Collect particles in CB                                                //
    ////////////////////////////////////////////////////////////////////////////
    
    for (UInt_t i = 0; i < fCBNCluster; i++)
    {
        // get cluster
        HitCluster_t* cl = fCBCluster[fCBClusterHit[i]];
        Int_t clNhits    = cl->GetNhits();
        UInt_t* clHits   = cl->GetHits();
        Int_t center     = cl->GetIndex();

        // set particle properties
        fPartCB[i]->SetDetector(kCBDetector);
        fPartCB[i]->SetPosition(cl->GetMeanPosition());
        fPartCB[i]->SetZ(fPartCB[i]->GetZ() - fTargetPosition);
        fPartCB[i]->SetEnergy(cl->GetEnergy());
        fPartCB[i]->SetTime(cl->GetTime() + fCBTimeOffset);
        fPartCB[i]->SetCentralElement(center);
        fPartCB[i]->SetCentralEnergy(cl->GetEnergy()*cl->GetCentralFrac());
        fPartCB[i]->SetClusterSize(clNhits);
        fPartCB[i]->SetClusterHits(clNhits, clHits);
        Double_t clHitE[clNhits];
        Double_t clHitT[clNhits];
        for (Int_t j = 0; j < clNhits; j++) 
        {
            clHitE[j] = fNaIEnergy[clHits[j]];
            clHitT[j] = fNaITime[clHits[j]];
        }
        fPartCB[i]->SetClusterHitEnergies(clNhits, clHitE);
        fPartCB[i]->SetClusterHitTimes(clNhits, clHitT);

        // link neutral/charged lists and fill charged particle properties
        Int_t pid_index;
        Double_t pid_energy = CheckClusterPID(cl, &pid_index);
        if (pid_energy > 0) 
        {
            // perform droop correction
            if (fCaLibReader)
            {
                if (fCaLibReader->GetPIDdroop())
                {   
                    // get PID element pedestal and gain
                    Double_t pid_ped = fPID->GetElement(pid_index)->GetA0();
                    Double_t pid_gain = fPID->GetElement(pid_index)->GetA1();

                    // de-calibrate to raw ADC channel value
                    Double_t ch = pid_energy / pid_gain + pid_ped;
                    
                    // get theta of cluster
                    Double_t pid_theta = fPartCB[i]->GetTheta() * TMath::RadToDeg();

                    // normalize to energy at 45 degrees
                    ch *= fCaLibReader->GetPIDDroopFunc(pid_index)->Eval(45) /
                          fCaLibReader->GetPIDDroopFunc(pid_index)->Eval(pid_theta);

                    // calibrate
                    pid_energy = pid_gain * (ch - pid_ped);
                }
            }
            
            // set PID index
            fPartCB[i]->SetPIDIndex(pid_index);

            // set PID energy
            fPartCB[i]->SetPIDEnergy(pid_energy);
            
            // set PID time
            if (fPIDTime) fPartCB[i]->SetPIDTime(fPIDTime[pid_index]);

            // link particle to charged list
            fPartCharged[fNCharged++] = fPartCB[i];
        }
        else 
        {
            // perform quadratic energy correction
            if (fCaLibReader)
            {
                if (fCaLibReader->GetCBquadEnergy())
                {
                    Double_t energy = fPartCB[i]->GetEnergy();
                    fPartCB[i]->SetEnergy(fCaLibReader->GetCBQuadEnergyPar0(center)*energy + 
                                          fCaLibReader->GetCBQuadEnergyPar1(center)*energy*energy);
                }
            }

            // link particle to neutral list
            fPartNeutral[fNNeutral++] = fPartCB[i];
        }
    } 

    
    ////////////////////////////////////////////////////////////////////////////
    // Collect particles in TAPS                                              //
    ////////////////////////////////////////////////////////////////////////////
    
    for (UInt_t i = 0; i < fTAPSNCluster; i++)
    {
        // get cluster
        HitCluster_t* cl = fTAPSCluster[fTAPSClusterHit[i]];
        Int_t clNhits    = cl->GetNhits();
        UInt_t* clHits   = cl->GetHits();
        Int_t center     = cl->GetIndex();

        // set particle properties
        fPartTAPS[i]->SetDetector(kTAPSDetector);
        fPartTAPS[i]->SetPosition(cl->GetMeanPosition());
        fPartTAPS[i]->SetZ(fPartTAPS[i]->GetZ() - fTargetPosition);
        fPartTAPS[i]->SetEnergy(cl->GetEnergy());
        fPartTAPS[i]->SetTime(cl->GetTime() + fTAPSTimeOffset);
        fPartTAPS[i]->SetCentralElement(center);
        fPartTAPS[i]->SetCentralEnergy(cl->GetEnergy()*cl->GetCentralFrac());
        fPartTAPS[i]->SetClusterSize(clNhits);
        fPartTAPS[i]->SetClusterHits(clNhits, clHits);
        Double_t clHitE[clNhits];
        Double_t clHitT[clNhits];
        for (Int_t j = 0; j < clNhits; j++) 
        {
            clHitE[j] = fBaF2PWOEnergy[clHits[j]];
            clHitT[j] = fBaF2PWOTime[clHits[j]];
        }
        fPartTAPS[i]->SetClusterHitEnergies(clNhits, clHitE);
        fPartTAPS[i]->SetClusterHitTimes(clNhits, clHitT);
        fPartTAPS[i]->SetCentralSGEnergy(fBaF2PWO->GetSGEnergy(cl->GetIndex()));
        
        // link neutral/charged lists and fill charged particle properties
        Int_t veto_index;
        Double_t veto_energy = CheckClusterVeto(cl, &veto_index);
        if (veto_energy > 0) 
        {
            // set Veto index
            fPartTAPS[i]->SetVetoIndex(veto_index);

            // set Veto energy
            fPartTAPS[i]->SetVetoEnergy(veto_energy);
            
            // set Veto time
            if (fVetoTime) fPartTAPS[i]->SetVetoTime(fVetoTime[veto_index]);

            // link particle to charged list
            fPartCharged[fNCharged++] = fPartTAPS[i];
        }
        else 
        {
            // perform quadratic energy correction
            if (fCaLibReader)
            {
                if (fCaLibReader->GetTAPSquadEnergy())
                {
                    Double_t energy = fPartTAPS[i]->GetEnergy();
                    fPartTAPS[i]->SetEnergy(fCaLibReader->GetCBQuadEnergyPar0(GetTAPSRing(center)-1)*energy + 
                                            fCaLibReader->GetCBQuadEnergyPar1(GetTAPSRing(center)-1)*energy*energy);
                }
            }

            // link particle to neutral list
            fPartNeutral[fNNeutral++] = fPartTAPS[i];
        }
    } 
    

    ////////////////////////////////////////////////////////////////////////////
    // Prepare MC particle collection                                         //
    ////////////////////////////////////////////////////////////////////////////
    
    if (fIsMC) 
    {
        // get momentum branch and number of tree events
        TBranch* b = gAR->GetBranch(EI_dircos);
        
        // get number of particles
        fNMC = *(Int_t*) (fEvent[EI_npart]);
        
        // read particle IDs and 4-momenta
        Int_t* g3_ID = (Int_t*) (fEvent[EI_idpart]);
        Float_t* etot = (Float_t*) (fEvent[EI_elab]);
        Float_t* ptot = (Float_t*) (fEvent[EI_plab]);
        Float_t dircos[fNMC][3];
        b->SetAddress(dircos);
        b->GetEntry(fEventCounter - fEventOffset);

        // create particles and set their properties
        for (Int_t i = 0; i < fNMC; i++)
        {
            // set particle ID
            fPartMC[i]->SetGeant3_ID(g3_ID[i]);
        
            // 4-momentum (convert to MeV)
            fPartMC[i]->SetP4(1000.*ptot[i]*dircos[i][0],
                              1000.*ptot[i]*dircos[i][1],
                              1000.*ptot[i]*dircos[i][2],
                              1000.*etot[i]);
        }

        // information output at first event
        if (fEventCounter == 0)
        {
            printf("\n\n\n  ****** MC PARTICLE COLLECTION ******\n\n");
            printf("   Found %d MC particles:\n\n", fNMC);
            for (Int_t i = 0; i < fNMC; i++) 
                printf("   %2d. %-22s (PDG: %5d   G3: %2d)\n", i+1, fPartMC[i]->GetName(), 
                       fPartMC[i]->GetPDG_ID(), fPartMC[i]->GetGeant3_ID());
            printf("\n");
            printf("\n");
        }
        
        // update event offset when at last event of current tree
        Long64_t tevents = b->GetEntries();
        if (fEventCounter - fEventOffset == tevents - 1) fEventOffset += tevents;
    }


    ////////////////////////////////////////////////////////////////////////////
    // Update corrected sum scaler histogram                                  //
    ////////////////////////////////////////////////////////////////////////////
    
    if (gAR->IsScalerRead())
    {
        // update clock scalers
        UpdateCorrectedScaler(0);
        UpdateCorrectedScaler(1);
        UpdateCorrectedScaler(144);
        UpdateCorrectedScaler(145);
            
        // update P2 scaler 
        if (fNt_P2TaggerRatio && fScalerSum[151] != fOldP2ScalerSum)
        {
            Double_t ratio, ratio_err, tagger; 
            
            // correct free-running p2 for total DAQ livetime
            Double_t sc_0 = fH_Corrected_Scalers->GetBinContent(1);
            Double_t sc_1 = fH_Corrected_Scalers->GetBinContent(2);
            Double_t p2 = sc_1 / sc_0 * (Double_t)fScaler[151];
            
            // channel wise
            TH1F* hSc = (TH1F*) gDirectory->Get("FPD_ScalerCurr");
            for (Int_t i = 0; i < 352; i++)
            {
                tagger = hSc->GetBinContent(i+1);
                ratio = p2 / tagger;
                ratio_err = ratio * TMath::Sqrt(1./p2 + 1./tagger + 1./sc_0 + 1./sc_1);
                fNt_P2TaggerRatio[i]->Fill(ratio, ratio_err);
            }
            
            // update old values
            fOldP2ScalerSum = fScalerSum[151];
        }
    }
 

    ////////////////////////////////////////////////////////////////////////////
    // Fill event information                                                 //
    ////////////////////////////////////////////////////////////////////////////
    
    // count event
    fH_EventInfo->Fill(0);

    // fill beam helicity
    if (fEBeamHelState == 1)       fH_EventInfo->Fill(1);
    else if (fEBeamHelState == -1) fH_EventInfo->Fill(2);
    else if (fEBeamHelState ==  0) fH_EventInfo->Fill(3);

    
    ////////////////////////////////////////////////////////////////////////////
    // Call virtual reconstruction method of child class                      //
    ////////////////////////////////////////////////////////////////////////////
    
    ReconstructPhysics();


    ////////////////////////////////////////////////////////////////////////////
    // Clean-up memory                                                        //
    ////////////////////////////////////////////////////////////////////////////
    
    // tagger hit arrays
    delete [] fTaggerPhotonHits;
    delete [] fTaggerPhotonEnergy;
    delete [] fTaggerPhotonTime;

    // detector particle arrays
    for (UInt_t i = 0; i < fCBNCluster; i++) delete fPartCB[i];
    for (UInt_t i = 0; i < fTAPSNCluster; i++) delete fPartTAPS[i];
    delete [] fPartCB;
    delete [] fPartTAPS;

    // neutral/charged particle arrays
    delete [] fPartNeutral;
    delete [] fPartCharged;

    
    ////////////////////////////////////////////////////////////////////////////
    // Increment event counter                                                //
    ////////////////////////////////////////////////////////////////////////////
    
    fEventCounter++;
}

//______________________________________________________________________________
Bool_t TA2MyPhysics::IsBadTaggerChannel(Int_t ch)
{
    // Check if the tagger channel 'ch' is in the list of bad tagger channels.
    // Return kTRUE if yes, otherwise or if no bad tagger channel list was
    // specified kFALSE;

    // loop over bad tagger channel list
    for (Int_t i = 0; i < fNBadTaggerChannels; i++)
    {
        if (ch == fBadTaggerChannels[i]) return kTRUE;
    }
    return kFALSE;
}

//______________________________________________________________________________
Bool_t TA2MyPhysics::IsTrigger(TOA2RecParticle& meson)
{
    // Software trigger method.
    // Check the trigger for the reconstructed meson 'meson'.

    // init variables
    Double_t cbSum = 0.;
    Int_t multCB = 0;
    Int_t multTAPS_LED1 = 0;
    Int_t multTAPS_LED2 = 0;

    // create and init the CB segment pattern
    Bool_t segPat[45];
    for (Int_t i = 0; i < 45; i++) segPat[i] = kFALSE;
    
    // create and init the TAPS block trigger pattern
    Bool_t tapsLED1Pat[6] = { kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE };
    Bool_t tapsLED2Pat[6] = { kFALSE, kFALSE, kFALSE, kFALSE, kFALSE, kFALSE };
    
    // check if LED values were read using CaLib
    Bool_t useCaLibLED = kFALSE;
    if (fCaLibReader)
    {
        if (fCaLibReader->GetTAPSled()) useCaLibLED = kTRUE;
    }

    // get number of decay particles
    Int_t nHits = meson.GetNDetectedProducts();
    
    // get particles
    TOA2DetParticle** part = meson.GetDetectedProducts();

    // loop over all particles
    for (Int_t i = 0; i < nHits; i++)
    {
        // particles in CB
        if (part[i]->GetDetector() == kCBDetector)
        {
            // get cluster size and hits
            Int_t clNhits    = part[i]->GetClusterSize();
            UInt_t* clHits   = part[i]->GetClusterHits();
            Double_t* clHitE = part[i]->GetClusterHitEnergies();
            
            // loop over all cluster hits
            for (Int_t j = 0; j < clNhits; j++)
            {
                // calculate the segment index
                Int_t seg = (Int_t)(clHits[j] / 16);

                // check NaI segment high discriminator threshold
                if (clHitE[j] > fTrigCBSegThr) segPat[seg] = kTRUE;

                // add energy to total sum
                cbSum += clHitE[j];
            }
        }

        // particles in TAPS
        if (part[i]->GetDetector() == kTAPSDetector)
        {
            // get the energy of the central detector
            Double_t centE = part[i]->GetCentralEnergy();
            
            // get the central element
            Int_t centElem = part[i]->GetCentralElement();

            // calculate the block index
            Int_t block = GetTAPSBlock(centElem);
            
            // check LED threshold either using CaLib or the block variables
            if (useCaLibLED)
            {
                // check LED1 trigger condition
                if (centE > fCaLibReader->GetTAPSLED1Thr(centElem)) tapsLED1Pat[block-1] = kTRUE;

                // check LED2 trigger condition
                if (centE > fCaLibReader->GetTAPSLED2Thr(centElem)) tapsLED2Pat[block-1] = kTRUE;
            }
            else
            {
                // check LED1 trigger condition
                if (centE > fTrigTAPSLED1Thr) tapsLED1Pat[block] = kTRUE;
            
                // check LED2 trigger condition
                if (centE > fTrigTAPSLED2Thr) tapsLED2Pat[block] = kTRUE;
            }
        }
    }

    //
    // count multiplicities
    //

    // loop over CB segments
    for (Int_t i = 0; i < 45; i++)
    {
        // count segments that fulfilled the segment high discriminator threshold
        if (segPat[i]) multCB++;
    }

    // loop over TAPS blocks
    for (Int_t i = 0; i < 6; i++)
    {
        // add blocks that fired the LED1 to the total multiplicity
        if (tapsLED1Pat[i]) multTAPS_LED1++;
        
        // add blocks that fired the LED2 to the TAPS alone multiplicity
        if (tapsLED2Pat[i]) multTAPS_LED2++;
    }

    //
    // make trigger decision
    //

    //printf("CB sum: %f   CB mult: %d   TAPS mult: %d\n", cbSum, multCB, multTAPS_LED1);
    //UShort_t l1 = fADC[0];
    //UShort_t l2 = fADC[1];
    //if (l1 & 0x01) printf("CBsum  ");
    //if (l1 & 0x08) printf("TAPSOR  ");
    //if (l2 & 0x10) printf("M1  ");
    //if (l2 & 0x20) printf("M2  ");
    //if (l2 & 0x40) printf("M3  ");
    //if (l2 & 0x80) printf("M4  ");
    //printf("\n\n");

    // CB sum + total multiplicity
    Int_t multTotal = multCB + multTAPS_LED1;
    if (cbSum > fTrigCBSum && multTotal >= fTrigTotalMult) return kTRUE;

    // TAPS alone
    if (multTAPS_LED2 >= fTrigTAPSMult) return kTRUE;

    // no trigger here
    return kFALSE;
}

//______________________________________________________________________________
void TA2MyPhysics::ClearCBTimeWalk()
{
    // Set the time walk parameters of all CB elements to zero.
    
    Double_t zeroWalk[4] = { 0, 0, 0, 0 };

    // check if NaI cluster detector is here
    if (fNaI)
    {
        Int_t nelem = fNaI->GetNelement();
        for (Int_t i = 0; i < nelem; i++) fNaI->GetElement(i)->GetTimeWalk()->SetWalk(zeroWalk);
    }
    else Error("ClearCBTimeWalk", "Could not (yet) find the NaI cluster detector"
               " to clear the time walk values!");
}

//______________________________________________________________________________
Double_t TA2MyPhysics::GetBGSubtractionWeight(TOA2RecParticle& meson, 
                                              Double_t taggerTime,
                                              Double_t limitPromptLowCB, Double_t limitPromptHighCB,
                                              Double_t limitBG1LowCB, Double_t limitBG1HighCB,
                                              Double_t limitBG2LowCB, Double_t limitBG2HighCB,
                                              Double_t limitPromptLowTAPS, Double_t limitPromptHighTAPS,
                                              Double_t limitBG1LowTAPS, Double_t limitBG1HighTAPS,
                                              Double_t limitBG2LowTAPS, Double_t limitBG2HighTAPS)
{
    // Return the weight for the tagger background subtraction using
    // - the detected meson 'meson'
    // - the tagger time 'taggerTime'
    // - the limits for the prompt region 'limitPromptLowCB/TAPS' and 'limitPromptHighCB/TAPS'
    // - the limits for the background region 1 'limitBG1LowCB/TAPS' and 'limitBG1HighCB/TAPS'
    // - the limits for the background region 2 'limitBG2LowCB/TAPS' and 'limitBG2HighCB/TAPS'

    // calculate the coincidence time taking into account the detector time
    // of the meson to remove the trigger time
    
    // meson time from TAPS
    if (meson.GetNDetectorHits(kTAPSDetector)) 
    {
        // calculate coincidence time
        Double_t coincTime = meson.GetTime() + taggerTime;
    
        // check for the prompt region (MC events are always considered prompt)
        if ((coincTime > limitPromptLowTAPS && coincTime < limitPromptHighTAPS) ||
            fIsMC) return 1.;
        // check for background region
        else if ((coincTime > limitBG1LowTAPS && coincTime < limitBG1HighTAPS) ||
                 (coincTime > limitBG2LowTAPS && coincTime < limitBG2HighTAPS))
                    return - (limitPromptHighTAPS - limitPromptLowTAPS) / 
                             (limitBG1HighTAPS - limitBG1LowTAPS + limitBG2HighTAPS - limitBG2LowTAPS);
        // useless background here
        return 0.;
    }
    // meson time from CB
    else 
    {
        // calculate coincidence time
        Double_t coincTime = meson.GetTime() - taggerTime;
        
        // check for the prompt region (MC events are always considered prompt)
        if ((coincTime > limitPromptLowCB && coincTime < limitPromptHighCB) ||
            fIsMC) return 1.;
        // check for background region
        else if ((coincTime > limitBG1LowCB && coincTime < limitBG1HighCB) ||
                 (coincTime > limitBG2LowCB && coincTime < limitBG2HighCB))
                    return - (limitPromptHighCB - limitPromptLowCB) / 
                             (limitBG1HighCB - limitBG1LowCB + limitBG2HighCB - limitBG2LowCB);
        // useless background here
        return 0.;
     }
}

//______________________________________________________________________________
void TA2MyPhysics::FillTaggerCoincidence(TOA2RecParticle& meson, Double_t taggerTime, TH1* hCB, TH1* hTAPS)
{
    // Fill the tagger coincidence time of the meson 'meson' using the tagger time 'taggerTime'
    // depending of the detector the meson time was calculated from into the histograms
    // 'hCB' or 'hTAPS', respectively.

    // meson time from TAPS
    if (meson.GetNDetectorHits(kTAPSDetector)) 
    {
        // calculate coincidence time
        Double_t coincTime = meson.GetTime() + taggerTime;
        if (hTAPS) hTAPS->Fill(coincTime);
    }
    // meson time from CB
    else 
    {
        // calculate coincidence time
        Double_t coincTime = meson.GetTime() - taggerTime;
        if (hCB) hCB->Fill(coincTime);
    }
}

//______________________________________________________________________________
void TA2MyPhysics::FillTaggerCoincidence2D(TOA2RecParticle& meson, Double_t taggerTime, 
                                           Int_t taggerElement, TH2* hCB, TH2* hTAPS)
{
    // Fill the tagger coincidence time of the meson 'meson' using the tagger time 'taggerTime'
    // against the tagged element 'taggerElement' depending of the detector the meson time was 
    // calculated from into the histogram 'hCB' or 'hTAPS', respectively.

    // meson time from TAPS
    if (meson.GetNDetectorHits(kTAPSDetector)) 
    {
        // calculate coincidence time
        Double_t coincTime = meson.GetTime() + taggerTime;
        if (hTAPS) hTAPS->Fill(coincTime, taggerElement);
    }
    // meson time from CB
    else 
    {
        // calculate coincidence time
        Double_t coincTime = meson.GetTime() - taggerTime;
        if (hCB) hCB->Fill(coincTime, taggerElement);
    }
}
 
//______________________________________________________________________________
void TA2MyPhysics::UpdateCorrectedScaler(Int_t sc)
{
    // Update the entry of the scaler 'sc' in the corrected scaler histogram.
    // IMPORTANT: Call this function only during a scaler-read event, i.e.
    // after checking gAR->IsScalerRead().

    // check scaler number
    if (sc >= gMyPhysics_MaxScalers)
    {
        Error("UpdateCorrectedScaler", "Scaler %d is larger than max. scaler value!", sc);
        return;
    }

    // check previous sum scaler entry to avoid double counting
    // do not do this in offline ROOT file input mode
    if ((gAR->IsOnline() && fScalerSum[sc] != fOldScalerSumValue[sc]) ||
        !gAR->IsOnline())
    {
        // free running, overflow vulnerable 24-bit scaler (total)
        if (sc == 0)
        {
            UInt_t scaler_0;
            if (fScaler[0] < fScaler[1]) scaler_0 = fScaler[0] + 16777216;
            else scaler_0 = fScaler[0];
            fH_Corrected_Scalers->SetBinContent(1, scaler_0);
            fH_Corrected_SumScalers->AddBinContent(1, scaler_0);
        }
        // free running, overflow vulnerable 24-bit scaler (tagger)
        else if (sc == 144)
        {
            UInt_t scaler_144;
            if (fScaler[144] < fScaler[145]) scaler_144 = fScaler[144] + 16777216;
            else scaler_144 = fScaler[144];
            fH_Corrected_Scalers->SetBinContent(145, scaler_144);
            fH_Corrected_SumScalers->AddBinContent(145, scaler_144);
        }
        else 
        {
            fH_Corrected_Scalers->SetBinContent(sc+1, fScaler[sc]);
            fH_Corrected_SumScalers->AddBinContent(sc+1, fScaler[sc]);
        }

        // update old sum scaler value
        fOldScalerSumValue[sc] = fScalerSum[sc];    
    }
}

//______________________________________________________________________________
inline Int_t TA2MyPhysics::GetVetoInFrontOfElement(Int_t id) const
{
    // Return the index of the veto that is installed in front of the
    // BaF2 or PWO element with the index 'id'.
    
    // check TAPS setup
    switch (fBaF2PWO->GetType())
    {
        case EBaF2:
        {
            return id;
        }
        case EBaF2_PWO_08:
        {
            // 1st PWO ring
            if (id >=   0 && id <=   3) return   0;
            if (id >=  67 && id <=  70) return  64;
            if (id >= 134 && id <= 137) return 128;
            if (id >= 201 && id <= 204) return 192;
            if (id >= 268 && id <= 271) return 256;
            if (id >= 335 && id <= 338) return 320;
            
            // other elements
            else return id - 3*(id/67 + 1);
        }
        case EBaF2_PWO_09:
        {
	    if ((fVeto->GetNelement()) == 384)
	    {
	        // 1st PWO ring
	        if (id >=   0 && id <=   3) return   0;
		if (id >=  73 && id <=  76) return  64;
		if (id >= 146 && id <= 149) return 128;
		if (id >= 219 && id <= 222) return 192;
		if (id >= 292 && id <= 295) return 256;
		if (id >= 365 && id <= 368) return 320;
            
		// 2nd PWO ring
		if (id >=   4 && id <=   7) return   1;
		if (id >=   8 && id <=  11) return   2;
		if (id >=  77 && id <=  80) return  65;
		if (id >=  81 && id <=  84) return  66;
		if (id >= 150 && id <= 153) return 129;
		if (id >= 154 && id <= 157) return 130;
		if (id >= 223 && id <= 226) return 193;
		if (id >= 227 && id <= 230) return 194;
		if (id >= 296 && id <= 299) return 257;
		if (id >= 300 && id <= 303) return 258;
		if (id >= 369 && id <= 372) return 321;
		if (id >= 373 && id <= 376) return 322;
		
		// other elements
		else return id - 9*(id/73 + 1);
	    }
	    else return id;
        }
        default:
        {
            Error("GetVetoInFrontOfElement", "TAPS setup could not be identified!");
            return 0;
        }
    }
}

//______________________________________________________________________________
Int_t TA2MyPhysics::GetTAPSRing(Int_t id) const
{
    // Return the number of the ring (1 to 11) the TAPS element
    // 'id' belongs to.
    
    // element layout (first and last elements of a ring)
    Int_t ringRange[11][2] = {{0, 0}, {1, 2}, {3, 5}, {6, 9}, {10, 14}, {15, 20},
                              {21, 27}, {28, 35}, {36, 44}, {45, 54}, {55, 63}};
             
    // get corresponding TAPS 2007 element number
    Int_t elem = GetVetoInFrontOfElement(id);
    
    // map element to first block
    Int_t mid = elem % 64;

    if      (mid >= ringRange[0][0]  && mid <= ringRange[0][1])  return 1;
    else if (mid >= ringRange[1][0]  && mid <= ringRange[1][1])  return 2;
    else if (mid >= ringRange[2][0]  && mid <= ringRange[2][1])  return 3;
    else if (mid >= ringRange[3][0]  && mid <= ringRange[3][1])  return 4;
    else if (mid >= ringRange[4][0]  && mid <= ringRange[4][1])  return 5;
    else if (mid >= ringRange[5][0]  && mid <= ringRange[5][1])  return 6;
    else if (mid >= ringRange[6][0]  && mid <= ringRange[6][1])  return 7;
    else if (mid >= ringRange[7][0]  && mid <= ringRange[7][1])  return 8;
    else if (mid >= ringRange[8][0]  && mid <= ringRange[8][1])  return 9;
    else if (mid >= ringRange[9][0]  && mid <= ringRange[9][1])  return 10;
    else if (mid >= ringRange[10][0] && mid <= ringRange[10][1]) return 11;
    else return 99;
}

//______________________________________________________________________________
Int_t TA2MyPhysics::GetTAPSBlock(Int_t id) const
{
    // Return the number of the block (1 to 6) the TAPS element
    // 'id' belongs to.
    
    // check TAPS setup
    switch (fBaF2PWO->GetType())
    {
        case EBaF2:
        {
            return (id / 64) + 1;
        }
        case EBaF2_PWO_08:
        {
            return (id / 67) + 1;
        }
        case EBaF2_PWO_09:
        {
            return (id / 73) + 1;
        }
        default:
        {
            Error("GetTAPSBlock", "TAPS setup could not be identified!");
            return 0;
        }
    }
}

//______________________________________________________________________________
Double_t TA2MyPhysics::CheckClusterVeto(HitCluster_t* inCluster, Int_t* outVetoIndex) const
{
    // Return the maximum energy deposited in one of the veto detectors of the
    // TAPS cluster 'inCluster'.
    // Returns 0 if no cluster veto fired.
    //
    // If 'outVetoIndex' is provided write the index of the corresponding veto
    // element to that variable (-1 if no cluster veto fired).
    
    Int_t center        = inCluster->GetIndex();
    UInt_t nNeighbours  = inCluster->GetNNeighbour();
    UInt_t* neighbours  = inCluster->GetNeighbour();
        
    Double_t maxVetoEnergy = 0;
    if (outVetoIndex) *outVetoIndex = -1;

    // loop over all veto hits
    for (UInt_t i = 0; i < fVetoNhits; i++)
    {
        // check central element
        if (fVetoHits[i] == GetVetoInFrontOfElement(center))
        {
            if (fVetoEnergy[fVetoHits[i]] > maxVetoEnergy) 
            {
                maxVetoEnergy = fVetoEnergy[fVetoHits[i]]
                                * TMath::Abs(TMath::Cos(inCluster->GetTheta() * TMath::DegToRad()));
                if (outVetoIndex) *outVetoIndex = fVetoHits[i];
            }
        }

        // loop over all neighbour elements
        for (UInt_t j = 0; j < nNeighbours; j++)
        {
            if (fVetoHits[i] == GetVetoInFrontOfElement((Int_t)neighbours[j]))
            {
                if (fVetoEnergy[fVetoHits[i]] > maxVetoEnergy) 
                {
                    maxVetoEnergy = fVetoEnergy[fVetoHits[i]]
                                    * TMath::Abs(TMath::Cos(inCluster->GetTheta() * TMath::DegToRad()));
                    if (outVetoIndex) *outVetoIndex = fVetoHits[i];
                }
            }
        }
    }

    return maxVetoEnergy;
}

//______________________________________________________________________________
Double_t TA2MyPhysics::CheckClusterPID(HitCluster_t* inCluster, Int_t* outPIDIndex) const
{
    // Return the theta-weighted energy deposited in the coincident PID element
    // of the CB cluster 'inCluster'.
    // Returns 0 if no coincident PID element fired.
    //
    // If 'outPIDIndex' is provided write the index of the PID element with the 
    // minimum difference in the phi angle to that variable (-1 if no cluster PID fired).

    Double_t cb_phi = inCluster->GetMeanPosition()->Phi() * TMath::RadToDeg();

    Double_t pidEnergy = 0;
    Double_t minPhiDiff = 180;
    Int_t pidIndex = -1;

    // loop over all PID hits
    for (UInt_t i = 0; i < fPIDNhits; i++)
    {
        // calculate CB-PID phi difference
        Double_t phi_diff = fPIDHitPos[fPIDHits[i]]->Z() - cb_phi;
        
        // map difference to the interval [0, 180]
        if (phi_diff >  180) phi_diff =  360 - phi_diff;
        if (phi_diff < -180) phi_diff = -360 - phi_diff;
        phi_diff = TMath::Abs(phi_diff);

        // fill CB-PID coincidence histograms
        //fH_CB_PID_Coinc_Hits[fPIDHits[i]]->Fill(phi_diff);
        
        // check phi difference limit
        if (phi_diff <= fCBPIDPhiLimit)
        {
            // calculate theta-weighted energy
            Double_t energy = fPIDEnergy[fPIDHits[i]]
                              * TMath::Abs(TMath::Sin(inCluster->GetTheta() * TMath::DegToRad()));
            
            // update minimal phi difference
            if (phi_diff < minPhiDiff)
            {
                pidEnergy = energy;
                pidIndex = fPIDHits[i];
                minPhiDiff = phi_diff;
            }
        }
    }
   
    // fill CB-PID dE vs. E histograms
    //if (pidEnergy > 0.) fH_CB_PID_dE_E[pidIndex]->Fill(inCluster->GetEnergy(), pidEnergy);

    // set PID index
    if (outPIDIndex) *outPIDIndex = pidIndex;
    
    // return PID energy
    return pidEnergy;
}

//______________________________________________________________________________
void TA2MyPhysics::SetP4(TLorentzVector& p4, Double_t Ekin, Double_t mass, TVector3* dir)
{
    // Set the 4-vector 'p4' using the kinetic energy 'Ekin', the mass
    // 'mass' and the reconstructed direction 'dir'.

    Double_t E = Ekin + mass;
    p4.SetE(E);
    TVector3 p = dir->Unit() * TMath::Sqrt(E*E - mass*mass);
    p4.SetVect(p);
}

//______________________________________________________________________________
void TA2MyPhysics::SetP4(TLorentzVector& p4, Double_t Ekin, Double_t mass, Double_t x, Double_t y, Double_t z)
{
    // Set the 4-vector 'p4' using the kinetic energy 'Ekin', the mass
    // 'mass' and the reconstructed direction 'x,y,x'.

    Double_t E = Ekin + mass;
    TVector3 dir(x, y, z);
    p4.SetE(E);
    TVector3 p = dir.Unit() * TMath::Sqrt(E*E - mass*mass);
    p4.SetVect(p);
}

//______________________________________________________________________________
void TA2MyPhysics::FillPSA(TH2* h, TOA2DetParticle* p, Double_t w)
{
    // Fill the PSA information of the particle 'p' into the histogram 'h'
    // using the weight 'w'.
    
    Double_t psaR;
    Double_t psaA;
    
    // get PSA information
    if (GetPSA(p, &psaR, &psaA)) h->Fill(psaA, psaR, w);
}

//______________________________________________________________________________
void TA2MyPhysics::FilldE_E(TH2* hCB, TH2* hTAPS, TOA2DetParticle* p, Double_t w)
{
    // Fill the dE/E information of the particle 'p' into the histogram 'hCB'
    // or 'hTAPS' using the weight 'w'. 
    
    // check the detector
    if (p->GetDetector() == kTAPSDetector) hTAPS->Fill(p->GetEnergy(), p->GetVetoEnergy(), w);
    else if (p->GetDetector() == kCBDetector) hCB->Fill(p->GetEnergy(), p->GetPIDEnergy(), w);
}

//______________________________________________________________________________
void TA2MyPhysics::FillTOF(TH2* hCB, TH2* hTAPS, TOA2DetParticle* p, Double_t t, Bool_t isMC, Double_t w)
{
    // Fill the TOF information of the particle 'p' into the histogram 'hCB'
    // or 'hTAPS' using the weight 'w'. The tagger time 't' and 'isMC' is used 
    // to calculate the TOF.
    
    // check the detector
    if (p->GetDetector() == kTAPSDetector) hTAPS->Fill(p->CalculateTOFTagger(t, isMC), p->GetEnergy(), w);
    else if (p->GetDetector() == kCBDetector) hCB->Fill(p->CalculateTOFTagger(t, isMC), p->GetEnergy(), w);
}

//______________________________________________________________________________
Bool_t TA2MyPhysics::CheckPSA(TOA2DetParticle* p, TCutG* c)
{   
    // Check if the PSA information of the particle 'p' is inside the graphical
    // cut 'c'. Return kTRUE if so, otherwise return kFALSE.
    // If the particle 'p' was not detected in TAPS kFALSE is returned.
    
    Double_t psaR;
    Double_t psaA;
    
    // get PSA information
    if (GetPSA(p, &psaR, &psaA))
    {
        // check cut
        return c->IsInside(psaA, psaR);
    }
    else return kFALSE;
}

//______________________________________________________________________________
Int_t TA2MyPhysics::GetForeignVetoHits(TOA2DetParticle* p)
{
    // Return the number of Veto hits that are not in front of the central element
    // or its neighbours of the particle 'p'.

    // check if the particle was detected in TAPS
    if (p->GetDetector() != kTAPSDetector) 
    {
        Error("IsForeignVetoHit", "The particle was not detected in TAPS!");
        return fVetoNhits;
    }

    // get the central element
    Int_t center = p->GetCentralElement();

    // get the corresponding detector cluster element from the BaF2 class
    HitCluster_t* c = fBaF2PWO->GetCluster(center);
    
    // get the central element's neighbours
    UInt_t nNeighbours = c->GetNNeighbour();
    UInt_t* neighbours = c->GetNeighbour();
 
    // the number of foreign veto hits
    Int_t nForeign = 0;

    // loop over all veto hits
    for (UInt_t i = 0; i < fVetoNhits; i++)
    {   
        Bool_t isForeign = kTRUE;

        // check central element
        if (fVetoHits[i] == GetVetoInFrontOfElement(center)) continue;

        // loop over all neighbour elements
        for (UInt_t j = 0; j < nNeighbours; j++)
        {
            if (fVetoHits[i] == GetVetoInFrontOfElement((Int_t)neighbours[j]))
            {
                isForeign = kFALSE;
                break;
            }
        }

        // count foreign hit
        if (isForeign) nForeign++;
    }

    return nForeign;
}

//______________________________________________________________________________
Bool_t TA2MyPhysics::GetPSA(TOA2DetParticle* p, Double_t* psaR, Double_t* psaA)
{   
    // Calculate the PSA information of the particle 'p'.
    // Save the PSA radius to 'psaR' and the PSA angle to psA.
    // If the particle 'p' was detected in TAPS and the PSA information could
    // be calculated and saved kTRUE is returned, otherwise kFALSE.
    
    // check if particle was detected in TAPS
    if (p->GetDetector() == kTAPSDetector)
    {   
        // calculate the PSA information
        Double_t lgE  = p->GetCentralEnergy();
        Double_t sgE  = p->GetCentralSGEnergy();
        Double_t r = TMath::Sqrt(sgE*sgE + lgE*lgE); 
        Double_t a = TMath::ATan(sgE/lgE)*TMath::RadToDeg();
        
        // saved information
        if (psaR) *psaR = r;
        else return kFALSE;
        if (psaA) *psaA = a;
        else return kFALSE;

        // calculation and checks were successful here
        return kTRUE;
    }
    else return kFALSE;
}

