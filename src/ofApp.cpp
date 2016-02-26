#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
    ofEnableAlphaBlending();
	leap.open();
    
	cam.setOrientation(ofPoint(-20, 0, 0));
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    estado = 0;
    
    //-------------------MODEL LOADER-----------------
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(50, 0);
    
    
    pointLight.setPosition(960, 810, -600);
    pointLight2.setPosition(960, 810, -600);
    pointLight3.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight3.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight3.setPointLight();
    pointLight3.setPosition(360, 810, -600);
    
    pointLight4.setDiffuseColor( ofColor(200.f, 255.f, 200.f));
    
    pointLight4.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight4.setPointLight();
    pointLight4.setPosition(1260, 810, -600);
    
    ofDisableArbTex(); // we need GL_TEXTURE_2D for our models coords.
    
    bAnimate = false;
    bAnimateMouse = false;
    animationPosition = 0;
    
    
    jar.loadModel("Jar/JarTextura.dae", false);
    jar.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);
    
    vodka.loadModel("Vodka/Vodka.dae", false);
    salsa.loadModel("SALSA/Slasa.dae", false);
    lata.loadModel("Refresco/Lata.dae", false);
    sal.loadModel("Salero/Salero.dae", false);
    limon.loadModel("Limon/Limon.dae", false);

    jar.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    jar.playAllAnimations();
    if(!bAnimate) {
        jar.setPausedForAllAnimations(true);
    }
    
    //-------------------EXTRAS-------------------------
    player.load("Fondo60.mov");
    bump.load("Bump.png");
    player.setLoopState(OF_LOOP_PALINDROME);
    player.play();
    firstHandPos.set(0, 0);
    enableTransition = true;
    size1Cont = 0;
    
    radius		= 180.f;
    center.set(ofGetWidth()*.5, ofGetHeight()*.5, 0);
    
    posInicial = true;
    posFinal = false;
    frameCont = 0;
    
    particles.push_back(*new particle());
    particles[0].setup(ofVec2f(jar.getPosition().x, jar.getPosition().y), ofVec2f(0, -26));
    particles.push_back(*new particle());
    particles[1].setup(ofVec2f(jar.getPosition().x, jar.getPosition().y), ofVec2f(10, -26));
    particles.push_back(*new particle());
    particles[2].setup(ofVec2f(jar.getPosition().x, jar.getPosition().y), ofVec2f(-10, -26));
    particles.push_back(*new particle());
    particles[3].setup(ofVec2f(jar.getPosition().x, jar.getPosition().y), ofVec2f(18, -26));
    particles.push_back(*new particle());
    particles[4].setup(ofVec2f(jar.getPosition().x, jar.getPosition().y), ofVec2f(-18, -26));
    
    
    //----------------Particulas-------------
    int num = 3000;
    p.assign(num, demoParticle());
    currentMode = PARTICLE_MODE_ATTRACT;
    currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
    
    resetParticles();
    showParticles = false;

}


