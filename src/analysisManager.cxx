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

    // clear cuts we allocated space for in the master
    std::cout << "deleting cuts..." << std::endl;
    for(std::vector<int>::size_type i = 0; i < cuts.size(); i++){
        delete cuts[i];
    }

    // clear the histograms we made 
    std::cout << "deleting pruned_histograms..." << std::endl;
    for(std::vector<int>::size_type i = 0; 
        i < pruned_histograms.size(); i++){
        delete pruned_histograms[i];
    }

    // close up IO
    std::cout << "closing files..." << std::endl;
    infile->Close();
    outfile->Close();
    std::cout << "deleting TFile pointers..." << std::endl;
    delete infile;
    delete outfile;

}


// open files, set up file pointers and check them
void analysisManager::initializeIO(const char* infile_path, 
    const char* outfile_path) {
    std::cout << "initializing IO...\n";
    infile = new TFile(infile_path, "READ");
    outfile = new TFile(outfile_path, "RECREATE");
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

// parse input files and fill histograms
// ~~~MAKES CUTS~~~~
// returns number of events passing cuts
// ADD branches here AND to eventObj.h
int analysisManager::pruneInputTree(){
    
    int event_number = 0;
    int num_passed_cuts = 0;
    bool passed_cuts = false;

    TTreeReader reader("T",infile);

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

    // make histograms to save the events
    // allocates memory and gets cleaned up in destructor
    generatePrunedHistograms();

    eventObj* this_event = new eventObj(); 

    clock_t timer;

    timer = clock();

    while(reader.Next()){

        // ~~~ INITIALIZE EVENT ~~~

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
        //    std::cout << "passed cuts: " << passed_cuts << std::endl;

        if(passed_cuts){
            if(verbose){
//                std::cout << "added event #" << event_number 
//                    << " to pruned_histogram\n";
            }
            addEventToPrunedHistos(this_event);
            num_passed_cuts++;
        }
        event_number++;
    }
    
    timer = clock() - timer;

    if(verbose)
        std::cout << "handled " << event_number 
            << " events in " << timer << " cycles: " 
            << ((double)timer)/CLOCKS_PER_SEC << " seconds" 
            << std::endl;
    // gimme that memory back
    delete this_event;
    return num_passed_cuts;
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
        //    std::cout << "applying cut: " << cuts[i]->name
        //        << std::endl;
        }
        cut_ok = cuts[i]->applyCut(this_event);
        retval = retval && cut_ok;
        if(!cut_ok){
            if(verbose){
        //        std::cout << "failed cut: " << cuts[i]->name
        //            << std::endl;
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

void analysisManager::debugPrunedHistograms(){
    for(std::vector<int>::size_type i=0; i!=pruned_histograms.size(); i++){
        std::cout << "histogram #" << i << ": " 
            << pruned_histograms[i]->GetName() << std::endl;
        std::cout << "pointer is: " << pruned_histograms[i] << std::endl;
    }
}


// generates all required histograms so they can be filled while pruning
void analysisManager::generatePrunedHistograms(){

    std::cout << "generating histograms..." << std::endl;

    // ADD HISTOGRAMS TO ADD HERE
    // shouldn't fail but oh well, check in case
    TH1D* theta_hist = new TH1D("pruned_theta_distribution",
        "pruned_theta_distribution", BIN_NUM, ROI_THETA_MIN, 
        ROI_THETA_MAX);
    if(!theta_hist){
        std::cout << "theta_hist is bunk!\n";
        exit(-1);
    }
    TH1D* doca_hist = new TH1D("pruned_doca_distribution",
        "pruned_doca_distribution", BIN_NUM, ROI_DOCA_MIN, 
        ROI_DOCA_MAX);
    if(!doca_hist){
        std::cout << "doca_hist is bunk!\n";
        exit(-1);
    }
    TH1D* weight_hist = new TH1D("pruned_weight_distribution",
        "pruned_weight_distribution", BIN_NUM,
        ROI_WEIGHT_MIN, ROI_WEIGHT_MAX);
    if(!weight_hist){
        std::cout << "weight_hist is bunk!\n";
        exit(-1);
    }
    TH1D* vertex_x_hist = new TH1D("pruned_vertex_x_distribution",
        "pruned_vertex_x_distribution", BIN_NUM, ROI_VERTEX_X_MIN, 
        ROI_VERTEX_X_MAX);
    if(!vertex_x_hist){
        std::cout << "vertex_x_hist is bunk!\n";
        exit(-1);
    }
    TH1D* vertex_y_hist = new TH1D("pruned_vertex_y_distribution",
        "pruned_vertex_y_distribution", BIN_NUM, ROI_VERTEX_Y_MIN, 
        ROI_VERTEX_Y_MAX);
    if(!vertex_y_hist){
        std::cout << "vertex_y_hist is bunk!\n";
        exit(-1);
    }
    TH1D* vertex_z_hist = new TH1D("pruned_veretx_z_distribution",
        "pruned_veretx_z_distribution", BIN_NUM, ROI_VERTEX_Z_MIN, 
        ROI_VERTEX_Z_MAX);
    if(!vertex_z_hist){
        std::cout << "vertex_z_hist is bunk!\n";
        exit(-1);
    }
    TH1D* gem_radial_dist_hist = 
        new TH1D("pruned_gem_radial_dist_distribution", 
        "pruned_gem_radial_dist_distribution", BIN_NUM, 
        ROI_GEM_RADIAL_DIST_MIN, ROI_GEM_RADIAL_DIST_MAX);
    if(!gem_radial_dist_hist){
        std::cout << "gem_radial_dist_hist is bunk!\n";
        exit(-1);
    }

    // now push into the Q
    pruned_histograms.push_back(theta_hist);
    pruned_histograms.push_back(doca_hist);
    pruned_histograms.push_back(weight_hist);
    pruned_histograms.push_back(vertex_x_hist);
    pruned_histograms.push_back(vertex_y_hist);
    pruned_histograms.push_back(vertex_z_hist);
    pruned_histograms.push_back(gem_radial_dist_hist);
}


// adds fields in eventobject struct to the respective histograms
// unfortunately, depends on the order in which the histograms are added to
// the pruned_histograms vector, which increases complexity of adding a
// histogram, oh well
void analysisManager::addEventToPrunedHistos(eventObj* eventToAdd){

    if(!eventToAdd){
        std::cout << "tried to add null event!" << std::endl;
    }

    // this order is determined in generatePrunedHistos
    // look there for indexing of each histogram
    pruned_histograms[0]->Fill(eventToAdd->event.theta,
        eventToAdd->event.weight);
    pruned_histograms[1]->Fill(eventToAdd->event.doca,
        eventToAdd->event.weight);
    pruned_histograms[2]->Fill(eventToAdd->event.weight,
        eventToAdd->event.weight);
    pruned_histograms[3]->Fill(eventToAdd->event.vertex_x,
        eventToAdd->event.weight);
    pruned_histograms[4]->Fill(eventToAdd->event.vertex_y,
        eventToAdd->event.weight);
    pruned_histograms[5]->Fill(eventToAdd->event.vertex_z,
        eventToAdd->event.weight);
    pruned_histograms[6]->Fill(eventToAdd->event.gem_radial_dist,
        eventToAdd->event.weight);
}

// writes the pruned histograms to the outfile
void analysisManager::writePrunedHistos(){
    if(!outfile){
        std::cout << "tried to write histogram without outfile" 
            << std::endl;  
        exit(-1);
    } 

    outfile->Write();
}
