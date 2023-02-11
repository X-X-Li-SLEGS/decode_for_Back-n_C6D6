#define N 2000
  TPad *pad1;
  TPad *pad2;
  TPad *pad0; 
  char fname[100];  
   TH1F* Peak[4];
   TH1F* Eng[5];
   TH1F* Eng2[5];
   TH1F* Tof[5];
   TH1F* Tof2[5];
   TH1F* PPoint[4];
   TH1F* PH[4];
   TH1F* SiPeak[4];  
   TH1F* SiTof[4];
   int NBCid1,NBCid2;
class C6D6Data
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
} ;      

class LiSiData
{
public:
  ULong64_t GPS_sec;  
  ULong64_t GPS_nsec;  
  Int_t    T0id;
  Int_t    BCid;
  Double_t SiTof;
  Float_t    SiPeakValue;
} ;

//===============================================================
///////////////////////////////////////////////////////////////////
void Canvas(){
  //Canv=new TCanvas("Canv","Fit IMF",900,600);
  TCanvas *c1 = new TCanvas("c1","c1",1000,800);  
  gStyle->SetFrameFillColor(10);
  gStyle->SetStatColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadColor(10);
  gStyle->SetPadBorderMode(0);
  gStyle->SetDrawBorder(0);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.05);
  gStyle->SetPadLeftMargin(0.08);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetOptStat(0); 
  gStyle->SetStatColor(0);
  //gStyle->SetTitleColor(0);
 
  pad1 = new TPad("pad1","pad1",0.0,0.0,1.,0.06);
  //pad1->SetFillColor(10);
  pad0 = new TPad("pad0","pad0",0.0,0.0,0.04,1.);
  //pad0->SetFillColor(10);
  pad2 = new TPad("pad2","pad2",0.04,0.06,1.,1.);
  //pad2 = new TPad("pad2","pad2",0.0,0.0,1.,1.);
  //pad2->SetFillColor(10);

  pad0->Draw();
  pad1->Draw();
  pad2->Draw();

  TString titleY1,titleY2,titleY3,titleX1,titleX2,titleX3;
  TLatex Yname,Xname; 

  pad0->cd();
  titleY2="Counts";
  Yname.SetTextSize(0.8);
  Yname.SetTextAlign(42);
  Yname.SetTextAngle(90.);
  Yname.DrawLatex(0.5,0.5,titleY2);
  
  pad1->cd();
  titleX1="Energy";
  Xname.SetTextSize(0.7);
  Xname.SetTextAlign(42);
  Xname.DrawLatex(0.5,0.5,titleX1);

}
void Canvas_tof(){
  //Canv=new TCanvas("Canv","Fit IMF",900,600);
  TCanvas *c1 = new TCanvas("c2","c2",1000,800);  
  gStyle->SetFrameFillColor(10);
  gStyle->SetStatColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetPadColor(10);
  gStyle->SetPadBorderMode(0);
  gStyle->SetDrawBorder(0);
  gStyle->SetOptFit(kFALSE);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetPadBottomMargin(0.05);
  gStyle->SetPadLeftMargin(0.08);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetOptStat(0); 
  gStyle->SetStatColor(0);
  //gStyle->SetTitleColor(0);
 
  pad1 = new TPad("pad1","pad1",0.0,0.0,1.,0.06);
  //pad1->SetFillColor(10);
  pad0 = new TPad("pad0","pad0",0.0,0.0,0.04,1.);
  //pad0->SetFillColor(10);
  pad2 = new TPad("pad2","pad2",0.04,0.06,1.,1.);
  //pad2 = new TPad("pad2","pad2",0.0,0.0,1.,1.);
  //pad2->SetFillColor(10);

  pad0->Draw();
  pad1->Draw();
  pad2->Draw();

  TString titleY1,titleY2,titleY3,titleX1,titleX2,titleX3;
  TLatex Yname,Xname; 

  pad0->cd();
  titleY2="Counts";
  Yname.SetTextSize(0.8);
  Yname.SetTextAlign(42);
  Yname.SetTextAngle(90.);
  Yname.DrawLatex(0.5,0.5,titleY2);
  
  pad1->cd();
  titleX1="Tof";
  Xname.SetTextSize(0.7);
  Xname.SetTextAlign(42);
  Xname.DrawLatex(0.5,0.5,titleX1);

}
////////////////////////////////////////////////
 void ReadData(){
  Long64_t C6D6_nentries = 0;
  Long64_t LiSi_nentries = 0;
  TChain * C6D6chain;
  TChain * LiSichain;
  C6D6Data C6D6Data_1;
  LiSiData LiSiData_1; 
  char fname[100];

  Long64_t C6D6_nentries2 = 0;
  Long64_t LiSi_nentries2 = 0;
  TChain * C6D6chain2;
  TChain * LiSichain2;
  C6D6Data C6D6Data_2;
  LiSiData LiSiData_2; 
  char fname2[100];

//------------------------------------------------------------------------------
  C6D6chain= new TChain("NeuDataTree");
  LiSichain= new TChain("SiDataTree");
  if(C6D6chain==0) return;
  if(LiSichain==0) return;
for(int i=1;i<=1;i++)
    {
   sprintf(fname,"../data/daq-17780-NORM-%05d.root",i);

   C6D6chain->Add(fname); 
   LiSichain->Add(fname);
  } 
   C6D6chain->SetBranchAddress("NeuData_branch",&C6D6Data_1);
   LiSichain->SetBranchAddress("SiData_branch",&LiSiData_1);

//

  C6D6chain2= new TChain("NeuDataTree");
  LiSichain2= new TChain("SiDataTree");
  if(C6D6chain2==0) return;
  if(LiSichain2==0) return;
for(int i=1;i<=1;i++)
    {
   sprintf(fname2,"../data/daq-17772-NORM-%05d.root",i);

   C6D6chain2->Add(fname2); 
   LiSichain2->Add(fname2);
  } 
   C6D6chain2->SetBranchAddress("NeuData_branch",&C6D6Data_2);
   LiSichain2->SetBranchAddress("SiData_branch",&LiSiData_2);
   
//=============================================================
   
   C6D6_nentries = C6D6chain->GetEntries();
   cout<<"C6D6_nentries =  "<<C6D6_nentries<<endl;
   LiSi_nentries = LiSichain->GetEntries();
   cout<<"LiSi_nentries =  "<<LiSi_nentries<<endl;
//
   C6D6_nentries2 = C6D6chain2->GetEntries();
   cout<<"C6D6_nentries =  "<<C6D6_nentries2<<endl;
   LiSi_nentries2 = LiSichain2->GetEntries();
   cout<<"LiSi_nentries =  "<<LiSi_nentries2<<endl;
//============================================================

  const Int_t kNNbins=2150;//1150
  Double_t kNEdges[kNNbins+1];
  Double_t kNmax=TMath::Log10(1.0E+01);
  Double_t kNmin=TMath::Log10(1.0E-08);
  Double_t kNint;
  kNint=(kNmax-kNmin)/kNNbins;
  Float_t kNlog;

  for(int i=0;i<kNNbins+1;i++){
    kNlog=kNint*i+kNmin;
    kNEdges[i]=TMath::Power(10,kNlog);
//    cout<<i<<"= "<<kNEdges[i]<<endl;
  } 
 const Int_t tNNbins=800;
  Float_t tNEdges[tNNbins+1];
  Float_t tNmax=TMath::Log10(3.0E+07);
  Float_t tNmin=TMath::Log10(1.0E+03);
  Float_t tNint;
  tNint=(tNmax-tNmin)/tNNbins;
  Float_t tNlog;

  for(int i=0;i<tNNbins+1;i++){
    tNlog=tNint*i+tNmin;
    tNEdges[i]=TMath::Power(10,tNlog);
  } 

 
   Eng[0] = new TH1F("Eng", "Engery-Det.1",kNNbins,kNEdges); 
   Eng[1] = new TH1F("Eng2","Engery-Det.2",kNNbins,kNEdges); 
   Eng[2] = new TH1F("Eng3","Engery-Det.3",kNNbins,kNEdges); 
   Eng[3] = new TH1F("Eng4","Engery-Det.4",kNNbins,kNEdges);
   Eng[4] = new TH1F("Eng5","Engery-Det.Tot",kNNbins,kNEdges);

   Eng2[0] = new TH1F("Eng", "Engery-Det.1",kNNbins,kNEdges); 
   Eng2[1] = new TH1F("Eng2","Engery-Det.2",kNNbins,kNEdges); 
   Eng2[2] = new TH1F("Eng3","Engery-Det.3",kNNbins,kNEdges); 
   Eng2[3] = new TH1F("Eng4","Engery-Det.4",kNNbins,kNEdges);
   Eng2[4] = new TH1F("Eng5","Engery-Det.Tot",kNNbins,kNEdges);
  
   Tof[0] = new TH1F("Tof", "Tof",tNNbins,tNEdges);
   Tof[1] = new TH1F("Tof2","Tof2",tNNbins,tNEdges);
   Tof[2] = new TH1F("Tof3","Tof3",tNNbins,tNEdges);
   Tof[3] = new TH1F("Tof4","Tof4",tNNbins,tNEdges);
   Tof[4] = new TH1F("Tof5","Tof5",tNNbins,tNEdges);


   Tof2[0] = new TH1F("Tof", "Tof",tNNbins,tNEdges);
   Tof2[1] = new TH1F("Tof2","Tof2",tNNbins,tNEdges);
   Tof2[2] = new TH1F("Tof3","Tof3",tNNbins,tNEdges);
   Tof2[3] = new TH1F("Tof4","Tof4",tNNbins,tNEdges);
   Tof2[4] = new TH1F("Tof5","Tof5",tNNbins,tNEdges);

   PH[0] = new TH1F("PH", "PH",4000,0,120000); 
   PH[1] = new TH1F("PH2","PH2",4000,0,120000);  
   PH[2] = new TH1F("PH3","PH3",4000,0,120000);  
   PH[3] = new TH1F("PH4","PH4",4000,0,120000);   

   Peak[0] = new TH1F("Peak", "Peak",2048,0,4096);  
   Peak[1] = new TH1F("Peak2","Peak2",2048,0,4096);  
   Peak[2] = new TH1F("Peak3","Peak3",2048,0,4096);  
   Peak[3] = new TH1F("Peak4","Peak4",2048,0,4096); 
  
   PPoint[0] = new TH1F("PPoint", "PPoint",tNNbins,tNEdges); 
   PPoint[1] = new TH1F("PPoint2","PPoint2",tNNbins,tNEdges); 
   PPoint[2] = new TH1F("PPoint3","PPoint3",tNNbins,tNEdges); 
   PPoint[3] = new TH1F("PPoint4","PPoint4",tNNbins,tNEdges); 
 
   SiPeak[0] = new TH1F("LiSiPeak","LiSiPeak",4096,0,4096);   
   SiPeak[1] = new TH1F("LiSiPeak2","LiSiPeak2",4096,0,4096);
    
   SiTof[0] = new TH1F("LiSiTof","LiSiTof",tNNbins,tNEdges);
   SiTof[1] = new TH1F("LiSiTof2","LiSiTof2",tNNbins,tNEdges);
   
  
//C6D6Data_1	
for (Long64_t jentry=0; jentry<C6D6_nentries;jentry++) 
    {
      C6D6chain->GetEntry(jentry); 
     if(C6D6Data_1.BCid==1)
     {
      Eng[0]->Fill(C6D6Data_1.Energy);
      Tof[0]->Fill(C6D6Data_1.Tof);
      PH[0]->Fill(C6D6Data_1.Ph);
      Peak[0]->Fill(C6D6Data_1.PeakValue);
      PPoint[0]->Fill(C6D6Data_1.PeakPoint);
      
      }
      else if(C6D6Data_1.BCid==2){
      Eng[1]->Fill(C6D6Data_1.Energy);
      Tof[1]->Fill(C6D6Data_1.Tof);
      PH[1]->Fill(C6D6Data_1.Ph);
      Peak[1]->Fill(C6D6Data_1.PeakValue);
      PPoint[1]->Fill(C6D6Data_1.PeakPoint);
	   
      }
      else if(C6D6Data_1.BCid==3){
      Eng[2]->Fill(C6D6Data_1.Energy);
      Tof[2]->Fill(C6D6Data_1.Tof);
      PH[2]->Fill(C6D6Data_1.Ph);
      Peak[2]->Fill(C6D6Data_1.PeakValue);
      PPoint[2]->Fill(C6D6Data_1.PeakPoint);	   
      }
      else if(C6D6Data_1.BCid==4){
      Eng[3]->Fill(C6D6Data_1.Energy);
      Tof[3]->Fill(C6D6Data_1.Tof);
      PH[3]->Fill(C6D6Data_1.Ph);
      Peak[3]->Fill(C6D6Data_1.PeakValue);
      PPoint[3]->Fill(C6D6Data_1.PeakPoint);  
      }  
     Eng[4]->Fill(C6D6Data_1.Energy);
     Tof[4]->Fill(C6D6Data_1.Tof);
  }

//LiSiData_2
 NBCid1 = 0;
 for (Long64_t jentry=0; jentry<LiSi_nentries;jentry++) 
    {
      LiSichain->GetEntry(jentry); 
     if(LiSiData_1.BCid==11 || LiSiData_1.BCid==12 || LiSiData_1.BCid==13 || LiSiData_1.BCid==11 || LiSiData_1.BCid==15 || LiSiData_1.BCid==16 || LiSiData_1.BCid==17 || LiSiData_1.BCid==18)
      {
        NBCid1 = NBCid1+1;
       }
  }
cout<<"BCid1 = "<<NBCid1<<endl;

//C6D6Data_2	
for (Long64_t jentry=0; jentry<C6D6_nentries;jentry++) 
    {
      C6D6chain2->GetEntry(jentry); 
     if(C6D6Data_2.BCid==1)
     {
      Tof2[0]->Fill(C6D6Data_2.Tof);
      Eng2[0]->Fill(C6D6Data_2.Energy);
      }
      else if(C6D6Data_2.BCid==2){
      Tof2[1]->Fill(C6D6Data_2.Tof); 
      Eng2[1]->Fill(C6D6Data_2.Energy);
      }
      else if(C6D6Data_2.BCid==3){
      Tof2[2]->Fill(C6D6Data_2.Tof); 
      Eng2[2]->Fill(C6D6Data_2.Energy);	   
      }
      else if(C6D6Data_2.BCid==4){
      Tof2[3]->Fill(C6D6Data_2.Tof); 
      Eng2[3]->Fill(C6D6Data_2.Energy);
      }  
    Eng2[4]->Fill(C6D6Data_2.Energy);
    Tof2[4]->Fill(C6D6Data_2.Tof);
  }

//LiSiData_2
 NBCid2 = 0;
 for (Long64_t jentry=0; jentry<LiSi_nentries2;jentry++) 
    {
      LiSichain2->GetEntry(jentry); 
     if(LiSiData_2.BCid==11 || LiSiData_2.BCid==12 || LiSiData_2.BCid==13 || LiSiData_2.BCid==11 || LiSiData_2.BCid==15 || LiSiData_2.BCid==16 || LiSiData_2.BCid==17 || LiSiData_2.BCid==18)
      {
        NBCid2 = NBCid2+1;
       }
  }
cout<<"BCid2 = "<<NBCid2<<endl;

//
}

