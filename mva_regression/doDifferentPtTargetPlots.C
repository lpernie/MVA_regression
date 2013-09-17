{
  tdrStyle->SetOptStat(1111);
  TFile *fileVariables = TFile::Open("/afs/cern.ch/work/a/atakeda/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12HZZ4l.root");
  TFile *fileTarget_ptSplit = TFile::Open("targets_ZjetsMad_Summer12/new/ElectronEnergyRegression_ptSplit_Testing_V11_nTrees_100.root");
  TFile *fileTarget_noPtSplit = TFile::Open("targets_ZjetsMad_Summer12/new/ElectronEnergyRegression_noPtSplit_Testing_V11_nTrees_100.root");

  TTree* tree=(TTree*)fileVariables->Get("T1");
  tree->AddFriend("targeteb_lowPt_tree",fileTarget_ptSplit);
  tree->AddFriend("targeteb_tree",fileTarget_noPtSplit);
  tree->AddFriend("targetee_lowPt_tree",fileTarget_ptSplit);
  tree->AddFriend("targetee_tree",fileTarget_noPtSplit);

  tree->AddFriend("targetebvar_lowPt_tree",fileTarget_ptSplit);
  tree->AddFriend("targetebvar_tree",fileTarget_noPtSplit);
  tree->AddFriend("targeteevar_lowPt_tree",fileTarget_ptSplit);
  tree->AddFriend("targeteevar_tree",fileTarget_noPtSplit);

  TString option="HZZ";
 
  TCut energycut="(GeneratedEnergyStatus3-GeneratedEnergyStatus1)/GeneratedEnergyStatus3<0.01 && GeneratedEnergyStatus3>=GeneratedEnergyStatus1";
  TCut isEB="abs(eta)<1.479";
  TCut isEE="abs(eta)>1.479 && abs(eta)<2.5";
  TCut ptCut="pt<15";
  TCut mcMatchCut="mcmatch==1";
  TCut fiducialcut="(abs(eta)<1.4442 || abs(eta)>1.556)";

  tree->Draw("(SCRawEnergy*targeteb_lowPt-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hPtSplit_eb(100,-0.5,0.5)",energycut && isEB && ptCut && mcMatchCut);
  tree->Draw("(SCRawEnergy*targeteb-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hnoPtSplit_eb(100,-0.5,0.5)",energycut && isEB && ptCut && mcMatchCut,"sameep");
  hPtSplit_eb->SetLineColor(kRed);
  hPtSplit_eb->SetMarkerColor(kRed);

  TLegend* leg=new TLegend(0.7,0.7,0.8,0.8);
  leg->SetFillColor(0);
  leg->AddEntry(hPtSplit_eb,"Pt Split","l");
  leg->AddEntry(hnoPtSplit_eb,"No Pt Split","l");
  leg->Draw();

  c1->SaveAs("plots/"+option+"DiffEregEtrueOverEtruePt15.png");


  ///Eta ranges plots
  Double_t lowBin=0,upperBin=0.1;
  for(int i=0;i<13;++i){
    if(i!=0){    
      lowBin=upperBin;
      upperBin+=0.1;
    }

  
    TString etaBinCut="abs(eta)>";//+TString("lowBin")+" && abs(eta)<"+TString("upperBin");
    etaBinCut+=lowBin;
    etaBinCut+=" && abs(eta)<";
    etaBinCut+=upperBin;
    //    cout<<etaBinCut;

    stringstream lowEdgeString;
    stringstream upperEdgeString;
    stringstream istring;
    istring<<i;

    TH1F* hPtSplit_eta=new TH1F("hPtSplit_eta","hPtSplit_eta",100,-0.5,0.5);
    TH1F* hnoPtSplit_eta=new TH1F("hnoPtSplit_eta","hnoPtSplit_eta",100,-0.5,0.5);

    tree->Draw("(SCRawEnergy*targeteb_lowPt-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hPtSplit_eta",energycut && isEB && ptCut && mcMatchCut && etaBinCut);
    tree->Draw("(SCRawEnergy*targeteb-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hnoPtSplit_eta",energycut && isEB && ptCut && mcMatchCut && etaBinCut,"sameep");

    //    if(i==0){
    hPtSplit_eta->SetLineColor(kRed);
    hPtSplit_eta->SetMarkerColor(kRed);
    //    }
    leg->Draw();

    //    cout<<"draw ok"<<endl;
    lowEdgeString<<lowBin;
    upperEdgeString<<upperBin;
    TString binningString="range_";
    binningString+=lowEdgeString.str();
    binningString+="_";
    binningString+=upperEdgeString.str();

    c1->SaveAs("plots/"+option+"DiffEregOverEtrueEta"+binningString+".png");



  }


  ///removing the crack
  lowBin=1.3;
  upperBin=1.442;
  TString etaBinCut="abs(eta)>";//+TString("lowBin")+" && abs(eta)<"+TString("upperBin");
  etaBinCut+=lowBin;
  etaBinCut+=" && abs(eta)<";
  etaBinCut+=upperBin;
  //  cout<<etaBinCut;

  stringstream lowEdgeString;
  stringstream upperEdgeString;
  
  TH1F* hPtSplit_eta=new TH1F("hPtSplit_eta","hPtSplit_eta",100,-0.5,0.5);
  TH1F* hnoPtSplit_eta=new TH1F("hnoPtSplit_eta","hnoPtSplit_eta",100,-0.5,0.5);

  
  tree->Draw("(SCRawEnergy*targeteb_lowPt-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hPtSplit_eta",energycut && isEB && ptCut && mcMatchCut && etaBinCut);
  tree->Draw("(SCRawEnergy*targeteb-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hnoPtSplit_eta",energycut && isEB && ptCut && mcMatchCut&& etaBinCut,"sameep");
  
  hPtSplit_eta->SetLineColor(kRed);
  hPtSplit_eta->SetMarkerColor(kRed);
  
  leg->Draw();


    
  lowEdgeString<<lowBin;
  upperEdgeString<<upperBin;
  TString binningString="range_";
  binningString+=lowEdgeString.str();
  binningString+="_";
  binningString+=upperEdgeString.str();
  
  c1->SaveAs("plots/"+option+"DiffEregOverEtrueEta"+binningString+".png");
  

  lowBin=1.566;
  upperBin=1.7;
  TString etaBinCut="abs(eta)>";//+TString("lowBin")+" && abs(eta)<"+TString("upperBin");
  etaBinCut+=lowBin;
  etaBinCut+=" && abs(eta)<";
  etaBinCut+=upperBin;
  //  cout<<etaBinCut;
  
  stringstream lowEdgeString;
  stringstream upperEdgeString;
  
  TH1F* hPtSplit_eta=new TH1F("hPtSplit_eta","hPtSplit_eta",100,-0.5,0.5);
  TH1F* hnoPtSplit_eta=new TH1F("hnoPtSplit_eta","hnoPtSplit_eta",100,-0.5,0.5);

  tree->Draw("(SCRawEnergy*targetee_lowPt-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hPtSplit_eta",energycut && isEE && ptCut && mcMatchCut && etaBinCut);
  tree->Draw("(SCRawEnergy*targetee-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hnoPtSplit_eta",energycut && isEE && ptCut && mcMatchCut&& etaBinCut,"sameep");

  hPtSplit_eta->SetLineColor(kRed);
  hPtSplit_eta->SetMarkerColor(kRed);

  leg->Draw();
  

  
  lowEdgeString<<lowBin;
  upperEdgeString<<upperBin;
  TString binningString="range_";
  binningString+=lowEdgeString.str();
  binningString+="_";
  binningString+=upperEdgeString.str();
  
  c1->SaveAs("plots/"+option+"DiffEregOverEtrueEta"+binningString+".png");
  
  Double_t lowBin=1.7,upperBin=1.8;
  for(int i=0;i<8;++i){
    if(i!=0){    
      lowBin=upperBin;
      upperBin+=0.1;
    }
  TString etaBinCut="abs(eta)>";//+TString("lowBin")+" && abs(eta)<"+TString("upperBin");
  etaBinCut+=lowBin;
  etaBinCut+=" && abs(eta)<";
  etaBinCut+=upperBin;
  //  cout<<etaBinCut;
  
  stringstream lowEdgeString;
  stringstream upperEdgeString;
  
  TH1F* hPtSplit_eta=new TH1F("hPtSplit_eta","hPtSplit_eta",100,-0.5,0.5);
  TH1F* hnoPtSplit_eta=new TH1F("hnoPtSplit_eta","hnoPtSplit_eta",100,-0.5,0.5);

  

  tree->Draw("(SCRawEnergy*targetee_lowPt-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hPtSplit_eta",energycut && isEE && ptCut && mcMatchCut && etaBinCut);
  tree->Draw("(SCRawEnergy*targetee-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hnoPtSplit_eta",energycut && isEE && ptCut && mcMatchCut&& etaBinCut,"sameep");
  
  hPtSplit_eta->SetLineColor(kRed);
  hPtSplit_eta->SetMarkerColor(kRed);
  
  leg->Draw();

  
  lowEdgeString<<lowBin;
  upperEdgeString<<upperBin;
  TString binningString="range_";
  binningString+=lowEdgeString.str();
  binningString+="_";
  binningString+=upperEdgeString.str();
  
  c1->SaveAs("plots/"+option+"DiffEregOverEtrueEta"+binningString+".png");
    
  }

  ////ranges in eb
  lowBin=0;
  upperBin=0.8;
  TString etaBinCut="abs(eta)>";//+TString("lowBin")+" && abs(eta)<"+TString("upperBin");
  etaBinCut+=lowBin;
  etaBinCut+=" && abs(eta)<";
  etaBinCut+=upperBin;
  //  cout<<etaBinCut;
  
  stringstream lowEdgeString;
  stringstream upperEdgeString;
  
  TH1F* hPtSplit_eta=new TH1F("hPtSplit_eta","hPtSplit_eta",100,-0.5,0.5);
  TH1F* hnoPtSplit_eta=new TH1F("hnoPtSplit_eta","hnoPtSplit_eta",100,-0.5,0.5);

  tree->Draw("(SCRawEnergy*targeteb_lowPt-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hPtSplit_eta",energycut && isEB && ptCut && mcMatchCut && etaBinCut);
  tree->Draw("(SCRawEnergy*targeteb-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hnoPtSplit_eta",energycut && isEB && ptCut && mcMatchCut&& etaBinCut,"sameep");

  hPtSplit_eta->SetLineColor(kRed);
  hPtSplit_eta->SetMarkerColor(kRed);

  leg->Draw();
  

  
  lowEdgeString<<lowBin;
  upperEdgeString<<upperBin;
  TString binningString="range_";
  binningString+=lowEdgeString.str();
  binningString+="_";
  binningString+=upperEdgeString.str();
  
  c1->SaveAs("plots/"+option+"DiffEregOverEtrueEta"+binningString+".png");

  ////ranges in eb
  lowBin=0.8;
  upperBin=1.442;
  TString etaBinCut="abs(eta)>";//+TString("lowBin")+" && abs(eta)<"+TString("upperBin");
  etaBinCut+=lowBin;
  etaBinCut+=" && abs(eta)<";
  etaBinCut+=upperBin;
  //  cout<<etaBinCut;
  
  stringstream lowEdgeString;
  stringstream upperEdgeString;
  
  TH1F* hPtSplit_eta=new TH1F("hPtSplit_eta","hPtSplit_eta",100,-0.5,0.5);
  TH1F* hnoPtSplit_eta=new TH1F("hnoPtSplit_eta","hnoPtSplit_eta",100,-0.5,0.5);

  tree->Draw("(SCRawEnergy*targeteb_lowPt-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hPtSplit_eta",energycut && isEB && ptCut && mcMatchCut && etaBinCut);
  tree->Draw("(SCRawEnergy*targeteb-GeneratedEnergyStatus3)/GeneratedEnergyStatus3>>hnoPtSplit_eta",energycut && isEB && ptCut && mcMatchCut&& etaBinCut,"sameep");

  hPtSplit_eta->SetLineColor(kRed);
  hPtSplit_eta->SetMarkerColor(kRed);

  leg->Draw();
  

  
  lowEdgeString<<lowBin;
  upperEdgeString<<upperBin;
  TString binningString="range_";
  binningString+=lowEdgeString.str();
  binningString+="_";
  binningString+=upperEdgeString.str();
  
  c1->SaveAs("plots/"+option+"DiffEregOverEtrueEta"+binningString+".png");




  //  TGraphErrors* graphPtSplit_eta=new TGraphErrors(100);
  //  TGraphErrors* graphnoPtSplit_eta=new TGraphErrors(100);


  tree->Draw("(SCRawEnergy*targeteb_lowPt-GeneratedEnergyStatus3)/GeneratedEnergyStatus3:eta>>graphPtSplit_eta",energycut && isEB && ptCut && mcMatchCut &&fiducialcut,"ProfileX");
  tree->Draw("(SCRawEnergy*targeteb-GeneratedEnergyStatus3)/GeneratedEnergyStatus3:eta>>graphnoPtSplit_eta",energycut && isEB && ptCut && mcMatchCut &&fiducialcut,"ProfileX");

  //  TCanvas *c2=new TCanvas("c2","c2",1);
  //  c2->cd();

  graphPtSplit_eta->GetXaxis()->SetRangeUser(-1.5,1.5);
  graphPtSplit_eta->SetLineColor(kRed);
  graphPtSplit_eta->SetMarkerColor(kRed);
  graphPtSplit_eta->Draw("AP");
  graphnoPtSplit_eta->Draw("sameP");
  leg->Draw();


  c1->SaveAs("plots/"+option+"DiffEregOverEtrueEta_profile.png");

  
  
}
