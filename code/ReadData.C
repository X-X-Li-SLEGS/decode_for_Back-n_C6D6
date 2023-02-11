//lxx2021
//2021.05.06

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include "TChain.h"
#include "TObject.h"
#include <math.h>
#include <TMath.h>
#include "algorithm"
#include <iostream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <cstring> 
#include <numeric>
#include <fstream>
#include <TSystem.h>
#include<iostream>
using namespace std;

//int main(){
void ReadData(){

     //------------------Read Data--------------

     Int_t NFile = 6;
     const char* FileName[] = {"C-nat","Empty","Empty+Filter","Pb-nat","Pb+Filter","Yb","Yb+Filter"};
     TFile *fr;
     TTree *T0;
     TH1F  *Peak_T;
     char rootfilename[256];
     Int_t nentries = 0;
     char TName[100];
     Double_t Chn,Counts;
     std::ofstream outfile_Data;
     const Int_t kNNbins=2150;//1150
     for(int i=0; i<NFile; i++){
        sprintf(rootfilename,"../data/%s.root",FileName[i]);
        fr = new TFile(rootfilename);
        Peak_T = (TH1F*)fr->Get("Eng5");
        sprintf(TName,"../data/%s.txt",FileName[i]);
        nentries = Peak_T->GetEntries();
        cout<<nentries<<endl;
        for (Int_t i=0;i<kNNbins;i++) {
            Chn      = Peak_T->GetBinCenter(i);
            Counts   = Peak_T->GetBinContent(i);
            outfile_Data.open(TName,ios::app);
            outfile_Data<<Chn<<" "<<Counts<<endl;
            outfile_Data.close();
        }

     } 

    


    //------------------Finish--------------
     clock_t stop;
     stop = clock(); 
     cout<<"time = "<<stop/CLOCKS_PER_SEC<<"s"<<endl; 
     gSystem->Exit(1);
}
 

