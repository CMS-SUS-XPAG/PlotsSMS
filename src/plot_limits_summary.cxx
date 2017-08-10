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
#include "TError.h" // Controls error level reporting

// User includes
#include "utilities.hpp"
#include "model_limits.hpp"

using namespace std;
namespace{
  bool do_prelim = false;
  bool no_shade = true;
  bool debug = false;

  int LineWidth = 4;
  float cmsH = 0.075;
  float legLineH = 0.039;
  float legTextSize = 0.035;
  float TopMargin = 0.08;
  float legY = 1-TopMargin-cmsH-0.025;
  float fillTransparency = 0.06;

  TString lsp = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TString chipm = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{#pm}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TString chi2 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1]{#scale[0.85]{_{2}}}";
  TString ifb("fb#lower[.2]{^{-1}}");
  TString xl("x#lower[-.2]{_{l}}");

  TString flavDem = chipm+chi2+"#rightarrowl#tilde{#nu}l#tilde{l}, BF(ll)=0.5";
  TString tauRich = chipm+chi2+"#rightarrow#tilde{#tau}#nul#tilde{l}";
  TString tauDom  = chipm+chi2+"#rightarrow#tilde{#tau}#nu#tau#tilde{#tau}";

  TString WZdecay = "BF("+chi2+"#rightarrowZ#kern[0.3]{"+lsp+"})";
  TString WHdecay = "BF("+chi2+"#rightarrowH#kern[0.3]{"+lsp+"})";

  int cBenchmark(no_shade?kGray+2:1);
  int cSus16033(kBlue), cSus16036(kGreen+1), cSus16016(kGreen+1);
  int cSus16037(kRed);
  int cSus16022(kOrange+2), cSus16035(kCyan+1), cSus16042(kMagenta+1);
  int cSus16030(kRed);

  int c5050 = kBlue, c5005 = kAzure+7, c5095 = kSpring-6;
  int c10050 = kOrange-2, c10005 = kGray+1, c10095 = kOrange+2;
  int ctau50 = kPink+1;
  int cwh = kMagenta+1, cwh43 = kGreen+1, cwz39 = kRed, cwz48 = 1, cwz34 = kCyan+2;
}

TString getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp);
void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title, TString lumi);

