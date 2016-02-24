#pragma once

#include "ofMain.h"
#include "ofxLeapMotion2.h"
#include "ofxStrip.h"
#include "particle.hpp"

#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"

#include "demoParticle.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void draw0();
    void draw1();

    
	
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void exit();
    
    int estado;
    
	ofxLeapMotion leap;
	vector <ofxLeapMotionSimpleHand> simpleHands;
    
	vector <int> fingersFound;
	ofEasyCam cam;
    map <int, ofPolyline> fingerTrails;
    
    ofPoint firstHandPos;
    bool enableTransition;
    
    
    //------------------MODEL LOADER-------------------
    ofxAssimpModelLoader model;
    
    vector<ofxAssimpModelLoader> modelos;
    
    bool bAnimate;
    bool bAnimateMouse;
    float animationPosition;
    
    ofMesh mesh;
    ofLight	light;
    ofLight pointLight;
    
    //------------------EXTRAS-------------------------
    ofVideoPlayer player;
    float size1Cont;
    
    vector<particle> particles;
    bool posInicial;
    bool posFinal;
    
    int frameCont;
    
    float radius;
    ofVec3f center;
    
    //----------------Particulas-------------
    void resetParticles();
    particleMode currentMode;
    string currentModeStr;
    
    vector <demoParticle> p;
    vector <ofPoint> attractPoints;
    vector <ofPoint> attractPointsWithMovement;
    
    bool showParticles;
    
};
