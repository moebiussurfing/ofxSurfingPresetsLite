#pragma once
#include "ofMain.h"

#include "ofxSurfingPresetsLiteImgui.h"

#include "ofxSurfingImGui.h"
#include "surfingSceneTesters.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void draw();
	void keyPressed(int key);

	surfingSceneTesters scene;

	ofxSurfingGui ui;
	ofxSurfingPresetsLiteImGui presetsManager;
};
