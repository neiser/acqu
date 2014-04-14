#include "TA2GoAT.h"

ClassImp(TA2GoAT)

TA2GoAT::TA2GoAT(const char* Name, TA2Analysis* Analysis) : TA2AccessSQL(Name, Analysis),
                                                                    file(0),
                                                                    treeRawEvent(0),
                                                                    treeTagger(0),
                                                                    treeTrigger(0),
                                                                    treeDetectorHits(0),
                                                                    treeScaler(0),
                                                                    nParticles(0),
                                                                    Ek(0),
																	Theta(0),
																	Phi(0),
                                                                    time(0),
                                                                    clusterSize(0),
                                                                    Apparatus(0),
                                                                    d_E(0),
                                                                    WC0_E(0),
                                                                    WC1_E(0),
                                                                    WC_Vertex_X(0),
                                                                    WC_Vertex_Y(0),
                                                                    WC_Vertex_Z(0),
                                                                    nTagged(0),
                                                                    photonbeam_E(0),
                                                                    tagged_ch(0),
                                                                    tagged_t(0),
                                                                    nNaI_Hits(0),
                                                                    NaI_Hits(0),
                                                                    nPID_Hits(0),
                                                                    PID_Hits(0),
                                                                    nWC_Hits(0),
																	WC_Hits(0),
																	nBaF2_PbWO4_Hits(0),
																	BaF2_PbWO4_Hits(0),
                                                                    nVeto_Hits(0),
                                                                    Veto_Hits(0),
                                                                    ESum(0),
                                                                    Mult(0),
                                                                    nHelBits(0),
																	TriggerPattern(0),
                                                                    nError(0),
                                                                    ErrModID(0),
                                                                    ErrModIndex(0),
                                                                    ErrCode(0),
                                                                    eventNumber(0),
                                                                    eventID(0)														
{
    	strcpy(outputFolder,"~");
    	strcpy(fileName,"Acqu");

    	AddCmdList(RootTreeConfigKeys);
}


TA2GoAT::~TA2GoAT()
{
	if(treeRawEvent)
		delete treeRawEvent;
	if(treeTagger)
		delete treeTagger;
	if(treeTrigger)
		delete treeTrigger;
	if(treeDetectorHits)
		delete treeDetectorHits;
	if(treeScaler)
		delete treeScaler;
    if(file)
		delete file;
}

void    TA2GoAT::LoadVariable()
{
	// Including histogram output for testing purposes (quick check of variables)
   	TA2AccessSQL::LoadVariable();

	TA2DataManager::LoadVariable("nParticles", 	&nParticles,EISingleX);
	TA2DataManager::LoadVariable("Ek", 			Ek,			EDMultiX);
	TA2DataManager::LoadVariable("Theta", 		Theta,		EDMultiX);   
	TA2DataManager::LoadVariable("Phi", 		Phi,		EDMultiX);     	 	
	TA2DataManager::LoadVariable("time", 		time,		EDMultiX);

	TA2DataManager::LoadVariable("nTagged", 	&nTagged,	EISingleX);
	TA2DataManager::LoadVariable("photonbeam_E",photonbeam_E,EDMultiX);
	TA2DataManager::LoadVariable("taggedCh", 	tagged_ch,	EIMultiX);
	TA2DataManager::LoadVariable("taggedT", 	tagged_t,	EDMultiX);    

	TA2DataManager::LoadVariable("dE", 			d_E,		EDMultiX);
	TA2DataManager::LoadVariable("WC0E", 		WC0_E,		EDMultiX);   
	TA2DataManager::LoadVariable("WC1E", 		WC1_E,		EDMultiX);

	TA2DataManager::LoadVariable("ESum",		&ESum, 		EDSingleX);
 
    return;
    
}

void    TA2GoAT::SetConfig(Char_t* line, Int_t key)
{
    	switch(key)
    	{
    	case EG_OUTPUT_FOLDER:
     	   strcpy(outputFolder,line);
     	   while(outputFolder[strlen(outputFolder)-1]=='\n' 
						|| outputFolder[strlen(outputFolder)-1]=='\r')
			outputFolder[strlen(outputFolder)-1]='\0';
        return;
    	case EG_FILE_NAME:
        	strcpy(fileName,line);
        	while(fileName[strlen(fileName)-1]=='\n' 
						|| fileName[strlen(fileName)-1]=='\r')
			fileName[strlen(fileName)-1]='\0';
        return;
    	case EG_BEAM_HELICITY:
    	    	nHelBits = sscanf(line, "%i%s%s%s%s%s%s%s%s", &HelADC, HelBits[0], HelBits[1], HelBits[2], HelBits[3], HelBits[4], HelBits[5], HelBits[6], HelBits[7]);
    	    	nHelBits--;
    	    	if(nHelBits < 2) Error("SetConfig", "Not enough information to construct beam helicity!");
    	    	else
    	    	{
			printf("Helicity");
			for(int i=0; i<nHelBits; i++)
			{
    				HelInh[i] = false;
    				if(!strcmp(HelBits[i],"I") || !strcmp(HelBits[i],"i")) HelInh[i] = true;
    				else if(!strcmp(HelBits[i],"L") || !strcmp(HelBits[i],"l")) HelBeam[i] = false;
    				else if(!strcmp(HelBits[i],"H") || !strcmp(HelBits[i],"h")) HelBeam[i] = true;
				printf(" - %s %i %i",HelBits[i],HelInh[i],HelBeam[i]);
			}
			printf("\n");
    	    	}
	return;
    	default:
        	TA2AccessSQL::SetConfig(line, key);
    	}
}

