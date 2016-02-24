//
//  particle.cpp
//  example
//
//  Created by DIEGO AMOZOO on 2/22/16.
//
//

#include "particle.hpp"


//--------------------------------------------------------------
void particle::setup(ofVec2f newPos, ofVec2f newVel){
    pos = newPos;
    vel = newVel;
}

//--------------------------------------------------------------
void particle::update(){
    vel.y = vel.y + 0.5;
    
    pos.x = pos.x + vel.x;
    pos.y = pos.y + vel.y;
}

//--------------------------------------------------------------
void particle::unUpdate(){
    vel.y = vel.y - 0.5;
    
    pos.x = pos.x - vel.x;
    pos.y = pos.y - vel.y;
}