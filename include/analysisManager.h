#ifndef ANALYSISMANAGER_H
#define ANALYSISMANAGER_H
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
#include "../include/allCuts.h"
#include "../include/eventObj.h"

class analysisManager {
    public: 
        analysisManager(const char* infile_path, 
            const char* outfile_path, bool verbose);

        ~analysisManager();

        int pruneInputTree();

        bool addCut(cut* cutName);
        void debugAllCuts();

        const char* infile_path;
        const char* outfile_path;
        const bool verbose;
    
    private: 
        void welcome();
        void initializeIO(const char* infile_path, 
            const char* outfile_path);

        void checkFile(TFile* file, const char* filename);
        bool applyAllCuts(eventObj* this_event);

        std::vector<cut*> cuts;

        TFile* infile;
        TFile* outfile;
};
#endif 
