//================================================================================================
//
//
//________________________________________________________________________________________________

#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>                  // access to gROOT, entry point to ROOT system
#include <TSystem.h>                // interface to OS
#include <TFile.h>                  // file handle class
#include <TTree.h>                  // class to access ntuples
#include <TBranch.h>                // class to access branches in TTree
#include <TClonesArray.h>           // ROOT array class
#include <iostream>                 // standard I/O

#include "CITCommon/CommonData/interface/ElectronTree.h"

#endif

//*************************************************************************************************
//Fill Lepton Pt Spectrum
//*************************************************************************************************
void DoSplit(string inputFileName, string outputFileName, Int_t Option)
{  


  //*****************************************************************************************
  // Load Input
  //*****************************************************************************************
  citana::ElectronTree eleTree;
  eleTree.LoadTree(inputFileName.c_str(),0);
  eleTree.InitTree();
  cout << "Events in the ntuple: " << eleTree.tree_->GetEntries() << endl;


  //*************************************************************************************************
  //Output tree
  //*************************************************************************************************
  cout << "Output File : " << outputFileName << endl;
  TFile *outputFile = new TFile(outputFileName.c_str(), "RECREATE");
  outputFile->cd();
  TTree *outputTree = eleTree.tree_->CloneTree(0);

  for(UInt_t ientry=0; ientry < eleTree.tree_->GetEntries(); ientry++) {       	
    eleTree.tree_->GetEntry(ientry);
    if (ientry % 100000 == 0) cout << "Electron " << ientry << endl;

    if (Option == 0) {
      if (eleTree.fEventNumber % 2 == 0) {
        outputTree->Fill();
      }
    } 
    if (Option == 1) {
      if (eleTree.fEventNumber % 2 != 0) {
        outputTree->Fill();
      }
    }
  } 
  cout << "Events in output ntuple: " << outputTree->GetEntries() << endl;
  outputFile->Write();
  outputFile->Close();
  delete outputFile;

} 



//*************************************************************************************************
//Main Function
//*************************************************************************************************
void SplitElectronNtuples() {

  DoSplit("/xrootdfs/cms/local/micheli/eleGunNtuples/EB-Pt_5_15.root","/xrootdfs/cms/local/micheli/eleGunNtuples/EB-Pt_5_15.TestingVar.root",0);
  DoSplit("/xrootdfs/cms/local/micheli/eleGunNtuples/EB-Pt_5_15.root","/xrootdfs/cms/local/micheli/eleGunNtuples/EB-Pt_5_15.Testing.root",1);
  /*  DoSplit("/afs/cern.ch/work/s/sixie/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12HZZ4l.root","/afs/cern.ch/work/s/sixie/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12HZZ4l.Training.root",0);
    DoSplit("/afs/cern.ch/work/s/sixie/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12HZZ4l.root","ElectronNtuples.Summer12HZZ4l.Testing.root",1);
  DoSplit("/afs/cern.ch/work/s/sixie/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12HZZ1254l.root","ElectronNtuples.Summer12HZZ1254l.Training.root",0);
  DoSplit("/afs/cern.ch/work/s/sixie/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12HZZ1254l.root","ElectronNtuples.Summer12HZZ1254l.Testing.root",1);
  
  DoSplit("/afs/cern.ch/work/s/sixie/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12ZZ.root","ElectronNtuples.Summer12ZZ.Training.root",0);
  DoSplit("/afs/cern.ch/work/s/sixie/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12ZZ.root","ElectronNtuples.Summer12ZZ.Testing.root",1);
  
  DoSplit("/afs/cern.ch/work/s/sixie/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12DY.root","ElectronNtuples.Summer12DY.Training.root",0);
  DoSplit("/afs/cern.ch/work/s/sixie/public/LeptonScaleAndResolution/Electrons/ElectronNtuples.Summer12DY.root","ElectronNtuples.Summer12DY.Testing.root",1);*/
  

}

void SplitElectronNtuples(string inputFileName, string outputFileName, Int_t Option) {

  DoSplit(inputFileName,outputFileName,Option);
  
}