//--------------------------------------------------------------
void ofApp::update(){
    player.update();
	fingersFound.clear();
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        firstHandPos.set(simpleHands[0].handPos.x/2, ofMap(simpleHands[0].handPos.y, -500, 500, -100, 100, true));
        //firstHandPos.set(simpleHands[0].handPos.x, simpleHands[0].handPos.y);
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                
//                ofPolyline & polyline = fingerTrails[id];
//                ofPoint pt = simpleHands[i].fingers[fingerTypes[f]].pos;
//                
//                //if the distance between the last point and the current point is too big - lets clear the line
//                //this stops us connecting to an old drawing
//                if( polyline.size() && (pt-polyline[polyline.size()-1] ).length() > 50 ){
//                    polyline.clear();
//                }
//                
//                //add our point to our trail
//                polyline.addVertex(pt);
                
                fingersFound.push_back(id);
            }
        }
    }
	leap.markFrameAsOld();
    
    //-------------------MODEL LOADER-----------------
    jar.update();
    pointLight.setPosition(cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x,
                           sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y,
                           -cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);
    
    pointLight2.setPosition(sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y,
                           cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x,
                           -cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);
    
    pointLight3.setPosition(sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y,
                           -cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z,
                           cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x);
    
    pointLight4.setPosition(sin(ofGetElapsedTimef()*.8f) * radius * 2 + center.y,
                            cos(ofGetElapsedTimef()*.6f) * radius * 2 + center.x,
                            -cos(ofGetElapsedTimef()*.8f) * radius * 2 + center.z);
    
    
    if(bAnimateMouse) {
        jar.setPositionForAllAnimations(animationPosition);
    }
    
    mesh = jar.getCurrentAnimatedMesh(0);
    
    if (!simpleHands.size()) {
        firstHandPos.set(0, 0);
    }
    
    //-------------------Extras-----------------
    
    if (firstHandPos.y > 20 && estado == 0 && posInicial && enableTransition) {
        estado = 1;
        posInicial = false;
        enableTransition = false;
    }else if (firstHandPos.y > 20 && estado == 1 && posFinal && enableTransition){
        estado = 2;
        posFinal =  false;
        frameCont = ofGetFrameNum();
        enableTransition = false;
    }
    
    if (estado == 1) {
        size1Cont = size1Cont + 0.01;
        if (size1Cont > 1) {
            size1Cont = 0;
        }
        for (int i = 0; i < particles.size(); i++) {
            if (particles[i].pos.y < 900) {
                particles[i].update();
                posFinal = false;
            }else{
                posFinal = true;
                enableTransition = true;
            }
        }
    }else if (estado == 2){
        for (int i = 0; i < particles.size(); i++) {
            if (ofGetFrameNum()< frameCont + 107) {
                particles[i].unUpdate();
            }else{
                posInicial = true;
                posFinal = false;
                estado = 0;
                enableTransition =  true;
                particles[0].setup(ofVec2f(960, 810), ofVec2f(0, -26));
                particles[1].setup(ofVec2f(960, 810), ofVec2f(10, -26));
                particles[2].setup(ofVec2f(960, 810), ofVec2f(-10, -26));
                particles[3].setup(ofVec2f(960, 810), ofVec2f(18, -26));
                particles[4].setup(ofVec2f(960, 810), ofVec2f(-18, -26));
            }
        }
    }
    
    showParticles = !posInicial && !posFinal;
    if (showParticles) {
        currentMode = PARTICLE_MODE_REPEL;
    }else{
        currentMode = PARTICLE_MODE_ATTRACT;
    }
    
    //----------------Particulas-------------
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setMode(currentMode);
      //  p[i].update(particles[0].pos);
    }
    
    //lets add a bit of movement to the attract points
//    for(unsigned int i = 0; i < attractPointsWithMovement.size(); i++){
//        attractPointsWithMovement[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 0.7) * 12.0;
//        attractPointsWithMovement[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 12.0;
//    }
}