int main(){
  gErrorIgnoreLevel=kWarning; // Turns off ROOT INFO messages

  // Label definitions
  TString pp_gluglu("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}");
  TString basetitle(pp_gluglu+",  #tilde{g} #rightarrow ");
  TString mj("M#lower[-.1]{_{J}}"), dphi("#Delta#phi");
  TString mt2("M#lower[-.1]{_{T2}}"), mht("H_{#lower[-.4]{T}}^{miss}"), aT("#alpha#lower[-.1]{_{T}}");

  // Folder with root files containing the TGraphs
  TString folder("root/limits_moriond2017/");
  vector<model_limits> models;
  vector<float> mLSPs({0., 200.}); // mLSP values for which excluded mGlu is printed
  TString energy=""; // " (13 TeV)"; // Used when there are 8 and 13 TeV results


  // ///////////////////////////////    Defining WH only plot   //////////////////////////////
  // models.push_back(model_limits("WH","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}"));
  // models.back().lumi = "35.9"; models.back().reverseOrder = false;
  // models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 120);

  // models.back().add("SUS-16-039, 2l SS + #geq3l (WH)", folder+"ewkino_sus16_039_wh_gr.root", 
  // 		    cwh, "gr_obs", "gr_exp", 124.5);
  // models.back().add("SUS-16-043, 1l (WH)", folder+"ewkino_sus16_043_wh.root", 
  // 		    cwh43, "massplane_obs", "contourplot", 124.5);
  // models.back().add("SUS-16-045, H#rightarrow#gamma#gamma (WH)", folder+"ewkino_sus16_045_wh.root", 
  //  		    kOrange, "obs", "exp", 124.5);

  // ///////////////////////////////    Defining WZ only plot   //////////////////////////////
  // models.push_back(model_limits("WZ","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}"));
  // models.back().lumi = "35.9"; models.back().reverseOrder = false;
  // models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 250);
  // models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 250);

  // models.back().add("SUS-16-034, 2l OS (WZ)", folder+"ewkino_sus16_034_wz.root", 
  // 		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 9999);
  // models.back().add("This result, 3l (WZ)", folder+"ewkino_sus16_039_wz_gr.root", 
  // 		    cwz39, "gr_obs", "gr_exp", 7);
  // models.back().add("SUS-16-048, soft 2-lep (WZ)", folder+"ewkino_sus16_048_wz.root", 
  //  		    cwz48, "ex_obs_smoothed_graph", "ex_exp_smoothed_graph", 7);

  ///////////////////////////////    Defining EWKino only plot   //////////////////////////////
  models.push_back(model_limits("EWKino","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}"));
  models.back().lumi = "35.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 250);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 250);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 250);

  models.back().add("SUS-16-039, 2l SS + #geq3l (WH)", folder+"ewkino_sus16_039_wh_gr.root", 
  		    cwh, "gr_obs", "gr_exp", 124.5);
  models.back().add("SUS-16-043, 1l (WH)", folder+"ewkino_sus16_043_wh.root", 
  		    cwh43, "massplane_obs", "contourplot", 124.5);
  models.back().add("SUS-16-045, H#rightarrow#gamma#gamma (WH)", folder+"ewkino_sus16_045_wh.root", 
   		    kOrange, "obs", "exp", 124.5);
  models.back().add("SUS-16-034, 2l OS (WZ)", folder+"ewkino_sus16_034_wz.root", 
  		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 9999);
  models.back().add("This result, 3l (WZ)", folder+"ewkino_sus16_039_wz_gr.root", 
  		    cwz39, "gr_obs", "gr_exp", 7);
  models.back().add("SUS-16-048, soft 2-lep (WZ)", folder+"ewkino_sus16_048_wz.root", 
   		    cwz48, "ex_obs_smoothed_graph", "ex_exp_smoothed_graph", 7);

  ///////////////////////////////    Defining EWK combination plot   //////////////////////////////
  models.push_back(model_limits("EWKComb","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}", 0.9)); 
  models.back().lumi = "35.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 290);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 290);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 290);
  models.back().setRanges(125, 700, 0, 430);

  // models.back().add("SUS-17-004, comb. (WH)", folder+"ewkino_sus17_004_wh.root", 
  // 		    cwh43, "gr_obs", "gr_exp", 124.5);
  // models.back().add("SUS-17-004, comb. (WZ)", folder+"ewkino_sus17_004_wz.root", 
  // 		    cwz39, "gr_obs", "gr_exp", 25);
  // models.back().add("SUS-17-004, comb. (0.5#timesWH+0.5#timesWZ)", folder+"ewkino_sus17_004_wzmix.root", 
  // 		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 124.5);

  models.back().add(WHdecay+" = 1 (WH)", folder+"ewkino_sus17_004_wh.root", 
  		    cwh43, "gr_obs", "gr_exp", 124.5);
  models.back().add(WZdecay+" = 1 (WZ)", folder+"ewkino_sus17_004_wz.root", 
  		    cwz39, "gr_obs", "gr_exp", 7);
  models.back().add(WZdecay+" = "+WHdecay+" = 0.5 (WH+WZ)", folder+"ewkino_sus17_004_wzmix.root", 
  		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 124.5);

  ///////////////////////////////    Defining EWK-slep plot    /////////////////////////////////
  models.push_back(model_limits("EWK-slep-0p5","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}", 0.9));
  models.back().lumi = "35.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 590);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 590);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 850, false);

  models.back().add("SUS-16-039, 3l ("+flavDem+", "+xl+"=0.5)", folder+"ewkino_sus16_039_bf0p5_x0p5_gr.root", 
  		    c5050, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 3l ("+tauRich+", "+xl+"=0.5)", folder+"ewkino_sus16_039_bf1_x0p5_gr.root", 
  		    c10050, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 3l ("+tauDom+", "+xl+"=0.5)",folder+"ewkino_sus16_039_bftau_x0p5_gr.root", 
  		    ctau50, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 2l SS + #geq3l (WH)", folder+"ewkino_sus16_039_wh_gr.root", 
  		    cwh, "gr_obs", "gr_exp", 124.5);
  models.back().add("SUS-16-043, 1l (WH)", folder+"ewkino_sus16_043_wh.root", 
  		    cwh43, "graph_smoothed_Obs", "graph_smoothed_Exp", 124.5);
  models.back().add("SUS-16-034, 2l OS (WZ)", folder+"ewkino_sus16_034_wz.root", 
  		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 9999);
  models.back().add("SUS-16-039, 3l (WZ)", folder+"ewkino_sus16_039_wz_gr.root", 
  		    cwz39, "gr_obs", "gr_exp", 7);
  models.back().add("SUS-16-048, soft 2-lep (WZ)", folder+"ewkino_sus16_048_wz.root", 
   		    cwz48, "ex_obs_smoothed_graph", "ex_exp_smoothed_graph", 7);

  ///////////////////////////////    Defining EWK-slep plot    /////////////////////////////////
  models.push_back(model_limits("EWK-slep","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}", 0.8));
  models.back().lumi = "35.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 590);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 590);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{H}", 125, 850, false);

  models.back().add("SUS-16-039, 3l ("+tauRich+", "+xl+"=0.5)", folder+"ewkino_sus16_039_bf1_x0p5_gr.root", 
  		    c10050, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 3l ("+tauRich+", "+xl+"=0.05)",folder+"ewkino_sus16_039_bf1_x0p05_gr.root", 
  		    c10005, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 3l ("+tauRich+", "+xl+"=0.95)",folder+"ewkino_sus16_039_bf1_x0p95_gr.root", 
  		    c10095, "gr_obs_smoothed", "gr_exp_smoothed");

  models.back().add("SUS-16-039, 3l ("+flavDem+", "+xl+"=0.5)", folder+"ewkino_sus16_039_bf0p5_x0p5_gr.root", 
  		    c5050, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 2l SS + 3l ("+flavDem+", "+xl+"=0.05)",folder+"ewkino_sus16_039_bf0p5_x0p05_gr.root", 
  		    c5005, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 2l SS + 3l ("+flavDem+", "+xl+"=0.95)",folder+"ewkino_sus16_039_bf0p5_x0p95_gr.root", 
  		    c5095, "gr_obs_smoothed", "gr_exp_smoothed");

  models.back().add("SUS-16-039, 3l ("+tauDom+", "+xl+"=0.5)",folder+"ewkino_sus16_039_bftau_x0p5_gr.root", 
  		    ctau50, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 2l SS + #geq3l (WH)", folder+"ewkino_sus16_039_wh_gr.root", 
  		    cwh, "gr_obs", "gr_exp", 124.5);

  models.back().add("SUS-16-043, 1l (WH)", folder+"ewkino_sus16_043_wh.root", 
  		    cwh43, "graph_smoothed_Obs", "graph_smoothed_Exp", 124.5);
  models.back().add("SUS-16-034, 2l OS (WZ)", folder+"ewkino_sus16_034_wz.root", 
  		    cwz34, "gr_obs_smoothed", "gr_exp_smoothed", 9999);
  models.back().add("SUS-16-039, 3l (WZ)", folder+"ewkino_sus16_039_wz_gr.root", 
  		    cwz39, "gr_obs", "gr_exp", 7);
  models.back().add("SUS-16-048, soft 2-lep (WZ)", folder+"ewkino_sus16_048_wz.root", 
   		    cwz48, "ex_obs_smoothed_graph", "ex_exp_smoothed_graph", 7);

  ///////////////////////////////    Defining Sleptons plot    /////////////////////////////////
  models.push_back(model_limits("Sleptons","pp #rightarrow "+chi2+"#kern[0.3]{"+chipm+"}", 0.91));
  models.back().lumi = "35.9"; models.back().reverseOrder = false;
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}", 0, 590);
  models.back().addLine("m_{"+chipm+"} = m_{"+lsp+"}+m_{Z}", 91, 590);

  models.back().add("SUS-16-039, 3l ("+tauRich+", "+xl+"=0.5)", folder+"ewkino_sus16_039_bf1_x0p5_gr.root", 
  		    c10050, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 3l ("+tauRich+", "+xl+"=0.05)",folder+"ewkino_sus16_039_bf1_x0p05_gr.root", 
  		    c10005, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 3l ("+tauRich+", "+xl+"=0.95)",folder+"ewkino_sus16_039_bf1_x0p95_gr.root", 
  		    c10095, "gr_obs_smoothed", "gr_exp_smoothed");

  models.back().add("SUS-16-039, 3l ("+flavDem+", "+xl+"=0.5)", folder+"ewkino_sus16_039_bf0p5_x0p5_gr.root", 
  		    c5050, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 2l SS + 3l ("+flavDem+", "+xl+"=0.05)",folder+"ewkino_sus16_039_bf0p5_x0p05_gr.root", 
  		    c5005, "gr_obs_smoothed", "gr_exp_smoothed");
  models.back().add("SUS-16-039, 2l SS + 3l ("+flavDem+", "+xl+"=0.95)",folder+"ewkino_sus16_039_bf0p5_x0p95_gr.root", 
  		    c5095, "gr_obs_smoothed", "gr_exp_smoothed");

  models.back().add("SUS-16-039, 3l ("+tauDom+", "+xl+"=0.5)",folder+"ewkino_sus16_039_bftau_x0p5_gr.root", 
  		    ctau50, "gr_obs_smoothed", "gr_exp_smoothed");

  ///////////////////////////////    Defining T1tttt plot    /////////////////////////////////
  models.push_back(model_limits("T1tttt", basetitle+"t#kern[0.4]{#bar{t}}#kern[0.4]{"+lsp+"}"));
  models.back().lumi = "35.9"; 
  models.back().add("SUS-16-033, 0-lep ("+mht+")", folder+"t1tttt_sus16_033.root", 
  		    cSus16033, "ObsLim", "ExpLim");
  models.back().add("SUS-16-036, 0-lep ("+mt2+")", folder+"t1tttt_sus16_036.root", 
  		    cSus16036, "ObsLim", "ExpLim");
  // models.back().add("SUS-16-016, 0-lep ("+aT+")", folder+"t1tttt_sus16_016.root", 
  //  		    cSus16016, "observed_total", "expected_total");
  models.back().add("SUS-16-037, 1-lep ("+mj+")", folder+"t1tttt_sus16_037.root", 
   		    cSus16037, "graph_smoothed_Obs", "graph_smoothed_Exp");
  models.back().add("SUS-16-042, 1-lep ("+dphi+")", folder+"t1tttt_sus16_042.root", 
    		    cSus16042, "graph_smoothed_Obs", "graph_smoothed_Exp");
  models.back().add("SUS-16-035, #geq2-lep (SS)", folder+"t1tttt_sus16_035.root", 
   		    cSus16035, "ssobs", "ssexp");
  models.back().add("SUS-16-041, #geq3-lep", folder+"t1tttt_sus16_041.root", 
    		    kOrange, "contour_obs", "contour_exp");
  // models.back().add("SUS-16-030, 0-lep (top tag)", folder+"t1tttt_sus16_030.root", 
  // 		    cSus16030, "combined_obsExclOneTimesProspino_base_BR100pct", 
  // 		    "combined_expExclOneTimesProspino_base_BR100pct");
  // models.back().add("SUS-15-002, 0-lep ("+mht+"), 2.3 "+ifb+energy, folder+"t1tttt_sus15_002.root", 
  // 		    cBenchmark, "ObsLim", "ExpLim");


  ///////////////////////////////    Defining T1bbbb plot    /////////////////////////////////
  models.push_back(model_limits("T1bbbb", basetitle+"b#kern[0.23]{#bar{b}}#kern[0.2]{"+lsp+"}"));
  models.back().lumi = "35.9"; 
  models.back().add("SUS-16-033 ("+mht+")", folder+"t1bbbb_sus16_033.root", 
   		    cSus16033, "ObsLim", "ExpLim");
  models.back().add("SUS-16-036 ("+mt2+")", folder+"t1bbbb_sus16_036.root", 
  		    cSus16036, "ObsLim", "ExpLim");
  // models.back().add("SUS-16-016 ("+aT+")", folder+"t1bbbb_sus16_016.root", 
  //  		    cSus16016, "observed_total", "expected_total");
  // models.back().add("SUS-15-003 ("+mt2+"), 2.3 "+ifb+energy, folder+"t1bbbb_sus15_003.root", 
  // 		    cBenchmark, "gr_obs_smoothed", "gr_exp_smoothed");


  ///////////////////////////////    Defining T1qqqq plot    /////////////////////////////////
  models.push_back(model_limits("T1qqqq", basetitle+"q#kern[0.23]{#bar{q}}#kern[0.2]{"+lsp+"}"));
  models.back().lumi = "35.9"; 
  models.back().add("SUS-16-033 ("+mht+")", folder+"t1qqqq_sus16_033.root", 
  		    cSus16033, "ObsLim", "ExpLim");
  models.back().add("SUS-16-036 ("+mt2+")", folder+"t1qqqq_sus16_036.root", 
  		    cSus16036, "ObsLim", "ExpLim");
  // models.back().add("SUS-16-016 ("+aT+")", folder+"t1qqqq_sus16_016.root", 
  //  		    cSus16016, "observed_total", "expected_total");
  // models.back().add("SUS-15-003 ("+mt2+"), 2.3 "+ifb+energy, folder+"t1qqqq_sus15_003.root", 
  // 		    cBenchmark, "gr_obs_smoothed", "gr_exp_smoothed");

  ///////////////////////////////    Defining T5qqqqVV plot    /////////////////////////////////
  models.push_back(model_limits("T5qqqqVV", basetitle+"q#kern[0.2]{#bar{q}}#kern[0.2]{V}#kern[0.2]{"+lsp+"}"));
  models.back().lumi = "35.9"; 
  models.back().labMass = "m_{"+chipm+", "+chi2+"} = 0.5(m_{#lower[-0.12]{#tilde{g}}} + m_{"+lsp+"})";
  models.back().add("V = W/Z: SUS-16-033, 0-lep ("+mht+")", folder+"t5qqqqvv_sus16_033.root", 
  		    cSus16033, "ObsLim", "ExpLim");
  models.back().add("V = W: SUS-16-042, 1-lep ("+dphi+")", folder+"t5qqqqvv_sus16_042_gr.root", 
  		    cSus16042, "Index6", "Index3");
  models.back().add("V = W: SUS-16-035, #geq2-lep (SS)", folder+"t5qqqqvv_sus16_035.root", 
  		    cSus16035, "ssobs", "ssexp");



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
    TString xtitle = getModelParams(mod.model, Xmin, Xmax, Ymin, Ymax, glu_lsp);
    if(mod.Xmin != -99) Xmin = mod.Xmin; if(mod.Xmax != -99) Xmax = mod.Xmax;
    if(mod.Ymin != -99) Ymin = mod.Ymin; if(mod.Ymax != -99) Ymax = mod.Ymax;

    TH2D hbase = baseHistogram(Xmin, Xmax, Ymin, Ymax, xtitle);
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
      cout<<"Loading "<<mod.files[file]<<endl;
      TFile flimit(mod.files[file]);
      exp[file] = getGraph(flimit, mod.expnames[file]);
      obs[file] = getGraph(flimit, mod.obsnames[file]);
    }
    cout<<endl<<"Model "<<mod.model<<endl<<"============================="<<endl;
    for(size_t file(0); file < ncurves; file++){
      if (debug) cout<<"Doing "<<mod.files[file]<<endl;
      float mod_gl = glu_lsp;
      if(mod.glu_lsps[file]>0) mod_gl = mod.glu_lsps[file];

      if(mod.labels[file].Contains("16-048")){
	changeDmCoordinates(exp[file]);
	changeDmCoordinates(obs[file]);
      }
      setGraphStyle(exp[file], mod.colors[file], 2, LineWidth, mod_gl, debug);
      setGraphStyle(obs[file], mod.colors[file], 1, LineWidth, mod_gl, debug);
      //printExclGlu(obs[file], exp[file], mLSPs, mod.labels[file]);
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


TString getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp){
  TString xtitle = "m#kern[0.12]{_{#lower[-0.12]{#tilde{g}}}}";
  glu_lsp = 25; xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  if(model.Contains("Sleptons")){
    Xmin = 100; Xmax = 1250;
    Ymin = 0;   Ymax = 1350;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model.Contains("EWK-slep")){
    Xmin = 100; Xmax = 1250;
    Ymin = 0;   Ymax = 1570;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model.Contains("EWK-slep-0p5")){
    Xmin = 100; Xmax = 1250;
    Ymin = 0;   Ymax = 1260;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model == "EWKino"){
    Xmin = 100; Xmax = 650;
    Ymin = 0;   Ymax = 470;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model == "WZ"){
    Xmin = 100; Xmax = 650;
    Ymin = 0;   Ymax = 390;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model == "WH"){
    Xmin = 120; Xmax = 550;
    Ymin = 0;   Ymax = 190;
    glu_lsp = 25;
    xtitle = "m_{"+chi2+"} = m_{"+chipm+"}";
  }
  if(model.Contains("T1tttt")){
    Xmin = 700; Xmax = 2200;
    Ymin = 0;   Ymax = 2050;
    glu_lsp = 225;
  }
  if(model.Contains("T1tttt_leptonic") || model.Contains("T1tttt_hadronic")){
    Xmin = 700; Xmax = 2000;
    Ymin = 0;   Ymax = 1675;
    glu_lsp = 225;
  }
  if(model == "T1bbbb"){
    Xmin = 700; Xmax = 2200;
    Ymin = 0;   Ymax = 1950;
    glu_lsp = 25;
  }    
  if(model == "T1qqqq"){
    Xmin = 700; Xmax = 2200;
    Ymin = 0;   Ymax = 1750;
    glu_lsp = 25;
  }    
  if(model.Contains("T5qqqqVV")){
    Xmin = 700; Xmax = 2200;
    Ymin = 0;   Ymax = 1850;
    glu_lsp = 125;
  }    
  return xtitle;
}

