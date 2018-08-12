#include "../include/analysisManager.h"
using namespace std;

// initialization list construction
analysisManager::analysisManager(const char* infile_path, 
    double theta_min, double theta_max, 
    double scint_threshold, const char* outfile_path) : 
    infile_path(infile_path), theta_min(theta_min), theta_max(theta_max), 
    scint_threshold(scint_threshold), outfile_path(outfile_path) {

    welcome();
    initializeIO(infile_path, outfile_path);

}

// destructor has to close files and cleanup IO
analysisManager::~analysisManager(){
    cout << "closing files..." << endl;
    this->infile->Close();
    this->outfile->Close();
}


// open files, set up file pointers and check them
void analysisManager::initializeIO(const char* infile_path, 
    const char* outfile_path) {
    
    cout << "initializing IO...\n";

    this->infile = new TFile(infile_path, "READ");
    this->outfile = new TFile(outfile_path, "RECREATE");

    checkFile(infile, infile_path);
    checkFile(outfile, outfile_path); 

    cout << "IO initialzied...\n";
}

// helper function that looks for errors in setting up IO 
// dies and complains if shit is bad
void analysisManager::checkFile(TFile* file, const char* filename) {
    if(!file){
        cout << "filepointer null...\n";
        exit(-1);
    }
    if(file->IsZombie()){
        cout << filename << " is bunk!\n";
        exit(-1);
    }
}

// parse input files and fill TTrees
// ~~~MAKES CUTS~~~~
// returns number of events passing cuts
int analysisManager::parseInfile(){
    
    int num_processed = 0;

    TTreeReader reader("T",this->infile);

    //bool theta_pass, doca_pass, vertex_pass, blsc_pass, veto_pass,
    //  in_signal_region;
    
    cout << "Parsing input file...\n";
    
    // ADD BRANCHES TO READ HERE
    TTreeReaderValue<double> recon_theta(reader, "recon_theta");
    TTreeReaderValue<double> doca(reader, "doca");
    TTreeReaderValue<TVector3> vertex(reader, "recon_vertex");
    TTreeReaderValue<double> weight(reader, "weight");
    TTreeReaderValue<bool> hit_veto(reader, "VSC_hit");
    // blm -> beam line monitor
    TTreeReaderValue<bool> hit_blm(reader, "BLSC_hit");
    TTreeReaderValue<bool> tgt_scint_hit(reader, 
        "CHAMBER_VETO_scint_fired");
    TTreeReaderValue<double> tgt_scint_edep(reader, 
        "CHAMBER_VETO_scints_Edep");
    
    // iterate over all events, make cuts, and save good ones
    /* HERE COME THOSE CUTS */
    return num_processed;
}

// add cuts to the analysis manager in the form of names
// feels fucking clumsy but I'm too stupid to do better
// returns 0 if no problem, 1 if problem
bool analysisManager::addCut(string cutName){
    bool cut_ok = false;
    if(cutName.empty()){
        cout << "tried to add cut without name!\n";
    }
    else if(find(this->recognized_cuts.begin(),
        this->recognized_cuts.end(), cutName) == 
        this->recognized_cuts.end()){
        cout << cutName << " not in recognized list!\n";
        cout << "add the cut name or correct the typo!\n";
    }
    else{
        cuts.push_back(cutName);
        cout << "successfully added cut: " << cutName << endl;
        cut_ok = true;
    }
    return cut_ok;
}


// fuck no clue what I'm doing
//bool passCuts(vector<string> cuts){

//}

// print welcome
void analysisManager::welcome(){
    cout << endl;
    cout << "analysisManager initialized...\n";
    cout << "inFile: " << infile << endl;
    cout << "theta range: " << theta_min << " degrees to  " << theta_max <<
        " degrees" << endl;
    cout << "scintillator threshold: " << scint_threshold << endl;
    cout << "outfile: " << outfile << endl;
    cout << endl;
}
