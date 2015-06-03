int find2dPeaks(Crystal_t (&Crystal)[64],Params_t &Params , TH2F *FloodPEtagCry)
{

  TSpectrum2 *peak2d = new TSpectrum2(16);
  int nfound = peak2d->Search(FloodPEtagCry,2,"col",0.08);
  
  //store what has been found in the params struct
  Params.xpeaks = peak2d->GetPositionX();
  Params.ypeaks = peak2d->GetPositionY();
  Float_t tempXsigma[16] = {0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2};
  Float_t tempYsigma[16] = {0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2};
  //temporary sigmas
  Params.xsigma = &tempXsigma[0];
  Params.ysigma = &tempYsigma[0];
  
  //2d fit
  for ( int j = 0 ; j < nfound ; j++)
  {
    
    TF2 *f2 = new TF2("f2","[0]*TMath::Gaus(x,[1],[2])*TMath::Gaus(y,[3],[4])",Params.xpeaks[j]-2.0*Params.xsigma[j],Params.xpeaks[j]+2.0*Params.xsigma[j],Params.ypeaks[j]-2.0*Params.ysigma[j],Params.ypeaks[j]+2.0*Params.ysigma[j]);
    f2->SetParameters(14,Params.xpeaks[j],Params.xsigma[j],Params.ypeaks[j],Params.ysigma[j]);
    FloodPEtagCry->Fit("f2","QNR");
    Params.fit2DmeanX[j] = Params.xpeaks[j];
    Params.fit2DmeanY[j] = Params.ypeaks[j];
    //we insert the 2 sigma limit here, it makes things easier
    Params.fit2DsigmaX[j] = 2.5*std::abs(f2->GetParameter(2));
    Params.fit2DsigmaY[j] = 2.5*std::abs(f2->GetParameter(4));
  }
  
//   now find a way to avoid cuts to intersect
//   one possible way: - TODO this is very dirty and can be done in a much more general way
//   first, forget about the ellipes and use circles...
  for ( int j = 0 ; j < nfound ; j++)
  {
    float sx,sy;
    sx = Params.fit2DsigmaX[j];
    sy = Params.fit2DsigmaY[j];
    Params.fit2DsigmaX[j] = (sx+sy)/2.0;
    Params.fit2DsigmaY[j] = (sx+sy)/2.0;
  }
//   now for each peak, check if any other peak is closer than the sum of the relative circles
  
  
  for ( int j = 0 ; j < nfound ; j++)// run on all peaks
  {
    for ( int jOther = 0 ; jOther < nfound ; jOther++)//run on all peaks again, but...
    {
      if (jOther != j) //...do it only if it's not the same peak
      {
	float distance = TMath::Sqrt( TMath::Power(Params.fit2DmeanX[j]-Params.fit2DmeanX[jOther],2) + TMath::Power(Params.fit2DmeanY[j]-Params.fit2DmeanY[jOther],2) );
	float sumOfRadii = (Params.fit2DsigmaX[j]+Params.fit2DsigmaX[jOther]);
	if ( distance < sumOfRadii )
	{
	  //std::cout << "WARNING: Peaks of Module " << k << " Channel " << i << " are overlapping!" << std::endl;
	  Params.fit2DsigmaX[j] = distance * ( Params.fit2DsigmaX[j] / sumOfRadii );
	  Params.fit2DsigmaX[jOther] = distance * ( Params.fit2DsigmaX[jOther] / sumOfRadii );
	  Params.fit2DsigmaY[j] = Params.fit2DsigmaX[j];
	  Params.fit2DsigmaY[jOther] = Params.fit2DsigmaX[jOther];
	}
      }
    }
  }
  
  
  //for the moment, trivial
  //std::cout << "Fit values" << std::endl;
  for ( int j = 0 ; j < nfound ; j++)
  {
    Params.ellipseCenterX[j] = Params.fit2DmeanX[j];
    Params.ellipseCenterY[j] = Params.fit2DmeanY[j];
    Params.ellipseWidthX[j] = Params.fit2DsigmaX[j];
    Params.ellipseWidthY[j] = Params.fit2DsigmaY[j];
    TEllipse *ellipses;
    ellipses = new TEllipse(Params.ellipseCenterX[j],Params.ellipseCenterY[j],Params.ellipseWidthX[j],Params.ellipseWidthY[j]);
    //ellipses->SetFillColor(42);
    ellipses->SetFillStyle(4001);
    ellipses->SetLineColor(kRed);
    ellipses->SetLineWidth(2);
    ellipses->Draw();
    //std::cout  <<  Input[i].fit2DmeanX[j] << " " << Input[i].fit2DmeanY[j ]<< " " << Input[i].fit2DsigmaX[j] << " " << Input[i].fit2DsigmaY[j] << std::endl;
  }
  
  //
  
  
  
  //write TCuts for the crystals in the flood map
  for ( int j = 0 ; j < nfound ; j++)
  {
    Crystal[j].Data = true;
    
    std::stringstream tempStream;
    tempStream << "(( TMath::Power(FloodX - " <<
    Params.ellipseCenterX[j] <<
    ",2) / TMath::Power(" <<
    Params.ellipseWidthX[j]  <<
    ",2))  + ( TMath::Power(FloodY - " <<
    Params.ellipseCenterY[j] <<
    ",2) / TMath::Power(" <<
    Params.ellipseWidthY[j]  <<
    ",2))) < 1";
    Crystal[j].crystalCut = tempStream.str().c_str();
    
    
    
    //assing a crystal mppc - this is really fucked up coding...
    
    if(Params.ellipseCenterX[j] < Params.XBorder[0])
    {
      if(Params.ellipseCenterY[j] < Params.YBorder[0])
      {
	Crystal[j].crystalMPPC = 3;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[0] && Params.ellipseCenterY[j] < Params.YBorder[1])
      {
	Crystal[j].crystalMPPC = 2;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[1] && Params.ellipseCenterY[j] < Params.YBorder[2])
      {
	Crystal[j].crystalMPPC = 1;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[2])
      {
	Crystal[j].crystalMPPC = 0;
      }
    }
    if(Params.ellipseCenterX[j] > Params.XBorder[0] && Params.ellipseCenterX[j] < Params.XBorder[1])
    {
      if(Params.ellipseCenterY[j] < Params.YBorder[0])
      {
	Crystal[j].crystalMPPC = 7;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[0] && Params.ellipseCenterY[j] < Params.YBorder[1])
      {
	Crystal[j].crystalMPPC = 6;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[1] && Params.ellipseCenterY[j] < Params.YBorder[2])
      {
	Crystal[j].crystalMPPC = 5;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[2])
      {
	Crystal[j].crystalMPPC = 4;
      }
    }
    if(Params.ellipseCenterX[j] > Params.XBorder[1] && Params.ellipseCenterX[j] < Params.XBorder[2])
    {
      if(Params.ellipseCenterY[j] < Params.YBorder[0])
      {
	Crystal[j].crystalMPPC = 11;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[0] && Params.ellipseCenterY[j] < Params.YBorder[1])
      {
	Crystal[j].crystalMPPC = 10;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[1] && Params.ellipseCenterY[j] < Params.YBorder[2])
      {
	Crystal[j].crystalMPPC = 9;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[2])
      {
	Crystal[j].crystalMPPC = 8;
      }
    }
    if(Params.ellipseCenterX[j] > Params.XBorder[2])
    {
      if(Params.ellipseCenterY[j] < Params.YBorder[0])
      {
	Crystal[j].crystalMPPC = 15;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[0] && Params.ellipseCenterY[j] < Params.YBorder[1])
      {
	Crystal[j].crystalMPPC = 14;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[1] && Params.ellipseCenterY[j] < Params.YBorder[2])
      {
	Crystal[j].crystalMPPC = 13;
      }
      if(Params.ellipseCenterY[j] > Params.YBorder[2])
      {
	Crystal[j].crystalMPPC = 12;
      }
    }
    std::cout << Params.ellipseCenterX[j] << "\t" << Params.ellipseCenterY[j] << "\t" << Crystal[j].crystalMPPC << std::endl;
  }
  
  
  
  return nfound;
}