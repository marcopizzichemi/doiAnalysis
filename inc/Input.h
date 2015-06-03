struct Input_t
{
  int DigitizerChannel;					//num of the digitizer channel
  TString LabelName;					//name of the MPPC channel
  int CanvasCd;						//position in the 4x4 Canvases, so that the visualization is alwais in the same order
  bool Data;						//this channel have data (true) or no (false)
//   int BigCanvasPosition;				//position in the big canvas fo 64 spectra
//   bool Module[2];					//module 0 and 1 true or false
//   double param0,param1;					//saturation parameters
//   
//   Float_t *xpeaks;					//array with x positions of peaks in the 2d flood histogram generated accepting all the events
//   Float_t *ypeaks;					//array with y positions of peaks in the 2d flood histogram generated accepting all the events
//   Float_t *zpeaks;					//array with z positions of peaks in the 2d flood histogram generated accepting all the events
//   Float_t *xsigma;					//array with x sigmas of peaks in the 2d flood histogram generated accepting all the events
//   Float_t *ysigma;					//array with y sigmas of peaks in the 2d flood histogram generated accepting all the events
//   
//   float fit2DmeanX[4],fit2DmeanY[4];			//results of the 2d fits for the 4 crystals, means on X and Y
//   float fit2DsigmaX[4],fit2DsigmaY[4];  		//results of the 2d fits for the 4 crystals, sigmas on X and Y
//   float CrystalMean[4],CrystalSigma[4];			//for each MPPC, 4 crystals. Once the single spectra are extracted, a fit finds out the mean and sigma of the photoelectric peaks
//   float LightSharingCrystalMean,LightSharingCrystalSigma;
//   float SumCrystalMean[4],SumCrystalSigma[4];
//   float ellipseCenterX[4],ellipseCenterY[4];		//values that will be actually used in the elliptic cuts. could be different from the ones above if the ellipes intercept
//   float ellipseWidthX[4],ellipseWidthY[4];		//values that will be actually used in the elliptic cuts. could be different from the ones above if the ellipes intercept
//   
//   //histograms and canvases
  TH1F *channel;					//the histogram of this channel MPPC spectrum -
//   
//   TH1F *channelClone;					//the histogram of this channel MPPC spectrum - clone
//   TH1F *channelOriginal;				//the histogram of this channel MPPC spectrum - no cuts
//   //plots in photons
//   TH1F *LYchannel;					//same but scaled in Ph/MeV
//   TH1F *LYchannelTemp;					//same but scaled in Ph/MeV
//   TH1F *LYchannelOriginal;				//same but scaled in Ph/MeV
//   //plots in pixels fired
//   TH1F *PFchannel;					//same but scaled in Pixels Fired
//   TH1F *PFchannelTemp;					//same but scaled in Pixels Fired
//   TH1F *PFchannelOriginal;				//same but scaled in Pixels Fired
//   
//   
//   TCanvas *SpectraCanvas;				//canvas for the 2x2 spectra of the 4 crystals in this mppc
  bool IsCrystalHit[4];
  TCut crystalCut[4];
//   TCanvas *SumSpectraCanvas;
  TH1F *crystal[4];					//4 histograms for the 4 spectra of the crystals in this mppc
//   TH1F *crystalClone[4];
//   TH1F *crystalHighlight[4];
//   TH1F *SumcrystalHighlight[4];
//   TH1F *crystalSum[4];					//spectra of the 4 crystals, but using the 16ch sum instead of just trigger mppc
//   TH2F *recFlood;					//histogram for the 4x4 flood reconstruction
//   TCanvas *recChannelCanvas;
//   TCanvas *MPPCCanvas;
//   TCanvas *LightSharingCanvas;				//a canvas for the lightsharing when you cut only on the TriggerChannel info;
//   TCanvas *LightSharingCanvasPE;			//a canvas for the lightsharing when you cut on the TriggerChannel info and on the PE events;
//   TCanvas *LYLightSharingCanvasPE;			//a canvas for the lightsharing when you cut on the TriggerChannel info and on the PE events;
// 
//   double lightSharingData[32];				//array to hold the peak positions, in LY, of the sharing data. 32 is just for simmetry..
//   
//   TH1F *spectraTag[32];					//the 32 spectra when tagging on this channel
//   TH1F *spectraTagPE[32];				//the 32 spectra when tagging on this channel and cutting on the PE
//   TH1F *LYspectraTagPE[32];
//   TF2 *f2;						//2d function to fit the peaks in the flood histos
//   TCanvas *CrystalLightSharingCanvas[4];		//4 canvas, one per crystal, to show the light sharing
//   TH1F *CrystalLightSharing[4][32];			//4 crystal by 32 by 16 channels...
//   TCanvas *CrystalDOICanvas[4];				//4 canvas, one per crystal, to play with DOI
//   TCanvas *SumCrystalDOICanvas[4];
//   TH1F *DOIRatio[4];					//4 spectra for the DOI ratio (trigger channel / all channels), one per crystal
//   TH1F *SumDOIRatio[4];
//   TH1F *SumDOIRatioPeak[4];
};