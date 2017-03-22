// plot_limits_summary: Plots various limit curves on same canvas

#ifndef H_PLOT_LIMITS_SUMMARY
#define H_PLOT_LIMITS_SUMMARY

// System includes
#include <iostream>
#include <vector>

// ROOT includes
#include "TString.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TFile.h"

class dm_line {
public:
  TString label;
  float dm, maxHeight;
  bool above;

  dm_line(TString ilabel, float idm, float imaxHeight, bool iabove=true);
};

class model_limits {
public:
  TString model, title, labMass;
  TString lumi;
  float legScale;
  bool reverseOrder;
  std::vector<TString> labels, files, obsnames, expnames;
  std::vector<int> colors;
  std::vector<float> glu_lsps;
  std::vector<dm_line> lines;
  
  void add(TString label, TString file, int color, TString obsname, TString expname, float glu_lsp=-1.);
  void addLine(TString ilabel, float idm, float imaxHeight, bool above=true);
  model_limits(TString imodel, TString ititle, float ilegScale=1.);
};

void setCanvas(TCanvas &can, float lMargin, float tMargin, float rMargin, float bMargin);
TH2D baseHistogram(float Xmin, float Xmax, float Ymin, float Ymax);
void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title, TString lumi="");
TGraph* getGraph(TFile &flimit, TString gname);
void setGraphStyle(TGraph *graph, int color, int style, int width, double glu_lsp);
void getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp);
void reverseGraph(TGraph *graph);
void changeDmCoordinates(TGraph *graph);
void makeContinuousGraph(TGraph *graph);
void printExclGlu(TGraph *gobs, TGraph *gexp, std::vector<float> &mLSPs, TString label);
std::vector<float> intersectionLSP(TGraph *graph, std::vector<float> &mLSPs);

#endif
