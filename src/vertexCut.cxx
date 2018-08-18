#include "../include/vertexCut.h"
#include <math.h>

vertexCut::vertexCut(const char* name, double max_x, double max_y, 
    double max_z): cut(name), max_x(max_x), max_y(max_y), max_z(max_z) {
}

// checks to see if reconstructed vertex is within a fiducial volume
bool vertexCut::applyCut(eventObj* this_event){
    bool retval = false; 
    if(fabs(this_event->event.vertex_x) < max_x && 
        fabs(this_event->event.vertex_y) < max_y && 
        fabs(this_event->event.vertex_z) < max_z){
        retval = true;
    }
    return retval;
}
  
