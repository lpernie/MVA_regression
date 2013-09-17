//================================================================================================
//                      applyElectronEnergyRegression.C
// 
// Applies the weights created by trainElectronEnergyRegression.C to the desired ntuple file, and
// generates a .root file with "targets" i.e. the ratios to obtain the regression energy, as well
// as .gif plots for diagnostic
//
// USAGE
// applyElectronEnergyRegression(string applyingFile, string weightFilename, string outFileDirectory )
//
// applyingFile		= ntuple file to which apply the regression
// weightFilename	= file outputted by trainElectronEnergyRegression.C containing the weights
// outFileDirectory	= directory to which output the .gif and .root files (must end in /)
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
#include "TCanvas.h"
#include <string.h>
#include "TString.h"


void applyRegression_SingToPi(string applyingFile, TString weightFilename, TString trainingOrtesting, TString outFileDirectory, string nTrees) {
  ROOT::Cintex::Cintex::Enable();   

    TFile * weight_file = new TFile(weightFilename.Data());
    const GBRForest * forest_eb_correction = (GBRForest *)weight_file->Get("Correction");
    assert(forest_eb_correction);

    TTree *intree = 0;
    TChain *chainele = new TChain("h_mva");
    chainele->Add(applyingFile.c_str());
    chainele->LoadTree(0);    
    chainele->SetCacheSize(64*1024*1024);
    chainele->SetCacheLearnEntries();
    intree = chainele;

    GBRApply gbrapply;

    TFile * outFile = new TFile((outFileDirectory + "/"+ trainingOrtesting + "_nTrees_"+nTrees+".root").Data(), "RECREATE");

    //One or Two
    TString targ="", targ_name="";
    if( trainingOrtesting.Contains("Pi01") )      { cout<<"Running on Pi0 1"<<endl; targ = "target_1_tree"; targ_name = "target_1"; }
    else if( trainingOrtesting.Contains("Pi02") ) { cout<<"Running on Pi0 2"<<endl; targ = "target_2_tree"; targ_name = "target_2"; }
    else                                           { cout<<"Running on GAMMA"<<endl; targ = "target_1_tree"; targ_name = "target_1"; }

    std::vector<string> *varseb = new std::vector<std::string>;
    if(targ == "target_1_tree"){
    varseb->push_back("MVA_Pt_1");
    varseb->push_back("MVA_Nxtal_1");
    varseb->push_back("MVA_S4S9_1");
    varseb->push_back("MVA_S1S9_1");
    varseb->push_back("MVA_S2S9_1");
    varseb->push_back("MVA_Eta_1");
    varseb->push_back("MVA_Phi_1");
    varseb->push_back("MVA_Eta_1on5");
    varseb->push_back("MVA_Phi_1on2");
    varseb->push_back("MVA_Eta_1on2520");
    varseb->push_back("MVA_Phi_1on20");
    }
    if(targ == "target_2_tree"){
    varseb->push_back("MVA_Pt_2");
    varseb->push_back("MVA_Nxtal_2");
    varseb->push_back("MVA_S4S9_2");
    varseb->push_back("MVA_S1S9_2");
    varseb->push_back("MVA_S2S9_2");
    varseb->push_back("MVA_Eta_2");
    varseb->push_back("MVA_Phi_2");
    varseb->push_back("MVA_Eta_2on5");
    varseb->push_back("MVA_Phi_2on2");
    varseb->push_back("MVA_Eta_2on2520");
    varseb->push_back("MVA_Phi_2on20");
    }
 
    intree->LoadTree(0);
    TTree *intree_eb = gbrapply.ApplyAsFriend(intree, forest_eb_correction, *varseb, targ_name.Data());
    intree_eb->SetName(targ.Data());
    intree_eb->SetTitle(targ.Data());

    TH1F* mvaHisto_eb=new TH1F("mvaHisto_eb","mvaHisto_eb",1000,-0.2,0.2);
    intree->Project(mvaHisto_eb->GetName(),targ_name.Data(),"");

    outFile->cd();
    intree_eb->Write();
    
    mvaHisto_eb->Write();
    outFile->Write();
    outFile->Close();
}
