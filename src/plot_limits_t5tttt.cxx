//// plot_limits_summary: Plots various limit curves on same canvas

// System includes
#include <fstream>
#include <iostream>

// ROOT includes
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TPad.h"
#include "TError.h" // Controls error level reporting

// User includes
#include "plot_limits_summary.hpp"

using namespace std;
namespace{
  //double cmsH = 0.075;
  double cmsH = 0.05;
  float legLineH = 0.045;
  float legTextSize = 0.035;
  float fillTransparency = 0.08;

  TString lsp = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  // int c8TeV(kGray+2);
  // int cSus15002(kBlue), cSus15003(kOrange), cSus15004(kGreen+1), cSus15005(kMagenta+1);
  // int cSus15004_1l(kBlack), cSus15007(kRed), cSus15008(kCyan+2);
}

int main(){
  gErrorIgnoreLevel=kWarning; // Turns off ROOT INFO messages

  // Label definitions
  //TString lsp("#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}");
  TString pp_gluglu("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}");
  TString basetitle(pp_gluglu+",  #tilde{g} #rightarrow ");
  TString mj("M#lower[-.1]{_{J}}");
  //TString mt2("M#lower[-.1]{_{T2}}"), mht("#slash{H}#lower[-.1]{_{T}}"), aT("#alpha#lower[-.1]{_{T}}");

  // Folder with root files containing the TGraphs
  TString folder("root/limits_2015/");
  vector<model_limits> models;

  ///////////////////////////////    Defining T1tttt plot    /////////////////////////////////
  models.push_back(model_limits("T1tttt", pp_gluglu));
  models.back().add("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}, #tilde{g} #rightarrow t#kern[0.4]{#bar{t}}#kern[0.4]{"+lsp+"}", 
		    folder+"t1tttt_sus15_007.root", 
  		    kRed, "graph_smoothed_Obs", "graph_smoothed_Exp");
  models.back().add("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}+#tilde{t}_{1}#kern[0.3]{#tilde{t}}_{1}, #tilde{g} #rightarrow #tilde{t}_{1}t,  #tilde{t}_{1} #rightarrow #bar{t}#kern[0.4]{"
		    +lsp+"}   (m#kern[0.3]{_{#lower[-0.12]{#tilde{t}_{1}}}} - m#kern[0.12]{_{"+lsp+"}} = 175 GeV)", 
		    folder+"t5tttt_sus15_007.root", 
  		    kBlue, "graph_smoothed_Obs", "graph_smoothed_Exp");


  //////////////////////////////////////////////////////////////////////////////////////// 
  //////////////////////////////////    Making plots    //////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////// 
  
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
  baseleg.SetTextSize(0.034); baseleg.SetFillColor(0); 
  baseleg.SetFillStyle(0); baseleg.SetBorderSize(0);
  //baseleg.AddEntry(&hobs, "Observed");
  baseleg.AddEntry(&hexp, "Expected");
  legX = 0.75;
  TLegend obsleg(legX-legW, legY-legH, legX, legY);
  obsleg.SetTextSize(0.034); obsleg.SetFillColor(0); 
  obsleg.SetFillStyle(0); obsleg.SetBorderSize(0);
  obsleg.AddEntry(&hobs, "Observed");

  // Looping over each model
  cout<<endl;
  for(size_t imodel(0); imodel < models.size(); imodel++){
    model_limits mod(models[imodel]);

    // Creating base histogram and drawing lumi labels
    float Xmin(700), Xmax(1750), Ymin(0), Ymax(1800), glu_lsp;
    getModelParams(mod.model, Xmin, Xmax, Ymin, Ymax, glu_lsp);

    TH2D hbase = baseHistogram(Xmin, Xmax, Ymin, Ymax);
    hbase.Draw();
    addLabelsTitle(lMargin, tMargin, rMargin, mod.title);

    // Plotting limits
    size_t ncurves(mod.files.size());
    vector<TGraph*> obs(ncurves, 0), exp(ncurves, 0);
    // Getting all graphs first because the ones that come from TCanvas mess up the colors
    for(size_t file(0); file < ncurves; file++){
      TFile flimit(mod.files[file]);
      exp[file] = getGraph(flimit, mod.expnames[file]);
      obs[file] = getGraph(flimit, mod.obsnames[file]);
    }
    for(size_t file(0); file < ncurves; file++){
      if(mod.labels[file].Contains("T5tttt")) glu_lsp += 50;
      setGraphStyle(exp[file], mod.colors[file], 2, wexp, glu_lsp);
      setGraphStyle(obs[file], mod.colors[file], 1, wobs, glu_lsp);
      obs[file]->Draw("f same");

      TString obsname("obs"); obsname += imodel; obsname += file;
      obs[file]->SetName(obsname);
    } // Loop over curves in each model
    // Plotting the lines on top of the fills
    for(size_t file(0); file < ncurves; file++){
      if(exp[file]) exp[file]->Draw("same");
      obs[file]->Draw("same");
    }// Loop over curves in each model

    // Drawing legends
    baseleg.Draw();
    obsleg.Draw();
    legX = lMargin+0.03; legY = 1-tMargin-cmsH-0.035;
    legW = 0.13; 
    legH = legLineH * ncurves;
    TLegend limleg(legX, legY-legH, legX+legW, legY);
    limleg.SetTextSize(legTextSize); limleg.SetFillColor(0); 
    limleg.SetFillStyle(0); limleg.SetBorderSize(0);
    for(size_t file(0); file < ncurves; file++)
      limleg.AddEntry(obs[file]->GetName(), mod.labels[file], "fl");
    limleg.Draw();

    TString plotname(mod.model+"_limits_summary_cms.pdf");
    can.SaveAs(plotname);
    cout<<" open "<<plotname<<endl;
  } // Loop over models
  cout<<endl<<endl;
}


