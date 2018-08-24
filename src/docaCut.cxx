#include "../include/docaCut.h"

// initialized constructor overwrites base with parameters
docaCut::docaCut(const char* name, double doca_max): cut(name), 
    doca_max(doca_max){
}

bool docaCut::applyCut(eventObj* this_event){
    bool retval = false;
    double doca = this_event->event.doca;
    if(doca < doca_max) 
        retval = true;
    return retval;
}
