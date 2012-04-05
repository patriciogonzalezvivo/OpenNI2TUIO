/*
*  TUIO.h
*  
*  Created on 2/2/09.
*  Copyright 2009 NUI Group. All rights reserved.
*
*/

#ifndef TUIO_H
#define TUIO_H

#include "ofxXmlSettings.h"
#include "ofxOsc.h"
#include "Hand.h"

class TUIO {
	public:		
		TUIO();
		~TUIO();
		
		//methods
		void setup(const char* host, int port);
		void send(std::map<int, Hand> * hands);

		ofxOscSender	TUIOSocket; 
		const char*		localHost;
		int				TUIOPort;	
		int				TUIOFlashPort;
		bool 			bHeightWidth;
		bool			bIsConnected;
	
		bool			sendZ;
		string			header;

	private:
		int				frameseq;
		bool			send(string message);
		string			partialPrevMsg;
};

#endif