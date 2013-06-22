//
//  ofxIldaPoly.h
//  interactivelaser
//
//  Created by 武内 満 on 2013/06/02.
//
//

#pragma once

//#include <algorithm>

namespace ofxIlda {
    
	class Poly: public ofPolyline{
    private:
        ofVec2f headDirection, tailDirection;
        bool reversed;
        
        /**
         * Returns the current head or tail, according to the direction (reversed param)
         * @param head whether or not to return the head of the line
         */
        const ofPoint& getEnd(bool head){
            bool order = head && (!reversed);
            if (order){
                //return head;
                return *(getVertices().begin());
            } else {
                //return tail
                return *(getVertices().end()-1);
            }
        }
        /**
         * Returns the current head or tail directions, according to the direction (reversed param)
         * recalculates if necessary
         * @param head whether or not to return the head of the line
         */
        
        const ofVec2f& getEndDirection(bool head){
            bool order = head && (!reversed);
            if (order){
                //return head
                if (hasChanged() /* TODO: or is empty */){
                    headDirection = *(getVertices().begin() + 1) - *getVertices().begin();
                }
                return headDirection;
            } else {
                if (hasChanged() /* TODO: or is empty */){
                    tailDirection=(ofVec2f) (*(getVertices().end() - 1) - *(getVertices().end() - 2));
                    tailDirection.normalize();
                }
                return tailDirection;
            }
        }
        
        
	public:

		ofFloatColor color;
        
        /* Constructors */
        
		Poly() : color(ofFloatColor(1, 1, 1, 1)), reversed(false) {}
        
        Poly(ofFloatColor color) : color(color), reversed(false) {}
        
        Poly(const Poly& poly) : ofPolyline(poly), color(poly.color), reversed(false) {}
        
        Poly(const ofPolyline& polyline) : ofPolyline(polyline), color(ofFloatColor(1, 1, 1, 1)), reversed(false) {}
        
        Poly(const ofPolyline& polyline, ofFloatColor color) : ofPolyline(polyline), color(color), reversed(false) {}
        
		Poly(const vector<ofPoint>& verts) : ofPolyline(verts), color(ofFloatColor(1, 1, 1, 1)), reversed(false) {}
        
		Poly(const vector<ofPoint>& verts, ofFloatColor color) : ofPolyline(verts), color(color), reversed(false) {}
        
        void setFromPolyline(const ofPolyline& polyline) {
            ofFloatColor tmpColor = color;
            *this = polyline;
            color = tmpColor;
            reversed = false;
        }
        
        /* Getters and Setters */
        
        const ofPoint& getHead(){
            getEnd(true);
        }
        const ofPoint& getTail(){
            getEnd(false);
        }
        
        bool const isReversed(){ return reversed; }
        void reverse(){ reversed^=reversed; }
        
        const ofVec2f& getHeadDirection(){
            return getEndDirection(true);
        }
        
        const ofVec2f& getTailDirection(){
            return getEndDirection(false);
        }
        
        /* Mutators */
        
        void conformDirection() {
            if (reversed){
                //TODO: see if this leads to a bad access
                std::reverse(getVertices().begin(), getVertices().end());
            }
        }
    };
}