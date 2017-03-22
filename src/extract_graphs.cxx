//// Extracts TGraphs from files that have TCanvas

// System includes
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>  // setw
#include <getopt.h>

// ROOT includes
#include "TGraph.h"
#include "TString.h"
#include "TCanvas.h"
#include "TError.h" // Controls error level reporting
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TSystem.h"
#include "TKey.h"
#include "TFile.h"

using namespace std;
namespace{
  TString infolder = "";
}

void GetOptions(int argc, char *argv[]);
vector<TString> dirlist(const TString &folder, const TString &inname, const TString &tag="");


int main(int argc, char *argv[]){
  GetOptions(argc, argv);

  vector<TString> files = dirlist(infolder, "root");

  for(unsigned ind=0; ind<files.size(); ind++){
    cout<<endl<<"Opening "<<files[ind]<<endl;
    TString inname = infolder+"/"+files[ind];
    TFile source(inname);
    TKey *key;
    TIter nextkey(source.GetListOfKeys());
    while ((key = static_cast<TKey*>(nextkey()))) {
      TString className = key->GetClassName();
      if(className.Contains("TCanvas")){
	TString outname = inname; outname.ReplaceAll(".root","_gr.root");
	TFile outfile(outname, "recreate"); outfile.cd();
	TCanvas *c1 = static_cast<TCanvas*>(source.Get(key->GetName()));
	TObject *obj; 
	TIter next(c1->GetListOfPrimitives());
	while ((obj=next())) {
	  if(obj->InheritsFrom("TGraph")) obj->Write();
	} // Loop over TCanvas objects  
	outfile.Close();
	cout<<"Saved TGraphs from "<<inname<<" into "<<outname<<endl;
      } // It is a TCanvas
    } // Loop over contents of file
    
  }// Loop over files
}

// Returns list of directorites or files in folder
vector<TString> dirlist(const TString &folder, const TString &inname, const TString &tag){
  TString pwd(gSystem->pwd());
  vector<TString> v_dirs;
  TSystemDirectory dir(folder, folder);
  TList *files = dir.GetListOfFiles();
  if (files) {
    TSystemFile *file;
    TString fname;
    TIter next(files);
    while ((file=static_cast<TSystemFile*>(next()))) {
      fname = file->GetName();
      if (inname=="dir") {
        if ((file->IsDirectory() && !fname.Contains(".") && fname.Contains(tag))) v_dirs.push_back(fname);
      } else  if(fname.Contains(inname)) v_dirs.push_back(fname);
    }
  } // if(files)
  gSystem->cd(pwd); // The TSystemDirectory object seems to change current folder
  return v_dirs;
}


void GetOptions(int argc, char *argv[]){
  while(true){
    static struct option long_options[] = {
      {"infolder", required_argument, 0, 'f'},
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "f:w", long_options, &option_index);
    if( opt == -1) break;

    string optname;
    switch(opt){
    case 'f':
      infolder = optarg;
      break;
    default:
      printf("Bad option! getopt_long returned character code 0%o\n", opt);
      break;
    }
  }
}
