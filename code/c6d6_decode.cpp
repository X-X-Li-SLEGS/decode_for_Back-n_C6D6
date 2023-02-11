/*******************************************************************************
 * File: decode_without_daq.cc
 * Description: Parse data raw data file and save to root.
 * Comments:
 * Author: GMH(gumh@ihep.ac.cn)
 * Created: 2017/11/06
 *******************************************************************************/

#include "TFile.h"
#include "TTree.h"
#include "DataDef.h"
#include <fstream>
#include "TObject.h"
#include "TClonesArray.h"
#include "TVectorF.h"
#include <math.h>
#include <TMath.h>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
//#include <iomanip>
#include <TSpectrum.h>
#include "TSpectrum2.h"
#include "TRandom.h"
#include "TH1F.h"

using namespace std;

////////////////main///////////////////////////
int main(int argc, char **argv)
{
  const Int_t NumDet=12;          //

  const Int_t BCID[12] = {1, 2, 3, 4, 11, 12, 13, 14, 15, 16, 17, 18};
  
  Int_t i,j,k,n;
  Long64_t m;
//////////////////////////////////////////////////////////////////////////////////////   
    const Int_t Vlen=300000;
    Int_t T0ID;
    Int_t DetID;
    Int_t WLength;
    ULong64_t GPS_s;
    ULong64_t GPS_ns;
    Double_t WTime[Vlen];
    UShort_t WWave[Vlen];
/*  
  TFile *File_Result= TFile::Open(argv[3],"RECREATE");
  TTree *Data_Result= new TTree("Read_Wave","Tree for analysis raw data");

  Data_Result->Branch("T0ID_branch",&T0ID,"T0ID/I");           
  Data_Result->Branch("DetID_branch",&DetID,"DetID/I");  
  Data_Result->Branch("WLength_branch",&WLength,"WLength/I");  
  Data_Result->Branch("GPS_s_branch",&GPS_s,"GPS_s/l");           
  Data_Result->Branch("GPS_ns_branch",&GPS_ns,"GPS_ns/l");           
  Data_Result->Branch("WTime",&WTime,"WTime[WLength]/L");           
  Data_Result->Branch("WWave",&WWave,"WWave[WLength]/s");     
*/  
  Int_t nWaveLens=0;
  Double_t nTimeTag=0;
  UInt_t nT0ID=0;
  Int_t BChn=0;
  Int_t ADCcount=0;
  
//////////////////////////////////////////////////////////////////////////////////	

Int_t ii,jj,kk;

class NeuData
{
public:
  ULong64_t GPS_sec;  
  ULong64_t GPS_nsec;  
  Int_t    T0id;
  Int_t    BCid;
  Double_t  Energy;
  Double_t Tof;
  Float_t    Ph;
  Float_t  PeakValue;
  Double_t PeakPoint;    
} NeuData_1;      

class SiData
{
public:
  ULong64_t GPS_sec;  
  ULong64_t GPS_nsec;  
  Int_t    T0id;
  Int_t    BCid;
  Double_t SiTof;
  Float_t    SiPeakValue;
} SiData_1;  


  TFile* OutFile= new TFile(argv[2],"RECREATE");

  TTree * NeuDataTree=new TTree("NeuDataTree","Tree for record neutron data");
  NeuDataTree->Branch("NeuData_branch",&NeuData_1,"GPS_sec/l:GPS_nsec/l:T0id/I:BCid/I:Energy/D:Tof/D:Ph/F:PeakValue/F:PeakPoint/D"); 	
  
  TTree * SiDataTree=new TTree("SiDataTree","Tree for record LiSi data");
  SiDataTree->Branch("SiData_branch",&SiData_1,"GPS_sec/l:GPS_nsec/l:T0id/I:BCid/I:SiTof/D:SiPeakValue/F");   
  	
  Double_t        tof_gam;
  Double_t        tof_neu;
  Double_t         tof_real;
  Double_t         Eng_neu;
  Double_t         len=77.3;      //fit results
  Double_t         tofgamma=len/0.299;  	
   
   Int_t           ph_short;   
   Float_t         ph;
   Short_t	   peak;
   Int_t	   peak_point;
   Double_t	   peak_time;
   
  Float_t Base;
  
  Int_t first_flag;
  Int_t  sum_pre; 
  Int_t  sum_post; 
  
  Int_t point1=400;
  Int_t point2=2000;
  Int_t EvtC=0;
  
////////////////////////////////////////////////////////////////////////////////////

    ifstream *raw = new ifstream(argv[1]);  
    std::cout<<"File is "<<argv[1]<<" now"<<std::endl;
  
    file_head_t fileHead;
    raw->read((char*)&fileHead, sizeof(fileHead));

    FullFragment ff(20*1024*1024);
    while(raw->good()) {
        FullFragment::head_t *fh = ff.head();
		
        raw->read((char*)fh, sizeof(FullFragment::head_t));
        raw->read((char*)ff.firstFragment(), fh->size-sizeof(FullFragment::head_t));
        if(!raw->good()) {
            break;
        }

        Long64_t  dt = 0;
        {
            // 1. get TCM time
            UChar_t  *ptr = ff.firstFragment();
            for( i=0; i<ff.numberOfSubFragments(); i++) {
                FEECommonFragment feeF(ptr);
                if(feeF.packType() == Fragment::FDM) {
                }
                else if(feeF.packType() == Fragment::TCM) {
                    TCMFragment tcm(ptr);
                    dt = tcm.fixTof();
                }
                ptr += feeF.size();
            }
        }

{
	  for(k=0;k<NumDet;k++){
	        
	    for(j=0;j<Vlen;j++){
	      WTime[j]=0;
	      WWave[j]=0;
	    }
	    ADCcount=0;
            // 2. fill FEM 
            UChar_t  *ptr = ff.firstFragment();
            for(i=0; i<ff.numberOfSubFragments(); i++) {
                FEECommonFragment feeF(ptr);
		
                if(feeF.packType() == Fragment::TCM) {
                    //std::cout << "TCM" << std::endl;
                    TCMFragment tcm(ptr);
		    GPS_s=tcm.t0sec();
		    GPS_ns=tcm.t0ns();
		    		    
                }		
		
                else if(feeF.packType() == Fragment::FDM) {
                    FDMFragment fdm(ptr);
		    
		    BChn = fdm.channel() ;
                    //std::cout<<"BChn= "<<BChn<<std::endl;
		    nT0ID=(fdm.t0id());
		    // std::cout<<"nT0ID= "<<nT0ID<<std::endl;
    		    //nTimeTag=(fdm.tof() + dt)/1000 + 1000;
    		    nTimeTag=(fdm.tof() + dt) + 1200;//?
		     //std::cout<<"fdm.tof()= "<<(fdm.tof())<<std::endl;
		     //std::cout<<"dt= "<<(dt)<<std::endl;
		if(BChn==BCID[k]){
		    nWaveLens=(fdm.adcCount());//?
		   // std::cout<<"nWaveLens= "<<nWaveLens<<std::endl;  
		   // std::cout<<"BChn= "<<BChn<<std::endl;
		
		  for(m=0;m<nWaveLens;m++){
		      WWave[ADCcount]=(fdm.adcValue(m));
		      WTime[ADCcount]=(nTimeTag+m) ;
		      WLength=ADCcount;
		    ADCcount++;
		  }  
		}	    
               }
                ptr += feeF.size();
            }
		T0ID=nT0ID;
		DetID=BCID[k];            
            	//Data_Result->Fill();  
////////////////////////////////////////////////////////////////////////////////////	
Int_t PulseLength;
PulseLength=ADCcount+200;

  Float_t * PulseOrig = new Float_t[PulseLength];  
  Double_t * PulseTime = new Double_t[PulseLength];  
  Float_t * PulseBase = new Float_t[PulseLength];  
  Float_t * PulseTres = new Float_t[PulseLength]; 

if(DetID<5&&ADCcount>0){
  
  
  NeuData_1.GPS_sec=GPS_s;
  NeuData_1.GPS_nsec=GPS_ns;
  NeuData_1.T0id=T0ID;
  NeuData_1.BCid=DetID;
  
      
    for(ii=0;ii<ADCcount;ii++){
      PulseOrig[ii]=float(WWave[ii]);      
      PulseTime[ii]=WTime[ii];
      PulseBase[ii]=0;
      PulseTres[ii]=0;
    }
  Base=0;
  
  for(ii=0;ii<20;ii++){
    Base=Base+WWave[ii];
  }  
  Base=Base/20.0;
  
  first_flag=0;
  	
  for(ii=0;ii<point1;ii++){
    PulseBase[ii]=Base-PulseOrig[ii];
    if(PulseBase[ii]>60){
      PulseTres[ii]=PulseBase[ii];
      if(first_flag==0){
	first_flag=1;
	tof_gam=PulseTime[ii];
      }
    }
  }  
  
  for(ii=point2;ii<ADCcount;ii++){
    PulseBase[ii]=Base-PulseOrig[ii];
    if(PulseBase[ii]>20){
      PulseTres[ii]=PulseBase[ii];
    }
  }
//////////////////////////////////
  for(ii=point2;ii<ADCcount;ii++){
      tof_neu=0.;
      sum_pre=0;
      sum_post=0;
      
	sum_pre=PulseTres[ii-4]+PulseTres[ii-3]+PulseTres[ii-2]+PulseTres[ii-1];
	sum_post=PulseTres[ii]+PulseTres[ii+3]+PulseTres[ii+2]+PulseTres[ii+1];
	if(sum_pre==0&&sum_post>=60){
////////////	  
	  tof_neu=PulseTime[ii];
	  ph_short=0;   
	  ph=0;
	  peak=0;
	  peak_point=0;
	  peak_time=0.;
////////////	  	  
	  for(jj=ii;jj<ii+60;jj++){
	    ph=ph+PulseBase[jj];
	    if(PulseBase[jj]>peak){
	      peak=PulseBase[jj];
	      peak_point=jj;
	      peak_time=PulseTime[jj];
	    }
	  }
/////////////////////////////////////////////////////////////////
      tof_real=tof_neu-tof_gam+tofgamma;
      Eng_neu=(72.2977*len/tof_real)*(72.2977*len/tof_real);

		     //std::cout<<"DetID= "<<DetID<<std::endl;
      		     //std::cout<<"tof_real= "<<tof_real<<std::endl;
      		     //std::cout<<"Eng_neu= "<<Eng_neu<<std::endl;
      NeuData_1.Energy=Eng_neu;
      NeuData_1.Tof=tof_real;
      NeuData_1.Ph=ph;
      NeuData_1.PeakValue=peak;
      NeuData_1.PeakPoint=peak_time;
      
      NeuDataTree->Fill();    
      
      ii=ii+50;
    }
  }  
  //delete PulseOrig;
  //delete PulseTime;
  //delete PulseBase;
  //delete PulseTres;
}
  
///////////////////////////////////////////////
else if(DetID>4&&ADCcount>0){

  SiData_1.GPS_sec=GPS_s;
  SiData_1.GPS_nsec=GPS_ns;
  SiData_1.T0id=T0ID;  
  SiData_1.BCid=DetID;
   
    for(ii=0;ii<ADCcount;ii++){
      PulseOrig[ii]=float(WWave[ii]);      
      PulseTime[ii]=WTime[ii];
      PulseBase[ii]=0;
      PulseTres[ii]=0;
    }
  Base=0;
  
  for(ii=0;ii<40;ii++){
    Base=Base+WWave[ii];
  }  
  Base=Base/40;
 
      		     //std::cout<<"Base= "<<Base<<std::endl;
  
  for(ii=0;ii<ADCcount;ii++){
    PulseBase[ii]=PulseOrig[ii]-Base;
      		     //std::cout<<"PulseBase[ii]= "<<PulseBase[ii]<<std::endl;
    
    if(PulseBase[ii]>50){
      PulseTres[ii]=PulseBase[ii];
    }
  }  
  for(ii=50;ii<ADCcount-200;ii++){
      tof_neu=0;
      sum_pre=0;
      sum_post=0;
      
	sum_pre=PulseTres[ii-4]+PulseTres[ii-3]+PulseTres[ii-2]+PulseTres[ii-1];
	sum_post=PulseTres[ii]+PulseTres[ii+3]+PulseTres[ii+2]+PulseTres[ii+1];
	if(sum_pre==0&&sum_post>=200){
////////////	  
	  tof_neu=PulseTime[ii];
	  ph_short=0;   
	  ph=0;
	  peak=0;
	  peak_point=0;
	  peak_time=0;
////////////	  	  
	  for(jj=ii;jj<ii+1500;jj++){
	    if(PulseBase[jj]>peak){
	      peak=PulseBase[jj];
	      peak_point=jj;
	      peak_time=PulseTime[jj];
	    }
	  }
      		     //std::cout<<"peak= "<<peak<<std::endl;
	  
      SiData_1.SiPeakValue=peak;
      SiData_1.SiTof=peak_time;
        
      SiDataTree->Fill();   
      ii=ii+1500;
    }
  }   
}  
////////////////////////////////////////////////////////////////////////////////////
  delete PulseOrig;
  delete PulseTime;
  delete PulseBase;
  delete PulseTres;
////////////////////////////////////////////////////////////////////////////////////
      }
}
      EvtC++;
      cout<<"\rEvent number:"<<EvtC;
    }
      cout<<endl;
//
NeuDataTree->Write();
SiDataTree->Write();
OutFile->Close();

//Data_Result->Write();
//File_Result->Close();

raw->close();

    return 0;
}
