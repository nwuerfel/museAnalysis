#include "../include/vetoCut.h"

vetoCut::vetoCut(const char* name):cut(name){
}

// trivial cut: good example of cut "plugin"
bool vetoCut::applyCut(eventObj* this_event){
    return !(this_event->event.hit_veto);
}
