//// plot_combine_limits: Plots various limit curves on same canvas

// System includes
#include <fstream>
#include <iostream>
#include <ctime>

// ROOT includes
#include "TStyle.h"
#include "TLatex.h"
#include "TFile.h"

// User includes
#include "plot_combined_limits.hpp"

using namespace std;

int main(){
  time_t begtime, endtime;
  time(&begtime);

  TString folder("root/limits_2015/");
  TString lsp("#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}");

  vector<model_limits> models;
  models.push_back(model_limits("T1tttt", "#tilde{g}#kern[0.2]{#tilde{g}}, #tilde{g}#rightarrowt#bar{t}"+lsp));
  models.back().add("SUS-15-007, 1-lep (M_{J}), 2.1 fb^{-1}", folder+"t1tttt_sus15_007.root", 
		    "graph_smoothed_Obs", "graph_smoothed_Exp", kRed);
  models.back().add("SUS-15-002, 0-lep (MHT), 2.2 fb^{-1}", folder+"t1tttt_sus15_002.root", 
		    "ObsLim", "ExpLim", kBlue);

  // Creating canvas
  gStyle->SetOptStat(0);  
  float lMargin(0.14), tMargin(0.08), rMargin(0.02), bMargin(0.14);
  TCanvas can("canvas","", 600, 600);
  setCanvas(can, lMargin, tMargin, rMargin, bMargin);

  // Looping over each model
  for(size_t imodel(0); imodel < models.size(); imodel++){
    model_limits mod(models[imodel]);

    // Creating base histogram and drawing lumi labels
    float Xmin(700), Xmax(1950), Ymin(0), Ymax(1800);
    TH2D hbase = baseHistogram(Xmin, Xmax, Ymin, Ymax);
    hbase.Draw();
    drawCMSLumi(lMargin, tMargin, rMargin);

    // Plotting limits
    size_t ncurves(mod.files.size());
    vector<TGraph*> obs(ncurves), exp(ncurves);
    for(size_t file(0); file < ncurves; file++){
      TFile flimit(mod.files[file]);
      exp[file] = static_cast<TGraph*>(flimit.Get(mod.expnames[file]));
      setGraph(exp[file], mod.colors[file], 2, 2, 225);

      obs[file] = static_cast<TGraph*>(flimit.Get(mod.obsnames[file]));
      setGraph(obs[file], mod.colors[file], 1, 4, 225);
    } // Loop over curves in each model
 
    TString plotname(mod.model+"_comb_limits.pdf");
    can.SaveAs(plotname);
  } // Loop over models

  time(&endtime); 
  cout<<endl<<"Took "<<difftime(endtime, begtime)<<" seconds to "<<models.size()<<" plots"<<endl<<endl;
}

void setGraph(TGraph *graph, int color, int style, int width, double glu_lsp){
  graph->SetLineColor(color);
  graph->SetLineStyle(style);
  int fillcolor(color-10);
  graph->SetFillColor(fillcolor);
  graph->SetFillColorAlpha(fillcolor, 0.25);
  graph->SetFillStyle(1001);
  graph->SetLineWidth(width); 

  int np(graph->GetN());
  double mglu, iniglu, endglu, mlsp;
  graph->GetPoint(0, iniglu, mlsp);
  graph->GetPoint(np-1, endglu, mlsp);

  // Reversing graph if needed
  if(iniglu < endglu){
    vector<double> mglus, mlsps;
    for(int point(np-1); point >= 0; point--){
      graph->GetPoint(point, mglu, mlsp);
      mglus.push_back(mglu);
      mlsps.push_back(mlsp);
    }
    for(int point(0); point < np; point++)
      graph->SetPoint(point, mglus[point], mlsps[point]);
  }
  // Finding intersection of line between last 2 points and mlsp = mglu + glu_lsp
  double x1, y1, x2, y2;
  graph->GetPoint(np-1, x1, y1);
  graph->GetPoint(np-2, x2, y2);
  double slope((y1-y2)/(x1-x2)), offset(y1-slope*x1);
  double intersection((offset+glu_lsp)/(1-slope));

  // Adding extrapolation into the diagonal, and point for mglu = 0
  graph->SetPoint(np, intersection, intersection-glu_lsp);
  graph->SetPoint(np+1, 0, -glu_lsp);
  
  graph->Draw("f same");
  graph->Draw("same");
}

void setCanvas(TCanvas &can, float lMargin, float tMargin, float rMargin, float bMargin){
  can.SetLogz();
  can.SetTickx(1);
  can.SetTicky(1);
  can.SetLeftMargin(lMargin);
  can.SetTopMargin(tMargin);
  can.SetRightMargin(rMargin);
  can.SetBottomMargin(bMargin);
}

void drawCMSLumi(float lMargin, float tMargin, float rMargin){
  TLatex label; label.SetNDC();  
  label.SetTextAlign(11); label.SetTextFont(61); label.SetTextSize(0.75*tMargin);
  label.DrawLatex(lMargin, 1-tMargin+0.2*tMargin, "CMS");
  label.SetTextAlign(13); label.SetTextFont(52); label.SetTextSize(0.038);
  label.DrawLatex(0.27, 0.96, "Preliminary");
  label.SetTextAlign(31); label.SetTextFont(42); label.SetTextSize(0.6*tMargin);
  label.DrawLatex(1-rMargin, 1-tMargin+0.2*tMargin, "#sqrt[]{s} = 13 TeV");
}

TH2D baseHistogram(float Xmin, float Xmax, float Ymin, float Ymax){
  TH2D hbase("hbase", "", 1, Xmin, Xmax, 1, Ymin, Ymax);
  hbase.GetXaxis()->SetLabelFont(42);
  hbase.GetXaxis()->SetLabelSize(0.035);
  hbase.GetXaxis()->SetTitleFont(42);
  hbase.GetXaxis()->SetTitleSize(0.05);
  hbase.GetXaxis()->SetTitleOffset(1.2);
  hbase.GetXaxis()->SetTitle("m_{#tilde{g}} [GeV]");
  hbase.GetYaxis()->SetLabelFont(42);
  hbase.GetYaxis()->SetLabelSize(0.035);
  hbase.GetYaxis()->SetTitleFont(42);
  hbase.GetYaxis()->SetTitleSize(0.05);
  hbase.GetYaxis()->SetTitleOffset(1.3);
  hbase.GetYaxis()->SetTitle("m_{#tilde{#chi}_{1}^{0}} [GeV]");
  return hbase;
}

void model_limits::add(TString label, TString file, TString obsname, TString expname, int color){
  labels.push_back(label);
  files.push_back(file);
  obsnames.push_back(obsname);
  expnames.push_back(expname);
  colors.push_back(color);
}

model_limits::model_limits(TString imodel, TString ititle):
  model(imodel),
  title(ititle){
  }