void    TA2GoAT::PostInit()
{

   	Ek		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	Theta		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	Phi		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	time		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	clusterSize = new UChar_t[TA2GoAT_MAX_PARTICLE];
    
   	photonbeam_E= new Double_t[TA2GoAT_MAX_TAGGER];
   	tagged_ch	= new Int_t[TA2GoAT_MAX_TAGGER];
   	tagged_t	= new Double_t[TA2GoAT_MAX_TAGGER];
    
   	Apparatus	= new UChar_t[TA2GoAT_MAX_PARTICLE];
   	d_E			= new Double_t[TA2GoAT_MAX_PARTICLE];
   	WC0_E		= new Double_t[TA2GoAT_MAX_PARTICLE];
   	WC1_E		= new Double_t[TA2GoAT_MAX_PARTICLE];
    
   	WC_Vertex_X 	= new Double_t[TA2GoAT_MAX_PARTICLE];
   	WC_Vertex_Y 	= new Double_t[TA2GoAT_MAX_PARTICLE];
   	WC_Vertex_Z 	= new Double_t[TA2GoAT_MAX_PARTICLE];
    
   	NaI_Hits	= new Int_t[TA2GoAT_MAX_HITS];  
   	PID_Hits	= new Int_t[TA2GoAT_MAX_HITS];
   	WC_Hits		= new Int_t[TA2GoAT_MAX_HITS];
   	BaF2_PbWO4_Hits	= new Int_t[TA2GoAT_MAX_HITS];
   	Veto_Hits	= new Int_t[TA2GoAT_MAX_HITS];
    
   	TriggerPattern = new Int_t[32];

    ErrModID 	= new Int_t[TA2GoAT_MAX_ERROR];
   	ErrModIndex 	= new Int_t[TA2GoAT_MAX_ERROR];
   	ErrCode 	= new Int_t[TA2GoAT_MAX_ERROR];

   	printf("---------\n");
   	printf("Init Tree\n");
   	printf("---------\n");
    
   	// Append input filename to output tree name.
   	TString fullName;
   	if(gAR->GetProcessType() == EMCProcess) fullName = gAR->GetTreeFileList(0);        
   	else  fullName = gAR->GetFileName();
		
	int length = fullName.Length();
	int last = 0;
	for (int i = 0; i < length; i++)
	{
		int index = fullName.Index("/"); 
		fullName.Remove(0,index+1);	
		if (index == -1) break;
		last += index+1; 
	}
	Char_t inFile[256], str[256];
    	if(gAR->GetProcessType() == EMCProcess) 
		sscanf( gAR->GetTreeFileList(0)+last, "%[^.].root\n", inFile);       
    	else    
		sscanf( gAR->GetFileName()+last, "%[^.].dat\n", inFile);	

    	sprintf(str, "%s/%s_%s.root", outputFolder, fileName, inFile);        
   	printf("Root file saved to %s\n", str);  

   	file		= new TFile(str,"RECREATE");
	treeRawEvent	= new TTree("treeRawEvent", "treeRawEvent");
	treeTagger	= new TTree("treeTagger","treeTagger");
	treeTrigger	= new TTree("treeTrigger","treeTrigger");
	treeDetectorHits = new TTree("treeDetectorHits", "treeDetectorHits");
	
	treeRawEvent->Branch("nParticles",&nParticles,"nParticles/I");
	treeRawEvent->Branch("Ek",  Ek,  "Ek[nParticles]/D");	
	treeRawEvent->Branch("Theta",  Theta,  "Theta[nParticles]/D");	
	treeRawEvent->Branch("Phi",  Phi,  "Phi[nParticles]/D");	
	treeRawEvent->Branch("time", time, "time[nParticles]/D");
	treeRawEvent->Branch("clusterSize", clusterSize, "clusterSize[nParticles]/b");
	treeRawEvent->Branch("Apparatus", Apparatus, "Apparatus[nParticles]/b");
	treeRawEvent->Branch("d_E", d_E, "d_E[nParticles]/D");	
	treeRawEvent->Branch("WC0_E", WC0_E, "WC0_E[nParticles]/D");	
	treeRawEvent->Branch("WC1_E", WC1_E, "WC1_E[nParticles]/D");
	treeRawEvent->Branch("WC_Vertex_X", WC_Vertex_X, "WC_Vertex_X[nParticles]/D");	
	treeRawEvent->Branch("WC_Vertex_Y", WC_Vertex_Y, "WC_Vertex_Y[nParticles]/D");	
	treeRawEvent->Branch("WC_Vertex_Z", WC_Vertex_Z, "WC_Vertex_Z[nParticles]/D");
	
	treeTagger->Branch("nTagged", &nTagged,"nTagged/I");
	treeTagger->Branch("photonbeam_E", photonbeam_E, "photonbeam_E[nTagged]/D");
	treeTagger->Branch("tagged_ch", tagged_ch, "tagged_ch[nTagged]/I");
	treeTagger->Branch("tagged_t", tagged_t, "tagged_t[nTagged]/D");

	treeTrigger->Branch("ESum", &ESum, "ESum/D");
	treeTrigger->Branch("Mult", &Mult, "Mult/I");
	if(nHelBits > 1) treeTrigger->Branch("Helicity", &Helicity, "Helicity/O");
	treeTrigger->Branch("nError", &nError, "nError/I");
	treeTrigger->Branch("ErrModID", ErrModID, "ErrModID[nError]/I");
	treeTrigger->Branch("ErrModIndex", ErrModIndex, "ErrModIndex[nError]/I");
	treeTrigger->Branch("ErrCode", ErrCode, "ErrCode[nError]/I");
	treeTrigger->Branch("nTriggerPattern", &nTriggerPattern, "nTriggerPattern/I");
	treeTrigger->Branch("TriggerPattern", TriggerPattern, "TriggerPattern[nTriggerPattern]/I");
	
	treeDetectorHits->Branch("nNaI_Hits", &nNaI_Hits, "nNaI_Hits/I");
	treeDetectorHits->Branch("NaI_Hits", NaI_Hits, "NaI_Hits[nNaI_Hits]/I");
	treeDetectorHits->Branch("nPID_Hits", &nPID_Hits, "nPID_Hits/I");
	treeDetectorHits->Branch("PID_Hits", PID_Hits, "PID_Hits[nPID_Hits]/I");
	treeDetectorHits->Branch("nWC_Hits", &nWC_Hits, "nWC_Hits/I");
	treeDetectorHits->Branch("WC_Hits", WC_Hits, "WC_Hits[nWC_Hits]/I");	
	treeDetectorHits->Branch("nBaF2_PbWO4_Hits", &nBaF2_PbWO4_Hits, "nBaF2_PbWO4_Hits/I");
	treeDetectorHits->Branch("BaF2_PbWO4_Hits", BaF2_PbWO4_Hits, "BaF2_PbWO4_Hits[nBaF2_PbWO4_Hits]/I");
	treeDetectorHits->Branch("nVeto_Hits", &nVeto_Hits, "nVeto_Hits/I");
	treeDetectorHits->Branch("Veto_Hits", Veto_Hits, "Veto_Hits[nVeto_Hits]/I");

	// Store Scalers for non-MC process
	if (gAR->GetProcessType() != EMCProcess) 
	{
		treeScaler = new TTree("treeScaler", "treeScaler");	
		treeScaler->Branch("eventNumber", &eventNumber, "eventNumber/I");
		treeScaler->Branch("eventID", &eventID, "eventID/I");
		printf("GetMaxScaler: %d\n", GetMaxScaler());
		sprintf(str, "Scaler[%d]/i", GetMaxScaler());
		treeScaler->Branch("Scaler", fScaler, str);
	}

	// Define Histograms which will be saved to root tree
	DefineHistograms();

	gROOT->cd();
	
	eventNumber	= 0;
	
	// Default SQL-physics initialisation
    	TA2AccessSQL::PostInit();	

}

