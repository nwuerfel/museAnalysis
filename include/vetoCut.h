#ifndef VETOCUT_H
#define VETOCUT_H
#include "cut.h"

class vetoCut: public cut {
    public: 
        vetoCut(const char* name);
    
        bool applyCut(eventObj* this_event);
};

#endif /* VETOCUT_H */
