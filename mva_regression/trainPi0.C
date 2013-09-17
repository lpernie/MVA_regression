//================================================================================================
//			trainElectronEnergyRegression.C
//
// Given an appropriate electron ntuple, uses the classes GBRTrees etc. in order to train a regression
// to recover the generated electron energy
//
// USAGE
// trainElectronEnergyRegression(char* trainingFile, char* outWeightFile, char* optonChar)
//
// trainingFile		= ntuple file on which to perform the training
// outWeightFile	= output file, to which it will save the weights in a .root file
// optionChar		= denotes which version of the training is being performed
//________________________________________________________________________________________________

#include "TFile.h"
#include "TTree.h"
#include "GBRTrainer.h"
#include "GBRForest.h"
#include "GBRApply.h"
#include "TCut.h"
#include "TH2D.h"
#include "TSystem.h"
#include "TChain.h"
#include "TObjArray.h"
#include "TTreeFormula.h"
#include "TCanvas.h"
#include "Cintex/Cintex.h"

int GetTotalEvents(TChain *chain) {

    int numevents = 0;

    TObjArray *files = chain->GetListOfFiles();
    for (int i=0; i<files->GetEntries(); ++i) {    
	  TFile *file = TFile::Open(files->At(i)->GetTitle(),"READ");

	  TDirectory *fwkdir = (TDirectory*)file->FindObjectAny("AnaFwkMod");
	  TH1D *hevents = (TH1D*)fwkdir->Get("hDAllEvents");

	  numevents += hevents->GetEntries();

	  file->Close();

    }

    return numevents;

}

