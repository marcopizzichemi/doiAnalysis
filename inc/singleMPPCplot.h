TH1F* singleMPPCplot(int j,int z,Crystal_t (&Crystal)[64],Params_t &Params,TChain *chain)
{
  //gStyle->SetOptStat(0);
  //gStyle->SetOptFit(1);
  std::stringstream tempStream;
  TCanvas *Ctemp = new TCanvas("temp","temp");
  Ctemp->cd();
  std::cout << "Single Spectrum - z " << z << std::endl;
  tempStream.str("");
  tempStream << "SingleSpectrum" << j << "." << z;
  TH1F *histo = new TH1F (tempStream.str().c_str(),"",250,500,5500);
  tempStream.str("");
  tempStream << "ch" << Crystal[j].crystalMPPC << ">>" << "SingleSpectrum" << j << "." << z;
  
  histo->GetXaxis()->SetTitle("ADC");
  histo->GetYaxis()->SetTitle("A.U.");
//   std::cout << tempStream.str().c_str() << "\t" << Crystal[j].crystalCut+Params.triggerPhotopeakCut+Params.zCut[z] << std::endl;
  chain->Draw(tempStream.str().c_str(),Crystal[j].crystalCut+Params.triggerPhotopeakCut+Params.zCut[z]);
  tempStream.str("");
  tempStream << "Single Spectrum of Crystal " << j << ", MPPC " << Crystal[j].crystalMPPC;
  
  
  
  histo->SetTitle(tempStream.str().c_str());
  histo->SetFillColor(z+2);
  histo->SetFillStyle(3001);
  
  //find the entries for this z in this crystal
  Crystal[j].SingleZentries[z] = histo->GetMaximum();
  histo->Scale(Params.zScaling[z]*(1/histo->GetMaximum()));

  
  
  //histo->Scale(1/histo->GetEntries());
  delete Ctemp;
  return histo;
}