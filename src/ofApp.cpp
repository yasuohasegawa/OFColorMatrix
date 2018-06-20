#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	srcImage.load("images/mona.jpg");
	shader.load("shader/shader.vert", "shader/shader.frag");

	//colorMat.invert();//invert
	//colorMat.adjustSaturation(10.0); // saturation
	//colorMat.adjustContrast(120.0,NAN,NAN); // contrast
	//colorMat.adjustBrightness(100, 100, 100); // brightness
	//colorMat.toGreyscale(0.5, 0.5, 0.5); // grayscale
	//colorMat.adjustHue(45.0);//hue

	//colorMat.randomize(0.7); // randomize color
	//colorMat.RGB2YUV(); // rgb to yuv
	//colorMat.YUV2RGB(); // yuv to rgb
	//colorMat.RGB2YIQ(); // rgb to yiq

	//colorMat.rotateRed(90.0); // rotate
	//colorMat.shearRed(0.5, 0.3); // shear
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	float scale = 2.0;
	float w = 220* scale;
	float h = 293* scale;

	shader.begin();
	shader.setUniformTexture("texture", srcImage, 0);
	shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	shader.setUniform1f("time", ofGetElapsedTimef());
	
	colorMat.apply(shader);

	srcImage.resize(w, h);
	srcImage.draw(0, 0, w, h);
	shader.end();

	// FPS
	ofSetColor(0, 0, 0);
	string msg = "fps: " + ofToString(ofGetFrameRate(), 2);
	ofDrawBitmapString(msg, ofGetWidth() - 100, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
