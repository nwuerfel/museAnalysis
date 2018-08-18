#ifndef EVENTOBJ_H
#define EVENTOBJ_H

class eventObj {
    public:
    
        eventObj();
        eventObj(int event_id, double theta, double doca, double weight, 
            double vertex_x, double vertex_y, double vertex_z,
            double gem_radial_dist, bool hit_veto, bool hit_blsc, 
            bool frame_hit, bool tgt_event);
        ~eventObj();

        int event_id;
        double theta;
        double doca;
        double weight;
        double vertex_x;
        double vertex_y;
        double vertex_z;
        double gem_radial_dist;
        bool hit_veto;
        bool hit_blsc;
        bool frame_hit;
        bool tgt_event;

        void debug();
};

#endif
