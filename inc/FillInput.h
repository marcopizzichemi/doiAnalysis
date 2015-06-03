int fillInput(Input_t (&Input)[16],Params_t &Params)
{
  //----------------------------------------------------------//
  //  Fill input struct with params                           //
  //----------------------------------------------------------//
  for(int i=0; i<16; i++)
  {
//     for (int j = 0 ; j < 4 ; j++) //Initialize these data 
//     {
//       Input[i].fit2DmeanX[j]		= 0;
//       Input[i].fit2DmeanY[j]		= 0;
//       Input[i].fit2DsigmaX[j]		= 0;
//       Input[i].fit2DsigmaY[j]		= 0;
//       Input[i].CrystalMean[j] 		= 0;
//       Input[i].CrystalSigma[j] 		= 0;
//       Input[i].ellipseCenterX[j]	= 0;
//       Input[i].ellipseCenterY[j]	= 0;
//       Input[i].ellipseWidthX[j]		= 0;
//       Input[i].ellipseWidthY[j]		= 0;
//     }
    Input[i].DigitizerChannel = i;
    Input[i].LabelName = Params.ChannelLabel[i];
    //int t = (i / 2);
    //Input[i].CanvasCd =  13-t + 2 * (t % 4);			//this way 4x4 canvases start with A1 in bottom left position
    Input[i].CanvasCd = Params.ChannelPosition[i];		//pass the position param to the input struct
    //Input[i].Data = !(i % 2); 				//this way only even channels have data
    Input[i].Data = Params.ChannelOn[i];			//pass the on off param to the input struct
//     Input[i].param0=1;						//Initialize saturation params
//     Input[i].param1=1;
//     if(Params.ChannelModule[i] == 0)
//     {
//       Input[i].Module[0] = true;
//       Input[i].Module[1] = false;
//     }
//     else
//     {
//       Input[i].Module[0] = false;
//       Input[i].Module[1] = true;
//     }
  }
  
  return 0;
}