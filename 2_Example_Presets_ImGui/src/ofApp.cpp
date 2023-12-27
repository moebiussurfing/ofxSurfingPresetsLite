#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#if 1
	ofSetWindowPosition(-1900, 50);
	ofSetWindowShape(1850, 1060);
#endif

	presetsManager.setUiPtr(&ui);
	presetsManager.addGroup(scene.params);
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofColor c = (scene.amount % 2 == 0) ? scene.c1 : scene.c2;
	ofBackground(c);
	scene.draw();

	ui.begin();
	{
		presetsManager.drawGui();
	}
	ui.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == ' ') presetsManager.setToggleMinimalMode();
}