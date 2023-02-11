#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "TROOT.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TH2.h"
#include "TStyle.h"
#include "TText.h"
#include "TLatex.h" 
//
void Canvas(){
  TPad *pad1;
  TPad *pad2;
  TPad *pad0; 
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
  titleX1="Channel";
  Xname.SetTextSize(0.7);
  Xname.SetTextAlign(42);
  Xname.DrawLatex(0.5,0.5,titleX1);

}
void Canvas_tof(){
  TPad *pad1;
  TPad *pad2;
  TPad *pad0; 
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
