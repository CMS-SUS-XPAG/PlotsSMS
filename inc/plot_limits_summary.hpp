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

class model_limits {
public:
  TString model, title;
  float legScale;
  std::vector<TString> labels, files, obsnames, expnames;
  std::vector<int> colors;

  void add(TString label, TString file, int color, TString obsname, TString expname);
  model_limits(TString imodel, TString ititle, float ilegScale=1.);
};

void setCanvas(TCanvas &can, float lMargin, float tMargin, float rMargin, float bMargin);
TH2D baseHistogram(float Xmin, float Xmax, float Ymin, float Ymax);
void addLabelsTitle(float lMargin, float tMargin, float rMargin, TString title);
TGraph* getGraph(TFile &flimit, TString gname);
void setGraphStyle(TGraph *graph, int color, int style, int width, double glu_lsp);
void getModelParams(TString model, float &Xmin, float &Xmax, float &Ymin, float &Ymax, float &glu_lsp);
void reverseGraph(TGraph *graph);

#endif
