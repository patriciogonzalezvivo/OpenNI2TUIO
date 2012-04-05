#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    
    
    //ofSetDataPathRoot("data/");
    
	context.setup();
	depth.setup(&context);
	user.setup(&context, true);
	context.toggleMirror();
    
	string host = "localhost";
	int	port	= 3333;
	
	ofxXmlSettings XML;
	
	if (XML.loadFile("config.xml")){
		host = XML.getValue("HOST", "localhost");
		port = XML.getValue("PORT", 3333);

		thresholdDistance = XML.getValue("THRESHOLD", 400);
        trackedUser = XML.getValue("TRACKEDUSER", 0);
	} else 
		cout << "config.xml file not found" << endl;
	
	hManager.setup(host.c_str() ,port);
	hManager.numberUsers(MAXUSER);
    
    logo.loadImage("logo.png");
    
	ofSetWindowShape(640, 480);
}

//--------------------------------------------------------------
void testApp::update(){
	// update the open ni context and user tracker
	context.update();
    depth.update();
	user.update();
	
    if ( user.getNumberOfTrackedUsers() > 0){
        if (trackedUser == 0){
            for(int i = 1; i <= MAXUSER; i++){
                if ( i <= user.getNumberOfTrackedUsers() )
                    trackAndCheck( i );
                else {
                    hManager.deactiveLeft( i );
                    hManager.deactiveRight( i );
                }
            } 
        } else {
            trackAndCheck( trackedUser );
        }
    } else {
        cleanHands( -1 ); //clean them all
    }
    
	hManager.send();
}

void testApp::trackAndCheck(int userNumber){
    if ((userNumber > 0) && (userNumber < MAXUSER+1)){
        ofxTrackedUser* tracked = user.getTrackedUser( userNumber );
        if((tracked != NULL) && tracked->neck.begin.x && tracked->neck.begin.y && tracked->neck.begin.x){
            ofPoint leftHand = tracked->left_lower_arm.end;
            ofPoint rightHand = tracked->right_lower_arm.end;
		
            ofPoint leftShoulder = tracked->left_shoulder.end;
            ofPoint rightShoulder = tracked->right_shoulder.end;
		
            //thresholdDistance = leftShoulder.distance(rightShoulder)* nArmsFactor;
            //thresholdDistance = leftHand.distance(tracked->left_lower_arm.begin)* nArmsFactor;
        
            if ( (leftShoulder.z - leftHand.z ) >= thresholdDistance )
                hManager.activeLeft( userNumber , leftHand);
            else
                hManager.deactiveLeft( userNumber );
		
            if ((rightShoulder.z - rightHand.z ) >= thresholdDistance )
                hManager.activeRight( userNumber, rightHand);
            else 
                hManager.deactiveRight( userNumber );
        
        } else {
            hManager.deactiveLeft( userNumber );
            hManager.deactiveRight( userNumber );
        }
    }
}

void testApp::cleanHands(int exceptUser){
    for(int i = 0; i < MAXUSER ; i++){
        if (i != exceptUser){
            hManager.deactiveLeft( i );
            hManager.deactiveRight( i );
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(100, 100, 100);
	ofSetColor(255);
	
	depth.draw();
	user.draw();
	hManager.draw();
	
	ofSetColor(250);
    ofDrawBitmapString("Threshold distance:  " + ofToString(thresholdDistance), 5,40);

    if(trackedUser == 0)
        ofDrawBitmapString("Tracked User number: ALL", 5,55);
    else
        ofDrawBitmapString("Tracked User number: " + ofToString(trackedUser), 5,55);
    
    ofSetColor(250);
	ofDrawBitmapString("Patricio Gonzalez Vivo.com",ofGetWidth()-260, ofGetHeight()-10 );
	logo.draw(ofGetWidth()-50, ofGetHeight()-50, 47,50.6);
	
	ofSetColor(255,255,255);
    ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate(), 2), 10, 15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch(key){
        case '+':
        case '>':
        case OF_KEY_UP:
            thresholdDistance++;
            break;
		case '-':
        case '<':
        case OF_KEY_DOWN:
			thresholdDistance--;
			break;
        
        case ',':
        case OF_KEY_LEFT:
            if (trackedUser > 0)
                trackedUser--;
            break;
		case '.':
        case OF_KEY_RIGHT:
            if (trackedUser < MAXUSER )
                trackedUser++;
			break;

        case '0':
            trackedUser = 0;
            break;
        case '1':
            trackedUser = 1;
            break;
        case '2':
            trackedUser = 2;
            break;
        case '3':
            trackedUser = 3;
            break;
        case '4':
            trackedUser = 4;
            break;
        case '5':
            trackedUser = 5;
            break;
        case '6':
            trackedUser = 6;
            break;
        case '7':
            trackedUser = 7;
            break;
        case '8':
            trackedUser = 8;
            break;
    }
    
    if ((trackedUser > 0) && (trackedUser <= MAXUSER))
        cleanHands( trackedUser );
    
}

//--------------------------------------------------------------
void testApp::exit(){
    ofxXmlSettings XML;
    if (XML.loadFile("config.xml")){
        XML.setValue("THRESHOLD", thresholdDistance);
        XML.setValue("TRACKEDUSER", trackedUser);
    }
    XML.saveFile();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {	
    if ( x > ofGetWidth()-300 && y > ofGetHeight()-30 )
        ofLaunchBrowser("http://www.patriciogonzalezvivo.com");
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}