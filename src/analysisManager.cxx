#include "../include/analysisManager.h"
#include "../include/eventObj.h"

// initialization list construction
analysisManager::analysisManager(const char* infile_path, 
    const char* outfile_path, bool verbose) : 
    infile_path(infile_path),  outfile_path(outfile_path),
    verbose(verbose){
    welcome();
    initializeIO(infile_path, outfile_path);

}

// destructor has to close files and cleanup IO
analysisManager::~analysisManager(){
    std::cout << "closing files..." << std::endl;
    this->infile->Close();
    this->outfile->Close();
}


// open files, set up file pointers and check them
void analysisManager::initializeIO(const char* infile_path, 
    const char* outfile_path) {
    
    std::cout << "initializing IO...\n";

    this->infile = new TFile(infile_path, "READ");
    this->outfile = new TFile(outfile_path, "RECREATE");

    checkFile(infile, infile_path);
    checkFile(outfile, outfile_path); 

    std::cout << "IO initialzied...\n";
}

// helper function that looks for errors in setting up IO 
// dies and complains if shit is bad
void analysisManager::checkFile(TFile* file, const char* filename) {
    if(!file){
        std::cout << "filepointer null...\n";
        exit(-1);
    }
    if(file->IsZombie()){
        std::cout << filename << " is bunk!\n";
        exit(-1);
    }
}

// parse input files and fill TTrees
// ~~~MAKES CUTS~~~~
// returns number of events passing cuts
// ADD branches here AND to eventObj.h
int analysisManager::pruneInputTree(){
    
    int num_processed = 0;
    bool passed_cuts = false;

    TTreeReader reader("T",this->infile);

    std::cout << "Parsing input file...\n";

    // ADD BRANCHES TO READ HERE
    // theta from anamuse output is in radians
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
        // convert to degrees
        this_event->theta = *recon_theta * 180/M_PI;
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
        std::cout << "passed cuts: " << passed_cuts << std::endl;

    }

    delete this_event;

    return num_processed;
}

// adds cuts to manager, returns true on success
bool analysisManager::addCut(cut* this_cut){
    bool cut_ok = true;
    if(!this_cut){
        if(verbose)
            std::cout << "tried to add NULL cut\n";
        cut_ok = false;
    }
    else{
        cuts.push_back(this_cut);
        if(verbose){
            std::cout << "successfully added cut: " 
                << this_cut->name << std::endl;
        }
    }
    
    return cut_ok;
}

// iterate over analysisManager's cut list and apply all cuts
// to an event 
bool analysisManager::applyAllCuts(eventObj* this_event){
    bool retval = true;
    bool cut_result;
    for(std::vector<int>::size_type i = 0; i != cuts.size(); i++){
        if(verbose){
            std::cout << "applying cut: " << cuts[i]->name
                << std::endl;
        }
        cut_result = cuts[i]->applyCut(this_event);
        retval = retval && cut_result;
    }
    return retval;
}

void analysisManager::welcome(){
    std::cout << std::endl;
    std::cout << "********************\n";
    std::cout << "analysisManager initialized...\n";
    std::cout << "inFile: " << infile_path << std::endl;
    std::cout << "outfile: " << outfile_path << std::endl;
    std::cout << "verbose: " << verbose << std::endl;
    std::cout << "********************\n";
    std::cout << std::endl;
}

void analysisManager::debugAllCuts(){

    for(std::vector<int>::size_type i = 0; i != cuts.size(); i++){
        std::cout << "cut #" << i << ": " << cuts[i]->name 
            << std::endl; 
        std::cout << "pointer is: " << cuts[i] << std::endl;
    }

}
