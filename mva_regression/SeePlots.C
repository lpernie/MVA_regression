#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>

#include "TFile.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TCut.h"
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
#include "TColor.h"
#include "TPaveStats.h"
#include "TPave.h"
#include "TStyle.h"
#include <TSystem.h>

void SeePlots(TString NameWeightTest, TString NameWeight ){

    using namespace std;

    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

    TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);

    // Open Files, Create Output

    bool GAMMA_GUN=false;
    TString Type = "";
    TString Friend = "";
    if     ( NameWeight.Contains("Pi01") || NameWeight.Contains("Eta1") ) { Type = "1"; Friend = "target_1"; cout<<"Pi0 gamma 1"<<endl; }
    else if( NameWeight.Contains("Pi02") || NameWeight.Contains("Eta2") ) { Type = "2"; Friend = "target_2"; cout<<"Pi0 gamma 2"<<endl; }
    else                                                                  { Type = "1"; Friend = "target_1"; cout<<"Single Gamma"<<endl; GAMMA_GUN=true;}

    system( (string("mkdir -p ") + NameWeight).Data());

    TString Input = NameWeightTest + ".root";
    TFile* File = TFile::Open( Input.Data() );

    TString mode ="Test";
    TTree* Tree;
    Tree = (TTree*) File->Get("h_mva");
    Input = NameWeight + ".root";
    if( Type=="1" ) Tree->AddFriend("target_1_tree",Input.Data());
    if( Type=="2" ) Tree->AddFriend("target_2_tree",Input.Data());

    // Style
    myc1->cd();
    gStyle->SetOptStat(1111);

    //MVA cuts
    TCut traincut;
    if( !GAMMA_GUN && (Type == "1" || Type == "2") ) traincut = "MVA_E3x3_1>0.01 && MVA_E3x3_2>0.01 && MVA_Pt_1>1.4 && MVA_Pt_2>1.4 && MVA_S4S9_1>0.85 && MVA_S4S9_2>0.85 && MVA_Nxtal_1>6 && MVA_Nxtal_2>4";
    if( GAMMA_GUN )                                  traincut = "MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && (MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && MVA_Pt_1>0.55 && MVA_Nxtal_1>5 && MVA_S4S9_1>0.75";

    //Plots
    TString Output = NameWeight + "/" + mode + "_Risol_RedNoweight_BackWeight.png";
    TH1F *h1 = new TH1F("h1","Risol [red no weight]",100,-1,1);
    Tree->Draw("(((MVA_E3x3_"+Type+"*"+Friend+")-MVA_E3x3MC_"+Type+")/MVA_E3x3MC_"+Type+")>>h1","((MVA_E3x3_"+Type+"*"+Friend+")-MVA_E3x3MC_"+Type+")/MVA_E3x3MC_"+Type+"<1 && "+ string(traincut) );
    TH1F *h2 = new TH1F("h2","Risol 2 [red no weight]",100,-1,1);
    Tree->Draw("((MVA_E3x3_"+Type+"-MVA_E3x3MC_"+Type+")/MVA_E3x3MC_"+Type+")>>h2","(MVA_E3x3_"+Type+"-MVA_E3x3MC_"+Type+")/MVA_E3x3MC_"+Type+"<1 && "+ string(traincut));
    h1->Draw();
    h1->Draw(); h2->SetLineColor(2); h2->Draw("SAMES");
    myc1->SaveAs(Output.Data());
    delete h1; delete h2;

    Output = NameWeight + "/" + mode + "_Correction.png";
    TH1F *h1a = new TH1F("h1a","Correction",360,0,360);
    Tree->Draw(""+Friend+">>h1a");
    h1a->Draw();
    myc1->SaveAs(Output.Data());
    delete h1a;


