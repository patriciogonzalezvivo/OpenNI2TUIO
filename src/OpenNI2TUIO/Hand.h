/*
 *  OpenNI2TUIO
 *
 *  Created by Patricio González Vivo on 25/02/11.
 *  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the author nor the names of its contributors
 *       may be used to endorse or promote products derived from this software
 *       without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***********************************************************************/

#ifndef HAND
#define HAND

#include "ofMain.h"

class Hand {
public: 
	
	int					id;
	float				maccel;  //distance traveled since last frame
	float				age;     //how long the blob has been at war
	float				sitting; //how long hes been sitting in the same place
	float				downTime;
	float				lastTimeTimeWasChecked;
	ofPoint             centroid, lastCentroid, D;
	int					color;				
	
	//----------------------------------------
	Hand() {
		//simulated	= false;
		age			= 0.0f;
		sitting		= 0.0f;
		color		= 0xFFFFFF;
		
		//freakishly long variable name (ala Apple)
		lastTimeTimeWasChecked = ofGetElapsedTimeMillis(); //get current time as of creation
	}
	
	//----------------------------------------
	void draw(float x = 0, float y = 0, float inputWidth = ofGetWidth(), float inputHeight = ofGetHeight(), float outputWidth = ofGetWidth(), float outputHeight = ofGetHeight()){		
		ofPushMatrix();
		ofTranslate(x/inputWidth * outputWidth, y/inputHeight * outputHeight, 0.0f);
		//glTranslatef(-(x/inputWidth * outputWidth), -(y/inputHeight * outputHeight), 0.0f);                
		ofNoFill();
		ofSetColor(255);
		ofCircle(0, 0, 10);		
		ofPopMatrix();
	}
};
#endif