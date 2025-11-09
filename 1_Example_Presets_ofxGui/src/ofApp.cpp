#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

#if 1
	// OPTIONAL:
	// To exclude a parameter from the presets group
	scene.bUpdateAnim.setSerializable(false);
#endif

	// Pass group of parameters to the presets manager
	presetsManager.setup(scene.params);
}

//--------------------------------------------------------------
void ofApp::draw() {
	// Background color based on scene parameters
	ofColor c = (scene.amount % 2 == 0) ? scene.c1 : scene.c2;
	ofBackground(c);

	// Draw scene
	scene.draw();

	// Draw gui
	presetsManager.drawGui();

#if 0
	// OPTIONAL:
	// Simple callback to listen to index changes
	if (presetsManager.isChangedIndex()) ofLogNotice("ofApp") << "index changed to " << presetsManager.index.get();
#endif
}
