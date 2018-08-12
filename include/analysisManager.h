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

using namespace std;

class analysisManager {
    public: 
        analysisManager(const char* infile_path, double theta_min, 
            double theta_max, double scint_threshold, 
            const char* outfile_path);

        ~analysisManager();
        int parseInfile();

        const char* infile_path;
        const double theta_min; /* degrees */
        const double theta_max; /* degrees */
        const double scint_threshold; /* UNITS */
        const char* outfile_path;
    
    // histograms will go here
    // still not used to all the expressions
        TH1D* post_cut_data = new TH1D("post_cut_data", "",
            80, 20, 100);
    
    private: 
        void welcome();
        void initializeIO(const char* infile_path, 
            const char* outfile_path);

        void checkFile(TFile* file, const char* filename);

        TFile* infile;
        TFile* outfile;
};

#endif 
