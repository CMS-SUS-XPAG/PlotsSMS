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
#include "TList.h"
#include "TROOT.h"
#include "TPad.h"
#include "TMultiGraph.h"
#include "TError.h" // Controls error level reporting

// User includes
#include "plot_limits_summary.hpp"

using namespace std;
namespace{
  double cmsH = 0.075;
  float legLineH = 0.039;
  float legTextSize = 0.035;
  float fillTransparency = 0.06;

  TString lsp = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TString chipm = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{#pm}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TString chi2 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1]{#scale[0.85]{_{2}}}";
  int cBenchmark(1);
  int cSus16014(kBlue), cSus16015(kOrange), cSus16016(kGreen+3);//, cSus15004(kGreen+3);
  //int cSus15004_1l(kBlack), cSus15006(kGreen+1), cSus15007(kRed), cSus15008(kCyan+1);
  int cSus16022(kOrange+2);
  int cSus16030(kRed);
}

int main(){
  gErrorIgnoreLevel=kWarning; // Turns off ROOT INFO messages

  // Label definitions
  TString pp_gluglu("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}");
  TString basetitle(pp_gluglu+",  #tilde{g} #rightarrow ");
  TString mj("M#lower[-.1]{_{J}}"), dphi("#Delta#phi");
  TString mt2("M#lower[-.1]{_{T2}}"), mht("H_{#lower[-.4]{T}}^{miss}"), aT("#alpha#lower[-.1]{_{T}}");
  TString ifb("fb#lower[.2]{^{-1}}");
  TLatex labMass; labMass.SetNDC();  
  labMass.SetTextAlign(33); labMass.SetTextFont(42); 
 
  // Folder with root files containing the TGraphs
  TString folder("root/limits_ichep2016/");
  vector<model_limits> models;
  TString energy=""; // " (13 TeV)";

  ///////////////////////////////    Defining T1tttt plot    /////////////////////////////////
  models.push_back(model_limits("T1tttt", basetitle+"t#kern[0.4]{#bar{t}}#kern[0.4]{"+lsp+"}", 01));
  models.back().add("SUS-16-014, 0-lep ("+mht+"), 12.9 "+ifb+energy, folder+"t1tttt_sus16_014.root", 
  		    cSus16014, "ObsLim", "ExpLim");
  models.back().add("SUS-16-015, 0-lep ("+mt2+"), 12.9 "+ifb+energy, folder+"t1tttt_sus16_015.root", 
  		    cSus16015, "gr_obs_smoothed", "gr_exp_smoothed");
  // models.back().add("SUS-15-004, 0-lep (Razor), 2.1 "+ifb+energy, folder+"t1tbqq_sus15_004.root", 
  // 		    cSus15004, "Obs_T1tttt_MultiJet", "Exp_T1tttt_MultiJet");
  models.back().add("SUS-16-016, 0-lep ("+aT+"), 12.9 "+ifb+energy, folder+"t1tttt_sus16_016.root", 
   		    cSus16016, "observed_total", "expected_total");
  // models.back().add("SUS-15-004, 1-lep (Razor), 2.1 "+ifb+energy, folder+"t1tbqq_sus15_004.root", 
  // 		    cSus15004_1l, "Obs_T1tttt_MuMultiJet_EleMultiJet", "Exp_T1tttt_MuMultiJet_EleMultiJet");
  // models.back().add("SUS-15-006, 1-lep ("+dphi+"), 2.3 "+ifb+energy, folder+"t1tttt_sus15_006.root", 
  // 		    cSus15006, "graph_smoothed_Obs", "graph_smoothed_Exp");
  // models.back().add("SUS-15-007, 1-lep ("+mj+"), 2.2 "+ifb+energy, folder+"t1tttt_sus15_007.root", 
  // 		    cSus15007, "graph_smoothed_Obs", "graph_smoothed_Exp");
  // models.back().add("SUS-15-008, #geq2-lep (SS), 2.2 "+ifb+energy, folder+"t1tttt_sus15_008.root", 
  // 		    cSus15008, "ssobs", "ssexp");
  models.back().add("SUS-16-022, #geq3-lep, 12.9 "+ifb+energy, folder+"t1tttt_sus16_022.root", 
   		    cSus16022, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-030, 0-lep (top tag), 12.9 "+ifb+energy, folder+"t1tttt_sus16_030.root", 
		    cSus16030, "combined_obsExclOneTimesProspino_base_BR100pct", 
		    "combined_expExclOneTimesProspino_base_BR100pct");
  // models.back().add("SUS-14-010, 0+1+2+#geq3-lep, 19.5 "+ifb+" (8 TeV)", folder+"t1tttt_sus14_010.root", 
  // 		    cBenchmark, "T1tttt_SUS14010", "noplot");
  models.back().add("SUS-15-002, 0-lep ("+mht+"), 2.3 "+ifb+energy, folder+"t1tttt_sus15_002.root", 
  		    cBenchmark, "ObsLim", "ExpLim");

  ///////////////////////////////    Defining T1bbbb plot    /////////////////////////////////
  models.push_back(model_limits("T1bbbb", basetitle+"b#kern[0.23]{#bar{b}}#kern[0.2]{"+lsp+"}"));
  models.back().add("SUS-16-014 ("+mht+"), 12.9 "+ifb+energy, folder+"t1bbbb_sus16_014.root", 
   		    cSus16014, "ObsLim", "ExpLim");
  models.back().add("SUS-16-015 ("+mt2+"), 12.9 "+ifb+energy, folder+"t1bbbb_sus16_015.root", 
  		    cSus16015, "gr_obs_smoothed", "gr_exp_smoothed");
  // models.back().add("SUS-15-004 (Razor), 2.1 "+ifb+energy, folder+"t1tbqq_sus15_004.root", 
  // 		    cSus15004, "Obs_T1bbbb_MultiJet", "Exp_T1bbbb_MultiJet");
  models.back().add("SUS-16-016 ("+aT+"), 12.9 "+ifb+energy, folder+"t1bbbb_sus16_016.root", 
   		    cSus16016, "observed_total", "expected_total");
  // models.back().add("SUS-14-011 (Razor), 19.3 "+ifb+" (8 TeV)", folder+"t1bbbb_sus14_011.root", 
  // 		    cBenchmark, "T1bbbb_SUS14011", "noplot");
  models.back().add("SUS-15-003 ("+mt2+"), 2.3 "+ifb+energy, folder+"t1bbbb_sus15_003.root", 
  		    cBenchmark, "gr_obs_smoothed", "gr_exp_smoothed");


  ///////////////////////////////    Defining T1qqqq plot    /////////////////////////////////
  models.push_back(model_limits("T1qqqq", basetitle+"q#kern[0.23]{#bar{q}}#kern[0.2]{"+lsp+"}"));
  models.back().add("SUS-16-014 ("+mht+"), 12.9 "+ifb+energy, folder+"t1qqqq_sus16_014.root", 
  		    cSus16014, "ObsLim", "ExpLim");
  models.back().add("SUS-16-015 ("+mt2+"), 12.9 "+ifb+energy, folder+"t1qqqq_sus16_015.root", 
  		    cSus16015, "gr_obs_smoothed", "gr_exp_smoothed");
  // models.back().add("SUS-15-004 (Razor), 2.1 "+ifb+energy, folder+"t1tbqq_sus15_004.root", 
  // 		    cSus15004, "Obs_T1qqqq_MultiJet", "Exp_T1qqqq_MultiJet");
  models.back().add("SUS-16-016 ("+aT+"), 12.9 "+ifb+energy, folder+"t1qqqq_sus16_016.root", 
   		    cSus16016, "observed_total", "expected_total");
  // models.back().add("SUS-13-019 ("+mt2+"), 19.5 "+ifb+" (8 TeV)", folder+"t1qqqq_sus13_019.root", 
  // 		    cBenchmark, "T1_SUS13019", "noplot");
  models.back().add("SUS-15-003 ("+mt2+"), 2.3 "+ifb+energy, folder+"t1qqqq_sus15_003.root", 
  		    cBenchmark, "gr_obs_smoothed", "gr_exp_smoothed");


  //////////////////////////////////////////////////////////////////////////////////////// 
  //////////////////////////////////    Making plots    //////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////// 
  
  // Creating canvas
  gStyle->SetOptStat(0);  
  float lMargin(0.14), tMargin(0.08), rMargin(0.02), bMargin(0.14);
  TCanvas can("canvas","", 600, 600);
  setCanvas(can, lMargin, tMargin, rMargin, bMargin);

  // Creating base legend for observed/expected
  int wobs(4), wexp(4);
  TH1D hobs("hobs","",1,0,1), hexp("hexp","",1,0,1);
  hobs.SetLineColor(1); hobs.SetLineWidth(wobs);
  hexp.SetLineColor(1); hexp.SetLineStyle(2); hexp.SetLineWidth(wexp);


  // double legX(1-rMargin-0.04), legY(1-tMargin-0.03);
  // double legW = 0.2, legH = 0.07;
  // TLegend baseleg(legX-legW, legY-legH, legX, legY);
  // baseleg.SetTextSize(0.034); baseleg.SetFillColor(0); 
  // baseleg.SetFillStyle(0); baseleg.SetBorderSize(0);
  // //baseleg.AddEntry(&hobs, "Observed");
  // baseleg.AddEntry(&hexp, "Expected");
  // legX = 0.75;
  // TLegend obsleg(legX-legW, legY-legH, legX, legY);
  // obsleg.SetTextSize(0.034); obsleg.SetFillColor(0); 
  // obsleg.SetFillStyle(0); obsleg.SetBorderSize(0);
  // obsleg.AddEntry(&hobs, "Observed");


  double legX(1-rMargin-0.02), legY(1-tMargin-0.08);
  double legW = 0.19, legH = 0.07;
  TLegend baseleg(legX-legW, legY-legH, legX, legY);
  baseleg.SetTextSize(0.034); baseleg.SetFillColor(0); 
  baseleg.SetFillStyle(0); baseleg.SetBorderSize(0);
  //baseleg.AddEntry(&hobs, "Observed");
  baseleg.AddEntry(&hexp, "Expected");
  legY = legY - 0.04;
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
      //cout<<"Doing "<<mod.files[file]<<endl;
      TFile flimit(mod.files[file]);
      exp[file] = getGraph(flimit, mod.expnames[file]);
      obs[file] = getGraph(flimit, mod.obsnames[file]);
    }
    for(size_t file(0); file < ncurves; file++){
      //cout<<"Doing "<<mod.files[file]<<endl;
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
    legH = legLineH * ncurves * mod.legScale;
    TLegend limleg(legX, legY-legH, legX+legW, legY);
    limleg.SetTextSize(legTextSize * mod.legScale); limleg.SetFillColor(0); 
    limleg.SetFillStyle(0); limleg.SetBorderSize(0);
    for(size_t file(0); file < ncurves; file++)
      limleg.AddEntry(obs[file]->GetName(), mod.labels[file], "fl");
    limleg.Draw();
    labMass.SetTextSize(legTextSize * mod.legScale * 1.2);
    labMass.DrawLatex(0.93, legY-legH-0.5*legLineH, mod.labMass);

    TString plotname(mod.model+"_limits_summary_cms.pdf");
    can.SaveAs(plotname);
    cout<<" open "<<plotname<<endl;
  } // Loop over models
  cout<<endl<<endl;
}