//--------------------------------------------------------------
void ofApp::draw(){

    ofPushStyle();
    ofSetColor(255, 255, 255);
    player.draw(0, 0, 1920, 1080);
    ofPopStyle();

    
    //-------------------MODEL LOADER-----------------
    if (estado == 0) {
        ofSetColor(255);
        
        //ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        jar.setPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75 , 0);
        ofEnableDepthTest();
#ifndef TARGET_PROGRAMMABLE_GL
        glShadeModel(GL_SMOOTH); //some model / light stuff
#endif
        pointLight.setDiffuseColor( ofColor(100.f, 195.f, 255.f));
        pointLight.setSpecularColor( ofColor(200.f, 220.f, 0.f));
        pointLight.setPointLight();
        pointLight2.setDiffuseColor( ofColor(100.f, 195.f, 255.f));
        pointLight2.setSpecularColor( ofColor(200.f, 200.f, 200.f));
        pointLight2.setPointLight();
        pointLight3.setDiffuseColor( ofColor(100.f, 195.f, 255.f));
        pointLight3.setSpecularColor( ofColor(200.f, 200.f, 200.f));
        pointLight3.setPointLight();
        
        light.enable();
        pointLight.enable();
        pointLight2.enable();
        pointLight3.enable();
        pointLight4.enable();
        light.setAreaLight(2000, 2000);
        ofEnableSeparateSpecularLight();
        
        ofPushMatrix();
        //ofScale(0.5, 0.5);
        ofTranslate(jar.getPosition().x, jar.getPosition().y  , -500);
        ofRotate(-firstHandPos.x, 0, 1, 0);
        //ofRotate(- mouseX, 0, 1, 0);
        ofTranslate(-jar.getPosition().x, -jar.getPosition().y +250, 0);
        
        ofTranslate(jar.getPosition().x, jar.getPosition().y - 550, 0);
        ofRotateX(firstHandPos.y/5);
        ofTranslate(-jar.getPosition().x, -jar.getPosition().y + 550, 0);
        jar.drawFaces();
        ofPopMatrix();
#ifndef TARGET_PROGRAMMABLE_GL
        glEnable(GL_NORMALIZE);
#endif

//        ofPopMatrix();
        
        

        
        //------------------LEAP END-----------------------
        
        ofDisableDepthTest();
        light.disable();
        pointLight.disable();
        pointLight2.disable();
        pointLight3.disable();
        pointLight4.disable();
        ofDisableLighting();
        ofDisableSeparateSpecularLight();
    }else if(estado == 1 || estado == 2){
        draw0();
    }
    

    
    ofPushStyle();
    ofSetColor(255, 255, 0);
    ofDrawBitmapString(ofToString(jar.getPosition()), 20, 900);
    ofDrawBitmapString(ofToString(size1Cont), 20, 910);
    ofDrawBitmapString(ofToString(particles.size()), 20, 920);
    ofDrawBitmapString(ofToString(particles[0].vel), 20, 930);
    ofDrawBitmapString("estado  = " + ofToString(estado), 20, 20);
    ofDrawBitmapString("inicial = " + ofToString(posInicial), 20, 30);
    ofDrawBitmapString("final   = " + ofToString(posFinal), 20, 40);
    ofDrawBitmapString("frames  = " + ofToString(frameCont), 20, 50);
    ofDrawBitmapString("transition = " + ofToString(enableTransition), 20, 60);
    ofPopStyle();
    
    //----------------LEAP-----------------
    cam.begin();
    
    
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        ofPoint handNormal = simpleHands[i].handNormal;
        
        ofSetColor(0, 0, 255);
        ofDrawSphere(handPos.x, handPos.y, handPos.z, 5);
        ofSetColor(255, 255, 0);
        ofDrawArrow(handPos, handPos + 100*handNormal);
        
        for (int f=0; f<5; f++) {
            ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
            ofSetColor(0, 255, 0);
            ofDrawSphere(mcp.x, mcp.y, mcp.z, 5);
            ofDrawSphere(pip.x, pip.y, pip.z, 5);
            ofDrawSphere(dip.x, dip.y, dip.z, 5);
            ofDrawSphere(tip.x, tip.y, tip.z, 5);
            
            ofSetColor(255, 0, 0, 100);
            ofSetLineWidth(4);
            ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
            ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
            ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
        }
    }
    
    //    for(int i = 0; i < fingersFound.size(); i++){
    //        ofxStrip strip;
    //        int id = fingersFound[i];
    //
    //        ofPolyline & polyline = fingerTrails[id];
    //        strip.generate(polyline.getVertices(), 15, ofPoint(0, 0.5, 0.5) );
    //
    //        ofSetColor(255 - id * 15, 0, id * 25);
    //        strip.getMesh().draw();
    //    }
    

    cam.end();
    
    //----------------Particulas-------------
    
    if (showParticles) {
        for(unsigned int i = 0; i < p.size(); i++){
            //p[i].draw(i);
        }
        
        ofSetColor(190);
        if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
            for(unsigned int i = 0; i < attractPoints.size(); i++){
                ofNoFill();
                ofCircle(attractPointsWithMovement[i], 10);
                ofFill();
                ofCircle(attractPointsWithMovement[i], 4);
            }
        }
    }
    ofPopStyle();
    ofPushStyle();
    ofSetColor(255, 255, 255);
    bump.draw(0, 0, 1920, 1080);
    ofPopStyle();
    
}

