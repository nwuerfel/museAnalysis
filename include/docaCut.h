#ifndef DOCACUT_H
#define DOCACUT_H
#include "cut.h"

class docaCut: public cut {
    public: 
        docaCut(const char* name, double doca_max);
        bool applyCut(eventObj* this_event);

        double doca_max;
};

#endif /* DOCACUT_H */