void    TA2GoAT::Reconstruct()
{
	// Fill standard data check histograms
	DataCheckHistograms();

	// Output scaler info on scaler read events
	if((gAR->IsScalerRead()) && (gAR->GetProcessType() != EMCProcess))
	{
		eventID	= gAN->GetNDAQEvent();
		treeScaler->Fill();		
	}

	if(fTagger && fLadder)
	{
        // Get the conversion of tagger channel to photon energy
	Double_t electron_E = fTagger->GetBeamEnergy();
        const Double_t* ChToE = fLadder->GetECalibration();		
		
		// Collect Tagger M0 Hits
		nTagged	= fLadder->GetNhits();
		for(int i=0; i<nTagged; i++)
		{
			tagged_ch[i]	= fLadder->GetHits(i);
			tagged_t[i]		= (fLadder->GetTimeOR())[i];
			photonbeam_E[i] = electron_E - ChToE[tagged_ch[i]];
		}
	
		// Collect Tagger M+ Hits
		for(int m=1; m<fLadder->GetNMultihit(); m++)
		{
			for(int i=0; i<fLadder->GetNhitsM(m); i++)
			{
				tagged_ch[nTagged+i] 	= (fLadder->GetHitsM(m))[i];
				tagged_t[nTagged+i]	 	= (fLadder->GetTimeORM(m))[i];
				photonbeam_E[nTagged+i] = electron_E - ChToE[tagged_ch[nTagged+i]];
			}
			nTagged	+= fLadder->GetNhitsM(m);
		}
	}
	else nTagged = 0;
	
	// Gather particle information
	nParticles = 0;
	if(fCB)
	{
	// Collect CB Hits
    	nParticles	= fCB->GetNParticle();      
		for(int i=0; i<nParticles; i++)
		{
			TA2Particle part = fCB->GetParticles(i);
			
			part.SetParticleID(kRootino); // Set mass to 0 (rootino)
			part.SetMass(0.0);

			// Reset a bunch of inconsistant "no-value" markers
			if(TMath::Abs(part.GetT()) >= TA2GoAT_NULL) Ek[i] = 0.0;
			else Ek[i] = part.GetT();

			if(TMath::Abs(part.GetTime()) >= TA2GoAT_NULL) time[i] = 0.0;
			else time[i] = part.GetTime();
			
			if(TMath::Abs(part.GetVetoEnergy()) >= TA2GoAT_NULL) d_E[i] = 0.0;
			else d_E[i]	= part.GetVetoEnergy();
			
			if(TMath::Abs(part.GetEnergyMwpc0()) >= TA2GoAT_NULL) WC0_E[i] = 0.0;
			else WC0_E[i] = part.GetEnergyMwpc0();
			
			if(TMath::Abs(part.GetEnergyMwpc1()) >= TA2GoAT_NULL) WC1_E[i] = 0.0;
			else WC1_E[i] = part.GetEnergyMwpc1();
			
			if(TMath::Abs(part.GetPsVertex().X()) >= TA2GoAT_NULL) WC_Vertex_X[i] = 0.0;
			else WC_Vertex_X[i]  = part.GetPsVertex().X();			

			if(TMath::Abs(part.GetPsVertex().Y()) >= TA2GoAT_NULL) WC_Vertex_Y[i] = 0.0;
			else WC_Vertex_Y[i]  = part.GetPsVertex().Y();	
			
			if(TMath::Abs(part.GetPsVertex().Z()) >= TA2GoAT_NULL) WC_Vertex_Z[i] = 0.0;
			else WC_Vertex_Z[i]  = part.GetPsVertex().Z();				
			
			// Store other values which don't have this "no-value" option
			Apparatus[i]	= (UChar_t)EAppCB;
			Theta[i]		= part.GetThetaDg();
			Phi[i]			= part.GetPhiDg();			
			clusterSize[i]  = (UChar_t)part.GetClusterSize();

		}
	}

	if(fTAPS)
	{
		// Collect TAPS Hits
		for(int i=0; i<fTAPS->GetNParticle(); i++)
		{
			TA2Particle part = fTAPS->GetParticles(i);
			
			part.SetParticleID(kRootino); // Set mass to 0 (rootino)
			part.SetMass(0.0);				

			// Reset a bunch of inconsistant "no-value" markers
			if(TMath::Abs(part.GetT()) >= TA2GoAT_NULL) Ek[nParticles+i] = 0.0;
			else Ek[nParticles+i] = part.GetT();

			if(TMath::Abs(part.GetTime()) >= TA2GoAT_NULL) time[nParticles+i] = 0.0;
			else time[nParticles+i] = part.GetTime();
			
			if(TMath::Abs(part.GetVetoEnergy()) >= TA2GoAT_NULL) d_E[nParticles+i] = 0.0;
			else d_E[nParticles+i]	= part.GetVetoEnergy();
			
			// Set WC values to NULL
			WC0_E[nParticles+i] = 0.0;
			WC1_E[nParticles+i] = 0.0;
			WC_Vertex_X[nParticles+i] = 0.0;
			WC_Vertex_Y[nParticles+i] = 0.0;
			WC_Vertex_Z[nParticles+i] = 0.0;
			
			// Store other values which don't have this "no-value" option
			Apparatus[nParticles+i]		= (UChar_t)EAppTAPS;
			Theta[nParticles+i]			= part.GetThetaDg();
			Phi[nParticles+i]			= part.GetPhiDg();			
			time[nParticles+i]			= part.GetTime();	
			clusterSize[nParticles+i]  	= (UChar_t)part.GetClusterSize();

		}
		nParticles += fTAPS->GetNParticle(); // update number of particles
	}

	// Get Detector Hits
	if(fNaI)
	{
		nNaI_Hits = fNaI->GetNhits();
		for(int i=0; i<nNaI_Hits; i++)   
			{ NaI_Hits[i] = fNaI->GetHits(i); }
	}

	if(fPID)
	{
		nPID_Hits = fPID->GetNhits();
		for(int i=0; i<nPID_Hits; i++)   
			{ PID_Hits[i] = fPID->GetHits(i); }
	}

	if(fMWPC)
	{
		nWC_Hits = fMWPC->GetNhits();
		for(int i=0; i<nWC_Hits; i++)    
			{ WC_Hits[i] = fMWPC->GetHits(i); }
	}

	if(fBaF2PWO)
	{
		nBaF2_PbWO4_Hits = fBaF2PWO->GetNhits();
		for(int i=0; i<nBaF2_PbWO4_Hits; i++)
			{ BaF2_PbWO4_Hits[i] = fBaF2PWO->GetHits(i); }
	}

	if(fVeto)
	{
		nVeto_Hits = fVeto->GetNhits();
		for(int i=0; i<nVeto_Hits; i++) 
			{ Veto_Hits[i] = fVeto->GetHits(i);}
	}
	
	// Get Trigger information
	TriggerReconstruction();

	nError = gAR->GetHardError();
	ReadErrorMk2_t *ErrorBlock = gAR->GetHardwareError();
	ReadErrorMk2_t *Error;
	for(int i=0; i<nError; i++)
	{
		Error = ErrorBlock + i;
		ErrModID[i] = Error->fModID;
		ErrModIndex[i] = Error->fModIndex;
		ErrCode[i] = Error->fErrCode;
	}

	if(nHelBits > 1)
	{
		Bool_t HelBit;
		Helicity = true;
		HelInver = true;
		for(int i=0; i<nHelBits; i++)
		{
			HelBit = (fADC[HelADC] & 1<<i);
			if(HelInh[i] && HelBit)
			{
				ErrCode[nError] = 9;
				nError++;
				break;
			}
			else if(HelInh[i]) continue;
			Helicity = (Helicity && (HelBeam[i] == HelBit));
			HelInver = (HelInver && (HelBeam[i] != HelBit));
			if(Helicity == HelInver)
			{
				ErrCode[nError] = 10;
				nError++;
				break;
			}
		}
	} 

	//Apply EndBuffer
    	Ek[nParticles] 		= EBufferEnd;
    	Theta[nParticles]	= EBufferEnd;
    	Phi[nParticles]		= EBufferEnd;
    	time[nParticles] 	= EBufferEnd;
    	WC0_E[nParticles] 	= EBufferEnd;
    	WC1_E[nParticles] 	= EBufferEnd;
    	WC_Vertex_X[nParticles] = EBufferEnd;  
    	WC_Vertex_Y[nParticles] = EBufferEnd;    
	WC_Vertex_Z[nParticles] = EBufferEnd;    
	d_E[nParticles] 	= EBufferEnd;    
    tagged_ch[nTagged] 	= EBufferEnd;
    tagged_t[nTagged] 	= EBufferEnd;	
	
	//Fill Trees
	treeRawEvent->Fill();
	treeTagger->Fill();
	treeTrigger->Fill();
	treeDetectorHits->Fill();

	//increment event number
	eventNumber++;	
}

