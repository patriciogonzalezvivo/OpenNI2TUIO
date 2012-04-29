#ifndef _TEST_APP
#define _TEST_APP

// uncomment this for OF 007 compatibility
//#define OFX_OPENNI_BETA

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxOpenCv.h"

class testApp : public ofBaseApp{
public:
	
	ofxOpenNIContext	context;
	ofxDepthGenerator	depth;
	ofxUserGenerator	user;
	ofxImageGenerator	image;
	
	float thresholdDistance;
	
	unsigned char * imagePixels;
	ofxCvGrayscaleImage img;
	
	ofxCvContourFinder 	contourFinder;
	vector<ofPoint> fingers;
	vector<ofPoint> getFingerTips( ofxCvGrayscaleImage input);
	
	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
};
#endif
