#include "../include/analysisManager.h"
#include "../include/eventObj.h"
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
// ADD branches here AND to eventObj.h
int analysisManager::readPrunedTree(){
    
    int num_processed = 0;

    TTreeReader reader("T",this->infile);

    //bool theta_pass, doca_pass, vertex_pass, blsc_pass, veto_pass,
    //  in_signal_region;
    
    cout << "Parsing input file...\n";

    // named_branch iters have the iterator and branch name so that 
    // "plugin" cuts can determine whether they have the right fields
    
    // ADD BRANCHES TO READ HERE
    TTreeReaderValue<double> recon_theta(reader, "recon_theta");
    TTreeReaderValue<double> doca(reader, "recon_doca");
    TTreeReaderValue<double> weight(reader, "weight");
    TTreeReaderValue<bool> hit_veto(reader, "VSC_hit");
    // blsc -> beam line (monitor) scintillator
    TTreeReaderValue<bool> hit_blsc(reader, "BLSC_hit");
    TTreeReaderValue<bool> frame_hit(reader, "frame_hit");
    TTreeReaderValue<bool> tgt_event(reader, "CHAMBER_target_event");

    eventObj* this_event = new eventObj(); 

    while(reader.Next()){

        // ~~~ INITIALIZE EVENT ~~~
        this_event->theta = *recon_theta;
        this_event->doca = *doca;
        this_event->weight = *weight;
        this_event->hit_veto = *hit_veto;
        this_event->hit_blsc = *hit_blsc;
        this_event->frame_hit = *frame_hit;
        this_event->frame_hit = *tgt_event;
        // ~~~ E O I ~~~
    
        num_processed++;

        // Debug
        this_event->debug();
        if(num_processed == 3)
            break;
        // EOD

    }

    delete this_event;

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

//bool analysisManager::applyCuts(){
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