void	TA2GoAT::DefineHistograms()
{
	// Define new data check histograms
	Check_CBdE_E		= new TH2F("Check_CBdE_E", "dE_E (all CB clusters compared to PID hits)", 	400, 0, 400, 100, 0, 10);
	Check_CBPhiCorr 	= new TH2F("Check_CBPhiCorr","PID-NaI phi correlation (all CB clusters compared to PID hits)", 24,  0,  24, 180, -180, 180);

	Check_CBdE_E_1PID 	 = new TH2F("Check_CBdE_E_1PID", "dE_E (all CB clusters compared to single PID hits)", 	400, 0, 400, 100, 0, 10);
	Check_CBPhiCorr_1PID = new TH2F("Check_CBPhiCorr_1PID","PID-NaI phi correlation (all CB clusters compared to single PID hits)", 24,  0,  24, 180, -180, 180);

	Check_CBdE_E_pi0 	= new TH2F("Check_CBdE_E_pi0", 		"dE_E after pi0 identification", 	400, 0, 400, 100, 0, 10);
	Check_CBPhiCorr_pi0 = new TH2F("Check_CBPhiCorr_pi0",	"PID-NaI phi correlation after pi0 identification", 24,  0,  24, 180, -180, 180);

	Check_TAPSdE_E		= new TH2F("Check_TAPSdE_E", "dE_E (all TAPS clusters compared to Veto hits)", 	400, 0, 400, 100, 0, 10);
	Check_TAPSPhiCorr 	= new TH2F("Check_TAPSPhiCorr","TAPS-Veto phi correlation (all TAPS clusters compared to Veto hits)", 438,  0,  438, 180, -180, 180);

	Check_TAPSdE_E_1Veto	= new TH2F("Check_TAPSdE_E_1PID", "dE_E (all TAPS clusters compared to single Veto hits)", 	400, 0, 400, 100, 0, 10);
	Check_TAPSPhiCorr_1Veto = new TH2F("Check_TAPSPhiCorr_1Veto","TAPS-Veto phi correlation (all TAPS clusters compared to single Veto hits)", 438,  0,  438, 180, -180, 180);

	Check_CBHits 		= new TH2F("Check_CBHits", 		"CB Hits by event number", 		10000,  0,  10000000, 720, 0, 720);	
	Check_CBADCHits 	= new TH2F("Check_CBADCHits", 	"CB ADC Hits by event number", 	10000,  0,  10000000, 720, 0, 720);
	Check_CBTDCHits 	= new TH2F("Check_CBTDCHits",	"CB TDC Hits by event number", 	10000,  0,  10000000, 720, 0, 720);

	Check_PIDHits 		= new TH2F("Check_PIDHits", 	"PID Hits by event number", 	10000,  0,  10000000, 24, 0, 24);	
	Check_PIDADCHits 	= new TH2F("Check_PIDADCHits", 	"PID ADC Hits by event number", 10000,  0,  10000000, 24, 0, 24);
	Check_PIDTDCHits 	= new TH2F("Check_PIDTDCHits",	"PID TDC Hits by event number", 10000,  0,  10000000, 24, 0, 24);

	Check_TAPSHits 		= new TH2F("Check_TAPSHits", 	"TAPS Hits by event number", 	10000,  0,  10000000, 438, 0, 438);	
	Check_TAPSADCHits 	= new TH2F("Check_TAPSADCHits", "TAPS ADC Hits by event number",10000,  0,  10000000, 438, 0, 438);
	Check_TAPSTDCHits 	= new TH2F("Check_TAPSTDCHits", "TAPS TDC Hits by event number",10000,  0,  10000000, 438, 0, 438);

	Check_VetoHits 		= new TH2F("Check_VetoHits", 	"Veto Hits by event number", 	10000,  0,  10000000, 438, 0, 438);	
	Check_VetoADCHits 	= new TH2F("Check_VetoADCHits", "Veto ADC Hits by event number",10000,  0,  10000000, 438, 0, 438);
	Check_VetoTDCHits 	= new TH2F("Check_VetoTDCHits", "Veto TDC Hits by event number",10000,  0,  10000000, 438, 0, 438);
	
}