/*
    //TH2
    gStyle->SetOptStat(0);
    Output = NameWeight + "/" + mode + "_Risol_RedNoweight_BackWeight.png";
    TH2F *mva_phi_eta = new TH2F("mva_phi_eta","MVA Correction  #phi on x, #eta on y",360,0.5,360.5,171,-85.5,85.5);
    Tree->Draw(""+Friend+":MVA_Eta_"+Type+":MVA_Phi_"+Type+">>mva_phi_eta",traincut,"TProfile2D" );
    mva_phi_eta->GetZaxis()->SetRangeUser(0.95,1.25);
    mva_phi_eta->Draw("colz");
    myc1->SaveAs(Output.Data());
    delete mva_phi_eta; 
*/

    for(int j=0; j<2; j++){
	  TString num = "";
	  if( j==0 ) num = "1";
	  else       num = "2";
	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_iEta_"+ num +".png";
	  TH2F *h3;
	  if( Type==1 ) h3 = new TH2F("h3",""+Friend+" vs iETA_"+num+"",171,-85.5,85.5,100,0.8,1.35);
	  else          h3 = new TH2F("h3",""+Friend+" vs iETA_"+num+"",171,-85.5,85.5,150,0.8,1.5);
	  gStyle->SetOptStat(0);
	  Tree->Draw(""+Friend+":MVA_Eta_"+num+">>h3",traincut,"colz");
	  h3->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h3;


	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_iPhi_"+ num +".png";
	  gStyle->SetOptStat(0);
	  TH2F *h5;
	  if( Type==1 ) h5 = new TH2F("h5",""+Friend+" vs iPHI_"+num+"",360, 0.5, 360.5, 100, 0.8, 1.35);
	  else          h5 = new TH2F("h5",""+Friend+" vs iPHI_"+num+"",360, 0.5, 360.5, 150, 0.8, 1.5);
	  Tree->Draw(""+Friend+":MVA_Phi_"+num+">>h5",(traincut),"colz");
	  h5->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h5;

	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_Nvtx_"+ num +".png";
	  gStyle->SetOptStat(0);
	  Tree->Draw(""+Friend+":MVA_Nxtal_"+num+"", (traincut),"colz");
	  myc1->SaveAs(Output.Data());

	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_Pt_"+ num +".png";
	  TH2F *h26;
	  if( Type==1 ) h26 = new TH2F("h26",""+Friend+" vs Pt_"+Type+"", 100, 0., 15., 100, 0.9, 1.2);
	  else          h26 = new TH2F("h26",""+Friend+" vs Pt_"+Type+"", 100, 0., 6., 100, 0.9, 1.2);
	  gStyle->SetOptStat(0);
	  Tree->Draw(""+Friend+":MVA_Pt_"+num+">>h26", (traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h26;

	  if(GAMMA_GUN) j=2;
    }

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_E.png";
    TH2F *h27 = new TH2F("h27",""+Friend+" vs E_"+Type+"", 100, 0., 15., 100, 0.9, 1.2);
    gStyle->SetOptStat(0);
    Tree->Draw(""+Friend+":MVA_E3x3_"+Type+">>h27",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h27;

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_S4S9.png";
    TH2F *h10 = new TH2F("h10",""+Friend+" vs S4S9_"+Type+"", 100, 0.88, 1.01, 100, 0.9, 1.2);
    gStyle->SetOptStat(0);
    Tree->Draw(""+Friend+":MVA_S4S9_"+Type+">>h10",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h10;

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_S2S9.png";
    TH2F *h11 = new TH2F("h11",""+Friend+" vs S2S9_"+Type+"", 100, 0.75, 0.94, 100, 0.9, 1.2);
    gStyle->SetOptStat(0);
    Tree->Draw(""+Friend+":MVA_S2S9_"+Type+">>h11",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h11;

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_S1S9.png";
    TH2F *h12 = new TH2F("h12",""+Friend+" vs S1S9_"+Type+"", 100, 0.7, 0.88, 100, 0.9, 1.2);
    gStyle->SetOptStat(0);
    Tree->Draw(""+Friend+":MVA_S1S9_"+Type+">>h12",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h12;

    if(!GAMMA_GUN){
	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_E1E2.png";
	  TH2F *h32 = new TH2F("h32",""+Friend+" vs E1E2", 100, 0.9, 5.0, 100, 0.9, 1.2);
	  gStyle->SetOptStat(0);
	  Tree->Draw(""+Friend+":(MVA_E3x3_1/MVA_E3x3_2)>>h32",(traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h32;
    }

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_RatioEnergies.png";
    gStyle->SetOptStat(0);
    TH2F *h7 = new TH2F("h7",""+Friend+" vs Emc/Erec_"+Type+"",100, 0.8, 1.2, 100,0.8,1.2);
    Tree->Draw(""+Friend+":(MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+")>>h7","(MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+")>0.5 && (MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+")<1.5 && "+ string(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h7;

    //Now vs Emc/Erec

    for(int j=0; j<2; j++){
	  TString num = "";
	  if( j==0 ) num = "1";
	  else       num = "2";
	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_iEta_"+ num +".png";
	  TH2F *h8 = new TH2F("h8","Emc/Erec vs iETA_"+num+"", 171, -85.5, 85.5,100,0.8,1.35);
	  gStyle->SetOptStat(0);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_Eta_"+num+">>h8",(traincut),"colz");
	  h8->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h8;

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_iPhi_"+ num +".png";
	  TH2F *h9 = new TH2F("h9","Emc/Erec vs iPHI_"+num+"", 360, 0.5, 360.5, 100, 0.8, 1.35);
	  gStyle->SetOptStat(0);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_Phi_"+num+">>h9",(traincut),"colz");
	  h9->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h9;

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_Nvtx_"+ num +".png";
	  gStyle->SetOptStat(0);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_Nxtal_"+num+"",(traincut),"colz");
	  myc1->SaveAs(Output.Data());

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_Pt_"+ num +".png";
	  TH2F *h28 = new TH2F("h28","Emc/Erec vs Pt_"+num+"", 100, 0., 15., 100, 0.9, 1.2);
	  gStyle->SetOptStat(0);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_Pt_"+num+">>h28",(traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h28;

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_E_"+ num +".png";
	  TH2F *h29 = new TH2F("h29","Emc/Erec vs E_"+num+"", 100, 0., 15., 100, 0.9, 1.2);
	  gStyle->SetOptStat(0);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_E3x3_"+num+">>h29",(traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h29;

	  if(GAMMA_GUN) j=2;
    }

    Output = NameWeight + "/" + mode + "_ErecEmc_2D_S4S9.png";
    TH2F *h13 = new TH2F("h13","Emc/Erec vs S4S9_"+Type+"", 100, 0.88, 1.01, 100, 0.9, 1.2);
    gStyle->SetOptStat(0);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S4S9_"+Type+">>h13",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h13;

    Output = NameWeight + "/" + mode + "_ErecEmc_2D_S2S9.png";
    TH2F *h14 = new TH2F("h14","Emc/Erecvs S2S9_"+Type+"", 100, 0.75, 0.94, 100, 0.9, 1.2);
    gStyle->SetOptStat(0);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S2S9_"+Type+">>h14",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h14;

    Output = NameWeight + "/" + mode + "_ErecEmc_2D_S1S9.png";
    TH2F *h15 = new TH2F("h15","Emc/Erec vs S1S9_"+Type+"", 100, 0.7, 0.88, 100, 0.9, 1.2);
    gStyle->SetOptStat(0);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S1S9_"+Type+">>h15",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h15;

    if(!GAMMA_GUN){
	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_E1E2.png";
	  TH2F *h33 = new TH2F("h33","Emc/Erec vs E1E2_", 100, 0.9, 5.0, 100, 0.9, 1.2);
	  gStyle->SetOptStat(0);
	  Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_E3x3_1/MVA_E3x3_2>>h33",(traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h33;
    }
    //Profile Comparison

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_iEta.png";
    gStyle->SetOptStat(0);
    TProfile *h4 = new TProfile("h4",""+Friend+" vs iETA_"+Type+" (profile) red="+Friend+" back=Erec/Emc",171,-85.5,85.5, 0.9, 1.1);
    Tree->Draw(""+Friend+":MVA_Eta_"+Type+">>h4",(traincut),"ProfileX");
    h4->SetMinimum(1.); h4->SetMaximum(1.08); h4->SetLineColor(2);
    TProfile *h16 = new TProfile("h16","Emc/Erec vs iETA_"+Type+" (profile)",171,-85.5,85.5, 0.9, 1.1);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_Eta_"+Type+">>h16",(traincut),"ProfileX");
    h4->Draw("colz"); h16->SetMinimum(1.); h16->SetMaximum(1.08); h16->Draw("colz same");
    myc1->SaveAs(Output.Data());
    delete h4; delete h16;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_iPhi.png";
    gStyle->SetOptStat(0);
    TProfile *h6 = new TProfile("h6",""+Friend+" vs iPHI_"+Type+" (profile) red="+Friend+" back=Erec/Emc",360,0.5, 360.5, 0.9, 1.1);
    Tree->Draw(""+Friend+":MVA_Phi_"+Type+">>h6",(traincut),"ProfileX");
    h6->SetMinimum(1.); h6->SetMaximum(1.08); h6->SetLineColor(2);
    TProfile *h17 = new TProfile("h17","Emc/Erec vs iPHI_"+Type+" (profile)",360,0.5, 360.5, 0.9, 1.1);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_Phi_"+Type+">>h17",(traincut),"ProfileX");
    h6->Draw("colz"); h17->SetMinimum(1.); h17->SetMaximum(1.08); h17->Draw("colz same");
    myc1->SaveAs(Output.Data());
    delete h6; delete h17;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_S4S9.png";
    gStyle->SetOptStat(0);
    TProfile *h18 = new TProfile("h18",""+Friend+" vs S4S9_"+Type+" (profile) red="+Friend+" back=Erec/Emc", 100, 0.75, 0.92, 0.9, 1.1);
    Tree->Draw(""+Friend+":MVA_S1S9_"+Type+">>h18",(traincut),"ProfileX");
    h18->SetMinimum(0.95); h18->SetLineColor(2);
    TProfile *h19 = new TProfile("h19","Emc/Erec vs S4S9_"+Type+" (profile)",100, 0.75, 1.1, 0.9, 1.1);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S1S9_"+Type+">>h19",(traincut),"ProfileX");
    h18->Draw("colz"); h19->SetMinimum(0.95); h19->Draw("colz same");
    myc1->SaveAs(Output.Data());
    delete h18; delete h19;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_S1S9.png";
    gStyle->SetOptStat(0);
    TProfile *h20 = new TProfile("h20",""+Friend+" vs S1S9_"+Type+" (profile) red="+Friend+" back=Erec/Emc", 100, 0.58, 0.94, 0.9, 1.1);
    Tree->Draw(""+Friend+":MVA_S1S9_"+Type+">>h20",(traincut),"ProfileX");
    h20->SetMinimum(0.95); h20->SetLineColor(2);
    TProfile *h21 = new TProfile("h21","Emc/Erec vs S1S9_"+Type+" (profile)",100, 0.58, 0.94, 0.9, 1.1);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S1S9_"+Type+">>h21",(traincut),"ProfileX");
    h20->Draw("colz"); h21->SetMinimum(0.95); h21->Draw("colz same");
    myc1->SaveAs(Output.Data());
    delete h20; delete h21;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_S2S9.png";
    gStyle->SetOptStat(0);
    TProfile *h22 = new TProfile("h22",""+Friend+" vs S2S9_"+Type+" (profile) red="+Friend+" back=Erec/Emc", 100, 0.58, 0.94, 0.9, 1.1);
    Tree->Draw(""+Friend+":MVA_S2S9_"+Type+">>h22",(traincut),"ProfileX");
    h22->SetMinimum(0.95); h22->SetLineColor(2);
    TProfile *h23 = new TProfile("h23","Emc/Erec vs S2S9_"+Type+" (profile)",100, 0.58, 0.94, 0.9, 1.1);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S2S9_"+Type+">>h23",(traincut),"ProfileX");
    h22->Draw("colz"); h23->SetMinimum(0.95); h23->Draw("colz same");
    myc1->SaveAs(Output.Data());
    delete h22; delete h23;

    if(!GAMMA_GUN){
	  Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_E1E2.png";
	  gStyle->SetOptStat(0);
	  TProfile *h34 = new TProfile("h34",""+Friend+" vs E1/E2 (profile) red="+Friend+" back=Erec/Emc", 100, 0.9, 5., 0.9, 1.1);
	  Tree->Draw(""+Friend+":MVA_E3x3_1/MVA_E3x3_2>>h34",(traincut),"ProfileX");
	  h34->SetMinimum(0.95); h34->SetLineColor(2);
	  TProfile *h35 = new TProfile("h35","Emc/Erec vs E1/E2 (profile)",100, 0.9, 5., 0.9, 1.1);
	  Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_E3x3_1/MVA_E3x3_2>>h35",(traincut),"ProfileX");
	  h34->Draw("colz"); h35->SetMinimum(0.95); h35->Draw("colz same");
	  myc1->SaveAs(Output.Data());
	  delete h34; delete h35;
    }

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_Pt.png";
    gStyle->SetOptStat(0);
    TProfile *h24;
    if( Type==1 ) h24 = new TProfile("h24",""+Friend+" vs Pt_"+Type+" (profile) red="+Friend+" back=Erec/Emc", 100, 0., 15., 0.9, 1.1);
    else          h24 = new TProfile("h24",""+Friend+" vs Pt_"+Type+" (profile) red="+Friend+" back=Erec/Emc", 100, 0., 6., 0.9, 1.1);
    Tree->Draw(""+Friend+":MVA_Pt_"+Type+">>h24",(traincut),"ProfileX");
    h24->SetMinimum(0.95); h24->SetLineColor(2);
    TProfile *h25;
    if( Type==1 ) h25 = new TProfile("h25","Emc/Erec vs Pt_"+Type+" (profile)",100, 0., 15., 0.9, 1.1);
    else          h25 = new TProfile("h25","Emc/Erec vs Pt_"+Type+" (profile)",100, 0., 6., 0.9, 1.1);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_Pt_"+Type+">>h25",(traincut),"ProfileX");
    h24->Draw("colz"); h25->SetMinimum(0.95); h25->Draw("colz same");
    myc1->SaveAs(Output.Data());
    delete h24; delete h25;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_E.png";
    gStyle->SetOptStat(0);
    TProfile *h30 = new TProfile("h30",""+Friend+" vs E_"+Type+" (profile) red="+Friend+" back=Erec/Emc", 100, 0., 15., 0.9, 1.1);
    Tree->Draw(""+Friend+":MVA_E3x3_"+Type+">>h30",(traincut),"ProfileX");
    h30->SetMinimum(0.95); h30->SetLineColor(2);
    TProfile *h31 = new TProfile("h31","Emc/Erec vs E_"+Type+" (profile)",100, 0., 15., 0.9, 1.1);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_E3x3_"+Type+">>h31",(traincut),"ProfileX");
    h30->Draw("colz"); h31->SetMinimum(0.95); h31->Draw("colz same");
    myc1->SaveAs(Output.Data());
    delete h30; delete h31;

    delete myc1;

}
