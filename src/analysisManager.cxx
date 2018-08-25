#include "../include/analysisManager.h"

// initialization list construction
analysisManager::analysisManager(std::vector<const char*> infile_paths, 
    const char* outfile_path, int verbose) : 
    infile_paths(infile_paths),  outfile_path(outfile_path),
    verbose(verbose){

    welcome();
    initializeIO(infile_paths, outfile_path);

}

// destructor has to close files and cleanup IO
analysisManager::~analysisManager(){

    // clear cuts we allocated space for in the master
    std::cout << "deleting cuts..." << std::endl;
    for(std::vector<int>::size_type i = 0; i < cuts.size(); i++){
        delete cuts[i];
    }

    // clear the histograms we made for all files 
    std::cout << "deleting pruned_histograms..." << std::endl;
    for(std::vector<int>::size_type i=0; i<infiles.size();i++){
        for(std::vector<int>::size_type j = 0; 
            j < pruned_histograms.size(); j++){
            delete pruned_histograms[i][j];
        }
    }
    // close up IO
    cleanupIO();
}


// open files, set up file pointers and check them
void analysisManager::initializeIO(std::vector<const char*> infile_paths, 
    const char* outfile_path) {
    std::cout << "initializing IO...\n";
    for(std::vector<int>::size_type i=0;i<infile_paths.size();i++){
        infiles.push_back(new TFile(infile_paths[i], "READ"));
    }
    outfile = new TFile(outfile_path, "RECREATE");
    checkFiles(infiles, infile_paths);
    checkFile(outfile, outfile_path); 
    std::cout << "IO initialized...\n";
}

