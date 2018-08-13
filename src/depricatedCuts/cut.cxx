#include "../include/analysisTypes.h"
#include "../include/cut.h"

using namespace std;

// initialization list constructor
cut::cut(string cut_name) : cut_name(cut_name){
    cout << "instantiated cut: " cut_name << endl;
}

cut::applyCut(vector<named_branchiter>){
    cout << "applying generic cut, aka nothing..." << endl;
}
