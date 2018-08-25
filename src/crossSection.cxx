#include "../include/analysisManager.h"
#include "../include/allCuts.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * @trashman
 * Aug 11, 2018 
 * nwuerfel@umich.edu   
 * Umich goobloo
 * AP AP AP AP
 */

/* Processes data output from Anamuse, makes cuts and then generates the
 * experimental cross section which is proportional to the angular distribution
 * indeed the cross section as a function of theta and phi is:
 * xsec(th,phi) = 1/n*t*O Fout/Fin 
 * where n is number density of target, t is thickness (assumed to be diameter
 * of the cylinder right now), O is the sr angular coverage of the detectors and
 * F is the flux as a function of theta and phi
 */ 

int main(int argc, char* argv[]){

    //tmp to be replaced by params file
    double theta_min = 0;
    double theta_max = 100;    
    double vertex_max_x = 45.0;
    double vertex_max_y = 100.0; 
    double vertex_max_z = 50.0;
    double doca_max = 25.0;
    double gem_max_radial_dist = 34.0;

    const std::string defaultOutputPath = "../outputs/";
    const std::string defaultOutputName =  "default.out";
    const std::string usage = "usage: ./crossSection <infileNo>" 
        "<infile0> ... <infileNo-1> optional:<outfile>";

    // not sure what this is about but for now I keep legacy 
    //TApplication* app = new TApplication();    
    //gStyle->SetOptStat(0);

    // parse args
    if(argc < 3){
        std::cout << usage << std::endl;
        exit(-1);
    }


    // for now, we have user tell how many ifiles to expect
    int infile_path_num = atoi(argv[1]);
    if(argc-2 < infile_path_num){
        std::cout << "you told me I'd get "  << infile_path_num
            << " files, but I only see " << argc-2 << "of them..."
            << std::endl;
        exit(-1);
    }

    std::vector<const char*> infile_path_list;
    for(int i=0;i<infile_path_num;i++){
        infile_path_list.push_back(argv[i+2]); 
    }

    std::string outfile_name;

    // setoutfile if provided
    if (argv[infile_path_num+2]){
        outfile_name = argv[infile_path_num+2];
    }
    // default else
    else {
        outfile_name = defaultOutputName;
    }

    const char* outfile_path = (defaultOutputPath+outfile_name).c_str();

    // call mr. analysis
    // verbose for now
    analysisManager* analyzer = new analysisManager(infile_path_list, 
        outfile_path, 1);

    // initialize cuts to add to anlaysis
    cut* test_cut = new cut("test_cut");    
    thetaCut* theta_cut = new thetaCut("theta_cut", 
        theta_min, theta_max);
    vetoCut* veto_cut = new vetoCut("veto_cut");
    blscCut* blsc_cut = new blscCut("blsc_cut"); 
    vertexCut* vertex_cut = new vertexCut("vertex_cut",
        vertex_max_x, vertex_max_y, vertex_max_z);
    docaCut* doca_cut = new docaCut("doca_cut", doca_max);
    gemXYCut* gem_radial_cut = new gemXYCut("gem_radial_cut",
        gem_max_radial_dist);
    
    bool cuts_ok = true;

    // add cuts to the analysis manager
    cuts_ok = cuts_ok && analyzer->addCut(test_cut);
    cuts_ok = cuts_ok && analyzer->addCut(theta_cut);
    cuts_ok = cuts_ok && analyzer->addCut(veto_cut);
    cuts_ok = cuts_ok && analyzer->addCut(blsc_cut);
    cuts_ok = cuts_ok && analyzer->addCut(vertex_cut);
    cuts_ok = cuts_ok && analyzer->addCut(doca_cut);
    cuts_ok = cuts_ok && analyzer->addCut(gem_radial_cut);
   
    // debug
    //analyzer->debugAllCuts();
    // EOD

    if(!cuts_ok){
        std::cout << "couldn't add all your cuts, sorry :/\n";
        exit(-1);
    }

    int num_pass_cuts = analyzer->pruneInputTrees();

    // debug
    analyzer->debugPrunedHistograms(); 
    // EOD

    std::cout << num_pass_cuts << " good events saved" << std::endl;

    std::cout << "pushing histos into outfile" << std::endl;
    analyzer->writePrunedHistos();

    // wrap up
    delete analyzer;
    //no need to free cuts, handled by the analyzer
    std::cout << "I'm done, go home please...\n";
    return 0;
}
