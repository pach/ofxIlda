#pragma once

#include "ofMain.h"
#include "ofxEtherdream.h"

class testApp : public ofBaseApp{
    
    enum DemoMode {
        NONE = 0,
        OSCILLATIONS = 1,
        DOODLES = 2
    };
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    
    bool showHud;
    float w,h;
    
    DemoMode demo;
    
    ofxIlda::Frame ildaFrame;   // stores and manages ILDA frame drawings

    ofxEtherdream etherdream;   // interface to the etherdream device
};
