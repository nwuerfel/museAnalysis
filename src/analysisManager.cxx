#include "../include/analysisManager.h"
#include "../include/eventObj.h"
using namespace std;


// initialization list construction
analysisManager::analysisManager(const char* infile_path, 
    double theta_min, double theta_max, 
    double scint_threshold, const char* outfile_path, bool verbose) : 
    infile_path(infile_path), theta_min(theta_min), theta_max(theta_max), 
    scint_threshold(scint_threshold), outfile_path(outfile_path),
    verbose(verbose){

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
    bool passed_cuts = false;

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
        if(verbose)
            this_event->debug();
        if(num_processed == 3)
            break;
        // EOD

        // invoke cuts on the event 
        passed_cuts = applyAllCuts(this_event);
        cout << "passed cuts: " << passed_cuts << endl;

    }

    delete this_event;

    return num_processed;
}

// adds cuts to manager, returns true on success
bool analysisManager::addCut(cut* this_cut){
    bool cut_ok = true;
    if(!this_cut){
        cout << "tried to add NULL cut\n";
        cut_ok = false;
    }
    else{
        cuts.push_back(this_cut);
        cout << "successfully added cut: " << this_cut->name << endl;
    }
    
    return cut_ok;
}

// iterate over analysisManager's cut list and apply all cuts
// to an event 
bool analysisManager::applyAllCuts(eventObj* this_event){
    bool retval = true;
    bool cut_result;
    for(vector<int>::size_type i = 0; i != cuts.size(); i++){
        if(verbose)
            cout << "applying cut: " << cuts[i]->name << endl;
        cut_result = cuts[i]->applyCut(this_event);
        retval = retval && cut_result;
    }
    return retval;
}

void analysisManager::welcome(){
    cout << endl;
    cout << "analysisManager initialized...\n";
    cout << "inFile: " << infile << endl;
    cout << "theta range: " << theta_min << " degrees to  " 
        << theta_max << " degrees" << endl;
    cout << "scintillator threshold: " << scint_threshold << endl;
    cout << "outfile: " << outfile << endl;
    cout << endl;
}
