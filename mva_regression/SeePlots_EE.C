#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>

#include "TFile.h"
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
#include "TPaveStats.h"
#include "TPave.h"

void SeePlots_EE(TString NameWeightTest, TString NameWeight ){

    using namespace std;

    TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);

    // Open Files, Create Output

    bool GAMMA_GUN=false;
    TString Type = "";
    TString Friend = "";
    if     ( NameWeight.Contains("Pi01") ) { Type = "1"; Friend = "target_1"; cout<<"Pi0 gamma 1"<<endl; }
    else if( NameWeight.Contains("Pi02") ) { Type = "2"; Friend = "target_2"; cout<<"Pi0 gamma 2"<<endl; }
    else                                   { Type = "1"; Friend = "target_1"; cout<<"Single Gamma"<<endl; GAMMA_GUN=true;}

    system( (string("mkdir -p ") + NameWeight).Data());

    TString Input = NameWeightTest + ".root";
    TFile* File = TFile::Open( Input.Data() );

    TString mode ="Test";
    TTree* Tree;
    Tree = (TTree*) File->Get("h_EEmva");
    Input = NameWeight + ".root";
    if( Type=="1" ) Tree->AddFriend("target_1_tree",Input.Data());
    if( Type=="2" ) Tree->AddFriend("target_2_tree",Input.Data());

    // Style
    myc1->cd();
    gStyle->SetPalette(1);
    gStyle->SetOptStat(1111);

    //MVA cuts
    TCut traincut;
    //if( !GAMMA_GUN && (Type == "1" || Type == "2") ) traincut = "(MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && (MVA_E3x3MC_2/MVA_E3x3_2)<3.0 && MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && MVA_E3x3_2>0.01 && MVA_Nxtal_1>6 && MVA_Nxtal_2>4 && MVA_E3x3MC_2>0.01 && MVA_Pt_1>0.5 && MVA_Pt_2>0.5 && MVA_S4S9_1>0.8 && MVA_S4S9_2>0.8 && MVA_EPi0>0.8";
    if( !GAMMA_GUN && (Type == "1" || Type == "2") ) traincut = "TMath::Abs(MVA_nEta_1)>1.656 && TMath::Abs(MVA_nEta_1)<2.59 && (MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && (MVA_E3x3MC_2/MVA_E3x3_2)<3.0 && MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01  && MVA_E3x3_2>0.01 && MVA_Nxtal_1>6 && MVA_Nxtal_2>5 && MVA_E3x3MC_2>0.01 && MVA_Pt_1>0.5 && MVA_Pt_2>0.5 && MVA_S4S9_1>0.8 && MVA_S4S9_2>0.8 && MVA_EPi0>1.5";
    if( GAMMA_GUN )                                  traincut = "MVA_E3x3_1>0.01 && MVA_E3x3MC_1>0.01 && (MVA_E3x3MC_1/MVA_E3x3_1)<3.0 && MVA_Pt_1>0.55 && MVA_Nxtal_1>5 && MVA_S4S9_1>0.75";

    //Plots
    TString Output = NameWeight + "/" + mode + "_Risol_RedNoweight_BlackWeight.png";
    TH1F *h1 = new TH1F("h1","Risol [red no weight]",100,-1,1);
    Tree->Draw("(((MVA_E3x3_"+Type+"*"+Friend+")-MVA_E3x3MC_"+Type+")/MVA_E3x3MC_"+Type+")>>h1","((MVA_E3x3_"+Type+"*"+Friend+")-MVA_E3x3MC_"+Type+")/MVA_E3x3MC_"+Type+"<1 && "+ string(traincut) );
    TH1F *h2 = new TH1F("h2","Risol 2 [red no weight]",100,-1,1);
    Tree->Draw("((MVA_E3x3_"+Type+"-MVA_E3x3MC_"+Type+")/MVA_E3x3MC_"+Type+")>>h2","(MVA_E3x3_"+Type+"-MVA_E3x3MC_"+Type+")/MVA_E3x3MC_"+Type+"<1 && "+ string(traincut));
    h1->Draw();
    h1->SetMaximum( h2->GetMaximum() );
    h1->Draw(); h2->SetLineColor(2); h2->Draw("SAMES");
    myc1->SaveAs(Output.Data());
    delete h1; delete h2;

    //TString Output = NameWeight + "/" + mode + "_Risol_RedNoweight_BlackWeight.png";
