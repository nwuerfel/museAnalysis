#ifndef CUT_H
#define CUT_H
#include <iostream>
#include "../include/eventObj.h"

class cut {
    public:
        cut(const char* name);
        virtual ~cut();

        virtual bool applyCut(eventObj* this_event);

        const char* name;
};
#endif
