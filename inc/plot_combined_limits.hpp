// plot_combine_limits: Plots various limit curves on same canvas

#ifndef H_PLOT_COMBINED_LIMITS
#define H_PLOT_COMBINED_LIMITS

// System includes
#include <iostream>
#include <vector>

// ROOT includes
#include "TString.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TGraph.h"

class model_limits {
public:
  TString model, title;
  std::vector<TString> labels, files, obsnames, expnames;
  std::vector<int> colors;

  void add(TString label, TString file, TString obsname, TString expname, int color);
  model_limits(TString imodel, TString ititle);
};

void setCanvas(TCanvas &can, float lMargin, float tMargin, float rMargin, float bMargin);
TH2D baseHistogram(float Xmin, float Xmax, float Ymin, float Ymax);
void drawCMSLumi(float lMargin, float tMargin, float rMargin);
void setGraph(TGraph *graph, int color, int style, int width, double glu_lsp);

#endif
