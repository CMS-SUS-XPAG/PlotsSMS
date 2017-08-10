#include "model_limits.hpp"

using namespace std;

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
  legScale(ilegScale),
  Xmin(-99),
  Xmax(-99),
  Ymin(-99),
  Ymax(-99){
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

void model_limits::setRanges(float iXmin, float iXmax, float iYmin, float iYmax){
  Xmin = iXmin;
  Xmax = iXmax;
  Ymin = iYmin;
  Ymax = iYmax;
}

