#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	// load the default ofxOpenNI config settings
	// see bin/data/openni/config/ofxopenni_config.xml
	context.setupUsingXMLFile();
	
	// setup resources
	depth.setup(&context);
	image.setup(&context);
	user.setup(&context, &depth);
	
	ofSetWindowShape(640, 480);
	
	imagePixels = new unsigned char[640 * 480];
	img.allocate(640, 480);
	fingers.clear();
}

//--------------------------------------------------------------
void testApp::update(){
	// update the open ni context and user tracker
	context.update();
	user.update();
	
	// Extract depth map (pix) and user mask (userPix)
	xn::DepthGenerator depthGenerator = depth.getXnDepthGenerator();
	xn::DepthMetaData depthmetadata;
	depthGenerator.GetMetaData(depthmetadata);
	unsigned short* pix = (unsigned short*) depthmetadata.Data();
	
	xn::UserGenerator userGenerator = user.getXnUserGenerator();
	xn::SceneMetaData sceneMetaData;
	userGenerator.GetUserPixels(0, sceneMetaData);
	unsigned short *userPix = (unsigned short*)sceneMetaData.Data();
	
	// Calculate the deepest user point
	float deepest = 0.0;
	for (int i =0 ; i < 640 * 480; i++) {
		imagePixels[i] = 0;
		
		if(userPix[i]>0 && pix[i]>deepest) 
			deepest = pix[i];
	}
	
	// Check for Tracked Users and then check for active hands
	for(int i = 0; i < user.getNumTrackedUsers(); ++i){
		ofxTrackedUser* tracked = user.getTrackedUser(i);
		
		if((tracked != NULL) && tracked->neck.begin.x && tracked->neck.begin.y && tracked->neck.begin.x){
			ofPoint leftHand = tracked->left_lower_arm.end;
			ofPoint rightHand = tracked->right_lower_arm.end;
		
			ofPoint leftShoulder = tracked->left_shoulder.end;
			ofPoint rightShoulder = tracked->right_shoulder.end;
		
			float thresholdDistance = leftHand.distance(tracked->left_lower_arm.begin);
			
			//------------------------------------------------------ Check Active Hands
			int leftDist = leftShoulder.z - leftHand.z;
			if ( leftDist >= thresholdDistance ){
				for( int x = 0; x < 640; x++ ){
					for( int y = 0; y < 480; y++ ){
						int p = y * 640 + x;
						if ((ofDist(leftHand.x, leftHand.y, x, y) < 70) && userPix[p]>0 ){
							char depthHand = CLAMP(255-pix[(int)leftHand.y * 640 + (int)leftHand.x] / (deepest/(220)), 0, 255);
							char depthVal= CLAMP(255-pix[p] / (deepest/(220)), 0, 255);
							imagePixels[p] = ((depthVal*1.1) >= depthHand )? 255:0;
						}
					}
				}
			}
			
			int rightDist = rightShoulder.z - rightHand.z;
			if ( rightDist >= thresholdDistance ){
				for( int x = 0; x < 640; x++ ){
					for( int y = 0; y < 480; y++ ){
						int p = y * 640 + x;
						if ((ofDist(rightHand.x, rightHand.y, x, y) < 70) && userPix[p]>0 ){
							char depthHand = CLAMP(255-pix[(int)rightHand.y * 640 + (int)rightHand.x] / (deepest/(220)), 0, 255);
							char depthVal= CLAMP(255-pix[p] / (deepest/(220)), 0, 255);
							imagePixels[p] = ((depthVal*1.1) >= depthHand )? 255:0;
						}
					}
				}
			}
		}
	}
	
	img.setFromPixels(imagePixels, 640, 480);
	fingers = getFingerTips(img);
}

vector<ofPoint> testApp::getFingerTips( ofxCvGrayscaleImage input) {
	CvMemStorage*	storage = cvCreateMemStorage(0);
	CvSeq*			contours;
	CvPoint*		PointArray;
	
	int*			hull;
    int				hullsize;
	
	vector<ofPoint> fingerTips;
	
    //START TO FIND THE HULL POINTS
    cvFindContours( input.getCvImage(), storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	
	// If there is a contour it«ll make it more simple
    if (contours)
		contours = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, 10, 1 );
	
    for( ; contours != 0; contours = contours->h_next ){
        int count = contours->total; // This is number point in contour
		
        CvRect rect = cvContourBoundingRect( contours, 1);
		
        if ( (rect.width*rect.height) > 300 ){		// Analize the bigger contour
            PointArray = (CvPoint*)malloc( count*sizeof(CvPoint) ); // Alloc memory for contour point set.
            hull = (int*)malloc(sizeof(int)*count);	// Alloc memory for indices of convex hull vertices.
			
            cvCvtSeqToArray(contours, PointArray, CV_WHOLE_SEQ); // Get contour point set.
			
            // Find convex hull for curent contour.
            cvConvexHull( PointArray,
						 count,
						 NULL,
						 CV_COUNTER_CLOCKWISE,
						 hull,
						 &hullsize);
			
			int upper = 640, lower = 0;
			for	(int j=0; j<hullsize; j++) {
				int idx = hull[j]; // corner index
				if (PointArray[idx].y < upper) 
					upper = PointArray[idx].y;
				if (PointArray[idx].y > lower) 
					lower = PointArray[idx].y;
			}
			float cutoff = lower - (lower - upper) * 0.1f;
			
			// find interior angles of hull corners
			for (int j=0; j<hullsize; j++) {
				int idx = hull[j]; // corner index
				int pdx = idx == 0 ? count - 1 : idx - 1; //  predecessor of idx
				int sdx = idx == count - 1 ? 0 : idx + 1; // successor of idx
				
				cv::Point v1 = cv::Point(PointArray[sdx].x - PointArray[idx].x, PointArray[sdx].y - PointArray[idx].y);
				cv::Point v2 = cv::Point(PointArray[pdx].x - PointArray[idx].x, PointArray[pdx].y - PointArray[idx].y);
				
				float angle = acos( (v1.x*v2.x + v1.y*v2.y) / (norm(v1) * norm(v2)) );
				
				// low interior angle + within upper 90% of region -> we got a finger
				if (angle < 1 && PointArray[idx].y < cutoff) {
					int u = PointArray[idx].x;
					int v = PointArray[idx].y;
					
					fingerTips.push_back(ofPoint(u,v));
				}
			}
			
            // Free memory.
            free(PointArray);
            free(hull);
			
        }
    }
	
    cvClearMemStorage( storage );
	
	return fingerTips;
}



//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(100, 100, 100);
	ofSetColor(255);
	
	//depth.draw();
	//image.draw(645, 10, 400, 300);
	//image.draw(0, 0, 640, 480);
	//ofSetColor(255);
	//img.draw(0,0,640,480);
	
	
	// draw all the users automatically?
	user.draw();
	
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetColor(255);
	img.draw(0,0,640,480);
	ofDisableBlendMode();

	for(int i = 0; i < fingers.size(); i++){
		ofSetColor(255,0,0);
		ofCircle(fingers[i].x, fingers[i].y,5);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
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
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}