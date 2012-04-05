#ifndef _TEST_APP
#define _TEST_APP

#define MAXUSER 8

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "HandManager.h"
#include "ofxXmlSettings.h"

class testApp : public ofBaseApp{
public:
	
	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    
    void trackAndCheck( int userNumber );
    void cleanHands( int exceptUser );
    
    ofxOpenNIContext	context;
	ofxDepthGenerator	depth;
	ofxUserGenerator	user;
	
    HandManager hManager;
    ofImage logo;
    
    int trackedUser;
	
    float thresholdDistance;
};
#endif
