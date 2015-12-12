//// plot_combine_limits: Plots various limit curves on same canvas

// System includes
#include <fstream>
#include <iostream>

// ROOT includes
#include "TStyle.h"
#include "TLatex.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH1D.h"

// User includes
#include "plot_combined_limits.hpp"

using namespace std;
namespace{
  double cmsH = 0.075;
}

int main(){
  TString folder("root/limits_2015/");
  TString lsp("#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}");
  TString pp_gluglu("pp #rightarrow #tilde{g}#kern[0.2]{#tilde{g}}");
  TString basetitle(pp_gluglu+",  #tilde{g} #rightarrow ");
  vector<model_limits> models;

  ///////////////////////////////    Defining T1tttt plot    /////////////////////////////////
  models.push_back(model_limits("T1tttt", basetitle+"t#kern[0.4]{#bar{t}}#kern[0.4]{"+lsp+"}"));
  models.back().add("SUS-14-010, 0+1+2+>2-lep, 19.5 fb^{-1} (8 TeV)", folder+"t1tttt_sus14_010.root", 
		    kBlack, "T1tttt_SUS14010", "T1tttt_SUS14010");
  models.back().add("SUS-15-002, 0-lep (MHT), 2.2 fb^{-1} (13 TeV)", folder+"t1tttt_sus15_002.root", 
		    kBlue, "ObsLim", "ExpLim");
  models.back().add("SUS-15-003, 0-lep (MT2), 2.2 fb^{-1} (13 TeV)", folder+"t1tttt_sus15_003.root", 
		    kGreen+2, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-15-007, 1-lep (M#lower[-.1]{_{J}}), 2.1 fb^{-1} (13 TeV)", folder+"t1tttt_sus15_007.root", 
		    kRed, "graph_smoothed_Obs", "graph_smoothed_Exp");
  models.back().add("SUS-15-008, 2-lep (SS), 2.2 fb^{-1} (13 TeV)", folder+"t1tttt_sus15_008.root", 
		    kMagenta+1, "ssobs", "ssexp");


  //////////////////////////////////    Making plots    //////////////////////////////////
  
  // Creating canvas
  gStyle->SetOptStat(0);  
  float lMargin(0.14), tMargin(0.08), rMargin(0.02), bMargin(0.14);
  TCanvas can("canvas","", 600, 600);
  setCanvas(can, lMargin, tMargin, rMargin, bMargin);

  // Creating base legend for observed/expected
  int wobs(4), wexp(2);
  TH1D hobs("hobs","",1,0,1), hexp("hexp","",1,0,1);
  hobs.SetLineColor(1); hobs.SetLineWidth(wobs);
  hexp.SetLineColor(1); hexp.SetLineStyle(2); hexp.SetLineWidth(wexp);
  double legX(1-rMargin-0.04), legY(1-tMargin-0.03);
  double legW = 0.2, legH = 0.07;
  TLegend baseleg(legX-legW, legY-legH, legX, legY);
  baseleg.SetTextSize(0.032); baseleg.SetFillColor(0); 
  baseleg.SetFillStyle(0); baseleg.SetBorderSize(0);
  baseleg.AddEntry(&hobs, "Observed");
  baseleg.AddEntry(&hexp, "Expected");

  // Looping over each model
  for(size_t imodel(0); imodel < models.size(); imodel++){
    model_limits mod(models[imodel]);

    // Creating base histogram and drawing lumi labels
    float Xmin(700), Xmax(1750), Ymin(0), Ymax(1800);
    TH2D hbase = baseHistogram(Xmin, Xmax, Ymin, Ymax);
    hbase.Draw();
    drawCMSLumi(lMargin, tMargin, rMargin, mod.title);

    // Plotting limits
    size_t ncurves(mod.files.size());
    vector<TGraph*> obs(ncurves), exp(ncurves);
    for(size_t file(0); file < ncurves; file++){
      TFile flimit(mod.files[file]);
      exp[file] = static_cast<TGraph*>(flimit.Get(mod.expnames[file]));
      setGraph(exp[file], mod.colors[file], 2, wexp, 225);

      obs[file] = static_cast<TGraph*>(flimit.Get(mod.obsnames[file]));
      setGraph(obs[file], mod.colors[file], 1, wobs, 225);
      TString obsname("obs"); obsname += imodel; obsname += file;
      obs[file]->SetName(obsname);
    } // Loop over curves in each model
    for(size_t file(0); file < ncurves; file++){
      exp[file]->Draw("same");
      obs[file]->Draw("same");
    }// Loop over curves in each model

    // Drawing legends
    baseleg.Draw();
    double legSingle = 0.037;
    legX = lMargin+0.03; legY = 1-tMargin-cmsH-0.04;
    legW = 0.13; 
    legH = legSingle * ncurves;
    TLegend limleg(legX, legY-legH, legX+legW, legY);
    limleg.SetTextSize(0.034); limleg.SetFillColor(0); 
    limleg.SetFillStyle(0); limleg.SetBorderSize(0);
    for(size_t file(0); file < ncurves; file++)
      limleg.AddEntry(obs[file]->GetName(), mod.labels[file], "fl");
    limleg.Draw();

    TString plotname(mod.model+"_comb_limits.pdf");
    can.SaveAs(plotname);
  } // Loop over models

}


void setGraph(TGraph *graph, int color, int style, int width, double glu_lsp){
  graph->SetLineColor(color);
  graph->SetLineStyle(style);
  int fillcolor(color-4);
  graph->SetFillColor(fillcolor);
  graph->SetFillColorAlpha(fillcolor, 0.15);
  graph->SetFillStyle(1001);
  graph->SetLineWidth(width); 

  int np(graph->GetN());
  double mglu, iniglu, endglu, mlsp;
  graph->GetPoint(0, iniglu, mlsp);
  graph->GetPoint(np-1, endglu, mlsp);

  // Reversing graph if printed towards increasing mgluino
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
  // Finding intersection of line between last 2 points and mlsp = mglu - glu_lsp
  double x1, y1, x2, y2;
  graph->GetPoint(np-1, x1, y1);
  graph->GetPoint(np-2, x2, y2);
  double slope((y1-y2)/(x1-x2)), offset(y1-slope*x1);
  double intersection((offset+glu_lsp)/(1-slope));

  // Adding extrapolation into the diagonal, and point for mglu = 0
  graph->SetPoint(np, intersection, intersection-glu_lsp);
  graph->SetPoint(np+1, 0, -glu_lsp);
  //cout<<intersection<<endl;
  
  graph->Draw("f same");
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

void drawCMSLumi(float lMargin, float tMargin, float rMargin, TString title){
  TLatex label; label.SetNDC();  
  // Printing CMS Preliminary
  double offsetx(0.025), ycms(1-tMargin-cmsH);
  label.SetTextAlign(11); label.SetTextFont(61); label.SetTextSize(0.75*tMargin);
  label.DrawLatex(lMargin+offsetx, ycms, "CMS");
  label.SetTextAlign(11); label.SetTextFont(52); label.SetTextSize(0.038);
  label.DrawLatex(0.27+offsetx, ycms, "Preliminary");
  // Printing top title
  label.SetTextAlign(22); label.SetTextFont(42); label.SetTextSize(0.6*tMargin);
  label.DrawLatex((1-rMargin-lMargin)/2.+lMargin-0.02, 1-tMargin/2., title);
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

void model_limits::add(TString label, TString file, int color, TString obsname, TString expname){
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
