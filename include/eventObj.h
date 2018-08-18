#ifndef EVENTOBJ_H
#define EVENTOBJ_H

struct event_{
    // alignment 8 pay attention to packing!
    double theta;
    double doca;
    double weight;
    double vertex_x;
    double vertex_y;
    double vertex_z;
    double gem_radial_dist;
    int event_id;
    bool hit_veto;
    bool hit_blsc;
    bool frame_hit;
    bool tgt_event;
};

class eventObj {
    public:
    
        eventObj();
        ~eventObj();

        event_ event;

        void debug();
};

#endif
