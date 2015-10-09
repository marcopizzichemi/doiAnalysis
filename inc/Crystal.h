struct Crystal_t 
{
  //int DigitizerChannel;					//num of the digitizer channel
  //TString LabelName;					//name of the MPPC channel
  //int CanvasCd;						//position in the 4x4 Canvases, so that the visualization is in the same order
  bool Data;						//this crystal have data (true) or no (false)
  
  
  TCut crystalCut;
  int crystalMPPC;
  TCut crystalPeakCut;
  long int SingleZentries[5];
  long int SumZentries[5];
  
  TCanvas* CsingleMppc;
  TH1F* singleMppc;
  
  TCanvas* ZCsingleMppc[5];
  TH1F* ZsingleMppc[5];
  
  TCanvas* CallMppc;
  TH1F* allMppc;
  
  TCanvas* ZCallMppc[5];
  TH1F* ZallMppc[5];
  TCut ZcrystalPeakCut[5];
  
  
  TCanvas* Cratio;
  TCanvas* CZratio[5];
  TCanvas* CZratioAll;
  TH1F* Zratio[5];
  TH1F* ZratioAll;
  TH1F* ratio;
  
  double x[5],y[5],ex[5],ey[5];
  
  double fitPar0,fitPar1;
  
  TCanvas *Cplot; 
  TGraphErrors *doiR; 
  
 
};