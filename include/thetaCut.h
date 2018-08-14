#ifndef THETACUT_H
#define THETACUT_H
#include "../include/cut.h"

class thetaCut : public cut {
    public:
        thetaCut(const char* name, double theta_min,
            double theta_max);

        bool applyCut(eventObj* this_event);
        
        const double theta_min;
        const double theta_max;
};

#endif