void analysisManager::cleanupIO(){

    std::cout << "closing files..." << std::endl;
    for(std::vector<int>::size_type i=0;i<infiles.size();i++){
        infiles[i]->Close();
    }
    outfile->Close();
    std::cout << "deleting outfile ptr..." << std::endl;
    delete outfile;
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

void analysisManager::checkFiles(std::vector<TFile*> files, 
    std::vector<const char*> filenames){

    if(files.size()!=filenames.size()){
        std::cout << "not all file pointers have a name!" << std::endl;
        exit(-1);
    }

    for(std::vector<int>::size_type i=0;i<files.size();i++){
        checkFile(files[i],filenames[i]); 
    }

}

// wrapper for pruner
// returns total # of events that pass cuts
int analysisManager::pruneInputTrees(){
    int total_passed_cuts=0;
    if(this->infiles.empty()){
        std::cout << "no raw infiles to prune?" << std::endl;    
        exit(1);
    }
    for(std::vector<int>::size_type i=0;i<this->infiles.size();i++){
       total_passed_cuts += pruneInputTree(this->infiles[i],i);  
    }
    return total_passed_cuts;
}

// parse input files and fill histograms
// ~~~MAKES CUTS~~~~
// returns number of events passing cuts
// ADD branches here AND to eventObj.h
// infile_num is the indx of the input file being processed
int analysisManager::pruneInputTree(TFile* infile, int infile_num){
    
    int event_number = 0;
    int num_passed_cuts = 0;
    bool passed_cuts = false;

    TTreeReader reader("T",infile);

    std::cout << "Parsing input file: " << infile_num <<"...\n";

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
    generatePrunedHistograms(infile_num);

    eventObj* this_event = new eventObj(); 

    std::cout << "now pruning tree..." << std::endl;

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
        if(verbose>3)
            std::cout << "passed cuts: " << passed_cuts << std::endl;

        if(passed_cuts){
            if(verbose>4){
                std::cout << "added event #" << event_number 
                    << " to pruned_histogram\n";
            }
            addEventToPrunedHistos(this_event,infile_num);
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
        if(verbose>1){
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
        if(verbose>4){
            std::cout << "applying cut: " << cuts[i]->name
                << std::endl;
        }
        cut_ok = cuts[i]->applyCut(this_event);
        retval = retval && cut_ok;
        if(!cut_ok){
            if(verbose>4){
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
    std::cout << "num of inFiles: " << infile_paths.size() << std::endl;
    debugInfiles();
    std::cout << "outfile: " << outfile_path << std::endl;
    std::cout << "verbosity: " << verbose << std::endl;
    std::cout << "********************\n";
    std::cout << std::endl;
}

void analysisManager::debugInfiles(){
    for(std::vector<int>::size_type i=0;i<infile_paths.size();i++){
        std::cout << "infile #"  << i << ":" << std::endl
            << infile_paths[i] << std::endl; 
    }    
}

void analysisManager::debugAllCuts(){
    for(std::vector<int>::size_type i = 0; i != cuts.size(); i++){
        std::cout << "cut #" << i << ": " << cuts[i]->name 
            << std::endl; 
        std::cout << "pointer is: " << cuts[i] << std::endl;
    }
}

void analysisManager::debugPrunedHistograms(){
    for(std::vector<int>::size_type i=0; i<infiles.size();i++){
        std::cout << "Histograms for file #" << i << ":" << std::endl;
        for(std::vector<int>::size_type j=0; 
            j!=pruned_histograms[i].size(); j++){
            std::cout << "histogram #" << j << ": " 
                << pruned_histograms[i][j]->GetName() << std::endl;
            std::cout << "pointer is: " << pruned_histograms[i][j] 
                << std::endl;
        }
    }
}


// generates all required histograms so they can be filled while pruning
void analysisManager::generatePrunedHistograms(int infile_num){

    std::cout << "generating histograms..." << std::endl;

    std::string file_num_str = std::to_string(infile_num);

    // ADD HISTOGRAMS TO ADD HERE

    // names are lames
    std::string theta_name_prefix ("pruned_theta_distribution_");
    TH1D* theta_hist = new TH1D((theta_name_prefix+file_num_str).c_str(),
        (theta_name_prefix+file_num_str).c_str(), BIN_NUM, ROI_THETA_MIN, 
        ROI_THETA_MAX);
    if(!theta_hist){
        std::cout << "theta_hist is bunk!\n";
        exit(-1);
    }

    std::string doca_name_prefix ("pruned_doca_distribution_");
    TH1D* doca_hist = new TH1D((doca_name_prefix+file_num_str).c_str(),
        (doca_name_prefix+file_num_str).c_str(), BIN_NUM, ROI_DOCA_MIN, 
        ROI_DOCA_MAX);
    if(!doca_hist){
        std::cout << "doca_hist is bunk!\n";
        exit(-1);
    }

    std::string weight_name_prefix ("pruned_weight_distribution_");
    TH1D* weight_hist = new TH1D((weight_name_prefix+file_num_str).c_str(),
        (weight_name_prefix+file_num_str).c_str(), BIN_NUM,
        ROI_WEIGHT_MIN, ROI_WEIGHT_MAX);
    if(!weight_hist){
        std::cout << "weight_hist is bunk!\n";
        exit(-1);
    }

    std::string vertex_x_prefix ("pruned_vertex_x_distribution_");
    TH1D* vertex_x_hist = new TH1D((vertex_x_prefix+file_num_str).c_str(),
        (vertex_x_prefix+file_num_str).c_str(), BIN_NUM, ROI_VERTEX_X_MIN, 
        ROI_VERTEX_X_MAX);
    if(!vertex_x_hist){
        std::cout << "vertex_x_hist is bunk!\n";
        exit(-1);
    }
    
    std::string vertex_y_prefix ("pruned_vertex_y_distribution_");
    TH1D* vertex_y_hist = new TH1D((vertex_y_prefix+file_num_str).c_str(),
        (vertex_y_prefix+file_num_str).c_str(), BIN_NUM, ROI_VERTEX_Y_MIN, 
        ROI_VERTEX_Y_MAX);
    if(!vertex_y_hist){
        std::cout << "vertex_y_hist is bunk!\n";
        exit(-1);
    }

    std::string vertex_z_prefix ("pruned_vertex_z_distribution_");
    TH1D* vertex_z_hist = new TH1D((vertex_z_prefix+file_num_str).c_str(),
        (vertex_z_prefix+file_num_str).c_str(), BIN_NUM, ROI_VERTEX_Z_MIN, 
        ROI_VERTEX_Z_MAX);
    if(!vertex_z_hist){
        std::cout << "vertex_z_hist is bunk!\n";
        exit(-1);
    }

    std::string gem_dist_prefix ("pruned_gem_radial_dist_distribution_");
    TH1D* gem_radial_dist_hist = 
        new TH1D((gem_dist_prefix+file_num_str).c_str(), 
        (gem_dist_prefix+file_num_str).c_str(), BIN_NUM, 
        ROI_GEM_RADIAL_DIST_MIN, ROI_GEM_RADIAL_DIST_MAX);
    if(!gem_radial_dist_hist){
        std::cout << "gem_radial_dist_hist is bunk!\n";
        exit(-1);
    }

    // now push into a temp
    std::vector<TH1D*> temp;
    temp.push_back(theta_hist);
    temp.push_back(doca_hist);
    temp.push_back(weight_hist);
    temp.push_back(vertex_x_hist);
    temp.push_back(vertex_y_hist);
    temp.push_back(vertex_z_hist);
    temp.push_back(gem_radial_dist_hist);

    // push that into the Q
    pruned_histograms.push_back(temp);
}


// adds fields in eventobject struct to the respective histograms
// unfortunately, depends on the order in which the histograms are added to
// the pruned_histograms vector, which increases complexity of adding a
// histogram, oh well
// now associated to an infile, so the index needs to be known
void analysisManager::addEventToPrunedHistos(eventObj* eventToAdd,
    int infile_num){

    if(!eventToAdd){
        std::cout << "tried to add null event!" << std::endl;
    }

    // this order is determined in generatePrunedHistos
    // look there for indexing of each histogram
    pruned_histograms[infile_num][0]->Fill(eventToAdd->event.theta,
        eventToAdd->event.weight);
    pruned_histograms[infile_num][1]->Fill(eventToAdd->event.doca,
        eventToAdd->event.weight);
    pruned_histograms[infile_num][2]->Fill(eventToAdd->event.weight);
    pruned_histograms[infile_num][3]->Fill(eventToAdd->event.vertex_x,
        eventToAdd->event.weight);
    pruned_histograms[infile_num][4]->Fill(eventToAdd->event.vertex_y,
        eventToAdd->event.weight);
    pruned_histograms[infile_num][5]->Fill(eventToAdd->event.vertex_z,
        eventToAdd->event.weight);
    pruned_histograms[infile_num][6]->
        Fill(eventToAdd->event.gem_radial_dist, 
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

 
