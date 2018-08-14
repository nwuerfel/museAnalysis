#include "../include/cut.h"
#include "../include/thetaCut.h"

// initialized constructor overwrites base with parameters
thetaCut::thetaCut(const char* name, double theta_min,
    double theta_max): cut(name), theta_min(theta_min), 
    theta_max(theta_max){
        
    std::cout << "I'm thetaCut: " << name << std::endl;
        
}

// here we define a simple "cut plugin"
bool thetaCut::applyCut(eventObj* this_event){
        
    std::cout << "wtf my name is: " << name << std::endl;
    
    bool retval = false;
    double theta = this_event->theta;
    if(theta > theta_min && theta < theta_max) 
        retval = true;
    return retval;
}
