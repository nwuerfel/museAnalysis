#include "../include/analysisManager.h"

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

    // clear cuts we allocated space for in the master
    std::cout << "deleting cuts..." << std::endl;
    for(std::vector<int>::size_type i = 0; i < cuts.size(); i++){
        delete cuts[i];
    }
}


// open files, set up file pointers and check them
void analysisManager::initializeIO(const char* infile_path, 
    const char* outfile_path) {
    
    std::cout << "initializing IO...\n";

    this->infile = new TFile(infile_path, "READ");
    this->outfile = new TFile(outfile_path, "RECREATE");

    checkFile(infile, infile_path);
    checkFile(outfile, outfile_path); 

    std::cout << "IO initialized...\n";
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
    
    int event_number = 0;
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
    TTreeReaderValue<TVector3> vertex(reader, "recon_vertex");

    eventObj* this_event = new eventObj(); 

    while(reader.Next()){

        // ~~~ INITIALIZE EVENT ~~~
        this_event->event_id = event_number;
        // convert to degrees
        this_event->theta = *recon_theta * 180/M_PI;
        this_event->doca = *doca;
        this_event->weight = *weight;
        this_event->vertex_x = vertex->x();
        this_event->vertex_y = vertex->y();
        this_event->vertex_z = vertex->z();
        this_event->hit_veto = *hit_veto;
        this_event->hit_blsc = *hit_blsc;
        this_event->frame_hit = *frame_hit;
        this_event->frame_hit = *tgt_event;
        // ~~~ E O I ~~~
    

        // invoke cuts on the event 
        passed_cuts = applyAllCuts(this_event);
        if(verbose)
            std::cout << "passed cuts: " << passed_cuts << std::endl;

        // Debug
        if(verbose && *doca > 25)
            this_event->debug();
        // EOD


        //TODO remove
        if(event_number == 100)
            break;
 
        event_number++;
    }
    delete this_event;
    return event_number;
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
// to an event and ends if any cut fails
bool analysisManager::applyAllCuts(eventObj* this_event){
    bool retval = true;
    bool cut_ok;
    for(std::vector<int>::size_type i = 0; i != cuts.size(); i++){
        if(verbose){
            std::cout << "applying cut: " << cuts[i]->name
                << std::endl;
        }
        cut_ok = cuts[i]->applyCut(this_event);
        retval = retval && cut_ok;
        if(!cut_ok){
            if(verbose){
                std::cout << "failed cut: " << cuts[i]->name
                    << std::endl;
            }
            break;
        }
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
