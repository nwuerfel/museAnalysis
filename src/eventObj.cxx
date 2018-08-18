#include "../include/eventObj.h"
#include <iostream>

// declaration
eventObj::eventObj(){
}

// initialization list
eventObj::eventObj(int event_id, double theta, double doca, 
        double weight, double vertex_x, double vertex_y, double vertex_z,
        double gem_radial_dist, bool hit_veto, bool hit_blsc, 
        bool frame_hit, bool tgt_event):
        event_id(event_id), theta(theta), doca(doca), weight(weight), 
        vertex_x(vertex_x), vertex_y(vertex_y), vertex_z(vertex_z), 
        gem_radial_dist(gem_radial_dist),
        hit_veto(hit_veto), hit_blsc(hit_blsc), frame_hit(frame_hit), 
        tgt_event(tgt_event) {
    // intentionally blank
}

eventObj::~eventObj(){
    std::cout << "lol I'm dead" << std::endl;
}

void eventObj::debug(){
    std::cout << "....\n";
    std::cout << "event #" << event_id << " debugging: \n"; 
    std::cout << "theta: "<< theta << std::endl;
    std::cout << "doca: " << doca << std::endl;
    std::cout << "weight: " << weight << std::endl;
    std::cout << "vertex_x: " << vertex_x << std::endl;
    std::cout << "vertex_y: " << vertex_y << std::endl;
    std::cout << "vertex_z: " << vertex_z << std::endl;
    std::cout << "gem_radial_dist: " << gem_radial_dist << std::endl;
    std::cout << "hit_veto: " << hit_veto << std::endl;
    std::cout << "hit_blsc: " << hit_blsc << std::endl;
    std::cout << "frame_hit: " << frame_hit << std::endl;
    std::cout << "tgt_event: " << tgt_event << std::endl;
    std::cout << "....\n";
}
