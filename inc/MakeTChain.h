int makeTChain(Params_t &Params, TChain *chain)
{
//   chain->Add(Params.zFileName[0].c_str());
  
  for(int i = 0 ; i < Params.zFileName.size() ; i++)
  {
    chain->Add(Params.zFileName[i].c_str());
  }
  return 0;
}