int printConfigFile(Params_t &Params)
{
  std::cout << "/***********************************************/" << std::endl;
  std::cout << "|                                               |" << std::endl;
  std::cout << "|          Printing config parameters           |" << std::endl;
  std::cout << "|                                               |" << std::endl;
  std::cout << "/***********************************************/" << std::endl;
  std::cout << "Tagging Crystal Ch =\t" 			<< Params.taggingCrystal 		<< std::endl;
  //std::cout << "Saturation File =\t" 			<< Params.saturationFileName 		<< std::endl;
  //std::cout << "Coincidence \t=\t" 			<< Params.coincidence 			<< std::endl;
  //std::cout << "Deep analysis \t=\t" 			<< Params.deepAnalysis 			<< std::endl;
  //std::cout << "DOI analysis \t=\t" 			<< Params.doiAnalysis 			<< std::endl;
  //std::cout << "Save analysis TTree \t=\t" 		<< Params.saveAnalysisTree 		<< std::endl;
  //std::cout << "Perform Grid Cut \t=\t" 		<< Params.CutGrid 			<< std::endl;
  //if(Params.CutGrid)
  //  std::cout << "Grid Cut Width \t=\t" 		<< Params.lipCutLevel 			<< std::endl;
//   std::cout << "Histos 1D Max \t=\t" 			<< Params.histomax 			<< std::endl;
//   std::cout << "Histos 1D Bins \t=\t" 			<< Params.binning 			<< std::endl;
//   std::cout << "Histos 2D-global Bins \t=\t" 		<< Params.histo2DglobalBin 		<< std::endl;
//   std::cout << "Histos 2D-channel Bins \t=\t" 		<< Params.histo2DchannelBin 		<< std::endl;
//   std::cout << "Histo 1D LY max \t=\t" 			<< Params.LYhistomax 			<< std::endl;
//   
//   std::cout << "Photodetector PDE \t=\t" 		<< Params.MppcPDE 			<< std::endl;
//   std::cout << "Photodetector Gain \t=\t" 		<< Params.MppcGain 			<< std::endl;
//   std::cout << "Gamma Source Energy \t=\t" 		<< Params.SourceEnergy 			<< std::endl;
//   std::cout << "ADC charge binning \t=\t" 		<< Params.AdcChargeBinning 		<< std::endl;
//   
//   std::cout << "Photoelectric hard coded cut Min (optional) \t=\t" 		<< Params.PEmin 		<< std::endl;
//   std::cout << "Photoelectric hard coded cut Max (optional) \t=\t" 		<< Params.PEmax 		<< std::endl;
  for (int i = 0 ; i < 16 ; i++)
  {
    std::cout << "Channel [" << i << "] \t=\t"	  	<< Params.ChannelOn[i] 		<< "\t"
    //<< Params.ChannelModule[i] 		<< "\t"
    << Params.ChannelLabel[i] 		<< "\t"
    << Params.ChannelPosition[i] 		<< std::endl; 
  }
  for (int i = 0 ; i < Params.zPosition.size() ; i++)
  {
    std::cout << "Z Position [" << i << "] \t=\t" << Params.zPosition[i] << "\t"
    << Params.zFileName[i] << "\t"
    << Params.zScaling[i] << std::endl;
  }
  
  for (int i = 0 ; i < 3 ; i++)
  {
    std::cout << "X Border [" << i << "] = " << Params.XBorder[i] << std::endl; 
  }
  for (int i = 0 ; i < 3 ; i++)
  {
    std::cout << "Y Border [" << i << "] = " << Params.YBorder[i] << std::endl; 
  }
  for(int i = 0 ; i < Params.zCut.size();i++)
  {
    std::cout << "Z Cut [" << i << "] = " << Params.zCut[i] << std::endl; 
  }
  
  
  
  
  return 0;
}