#ifndef BLSCCUT_H
#define BLSCCUT_H
#include "cut.h"

class blscCut: public cut {
    public: 
        blscCut(const char* name);
    
        bool applyCut(eventObj* this_event);
};

#endif /* BLSCCUT_H */
