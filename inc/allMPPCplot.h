TH1F* allMPPCplot(int j,int z,Crystal_t (&Crystal)[64],Input_t (&Input)[16],Params_t &Params,TChain *chain)
{
  //gStyle->SetOptStat(0);
  //gStyle->SetOptFit(1);
  std::cout << "All MPPC Spectrum - z " << z << std::endl;
  std::stringstream tempStream;
  tempStream.str("");
  tempStream << "CanvasSumSpectrum" << j << "." << z;
  Crystal[j].ZCallMppc[z] = new TCanvas(tempStream.str().c_str(),"");
  tempStream.str("");
  tempStream << "SumSpectrum" << j << "." << z;
  TH1F *histo = new TH1F (tempStream.str().c_str(),"",250,500,12500);
  std::string sumstring;
  tempStream.str("");
  int isNotFirst = 0;
  tempStream << "(";
  for(int i = 0; i < 16 ; i++)
  {
    if(Input[i].Data) //do it only if the channel have data
    {
      if(i != Params.taggingCrystal) //which really should never be
      {
	if(isNotFirst) tempStream << "+";
	isNotFirst++;
	tempStream << "ch" << i;
      }
    }
  }
  tempStream << ")";
  sumstring = tempStream.str();
  
  tempStream << ">>" << "SumSpectrum" << j << "." << z;

  histo->GetXaxis()->SetTitle("ADC");
  histo->GetYaxis()->SetTitle("Counts");
  
  chain->Draw(tempStream.str().c_str(),Crystal[j].crystalCut+Params.triggerPhotopeakCut+Params.zCut[z]);
  tempStream.str("");
  tempStream << "Sum Spectrum of Crystal " << j << ", MPPC " << Crystal[j].crystalMPPC;
  //std::cout << tempStream.str().c_str() << "\t" << Crystal[j].crystalCut+Params.triggerPhotopeakCut+Params.zCut[z] << std::endl;
  std::cout << histo->GetEntries() << std::endl;
  histo->SetTitle(tempStream.str().c_str());
  histo->SetFillColor(z+2);
  histo->SetFillStyle(3001);
  
  //find the entries for this z in this crystal
  Crystal[j].SumZentries[z] = histo->GetMaximum();
  histo->Scale(Params.zScaling[z]*(1/histo->GetMaximum()));
  
  
  TSpectrum *sCrystal;
  sCrystal = new TSpectrum(1);
  Int_t CrystalPeaksN = sCrystal->Search(histo,1,"goff",0.5); //TODO pass to "goff"
  Float_t *CrystalPeaks = sCrystal->GetPositionX();
  //Float_t *TagCrystalPeaksY = sTagCrystal->GetPositionY();
  TF1 *gauss = new TF1("gauss", "gaus");
  histo->Fit("gauss","Q","",CrystalPeaks[0] - 0.2*CrystalPeaks[0],CrystalPeaks[0] + 0.2*CrystalPeaks[0]);
  tempStream.str("");
  tempStream << sumstring << ">" << gauss->GetParameter(1) - 1.5*gauss->GetParameter(2) << "&&" << sumstring << "<" << gauss->GetParameter(1) + 2.0*gauss->GetParameter(2);
  
  Crystal[j].ZcrystalPeakCut[z] = tempStream.str().c_str();
  
//   std::cout << "aaaaaaaaaaaaaaaaa" << std::endl;
  
  //histo->Scale(1/histo->GetEntries());
  return histo;
}