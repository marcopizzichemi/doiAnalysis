//------------------------------------------------------------//
//                                                            //
//  PROGRAM FOR "DOI tests" USING DT5740                      //
//                                                            //
//------------------------------------------------------------//

// compile with 
// g++ -o ../build/doiAnalysis doiAnalysis.cpp `root-config --cflags --glibs` -lSpectrum -lMLP -lTreePlayer

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


#include "./inc/Input.h"
#include "./inc/Params.h"
#include "./inc/ParseConfigFile.h"
#include "./inc/PrintConfigFile.h"
#include "./inc/FillInput.h"
#include "./inc/MakeTChain.h"
#include "./inc/Make2Dhisto.h"
#include "./inc/Crystal.h"
#include "./inc/FillCrystal.h"
#include "./inc/Find2Dpeaks.h"
#include "./inc/singleMPPCplot.h"
#include "./inc/allMPPCplot.h"
#include "./inc/ratioPlot.h"

int main (int argc, char** argv)
{
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(1);
  gErrorIgnoreLevel = kError;
  //input parameters
  Params_t Params;
  //data structure
  Input_t Input[16];
  //crystal structure
  Crystal_t Crystal[64];
  
  int parallelNum ;
  
  //read the config file
  std::string configFileName;
  if(argc < 2)
    configFileName = "doi_analysis_config.txt";
  else
    configFileName = argv[1];
  if(argc < 3)
    parallelNum = -1;
  else
    parallelNum = atoi(argv[2]);
  
  std::fstream configFile(configFileName.c_str());
  parseConfigFile(configFile,Params);
  
  //print the config file data
  printConfigFile(Params);
  
  //fill data structure
  fillInput(Input,Params);
  
  fillCrystal(Crystal,Input,Params);
  
  //make the tchain
  TChain *chain = new TChain("adc");;
  makeTChain(Params,chain);
  
  std::stringstream tempStream;
  
  
  
  //make the first "uncut" flood histo
  TH2F *UncutFlood; 
  TCanvas *UncutFloodCanvas = new TCanvas("UncutFloodCanvas","",800,800);
  UncutFlood = make2Dhisto(Params,chain,"Uncut Weighted Sum","FloodY:FloodX","","COLZ");
  UncutFlood->GetXaxis()->SetTitle("X [mm]");
  UncutFlood->GetYaxis()->SetTitle("Y [mm]");
  UncutFlood->SetTitle("Weighted Average - No Cut");
  
  //spectrum of the tagging crystal
  TH1F *TriggerSpectrum = new TH1F("TriggerSpectrum","",1200,0,12000);
  tempStream.str("");
  tempStream << "Tagging >> TriggerSpectrum";
  TCanvas *TriggerSpectrumCanvas = new TCanvas("TriggerSpectrumCanvas","",800,600);
  TriggerSpectrum->GetXaxis()->SetTitle("ADC");
  TriggerSpectrum->GetYaxis()->SetTitle("Counts");
  TriggerSpectrum->SetTitle("Spectrum of Tagging Crystal");
  chain->Draw(tempStream.str().c_str());
  
  //restrict the region where to look for peaks. Fix for tspectrum...
  TriggerSpectrum->GetXaxis()->SetRangeUser(5000,9000);
  
  //find peak in the tagging crystal
  TSpectrum *sTagCrystal;
  sTagCrystal = new TSpectrum(1);
  Int_t TagCrystalPeaksN = sTagCrystal->Search(TriggerSpectrum,1,"",0.5); //TODO pass to "goff"
  Float_t *TagCrystalPeaks = sTagCrystal->GetPositionX();
  //Float_t *TagCrystalPeaksY = sTagCrystal->GetPositionY();
  TF1 *gauss = new TF1("gauss", "gaus");
  TriggerSpectrum->Fit("gauss","NQ","",TagCrystalPeaks[0] - 0.075*TagCrystalPeaks[0],TagCrystalPeaks[0] + 0.075*TagCrystalPeaks[0]);
  TriggerSpectrum->GetXaxis()->SetRangeUser(0,12000);
  //define a TCut for this peak
  Params.tagPhotopeakMin = gauss->GetParameter(1) - 1.5*gauss->GetParameter(2);
  Params.tagPhotopeakMax = gauss->GetParameter(1) + 2.0*gauss->GetParameter(2);
  tempStream.str("");
  tempStream << "Tagging > " << Params.tagPhotopeakMin << "&& Tagging < " << Params.tagPhotopeakMax;
  Params.triggerPhotopeakCut = tempStream.str().c_str();
  
  //superimpose on the same plot, to visualize the cut
  TH1F *TriggerSpectrumHighlight = new TH1F("TriggerSpectrumHighlight","",1200,0,12000);
  tempStream.str("");
  tempStream << "Tagging >> TriggerSpectrumHighlight";
  TriggerSpectrumHighlight->SetLineColor(3);
  TriggerSpectrumHighlight->SetFillColor(3);
  TriggerSpectrumHighlight->SetFillStyle(3001);
  chain->Draw(tempStream.str().c_str(),Params.triggerPhotopeakCut,"same");
  
  
  //another flood histo, but tagging on the PE of tag cry
  TH2F *FloodPEtagCry; 
  TCanvas *FloodPEtagCryCanvas = new TCanvas("FloodPEtagCryCanvas","",800,800);
  FloodPEtagCry = make2Dhisto(Params,chain,"Weighted Sum","FloodY:FloodX",Params.triggerPhotopeakCut,"COLZ");
  FloodPEtagCry->GetXaxis()->SetTitle("U");
  FloodPEtagCry->GetYaxis()->SetTitle("V");
  FloodPEtagCry->GetZaxis()->SetRangeUser(20,400);
  FloodPEtagCry->SetTitle("");
  
  //on this flood histogram, we look for 2d peaks and store in Params
  int nfound = find2dPeaks(Crystal,Params,FloodPEtagCry);
  
  // find a way to assign the peaks to a real crystal
  // use an external look up table to divide the 16 mppc areas in the flood histo
  // then sort the peaks in the mppc area from left to right and to to bottom
  
  
  
  
  
  //TCanvas *c2 = new TCanvas;
  if(parallelNum != -1 )
  {
    for ( int j = parallelNum ; j < parallelNum+1 ; j++)
    {
      std::cout << "Working on peak " << j << std::endl;
      
      
      std::cout << "Single Spectrum" << std::endl;
      TLegend *legendSingle = new TLegend(0.6,0.7,0.88,0.88,"");
      legendSingle->SetFillStyle(0);
      tempStream.str("");
      tempStream << "CanvasSingle" << j;
      Crystal[j].CsingleMppc = new TCanvas(tempStream.str().c_str(),tempStream.str().c_str(),800,600);
      
      
      tempStream.str("");
      tempStream << "SingleSpectrumAll" << j;
      Crystal[j].singleMppc = new TH1F (tempStream.str().c_str(),"",250,500,5500);
      tempStream.str("");
      tempStream << "ch" << Crystal[j].crystalMPPC << ">>" << "SingleSpectrumAll" << j;
      Crystal[j].singleMppc->GetXaxis()->SetTitle("ADC");
      Crystal[j].singleMppc->GetYaxis()->SetTitle("A.U.");
      //  std::cout << tempStream.str().c_str() << "\t" << Crystal[j].crystalCut+Params.triggerPhotopeakCut+Params.zCut[z] << std::endl;
      //Crystal[j].CsingleMppc->cd();
      //chain->Draw(tempStream.str().c_str(),Crystal[j].crystalCut+Params.triggerPhotopeakCut);
      tempStream.str("");
      tempStream << "Single Spectrum of Crystal " << j << ", MPPC " << Crystal[j].crystalMPPC;
      Crystal[j].singleMppc->SetTitle(tempStream.str().c_str());
      Crystal[j].singleMppc->SetFillColor(1);
      Crystal[j].singleMppc->SetFillStyle(3001);
      //histo->Scale(1/histo->GetEntries());
      //     delete Ctemp;
      
      
      
      for (int z = 0 ; z <  Params.zPosition.size() ; z++)
      {
	Crystal[j].ZsingleMppc[z] = singleMPPCplot(j,z,Crystal,Params,chain);
	//Crystal[j].CsingleMppc->cd();
	tempStream.str("");
	
	
	//       if(z==0)
	// 	legendSingle->AddEntry(Crystal[j].ZsingleMppc[z],"All Crystal","f");
	//       else
	//       {
	tempStream << "z" << z+1 << " = " << Params.zPosition[z] << "mm";
	legendSingle->AddEntry(Crystal[j].ZsingleMppc[z],tempStream.str().c_str(),"f");
	//       }
	Crystal[j].singleMppc->Add(Crystal[j].ZsingleMppc[z]);
	//       if(z==0)
	// 	Crystal[j].ZsingleMppc[z]->Draw();
	//       else
	// 	Crystal[j].ZsingleMppc[z]->Draw("same");
	
      }
      
      legendSingle->AddEntry(Crystal[j].singleMppc,"All Crystal","f");
      Crystal[j].CsingleMppc->cd();
      Crystal[j].singleMppc->Draw();
      for (int z = 0 ; z < Params.zPosition.size() ; z++)
      {
	Crystal[j].CsingleMppc->cd();
	Crystal[j].ZsingleMppc[z]->Draw("same");
      }
      
      legendSingle->Draw();
      
      
      
      
      
      
      
      
      //std::cout << "Working on peak " << j << std::endl;
      
      
      std::cout << "Sum Spectrum" << std::endl;
      TLegend *legendSum = new TLegend(0.6,0.7,0.88,0.88,"");
      legendSum->SetFillStyle(0);
      tempStream.str("");
      tempStream << "CanvasSum" << j;
      Crystal[j].CallMppc = new TCanvas(tempStream.str().c_str(),tempStream.str().c_str(),800,600);
      
      tempStream.str("");
      tempStream << "SumSpectrumAll" << j;
      Crystal[j].allMppc = new TH1F (tempStream.str().c_str(),"",250,500,12500);
      std::string sumstring;
      tempStream.str("");
      int isNotFirst = 0;
      tempStream << "(";
      for(int i = 0; i < 16 ; i++)
      {
	if(Input[i].Data) //do it only if the channel have data
	{
	  if(i != Params.taggingCrystal)
	  {
	    if(isNotFirst) tempStream << "+";
	    isNotFirst++;
	    tempStream << "ch" << i;
	  }
	}
      }
      tempStream << ")";
      sumstring = tempStream.str();
      tempStream << ">>" << "SumSpectrumAll" << j;
      Crystal[j].allMppc->GetXaxis()->SetTitle("ADC");
      Crystal[j].allMppc->GetYaxis()->SetTitle("A.U.");
      //  std::cout << tempStream.str().c_str() << "\t" << Crystal[j].crystalCut+Params.triggerPhotopeakCut+Params.zCut[z] << std::endl;
      //Crystal[j].CallMppc->cd();
      //chain->Draw(tempStream.str().c_str(),Crystal[j].crystalCut+Params.triggerPhotopeakCut);
      tempStream.str("");
      tempStream << "Sum Spectrum of Crystal " << j << ", MPPC " << Crystal[j].crystalMPPC;
      Crystal[j].allMppc->SetTitle(tempStream.str().c_str());
      Crystal[j].allMppc->SetFillColor(1);
      Crystal[j].allMppc->SetFillStyle(3001);
      //histo->Scale(1/histo->GetEntries());
      //     delete Ctemp;
      
      for (int z = 0 ; z < Params.zPosition.size() ; z++)
      {
	Crystal[j].ZallMppc[z] = allMPPCplot(j,z,Crystal,Input,Params,chain);
	
	tempStream.str("");
	
	
	tempStream << "z" << z+1 << " = " << Params.zPosition[z] << "mm";
	legendSum->AddEntry(Crystal[j].ZallMppc[z],tempStream.str().c_str(),"f");
	
	Crystal[j].allMppc->Add(Crystal[j].ZallMppc[z]);
	//       if(z==0)
	// 	Crystal[j].ZallMppc[z]->Draw();
	//       else
	
      }
      Crystal[j].CallMppc->cd();
      Crystal[j].allMppc->Draw();
      legendSum->AddEntry(Crystal[j].allMppc,"All Crystal","f");
      
      for (int z = 0 ; z < Params.zPosition.size() ; z++)
      {
	Crystal[j].CallMppc->cd();
	Crystal[j].ZallMppc[z]->Draw("same");
      }
      legendSum->Draw();
      
      
      
      
      
      
      
      
      std::cout << "Ratio Plot" << std::endl;
      TLegend *legendRatio = new TLegend(0.6,0.7,0.88,0.88,"");
      legendRatio->SetFillStyle(0);
      tempStream.str("");
      tempStream << "CanvasRatio" << j;
      Crystal[j].Cratio = new TCanvas(tempStream.str().c_str(),tempStream.str().c_str(),800,600);
      for (int z = 0 ; z < Params.zPosition.size() ; z++)
      {
	Crystal[j].Zratio[z] = ratioPlot(j,z,Crystal,Input,Params,chain);
	Crystal[j].Cratio->cd();
	tempStream.str("");
	
	tempStream << "z" << z+1 << " = " << Params.zPosition[z] - Params.Zzero << "mm";
	legendRatio->AddEntry(Crystal[j].Zratio[z],tempStream.str().c_str(),"f");
	
	if(z==0)
	  Crystal[j].Zratio[z]->Draw();
	else
	  Crystal[j].Zratio[z]->Draw("same");
      }
      legendRatio->Draw();
      
      //     gStyle->SetOptFit(1);
      std::cout << "Correlation Plot" << std::endl;
      tempStream.str("");
      tempStream << "Canvas_DOIvsR_" << j;
      
      Crystal[j].Cplot = new TCanvas(tempStream.str().c_str(),"",800,600);
      tempStream << ".png";
      std::string plotFile = tempStream.str().c_str();
      
      std::vector<double> tempEx,tempY;
      for(int tt = 0 ; tt < Params.zPosition.size()  ;tt++)
      {
	tempEx.push_back(0.005);
	tempY.push_back(Crystal[j].y[tt] - (Params.Zzero));
      }
      
      //Crystal[j].doiR = new TGraphErrors(5,&Crystal[j].x[0],&Crystal[j].y[0],&Crystal[j].ex[0],&Crystal[j].ey[0]);
      Crystal[j].doiR = new TGraphErrors(Params.zPosition.size(),&Crystal[j].x[0],&tempY[0],&tempEx[0],&Crystal[j].ey[0]);
      Crystal[j].doiR->GetXaxis()->SetTitle("W");
      Crystal[j].doiR->GetYaxis()->SetTitle("DOI [mm]");
      tempStream.str("");
      tempStream << "DOI vs. R - Crystal " << j << " - MPPC " << Crystal[j].crystalMPPC;
      Crystal[j].doiR->SetTitle("");
      Crystal[j].Cplot->cd();
      Crystal[j].doiR->Draw("A*");
      
      TF1 *fit = new TF1("fit", "[0]+x*[1]",0.2,0.6);
      fit->SetParameter(0,80);
      fit->SetParameter(1,-150);
      Crystal[j].doiR->Fit("fit","QR");
      fit->SetLineColor(2);
      Crystal[j].Cplot->cd();
      fit->Draw("same");
      
      //Crystal[j].Cplot->Write(plotFile.c_str());
      Crystal[j].fitPar0 = fit->GetParameter(0);
      Crystal[j].fitPar1 = fit->GetParameter(1);
      std::cout << "Crystal " << j << " - MPPC " << Crystal[j].crystalMPPC << " = " << fit->GetParameter(1) << std::endl;
      
    }
  }
  
  
  
  
  
  
  
  
  
  if(Params.saveAnalysisTree)
  {
    //     TFile* fTree = new TFile("temp.root","recreate");
    //     fTree->cd();
    //     chain->Write();
    //     fTree->Close();
  }
  
  
  
  
  
  
  std::stringstream OutFile;
  
  if(parallelNum != -1 )
    OutFile << "tempOut" << parallelNum  << ".root";
  else 
    OutFile << "tempOut.root";
  
  TString fileOutput = OutFile.str().c_str();
  TFile* fHisto = new TFile(fileOutput,"recreate");
  
  std::cout << "Writing results to ROOT file " << fileOutput << std::endl;
  TDirectory *directory[nfound]; //TDirectory 
  
  UncutFloodCanvas->Write();
  TriggerSpectrumCanvas->Write();
  FloodPEtagCryCanvas->Write();
  if(parallelNum != -1 )
  {
    for (  int j = parallelNum ; j < parallelNum+1 ; j++)
    {
      if(Crystal[j].Data)
      {
	
	std::stringstream DirStream;
	DirStream << "Crystal_" << j;
	//DirString = DirStream.str();
	directory[j] = fHisto->mkdir(DirStream.str().c_str());
	directory[j]->cd();
	
	
	Crystal[j].CsingleMppc->Write();
	Crystal[j].CallMppc->Write();
	Crystal[j].Cratio->Write();
	Crystal[j].Cplot->Write();
	for (int z = 0 ; z < Params.zPosition.size() ; z++)
	{
	  Crystal[j].ZsingleMppc[z]->Write();
	  Crystal[j].ZallMppc[z]->Write();
	  Crystal[j].Zratio[z]->Write();
	}
	
	//save fit results to a text file
	std::stringstream textFile;
	textFile << "CorrelationFit" << j << ".txt";
	
	ofstream dump;
	dump.open(textFile.str().c_str(),std::ofstream::out);
	dump << "Crystal" << "\t" << "MPPC" 			<< "\t" << "FloodX"  			<< "\t" << "FloodY" 			<< "\t" << "Fit(m)" << std::endl;
	dump << j 	<< "\t" << Crystal[j].crystalMPPC 	<< "\t" << Params.ellipseCenterX[j]  	<< "\t" << Params.ellipseCenterY[j] 	<< "\t" << Crystal[j].fitPar1 << std::endl;
	dump.close();
	
	
      }
    }
  }
  
  fHisto->Close();
  return 0;
  
}