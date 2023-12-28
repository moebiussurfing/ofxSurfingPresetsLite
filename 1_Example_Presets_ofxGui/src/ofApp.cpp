#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// OPTIONAL: 
	// customize window
#if 1
	int w = 1280;
	int h = 720;
	ofSetWindowShape(w, h);
	ofSetWindowPosition(-1920 + 1920 / 2 - w / 2, 1080 / 2 - h / 2);
	// position in left monitor and center on the screen
#endif

#if 1
	// OPTIONAL:
	// Exclude a parameter from the presets group
	scene.bUpdateAnim.setSerializable(false);
#endif

#if 1 
	// OPTIONAL: 
	// for custom paths or multiple kits
	//presetsManager.setPathGlobal("myApp");
	presetsManager.setKitName("myKit01");
#endif

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
