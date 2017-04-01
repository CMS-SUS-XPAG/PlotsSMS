//// plot_limits_summary: Plots various limit curves on same canvas

// System includes
#include <fstream>
#include <iostream>
#include <cmath>
#include <iomanip>  // setw

// ROOT includes
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TH1D.h"
#include "TROOT.h"
#include "TList.h"
#include "TLine.h"
#include "TROOT.h"
#include "TPad.h"
#include "TMultiGraph.h"
#include "TError.h" // Controls error level reporting

// User includes
#include "plot_limits_summary.hpp"

using namespace std;
namespace{
  bool no_shade = true;
  bool debug = false;

  int LineWidth = 4;
  float cmsH = 0.075;
  float legLineH = 0.039;
  float legTextSize = 0.035;
  float TopMargin = 0.08;
  float legY = 1-TopMargin-cmsH-0.025;
  float fillTransparency = 0.06;

  TString xtitle;
  TString lsp = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TString chipm = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{#pm}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TString chi2 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1]{#scale[0.85]{_{2}}}";
  TString ifb("fb#lower[.2]{^{-1}}");
  TString xl("x#lower[-.2]{_{l}}");
  int cBenchmark(no_shade?kGray+1:1);
  int cSus16014(kBlue), cSus16015(kOrange), cSus16016(kGreen+1);
  //int cSus16018(kBlack);
  int cSus16022(kOrange+2), cSus16020(kCyan+1), cSus16019(kMagenta+1);
  int cSus16030(kRed);
}

int main(){
  gErrorIgnoreLevel=kWarning; // Turns off ROOT INFO messages

  // Label definitions
  TString pp_gluglu("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}");
  TString basetitle(pp_gluglu+",  #tilde{g} #rightarrow ");
  TString mj("M#lower[-.1]{_{J}}"), dphi("#Delta#phi");
  TString mt2("M#lower[-.1]{_{T2}}"), mht("H_{#lower[-.4]{T}}^{miss}"), aT("#alpha#lower[-.1]{_{T}}");

  // Folder with root files containing the TGraphs
  TString folder("root/limits_ichep2016/");
  vector<model_limits> models;
  vector<float> mLSPs({0., 200.}); // mLSP values for which excluded mGlu is printed
  TString energy=""; // " (13 TeV)"; // Used when there are 8 and 13 TeV results

  ///////////////////////////////    Defining EWKino plot    /////////////////////////////////
  models.push_back(model_limits("EWKino", "pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}"));
  models.back().lumi = "12.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 230);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 230);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 230);
  models.back().add("SUS-16-024, 3-lep (WH)", folder+"ewkino_sus16_024_wh.root", 
  		    kMagenta+1, "gr_obs", "gr_exp", 124.5);
  models.back().add("SUS-16-024, 3-lep (WZ)", folder+"ewkino_sus16_024_wz.root", 
  		    kRed, "gr_obs", "gr_exp", 7);
  models.back().add("SUS-16-025, soft 2-lep (WZ)", folder+"ewkino_sus16_025.root", 
   		    1, "ex_obs_smoothed_graph", "ex_exp_smoothed_graph", 7);

  ///////////////////////////////    Defining EWK-slep plot    /////////////////////////////////
  models.push_back(model_limits("EWK-slep","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}", 0.9));
  models.back().lumi = "12.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 510);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 510);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 330, false);
  models.back().add("SUS-16-024, 2l SS + 3l (BF(ll)=0.5, "+xl+"=0.05)", folder+"ewkino_sus16_024_bf0p5_x0p05.root", 
  		    kOrange, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-024, 3-lep (BF(ll)=0.5, "+xl+"=0.5)", folder+"ewkino_sus16_024_bf0p5_x0p5.root", 
  		    kBlue, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-024, 3-lep (BF(#tau#tau)=1, "+xl+"=0.5)", folder+"ewkino_sus16_024_bf1_x0p05.root", 
  		    kGreen+1, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-024, 3-lep (WH)", folder+"ewkino_sus16_024_wh.root", 
  		    kMagenta+1, "gr_obs", "gr_exp", 124.5);
  models.back().add("SUS-16-024, 3-lep (WZ)", folder+"ewkino_sus16_024_wz.root", 
  		    kRed, "gr_obs", "gr_exp", 7);
  models.back().add("SUS-16-025, soft 2-lep (WZ)", folder+"ewkino_sus16_025.root", 
   		    1, "ex_obs_smoothed_graph", "ex_exp_smoothed_graph", 7);

  ///////////////////////////////    Defining T1tttt plot    /////////////////////////////////
  models.push_back(model_limits("T1tttt", basetitle+"t#kern[0.4]{#bar{t}}#kern[0.4]{"+lsp+"}"));
  models.back().lumi = "12.9"; 
  models.back().add("SUS-16-014, 0-lep ("+mht+")", folder+"t1tttt_sus16_014.root", 
  		    cSus16014, "ObsLim", "ExpLim");
  models.back().add("SUS-16-015, 0-lep ("+mt2+")", folder+"t1tttt_sus16_015.root", 
  		    cSus16015, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-016, 0-lep ("+aT+")", folder+"t1tttt_sus16_016.root", 
   		    cSus16016, "observed_total", "expected_total");
  // models.back().add("SUS-16-018, 1-lep ("+mj+")", folder+"t1tttt_sus16_018.root", 
  // 		    cSus16018, "graph_smoothed_Obs", "graph_smoothed_Exp");
  models.back().add("SUS-16-019, 1-lep ("+dphi+")", folder+"t1tttt_sus16_019.root", 
   		    cSus16019, "graph_smoothed_Obs", "graph_smoothed_Exp");
  models.back().add("SUS-16-020, #geq2-lep (SS)", folder+"t1tttt_sus16_020.root", 
   		    cSus16020, "ssobs", "ssexp");
  models.back().add("SUS-16-022, #geq3-lep", folder+"t1tttt_sus16_022.root", 
   		    cSus16022, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-030, 0-lep (top tag)", folder+"t1tttt_sus16_030.root", 
  		    cSus16030, "combined_obsExclOneTimesProspino_base_BR100pct", 
  		    "combined_expExclOneTimesProspino_base_BR100pct");
  // models.back().add("SUS-15-002, 0-lep ("+mht+"), 2.3 "+ifb+energy, folder+"t1tttt_sus15_002.root", 
  // 		    cBenchmark, "ObsLim", "ExpLim");

  ///////////////////////////////    Defining T1bbbb plot    /////////////////////////////////
  models.push_back(model_limits("T1bbbb", basetitle+"b#kern[0.23]{#bar{b}}#kern[0.2]{"+lsp+"}"));
  models.back().lumi = "12.9"; 
  models.back().add("SUS-16-014 ("+mht+")", folder+"t1bbbb_sus16_014.root", 
   		    cSus16014, "ObsLim", "ExpLim");
  models.back().add("SUS-16-015 ("+mt2+")", folder+"t1bbbb_sus16_015.root", 
  		    cSus16015, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-016 ("+aT+")", folder+"t1bbbb_sus16_016.root", 
   		    cSus16016, "observed_total", "expected_total");
  // models.back().add("SUS-15-003 ("+mt2+"), 2.3 "+ifb+energy, folder+"t1bbbb_sus15_003.root", 
  // 		    cBenchmark, "gr_obs_smoothed", "gr_exp_smoothed");


  ///////////////////////////////    Defining T1qqqq plot    /////////////////////////////////
  models.push_back(model_limits("T1qqqq", basetitle+"q#kern[0.23]{#bar{q}}#kern[0.2]{"+lsp+"}"));
  models.back().lumi = "12.9"; 
  models.back().add("SUS-16-014 ("+mht+")", folder+"t1qqqq_sus16_014.root", 
  		    cSus16014, "ObsLim", "ExpLim");
  models.back().add("SUS-16-015 ("+mt2+")", folder+"t1qqqq_sus16_015.root", 
  		    cSus16015, "gr_obs_smoothed", "gr_exp_smoothed");
  // models.back().add("SUS-16-016 ("+aT+")", folder+"t1qqqq_sus16_016.root", 
  //  		    cSus16016, "observed_total", "expected_total");
  // models.back().add("SUS-15-003 ("+mt2+"), 2.3 "+ifb+energy, folder+"t1qqqq_sus15_003.root", 
  // 		    cBenchmark, "gr_obs_smoothed", "gr_exp_smoothed");


  //////////////////////////////////////////////////////////////////////////////////////// 
  //////////////////////////////////    Making plots    //////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////// 
  
  //// Creating canvas
  gStyle->SetOptStat(0);  
  float lMargin(0.14), tMargin(TopMargin), rMargin(0.04), bMargin(0.14);
  TCanvas can("canvas","", 600, 600);
  setCanvas(can, lMargin, tMargin, rMargin, bMargin);

  //// Creating base legend for observed/expected
  TH1D hobs("hobs","",1,0,1), hexp("hexp","",1,0,1);
  hobs.SetLineColor(1); hobs.SetLineWidth(LineWidth);
  hexp.SetLineColor(1); hexp.SetLineStyle(2); hexp.SetLineWidth(LineWidth);

  double legX(1-rMargin-0.045), baselegY = 1-tMargin-cmsH-0.02;
  double legW = 0.14, legH = 0.07;
  TLegend baseleg(legX-legW, baselegY-legH, legX, baselegY);
  baseleg.SetTextSize(0.031); baseleg.SetFillColor(0); 
  baseleg.SetFillStyle(0); baseleg.SetBorderSize(0);
  baseleg.AddEntry(&hexp, "Expected");
  baselegY = baselegY - 0.038;
  TLegend obsleg(legX-legW, baselegY-legH, legX, baselegY);
  obsleg.SetTextSize(0.031); obsleg.SetFillColor(0); 
  obsleg.SetFillStyle(0); obsleg.SetBorderSize(0);
  obsleg.AddEntry(&hobs, "Observed");

  //// General line and label
  TLatex labMass; labMass.SetNDC();  
  labMass.SetTextAlign(33); labMass.SetTextFont(42); 
  TLine line;
  TLatex label;

  //// Looping over each model
  cout<<endl;
  for(size_t imodel(0); imodel < models.size(); imodel++){
    model_limits mod(models[imodel]);

    //// Creating base histogram and drawing lumi labels
    float Xmin(700), Xmax(1750), Ymin(0), Ymax(1800), glu_lsp;
    getModelParams(mod.model, Xmin, Xmax, Ymin, Ymax, glu_lsp);

    TH2D hbase = baseHistogram(Xmin, Xmax, Ymin, Ymax);
    hbase.Draw();
    addLabelsTitle(lMargin, tMargin, rMargin, mod.title, mod.lumi);

    //// Plot lines at constant dM
    float degrees = 180/3.141593*atan2((1-tMargin-bMargin)/(Ymax-Ymin) , (1-lMargin-rMargin)/(Xmax-Xmin));
    int lcolor(kGray+2);
    line.SetLineStyle(2); line.SetLineWidth(2); line.SetLineColor(lcolor);
    label.SetTextSize(0.027); label.SetTextAngle(degrees); label.SetTextColor(lcolor);
    for(size_t iline=0; iline<mod.lines.size(); iline++){
      float dM = mod.lines[iline].dm;
      float minGlu=dM;
      float maxh = mod.lines[iline].maxHeight;
      if(dM<Xmin) minGlu = Xmin;
      line.DrawLine(minGlu, minGlu-dM, dM+maxh, maxh);
      // offsetX controls where along the line to put the label, offsetY how far from it
      float offsetX=(Xmax-Xmin)/38, offsetY=(Xmax-Xmin)/38;
      if(mod.lines[iline].above){
	label.SetTextAlign(31); 
      } else {
	label.SetTextAlign(33);
	offsetX *= -0.4;
	offsetY *= -0.7;
      }
      label.DrawLatex(dM+maxh-offsetX, maxh-offsetX+offsetY, mod.lines[iline].label);
    }// Loop over lines

    //// Plotting limits
    size_t ncurves(mod.files.size());
    vector<TGraph*> obs(ncurves, 0), exp(ncurves, 0);
    // Getting all graphs first because the ones that come from TCanvas mess up the colors
    for(size_t file(0); file < ncurves; file++){
      //cout<<"Loading "<<mod.files[file]<<endl;
      TFile flimit(mod.files[file]);
      exp[file] = getGraph(flimit, mod.expnames[file]);
      obs[file] = getGraph(flimit, mod.obsnames[file]);
    }
    cout<<endl<<"Model "<<mod.model<<endl<<"============================="<<endl;
    for(size_t file(0); file < ncurves; file++){
      if (debug) cout<<"Doing "<<mod.files[file]<<endl;
      float mod_gl = glu_lsp;
      if(mod.glu_lsps[file]>0) mod_gl = mod.glu_lsps[file];
      setGraphStyle(exp[file], mod.colors[file], 2, LineWidth, mod_gl);
      setGraphStyle(obs[file], mod.colors[file], 1, LineWidth, mod_gl);
      printExclGlu(obs[file], exp[file], mLSPs, mod.labels[file]);
      obs[file]->Draw("f same");

      TString obsname("obs"); obsname += imodel; obsname += file;
      obs[file]->SetName(obsname);
    } // Loop over curves in each model
    //// Plotting the lines on top of the fills
    if(mod.reverseOrder)
      for(size_t file(ncurves-1); file < ncurves; file--){
	if(exp[file])  exp[file]->Draw("same");
	obs[file]->Draw("same");
      }// Loop over curves in each model
    else
      for(size_t file(0); file < ncurves; file++){
	if(exp[file])  exp[file]->Draw("same");
	obs[file]->Draw("same");
      }// Loop over curves in each model

    // Drawing legends
    baseleg.Draw();
    obsleg.Draw();
    legX = lMargin+0.03; //legY = 1-tMargin-cmsH-0.035;
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

    TString plotname(mod.model+"_limits_summary_cms"+(!no_shade?"_shade":"")+".pdf");
    can.SaveAs(plotname);
    cout<<" open "<<plotname<<endl<<endl;
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
  if(!no_shade) graph->SetFillColorAlpha(fillcolor, fillTransparency);
  else {
    if(color == cBenchmark) graph->SetFillColorAlpha(fillcolor, 0.35);
    else graph->SetFillColorAlpha(fillcolor, 0);
  }
  graph->SetFillStyle(1001);
  graph->SetLineWidth(width); 

  int np(graph->GetN());
  double mglu, iniglu, endglu, mlsp, inilsp, endlsp;

  // Printing all points before modifications
  if(debug){
    cout<<endl<<endl;
    for(int point(0); point < graph->GetN(); point++){
      graph->GetPoint(point, mglu, mlsp);
      cout<<point<<": "<<setw(7)<<mglu<<", "<<mlsp<<endl;
    }
  }

  // The EWKino-WZ graph had a jump. Fix it
  makeContinuousGraph(graph);

  graph->GetPoint(0, iniglu, inilsp);
  graph->GetPoint(np-1, endglu, endlsp);
  // Reversing graph if printed towards decreasing mgluino
  if(inilsp < endlsp) {
    reverseGraph(graph);
    endglu = iniglu;
    endlsp = inilsp;
  }
  // Adding a point so that it goes down to mLSP = 0, but not for WZ,SOS
  if(endlsp<20){
    graph->SetPoint(graph->GetN(), endglu, 0);
    np++;
  }

  reverseGraph(graph);

  if(debug){
    cout<<endl<<endl;
    for(int point(0); point < graph->GetN(); point++){
      graph->GetPoint(point, mglu, mlsp);
      if(style==1) cout<<point<<": "<<setw(7)<<mglu<<", "<<mlsp<<endl;
    }
  }

  // Adding a point at mLSP = 0, and removing points beyond the diagonal
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
    // cout<<"Slope is one"<<endl;
  }
}

void makeContinuousGraph(TGraph *graph){
  int np(graph->GetN()), iDiscontinuous=-1;
  double mglu, mlsp, mglu_prev=-99.;
  vector<double> mglus, mlsps;
  for(int point=0; point < np; point++){
    graph->GetPoint(point, mglu, mlsp);
    mglus.push_back(mglu);
    mlsps.push_back(mlsp);
    if(fabs(mglu-mglu_prev) > 200 && point>0 && mglu>=0 && mlsp>=0 && mglu_prev>=0) {
      iDiscontinuous = point;
      if(debug) cout<<"Found discontinuity at "<<point<<" from "<<mglu_prev<<" to "<<mglu<<endl;
    }
    mglu_prev = mglu;
  }
  if(iDiscontinuous>0){
    int index = iDiscontinuous;
    for(int point(0); point < np; point++){
      if(index==np) index=0;
      graph->SetPoint(point, mglus[index], mlsps[index]);
      index++;
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

// This function relies on setGraphStyle being called earlier to have the counter-clockwise
// orientation, and the point at mLSP = 0
void printExclGlu(TGraph *gobs, TGraph *gexp, vector<float> &mLSPs, TString label){
  size_t nLSPs=mLSPs.size();
  vector<float> mGlusObs = intersectionLSP(gobs, mLSPs);
  vector<float> mGlusExp = intersectionLSP(gexp, mLSPs);
  // Keeping only the SUS-16-XXX part of the label
  if(label.Index(',')>0) label.Remove(label.Index(','), label.Length()-label.Index(','));
  if(label.Index(' ')>0) label.Remove(label.Index(' '), label.Length()-label.Index(' '));
  cout<<label<<": observed limits for mLSP = ";
  for(size_t ilsp=0; ilsp<nLSPs; ilsp++) {
    cout<<round(mLSPs[ilsp]);
    if(ilsp<nLSPs-1) cout<<", ";
  }
  cout<<" are mGlu = ";
  for(size_t ilsp=0; ilsp<nLSPs; ilsp++) {
    cout<<setw(4)<<round(mGlusObs[ilsp]);
    if(ilsp<nLSPs-1) cout<<", ";
  }
  cout<<"  (for expected limits mGlu = ";
  for(size_t ilsp=0; ilsp<nLSPs; ilsp++) {
    cout<<setw(4)<<round(mGlusExp[ilsp]);
    if(ilsp<nLSPs-1) cout<<", ";
  }
  cout<<")"<<endl;
}

vector<float> intersectionLSP(TGraph *graph, vector<float> &mLSPs){
  size_t nLSPs=mLSPs.size();
  vector<float> mGlus(nLSPs,-99.);
  double mglu, mglu_prev=-99., mlsp, mlsp_prev=-99.;
  for(int point(0); point < graph->GetN(); point++){
    graph->GetPoint(point, mglu, mlsp);
    for(size_t ilsp=0; ilsp<nLSPs; ilsp++) {
      if(mLSPs[ilsp]<=mlsp && mLSPs[ilsp]>mlsp_prev){
	float mGluIntersec = mglu + (mglu_prev-mglu)*(mLSPs[ilsp]-mlsp)/(mlsp_prev-mlsp);
	if(ilsp==0) mGluIntersec = mglu;
	if(mGluIntersec>mGlus[ilsp]) mGlus[ilsp] = mGluIntersec;
      }
    } // Loop over desired mLSP intersections
    mglu_prev = mglu;
    mlsp_prev = mlsp;
  } // Loop over graph points
  return mGlus;
}

void getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp){
  xtitle = "m#kern[0.12]{_{#lower[-0.12]{#tilde{g}}}}";
  if(model == "EWK-slep"){
    Xmin = 100; Xmax = 1050;
    Ymin = 0;   Ymax = 890;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model == "EWKino"){
    Xmin = 100; Xmax = 430;
    Ymin = 0;   Ymax = 350;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model.Contains("T1tttt")){
    Xmin = 700; Xmax = 2000;
    Ymin = 0;   Ymax = 2250;
    glu_lsp = 225;
  }
  if(model.Contains("T1tttt_leptonic") || model.Contains("T1tttt_hadronic")){
    Xmin = 700; Xmax = 2000;
    Ymin = 0;   Ymax = 1675;
    glu_lsp = 225;
  }
  if(model == "T1bbbb"){
    Xmin = 700; Xmax = 2000;
    Ymin = 0;   Ymax = 1850;
    glu_lsp = 25;
  }    
  if(model == "T1qqqq"){
    Xmin = 700; Xmax = 2000;
    Ymin = 0;   Ymax = 1550;
    glu_lsp = 25;
  }    
  if(model.Contains("T5qqqqVV")){
    Xmin = 700; Xmax = 2000;
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

void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title, TString lumi){
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
  if(lumi=="") lumi = "13 TeV";
  else lumi += " "+ifb+" (13 TeV)";
  label.DrawLatex(1-rMargin-0.04, ycms+0.005, lumi);
}

TH2D baseHistogram(float Xmin, float Xmax, float Ymin, float Ymax){
  TH2D hbase("hbase", "", 1, Xmin, Xmax, 1, Ymin, Ymax);
  hbase.GetXaxis()->SetLabelFont(42);
  hbase.GetXaxis()->SetLabelSize(0.035);
  hbase.GetXaxis()->SetTitleFont(42);
  hbase.GetXaxis()->SetTitleSize(0.05);
  hbase.GetXaxis()->SetTitleOffset(1.2);
  hbase.GetXaxis()->SetTitle(xtitle+" [GeV]");
  hbase.GetYaxis()->SetLabelFont(42);
  hbase.GetYaxis()->SetLabelSize(0.035);
  hbase.GetYaxis()->SetTitleFont(42);
  hbase.GetYaxis()->SetTitleSize(0.05);
  hbase.GetYaxis()->SetTitleOffset(1.3);
  hbase.GetXaxis()->SetLabelOffset(0.009);
  hbase.GetYaxis()->SetTitle("m#kern[0.12]{_{"+lsp+"}} [GeV]");
  return hbase;
}

void model_limits::add(TString label, TString file, int color, TString obsname, TString expname, float glu_lsp){
  labels.push_back(label);
  files.push_back(file);
  obsnames.push_back(obsname);
  expnames.push_back(expname);
  colors.push_back(color);
  glu_lsps.push_back(glu_lsp);
}

model_limits::model_limits(TString imodel, TString ititle, float ilegScale):
  model(imodel),
  title(ititle),
  lumi(""),
  legScale(ilegScale){
  labMass = "";
  reverseOrder = true;
  }

void model_limits::addLine(TString label, float dm, float maxHeight, bool above){
  lines.push_back(dm_line(label, dm, maxHeight, above));
}

dm_line::dm_line(TString ilabel, float idm, float imaxHeight, bool iabove):
  label(ilabel),
  dm(idm),
  maxHeight(imaxHeight),
  above(iabove){
  }
