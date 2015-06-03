//function to make flood histograms
//

TH2F* make2Dhisto(Params_t &Params , TChain *chain, TString name, std::string variable, TCut cuts, TString par)
{
  TH2F *histo2D = new TH2F(name,"",500,-7,7,500,-7,7);
  std::stringstream tempStream;
  tempStream << variable << " >> " << name;
  chain->Draw(tempStream.str().c_str(),cuts,par);
  tempStream.str("");
  return histo2D;
}