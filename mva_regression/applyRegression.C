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


void applyRegression(string applyingFile, TString weightFilename,string trainingOrtesting, string outFileDirectory, string nTrees, string type) {
  ROOT::Cintex::Cintex::Enable();   

    TFile * weight_file = new TFile(weightFilename.Data());
    std::vector<string> *Var = ( std::vector<string>* )weight_file->Get("varlist");
    for(unsigned int i=0; i<Var->size(); i++ ){
         std::cout<<i<<std::endl;
    }
    const GBRForest * forest_eb_correction = (GBRForest *)weight_file->Get("Correction");
    assert(forest_eb_correction);

    TTree *intree = 0;
    TChain *chainele;
    if(type=="EB")      chainele = new TChain("h_mva");
    else if(type=="EE") chainele = new TChain("h_EEmva");
    else cout<<"Sorry... Wrong type! Put EB or EE"<<endl;
    chainele->Add(applyingFile.c_str());
    chainele->LoadTree(0);    
    chainele->SetCacheSize(64*1024*1024);
    chainele->SetCacheLearnEntries();
    intree = chainele;

    GBRApply gbrapply;

    std::vector<string> *varseb = (std::vector<string> *)weight_file->Get("varlist");

    TFile * outFile = new TFile((outFileDirectory + "/"+ trainingOrtesting + "_nTrees_"+nTrees+".root").c_str(), "RECREATE");

    //One or Two
    TString targ="", targ_name="";
    if( weightFilename.Contains("Pi0_1") || weightFilename.Contains("Eta_1") )      { cout<<"Running on gamma 1"<<endl; targ = "target_1_tree"; targ_name = "target_1"; }
    else if( weightFilename.Contains("Pi0_2") || weightFilename.Contains("Eta_2") ) { cout<<"Running on gamma 2"<<endl; targ = "target_2_tree"; targ_name = "target_2"; }
    else                                                                            { cout<<"Running on GAMMA"<<endl; targ = "target_1_tree"; targ_name = "target_1"; }

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
