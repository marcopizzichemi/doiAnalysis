int parseConfigFile(std::istream& FileHeader,Params_t &Params)
{
  //Params.zCut.push_back(""); //first z cut is just to have full plots
  // defines the strings to look for
  //   std::string FileName = "name of data file := ";       
  //   std::string NumberFormat = "!number format := ";             
  //   std::string SizeX = "!matrix size [1] := ";         
  //   std::string SizeY = "!matrix size [2] := ";
  //   std::string SizeZ = "!matrix size [3] := "; 
  //   std::string SpacingX = "scaling factor (mm/pixel) [1] := ";	 
  //   std::string SpacingY = "scaling factor (mm/pixel) [2] := ";	 
  //   std::string SpacingZ = "scaling factor (mm/pixel) [3] := ";
  std::stringstream ChIdentifierStream[16];
  std::string ChIdentifierString[16];
  std::stringstream ZFileIdentifier[16];
//   std::string saturation 		= "!Saturation File Name =";
//   std::string analysisString 		= "!Deep Analysis =";
//   std::string coincidenceString 	= "!Coincidence =";
//   std::string histoMaxString 		= "!Histo 1D Max =";
//   std::string histoBinString 		= "!Histo 1D Bin =";
//   std::string histo2DBinString 		= "!Histo 2D-global Bin =";
//   std::string histo2DChBinString 	= "!Histo 2D-channel Bin =";
//   std::string PEminString 		= "!Photoelectric Peak Min =";
//   std::string PEmaxString 		= "!Photoelectric Peak Max =";
//   std::string GridCutString 		= "!Grid Cut Width =";
  std::string SaveTreeString 		= "!Save Analysis TTree =";
  std::string taggingCrystalString      = "!Tagging channel =";
//   std::string doiAnalysisString 	= "!DOI Analysis =";
//   std::string LightSharingAnalysisString= "!Light Sharing Analysis =";
//   std::string MppcPDEString		= "!Photodetector PDE =";
//   std::string MppcGainString		= "!Photodetector Gain =";
//   std::string SourgeEnergyString	= "!Gamma Source Energy =";
//   std::string AdcChargeBinningString	= "!ADC charge binning =";
  std::string ZzeroString = "!Z zero =";
  std::stringstream inputTreeIdentifier[16];  //all right, 32 is maybe too optimistic... 
  std::stringstream XBorderIdentifier[3],YBorderIdentifier[3];
  
  
  //std::string analysisChoice;
  for (int i = 0; i < 16 ; i++)
  {
    ChIdentifierStream[i] << "!Channel [" << i << "] =";
    ChIdentifierString[i] = ChIdentifierStream[i].str(); //set the search channel strings
    Params.ChannelLabel[i] = "VOID"; //Initialize to VOID, then only the specified will be changed
    Params.ChannelOn[i] = false; //Initialize to off all channels 
    inputTreeIdentifier[i] << "!Z Position [" << i << "] =";
    ZFileIdentifier[i]  << "!Z File [" << i << "] =";
  }
  for (int i = 0; i < 3 ; i++)
  {
    XBorderIdentifier[i] << "!X Border [" << i << "] =";
    YBorderIdentifier[i]<< "!Y Border [" << i << "] =";
  }
  std::string s1; 
  //looks for the strings defined above, assing the rest of the line to the correct variable      
  while(getline(FileHeader,s1))	
  {
    unsigned int i;
    
    i = s1.find(taggingCrystalString);
    if (i==0)  
    {
      std::string tempString;
      tempString.assign(s1, taggingCrystalString.size() , s1.size());	
      tempString.erase(remove_if(tempString.begin(), tempString.end(), isspace), tempString.end());
      Params.taggingCrystal = atoi(tempString.c_str());
    }
    
    
    i = s1.find(ZzeroString);
    if (i==0)  
    {
      std::string tempString;
      tempString.assign(s1, ZzeroString.size() , s1.size());	
      tempString.erase(remove_if(tempString.begin(), tempString.end(), isspace), tempString.end());
      Params.Zzero = atof(tempString.c_str());
    }
    
    
    
    i = s1.find(SaveTreeString);
    if (i==0)  
    {
      std::string analysis;
      analysis.assign(s1, SaveTreeString.size() , s1.size());
      analysis.erase(remove_if(analysis.begin(), analysis.end(), isspace), analysis.end());
      if(analysis == "yes" |analysis == "Yes" |  analysis == "YES" | analysis == "on" | analysis == "On" | analysis == "ON" | analysis == "1" )
      {
	Params.saveAnalysisTree = true;
      } 
    }

    
    //channel parameters
    for (int j = 0 ; j < 16 ; j++)
    {
      i = s1.find(ChIdentifierString[j]); //find the info about the channel
      if (i==0) //if the channel is in the config file  
      {
	//first, turn on the channel
	Params.ChannelOn[j] = true;
	//then, tokenize the string
	std::string parametersString,noLeadingSpace;
	parametersString.assign(s1, ChIdentifierString[j].size() , s1.size());
	noLeadingSpace = parametersString.substr(parametersString.find_first_not_of(" "),parametersString.length()-parametersString.find_first_not_of(" "));
	
	//std::cout << "noLeadingSpace " << noLeadingSpace << std::endl;
	std::string::size_type pos = noLeadingSpace.find_first_of(" ,.-\t");
	std::string token = noLeadingSpace.substr(0, pos);
	std::string::size_type pos1 = noLeadingSpace.find_first_of(" ,.-\t",pos+1);
	std::string token1 = noLeadingSpace.substr(pos+1, pos1-1);
// 	std::string::size_type pos2 = noLeadingSpace.find_first_of(" ,.-\t",pos1+1);
// 	std::string token2 = noLeadingSpace.substr(pos1+1, pos2-1);
	//std::cout << token << " " << token1 << " " << token2 << std::endl;
	token.erase(remove_if(token.begin(), token.end(), isspace), token.end());
	token1.erase(remove_if(token1.begin(), token1.end(), isspace), token1.end());
// 	token2.erase(remove_if(token2.begin(), token2.end(), isspace), token2.end());
	//std::cout << token << " " << token1 << " " << token2 << std::endl;
	//finally assign to the Params
	//Params.ChannelModule[j] = atoi(token.c_str());
	Params.ChannelLabel[j] = token;
	Params.ChannelPosition[j] = atoi(token1.c_str());
// 	if(token == "TAG") 
// 	  Params.taggingCrystal = j;
      }
    }
    
    //input files
    for (int j = 0 ; j < 16 ; j++)
    {
      i = s1.find(inputTreeIdentifier[j].str()); //find the info about the channel
      if (i==0)
      {
	//then, tokenize the string
	std::string parametersString,noLeadingSpace;
	parametersString.assign(s1, inputTreeIdentifier[j].str().size() , s1.size());
	noLeadingSpace = parametersString.substr(parametersString.find_first_not_of(" "),parametersString.length()-parametersString.find_first_not_of(" "));
	
	//std::cout << "noLeadingSpace" << std::endl <<  noLeadingSpace << std::endl;
	std::string::size_type pos = noLeadingSpace.find_first_of(" ");
	std::string token = noLeadingSpace.substr(0, pos);
	std::string::size_type pos1 = noLeadingSpace.find_first_of(" ",pos+1);
	std::string token1 = noLeadingSpace.substr(pos+1, pos1-1);
	//std::string::size_type pos2 = noLeadingSpace.find_first_of(" ",pos1+1);
	//std::string token2 = noLeadingSpace.substr(pos1+1, pos2-1);
	//std::cout << token << "---" << token1 /*<< "---" << token2*/ << std::endl;
	token.erase(remove_if(token.begin(), token.end(), isspace), token.end());
	token1.erase(remove_if(token1.begin(), token1.end(), isspace), token1.end());
	//token2.erase(remove_if(token2.begin(), token2.end(), isspace), token2.end());
	//std::cout << token << "\t" << token1 /*<< "\t" << token2*/ << std::endl;
	//finally assign to the Params
	//Params.ChannelModule[j] = atoi(token.c_str());
	Params.zPosition.push_back(atof(token.c_str()));
	//Params.zFileName.push_back(token1.c_str());
	Params.zScaling.push_back(atof(token1.c_str()));
	
	double zmin,zmax;
	zmin = atof(token.c_str()) - 0.1;
	zmax = atof(token.c_str()) + 0.1;
	std::stringstream stream;
	stream << "ZPosition > " << zmin << " && " << "ZPosition < " << zmax;
	
	Params.zCut.push_back(stream.str().c_str());
      }
      
      i = s1.find(ZFileIdentifier[j].str()); //find the info about the channel
      if (i==0)
      {
        std::string tempString;
        tempString.assign(s1, ZFileIdentifier[j].str().size() , s1.size());	
        tempString.erase(remove_if(tempString.begin(), tempString.end(), isspace), tempString.end());
        Params.zFileName.push_back(tempString.c_str());
      }
      
      
    }
    
    //borders
    for (int j = 0 ; j < 3 ; j++)
    {
      i = s1.find(XBorderIdentifier[j].str()); //find the info about the channel
      if (i==0)
      {
        std::string tempString;
        tempString.assign(s1, XBorderIdentifier[j].str().size() , s1.size());	
        tempString.erase(remove_if(tempString.begin(), tempString.end(), isspace), tempString.end());
        Params.XBorder[j] = atof(tempString.c_str());
      }
      
      i = s1.find(YBorderIdentifier[j].str()); //find the info about the channel
      if (i==0)
      {
        std::string tempString;
        tempString.assign(s1, YBorderIdentifier[j].str().size() , s1.size());	
        tempString.erase(remove_if(tempString.begin(), tempString.end(), isspace), tempString.end());
        Params.YBorder[j] = atof(tempString.c_str());
      }
      
      
    }
    
    
    
    
  }
  
  
  //now we can calculate the rescaling for the plots where light is expressed in terms of Ph/MeV (rather then in ADC counts)
  
//   Params.LYhistomax 	= (Params.histomax*Params.AdcChargeBinning)/(Params.SourceEnergy*Params.MppcGain*Params.MppcPDE*1.6*TMath::Power(10,-19));
//   Params.LYbinning	= (int) round((Params.binning*Params.AdcChargeBinning)/(Params.SourceEnergy*Params.MppcGain*Params.MppcPDE*1.6*TMath::Power(10,-19))); 
//   
//   Params.PFhistomax	=  (Params.histomax*Params.AdcChargeBinning)/(Params.MppcGain*1.6*TMath::Power(10,-19));
//   Params.PFbinning	= (int) round((Params.binning*Params.AdcChargeBinning)/(Params.MppcGain*1.6*TMath::Power(10,-19)));


  
  return 0;
}