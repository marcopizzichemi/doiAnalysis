TH1F* ratioPlot(int j,int z,Crystal_t (&Crystal)[64],Input_t (&Input)[16],Params_t &Params,TChain *chain)
{
  //gStyle->SetOptStat(0);
  //gStyle->SetOptFit(1);
  std::cout << "Ratio - z " << z << std::endl;
  std::stringstream tempStream;
  tempStream.str("");
  tempStream << "CanvasRatio" << j << "." << z;
  Crystal[j].CZratio[z] = new TCanvas(tempStream.str().c_str(),"");
  tempStream.str("");
  tempStream << "Ratio" << j << "." << z;
  TH1F *histo = new TH1F (tempStream.str().c_str(),"",250,0,1);
  //std::string sumstring;
  tempStream.str("");
  tempStream << "ch" << Crystal[j].crystalMPPC << "/";
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
  //sumstring = tempStream.str();
  
  tempStream << ">>" << "Ratio" << j << "." << z;

  histo->GetXaxis()->SetTitle("W");
  histo->GetYaxis()->SetTitle("Counts");
  //std::cout << tempStream.str().c_str() << "\t" << Crystal[j].crystalCut+Params.triggerPhotopeakCut+Params.zCut[z] << std::endl;
  chain->Draw(tempStream.str().c_str(),Crystal[j].crystalCut+Params.triggerPhotopeakCut+Params.zCut[z]+Crystal[j].ZcrystalPeakCut[z]);
  tempStream.str("");
  tempStream << "Ratio Plot " << j << ", MPPC " << Crystal[j].crystalMPPC;
  histo->GetYaxis()->SetRangeUser(0,0.3);
  histo->SetTitle("");
  histo->SetFillColor(z+2);
  histo->SetFillStyle(3001);
  histo->Scale(1/histo->GetEntries());
  
  //TSpectrum *sCrystal;
  //sCrystal = new TSpectrum(1);
  //Int_t CrystalPeaksN = sCrystal->Search(histo,1,"",0.5); //TODO pass to "goff"
  //Float_t *CrystalPeaks = sCrystal->GetPositionX();
  //Float_t *TagCrystalPeaksY = sTagCrystal->GetPositionY();
  TF1 *gauss2 = new TF1("gauss2", "gaus",0,1);
  histo->Fit("gauss2","QRN");
  //tempStream.str("");
  //tempStream << sumstring << ">" << gauss->GetParameter(1) - 1.5*gauss->GetParameter(2) << "&&" << sumstring << "<" << gauss->GetParameter(1) + 2.0*gauss->GetParameter(2);
  
  //Crystal[j].ZcrystalPeakCut[z] = tempStream.str().c_str();
  //std::cout << gauss2->GetParameter(1) << "\t" << gauss2->GetParameter(2) << std::endl; 
  Crystal[j].x[z] = gauss2->GetParameter(1);
  Crystal[j].y[z] = Params.zPosition[z];
  Crystal[j].ex[z] = std::abs(gauss2->GetParameter(2));
  Crystal[j].ey[z] = 0.5;
  
  
  return histo;
}