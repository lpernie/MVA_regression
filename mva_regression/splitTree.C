#define splitTree_cxx
#include "splitTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "Cintex/Cintex.h"
#include <TString.h>

void splitTree::LoopAndSplit()
{

//   In a ROOT session, you can do:
//      Root > .L splitTree.C
//      Root > splitTree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();

   Long64_t nbytes = 0, nb = 0;

   TFile* outFile_even=TFile::Open("Testing.root","recreate");
   outFile_even->cd();
   TTree* outTree_even=(TTree*)fChain->CloneTree(0);
   outTree_even->SetName("T1");
   outTree_even->SetTitle("T1");
   //   TFile* outFile_odd=TFile::Open("TestingVar.root","recreate");
   //   TTree* outTree_odd=(TTree*)fChain->Clone(treeName);ss



   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      if(eventNumber%2==0){
	outTree_even->Fill();
      }
   }

   outFile_even->cd();
   outTree_even->Write();
   outFile_even->Write();
   outFile_even->Close();
}
