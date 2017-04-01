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
#include "utilities.hpp"
#include "model_limits.hpp"

using namespace std;
namespace{
  //double cmsH = 0.075;
  double cmsH = 0.03;
  float legLineH = 0.052;
  float legTextSize = 0.0375;
  float fillTransparency = 0.08;

  TString lsp = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  // int c8TeV(kGray+2);
  // int cSus15002(kBlue), cSus15003(kOrange), cSus15004(kGreen+1), cSus15005(kMagenta+1);
  // int cSus15004_1l(kBlack), cSus15007(kRed), cSus15008(kCyan+2);
}

TString getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp);
void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title);

int main(){
  gErrorIgnoreLevel=kWarning; // Turns off ROOT INFO messages

  // Label definitions
  //TString lsp("#tilde{#chi}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}");
  TString pp_gluglu("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}");
  TString basetitle(pp_gluglu+",  #tilde{g} #rightarrow ");
  TString mj("M#lower[-.1]{_{J}}");
  //TString mt2("M#lower[-.1]{_{T2}}"), mht("#slash{H}#lower[-.1]{_{T}}"), aT("#alpha#lower[-.1]{_{T}}");

  // Folder with root files containing the TGraphs
  TString folder("config/SUS16037/");
  vector<model_limits> models;

  ///////////////////////////////    Defining T1tttt plot    /////////////////////////////////
  models.push_back(model_limits("T1tttt", pp_gluglu));
  models.back().add("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}, #tilde{g} #rightarrow t#kern[0.4]{#bar{t}}#kern[0.4]{"+lsp+"}", 
		    folder+"T1tttt_results.root", 
  		    kRed-4, "graph_smoothed_Obs", "graph_smoothed_Exp");
  models.back().add("pp #rightarrow #tilde{g}#kern[0.3]{#tilde{g}}, #tilde{g} #rightarrow #tilde{t}_{1}t,  #tilde{t}_{1} #rightarrow #bar{t}#kern[0.4]{"
		    +lsp+"}   (m#kern[0.3]{_{#lower[-0.12]{#tilde{t}_{1}}}} - m#kern[0.12]{_{"+lsp+"}} = 175 GeV)", 
		    folder+"T5tttt_results.root", 
  		    kBlue+1, "graph_smoothed_Obs", "graph_smoothed_Exp");


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
  double legX(1-rMargin-0.04), legY(1-tMargin-0.13);
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
    TString xtitle = getModelParams(mod.model, Xmin, Xmax, Ymin, Ymax, glu_lsp);

    TH2D hbase = baseHistogram(Xmin, Xmax, Ymin, Ymax, xtitle);
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
      if(mod.labels[file].Contains("175")) glu_lsp += 40;
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
    legX = lMargin+0.03; legY = 1-tMargin-cmsH;
    legW = 0.13; 
    legH = legLineH * ncurves;
    TLegend limleg(legX, legY-legH, legX+legW, legY);
    limleg.SetTextSize(legTextSize); limleg.SetFillColor(0); 
    limleg.SetFillStyle(0); limleg.SetBorderSize(0);
    for(size_t file(0); file < ncurves; file++)
      limleg.AddEntry(obs[file]->GetName(), mod.labels[file], "fl");
    limleg.Draw();

    legY = 1-legY-legH-0.02-0.1; legH = 0.07;
    obsleg.SetY1NDC(legY-legH); obsleg.SetY2NDC(legY);
    baseleg.SetY1NDC(legY-legH); baseleg.SetY2NDC(legY);
    baseleg.Draw();
    obsleg.Draw();

    TString plotname(mod.model+"_limits_summary_cms.pdf");
    can.SaveAs(plotname);
    cout<<" open "<<plotname<<endl;
  } // Loop over models
  cout<<endl<<endl;
}

TString getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp){
  TString xtitle = "m#kern[0.12]{_{#lower[-0.12]{#tilde{g}}}}";
  if(model.Contains("T1tttt")){
    Xmin = 700; Xmax = 2200;
    Ymin = 0;   Ymax = 2050;
    glu_lsp = 225;
  }
  return xtitle;
}

void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title){
  TLatex label; label.SetNDC();  
  // Printing CMS Preliminary
  double offsetx(0.0), ycms(1-tMargin-cmsH);
  label.SetTextAlign(12); label.SetTextFont(61); label.SetTextSize(0.75*tMargin);
  label.DrawLatex(lMargin+offsetx, 1-tMargin/2., "CMS");
  label.SetTextAlign(12); label.SetTextFont(52); label.SetTextSize(0.038);
  label.DrawLatex(0.27+offsetx, 1-tMargin/2.-0.013, "Preliminary");
  // Printing top title
  label.SetTextAlign(21); label.SetTextFont(42); label.SetTextSize(0.6*tMargin);
  //label.DrawLatex((1-rMargin-lMargin)/2.+lMargin-0.05, 1-tMargin/2., title);
  //label.DrawLatex((1-rMargin-lMargin)/2.+lMargin-0.05-0.25, ycms, title);
  // Printing date
  label.SetTextAlign(31); label.SetTextFont(42); label.SetTextSize(0.6*tMargin);
  label.DrawLatex(1-rMargin, 1-tMargin+0.018, "35.9 fb^{-1} (13 TeV)");
}

