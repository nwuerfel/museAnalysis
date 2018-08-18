#include "../include/gemXYCut.h"

gemXYCut::gemXYCut(const char* name, double max_radial_dist): cut(name),
    max_radial_dist(max_radial_dist){
}

bool gemXYCut::applyCut(eventObj* this_event){
    bool retval = false;
    double grd = this_event->event.gem_radial_dist;
    if(grd < max_radial_dist) 
        retval = true;
    return retval;
}