void	TA2GoAT::WriteHistograms()
{
	// Write check histograms
	file->mkdir("CheckCB");
	file->cd("CheckCB");
	Check_CBdE_E->Write();
	Check_CBPhiCorr->Write();	
	
	Check_CBdE_E_1PID->Write();
	Check_CBPhiCorr_1PID->Write();

	Check_CBdE_E_pi0->Write();
	Check_CBPhiCorr_pi0->Write();

	file->cd();
	file->mkdir("CheckTAPS");
	file->cd("CheckTAPS");
	Check_TAPSdE_E->Write();
	Check_TAPSPhiCorr->Write();	
	
	Check_TAPSdE_E_1Veto->Write();
	Check_TAPSPhiCorr_1Veto->Write();
	
	file->cd();
	file->mkdir("CheckHitPatterns");
	file->cd("CheckHitPatterns");	
	// Zoom in on all the plots with eventNumber on x-Axis
	Check_CBHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_CBHits->Write();
	
	Check_CBADCHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_CBADCHits->Write();
	
	Check_CBTDCHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_CBTDCHits->Write();

	Check_PIDHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_PIDHits->Write();
	
	Check_PIDADCHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_PIDADCHits->Write();
	
	Check_PIDTDCHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_PIDTDCHits->Write();

	Check_TAPSHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_TAPSHits->Write();
	
	Check_TAPSADCHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_TAPSADCHits->Write();
	
	Check_TAPSTDCHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_TAPSTDCHits->Write();	
	
	Check_VetoHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_VetoHits->Write();
	
	Check_VetoADCHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_VetoADCHits->Write();
	
	Check_VetoTDCHits->GetXaxis()->SetRangeUser(0,eventNumber);
	Check_VetoTDCHits->Write();		

	file->cd();
}

