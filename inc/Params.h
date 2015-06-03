struct Params_t 
{
//   std::string saturationFileName;
//   bool coincidence;
//   bool deepAnalysis;
//   bool doiAnalysis;
  bool saveAnalysisTree;
//   bool LightSharingAnalysis;
  bool ChannelOn[16];
  //int ChannelModule[32];
  std::string ChannelLabel[16]; 
  int ChannelPosition[16];
  std::vector<double> zPosition,zScaling;
  std::vector<std::string> zFileName;
  std::vector<TCut> zCut;
  
  //borders in the flood histo
  double XBorder[3];
  double YBorder[3];
  
  //tagging crystal
  int taggingCrystal;
  double tagPhotopeakMin;
  double tagPhotopeakMax;
  TCut triggerPhotopeakCut;
  
  double Zzero;
  
  
  //2d peaks
  Float_t *xpeaks;					//array with x positions of peaks in the 2d flood histogram generated accepting all the events
  Float_t *ypeaks;					//array with y positions of peaks in the 2d flood histogram generated accepting all the events
  Float_t *zpeaks;					//array with z positions of peaks in the 2d flood histogram generated accepting all the events
  Float_t *xsigma;
  Float_t *ysigma;
  float fit2DmeanX[16],fit2DmeanY[16];			//results of the 2d fits for the 16 crystals, means on X and Y
  float fit2DsigmaX[16],fit2DsigmaY[16];  		//results of the 2d fits for the 16 crystals, sigmas on X and Y
  float ellipseCenterX[16],ellipseCenterY[16];		//values that will be actually used in the elliptic cuts. could be different from the ones above if the ellipes intercept
  float ellipseWidthX[16],ellipseWidthY[16];		//values that will be actually used in the elliptic cuts. could be different from the ones above if the ellipes intercept  
//   std::vector<TCut> crystalCut;
//   std::vector<int> crystalMPPC;
//   std::vector<TCut> crystalPeakCut;
//  
//   //histograms
//   std::vector<TCanvas*> CsingleMppc;
//   std::vector<TH1F*> singleMppc;
//   
//   std::vector<TCanvas*> CallMppc;
//   std::vector<TH1F*> allMppc;
//   
//   std::vector<TCanvas*> Cratio;
//   std::vector<TH1F*> ratio;
  
  
//   int histomax;
//   double MppcPDE;
//   double MppcGain;
//   double SourceEnergy;
//   double AdcChargeBinning;
//   double LYhistomax;
//   double PFhistomax;
//   int binning;
//   int LYbinning;
//   int PFbinning;
//   int histo2DglobalBin;
//   int histo2DchannelBin;
//   double PEmin;
//   double PEmax;
//   double lipCutLevel;
//   bool CutGrid;
};