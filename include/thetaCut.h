#ifndef THETACUT_H
#define THETACUT_H
#include <TLegend.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TApplication.h>
#include <TString.h>
#include <TFile.h>
#include <TVectorD.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TLine.h>
#include <TLatex.h>
#include <TGraph2D.h>
#include <TPaveText.h>
#include <TPaveLabel.h>
#include <TGaxis.h>
#include <TH1D.h>
#include <TH2D.h>
#include <iostream>
#include "../include/analysisTypes.h"
#include "../include/cut.h"

using namespace std;

class thetaCut : public cut {
    public:
        int applyCut(vector<named_branchiter> tree_data);
};

#endif
