//
//  ofxIldaPolyProcessor.h
//  ofxILDA demo
//
//  Created by Memo Akten on 21/05/2013.
//  Modified by Omer Shapira http://omershapira.com
//


/**
 * TODO:
 * * Create a path resampler based on curvature
 * * Create a polygon connector
 */

#pragma once

#include "ofxIldaPoly.h"

namespace ofxIlda {
    class PolyProcessor {
    public:
        struct {
            int smoothAmount;   // how much to smooth the path (zero to ignore)
            float optimizeTolerance;    // howmuch to optimize the path, based on curvature (zero to ignore)
            bool collapse;  // (not implemented yet)
            int targetPointCount;   // how many points in total should ALL paths in this frame be resampled to (zero to ignore)
            float spacing;  // desired spacing between points. Set automatically by targetPointCount, or set manually. (zero to ignore)
            bool doSpacing;
        } params;
        
        struct {
            float angleWeight; //How much weight to give to the change of angle at the beginning/end of a line
            float distanceWeight; //How much weight to give to a distance between one line end and another beginning
        } optimizationParams;
        
        
        //--------------------------------------------------------------
        PolyProcessor() {
            memset(&params, 0, sizeof(params));
            params.smoothAmount = 0;
            params.optimizeTolerance = 0;
            params.collapse = 0;
            params.targetPointCount = 500;
            params.spacing = 0;
            params.doSpacing = true;
            
            //TODO: Acutally balance between the two when they're both implemented
            optimizationParams.angleWeight = 0;
            optimizationParams.distanceWeight = 1;
        }
        
        //--------------------------------------------------------------
        string getString() {
            stringstream s;
            s << "polyProcessor.params:" << endl;
            s << "smoothAmount : " << params.smoothAmount << endl;
            s << "optimizeTolerance : " << params.optimizeTolerance << endl;
            s << "collapse : " << params.collapse << endl;
            s << "targetPointCount : " << params.targetPointCount << endl;
            s << "spacing : " << params.spacing << endl;
            return s.str();
        }
        
        //--------------------------------------------------------------
        void update(const vector<Poly> &origPolys, vector<Poly> &processedPolys) {
            float totalLength = 0;
            vector<int> pathLengths;
            processedPolys = origPolys;
            
            for(int i=0; i<processedPolys.size(); i++) {
                if(processedPolys[i].size()) {
                    //reverse if necessary
                    for (int i = 0; i<processedPolys.size(); i++) {
                        processedPolys[i].conformDirection();
                    }
                    
                    // smooth paths
                    if(params.smoothAmount > 0) processedPolys[i].setFromPolyline(processedPolys[i].getSmoothed(params.smoothAmount));
                    
                    // optimize paths
                    if(params.optimizeTolerance > 0) processedPolys[i].simplify(params.optimizeTolerance);
                    
                    // calculate total length (needed for auto spacing calculation)
                    if(params.targetPointCount > 0) {
                        float l = processedPolys[i].getPerimeter();
                        totalLength += l;
                        pathLengths.push_back(l);
                    }
                } else {
                    pathLengths.push_back(0);
                }
            }
            
            
            // calculate spacing based on desired total number of points
            if(params.targetPointCount) {
                params.spacing = totalLength / params.targetPointCount;
            }
            
            
            // resample paths based on spacing (either as calculated by targetPointCount, or set by user)
            if(params.doSpacing && params.spacing) {
                for(int i=0; i<processedPolys.size(); i++) {
                    processedPolys[i].setFromPolyline(processedPolys[i].getResampledBySpacing(params.spacing));
                }
            }
            sequenceBeam(processedPolys, false);
        }
        
        //--------------------------------------------------------------
        /**
         * Beam sequence planner.
         *
         * FIRST BASIC SOLUTION: Start out at current beam point, look for closest point (in or out)
         * in a different polygon
         *
         */
        void sequenceBeam(vector<Poly> &polys, bool lookahead, ofPoint lastPoint=ofPoint(0.5,0.5)){
             if (!lookahead){
                //convenience Point:
                Poly p;
                //Two, so a direction would exist
                p.lineTo(lastPoint);
                p.lineTo(lastPoint);
                //insert conveniece point
                polys.insert(polys.begin(), p);
                
                for (int i = 0; i < polys.size(); i++) {
                    float minCost = INFINITY;
                    int indexShift = 0;
                    for (int j = 1; i + j < polys.size(); j++) {
                        float c = cost(polys[i], polys[i+j], false);
                        //If the head is closer, mark it
                        if (c < minCost){
                            indexShift = j;
                            minCost = c;
                        }
                        c = cost(polys[i], polys[i+j], true);
                        //if the tail is closer, set the polyline as reversed and mark it
                        if (c < minCost){
                            polys[i+j].reverse();
                            indexShift = j;
                            minCost = c;
                        }
                    }
                    //found the closest one? bring it to the front
                    if (indexShift>0){
                        //TODO: go over the math again.
                        std::iter_swap(polys.begin() + i + 1, polys.begin() + i + indexShift);
                    }
                }
                //remove convenience point
                polys.erase(polys.begin());
            }
        }
        
        float cost(Poly l1, Poly l2, bool flipL2){
            float c = 0;
            //FIXME: Assumes normalized. Add cases for normalized vs non-normalized
            ofPoint p1 = l1.getHead();
            ofPoint p2 = flipL2 ? l2.getTail() : l2.getHead();
            c += p1.distance(p2) * optimizationParams.distanceWeight;
            //TODO: implement angle weight with dot products
            
            return c;
        }
        
    };
    
}   