#include "../include/cut.h"
#include "../include/eventObj.h"
#include <iostream>

// initialization list constructor
cut::cut(const char* name) : name(name){
//    std::cout << "instantiated cut: " << name << std::endl;
}

cut::~cut(){
}

bool cut::applyCut(eventObj* this_event){
    std::cout << "applying generic cut, aka nothing..." << std::endl;
    return true;
}
