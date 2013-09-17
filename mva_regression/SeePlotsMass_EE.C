#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>

#include "TFile.h"
#include "TLine.h"
#include "TFitResultPtr.h"
#include "TTree.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "THStack.h"
#include "TAttLine.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TChain.h"
#include "TMath.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TPaveStats.h"
#include "TLorentzVector.h"
#include "TPave.h"
#include "TLine.h"
#include "TCut.h"

void SeePlotsMass_EE(TString NameWeightTest, TString NameWeight1, TString NameWeight2 ){

    using namespace std;

    TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);
    TH1F *h_m = new TH1F("Pi0 Mass","Pi0 Mass",100,0.01,0.3);
    TH1F *h_m_corr = new TH1F("Pi0 Mass Corr","Pi0 Mass Corr",100,0.01,0.3);
    TH1F *h_m_corr1 = new TH1F("Pi0 Mass Corr (1)","Pi0 Mass Corr (1)",100,0.01,0.3);
    TH1F *h_m_corr2 = new TH1F("Pi0 Mass Corr (2)","Pi0 Mass Corr (2)",100,0.01,0.3);
    TH1F *h_DE1 = new TH1F("h_DE1","Delta E 1",50,-0.5,0.5);
    TH1F *h_DE2 = new TH1F("h_DE2","Delta E 2",50,-0.5,0.5);
    TLine *line = new TLine( 0.135, 0., 0.135, 100. );
    // Open Files, Create Output

    system( "mkdir -p Mass_Plot/" );

    TString Input = NameWeightTest + ".root";
    TFile* File = TFile::Open( Input.Data() );

    TString mode ="Test";
    TTree* Tree;
    Tree = (TTree*) File->Get("h_EEmva");
    TString Weight1 = NameWeight1 + ".root";
    TString Weight2 = NameWeight2 + ".root";
    Tree->AddFriend( "target_1_tree", Weight1.Data() ); //target_1
    Tree->AddFriend( "target_2_tree", Weight2.Data() ); //target_2

    // Style
    myc1->cd();
    gStyle->SetPalette(1);
    gStyle->SetOptStat(1111);

    //Mass
    Long64_t nentries = Tree->GetEntriesFast();
    Float_t En_1, En_2, EnMc_1, EnMc_2, Pt_1, Pt_2, Eta_1, Eta_2, Phi_1, Phi_2, S4S9_1, S4S9_2, Corr1, Corr2;
    Int_t Nxtal_1, Nxtal_2;
    bool Conv1, Conv2;
    Tree->SetBranchAddress( "MVA_E3x3_1", &En_1);
    Tree->SetBranchAddress( "MVA_E3x3_2", &En_2);
    Tree->SetBranchAddress( "MVA_E3x3MC_1", &EnMc_1);
    Tree->SetBranchAddress( "MVA_E3x3MC_2", &EnMc_2);
    Tree->SetBranchAddress( "MVA_Nxtal_1", &Nxtal_1);
    Tree->SetBranchAddress( "MVA_Nxtal_2", &Nxtal_2);
    Tree->SetBranchAddress( "MVA_S4S9_1", &S4S9_1);
    Tree->SetBranchAddress( "MVA_S4S9_2", &S4S9_2);
    Tree->SetBranchAddress( "MVA_Pt_1", &Pt_1);
    Tree->SetBranchAddress( "MVA_Pt_2", &Pt_2);
    Tree->SetBranchAddress( "MVA_nEta_1", &Eta_1);
    Tree->SetBranchAddress( "MVA_nEta_2", &Eta_2);
    Tree->SetBranchAddress( "MVA_nPhi_1", &Phi_1);
    Tree->SetBranchAddress( "MVA_nPhi_2", &Phi_2);
    Tree->SetBranchAddress( "MVA_isConv_1", &Conv1);
    Tree->SetBranchAddress( "MVA_isConv_2", &Conv2);
    Tree->SetBranchAddress( "target_1", &Corr1);
    Tree->SetBranchAddress( "target_2", &Corr2);

    //My Branch
    TFile* MassFile = new TFile("Mass_Plot/MassTrre.root","RECREATE");
    TTree* Final_tree = new TTree("Final_tree","Pi0 Analysis Tree");
    Float_t Mass_pi0, Mass_pi0_corr, s4s9_1, s4s9_2, GaPt_1, GaPt_2, E3x3MC1, E3x3MC2, E3x31, E3x32, cor1, cor2;
    Int_t xtal_1, xtal_2;
    bool Conve_1, Conve_2;
    Final_tree->Branch( "Mass_pi0", &Mass_pi0, "Mass_pi0/F");
    Final_tree->Branch( "Mass_pi0_corr", &Mass_pi0_corr, "Mass_pi0_corr/F");
    Final_tree->Branch( "s4s9_1", &s4s9_1, "s4s9_1/F");
    Final_tree->Branch( "s4s9_2", &s4s9_2, "s4s9_2/F");
    Final_tree->Branch( "GaPt_1", &GaPt_1, "GaPt_1/F");
    Final_tree->Branch( "GaPt_2", &GaPt_2, "GaPt_2/F");
    Final_tree->Branch( "xtal_1", &xtal_1, "xtal_1/I");
    Final_tree->Branch( "xtal_2", &xtal_2, "xtal_2/I");
    Final_tree->Branch( "Conve_1", &Conve_1, "Conve_1/O");
    Final_tree->Branch( "Conve_2", &Conve_2, "Conve_2/O");
    Final_tree->Branch( "E3x3MC1", &E3x3MC1, "E3x3MC1/F");
    Final_tree->Branch( "E3x3MC2", &E3x3MC2, "E3x3MC2/F");
    Final_tree->Branch( "E3x31", &E3x31, "E3x31/F");
    Final_tree->Branch( "E3x32", &E3x32, "E3x32/F");
    Final_tree->Branch( "cor1", &cor1, "cor1/F");
    Final_tree->Branch( "cor2", &cor2, "cor2/F");

    //Basic Cut
    //const TCut *traincut = new TCut("cut","(MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && (MVA_E3x3MC_2/MVA_E3x3_2)<3.0 && MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && MVA_E3x3_2>0.01 && MVA_Nxtal_1>6 && MVA_Nxtal_2>4 &&MVA_E3x3MC_2>0.01 && MVA_Pt_1>0.5 && MVA_Pt_2>0.5 && MVA_S4S9_1>0.8 && MVA_S4S9_2>0.8 && MVA_EPi0>0.8");
    const TCut *traincut = new TCut("TMath::Abs(MVA_nEta_1)>1.656 && TMath::Abs(MVA_nEta_1)<2.59 && (MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && (MVA_E3x3MC_2/MVA_E3x3_2)<3.0 && MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && MVA_E3x3_2>0.01 && MVA_Nxtal_1>6 && MVA_Nxtal_2>5 && MVA_E3x3MC_2>0.01 && MVA_Pt_1>0.5 && MVA_Pt_2>0.5 && MVA_S4S9_1>0.8 && MVA_S4S9_2>0.8 && MVA_EPi0>1.5");

    TLorentzVector Gamma1, Gamma2;
    TLorentzVector Gamma1_corr, Gamma2_corr;
    for(Long64_t iEntry=0; iEntry<nentries; iEntry++){
	  Tree->GetEntry(iEntry);
	  Gamma1.SetPtEtaPhiE(En_1/cosh(Eta_1), Eta_1, Phi_1, En_1);
	  Gamma2.SetPtEtaPhiE(En_2/cosh(Eta_2), Eta_2, Phi_2, En_2);
	  Gamma1_corr.SetPtEtaPhiE((En_1*Corr1)/cosh(Eta_1), Eta_1, Phi_1, En_1*Corr1);
	  Gamma2_corr.SetPtEtaPhiE((En_2*Corr2)/cosh(Eta_2), Eta_2, Phi_2, En_2*Corr2);
	  Mass_pi0      = (Gamma1+Gamma2).M();
	  Mass_pi0_corr = (Gamma1_corr+Gamma2_corr).M();
	  Conve_1 = Conv1; Conve_2 = Conv2;
	  s4s9_1 = S4S9_1; s4s9_2 = S4S9_2;
	  GaPt_1 = Gamma1.Pt(); GaPt_2 = Gamma2.Pt();
	  xtal_1 = Nxtal_1; xtal_2 = Nxtal_2;
	  E3x3MC1 = EnMc_1; E3x3MC2 = EnMc_2;
	  E3x31 = En_1; E3x32 = En_2;
	  cor1=Corr1; cor2=Corr2;
	  Final_tree->Fill();
	  if( Nxtal_1>7 && Nxtal_2>5 && S4S9_1>0.85 && S4S9_2>0.85 && Gamma1.Pt()>0.5 && Gamma2.Pt()>0.5 && (Gamma1+Gamma2).Pt()>0.8){
		h_DE1->Fill( (fabs(En_1-EnMc_1)-fabs(En_1*Corr1-EnMc_1))/EnMc_1 );
		h_DE2->Fill( (fabs(En_2-EnMc_2)-fabs(En_2*Corr2-EnMc_2))/EnMc_2 );
		h_m->Fill( (Gamma1+Gamma2).M() );
		h_m_corr->Fill( (Gamma1_corr+Gamma2_corr).M() );
		h_m_corr1->Fill( (Gamma1_corr+Gamma2).M() );
		h_m_corr2->Fill( (Gamma1+Gamma2_corr).M() );
       }
    }
    MassFile->cd();
    traincut->Write();
    Final_tree->Write();
    MassFile->Write();
    MassFile->Close();

    //Both Corrected
    h_DE1->Draw();
    myc1->SaveAs("Mass_Plot/h_DE1.png");
    h_DE2->Draw();
    myc1->SaveAs("Mass_Plot/h_DE2.png");
    line->SetY2( h_m_corr->GetMaximum() );
    h_m->SetLineColor(2); line->SetLineColor(1);
    h_m_corr->Draw(); h_m->Draw("same"); line->Draw("same");
    myc1->SaveAs("Mass_Plot/Mass_RedNoCorr_BlueCorr.png");
    //1st Corrected
    line->SetY2( h_m_corr1->GetMaximum() );
    h_m_corr1->Draw(); h_m->Draw("same"); line->Draw("same");
    myc1->SaveAs("Mass_Plot/Mass_RedNoCorr_BlueCorr_Only1.png");
    //1st Corrected
    line->SetY2( h_m_corr2->GetMaximum() );
    h_m_corr2->Draw(); h_m->Draw("same"); line->Draw("same");
    myc1->SaveAs("Mass_Plot/Mass_RedNoCorr_BlueCorr_Only2.png");

    delete myc1;
    }