void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title, TString lumi){
  TLatex label; label.SetNDC();  
  // Printing CMS Preliminary
  double offsetx(0.025), ycms(1-tMargin-0.075);
  label.SetTextAlign(11); label.SetTextFont(61); label.SetTextSize(0.75*tMargin);
  label.DrawLatex(lMargin+offsetx, ycms, "CMS");
  label.SetTextAlign(11); label.SetTextFont(52); label.SetTextSize(0.038);
  //if(do_prelim) label.DrawLatex(0.27+offsetx, ycms, "Preliminary");
  label.DrawLatex(0.27+offsetx, ycms, "Preliminary");

  // Printing top title
  label.SetTextAlign(22); label.SetTextFont(42); label.SetTextSize(0.6*tMargin);
  label.DrawLatex((1-rMargin-lMargin)/2.+lMargin-0.05, 1-tMargin/2., title);
  // Printing date
  label.SetTextAlign(31); label.SetTextFont(52); label.SetTextSize(0.45*tMargin);
  if(do_prelim) label.DrawLatex(1-rMargin-0.02, 1-tMargin+0.018, "Moriond 2017");
  // Printing energy
  label.SetTextAlign(31); label.SetTextFont(42); label.SetTextSize(0.53*tMargin);
  if(lumi=="") lumi = "13 TeV";
  else lumi += " "+ifb+" (13 TeV)";
  label.DrawLatex(1-rMargin-0.04, ycms+0.005, lumi);
}

