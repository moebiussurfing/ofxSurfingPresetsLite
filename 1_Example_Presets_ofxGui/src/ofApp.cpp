#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#if 1
	ofSetWindowPosition(-1900, 50);
	ofSetWindowShape(1280, 720);
#endif

	presetsManager.addGroup(scene.params);
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofBackground(scene.amount % 2 == 0 ? scene.c1 : scene.c2);
	scene.draw();

	presetsManager.drawGui();
}
