/*
 *  OpenNI2TUIO
 *
 *  Created by Patricio González Vivo on 25/02/11.
 *  Copyright 2011 http://PatricioGonzalezVivo.com All rights reserved.
 *  http://www.tuio.org/?tuio11
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

#ifndef HANDMANAGER
#define HANDMANAGER

#include "Hand.h"
#include "TUIO.h"

typedef struct{
	int id;
	bool left, right;
	int leftId, rightId;
} User;

class HandManager{
public:
	TUIO	tuio;
	
	int		nUsers, nHands, activeId;
	
	vector<User> users;
	map<int, Hand> activeHands;
	map<int, Hand> pasiveHands;
	
	HandManager();
	~HandManager();
	
	void	setup(const char* host, int port){
		tuio.setup(host, port);
	};
	
	void	numberUsers(int _n){
		for (int i = 0; i < _n; i++)
			addUser();
	};
	
	void	addUser();				// Add a user into the vector... and bouth hands into passive
	//void	delUser(int _id);		// Del a user from the vector and bouth hands
	
	int		addHand();
	void	updateHand(int _handId, ofPoint _pos);
	
	void	activeHand(int _handId);
	void	deactiveHand(int _handId);
	
	void	activeLeft(int _userId, ofPoint _pos);
	void	activeRight(int _userId, ofPoint _pos);
	void	deactiveLeft(int _userId);
	void	deactiveRight(int _userId);
	
	void	draw();
	
	void	send(){
		tuio.send(&activeHands);
	};
};

#endif