void 	TA2GoAT::DataCheckHistograms()
{
	
	if((fCB) && (fPID))
	{
		// Find the charged particle (if possible) and fill dE_E for this
		// Fill the charged particle phi against the PID hit
		for(int i=0; i<fCB->GetNParticle(); i++)
		{
			TA2Particle part = fCB->GetParticles(i);

			if (part.GetVetoEnergy() < 0)  continue;
			if (part.GetVetoEnergy() > 10) continue;			

			for(int j=0; j<fPID->GetNhits(); j++)
			{
				Check_CBdE_E->Fill(part.GetT(),part.GetVetoEnergy());
				Check_CBPhiCorr->Fill(fPID->GetHits(j),part.GetPhiDg());
			}

			if(fPID->GetNhits() == 1)
			{	
				Check_CBdE_E_1PID->Fill(part.GetT(),part.GetVetoEnergy());
				Check_CBPhiCorr_1PID->Fill(fPID->GetHits(0),part.GetPhiDg());	
			}
		}		
		
		// Do basic pi0 analysis on events with 3 NaI cluster and 1 PID hit
		// Fill dE_E for charged particle if pi0 reconstructed
		// Fill associated NaI cluster phi against single PID element		
		if( (fCB->GetNParticle() == 3) && (fPID->GetNhits() == 1) )	
		{
			// Get 3 particles, check for a pi0 from possible pairs
			TA2Particle part1 = fCB->GetParticles(0);
			TA2Particle part2 = fCB->GetParticles(1);
			TA2Particle part3 = fCB->GetParticles(2);
			
			TLorentzVector p4_12 = part1.GetP4() + part2.GetP4();
			TLorentzVector p4_13 = part1.GetP4() + part3.GetP4();
			TLorentzVector p4_23 = part2.GetP4() + part3.GetP4();
			
			// Calculated IM width/20 MeV (acts as a rough IM cut)
			Double_t IM_12 = TMath::Abs(135 - p4_12.M())/20;
			Double_t IM_13 = TMath::Abs(135 - p4_13.M())/20;
			Double_t IM_23 = TMath::Abs(135 - p4_23.M())/20;
			
			if((IM_12 <= 1.0) && (part3.GetVetoEnergy() > 0) && (part3.GetVetoEnergy() < 10))
			{
				Check_CBdE_E_pi0->Fill(part3.GetT(),part3.GetVetoEnergy());
				Check_CBPhiCorr_pi0->Fill(fPID->GetHits(0),part3.GetPhiDg());				
			}
			if((IM_13 <= 1.0) && (part2.GetVetoEnergy() > 0) && (part2.GetVetoEnergy() < 10))
			{
				Check_CBdE_E_pi0->Fill(part2.GetT(),part2.GetVetoEnergy());
				Check_CBPhiCorr_pi0->Fill(fPID->GetHits(0),part2.GetPhiDg());				
			}			
			if((IM_23 <= 1.0) && (part1.GetVetoEnergy() > 0) && (part1.GetVetoEnergy() < 10))
			{
				Check_CBdE_E_pi0->Fill(part1.GetT(),part1.GetVetoEnergy());
				Check_CBPhiCorr_pi0->Fill(fPID->GetHits(0),part1.GetPhiDg());				
			}			
			
		}
	}

	if((fTAPS) && (fVeto))
	{
		// Find the charged particle (if possible) and fill dE_E for this
		// Fill the charged particle phi against the Veto hit
		for(int i=0; i<fTAPS->GetNParticle(); i++)
		{
			TA2Particle part = fTAPS->GetParticles(i);

			if (part.GetVetoEnergy() < 0)  continue;
			if (part.GetVetoEnergy() > 10) continue;			

			for(int j=0; j<fVeto->GetNhits(); j++)
			{
				Check_TAPSdE_E->Fill(part.GetT(),part.GetVetoEnergy());
				Check_TAPSPhiCorr->Fill(fVeto->GetHits(j),part.GetPhiDg());
			}

			if(fVeto->GetNhits() == 1)
			{	
				Check_TAPSdE_E_1Veto->Fill(part.GetT(),part.GetVetoEnergy());
				Check_TAPSPhiCorr_1Veto->Fill(fVeto->GetHits(0),part.GetPhiDg());	
			}
		}
	}
	
	// Fill CB stability histograms
	if(fNaI)
	{
		for(int i=0; i<fNaI->GetNhits(); i++)   
				{ Check_CBHits->Fill(eventNumber,fNaI->GetHits(i)); }
			
		if (fNaI->IsRawHits())
		{
			for (int i=0; i< fNaI->GetNADChits(); i++)
				{ Check_CBADCHits->Fill(eventNumber,fNaI->GetRawEnergyHits()[i]); }	
				
			for (int i=0; i< fNaI->GetNTDChits(); i++)
				{ Check_CBTDCHits->Fill(eventNumber,fNaI->GetRawTimeHits()[i]);	}	
							
		}
	}
	
	// Fill PID stability histograms
	if(fPID)
	{
		for(int i=0; i<fPID->GetNhits(); i++)   
				{ Check_PIDHits->Fill(eventNumber,fPID->GetHits(i)); }
			
		if (fPID->IsRawHits())
		{
			for (int i=0; i< fPID->GetNADChits(); i++)
				{ Check_PIDADCHits->Fill(eventNumber,fPID->GetRawEnergyHits()[i]); }	
				
			for (int i=0; i< fPID->GetNTDChits(); i++)
				{ Check_PIDTDCHits->Fill(eventNumber,fPID->GetRawTimeHits()[i]);	}	
							
		}
	}	
	
	// Fill TAPS stability histograms
	if(fBaF2PWO)
	{
		for(int i=0; i<fBaF2PWO->GetNhits(); i++)   
				{ Check_TAPSHits->Fill(eventNumber,fBaF2PWO->GetHits(i)); }
			
		if (fBaF2PWO->IsRawHits())
		{
			if (!(fADC[0] & 1<<15)) // Ignore TAPS Pulser reads
			{			
				for (int i=0; i< fBaF2PWO->GetNADChits(); i++)
					{ Check_TAPSADCHits->Fill(eventNumber,fBaF2PWO->GetRawEnergyHits()[i]); }	
					
				for (int i=0; i< fBaF2PWO->GetNTDChits(); i++)
					{ Check_TAPSTDCHits->Fill(eventNumber,fBaF2PWO->GetRawTimeHits()[i]);	}	
			}
		}
	}	
	
	// Fill PID stability histograms
	if(fVeto)
	{
		for(int i=0; i<fVeto->GetNhits(); i++)   
				{ Check_VetoHits->Fill(eventNumber,fVeto->GetHits(i)); }
			
		if (fVeto->IsRawHits())
		{
			if (!(fADC[0] & 1<<15)) // Ingore TAPS Pulser reads
			{
				for (int i=0; i< fVeto->GetNADChits(); i++)
					{ Check_VetoADCHits->Fill(eventNumber,fVeto->GetRawEnergyHits()[i]); }	

				for (int i=0; i< fVeto->GetNTDChits(); i++)
					{ Check_VetoTDCHits->Fill(eventNumber,fVeto->GetRawTimeHits()[i]);	}	
			}			
		}
	}
			
}

