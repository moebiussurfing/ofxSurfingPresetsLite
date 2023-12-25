#pragma once
#include "ofMain.h"

#include "ofxSurfingPresetsLiteImgui.h"

#include "ofxSurfingImGui.h"
#include "surfingSceneTesters.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void draw();

	ofxSurfingGui ui;

	ofxSurfingPresetsLiteImGui presetsManager;
	surfingSceneTesters scene;
};
