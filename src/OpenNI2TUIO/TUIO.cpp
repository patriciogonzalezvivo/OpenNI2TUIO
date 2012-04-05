/*
*  TUIO.h
*  
*
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#include "TUIO.h"

TUIO::TUIO(){
	
}

TUIO::~TUIO(){
	// this could be useful for whenever we get rid of an object
}

void TUIO::setup(const char* host, int port) {
	ofxXmlSettings XML;
	
	localHost = host;
	TUIOPort = port;
	frameseq = 0;
	
	sendZ = true;
	header = "/tuio/2Dcur";
	
	if (XML.loadFile("config.xml")){
		sendZ = XML.getValue("SENDZ", 1);
	} else 
		cout << "config.xml file not found" << endl;
	
    if (sendZ)
        header = XML.getValue("HEADER", "/tuio/3Dcur");
    else
        header = XML.getValue("HEADER", "/tuio/2Dcur");
    
	TUIOSocket.setup(localHost, TUIOPort);
}

void TUIO::send(std::map<int, Hand> * hands){
	frameseq += 1;
	
	ofxOscBundle b;
	ofxOscMessage alive;
	// Sends alive message - saying 'Hey, there's no alive blobs'
	alive.setAddress(header);
	alive.addStringArg("alive");
			
	// Send fseq message
	ofxOscMessage fseq;
	fseq.setAddress( header );
	fseq.addStringArg( "fseq" );
	fseq.addIntArg(frameseq);
			
	if(hands->size() == 0){
		b.addMessage( alive );		// add message to bundle
		b.addMessage( fseq );		// add message to bundle
		TUIOSocket.sendBundle( b ); // send bundle
	} else {
		map<int, Hand>::iterator hand;
		for(hand = hands->begin(); hand != hands->end(); hand++){
			// omit point (0,0) since this means that we are outside of the range
			if((hand->second.centroid.x <= 0) && (hand->second.centroid.y <= 0) && (hand->second.id == -1))
				continue;
			//Set Message
			ofxOscMessage set;
			set.setAddress( header );
			set.addStringArg("set");
			set.addIntArg(hand->second.id);				// id
			set.addFloatArg(hand->second.centroid.x/ofGetWidth());	// x
			set.addFloatArg(hand->second.centroid.y/ofGetHeight());	// y
			if (sendZ)
				set.addFloatArg(hand->second.centroid.z/100.0f);	// z
			set.addFloatArg(hand->second.D.x/ofGetWidth());			// dX
			set.addFloatArg(hand->second.D.y/ofGetHeight());		// dY
			if (sendZ)
				set.addFloatArg(hand->second.D.z/100.0f);			// dZ
			set.addFloatArg(hand->second.maccel);		// m
			b.addMessage( set );						// add message to bundle
			alive.addIntArg(hand->second.id);			// add blob to list of ALL active IDs
		}
		b.addMessage( alive );	 //add message to bundle
		b.addMessage( fseq );	 //add message to bundle
		TUIOSocket.sendBundle( b ); //send bundle
	}
}