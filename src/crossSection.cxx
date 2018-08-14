#include "../include/analysisManager.h"
#include "../include/allcuts.h"
#include <stdio.h>
#include <stdlib.h>
#include <TFile.h>

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

    const std::string defaultOutputPath = "../outputs/";
    const std::string defaultOutputName =  "default.out";
    const std::string usage = "usage: ./crossSection <infile> "
        "optional:<outfile>";

    // not sure what this is about but for now I keep legacy 
    //TApplication* app = new TApplication();    
    //gStyle->SetOptStat(0);

    // parse args
    if(argc < 2){
        std::cout << usage << std::endl;
        exit(1);
    }
    const char* infile_path = argv[1];
    std::string outfile_name;

    // setoutfile if provided
    if (argc == 3){
        outfile_name = argv[2];
    }
    // default else
    else {
        outfile_name = defaultOutputName;
    }
    const char* outfile_path = (defaultOutputPath+outfile_name).c_str();

    // call mr. analysis
    // verbose for now
    analysisManager* analyzer = new analysisManager(infile_path, 
        outfile_path, true);

    // initialize cuts to add to anlaysis
    // analysisManager will delete them
    // later this will be handled by a parser and params files
    cut* first_cut = new cut("generic_cut");    
    thetaCut* theta_cut = new thetaCut("theta_cut", 
        theta_min, theta_max);
    vetoCut* veto_cut = new vetoCut("veto_cut");

    bool cuts_ok = true;

    // add cuts to the analysis manager
    cuts_ok = cuts_ok && analyzer->addCut(first_cut);
    cuts_ok = cuts_ok && analyzer->addCut(theta_cut);
    cuts_ok = cuts_ok && analyzer->addCut(veto_cut);

    // debug
    analyzer->debugAllCuts();
    
    if(!cuts_ok){
        std::cout << "couldn't add all the cuts, sorry :/\n";
        exit(-1);
    }

    int num_pass_cuts = analyzer->pruneInputTree();


    // wrap up
    delete analyzer;
    std::cout << "I'm done, go home please...\n";
    return 0;
    
}