void trainPi0(char* trainingFile, char* outWeightFile,  int nTrees, int nPi0, string type, bool isPi0 ) {

    GBRTrainer *train = new GBRTrainer;

    TTree *intree = 0;

    cout << "Training on file " << trainingFile << endl;
    TChain *chainele;

    if(type=="EB")      chainele = new TChain("h_mva");
    else if(type=="EE") chainele = new TChain("h_EEmva");
    else cout<<"Sorry... Wrong type! Put EB or EE in the options"<<endl;
    chainele->Add(trainingFile);
    chainele->LoadTree(0);    
    chainele->SetCacheSize(64*1024*1024);
    chainele->SetCacheLearnEntries();
    intree = chainele;

    train->AddTree(chainele);

    TCut traincut = "";

    if(type=="EB"){
	  if(nPi0==1 || nPi0==2){
		if(isPi0) traincut = "(MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && (MVA_E3x3MC_2/MVA_E3x3_2)<3.0 && MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && MVA_E3x3_2>0.01 && MVA_E3x3MC_2>0.01 && MVA_Pt_1>1.0 && MVA_Pt_2>1.0 && MVA_S4S9_1>0.75 && MVA_S4S9_2>0.7";
		else      traincut = "(MVA_E3x3MC_1/MVA_E3x3_1)<2.0 && (MVA_E3x3MC_2/MVA_E3x3_2)<2.0 && MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && MVA_E3x3_2>0.01 && MVA_E3x3MC_2>0.01 && MVA_Pt_1>1.2 && MVA_Pt_2>1.2 && MVA_S4S9_1>0.85 && MVA_S4S9_2>0.85 && MVA_Nxtal_1>4 && MVA_Nxtal_2>3";
		if(nPi0==1) cout<<"train on 1 gamma: cut "<<string(traincut)<<endl;
		if(nPi0==2) cout<<"train on 2 gamma: cut "<<string(traincut)<<endl;

	  }
	  else if(nPi0==0){
		traincut = "MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && (MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && MVA_Pt_1>0.55 && MVA_Nxtal_1>5 && MVA_S4S9_1>0.75";
		cout<<"train on single gamma: cut "<<string(traincut)<<endl;
	  }
	  else{
		cout<<"Sorry, number not valid. 1 = pi0 first gamma, 2 = pi0 second gamma, 0 = single gamma";
		return;
	  }
    }
    else if(type=="EE"){ 
	  if(nPi0==1 || nPi0==2){
		//traincut = "TMath::Abs(MVA_nEta_1)>1.656 && TMath::Abs(MVA_nEta_1)<2.59";
		traincut = "TMath::Abs(MVA_nEta_1)>1.656 && TMath::Abs(MVA_nEta_1)<2.59 && (MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && (MVA_E3x3MC_2/MVA_E3x3_2)<3.0 && MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && MVA_E3x3_2>0.01 && MVA_Nxtal_1>6 && MVA_Nxtal_2>5 && MVA_E3x3MC_2>0.01 && MVA_Pt_1>0.5 && MVA_Pt_2>0.5 && MVA_S4S9_1>0.8 && MVA_S4S9_2>0.8 && MVA_EPi0>1.5";
		if(nPi0==1) cout<<"train on 1 gamma: cut "<<string(traincut)<<endl;
		if(nPi0==2) cout<<"train on 2 gamma: cut "<<string(traincut)<<endl;

	  }
	  else if(nPi0==0){
		traincut = "MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && (MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && MVA_Pt_1>0.55 && MVA_Nxtal_1>5 && MVA_S4S9_1>0.75";
		cout<<"train on single gamma: cut "<<string(traincut)<<endl;
	  }
	  else{
		cout<<"Sorry, number not valid. 1 = pi0 first gamma, 2 = pi0 second gamma, 0 = single gamma";
		return;
	  }
    }
    else cout<<"Sorry... Wrong type! Put EB or EE in the options"<<endl;

    train->SetTrainingCut(std::string(traincut));//####

    const double maxsig = 3.0;
    const double shrinkage = 0.1;

    train->SetMinEvents(200);
    train->SetShrinkage(shrinkage);  
    train->SetMinCutSignificance(maxsig);

    if(nPi0==1 || nPi0==0) train->SetTargetVar("MVA_E3x3MC_1/MVA_E3x3_1");//##
    if(nPi0==2 )           train->SetTargetVar("MVA_E3x3MC_2/MVA_E3x3_2");//##

    std::vector<std::string> *varsf = new std::vector<std::string>;

    if(type=="EB"){
	  if(nPi0==1){
		varsf->push_back("MVA_E3x3_1/MVA_E3x3_2");
		varsf->push_back("MVA_Pt_1");
		varsf->push_back("MVA_Nxtal_1");
//		varsf->push_back("MVA_Nxtal_2");
//		varsf->push_back("MVA_S4S9_1");
//		varsf->push_back("MVA_S1S9_1");
//		varsf->push_back("MVA_S2S9_1");
		varsf->push_back("MVA_Eta_1");
		varsf->push_back("MVA_Phi_1");
		varsf->push_back("sqrt(pow((MVA_Eta_1-MVA_Eta_2),2)+pow((MVA_Phi_1-MVA_Phi_2),2))");
		varsf->push_back("MVA_Eta_1on5");
		varsf->push_back("MVA_Phi_1on2");
		varsf->push_back("MVA_Eta_1on2520");
		varsf->push_back("MVA_Phi_1on20");
	  }
	  if(nPi0==2){
		varsf->push_back("MVA_E3x3_1/MVA_E3x3_2");
		varsf->push_back("MVA_Pt_2");
//		varsf->push_back("MVA_Nxtal_1");
		varsf->push_back("MVA_Nxtal_2");
//		varsf->push_back("MVA_S4S9_2");
//		varsf->push_back("MVA_S1S9_2");
//		varsf->push_back("MVA_S2S9_2");
		varsf->push_back("MVA_Eta_2");
		varsf->push_back("MVA_Phi_2");
		varsf->push_back("sqrt(pow((MVA_Eta_1-MVA_Eta_2),2)+pow((MVA_Phi_1-MVA_Phi_2),2))");
		varsf->push_back("MVA_Eta_2on5");
		varsf->push_back("MVA_Phi_2on2");
		varsf->push_back("MVA_Eta_2on2520");
		varsf->push_back("MVA_Phi_2on20");
	  }
	  if(nPi0==0){
		varsf->push_back("MVA_Pt_1");
		varsf->push_back("MVA_Nxtal_1");
		varsf->push_back("MVA_S4S9_1");
		varsf->push_back("MVA_S1S9_1");
		varsf->push_back("MVA_S2S9_1");
		varsf->push_back("MVA_Eta_1");
		varsf->push_back("MVA_Phi_1");
		varsf->push_back("MVA_Eta_1on5");
		varsf->push_back("MVA_Phi_1on2");
		varsf->push_back("MVA_Eta_1on2520");
		varsf->push_back("MVA_Phi_1on20");
	  }
    }
    else if(type=="EE"){
	  if(nPi0==1){
//		varsf->push_back("MVA_EPi0");
//		varsf->push_back("MVA_E3x3_1/MVA_EPi0");
//		varsf->push_back("MVA_E3x3_1/MVA_E3x3_2");
		varsf->push_back("MVA_Pt_1");
		varsf->push_back("MVA_Nxtal_1");
		varsf->push_back("MVA_Nxtal_2");
		//varsf->push_back("MVA_S4S9_1");
		//varsf->push_back("MVA_S1S9_1");
		//varsf->push_back("MVA_S2S9_1");
//		varsf->push_back("MVA_ES1_1");
//		varsf->push_back("MVA_ES2_1");
		//#varsf->push_back("(MVA_ES1_1+MVA_ES2_1)/MVA_E3x3_1");
		varsf->push_back("(MVA_ES1_1+MVA_ES2_1)");
		//varsf->push_back("MVA_nEta_1");
		//varsf->push_back("MVA_nPhi_1");
		//varsf->push_back("MVA_iX_1");
		//varsf->push_back("MVA_iY_1");
		varsf->push_back("EtaRing_1");
		//varsf->push_back("sqrt(pow(MVA_iX_1,2)+pow(MVA_iY_1,2))");
		//varsf->push_back("sqrt( pow(MVA_iX_1-MVA_iX_2,2) + pow(MVA_iY_1-MVA_iY_2,2))");
	  }
	  if(nPi0==2){
//		varsf->push_back("MVA_EPi0");
//		varsf->push_back("MVA_E3x3_2/MVA_EPi0");
//		varsf->push_back("MVA_E3x3_1/MVA_E3x3_2");
		varsf->push_back("MVA_Pt_2");
		varsf->push_back("MVA_Nxtal_1");
		varsf->push_back("MVA_Nxtal_2");
		//varsf->push_back("MVA_S4S9_2");
		//varsf->push_back("MVA_S1S9_2");
		//varsf->push_back("MVA_S2S9_2");
//		varsf->push_back("MVA_ES1_2");
//		varsf->push_back("MVA_ES2_2");
		//varsf->push_back("(MVA_ES1_2+MVA_ES2_2)/MVA_E3x3_2");
		varsf->push_back("(MVA_ES1_2+MVA_ES2_2)");
		//varsf->push_back("MVA_nEta_2");
		//varsf->push_back("MVA_nPhi_2");
		//varsf->push_back("MVA_iX_2");
		//varsf->push_back("MVA_iY_2");
		varsf->push_back("EtaRing_2");
		//varsf->push_back("sqrt(pow(MVA_iX_2,2)+pow(MVA_iY_2,2))");
		//varsf->push_back("sqrt( pow(MVA_iX_1-MVA_iX_2,2) + pow(MVA_iY_1-MVA_iY_2,2))");
	  }
    }
    else cout<<"Sorry... Wrong type! Put EB or EE"<<endl;


    for (unsigned int i=0; i<varsf->size(); ++i) {
	  cout << "var " << i << " = " << varsf->at(i) << endl;
	  train->AddInputVar(varsf->at(i));
    }

    ROOT::Cintex::Cintex::Enable();   
    GBRApply gbrapply;

    //Train Barrel Energy Regression
    intree->LoadTree(0);  
    const GBRForest *forest = train->TrainForest(nTrees);//Fa il Training
    delete train;

    //Apply Barrel Energy Regression
    intree->LoadTree(0);  
    gbrapply.ApplyAsFriend(intree, forest, *varsf, "targetp");

    TString fname;

    fname = outWeightFile;

    TFile *fout = new TFile(fname,"RECREATE");  
    cout << "Saving weights to file " << fname << endl;

    fout->WriteObject(forest,"Correction");
    fout->WriteObject(varsf, "varlist");

}