/*
    TH1F *h1 = new TH1F("h1","Risol [red no weight]",100,0,2);
    Tree->Draw("((MVA_E3x3MC_"+Type+")/(MVA_E3x3_"+Type+")/"+Friend+")>>h1","(MVA_E3x3MC_"+Type+")/MVA_E3x3_"+Type+"<3 && "+ string(traincut) );
    TH1F *h2 = new TH1F("h2","Risol 2 [red no weight]",100,0,2);
    Tree->Draw("((MVA_E3x3MC_"+Type+")/MVA_E3x3_"+Type+")>>h2","(MVA_E3x3MC_"+Type+")/MVA_E3x3_"+Type+"<3 && "+ string(traincut));
    h1->Draw();
    TLine *ax = new TLine(1.,0.,1.,h2->GetMaximum());
    h1->Draw(); h2->SetLineColor(2); h2->Draw("SAMES"); ax->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h1; delete h2;

    for(int j=0; j<2; j++){

	  TString num = "";
	  if( j==0 ) num = "1";
	  else       num = "2";

	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_EtaRing_"+ num +".png";
	  gStyle->SetOptStat(0);
	  TH2F *h36;
	  h36 = new TH2F("h36",""+Friend+" vs EtaRing_"+num+"",30, 5.5, 35.5, 100, 0.95, 1.7);
	  Tree->Draw(""+Friend+":EtaRing_"+num+">>h36",(traincut),"colz");
	  h36->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h36;

	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_MVA_nEta_"+ num +".png";
	  TH2F *h3;
	  if( Type==1 ) h3 = new TH2F("h3",""+Friend+" vs MVA_nEta_"+num+"",50,1.6,2.6,100,0.95,1.7);
	  else          h3 = new TH2F("h3",""+Friend+" vs MVA_nEta_"+num+"",50,1.6,2.6,150,0.95,1.7);
	  gStyle->SetOptStat(0);
	  Tree->Draw(""+Friend+":TMath::Abs(MVA_nEta_"+num+")>>h3",traincut,"colz");
	  h3->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h3;


	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_ES1_"+ num +".png";
	  gStyle->SetOptStat(0);
	  TH2F *h5;
	  h5 = new TH2F("h5",""+Friend+" vs nES1_"+num+"",50, 0., 1.15, 100, 0.95, 1.7);
	  Tree->Draw(""+Friend+":MVA_ES1_"+num+">>h5",(traincut),"colz");
	  h5->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h5;

	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_ES2_"+ num +".png";
	  TH2F *h5a;
	  h5a = new TH2F("h5a",""+Friend+" vs nES2_"+num+"",50, 0., 2., 100, 0.95, 1.7);
	  Tree->Draw(""+Friend+":MVA_ES2_"+num+">>h5a",(traincut),"colz");
	  h5a->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h5a;

	  //Output = NameWeight + "/" + mode + "_"+Friend+"_2D_dR_"+ num +".png";
	  //TH2F *h5a;
	  //if( Type==1 ) h5a = new TH2F("h5a",""+Friend+" vs dR_"+num+"",30, 20.5, 50.5, 100, 0.95, 1.7);
	  //else          h5a = new TH2F("h5a",""+Friend+" vs dR_"+num+"",30, 20.5, 50.5, 150, 0.95, 1.7);
	  //Tree->Draw(""+Friend+":sqrt(pow(MVA_iX_"+num+",2)+pow(MVA_iY_"+num+",2))>>h5a",(traincut),"colz");
	  //h5a->Draw("colz");
	  //myc1->SaveAs(Output.Data());
	  //delete h5a;


	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_Nvtx_"+ num +".png";
	  Tree->Draw(""+Friend+":MVA_Nxtal_"+num+"", (traincut),"colz");
	  myc1->SaveAs(Output.Data());

	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_Pt_"+ num +".png";
	  TH2F *h26;
	  if( Type==1 ) h26 = new TH2F("h26",""+Friend+" vs Pt_"+Type+"", 100, 1.3, 15., 100, 0.95, 1.7);
	  else          h26 = new TH2F("h26",""+Friend+" vs Pt_"+Type+"", 100, 1.3, 6., 100, 0.95, 1.7);
	  gStyle->SetOptStat(0);
	  Tree->Draw(""+Friend+":MVA_Pt_"+num+">>h26", (traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h26;

	  if(GAMMA_GUN) j=2;
    }

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_E.png";
    TH2F *h27 = new TH2F("h27",""+Friend+" vs E_"+Type+"", 100, 3.7, 15., 100, 0.95, 1.7);
    gStyle->SetOptStat(0);
    Tree->Draw(""+Friend+":MVA_E3x3_"+Type+">>h27",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h27;

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_S4S9.png";
    TH2F *h10 = new TH2F("h10",""+Friend+" vs S4S9_"+Type+"", 100, 0.7, 1.01, 100, 0.95, 1.7);
    gStyle->SetOptStat(0);
    Tree->Draw(""+Friend+":MVA_S4S9_"+Type+">>h10",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h10;

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_S2S9.png";
    TH2F *h11 = new TH2F("h11",""+Friend+" vs S2S9_"+Type+"", 100, 0.75, 0.94, 100, 0.95, 1.7);
    gStyle->SetOptStat(0);
    Tree->Draw(""+Friend+":MVA_S2S9_"+Type+">>h11",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h11;

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_S1S9.png";
    TH2F *h12 = new TH2F("h12",""+Friend+" vs S1S9_"+Type+"", 100, 0.7, 0.88, 100, 0.95, 1.7);
    gStyle->SetOptStat(0);
    Tree->Draw(""+Friend+":MVA_S1S9_"+Type+">>h12",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h12;

    if(!GAMMA_GUN){
	  Output = NameWeight + "/" + mode + "_"+Friend+"_2D_E1E2.png";
	  TH2F *h32 = new TH2F("h32",""+Friend+" vs E1E2", 100, 0.8, 3.2, 100, 0.85, 1.7);
	  gStyle->SetOptStat(0);
	  Tree->Draw(""+Friend+":(MVA_E3x3_1/MVA_E3x3_2)>>h32",(traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h32;
    }

    Output = NameWeight + "/" + mode + "_"+Friend+"_2D_RatioEnergies.png";
    gStyle->SetOptStat(0);
    TH2F *h7 = new TH2F("h7",""+Friend+" vs Emc/Erec_"+Type+"",100, 0.8, 1.2, 100,0.95,1.5);
    Tree->Draw(""+Friend+":(MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+")>>h7","(MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+")>0.5 && (MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+")<1.5 && "+ string(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h7;

    //Now vs Emc/Erec

    for(int j=0; j<2; j++){
	  TString num = "";
	  if( j==0 ) num = "1";
	  else       num = "2";

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_EtaRing_"+ num +".png";
	  TH2F *h38 = new TH2F("h38","Emc/Erec vs EtaRing_"+num+"", 30, 5.5, 35.5,100,0.85,1.7);
	  gStyle->SetOptStat(0);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":EtaRing_"+num+">>h38",(traincut),"colz");
	  h38->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h38;

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_nEta_"+ num +".png";
	  TH2F *h8 = new TH2F("h8","Emc/Erec vs nEta_"+num+"", 50, 1.6, 2.6,100,0.85,1.7);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":TMath::Abs(MVA_nEta_"+num+")>>h8",(traincut),"colz");
	  h8->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h8;

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_ES1_"+ num +".png";
	  TH2F *h9 = new TH2F("h9","Emc/Erec vs ES1_"+num+"", 50, 0., 2., 100, 0.85, 1.7);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_ES1_"+num+">>h9",(traincut),"colz");
	  h9->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h9;

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_ES2_"+ num +".png";
	  TH2F *h9a = new TH2F("h9a","Emc/Erec vs ES2_"+num+"", 50, 0., 2., 100, 0.85, 1.7);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_ES2_"+num+">>h9a",(traincut),"colz");
	  h9a->Draw("colz");
	  myc1->SaveAs(Output.Data());
	  delete h9a;

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_Nvtx_"+ num +".png";
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_Nxtal_"+num+"",(traincut),"colz");
	  myc1->SaveAs(Output.Data());

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_Pt_"+ num +".png";
	  TH2F *h28 ;
	  if( Type==1 ) h28 = new TH2F("h28","Emc/Erec vs Pt_"+num+"", 100, 1.4, 15., 100, 0.85, 1.7);
	  else          h28 = new TH2F("h28","Emc/Erec vs Pt_"+num+"", 100, 1.4, 6., 100, 0.85, 1.7);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_Pt_"+num+">>h28",(traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h28;

	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_E_"+ num +".png";
	  TH2F *h29 = new TH2F("h29","Emc/Erec vs E_"+num+"", 100, 0., 15., 100, 0.85, 1.7);
	  Tree->Draw("MVA_E3x3MC_"+num+"/MVA_E3x3_"+num+":MVA_E3x3_"+num+">>h29",(traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h29;

	  if(GAMMA_GUN) j=2;
    }

    Output = NameWeight + "/" + mode + "_ErecEmc_2D_S4S9.png";
    TH2F *h13 = new TH2F("h13","Emc/Erec vs S4S9_"+Type+"", 100, 0.7, 1.01, 100, 0.85, 1.7);
    gStyle->SetOptStat(0);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S4S9_"+Type+">>h13",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h13;

    Output = NameWeight + "/" + mode + "_ErecEmc_2D_S2S9.png";
    TH2F *h14 = new TH2F("h14","Emc/Erecvs S2S9_"+Type+"", 100, 0.75, 0.94, 100, 0.85, 1.7);
    gStyle->SetOptStat(0);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S2S9_"+Type+">>h14",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h14;

    Output = NameWeight + "/" + mode + "_ErecEmc_2D_S1S9.png";
    TH2F *h15 = new TH2F("h15","Emc/Erec vs S1S9_"+Type+"", 100, 0.7, 0.88, 100, 0.85, 1.7);
    gStyle->SetOptStat(0);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S1S9_"+Type+">>h15",(traincut),"colz");
    myc1->SaveAs(Output.Data());
    delete h15;

    if(!GAMMA_GUN){
	  Output = NameWeight + "/" + mode + "_ErecEmc_2D_E1E2.png";
	  TH2F *h33 = new TH2F("h33","Emc/Erec vs E1E2_", 100, 0.9, 5.0, 100, 0.85, 1.7);
	  gStyle->SetOptStat(0);
	  Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_E3x3_1/MVA_E3x3_2>>h33",(traincut),"colz");
	  myc1->SaveAs(Output.Data());
	  delete h33;
    }
    //Profile Comparison
*/
    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_EtaRing.png";
    gStyle->SetOptStat(0);
    TProfile *h37 = new TProfile("h37",""+Friend+" vs EtaRing_"+Type+" (profile) red="+Friend+" black=Erec/Emc",30,5.5,35.5, 0., 10.);
    Tree->Draw(""+Friend+":EtaRing_"+Type+">>h37",(traincut),"ProfileX");
    h37->SetLineColor(2); h37->SetMinimum(0.9); h37->SetMaximum(1.3);
    TProfile *h39 = new TProfile("h39","Emc/Erec vs EtaRing_"+Type+" (profile)", 30, 5.5, 35.5, 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":EtaRing_"+Type+">>h39",(traincut),"ProfileX");
    h37->Draw(); h39->SetMinimum(0.9); h39->SetMaximum(1.3); h39->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h37; delete h39;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_nEta.png";
    gStyle->SetOptStat(0);
    TProfile *h4 = new TProfile("h4",""+Friend+" vs nEta_"+Type+" (profile) red="+Friend+" black=Erec/Emc",50,1.6,2.6, 0., 10.);
    Tree->Draw(""+Friend+":TMath::Abs(MVA_nEta_"+Type+")>>h4",(traincut),"ProfileX");
    h4->SetLineColor(2);
    TProfile *h16 = new TProfile("h16","Emc/Erec vs nEta_"+Type+" (profile)", 50, 1.6, 2.6, 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":TMath::Abs(MVA_nEta_"+Type+")>>h16",(traincut),"ProfileX");
    h4->Draw();  h16->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h4; delete h16;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_ES1.png";
    TProfile *h6 = new TProfile("h6",""+Friend+" vs ES1_"+Type+" (profile) red="+Friend+" black=Erec/Emc",50,0., 2., 0., 10.);
    Tree->Draw(""+Friend+":MVA_ES1_"+Type+">>h6",(traincut),"ProfileX");
    h6->SetLineColor(2);
    TProfile *h17 = new TProfile("h17","Emc/Erec vs ES1_"+Type+" (profile)",50,0., 2., 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_ES1_"+Type+">>h17",(traincut),"ProfileX");
    h6->Draw();  h17->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h6; delete h17;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_ES2.png";
    TProfile *h6a = new TProfile("h6a",""+Friend+" vs ES2_"+Type+" (profile) red="+Friend+" black=Erec/Emc",50,0., 2., 0., 10.);
    Tree->Draw(""+Friend+":MVA_ES2_"+Type+">>h6a",(traincut),"ProfileX");
    h6a->SetLineColor(2);
    TProfile *h17a = new TProfile("h17a","Emc/Erec vs ES2_"+Type+" (profile)",50,0., 2., 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_ES2_"+Type+">>h17a",(traincut),"ProfileX");
    h6a->Draw(); h17a->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h6a; delete h17a;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_dR.png";
    gStyle->SetOptStat(0);
    TProfile *h6b = new TProfile("h6b",""+Friend+" vs idR_"+Type+" (profile) red="+Friend+" black=Erec/Emc",30,20.5, 50.5, 0.95, 1.7);
    Tree->Draw(""+Friend+":sqrt(pow(MVA_iX_"+Type+",2)+pow(MVA_iY_"+Type+",2))>>h6b",(traincut),"ProfileX");
    h6b->SetMinimum(0.95); h6b->SetLineColor(2);
    TProfile *h17b = new TProfile("h17b","Emc/Erec vs dR_"+Type+" (profile)", 30,20.5, 50.5, 0.95, 1.7);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":sqrt(pow(MVA_iX_"+Type+",2)+pow(MVA_iY_"+Type+",2))>>h17b",(traincut),"ProfileX");
    h6b->Draw(); h17b->SetMinimum(0.95); h17b->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h6b; delete h17b;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_S4S9.png";
    gStyle->SetOptStat(0);
    TProfile *h18 = new TProfile("h18",""+Friend+" vs S4S9_"+Type+" (profile) red="+Friend+" black=Erec/Emc", 100, 0.7, 1.1, 0., 10.);
    Tree->Draw(""+Friend+":MVA_S1S9_"+Type+">>h18",(traincut),"ProfileX");
    h18->SetLineColor(2);
    TProfile *h19 = new TProfile("h19","Emc/Erec vs S4S9_"+Type+" (profile)",100, 0.75, 1.1, 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S1S9_"+Type+">>h19",(traincut),"ProfileX");
    h18->Draw(); h19->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h18; delete h19;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_S1S9.png";
    gStyle->SetOptStat(0);
    TProfile *h20 = new TProfile("h20",""+Friend+" vs S1S9_"+Type+" (profile) red="+Friend+" black=Erec/Emc", 100, 0.58, 0.94, 0., 10.);
    Tree->Draw(""+Friend+":MVA_S1S9_"+Type+">>h20",(traincut),"ProfileX");
    h20->SetLineColor(2);
    TProfile *h21 = new TProfile("h21","Emc/Erec vs S1S9_"+Type+" (profile)",100, 0.58, 0.94, 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S1S9_"+Type+">>h21",(traincut),"ProfileX");
    h20->Draw(); h21->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h20; delete h21;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_S2S9.png";
    gStyle->SetOptStat(0);
    TProfile *h22 = new TProfile("h22",""+Friend+" vs S2S9_"+Type+" (profile) red="+Friend+" black=Erec/Emc", 100, 0.58, 0.94, 0., 10.);
    Tree->Draw(""+Friend+":MVA_S2S9_"+Type+">>h22",(traincut),"ProfileX");
    h22->SetLineColor(2);
    TProfile *h23 = new TProfile("h23","Emc/Erec vs S2S9_"+Type+" (profile)",100, 0.58, 0.94, 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_S2S9_"+Type+">>h23",(traincut),"ProfileX");
    h22->Draw(); h23->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h22; delete h23;

    if(!GAMMA_GUN){
	  Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_E1E2.png";
	  gStyle->SetOptStat(0);
	  TProfile *h34 = new TProfile("h34",""+Friend+" vs E1/E2 (profile) red="+Friend+" black=Erec/Emc", 100, 0.9, 5., 0., 10.);
	  Tree->Draw(""+Friend+":MVA_E3x3_1/MVA_E3x3_2>>h34",(traincut),"ProfileX");
	  h34->SetLineColor(2);
	  TProfile *h35 = new TProfile("h35","Emc/Erec vs E1/E2 (profile)",100, 0.9, 5., 0., 10.);
	  Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_E3x3_1/MVA_E3x3_2>>h35",(traincut),"ProfileX");
	  h34->Draw(); h35->Draw("same");
	  myc1->SaveAs(Output.Data());
	  delete h34; delete h35;
    }

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_Pt.png";
    gStyle->SetOptStat(0);
    TProfile *h24 = new TProfile("h24",""+Friend+" vs Pt_"+Type+" (profile) red="+Friend+" black=Erec/Emc", 50, 1.3, 15., 0., 10.);
    //TProfile *h24;
    //if( Type==1 ) h24 = new TProfile("h24",""+Friend+" vs Pt_"+Type+" (profile) red="+Friend+" black=Erec/Emc", 50, 1.3, 15., 0., 10.);
    //else          h24 = new TProfile("h24",""+Friend+" vs Pt_"+Type+" (profile) red="+Friend+" black=Erec/Emc", 50, 1.3, 6., 0., 10.);
    Tree->Draw(""+Friend+":MVA_Pt_"+Type+">>h24",(traincut),"ProfileX");
    h24->SetLineColor(2);
    TProfile *h25 = new TProfile("h25","Emc/Erec vs Pt_"+Type+" (profile)",50, 1.3, 15., 0., 10.);
    //TProfile *h25;
    //if( Type==1 ) h25 = new TProfile("h25","Emc/Erec vs Pt_"+Type+" (profile)",50, 1.3, 15., 0., 10.);
    //else          h25 = new TProfile("h25","Emc/Erec vs Pt_"+Type+" (profile)",50, 1.3, 6., 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_Pt_"+Type+">>h25",(traincut),"ProfileX");
    h24->Draw(); h25->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h24; delete h25;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_E.png";
    gStyle->SetOptStat(0);
    TProfile *h30 = new TProfile("h30",""+Friend+" vs E_"+Type+" (profile) red="+Friend+" black=Erec/Emc", 100, 0., 15., 0., 10.);
    Tree->Draw(""+Friend+":MVA_E3x3_"+Type+">>h30",(traincut),"ProfileX");
    h30->SetLineColor(2);
    TProfile *h31 = new TProfile("h31","Emc/Erec vs E_"+Type+" (profile)",100, 0., 15., 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":MVA_E3x3_"+Type+">>h31",(traincut),"ProfileX");
    h30->Draw(); h31->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h30; delete h31;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_EOverES.png";
    gStyle->SetOptStat(0);
    TProfile *h30a = new TProfile("h30a",""+Friend+" vs E_"+Type+" /ES (profile) red="+Friend+" black=Erec/Emc", 100, 0., 0.45, 0., 10.);
    Tree->Draw(""+Friend+":(MVA_ES1_"+Type+"+MVA_ES2_"+Type+")/MVA_E3x3_"+Type+">>h30a",(traincut),"ProfileX");
    h30a->SetLineColor(2);
    TProfile *h31a = new TProfile("h31a","Emc/Erec vs E_"+Type+" /ES (profile)",100, 0., 0.45, 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":(MVA_ES1_"+Type+"+MVA_ES2_"+Type+")/MVA_E3x3_"+Type+">>h31a",(traincut),"ProfileX");
    h30a->Draw(); h31a->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h30a; delete h31a;

    Output = NameWeight + "/" + mode + "_"+Friend+"RedERATIOblack_Profile_ESsum.png";
    gStyle->SetOptStat(0);
    TProfile *h30b = new TProfile("h30b",""+Friend+" vs E_"+Type+" /ES (profile) red="+Friend+" black=Erec/Emc", 100, 0., 0.45, 0., 10.);
    Tree->Draw(""+Friend+":(MVA_ES1_"+Type+"+MVA_ES2_"+Type+")>>h30b",(traincut),"ProfileX");
    h30b->SetLineColor(2);
    TProfile *h31b = new TProfile("h31b","E ES sum",100, 0., 0.45, 0., 10.);
    Tree->Draw("MVA_E3x3MC_"+Type+"/MVA_E3x3_"+Type+":(MVA_ES1_"+Type+"+MVA_ES2_"+Type+")>>h31b",(traincut),"ProfileX");
    h30b->Draw(); h31b->Draw("same");
    myc1->SaveAs(Output.Data());
    delete h30b; delete h31b;

    delete myc1;

}
