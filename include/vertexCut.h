#ifndef VERTEXCUT_H
#define VERTEXCUT_H
#include "../include/cut.h"
class vertexCut: public cut {
    public: 
        vertexCut(const char* name, double max_x, double max_y,
            double max_z);

        bool applyCut(eventObj* this_event);

        double max_x;
        double max_y;
        double max_z;
};
#endif /* VERTEXCUT_H */
