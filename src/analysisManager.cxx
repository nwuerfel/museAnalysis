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
    std::cout << "deleting TFile pointers..." << std::endl;
    delete this->infile;
    delete this->outfile;

    // clear cuts we allocated space for in the master
    std::cout << "deleting cuts..." << std::endl;
    for(std::vector<int>::size_type i = 0; i < cuts.size(); i++){
        delete cuts[i];
    }

    // clear the eventObjects we made 
    std::cout << "deleting pruned_events..." << std::endl;
    for(std::vector<int>::size_type i = 0; 
        i < pruned_event_list.size(); i++){
        delete pruned_event_list[i];
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
    TTreeReaderValue<TVector3> recon_tgt_hit_pos(reader,
        "recon_tgt_hit_pos");

    while(reader.Next()){

        // ~~~ INITIALIZE EVENT ~~~
        eventObj* this_event = new eventObj(); 

        this_event->event.event_id = event_number;
        // convert to degrees
        this_event->event.theta = *recon_theta * 180/M_PI;
        this_event->event.doca = *doca;
        this_event->event.weight = *weight;
        this_event->event.vertex_x = vertex->x();
        this_event->event.vertex_y = vertex->y();
        this_event->event.vertex_z = vertex->z();
        // gives the radial distance from the center of the gem
        // TODO which gem?
        this_event->event.gem_radial_dist = recon_tgt_hit_pos->Perp();
        this_event->event.hit_veto = *hit_veto;
        this_event->event.hit_blsc = *hit_blsc;
        this_event->event.frame_hit = *frame_hit;
        this_event->event.frame_hit = *tgt_event;
        // ~~~ E O I ~~~
    

        // invoke cuts on the event 
        passed_cuts = applyAllCuts(this_event);
        if(verbose)
            std::cout << "passed cuts: " << passed_cuts << std::endl;

        // Debug
        if(verbose && recon_tgt_hit_pos->Perp() > 34){
            this_event->debug();
        }
        // EOD

        if(passed_cuts){
            if(verbose){
                std::cout << "added event# " << event_number 
                    << " to pruned event list\n";
            }
            pruned_event_list.push_back(this_event);
        }
        // free memory from unsaved events
        else{
            delete this_event;
        }
        //TODO remove
        if(event_number == 100)
            break;
        event_number++;
    }
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
