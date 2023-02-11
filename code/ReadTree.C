
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
} ;      

class SiData
{
public:
  ULong64_t GPS_sec;  
  ULong64_t GPS_nsec;  
  Int_t    T0id;
  Int_t    BCid;
  Double_t SiTof;
  Float_t    SiPeakValue;
} ;

void ReadTree(){
  
  Int_t i,j,k;
  Long64_t jentry=0;
  NeuData NeuData_1; 
  SiData  SiData_1; 
  
  TChain * NeuChain= new TChain("NeuDataTree");
  
  TChain * SiChain= new TChain("SiDataTree");
/*
    char fname[40];
    for(Int_t i=0;i<3;i++)
    {
    sprintf(fname,"./OutTree%d.root",i);
    NeuChain->Add(fname);
    }
*/   
    NeuChain->Add("r2.root");
    SiChain->Add("r2.root");

  NeuChain->SetBranchAddress("NeuData_branch",&NeuData_1);
  SiChain->SetBranchAddress("SiData_branch",&SiData_1);

  Long64_t nentries1 = 0;
  nentries1 = NeuChain->GetEntries();
  cout<<"nentries1 =  "<<nentries1<<endl;

  Long64_t nentries2 = 0;
  nentries2 = SiChain->GetEntries();
  cout<<"nentries2 =  "<<nentries2<<endl;  
/////////////////////////////////////////////////
  const Int_t kNNbins=100;
  Float_t kNEdges[kNNbins+1];
  Float_t kNmax=TMath::Log10(50);
  Float_t kNmin=TMath::Log10(1.0E-07);
  Float_t kNint;
  kNint=(kNmax-kNmin)/kNNbins;
  Float_t kNlog;

  for(i=0;i<kNNbins+1;i++){
    kNlog=kNint*i+kNmin;
    kNEdges[i]=TMath::Power(10,kNlog);
//    cout<<i<<"= "<<kNEdges[i]<<endl;
  }  
  
  const Int_t tNNbins=200;
  Float_t tNEdges[tNNbins+1];
  Float_t tNmax=TMath::Log10(8.0E+07);
  Float_t tNmin=TMath::Log10(1.0E+00);
  Float_t tNint;
  tNint=(tNmax-tNmin)/tNNbins;
  Float_t tNlog;

  for(i=0;i<tNNbins+1;i++){
    tNlog=tNint*i+tNmin;
    tNEdges[i]=TMath::Power(10,tNlog);
  } 

  TH1F* Eng = new TH1F("Eng","Eng",kNNbins,kNEdges); 
  TH1F* Tof = new TH1F("Tof","Tof",tNNbins,tNEdges);
  TH1F* Ph = new TH1F("Ph","Ph",4000,0,120000);  
  TH1F* Peak = new TH1F("Peak","Peak",4096,0,4096);  
  
  TH1F* SiPeak = new TH1F("SiPeak","SiPeak",4096,0,4096);  
  TH1F* SiTof = new TH1F("SiTof","SiTof",tNNbins,tNEdges);

/////////////////////////////////////////////////
      NeuChain->GetEntry(0); 
	  cout<<"start time: "<<NeuData_1.GPS_sec<<endl;
      NeuChain->GetEntry(nentries1-1); 
	  cout<<"stop time: "<<NeuData_1.GPS_sec<<endl;


  for (jentry=0; jentry<nentries1;jentry++) 
    {
      NeuChain->GetEntry(jentry); 
	
      if(NeuData_1.BCid==1){
      Eng->Fill(NeuData_1.Energy);
      Tof->Fill(NeuData_1.Tof);
      Ph->Fill(NeuData_1.Ph);
      Peak->Fill(NeuData_1.PeakValue);
      }  
    }

  for (jentry=0; jentry<nentries2;jentry++) 
    {
      SiChain->GetEntry(jentry); 
	
      if(SiData_1.BCid==6){
      SiTof->Fill(SiData_1.SiTof);
      SiPeak->Fill(SiData_1.SiPeakValue);
      }  
    }
    
    
  TCanvas *c1 = new TCanvas("c1","c1",900,500);
  c1->Divide(3,2);
  c1->cd(1);
  Eng->GetXaxis()->SetTitle("Energy / MeV");
  Eng->GetYaxis()->SetTitle("Count ");
  Eng->Draw();

  c1->cd(2);
  Tof->Draw();
  c1->cd(3);
  Ph->Draw();  
  c1->cd(4);
  SiTof->Draw();
  c1->cd(5);
  Peak->Draw();
  c1->cd(6);
  SiPeak->Draw();



}
