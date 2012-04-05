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

#include "HandManager.h"

HandManager::HandManager(){
	users.clear();
	activeHands.clear();
	pasiveHands.clear();
	
	nUsers = 0;
	nHands = 0;
	activeId = 0;
}

HandManager::~HandManager(){
	users.clear();
	activeHands.clear();
	pasiveHands.clear();
};

void HandManager::addUser(){
	User u;					// Create a new User
	u.id = nUsers;			//	gives him an id
	u.leftId = addHand();	
	u.left = false;
	u.rightId  = addHand();
	u.right = false;
	
	users.push_back(u);
	nUsers++;
}

int HandManager::addHand(){
	Hand h;

	h.id = -1;				// When it becomes active it will have a valid id
	h.centroid.set(0,0,0);
	h.lastCentroid.set(0,0,0);
	h.D.set(0,0,0);

	pasiveHands[nHands] = h;
	nHands++;
	return nHands - 1;
}

/*
void HandManager::delUser(int _id){
	for (int i = 0; i < users.size(); i++) {
		if ( users[i].id == _id ){
			if (users[i].left)
				activeHands.erase(users[i].leftId);
			else
				pasiveHands.erase(users[i].leftId);
			
			if (users[i].right)
				activeHands.erase(users[i].rightId);
			else
				pasiveHands.erase(users[i].rightId);
			
			users.erase(users.begin() + i);
		}
	}
}*/

void HandManager::updateHand(int _id, ofPoint _pos){
	int now = ofGetElapsedTimeMillis();
	
	activeHands[_id].lastCentroid = activeHands[_id].centroid;
	activeHands[_id].centroid = _pos;
	
	ofPoint tD;
	//get the Differences in position
	//tD.set(activeHands[_id].centroid.x - activeHands[_id].lastCentroid.x, 
	//	   activeHands[_id].centroid.y - activeHands[_id].lastCentroid.y);
	//calculate the acceleration
	//float posDelta = sqrtf((tD.x*tD.x)+(tD.y*tD.y));
	
	//float a = 1.0f - 1.0f / expf(posDelta / (1.0f + (float)MOVEMENT_FILTERING*10));
	//activeHands[_id].centroid.x = a * activeHands[_id].centroid.x + (1-a) * activeHands[_id].lastCentroid.x;
	//activeHands[_id].centroid.y = a * activeHands[_id].centroid.y + (1-a) * activeHands[_id].lastCentroid.y;
	
	//get the Differences in position
	activeHands[_id].D.set(activeHands[_id].centroid.x - activeHands[_id].lastCentroid.x, 
						   activeHands[_id].centroid.y - activeHands[_id].lastCentroid.y,
						   activeHands[_id].centroid.z - activeHands[_id].lastCentroid.z);
	
	tD = activeHands[_id].D;
	activeHands[_id].maccel = sqrtf((tD.x* tD.x)+(tD.y*tD.y)) / (now - activeHands[_id].lastTimeTimeWasChecked);
	
	//calculate the age
	activeHands[_id].age = ofGetElapsedTimef() - activeHands[_id].downTime;
	
	activeHands[_id].lastTimeTimeWasChecked = now;
}

void HandManager::activeHand(int _handId){
	activeHands[_handId] = pasiveHands[_handId];
	
	pasiveHands.erase(_handId);
	
	activeHands[_handId].id = activeId;
	activeId++;
	
	activeHands[_handId].downTime = ofGetElapsedTimef();
}

void HandManager::deactiveHand(int _handId){
	activeHands[_handId].id = -1;
	activeHands[_handId].centroid.set(0,0,0);
	activeHands[_handId].lastCentroid.set(0,0,0);
	activeHands[_handId].D.set(0,0,0);
	activeHands[_handId].maccel = 0;
	pasiveHands[_handId] = activeHands[_handId];
	
	activeHands.erase(_handId);
}

void HandManager::activeLeft(int _userId, ofPoint _pos){
	for (int i = 0; i < users.size(); i++) {
		if (users[i].id == _userId){
			if (!(users[i].left)){
				activeHand(users[i].leftId);
				users[i].left = true;
			} 
			
			updateHand(users[i].leftId, _pos);
		}
	}
};

void HandManager::activeRight(int _userId, ofPoint _pos){
	for (int i = 0; i < users.size(); i++) {
		if (users[i].id == _userId){
			if (!(users[i].right)){
				activeHand(users[i].rightId);
				users[i].right = true;
			}
			
			updateHand(users[i].rightId, _pos);
		}
	}
};

void HandManager::deactiveLeft(int _userId){
	for (int i = 0; i < users.size(); i++) {
		if (users[i].id == _userId){
			if (users[i].left){
				deactiveHand(users[i].leftId);
				users[i].left = false;
			}
		}
	}
};

void HandManager::deactiveRight(int _userId){
	for (int i = 0; i < users.size(); i++) {
		if (users[i].id == _userId){
			if (users[i].right){
				deactiveHand(users[i].rightId);
				users[i].right = false;
			}
		}
	}
};

void HandManager::draw(){
	map<int, Hand>::iterator hand;
	for(hand = activeHands.begin(); hand != activeHands.end(); hand++){
		// omit point (0,0) since this means that we are outside of the range
		if((hand->second.centroid.x <= 0) && (hand->second.centroid.y <= 0) && (hand->second.id == -1))
			continue;
		
		ofSetColor(0,255,0);
		ofCircle(hand->second.centroid.x,hand->second.centroid.y, 0, 20);
		ofSetColor(0);
		ofDrawBitmapString(ofToString((int)hand->second.id),hand->second.centroid.x-5,hand->second.centroid.y);
		ofDrawBitmapString("z:"+ofToString((int)hand->second.centroid.z/100.0f),hand->second.centroid.x-15,hand->second.centroid.y+10);
	}
}