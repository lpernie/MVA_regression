#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "TTree.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TRegexp.h"
#include "TLegend.h"
#include "TLatex.h"

#include "../MitEdm/GBRTrain/interface/GBRApply.h"
#include "../CondFormats/EgammaObjects/interface/GBRForest.h"
#include "Cintex/Cintex.h"
#include "TLorentzVector.h"

using namespace TMVA;

void Test_CC_onTest(int NumEvents=-1){

    //Opening FILES
    TString nameFile = "MVA_EtPi0_testing.root";
    TFile *Test_file = TFile::Open( nameFile.Data() ); 
    if(Test_file==0 || Test_file->IsZombie()) {
	  std::cerr << "Opening ROOT Test_file failed!" << std::endl;  exit(1);
    }
    nameFile = "/afs/cern.ch/work/l/lpernie/ECALpro/gitHubCalib/CMSSW_5_3_6/src/CalibCode/submit/common/JOSH_MVA_pi01_Mediumtrain.root";
    TFile *MVA1_file = TFile::Open( nameFile.Data() ); 
    if(MVA1_file==0 || MVA1_file->IsZombie()) {
	  std::cerr << "Opening ROOT MVA1_file failed!" << std::endl;  exit(1);
    }
    nameFile = "/afs/cern.ch/work/l/lpernie/ECALpro/gitHubCalib/CMSSW_5_3_6/src/CalibCode/submit/common/JOSH_MVA_pi02_Mediumtrain.root";
    TFile *MVA2_file = TFile::Open( nameFile.Data() ); 
    if(MVA2_file==0 || MVA2_file->IsZombie()) {
	  std::cerr << "Opening ROOT MVA2_file failed!" << std::endl;  exit(1);
    }

    //Pi0 Variables
    Float_t En_1, En_2, EnMc_1, EnMc_2, Pt_1, Pt_2, S4S9_1, S4S9_2, S2S9_1, S2S9_2, S1S9_1, S1S9_2, nEta_1, nEta_2, nPhi_1, nPhi_2;
    Int_t Nxtal_1, Nxtal_2, Eta_1, Eta_2, Phi_1, Phi_2;
    bool Conv1, Conv2;
    TTree* Tree;
    Tree = (TTree*) Test_file->Get("h_mva");
    Tree->SetBranchAddress( "MVA_E3x3_1", &En_1);
    Tree->SetBranchAddress( "MVA_E3x3_2", &En_2);
    Tree->SetBranchAddress( "MVA_E3x3MC_1", &EnMc_1);
    Tree->SetBranchAddress( "MVA_E3x3MC_2", &EnMc_2);
    Tree->SetBranchAddress( "MVA_Nxtal_1", &Nxtal_1);
    Tree->SetBranchAddress( "MVA_Nxtal_2", &Nxtal_2);
    Tree->SetBranchAddress( "MVA_S4S9_1", &S4S9_1);
    Tree->SetBranchAddress( "MVA_S4S9_2", &S4S9_2);
    Tree->SetBranchAddress( "MVA_S2S9_1", &S2S9_1);
    Tree->SetBranchAddress( "MVA_S2S9_2", &S2S9_2);
    Tree->SetBranchAddress( "MVA_S1S9_1", &S1S9_1);
    Tree->SetBranchAddress( "MVA_S1S9_2", &S1S9_2);
    Tree->SetBranchAddress( "MVA_Pt_1", &Pt_1);
    Tree->SetBranchAddress( "MVA_Pt_2", &Pt_2);
    Tree->SetBranchAddress( "MVA_Eta_1", &Eta_1);
    Tree->SetBranchAddress( "MVA_Eta_2", &Eta_2);
    Tree->SetBranchAddress( "MVA_Phi_1", &Phi_1);
    Tree->SetBranchAddress( "MVA_Phi_2", &Phi_2);
    Tree->SetBranchAddress( "MVA_nEta_1", &nEta_1);
    Tree->SetBranchAddress( "MVA_nEta_2", &nEta_2);
    Tree->SetBranchAddress( "MVA_nPhi_1", &nPhi_1);
    Tree->SetBranchAddress( "MVA_nPhi_2", &nPhi_2);
    Tree->SetBranchAddress( "MVA_isConv_1", &Conv1);
    Tree->SetBranchAddress( "MVA_isConv_2", &Conv2);

    //MVA Corrections
    const GBRForest *forest_EB_1 = (GBRForest *)MVA1_file->Get("Correction");
    const GBRForest *forest_EB_2 = (GBRForest *)MVA2_file->Get("Correction");

    //MY TTREE
    TFile* FinalFile = new TFile("TestCC_as_Calib.root","RECREATE");
    TTree* Final_tree = new TTree("Final_tree","Test CC as Calib Tree");
    Float_t Mass_pi0, Mass_pi0_corr, corr1, corr2;
    Int_t iETA1, iETA2, iPHI1, iPHI2;
    bool Conve_1, Conve_2;
    Final_tree->Branch( "corr1", &corr1, "corr1/F");
    Final_tree->Branch( "corr2", &corr2, "corr2/F");
    Final_tree->Branch( "Mass_pi0", &Mass_pi0, "Mass_pi0/F");
    Final_tree->Branch( "Mass_pi0_corr", &Mass_pi0_corr, "Mass_pi0_corr/F");
    Final_tree->Branch( "iETA1", &iETA1, "iETA1/I");
    Final_tree->Branch( "iETA2", &iETA2, "iETA2/I");
    Final_tree->Branch( "iPHI1", &iPHI1, "iPHI1/I");
    Final_tree->Branch( "iPHI2", &iPHI2, "iPHI2/I");

    //LOOP
    Long64_t nentries = NumEvents==-1 ? Tree->GetEntriesFast() : NumEvents;
    for(Long64_t iEntry=0; iEntry<nentries; iEntry++){
	  Tree->GetEntry(iEntry);

	  float value_pi01[14];
	  value_pi01[0] = ( En_1/En_2 );
	  value_pi01[1] = ( Pt_1 );
	  value_pi01[2] = ( Nxtal_1 );
	  value_pi01[3] = ( Nxtal_2 );
	  value_pi01[4] = ( S4S9_1 );
	  value_pi01[5] = ( S1S9_1 );
	  value_pi01[6] = ( S2S9_1 );
	  value_pi01[7] = ( Eta_1 );
	  value_pi01[8] = ( Phi_1 );
	  value_pi01[9] = ( sqrt(pow((Eta_1-Eta_2),2)+pow((Phi_1-Phi_2),2)));
	  value_pi01[10] = ( Eta_1%5 );
	  value_pi01[11] = ( Phi_1%2 );
	  value_pi01[12] = ( (TMath::Abs(Eta_1)<=25)*(Eta_1%25) + (TMath::Abs(Eta_1)>25)*((Eta_1-25*TMath::Abs(Eta_1)/Eta_1)%20) );
	  value_pi01[13] = ( Phi_1%20 );
	  float Correct1 = forest_EB_1->GetResponse(value_pi01);
	  cout<<"Correction1: "<<Correct1<<" iEta: "<<Eta_1<<" iPhi "<<Phi_1<<" E "<<En_1<<endl;
	  float value_pi02[14];
	  value_pi02[0] = ( En_1/En_2 );
	  value_pi02[1] = ( Pt_2 );
	  value_pi02[2] = ( Nxtal_1 );
	  value_pi02[3] = ( Nxtal_2 );
	  value_pi02[4] = ( S4S9_2 );
	  value_pi02[5] = ( S1S9_2 );
	  value_pi02[6] = ( S2S9_2 );
	  value_pi02[7] = ( Eta_2 );
	  value_pi02[8] = ( Phi_2 );
	  value_pi02[9] = ( sqrt(pow((Eta_1-Eta_2),2)+pow((Phi_1-Phi_2),2)));
	  value_pi02[10] = ( Eta_2%5 );
	  value_pi02[11] = ( Phi_2%2 );
	  value_pi02[12] = ( (TMath::Abs(Eta_2)<=25)*(Eta_2%25) + (TMath::Abs(Eta_2)>25)*((Eta_2-25*TMath::Abs(Eta_2)/Eta_2)%20) );
	  value_pi02[13] = ( Phi_2%20 );
	  float Correct2 = forest_EB_2->GetResponse(value_pi02);
	  cout<<"Correction2: "<<Correct2<<" iEta: "<<Eta_2<<" iPhi "<<Phi_2<<" E "<<En_2<<endl;
	  //TTree Final
	  iETA1 = Eta_1; iETA2 = Eta_2; iPHI1 = Phi_1; iPHI2 = Phi_2;
	  corr1 = Correct1; corr2 = Correct2;
	  TLorentzVector Gamma1, Gamma2;
	  TLorentzVector Gamma1_corr, Gamma2_corr;
	  Gamma1.SetPtEtaPhiE(En_1/cosh(nEta_1), nEta_1, nPhi_1, En_1);
	  Gamma2.SetPtEtaPhiE(En_2/cosh(nEta_2), nEta_2, nPhi_2, En_2);
	  Gamma1_corr.SetPtEtaPhiE((En_1*corr1)/cosh(nEta_1), nEta_1, nPhi_1, En_1*corr1);
	  Gamma2_corr.SetPtEtaPhiE((En_2*corr2)/cosh(nEta_2), nEta_2, nPhi_2, En_2*corr2);
	  Mass_pi0      = (Gamma1+Gamma2).M();
	  Mass_pi0_corr = (Gamma1_corr+Gamma2_corr).M();
	  Final_tree->Fill();
    }
    FinalFile->cd();
    Final_tree->Write();
    FinalFile->Write();
    FinalFile->Close();
}
