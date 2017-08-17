//simple analysis

// compile with
// g++ -o ../build/doifit doifit.cpp `root-config --cflags --glibs`


#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TH1F.h"
#include "TChain.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "TMath.h"

struct Crystal_t
{
  std::vector<int> x;
  std::vector<int> y;
  std::vector<double> w;
  std::vector<double> z;
  std::vector<double> s;
  std::vector<double> sqrt_nentries;
  TCanvas *Cplot;
  TGraphErrors *doiR;
  double m,q,avgs,doires;
} Crystal[8];


class point_t
{
public:
  int x;
  int y;
  double w;
  double z;
  double s;
  double sqrt_nentries;

  friend std::istream& operator>>(std::istream& input, point_t& s)
  {
    input >> s.x;
    input >> s.y;
    input >> s.w;
    input >> s.z;
    input >> s.s;
    input >> s.sqrt_nentries;
    return input;
  }
};





int main (int argc, char** argv)
{
  std::vector<std::string> file;
  for(int i = 1 ; i < argc ; i++)
  {
    std::stringstream inputStream;
    inputStream << "./" <<  argv[i] << "/doiData.txt";
    file.push_back(inputStream.str());
  }

  std::ifstream data[file.size()];
  int x,y;
  double w,z,s,sqrt_nentries;
  std::vector<int> counter;

  for(int i = 0 ; i < file.size() ; i++)
  {
    data[i].open(file[i].c_str());
    int fileCounter = 0;
    point_t pointData;
    while(data[i] >> pointData)
    {
      Crystal[fileCounter].x.push_back(pointData.x);
      Crystal[fileCounter].y.push_back(pointData.y);
      Crystal[fileCounter].w.push_back(pointData.w);
      Crystal[fileCounter].z.push_back(pointData.z);
      Crystal[fileCounter].s.push_back(pointData.s);
      Crystal[fileCounter].sqrt_nentries.push_back(pointData.sqrt_nentries);
      fileCounter++;
    }
    counter.push_back(fileCounter);
  }

  const int NumOfCrystals = counter[0];
  for(int i = 1 ; i < counter.size() ; i++)
  {
    if (counter[i] != NumOfCrystals)
    {
      std::cout << "ERROR: doiData.txt files have different number of rows!" << std::endl;
      std::cout << "Aborting" << std::endl;
      return -1;
    }
  }




  //calc average sigma w peak (multiply first by sqrt_nentries)
  for(int i = 0 ; i < NumOfCrystals ; i++)
  {
    double partial = 0;
    for(int j = 0 ; j < Crystal[i].s.size(); j++)
    {
      partial += Crystal[i].s[j] * Crystal[i].sqrt_nentries[j];
    }
    Crystal[i].avgs = partial/Crystal[i].s.size();
  }


  std::stringstream tempStream;

  for(int j = 0 ; j < NumOfCrystals ; j++)
  {
    std::cout << "Correlation Plot" << std::endl;
    tempStream.str("");
    tempStream << "Canvas_DOIvsR_" << Crystal[j].x[0] << "_" << Crystal[j].y[0];

    Crystal[j].Cplot = new TCanvas(tempStream.str().c_str(),"",800,600);
    tempStream << ".png";
    std::string plotFile = tempStream.str().c_str();

    std::vector<double> tempEy,tempEx;
    for(int tt = 0 ; tt < file.size()  ;tt++)
    {
      tempEy.push_back(0.5);
//       tempEx.push_back( Crystal[tt].s[j]);
    }

    //Crystal[j].doiR = new TGraphErrors(5,&Crystal[j].x[0],&Crystal[j].y[0],&Crystal[j].ex[0],&Crystal[j].ey[0]);
    Crystal[j].doiR = new TGraphErrors(file.size(),&Crystal[j].w[0],&Crystal[j].z[0],&Crystal[j].s[0],&tempEy[0]);
    Crystal[j].doiR->GetXaxis()->SetTitle("W");
    Crystal[j].doiR->GetYaxis()->SetTitle("DOI [mm]");
    tempStream.str("");
    tempStream << "DOI vs. R - Crystal " << j ;
    Crystal[j].doiR->SetTitle("");
    Crystal[j].Cplot->cd();
    Crystal[j].doiR->Draw("A*");

    TF1 *fit = new TF1("fit", "[0]+x*[1]",0,1);
    fit->SetParameter(0,80);
    fit->SetParameter(1,-100);
    Crystal[j].doiR->Fit("fit","QR");
    fit->SetLineColor(2);
    Crystal[j].Cplot->cd();
    fit->Draw("same");

    Crystal[j].Cplot->Print(plotFile.c_str());
//     Crystal[j].fitPar0 = fit->GetParameter(0);
//     Crystal[j].fitPar1 = fit->GetParameter(1);
    Crystal[j].m = fit->GetParameter(1);
    Crystal[j].q = fit->GetParameter(0);
    Crystal[j].doires = std::abs(fit->GetParameter(1) * Crystal[j].avgs *2.355);
    std::cout << "Crystal " << j << " = " << fit->GetParameter(1) << " , " << Crystal[j].doires<< std::endl;



  }

  std::ofstream calib;
  calib.open("calibration_params.txt");

  for(int j = NumOfCrystals - 1 ; j > -1 ; j--)
  {
    calib << Crystal[j].x[0] << "\t"
          << Crystal[j].y[0] << "\t"
	  << Crystal[j].m << "\t"
	  << Crystal[j].q << "\t"
	  << Crystal[j].doires << "\t"
	  << Crystal[j].avgs << "\t";
    for(int i = 0 ; i < Crystal[j].w.size(); i++)
      calib << Crystal[j].w[i] << "\t"<< Crystal[j].s[i] << "\t" <<  Crystal[j].sqrt_nentries[i] << "\t";
    calib << std::endl;
  }
  calib.close();


  std::ofstream doi;
  doi.open("doi.txt");
  for(int j = NumOfCrystals -1 ; j > -1 ; j--)
  {
    doi << Crystal[j].x[0] << "\t" << Crystal[j].y[0] << "\t" ;
    doi << Crystal[j].doires << "\t" << Crystal[j].avgs <<std::endl;
  }
  doi.close();
  return 0;
}