//--------------------------------------------------------------
void ofApp::draw0(){
    ofPushMatrix();
    ofTranslate(0, -100);
    
    ofPushStyle();
    ofSetColor(255, 230, 10);
    for (int i = 0; i < particles.size(); i++) {
        //ofCircle(particles[i].pos.x, particles[i].pos.y, 10);
    }
    ofPopStyle();
    
    ofEnableDepthTest();
#ifndef TARGET_PROGRAMMABLE_GL
    glShadeModel(GL_SMOOTH); //some model / light stuff
#endif
    //light.enable();
    pointLight.setDiffuseColor( ofColor(255.f, 200.f, 150.f));
    
    
    //------------VODKA y salero------------------------
    // specular color, the highlight/shininess color //
    pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight.setPointLight();
    pointLight2.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
    pointLight2.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight2.setPointLight();
    pointLight.enable();
    pointLight2.enable();
    ofEnableSeparateSpecularLight();

    ofPushMatrix();
    //ofScale(0.5, 0.5);
    vodka.setPosition(ofMap(particles[4].pos.x, 1050, 1600, 0, 280), particles[4].pos.y , vodka.getPosition().z);
    ofTranslate(vodka.getPosition().x+900, vodka.getPosition().y, -500);
    ofRotate(-firstHandPos.x + 80, 0, 1, 0);
    ofTranslate(-vodka.getPosition().x, -vodka.getPosition().y + 400, 0);
    
    ofTranslate(vodka.getPosition().x, vodka.getPosition().y - 550, 0);
    ofRotateX(firstHandPos.y/5);
    ofTranslate(-vodka.getPosition().x, -vodka.getPosition().y + 550, 0);
    vodka.drawFaces();
    ofPopMatrix();
    
    ofPushMatrix();
    ofScale(0.5, 0.5);
    sal.setPosition(particles[1].pos.x, particles[1].pos.y , sal.getPosition().z);
    ofTranslate(sal.getPosition().x+900, sal.getPosition().y, -500);
    ofRotate(-firstHandPos.x+ 80, 0, 1, 0);
    ofTranslate(-sal.getPosition().x, -sal.getPosition().y + 1500, 0);
    
    ofTranslate(sal.getPosition().x, sal.getPosition().y - 550, 0);
    ofRotateX(firstHandPos.y/5);
    ofTranslate(-sal.getPosition().x, -sal.getPosition().y + 550, 0);
    sal.drawFaces();
    ofPopMatrix();
    
    pointLight.disable();
    pointLight2.disable();
    
    //------------SALSA------------------------
    
    pointLight.setSpecularColor( ofColor(255.f, 0.f, 0.f));
    pointLight.setPointLight();
    pointLight2.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
    pointLight2.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight2.setPointLight();
    pointLight.enable();
    pointLight2.enable();
    ofEnableSeparateSpecularLight();
    
    ofPushMatrix();
    ofScale(0.5, 0.5);
    salsa.setPosition(particles[2].pos.x, particles[4].pos.y , salsa.getPosition().z);
    ofTranslate(salsa.getPosition().x+900, salsa.getPosition().y, -500);
    ofRotate(-firstHandPos.x+ 30, 0, 1, 0);
    ofTranslate(-salsa.getPosition().x, -salsa.getPosition().y + 1500, 0);
    
    ofTranslate(salsa.getPosition().x, salsa.getPosition().y - 550, 0);
    ofRotateX(firstHandPos.y/5);
    ofTranslate(-salsa.getPosition().x, -salsa.getPosition().y + 550, 0);
    salsa.drawFaces();
    ofPopMatrix();
    
    pointLight.disable();
    pointLight2.disable();
    
    //------------LIMON------------------------
    
    pointLight.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight.setPointLight();
    pointLight2.setDiffuseColor( ofColor(255.f, 255.f, 255.f));
    pointLight2.setSpecularColor( ofColor(255.f, 255.f, 255.f));
    pointLight2.setPointLight();
    pointLight.enable();
    pointLight2.enable();
    ofEnableSeparateSpecularLight();
    
    ofPushMatrix();
    ofScale(0.05, 0.25);
    limon.setScale(0.5, 0.5, 0.5);
    limon.setPosition((ofMap(particles[4].pos.x, 350, -280, -100, 1300) + 16000)*2, particles[3].pos.y, limon.getPosition().z);
    ofTranslate(limon.getPosition().x+900, limon.getPosition().y, -500);
    ofRotate(-firstHandPos.x + 80, 0, 1, 0);
    ofTranslate(-limon.getPosition().x, -limon.getPosition().y + 3500, 0);
    
    ofTranslate(limon.getPosition().x, limon.getPosition().y - 550, 0);
    ofRotateX(firstHandPos.y/5);
    ofTranslate(-limon.getPosition().x, -limon.getPosition().y + 550, 0);
    limon.drawFaces();
    ofPopMatrix();
    
    pointLight.disable();
    pointLight2.disable();
    
    //------------LATA------------------------
    light.disable();
    pointLight.setPosition(-.4 * radius * 2 + center.x,
                           -.4 * radius * 2 + center.y,
                           -.4 * radius * 2 + center.z);
    
    pointLight2.setPosition(-.5 * radius * 2 + center.y,
                            -.5 * radius * 2 + center.x,
                            -.5 * radius * 2 + center.z);
    
    pointLight3.setPosition(-.6 * radius * 2 + center.y,
                            -.6 * radius * 2 + center.z,
                            -.6 * radius * 2 + center.x);
    pointLight.setDiffuseColor( ofColor(155.f, 255.f, 0.f));
    pointLight.setSpecularColor( ofColor(200.f, 220.f, 0.f));
    pointLight.setPointLight();
    pointLight2.setDiffuseColor( ofColor(55.f, 155.f, 0.f));
    pointLight2.setSpecularColor( ofColor(200.f, 200.f, 200.f));
    pointLight2.setPointLight();
    pointLight3.setDiffuseColor( ofColor(155.f, 255.f, 0.f));
    pointLight3.setSpecularColor( ofColor(200.f, 200.f, 200.f));
    pointLight3.setPointLight();
    //pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();
    ofEnableSeparateSpecularLight();
    
    ofPushMatrix();
    ofScale(0.5, 0.5);
    lata.setPosition(particles[0].pos.x, particles[0].pos.y , lata.getPosition().z);
    ofTranslate(lata.getPosition().x+900, lata.getPosition().y, -500);
    ofRotate(-firstHandPos.x+ 30, 0, 1, 0);
    ofTranslate(-lata.getPosition().x, -lata.getPosition().y + 1500, 0);
    
    ofTranslate(lata.getPosition().x, lata.getPosition().y - 550, 0);
    ofRotateX(firstHandPos.y/5);
    ofTranslate(-lata.getPosition().x, -lata.getPosition().y + 550, 0);
    lata.drawFaces();
    ofPopMatrix();
    
    pointLight.disable();
    pointLight2.disable();
    pointLight3.disable();

#ifndef TARGET_PROGRAMMABLE_GL
    glEnable(GL_NORMALIZE);
#endif
    ofDisableDepthTest();
    

    ofDisableLighting();
    ofDisableSeparateSpecularLight();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::draw1(){
//    ofSetColor(255);
//    
//    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//    
//    ofEnableDepthTest();
//#ifndef TARGET_PROGRAMMABLE_GL
//    glShadeModel(GL_SMOOTH); //some model / light stuff
//#endif
//    light.enable();
//    ofEnableSeparateSpecularLight();
//    ofPushMatrix();
//    ofTranslate(0, 200);
//    for (int i = 0; i < 5; i++) {
//        ofPushMatrix();
//        ofScale(0.3, 0.3);
//        ofTranslate(model.getPosition().x + (i * 1200), model.getPosition().y, 0);
//        ofRotate(-firstHandPos.x, 0, 1, 0);
//        ofTranslate(-model.getPosition().x , -model.getPosition().y + 1500, 0);
//        
//        ofTranslate(model.getPosition().x, model.getPosition().y - 550, 0);
//        ofRotateX(firstHandPos.y/5);
//        ofTranslate(-model.getPosition().x , -model.getPosition().y + 550, 0);
//        
//        model.drawFaces();
//        
//        ofPushStyle();
//        ofSetColor(255, 0, 0);
//        ofPopStyle();
//        ofPopMatrix();
//    }
//    ofPopMatrix();
//    
//
//#ifndef TARGET_PROGRAMMABLE_GL
//    glEnable(GL_NORMALIZE);
//#endif
//    ofPushMatrix();
//    ofTranslate(model.getPosition().x-300, model.getPosition().y, 0);
//    ofRotate(-mouseX, 0, 1, 0);
//    ofTranslate(-model.getPosition().x , -model.getPosition().y, 0);
//    
//    ofxAssimpMeshHelper & meshHelper = model.getMeshHelper(0);
//    
//    ofMultMatrix(model.getModelMatrix());
//    ofMultMatrix(meshHelper.matrix);
//    
//    ofMaterial & material = meshHelper.material;
//    if(meshHelper.hasTexture()){
//        meshHelper.getTextureRef().bind();
//    }
//    material.begin();
//    //mesh.drawWireframe();
//    material.end();
//    if(meshHelper.hasTexture()){
//        meshHelper.getTextureRef().unbind();
//    }
//    ofPopMatrix();
//    
//    ofDisableDepthTest();
//    light.disable();
//    ofDisableLighting();
//    ofDisableSeparateSpecularLight();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofPoint modelPosition(ofGetWidth() * 0.5, (float)ofGetHeight() * 0.75);
//    switch (key) {
//        case '1':
//            model.loadModel("astroBoy_walk.dae");
//            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
//            ofEnableSeparateSpecularLight();
//            break;
//        case '2':
//            model.loadModel("TurbochiFromXSI.dae");
//            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
//            model.setRotation(0, -180, 1, 0, 0);
//            ofEnableSeparateSpecularLight();
//            break;
//        case '3':
//            model.loadModel("dwarf.x");
//            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
//            ofDisableSeparateSpecularLight();
//            break;
//        case '4':
//            model.loadModel("monster-animated-character-X.X");
//            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
//            model.setRotation(0, -90, 0, 0, 1);
//            ofDisableSeparateSpecularLight();
//            break;
//        case '5':
//            model.loadModel("squirrel/NewSquirrel.3ds");
//            model.setPosition(modelPosition.x, modelPosition.y, modelPosition.z);
//            ofDisableSeparateSpecularLight();
//            break;
//        case ' ':
//            bAnimate = !bAnimate;
//            break;
//        default:
//            break;
//    }
//    
//    mesh = model.getMesh(0);
//    
//    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
//    model.playAllAnimations();
//    if(!bAnimate) {
//        model.setPausedForAllAnimations(true);
//    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // scrub through aninations manually.
    animationPosition = y / (float)ofGetHeight();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
//    // pause all animations, so we can scrub through them manually.
//    model.setPausedForAllAnimations(true);
//    animationPosition = y / (float)ofGetHeight();
//    bAnimateMouse = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
//    // unpause animations when finished scrubbing.
//    if(bAnimate) {
//        model.setPausedForAllAnimations(false);
//    }
//    bAnimateMouse = false;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}

//--------------------------------------------------------------
void ofApp::resetParticles(){
    
    //these are the attraction points used in the forth demo
    attractPoints.clear();
    for(int i = 0; i < 4; i++){
        attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
    }
    
    attractPointsWithMovement = attractPoints;
    
    for(unsigned int i = 0; i < p.size(); i++){
        p[i].setMode(currentMode);
        p[i].setAttractPoints(&attractPointsWithMovement);;
        p[i].reset();
    }	
}
