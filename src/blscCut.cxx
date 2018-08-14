#include "../include/blscCut.h"

blscCut::blscCut(const char* name):cut(name){
}

bool blscCut::applyCut(eventObj* this_event){
    return !(this_event->hit_blsc);
}
