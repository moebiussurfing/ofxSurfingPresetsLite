#pragma once
#include "ofMain.h"

#include "ofxSurfingPresetsLite.h"

//----

class ofxSurfingPresetsLiteOfxGui : public ofxSurfingPresetsLite {

public:
	ofxPanel gui;
	ofxPanel guiParams;
	SurfingOfxGuiPanelsManager guiManager;

	//----

public:
	void setupGui() override {
		ofLogNotice("ofxSurfingPresetsLite") << "setupGui()";

		gui.setup(parameters);
		guiParams.setup(paramsPreset);

		guiManager.setup(&gui);
		guiManager.add(&gui, bGui);
		guiManager.add(&guiParams, bGuiParams);
		guiManager.startup();

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

		//ofxSurfing::setGuiPositionRightTo(guiParams,gui);
		//gui.draw();
		//if (bGuiParams) guiParams.draw();

		guiManager.draw();
	}

	void drawHelp() override {
		if (!bHelp) return;

		//buildHelp();

		if (bGui && bGuiParams)
			ofxSurfing::ofDrawBitmapStringBox(sHelp, &guiParams);
		else if (bGui && !bGuiParams)
			ofxSurfing::ofDrawBitmapStringBox(sHelp, &gui);
	}
};
