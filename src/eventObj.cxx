#include "../include/eventObj.h"
#include <iostream>

// declaration
eventObj::eventObj(){
}

eventObj::~eventObj(){
    //std::cout << "lol I'm dead" << std::endl;
}

void eventObj::debug(){
    std::cout << "....\n";
    std::cout << "event #" << event.event_id << " debugging: \n"; 
    std::cout << "theta: "<< event.theta << std::endl;
    std::cout << "doca: " << event.doca << std::endl;
    std::cout << "weight: " << event.weight << std::endl;
    std::cout << "vertex_x: " << event.vertex_x << std::endl;
    std::cout << "vertex_y: " << event.vertex_y << std::endl;
    std::cout << "vertex_z: " << event.vertex_z << std::endl;
    std::cout << "gem_radial_dist: " << event.gem_radial_dist
        << std::endl;
    std::cout << "hit_veto: " << event.hit_veto << std::endl;
    std::cout << "hit_blsc: " << event.hit_blsc << std::endl;
    std::cout << "frame_hit: " << event.frame_hit << std::endl;
    std::cout << "tgt_event: " << event.tgt_event << std::endl;
    std::cout << "....\n";
}
