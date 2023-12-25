#pragma once
#include "ofMain.h"

#include "ofxSurfingPresetsLite.h"

//----

class ofxSurfingPresetsLiteOfxGui : public ofxSurfingPresetsLite {

public:
	ofxPanel gui;
	ofxPanel guiPreset;

	//----

public:
	void setupGui() override {
		ofLogNotice("ofxSurfingPresetsLite") << "setupGui()";

		gui.setup(params);
		guiPreset.setup(paramsPreset);

		refreshGui();
	}

	void refreshGui() override {
		ofLogNotice("ofxSurfingPresetsLite") << "refreshGui()";
		
		gui.getGroup(paramsInternal.getName()).minimize();
		gui.getGroup(paramsManager.getName()).minimize();
		gui.getGroup(paramsKit.getName()).minimize();
	}

	void drawGui() {
		if (!bGui) return;
		drawHelp();

		ofxSurfing::setGuiPositionRightTo(guiPreset,gui);
		
		gui.draw();
		guiPreset.draw();
	}
};
