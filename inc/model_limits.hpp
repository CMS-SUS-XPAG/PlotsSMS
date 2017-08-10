#ifndef H_MODEL_LIMITS
#define H_MODEL_LIMITS

// System includes
#include <iostream>
#include <vector>

// ROOT includes
#include "TString.h"

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
  float Xmin, Xmax, Ymin, Ymax;

  void add(TString label, TString file, int color, TString obsname, TString expname, float glu_lsp=-1.);
  void addLine(TString ilabel, float idm, float imaxHeight, bool above=true);
  void setRanges(float iXmin, float iXmax, float iYmin, float iYmax);
  model_limits(TString imodel, TString ititle, float ilegScale=1.);
};



#endif
