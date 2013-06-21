#pragma once

#include "ofMain.h"
#include "ofxEtherdream.h"

class testApp : public ofBaseApp{
    
    enum DemoMode {
        NONE = 0,
        OSCILLATIONS,
        DOODLES,
        TEXT,
        MILKYWAY
    };
    
    enum doodle {
        EYE,
        TRIANGLE
    };
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
    void drawSomething(doodle d, ofPoint center, float scale, ofFloatColor c);
    
    bool showHud;
    float w,h;
    
    DemoMode demo;
    
    ofxIlda::Frame ildaFrame;   // stores and manages ILDA frame drawings
    
    ofxEtherdream etherdream;   // interface to the etherdream device
};
