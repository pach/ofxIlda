/*
 * ofxIldaPoly.h
 * =============
 * Created by 武内 満 on 2013/06/02
 * Modified by Omer Shapira.
 *
 * An encapsulation of ofPolyline with extra features.
 * has a reverse() function if directionality is needed.
 *
 */


#pragma once

//#include <algorithm>

namespace ofxIlda {
    
	class Poly: public ofPolyline{
    private:
        ofVec2f headDirection, tailDirection;
        bool reversed, directionChanged;
        
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
                if (hasChanged() || directionChanged /* TODO: or is empty */){
                    headDirection = *(getVertices().begin() + 1) - *getVertices().begin();
                    directionChanged = false;
                }
                return headDirection;
            } else {
                if (hasChanged() || directionChanged /* TODO: or is empty */){
                    tailDirection=(ofVec2f) (*(getVertices().end() - 1) - *(getVertices().end() - 2));
                    tailDirection.normalize();
                    directionChanged = false;
                }
                return tailDirection;
            }
        }
        
        void applyMatrix(ofMatrix4x4& m){
            //TODO: Make this
        }
        
	public:

		ofFloatColor color;
        
        /* Constructors */
        
        Poly(): color(ofFloatColor(1, 1, 1, 1)), reversed(false) {}
        
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
        void reverse(){
            reversed^=reversed;
            directionChanged = true;
        }
        
        const ofVec2f& getHeadDirection(){
            return getEndDirection(true);
        }
        
        const ofVec2f& getTailDirection(){
            return getEndDirection(false);
        }
        
        /* Mutators */
        
        void scaleAboutOrigin(const float scaleBy){
            ofPoint origin = ofPoint(0,0,0);
            scaleAboutPoint(scaleBy, origin);
        }

        void scaleAboutPoint(const float scaleBy, ofPoint& point){
            //TODO: Implement this with a matrix
            std::vector<ofPoint>& pts = getVertices();
            for (std::vector<ofPoint>::iterator pt = pts.begin(); pt != pts.end(); ++pt)
            {
                //TODO: check your math
                //move to origin
                (*pt) = (*pt) - point;
                //scale about origin
                pt->scale(scaleBy);
                //move back
                (*pt) = (*pt) + point;
            }
        }
        
        void translate(ofPoint& moveBy){
            //TODO: Make this using applyMatrix
            std::vector<ofPoint>& pts = getVertices();
            for (std::vector<ofPoint>::iterator pt = pts.begin(); pt != pts.end(); ++pt)
            {
                (*pt) = (*pt) + moveBy;
            }
        }
        
        /**
         * Changes the max dimension to 1.0, and moves the centroid to 0.5,0.5.
         */
        
        void normalise(){
            //TODO: Change to use applyMatrix
            ofRectangle boundingBox = getBoundingBox();
            float corner = MAX(boundingBox.getWidth(), boundingBox.getHeight());
            ofPoint centroid = getCentroid2D();
            scaleAboutPoint(1.0/corner, centroid);
            ofPoint moveBy = ofPoint(0.5,0.5,0) - centroid;
            translate(moveBy);
        }
        
        void conformDirection() {
            if (reversed){
                //TODO: see if this leads to a bad access
                std::reverse(getVertices().begin(), getVertices().end());
            }
        }
    };
}