void    TA2GoAT::Finish()
{
	printf("------------------\n");
	printf("Write Tree to file\n");
	printf("------------------\n");
	
	file->cd();
	
	if(treeRawEvent) 
	{
		treeRawEvent->Write();	// Write	
		delete treeRawEvent; 	// Close and delete in memory
	}
	if(treeTagger) 
	{
		treeTagger->Write();	// Write	
		delete treeTagger; 	// Close and delete in memory
	}	
	if(treeTrigger) 
	{
		treeTrigger->Write();	// Write	
		delete treeTrigger; 	// Close and delete in memory
	}		
	if(treeDetectorHits) 
	{
		treeDetectorHits->Write();// Write	
		delete treeDetectorHits;  // Close and delete in memory
	}		
	if(treeScaler)
	{
		treeScaler->Write();	// Write	
		delete treeScaler; 	// Close and delete in memory
    	}

	WriteHistograms();
	
    	if(file) 
		delete file;		// Close and delete in memory

	
	TA2AccessSQL::Finish();
}

void    TA2GoAT::ParseMisc(char* line)
{
	TA2AccessSQL::ParseMisc(line);
}

void 	TA2GoAT::TriggerReconstruction()
{
	if (fNaI) ESum = fNaI->GetTotalEnergy();
	if(gAR->GetProcessType() == EMCProcess) TriggerMC();
	else TriggerHW();

	
}

