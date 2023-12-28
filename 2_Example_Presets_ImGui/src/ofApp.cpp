#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#if 1
	int w = 1900;
	int h = 1030;
	ofSetWindowShape(w, h);
	ofSetWindowPosition(-1920 + 1920 / 2 - w / 2, 1080 / 2 - h / 2);
	// position in left monitor and center on the screen
#endif

	presetsManager.setUiPtr(&ui);
	presetsManager.setup(scene.params);
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