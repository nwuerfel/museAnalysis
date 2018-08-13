#include "../include/analysisManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <TFile.h>

using namespace std;

/* Noah Wuerfel 
 * Aug 11, 2018 
 * nwuerfel@umich.edu   
 * Umich goobloo
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

    const string defaultOutputPath = "../outputs/";
    const string defaultOutputName =  "default.out";
    const string usage = "usage: ./crossSection <infile> "
        "<theta_min> <theta_max> <veto scint threshold> "
        "optional:<outfile>";

    // not sure what this is about but for now I keep legacy 
    //TApplication* app = new TApplication();    
    //gStyle->SetOptStat(0);

    // parse args
    if(argc < 5){
        cout << usage << endl;
        exit(1);
    }
    const char* infile_path = argv[1];
    double theta_min = (double) strtol(argv[2],NULL,10);
    double theta_max = (double) strtol(argv[3],NULL,10);
    double scint_threshold = (double) strtol(argv[4],NULL,10);
    string outfile_name;

    // setoutfile if provided
    if (argc == 6){
        outfile_name = argv[5];
    }
    // default else
    else {
        outfile_name = defaultOutputName;
    }
    const char* outfile_path = (defaultOutputPath+outfile_name).c_str();

    // call mr. analysis
    analysisManager* analyzer = new analysisManager(infile_path, 
        theta_min, theta_max, scint_threshold, outfile_path);

    bool cuts_ok = true;

    // anding the truth and then checking at end tells us if any cuts
    // failed withotu having to check everytime
    // addCut throws specific warnigns
    cuts_ok = cuts_ok && analyzer->addCut("theta_cut");
    
    if(!cuts_ok){
        cout << "couldn't add all the cuts, sorry :/\n";
        exit(-1);
    }

    int num_pass_cuts = analyzer->readPrunedTree();

    //cout << num_pass_cuts << endl;

    // wrap up
    return 0;
    
}