TGraph* getGraph(TFile &flimit, TString gname){
  if(gname == "noplot") return 0;
  TGraph *graph = static_cast<TGraph*>(flimit.Get(gname));
  if(graph->InheritsFrom(TMultiGraph::Class())){
    TMultiGraph *mgraph = static_cast<TMultiGraph*>(flimit.Get(gname));
    TList *list = mgraph->GetListOfGraphs();
    TIter liter(list);
    return static_cast<TGraph*>(liter());
  }

  // If the TGraph is not directly provided in the root file, try to extract it from a TCanvas
  if(graph==0) {
    TPad *current_pad = static_cast<TPad*>(gPad);
    TCanvas *c1 = static_cast<TCanvas*>(flimit.Get("c1"));
    current_pad->cd();
    if(c1==0) {
      cout<<"Could not find graph "<<gname<<" in file "<<flimit.GetName()<<". Exiting!"<<endl<<endl;
      exit(0);
    }
    graph = static_cast<TGraph*>(c1->GetListOfPrimitives()->FindObject(gname));
    if(graph==0) {
      cout<<"Could not find graph "<<gname<<" in file "<<flimit.GetName()<<". Exiting!"<<endl<<endl;
      exit(0);
    }
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
      while(point <= graph->GetN()) {
	graph->RemovePoint(graph->GetN()-1);
	np--;
      }
      break;
    }
  }
  // Finding intersection of line between last 2 points and mlsp = mglu - glu_lsp
  double x1, y1, x2, y2;
  graph->GetPoint(np-1, x1, y1);
  graph->GetPoint(np-2, x2, y2);
  double slope((y1-y2)/(x1-x2)), offset(y1-slope*x1);
  double intersection((offset+glu_lsp)/(1-slope));
   // cout<<endl<<"("<<x1<<","<<y1<<") to ("<<x2<<","<<y2<<") -> intersection at ("
   //     <<intersection<<","<<intersection-glu_lsp<<"), slope "<<slope<<", offset "<<offset<<endl;


  // Adding extrapolation into the diagonal, and point for mglu = 0
  if(slope<1) graph->SetPoint(graph->GetN(), intersection, intersection-glu_lsp);
  graph->SetPoint(graph->GetN(), 0, -glu_lsp);
  if(x1 == x2 || y1 == y2 || slope == 1){
    for(int point(0); point < graph->GetN(); point++){
      graph->GetPoint(point, mglu, mlsp);
      cout<<point<<": "<<mglu<<", "<<mlsp<<endl;
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
    Xmin = 700; Xmax = 2050;
    Ymin = 0;   Ymax = 2085;
    glu_lsp = 225;
  }
  if(model.Contains("T1tttt_leptonic") || model.Contains("T1tttt_hadronic")){
    Xmin = 700; Xmax = 2050;
    Ymin = 0;   Ymax = 1675;
    glu_lsp = 225;
  }
  if(model == "T1bbbb"){
    Xmin = 700; Xmax = 2050;
    Ymin = 0;   Ymax = 1985;
    glu_lsp = 25;
  }    
  if(model == "T1qqqq"){
    Xmin = 700; Xmax = 2050;
    Ymin = 0;   Ymax = 1750;
    glu_lsp = 25;
  }    
  if(model.Contains("T5qqqqVV")){
    Xmin = 700; Xmax = 2050;
    Ymin = 0;   Ymax = 1650;
    glu_lsp = 115;
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
  label.SetTextAlign(11); label.SetTextFont(61); label.SetTextSize(0.75*tMargin);
  label.DrawLatex(lMargin+offsetx, ycms, "CMS");
  label.SetTextAlign(11); label.SetTextFont(52); label.SetTextSize(0.038);
  label.DrawLatex(0.27+offsetx, ycms, "Preliminary");


  // Printing top title
  label.SetTextAlign(22); label.SetTextFont(42); label.SetTextSize(0.6*tMargin);
  label.DrawLatex((1-rMargin-lMargin)/2.+lMargin-0.05, 1-tMargin/2., title);
  // Printing date
  label.SetTextAlign(31); label.SetTextFont(52); label.SetTextSize(0.45*tMargin);
  label.DrawLatex(1-rMargin-0.02, 1-tMargin+0.018, "ICHEP 2016");
  // Printing energy
  label.SetTextAlign(31); label.SetTextFont(42); label.SetTextSize(0.53*tMargin);
  label.DrawLatex(1-rMargin-0.04, ycms+0.005, "13 TeV");
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
  labMass = "";
  }
