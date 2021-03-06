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
#include <time.h>
#include <iostream>
#include <assert.h>
#include "../include/allCuts.h"
#include "../include/eventObj.h"

#define ROI_THETA_MIN 20
#define ROI_THETA_MAX 100
#define ROI_DOCA_MIN 0
#define ROI_DOCA_MAX 25 
#define ROI_WEIGHT_MIN 0
#define ROI_WEIGHT_MAX 0.01
#define ROI_VERTEX_X_MIN -50
#define ROI_VERTEX_X_MAX 50
#define ROI_VERTEX_Y_MIN -50
#define ROI_VERTEX_Y_MAX 50
#define ROI_VERTEX_Z_MIN -50
#define ROI_VERTEX_Z_MAX 50
#define ROI_GEM_RADIAL_DIST_MIN 0
#define ROI_GEM_RADIAL_DIST_MAX 40
#define BIN_NUM 80
#define MAX_NAME_SIZE 80

class analysisManager {
    public: 
        analysisManager(std::vector<const char*> infile_paths, 
            const char* outfile_path, int verbose);

        ~analysisManager();

        int pruneInputTrees();

        bool addCut(cut* cutName);
        void debugAllCuts();
        void debugPrunedHistograms();
        void writePrunedHistos();

        std::vector<const char*> infile_paths;
        const char* outfile_path;
        const int verbose;

        std::vector<std::vector<TH1D*>> pruned_histograms;

    private: 
        void welcome();
        void initializeIO(std::vector<const char*> infile_paths, 
            const char* outfile_path);
        void cleanupIO();

        int pruneInputTree(TFile* infile, int infile_num);

        void checkFile(TFile* file, const char* filename);
        void checkFiles(std::vector<TFile*> files, 
            std::vector<const char*> filenames);
        void debugInfiles();
        void generatePrunedHistograms(int infile_num);
        void addEventToPrunedHistos(eventObj* eventToAdd, 
            int infile_num);
        bool applyAllCuts(eventObj* this_event);

        std::vector<cut*> cuts;
        std::vector<TFile*> infiles;

        //TFile* infile;
        TFile* outfile;
};
#endif 
