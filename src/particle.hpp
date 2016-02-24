//
//  particle.hpp
//  example
//
//  Created by DIEGO AMOZOO on 2/22/16.
//
//

#ifndef particle_hpp
#define particle_hpp

#include <stdio.h>
#include "ofMain.h"

#endif /* particle_hpp */

class particle{
public:
    ofVec2f pos;
    ofVec2f vel;
    
    void setup(ofVec2f, ofVec2f);
    void update();
    void unUpdate();
};