#ifndef GEMXYCUT_H
#define GEMXYCUT_H
#include "cut.h"

class gemXYCut: public cut {
    public: 
        gemXYCut(const char* name, double max_radial_dist);
        bool applyCut(eventObj* this_event);

        double max_radial_dist;
};

#endif /* GEMXYCUT_H */