TGraph* getGraph(TFile &flimit, TString gname){
  TGraph *graph = static_cast<TGraph*>(flimit.Get(gname));
  // If the TGraph is not directly provided in the root file, try to extract it from a TCanvas
  if(graph==0) {
    TPad *current_pad = static_cast<TPad*>(gPad);
    TCanvas *c1 = static_cast<TCanvas*>(flimit.Get("c1"));
    current_pad->cd();
    if(c1==0) return 0;
    graph = static_cast<TGraph*>(c1->GetListOfPrimitives()->FindObject(gname));
    if(graph==0) return 0;
  }
  return graph;
}

void setGraphStyle(TGraph* graph, int color, int style, int width, double glu_lsp){
  if(graph==0) return;

  // Setting graph style
  graph->SetLineColor(color);
  graph->SetLineStyle(style);
  int fillcolor(color);
  graph->SetFillColor(fillcolor);
  graph->SetFillColorAlpha(fillcolor, fillTransparency);
  graph->SetFillStyle(1001);
  graph->SetLineWidth(width); 

  int np(graph->GetN());
  double mglu, iniglu, endglu, mlsp;
  graph->GetPoint(0, iniglu, mlsp);
  graph->GetPoint(np-1, endglu, mlsp);
  // Reversing graph if printed towards decreasing mgluino
  if(iniglu > endglu) reverseGraph(graph);
  // Adding a point so that it goes down to mLSP = 0
  graph->SetPoint(graph->GetN(), max(iniglu,endglu), 0);
  np++;

  reverseGraph(graph);

  // Adding a point at LSP = 0, and removing points beyond the diagonal
  for(int point(0); point < np; point++){
    graph->GetPoint(point, mglu, mlsp);
    if(mlsp > mglu-glu_lsp){
      while(point <= graph->GetN()) 
	graph->RemovePoint(graph->GetN()-1);
      break;
    }
  }
  // Finding intersection of line between last 2 points and mlsp = mglu - glu_lsp
  double x1, y1, x2, y2;
  graph->GetPoint(np-1, x1, y1);
  graph->GetPoint(np-2, x2, y2);
  double slope((y1-y2)/(x1-x2)), offset(y1-slope*x1);
  double intersection((offset+glu_lsp)/(1-slope));
  cout<<"intersection "<<intersection<<", slope "<<slope<<", glu_lsp "<<glu_lsp<<endl;

  // Adding extrapolation into the diagonal, and point for mglu = 0
  if(slope!=1) graph->SetPoint(graph->GetN(), intersection, intersection-glu_lsp);
  graph->SetPoint(graph->GetN(), 0, -glu_lsp);
  if(x1 == x2 || y1 == y2 || slope == 1){
    for(int point(0); point < graph->GetN(); point++){
      graph->GetPoint(point, mglu, mlsp);
      //cout<<point<<": "<<mglu<<", "<<mlsp<<endl;
    }
  }
}

