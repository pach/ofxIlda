#include "testApp.h"
#include "ofxIldaFrame.h"


//--------------------------------------------------------------
void testApp::setup(){
    
    showHud = true;
    w = 1.0;//ofGetWidth();
    h = 1.0;//ofGetHeight();
    
    ofBackground(0);
    
    etherdream.setup();
    etherdream.setPPS(25000);
    
    ildaFrame.params.output.doCapX=true;
    ildaFrame.params.output.doCapY=true;
    ildaFrame.params.output.blankCount=10;
    ildaFrame.params.output.endCount=10;
    ildaFrame.polyProcessor.params.targetPointCount=400;
    
    demo = testApp::OSCILLATIONS;
    

    
    ///////////
    
    
    if (demo==testApp::DOODLES)
    {
        /* Eye */
        //        poly.arc(ofPoint(w/2.,h/2.), w*0.3, h*0.1, -90, 270, false);
        //        poly.arc(ofPoint(w/2.,h/2.), w*0.1, h*0.1, 270, -90, false);
        //        ildaFrame.addPoly(ofxIlda::Poly(poly.getResampledByCount(50)), ofFloatColor(0,1,1,1));
        
        /* Triangle */
        ildaFrame.addPoly(ofFloatColor(0,0,1,1));
        ildaFrame.getLastPoly().lineTo(ofPoint(0.5,0.0));
        ildaFrame.getLastPoly().lineTo(ofPoint(0,1));
        ildaFrame.getLastPoly().lineTo(ofPoint(1,1));
        ildaFrame.getLastPoly().lineTo(ofPoint(0.5,0.0));
    } else if (demo==testApp::OSCILLATIONS){
        //lines
        //    ildaFrame.addPoly(ofFloatColor(1,1,1,1));
    }
}

//--------------------------------------------------------------
void testApp::update(){
    if (demo==OSCILLATIONS){
        ildaFrame.getPolys().clear();
        ildaFrame.addPoly(ofFloatColor(
                                       (int)ofGetElapsedTimef()%3,
                                       (int)ofGetElapsedTimef()%5,
                                       (int)ofGetElapsedTimef()%7,
                                       1));
        
        ofxIlda::Poly& p = ildaFrame.getLastPoly();
        p.clear();
        
        for (int i = 0 ; i < 30 ; i ++){
            float t = (ofGetElapsedTimef() + i*10.0);
            float scale = sqrt(abs(sin(ofGetElapsedTimef())));
            ofPoint pnt = (ofPoint(0.5,0.5) + scale * ofPoint(0.5*cos(t*1.1), 0.5*sin(t)));
            p.lineTo(pnt);
        }
    }
}

//--------------------------------------------------------------
void testApp::draw() {
    // do your thang
    ildaFrame.update();
    
    // draw to the screen
    ildaFrame.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    // send points to the etherdream
    etherdream.setPoints(ildaFrame);
    
    ofSetColor(255);
    //    poly.draw();
    //    poly.getResampledByCount(2).draw();
    ildaFrame.params;
    
    if (showHud) ofDrawBitmapString(ildaFrame.getString(), 10, 30);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key) {
        case 'f': ofToggleFullscreen(); break;
            
            
            // clear the frame
        case 'c': ildaFrame.clear(); break;
            
            // draw rectangle
        case 'r': {
            ofPolyline p = ofPolyline::fromRectangle(ofRectangle(ofRandomuf()/2, ofRandomuf()/2, ofRandomuf()/2, ofRandomuf()/2));
            ildaFrame.addPoly(p);
        }
            break;
            
            // change color
        case 'R': ildaFrame.params.output.color.r = 1 - ildaFrame.params.output.color.r; break;
        case 'G': ildaFrame.params.output.color.g = 1 - ildaFrame.params.output.color.g; break;
        case 'B': ildaFrame.params.output.color.b = 1 - ildaFrame.params.output.color.b; break;
            
            // toggle draw lines (on screen only)
        case 'l': ildaFrame.params.draw.lines ^= true; break;
            
            // toggle loop for last poly
        case 'o': ildaFrame.getLastPoly().setClosed(ildaFrame.getLastPoly().isClosed()); break;
            
            // toggle draw points (on screen only)
        case 'p': ildaFrame.params.draw.points ^= true; break;
            
            // adjust point count
        case '.': ildaFrame.polyProcessor.params.targetPointCount++; break;
        case ',': if(ildaFrame.polyProcessor.params.targetPointCount > 10) ildaFrame.polyProcessor.params.targetPointCount--; break;
            
            // adjust point count quicker
        case '>': ildaFrame.polyProcessor.params.targetPointCount += 10; break;
        case '<': if(ildaFrame.polyProcessor.params.targetPointCount > 20) ildaFrame.polyProcessor.params.targetPointCount -= 10; break;
            
            // flip image
        case 'x': ildaFrame.params.output.transform.doFlipX ^= true; break;
        case 'y': ildaFrame.params.output.transform.doFlipY ^= true; break;
            
            // cap image
        case 'X': ildaFrame.params.output.doCapX ^= true; break;
        case 'Y': ildaFrame.params.output.doCapY ^= true; break;
            
            // move output around
        case OF_KEY_UP: ildaFrame.params.output.transform.offset.y -= 0.05; break;
        case OF_KEY_DOWN: ildaFrame.params.output.transform.offset.y += 0.05; break;
        case OF_KEY_LEFT: ildaFrame.params.output.transform.offset.x -= 0.05; break;
        case OF_KEY_RIGHT: ildaFrame.params.output.transform.offset.x += 0.05; break;
            
            // scale output
        case 'w': ildaFrame.params.output.transform.scale.y += 0.05; break;
        case 's': ildaFrame.params.output.transform.scale.y -= 0.05; break;
        case 'a': ildaFrame.params.output.transform.scale.x -= 0.05; break;
        case 'd': ildaFrame.params.output.transform.scale.x += 0.05; break;
            
        case 'C': ildaFrame.drawCalibration(); break;
            
        case '1': ildaFrame.params.output.blankCount-=2; break;
        case '2': ildaFrame.params.output.blankCount+=2; break;
        case '3': ildaFrame.params.output.endCount-=2; break;
        case '4': ildaFrame.params.output.endCount+=2; break;
            
        case '|': showHud ^= showHud;
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    // draw a line to the mouse cursor (normalized coordinates) in the last poly created
    ildaFrame.getLastPoly().lineTo(x / (float)ofGetWidth(), y / (float)ofGetHeight());
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    // create a new poly in the ILDA frame
    ildaFrame.addPoly();
}
