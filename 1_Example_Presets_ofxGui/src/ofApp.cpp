#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
#if 1
	int w = 1280;
	int h = 720;
	ofSetWindowShape(w, h);
	ofSetWindowPosition(-1920 + 1920 / 2 - w / 2, 1080 / 2 - h / 2);
	// position in left monitor and center on the screen
#endif

	// Exclude parameter from the presets
	scene.bUpdateAnim.setSerializable(false);

	presetsManager.setPathGlobal("myApp");
	presetsManager.setPathPresets("myKit01");
	presetsManager.setup(scene.params);
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofColor c = (scene.amount % 2 == 0) ? scene.c1 : scene.c2;
	ofBackground(c);
	scene.draw();

	presetsManager.drawGui();

	// Simple callback to listen to index changes.
	//if (presetsManager.isChangedIndex()) ofLogNotice("ofApp") << "index changed";
}