//main
void Plot(){
/*
Canvas_tof();
pad2->cd();
pad2->Clear();
pad2->SetLogy();
pad2->SetLogx();
ReadData();
//Draw

 Tof[4]->Draw();
 //Tof[1]->Draw("same");
 //Tof[2]->Draw("same");
 //Tof[3]->Draw("same");

 Tof2[4]->Draw("same");
 //Tof2[1]->Draw("same");
 //Tof2[2]->Draw("same");
 //Tof2[3]->Draw("same");

 Tof[4]->SetLineColor(1);
 Tof2[4]->SetLineColor(2);
*/
/*
 for(int num=0;num<4;num++){
 Tof[num]->SetLineColor(num+1);  
 }
*/

Canvas(); 
pad2->cd();
pad2->Clear();
pad2->SetLogy();
pad2->SetLogx();
ReadData();
  Eng[4]->Scale(1./NBCid1);
  Eng2[4]->Scale(1./NBCid2);
  Eng[4]->Draw("EHIST");
  Eng2[4]->Draw("EHISTsame");
  Eng[4]->SetLineColor(1);
  Eng2[4]->SetLineColor(2);

  TLegend* legend1=new TLegend(0.15,0.1,0.22,0.45);
  legend1->SetTextFont(10);
  legend1->SetFillColor(10);
  legend1->SetTextSize(0.03);
  legend1->SetBorderSize(0);
  legend1->AddEntry(Eng[4],"Yb-nat","L");
  legend1->AddEntry(Eng2[4],"Empty","L");
  //legend1->AddEntry(Eng3[4],"no beam","L");
  //legend1->AddEntry(Eng4[4],"nat-Pb","L");
  //legend1->AddEntry(Tof[4],"Yb-nat","L");
  //legend1->AddEntry(Tof2[4],"Yb-nat + Filter","L");
  legend1->Draw();

     char fileout[256];
     sprintf(fileout,"../Yb.root");
     TFile *fout = new TFile(fileout,"recreate");
     Eng[4]->Write();
     fout->Close();
/*
    char fileout2[256];
     sprintf(fileout2,"../C-nat.root");
     TFile *fout2 = new TFile(fileout2,"recreate");
     Eng2[0]->Write();
     fout2->Close();
 */

//
}      
//===================================================================

 