void 	TA2GoAT::TriggerMC() 
{
	// really rough, just the basic idea 
	// A good example is done in TA2BasePhysics but requires some extra work
	// Also need some flag for new and old 
	// Set some basic discriminator thresh for now
	Double_t DiscTh = 5.0; 
	Mult = 0;
		
	if(fNaI) 
	{
		for (Int_t i = 0; i < 45; i++) 
		{ 
			Bool_t flag = kFALSE;
			for (Int_t j = 0; j < 16; j++) 
			{
				if ((fNaI->GetEnergyAll(i*16 + j)) >= DiscTh) flag = kTRUE;
			}
			if (flag == kTRUE) Mult++;
		}
	}
		
	if (fBaF2PWO) {
		for (Int_t i = 0; i < 6; i++) { 
			Bool_t flag = kFALSE;
			for (Int_t j = 12; j < 71; j++) {  
			// really add some check of how many crystals are used, skip PbWO4s
				if ((fBaF2PWO->GetEnergyAll(i*71 + j)) >= DiscTh) flag = kTRUE;
			}
			if (flag == kTRUE) Mult++; 		
		}
	}
		
	// True Hardware multiplicities store only M2, M3, M4+
	//  Reduce MC multiplicity to reflect this limitation
	if (Mult == 1) Mult = 0;	
	if (Mult > 4) Mult = 4;
}
	
void 	TA2GoAT::TriggerHW() 
{
	nTriggerPattern = 0;
	for (int i= 0; i < 16; i++)
	{
		if (fADC[0] & 1<<i) 
		{ 
			TriggerPattern[nTriggerPattern] = i; 
			nTriggerPattern++;
		}
		if (fADC[1] & 1<<i) 
		{ 
			TriggerPattern[nTriggerPattern] = i+16;
			nTriggerPattern++;
		}
	}
	
	Mult = 0;
	
	if (fADC[0] & 1<<11) Mult+=2;
	if (fADC[0] & 1<<12) Mult++;
	if (fADC[0] & 1<<13) Mult++;
 	
}
