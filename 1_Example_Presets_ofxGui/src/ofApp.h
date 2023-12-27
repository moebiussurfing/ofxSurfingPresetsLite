#pragma once
#include "ofMain.h"

#include "ofxSurfingPresetsLiteOfxGui.h"

#include "surfingSceneTesters.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void draw();

	SurfingPresetsLiteOfxGui presetsManager;
	SurfingSceneTesters scene;
};
