#include "utilities.hpp"
#include "TMultiGraph.h"
#include "TLatex.h"

using namespace std;

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

void setGraphStyle(TGraph* graph, int color, int style, int width, double glu_lsp, bool debug){
  if(graph==0) return;

  // Setting graph style
  graph->SetLineColor(color);
  graph->SetLineStyle(style);
  int fillcolor(color);
  graph->SetFillColor(fillcolor);
  // if(!no_shade) graph->SetFillColorAlpha(fillcolor, fillTransparency);
  // else {
  //   if(color == cBenchmark) graph->SetFillColorAlpha(fillcolor, 0.35);
  //   else graph->SetFillColorAlpha(fillcolor, 0);
  // }
  graph->SetFillColorAlpha(fillcolor, 0);
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
    if(mlsp > mglu-glu_lsp && glu_lsp<1000){
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
  if(glu_lsp<1000) graph->SetPoint(graph->GetN(), 0, -glu_lsp);
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

void changeDmCoordinates(TGraph *graph){
  int np(graph->GetN());
  double mglu, mlsp;
  for(int point(0); point < np; point++){
    graph->GetPoint(point, mglu, mlsp);
    graph->SetPoint(point, mglu, mglu-mlsp);
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


void setCanvas(TCanvas &can, float lMargin, float tMargin, float rMargin, float bMargin){
  can.SetLogz();
  can.SetTickx(1);
  can.SetTicky(1);
  can.SetLeftMargin(lMargin);
  can.SetTopMargin(tMargin);
  can.SetRightMargin(rMargin);
  can.SetBottomMargin(bMargin);
}

TH2D baseHistogram(float Xmin, float Xmax, float Ymin, float Ymax, TString xtitle){
  TString lsp = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  TH2D hbase("hbase", "", 1, Xmin, Xmax, 1, Ymin, Ymax);
  hbase.GetXaxis()->SetLabelFont(42);
  hbase.GetXaxis()->SetLabelSize(0.035);
  hbase.GetXaxis()->SetTitleFont(42);
  hbase.GetXaxis()->SetTitleSize(0.05);
  hbase.GetXaxis()->SetTitleOffset(1.2);
  hbase.GetXaxis()->SetLabelOffset(0.009);
  hbase.GetXaxis()->SetTitle(xtitle+" [GeV]");

  hbase.GetYaxis()->SetLabelFont(42);
  hbase.GetYaxis()->SetLabelSize(0.035);
  hbase.GetYaxis()->SetTitleFont(42);
  hbase.GetYaxis()->SetTitleSize(0.05);
  hbase.GetYaxis()->SetTitleOffset(1.35);
  hbase.GetYaxis()->SetTitle("m#kern[0.12]{_{"+lsp+"}} [GeV]");
  return hbase;
}