void reverseGraph(TGraph *graph){
  int np(graph->GetN());
  double mglu, mlsp;
  vector<double> mglus, mlsps;
  for(int point(np-1); point >= 0; point--){
    graph->GetPoint(point, mglu, mlsp);
    mglus.push_back(mglu);
    mlsps.push_back(mlsp);
  }
  for(int point(0); point < np; point++)
    graph->SetPoint(point, mglus[point], mlsps[point]);
}

void getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp){
  if(model == "T1tttt"){
    Xmin = 600; Xmax = 1750;
    Ymin = 0;   Ymax = 1285;
    glu_lsp = 225;
  }
  if(model == "T1bbbb"){
    Xmin = 600; Xmax = 1950;
    Ymin = 0;   Ymax = 1885;
    glu_lsp = 25;
  }    
  if(model == "T1qqqq"){
    Xmin = 600; Xmax = 1950;
    Ymin = 0;   Ymax = 1750;
    glu_lsp = 25;
  }    
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

void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title){
  TLatex label; label.SetNDC();  
  // Printing CMS Preliminary
  double offsetx(0.025), ycms(1-tMargin-cmsH);
  label.SetTextAlign(12); label.SetTextFont(61); label.SetTextSize(0.75*tMargin);
  label.DrawLatex(lMargin+offsetx, 1-tMargin/2., "CMS");
  label.SetTextAlign(12); label.SetTextFont(52); label.SetTextSize(0.038);
  //label.DrawLatex(0.27+offsetx, 1-tMargin/2.-0.013, "Preliminary");
  // Printing top title
  label.SetTextAlign(21); label.SetTextFont(42); label.SetTextSize(0.6*tMargin);
  //label.DrawLatex((1-rMargin-lMargin)/2.+lMargin-0.05, 1-tMargin/2., title);
  //label.DrawLatex((1-rMargin-lMargin)/2.+lMargin-0.05-0.25, ycms, title);
  cout<<ycms<<title<<endl;
  // Printing date
  label.SetTextAlign(31); label.SetTextFont(42); label.SetTextSize(0.45*tMargin);
  label.DrawLatex(1-rMargin-0.02, 1-tMargin+0.018, "2.2 fb^{-1} (13 TeV)");
}

TH2D baseHistogram(float Xmin, float Xmax, float Ymin, float Ymax){
  TH2D hbase("hbase", "", 1, Xmin, Xmax, 1, Ymin, Ymax);
  hbase.GetXaxis()->SetLabelFont(42);
  hbase.GetXaxis()->SetLabelSize(0.035);
  hbase.GetXaxis()->SetTitleFont(42);
  hbase.GetXaxis()->SetTitleSize(0.05);
  hbase.GetXaxis()->SetTitleOffset(1.2);
  hbase.GetXaxis()->SetTitle("m#kern[0.12]{_{#lower[-0.12]{#tilde{g}}}} [GeV]");
  hbase.GetYaxis()->SetLabelFont(42);
  hbase.GetYaxis()->SetLabelSize(0.035);
  hbase.GetYaxis()->SetTitleFont(42);
  hbase.GetYaxis()->SetTitleSize(0.05);
  hbase.GetYaxis()->SetTitleOffset(1.3);
  hbase.GetYaxis()->SetTitle("m#kern[0.12]{_{"+lsp+"}} [GeV]");
  return hbase;
}

void model_limits::add(TString label, TString file, int color, TString obsname, TString expname){
  labels.push_back(label);
  files.push_back(file);
  obsnames.push_back(obsname);
  expnames.push_back(expname);
  colors.push_back(color);
}

model_limits::model_limits(TString imodel, TString ititle, float ilegScale):
  model(imodel),
  title(ititle),
  legScale(ilegScale){
  }
