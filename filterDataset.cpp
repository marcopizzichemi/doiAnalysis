//------------------------------------------------------------//
//                                                            //
//  PROGRAM FOR PREPARING DOI TEST                            //
//                                                            //
//------------------------------------------------------------//

// compile with 
// g++ -o filterDataset filterDataset.cpp `root-config --cflags --glibs`

#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TTree.h"
#include "TFile.h"
#include "TF2.h"
#include "TSpectrum.h"
#include "TSpectrum2.h"
#include "TTreeFormula.h"
#include "TMath.h"
#include "TChain.h"
#include "TCut.h"
#include "TLine.h"
#include "TError.h"
#include "TEllipse.h"
#include "TFormula.h"
#include "TGraphErrors.h"


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
#include <cmath> 


int main(int argc, char** argv)
{
  
  std::string inputFileName = argv[1];
  long long int nevent = atoi(argv[2]);
  //input TTree
  ULong64_t     InTreeExtendedTimeTag;
  ULong64_t     InTreeDeltaTimeTag;
  Short_t      *InTreeAdcChannel;
  int           InTreeTriggerChannel;
  Short_t       InTreeTagging;
  Float_t       InTreeFloodX;
  Float_t       InTreeFloodY;
  Float_t       InTreeFloodZ;
  Float_t       InTreeTheta;
  Float_t       InTreePhi;
  Bool_t        InTreeBadevent;
  Float_t       InTreeZPosition;
  
  TBranch      *b_ExtendedTimeTag;
  TBranch      *b_DeltaTimeTag;
  TBranch     **b_AdcChannel;
  TBranch      *b_TriggerChannel;
  TBranch      *b_Tagging;
  TBranch      *b_floodx;
  TBranch      *b_floody;
  TBranch      *b_floodz;
  TBranch      *b_Theta;
  TBranch      *b_Phi;
  TBranch      *b_Badevent;
  TBranch      *b_ZPosition;
  
  
  //output Tree
  ULong64_t     OutTreeExtendedTimeTag;
  ULong64_t     OutTreeDeltaTimeTag;
  Short_t      *OutTreeAdcChannel;
  int           OutTreeTriggerChannel;
  Short_t       OutTreeTagging;
  Float_t       OutTreeFloodX;
  Float_t       OutTreeFloodY;
  Float_t       OutTreeFloodZ;
  Float_t       OutTreeTheta;
  Float_t       OutTreePhi;
  Bool_t        OutTreeBadevent;
  Float_t       OutTreeZPosition;
  
  int inputChannels = 16;
  
  OutTreeAdcChannel    = new Short_t  [inputChannels]; 
  
  TFile* fTreeOut = new TFile("tempFilter.root","recreate");
  TTree *outTree = new TTree("adc","adc");
  //set branches also for the output ttree
  outTree->Branch("ExtendedTimeTag",&OutTreeExtendedTimeTag,"ExtendedTimeTag/l"); 
  outTree->Branch("DeltaTimeTag",&OutTreeDeltaTimeTag,"DeltaTimeTag/l");
  //branches of the 32 channels data
  for (int i = 0 ; i < 16 ; i++)
  {
    //empty the stringstreams
    std::stringstream sname,stype;
    sname << "ch" << i;
    stype << "ch" << i << "/S";  
    outTree->Branch(sname.str().c_str(),&OutTreeAdcChannel[i],stype.str().c_str());
  }
  outTree->Branch("Tagging",&OutTreeTagging,"Tagging/S");
  outTree->Branch("TriggerChannel",&OutTreeTriggerChannel,"TriggerChannel/I"); 
  outTree->Branch("FloodX",&OutTreeFloodX,"FloodX/F"); 
  outTree->Branch("FloodY",&OutTreeFloodY,"FloodY/F"); 
  outTree->Branch("FloodZ",&OutTreeFloodZ,"FloodZ/F");
  outTree->Branch("ZPosition",&OutTreeZPosition,"TreeZPosition/F");
  outTree->Branch("Theta",&OutTreeTheta,"Theta/F"); 
  outTree->Branch("Phi",&OutTreePhi,"Phi/F");
  outTree->Branch("BadEvent",&OutTreeBadevent,"BadEvent/O"); 
  
  
  //input file
  TFile *fFileIn = new TFile(inputFileName.c_str());
  fFileIn->cd();
  TTree *intree;
  fFileIn->GetObject("adc",intree);

  InTreeAdcChannel     = new Short_t  [inputChannels]; 
  b_AdcChannel         = new TBranch* [inputChannels];
  
  
  intree->SetBranchAddress("ExtendedTimeTag", &InTreeExtendedTimeTag, &b_ExtendedTimeTag);
  intree->SetBranchAddress("DeltaTimeTag", &InTreeDeltaTimeTag, &b_DeltaTimeTag);
  for(int i=0; i<16; i++)
  {
    std::stringstream sname;
    sname << "ch" << i;
    intree->SetBranchAddress(sname.str().c_str(), &InTreeAdcChannel[i], &b_AdcChannel[i]);
  }
  intree->SetBranchAddress("Tagging", &InTreeTagging, &b_Tagging);
  intree->SetBranchAddress("TriggerChannel", &InTreeTriggerChannel, &b_TriggerChannel);
  intree->SetBranchAddress("FloodX", &InTreeFloodX, &b_floodx);
  intree->SetBranchAddress("FloodY", &InTreeFloodY, &b_floody);
  intree->SetBranchAddress("FloodZ", &InTreeFloodZ, &b_floodz);
  intree->SetBranchAddress("ZPosition", &InTreeZPosition, &b_ZPosition);
  intree->SetBranchAddress("Theta", &InTreeTheta, &b_Theta);
  intree->SetBranchAddress("Phi", &InTreePhi, &b_Phi);
  intree->SetBranchAddress("BadEvent", &InTreeBadevent, &b_Badevent);
  

  
  
//   int nevent = 100000;
  
  for (Int_t i=0;i<nevent;i++) 
  { 
    //loop on all the entries of tchain
    intree->GetEvent(i);
    
    OutTreeExtendedTimeTag = InTreeExtendedTimeTag;
    OutTreeDeltaTimeTag = InTreeDeltaTimeTag;
    
    for (int j = 0 ; j < inputChannels ; j++) //loop on input chain channels
    {
      OutTreeAdcChannel[j] = InTreeAdcChannel [j];
    }
    
    OutTreeTriggerChannel =  InTreeTriggerChannel;
    OutTreeTagging        =  InTreeTagging;
    OutTreeFloodX         =  InTreeFloodX;
    OutTreeFloodY         =  InTreeFloodY;
    OutTreeFloodZ         =  InTreeFloodZ;
    OutTreeTheta          =  InTreeTheta;
    OutTreePhi            =  InTreePhi;
    OutTreeBadevent       =  InTreeBadevent;
    OutTreeZPosition      =  InTreeZPosition;
    
    outTree->Fill();
    
  }
  
  
  
  
  
  fTreeOut->cd();
  outTree->Write();
  fTreeOut->Close();
  
  /*
  chain->Add("./z0/Run_2015_02_06_14_37_34/temp.root",1000000);
  long long int nentries = chain->GetEntries();
  cout << nentries << endl;
  cout <<(int) nentries * 0.792 << endl;
  cout <<(int) nentries * 0.627 << endl;
  cout <<(int) nentries * 0.497 << endl;
  cout <<(int) nentries * 0.393 << endl;
  cout << endl;
  cout << nentries << endl;
  chain->Add("./z1/Run_2015_03_06_07_48_56/temp.root",(int) nentries * 0.792);
  cout << chain->GetEntries() << endl;
  chain->Add("./z2/Run_2015_03_06_13_58_04/temp.root",(int) nentries * 0.627);
  cout << chain->GetEntries() << endl;
  chain->Add("./z3/Run_2015_04_06_08_04_11/temp.root",(int) nentries * 0.497);
  cout << chain->GetEntries() << endl;
  chain->Add("./z4/Run_2015_04_06_12_02_55/temp.root",(int) nentries * 0.393);
  cout << chain->GetEntries() << endl;*/
  
  return 0;
}