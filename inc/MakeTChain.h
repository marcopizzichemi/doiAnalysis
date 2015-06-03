int makeTChain(Params_t &Params, TChain *chain)
{
  for(int i = 0 ; i < Params.zFileName.size() ; i++)
  {
    chain->Add(Params.zFileName[i].c_str());
  }
  
  return 0;
}