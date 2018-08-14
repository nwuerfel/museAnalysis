#include "../include/cut.h"
#include "../include/eventObj.h"
#include <iostream>

// initialization list constructor
cut::cut(const char* name) : name(name){
}

cut::~cut(){
}

bool cut::applyCut(eventObj* this_event){
    return true;
}
