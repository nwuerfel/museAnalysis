#include "../include/analysisTypes.h"
#include "../include/cut.h"
#include "../include/eventObj.h"
#include <iostream>

// initialization list constructor
cut::cut(std::string name) : name(name.c_str()){
    std::cout << "instantiated cut: " << name << std::endl;
}

bool cut::applyCut(eventObj* this_event){
    std::cout << "applying generic cut, aka nothing..." << std::endl;
    return true;
}
