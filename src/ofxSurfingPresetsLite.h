
#pragma once
#include "ofMain.h"

#include "ofxSurfingHelpersLite.h"
#include "ofxSurfingHelpersParameters.h"

//--

/*
	TODO: 
    add auto saver when changes.
		using autoSaver?
		avoid exit save.
	add multiple ofParameterGroup/s
	in the imgui matrix: 
		fix colors on flipping 
		fix direction
	add rename/custom names presets?
*/

//----

class SurfingPresetsLite {
public:
	SurfingPresetsLite() {
		ofLogNotice("SurfingPresetsLite") << "Constructor()";
		addListeners();

		dir.allowExt("JSON");
		dir.allowExt("json");
	}

	~SurfingPresetsLite() {
		ofLogNotice("SurfingPresetsLite") << "Destructor()";

		removeListeners();

		destructUI();

		if (!bDoneExit) {
			ofLogWarning("SurfingPresetsLite") << "Call exit() forced!";
			exit();
		}
	}

protected:
	void addListeners() {

		ofAddListener(parameters.parameterChangedE(), this, &SurfingPresetsLite::Changed);
		ofAddListener(ofEvents().update, this, &SurfingPresetsLite::update);
		ofAddListener(ofEvents().exit, this, &SurfingPresetsLite::exit);

		ofSetLogLevel("SurfingPresetsLite", OF_LOG_NOTICE);

		addKeysListeners();
	}

	void removeListeners() {

		ofRemoveListener(parameters.parameterChangedE(), this, &SurfingPresetsLite::Changed);
		ofRemoveListener(ofEvents().update, this, &SurfingPresetsLite::update);
		ofRemoveListener(ofEvents().exit, this, &SurfingPresetsLite::exit);

		removeKeysListeners();
	}

	virtual void destructUI() {
		ofLogNotice("SurfingPresetsLite") << "destructUI() empty";
	}

private:
	void exit(ofEventArgs & args) {
		ofLogNotice("SurfingPresetsLite") << "exit(args)";
		exit();
	}

	void exit() {
		ofLogNotice("SurfingPresetsLite") << "exit()";
		// save settings
		std::string s;
		if (pathGlobal == "")
			s = pathSettings;
		else
			s = pathGlobal + "\\" + pathSettings;
		ofxSurfing::saveGroup(parameters, s);

		if (bAutoSave) doSave(false);

		bDoneExit = true;
	}

	//private:
public:
	// //TODO: fix
	// Customize name to avoid window names colliding
	// with other preset manager instances
	//--------------------------------------------------------------
	void setNameUI(std::string s) {
		std::string n = "";
		n += "UI ";
		n += "PRESETS ";
		n += s;
		bGui.set(n, true);
	}

private:
	std::string pathSettings = "Presets.json";

	bool bDoneStartup = false;
	bool bDoneExit = false;

	//----

protected:
	bool bModKeyControl = false;
	bool bModKeyAlt = false;

	//----

public:
	ofParameter<void> vReset { "Reset" };
	ofParameter<void> vResetSetInit { "Reset Set" };
	ofParameter<void> vRandom { "Random" };

	ofParameter<int> index { "Index", -1, -1, -1 };

	ofParameter<bool> bCycled { "Cycled", true };
	ofParameter<int> numPresetsForPopulating { "Num presets", 9, 1, 32 }; // max presets but only for populating functions!

public:
	ofParameter<bool> bGui { "PRESETS", true };
	ofParameter<bool> bGuiParams { "UI PARAMETERS", true };
	ofParameter<bool> bGuiClicker { "UI CLICKER", false };
	ofParameter<bool> bGuiFloating { "Floating Window", true };
	ofParameter<bool> bGuiExpand { "Expand", false };

private:
	int index_PRE = -2; // pre

public:
	void setToggleGuiVisible() { bGui = !bGui; }
	void setGuiVisible(bool b = true) { bGui = b; }
	bool getGuiVisible() { return bGui; }

	//--

public:
	ofParameter<void> vPrevious { "<" };
	ofParameter<void> vNext { ">" };

protected:
	ofParameter<void> vNew { "New" };
	ofParameter<void> vDelete { "Delete" };
	ofParameter<void> vSave { "Save" };
	ofParameter<void> vLoad { "Load" };

	ofParameter<void> vScanKit { "Scan Files" };
	ofParameter<void> vClearKit { "Clear Kit" };
	ofParameter<void> vPopulateKit { "Populate Current" };
	ofParameter<void> vPopulateRandomKit { "Populate Random" };

	ofParameter<bool> bAutoSave { "AutoSave", true }; // edit mode
	ofParameter<bool> bKeys { "Keys", true };
	ofParameter<bool> bHelp { "Help", true };

	vector<char> keyCommandsChars;

	//ofParameter<std::string> indexName { "Name", "NONE" };//TODO
	//ofParameter<bool> bGuiClickerMini { "Mini Clicker", false };
	//ofParameter<void> vRename { "Rename" }; //TODO

private:
	void setup() {
		ofLogNotice("SurfingPresetsLite") << "setup()";

		doRefreshKitFiles();
		setupParameters();
		setupGui();
	}

	virtual void refreshGui() {
		ofLogNotice("SurfingPresetsLite") << "refreshGui() empty";
	}

	virtual void setupParametersExtra() {
		ofLogNotice("SurfingPresetsLite") << "setupParametersExtra() empty";
	}

	virtual void setupParameters() {
		ofLogNotice("SurfingPresetsLite") << "setupParameters()";

		//indexName.setSerializable(false);
		paramsKit.setSerializable(false);

		paramsBrowse.add(index);
		paramsBrowse.add(vNext);
		paramsBrowse.add(vPrevious);

		paramsManager.add(vSave);
		paramsManager.add(vLoad);
		paramsManager.add(vNew);
		paramsManager.add(vDelete);
		paramsManager.add(vResetSetInit);
		paramsManager.add(vReset);
		paramsManager.add(vRandom);
		paramsManager.add(bAutoSave);

		paramsKit.add(vPopulateKit);
		paramsKit.add(vPopulateRandomKit);
		paramsKit.add(vClearKit);
		paramsKit.add(vScanKit);

		paramsAdvanced.add(bCycled);
		paramsAdvanced.add(numPresetsForPopulating);
		paramsInternal.add(bGui); // only for serializing settings. could be hidden of the ui
		paramsAdvanced.add(paramsInternal);

		//parameters.add(bGuiParams);//exposed also on guiManager
		parameters.add(paramsBrowse);
		parameters.add(paramsManager);
		parameters.add(paramsKit);
		parameters.add(paramsAdvanced);
		parameters.add(bHelp);
		parameters.add(bKeys);

		//not usable for ofxGui besides ImGui
		//paramsInternal.add(bGuiClicker);
		//paramsInternal.add(numColumnsPerRow);
		//paramsInternal.add(bGuiFloating);
		//paramsInternal.add(bGuiExpand);
		//paramsUI.add(bGuiClicker);
		//params.add(paramsUI);
		//paramsBrowse.add(indexName);
		//paramsManager.add(vRename);

		setupParametersExtra();
	}

	virtual void setupGui() {
		ofLogNotice("SurfingPresetsLite") << "setupGui() empty";
	}

	void refreshIndexRanges() {
		ofLogNotice("SurfingPresetsLite") << "refreshIndexRanges()";

		bool bEmpty = (dir.size() == 0);
		if (!bEmpty) {
			index.setMin(0);
			index.setMax(dir.size() - 1);
		} else {
			index.setMin(-1);
			index.setMax(-1);
			index.set(-1);
		}

		// clamp index
		if (index < index.getMin())
			index.set(index.getMin());
		else if (index > index.getMax())
			index.set(index.getMax());
	}

	void startup() {
		ofLogNotice("SurfingPresetsLite") << "startup()";

		doRefreshKitFiles();

		index_PRE = -2;

		std::string s;
		if (pathGlobal == "")
			s = pathSettings;
		else
			s = pathGlobal + "\\" + pathSettings;

		// load settings
		ofxSurfing::loadGroup(parameters, s);

		//workflow
		//must set some init states to avoid exceptions on reset!
		ofxSurfing::doResetSetInit(paramsPreset);

		// fix if index is weird or not correlated to dir files!
		refreshIndexRanges();

		// flag to create help info
		bFlagBuildHelp = true;

		//TODO: fix > 9
		// define some standard key strokes to trig presets
		vector<char> pChars = {
			'0',
			'1',
			'2',
			'3',
			'4',
			'5',
			'6',
			'7',
			'8',
			'9',
			'q',
			'w',
			'e',
			'r',
			't',
			'y',
			'u',
			'i',
			'o',
			'p',
			'a',
			's',
			'd',
			'f',
			'g',
			'h',
			'j',
			'k',
			'l',
			'z',
			'x',
			'c',
			'v',
			'b',
			'n',
			'm'
		};
		numPresetsForPopulating = ofClamp(numPresetsForPopulating.get(), 0, numPresetsForPopulating.getMax());
		keyCommandsChars.clear();
		for (size_t i = 0; i < numPresetsForPopulating; i++) {
			keyCommandsChars.push_back(pChars[i]);
		}

		//--

		// Auto populate if there's no preset files found!

		if (getNumPresets() == 0) {
			ofLogNotice("SurfingPresetsLite") << "No file presets found. Populating a kit of random presets.";
			doPopulateRandomKit();
		}

		//--

		bDoneStartup = true;
	}

	void update(ofEventArgs & args) {
		if (!bDoneStartup) {
			startup();
		}

		if (bFlagBuildHelp) {
			bFlagBuildHelp = false;
			buildHelp();
		}
	}

protected:
	std::string sHelp = "";
	bool bFlagBuildHelp = false;

public:
	void buildHelp() {
		sHelp = "";
		sHelp += "PRESETS\nMANAGER\n";
		sHelp += "\n";
		if (bKeys) {
			sHelp += "KEYS\n";
			sHelp += "\n";
			sHelp += "g           UI\n";
			sHelp += "h           Help\n";
			sHelp += "\n";
			sHelp += "BACKSPACE   Random\n";
			sHelp += "+Ctrl       Reset\n";
			sHelp += "\n";
			sHelp += "RETURN      New preset\n";
			sHelp += "+Ctrl       Populate\n";
			sHelp += "            Random Kit\n";
			sHelp += "\n";
			sHelp += "LEFT/RIGHT  Browse\n";
#ifdef SURFING__OFX_PRESETS_LITE__USING__IM_GUI
			sHelp += "UP/DOWN     Row\n";
#endif
		} else {
			sHelp += "KEYS DISABLED\n";
		}
		sHelp += "\n";
		sHelp += "Preset\n";
		sHelp += ofToString(getPresetIndex()) + "/" + ofToString(getPresetIndexLast()) + "\n";
		sHelp += "\n";
		sHelp += "File:\n";
		if (index < dir.size()) sHelp += dir[index].path() + "\n";
		sHelp += "\n";
		if (pathGlobal != "") {
			sHelp += "Path Global:\n";
			sHelp += pathGlobal + "\n";
			sHelp += "\n";
		}
		sHelp += "Kit Name:\n";
		sHelp += kitName + "\n";
		sHelp += "\n";
		sHelp += "Files:\n";
		if (dir.size() > 0) sHelp += ofToString(getListFiles()) + "\n";
		if (bKeys) {
			sHelp += "F5          Scan\n";
		}
	}

	virtual void drawHelp() {
		if (!bHelp) return;

		ofxSurfing::ofDrawBitmapStringBox(sHelp, ofxSurfing::SURFING_LAYOUT_BOTTOM_RIGHT);
	}

	//--

private:
	void doSave(bool bRefreshFiles = true) {
		// Here, fileBaseName is/must be already named and ready to be used here!

		if (getNumFiles() == 0) fileBaseName = "00"; // prepare name for first preset

		// Note that saves the previously settled file name when picking the index!
		ofLogNotice("SurfingPresetsLite") << "doSave(" << bRefreshFiles << "): " << fileBaseName;

		// Save preset
		ofxSurfing::saveGroup(paramsPreset, getPresetPath());

		//--

		if (bRefreshFiles) {
			// scan files
			doRefreshKitFiles();
		}
	}

	void doLoad(int index_) {
		ofLogNotice("SurfingPresetsLite") << "doLoad(" << index_ << ")";

		if (index_ != index) {
			index_PRE = index_;
			index = index_;
		}

		if (index < dir.size() && index >= 0) {
			fileBaseName = dir[index].getBaseName();
			doLoad(fileBaseName);
		} else {
			ofLogError("SurfingPresetsLite") << "Preset Index out of files range!";
		}
	}

	void doLoad(std::string baseName = "") {
		if (baseName != "") {
			if (fileBaseName != baseName)
				fileBaseName = baseName;
		}
		ofLogNotice("SurfingPresetsLite") << "doLoad(): " << fileBaseName;

		// Load preset
		ofxSurfing::loadGroup(paramsPreset, getPresetPath());
	}

	//--

public:
	void setup(ofParameterGroup & group) {
		addGroup(group);
	}

private:
	//TODO: allow more than one group...
	// recursive or multiple addGroup calls?
	void addGroup(ofParameterGroup & group) {

		//TODO: make a pointer, bc maybe that `=` not work well..
		auto ptype = group.type();
		bool isGroup = ptype == typeid(ofParameterGroup).name();
		if (isGroup) {
			paramsPreset = group;
		}

		name = group.getName();

		setNameUI(name); //rename bGui param
		parameters.setName(ofToString("PRESETS ") + name);

		pathSettings = name + ofToString("_Presets.json");

		setup();
	}

public:
	std::string getName() const {
		return name;
	}

public:
	void setPathGlobal(const std::string & p) {
		pathGlobal = p;
		ofLogNotice("SurfingPresetsLite") << "setPathGlobal(): " << pathGlobal;
	}

public:
	// Sub path related to inside pathGlobal.
	// Must call setPathGlobal before this too.
	// Useful to handle multiple presets kits!
	void setKitName(const std::string & p) {
		kitName = p;
		ofLogNotice("SurfingPresetsLite") << "setKitName(): " << kitName;
	}

private:
	void setFileBaseName(const std::string & p) {
		fileBaseName = p;
		ofLogNotice("SurfingPresetsLite") << "setFileBaseName(): " << fileBaseName;
	}

public:
	void doLoadPrevious() {
		ofLogNotice("SurfingPresetsLite") << "doLoadPrevious()";
		if (index.getMax() == -1) return;
		int i = index;

		if (index > index.getMin())
			i--;
		else if (i == index.getMin())
			if (bCycled)
				i = index.getMax();
		//else
		//i = index.getMin();
		index.set(i);
	}

	void doLoadNext() {
		ofLogNotice("SurfingPresetsLite") << "doLoadNext()";
		if (index.getMax() == -1) return;
		int i = index;
		if (i < index.getMax())
			i++;
		else if (i == index.getMax())
			if (bCycled)
				i = index.getMin();
		//else
		//i = index.getMax();
		index.set(i);
	}

	virtual void doLoadNextRow() {
		ofLogNotice("SurfingPresetsLite") << "doLoadNextRow()";

		int i = index;
		if (bCycled)
			if (i > index.getMax()) i = index.getMin() + i;
		index = ofClamp(i, index.getMin(), index.getMax());
	}

	virtual void doLoadPreviousRow() {
		ofLogNotice("SurfingPresetsLite") << "doLoadPreviousRow()";

		int i = index;
		if (bCycled)
			if (i < index.getMin()) i = index.getMax() - i;
		index = ofClamp(i, index.getMin(), index.getMax());
	}

private:
	void Changed(ofAbstractParameter & e) {
		std::string name = e.getName();
		ofLogNotice("SurfingPresetsLite") << "Changed: " << name << ": " << e;

		bFlagBuildHelp = true;

		if (name == index.getName()) {
			doProcessIndexCallback();
		}

		else if (name == vLoad.getName()) {
			doLoad();
		}

		else if (name == vSave.getName()) {
			doSave(false);
		}

		else if (name == vDelete.getName()) {
			doDelete();
		}

		else if (name == vNew.getName()) {
			doNewPreset();
		}

		else if (name == vPrevious.getName()) {
			doLoadPrevious();
		}

		else if (name == vNext.getName()) {
			doLoadNext();
		}

		else if (name == vScanKit.getName()) {
			doRefreshKitFiles();
		}

		else if (name == vClearKit.getName()) {
			doClearKit(); //autocreates a new preset
		}

		else if (name == vPopulateKit.getName()) {
			doPopulateKit(numPresetsForPopulating);
		}

		else if (name == vPopulateRandomKit.getName()) {
			doPopulateRandomKit(numPresetsForPopulating);
		}

		else if (name == vReset.getName()) {
			doResetInit();
		}

		else if (name == vResetSetInit.getName()) {
			doResetSetInit();
		}

		else if (name == vRandom.getName()) {
			doRandomize();
		}
	}

	void doProcessIndexCallback() {
		if (dir.size() == 0) return;

		// Changed?
		if (index != index_PRE) {
			ofLogNotice("SurfingPresetsLite") << ofToString(index_PRE) << " > " << ofToString(index);
			bIsChangedIndex = true;

			// log the index/files changes from->to
			if ((index_PRE < fileBaseNames.size() && index < fileBaseNames.size()) && (index_PRE < dir.size() && index < dir.size())) {
				ofLogVerbose("SurfingPresetsLite: Names: ") << ofToString(fileBaseNames[index_PRE]) << " > "
															<< ofToString(fileBaseNames[index]);
				ofLogNotice("SurfingPresetsLite: Filenames: ") << ofToString(dir[index_PRE].getFileName()) << " > "
															   << ofToString(dir[index].getFileName());
			}

			//--

			// 1. Common load.
			// Also will save the previous index preset if AutoSave enabled.

			if (!bModKeyControl && !bModKeyAlt) // Ctrl nor Alt not pressed.
			{
				// 1.1 save if enabled AutoSave
				if (bAutoSave) {
					if (index_PRE < fileBaseNames.size()) {
						fileBaseName = fileBaseNames[index_PRE];
						doSave(false);
					} else {
						ofLogVerbose("SurfingPresetsLite") << "Preset Index out of files range!";
					}
				}

				//--

				// 1.2 load
				ofLogNotice("SurfingPresetsLite") << index.getName() + ": " << ofToString(index);

				if (index < fileBaseNames.size()) {
					fileBaseName = fileBaseNames[index];
					doLoad();
				} else {
					ofLogError("SurfingPresetsLite") << "File out of range";
				}

				index_PRE = index;
			}

			//--

			// 2. Save / Copy

			// Save to clicked preset index
			// Ctrl pressed. Alt not pressed.

			else if (bModKeyControl && !bModKeyAlt) {
				if (index < fileBaseNames.size()) {
					fileBaseName = fileBaseNames[index];
					doSave(false);
					ofLogNotice("SurfingPresetsLite") << "Preset Copy!";

					index_PRE = index;
				}
			}

			//--

			//TODO:

			// 3. Swap between current/previous

			// (from/to) pre/current index
			// Ctrl not pressed. Alt pressed.

			else if (!bModKeyControl && bModKeyAlt) {
				if (index < fileBaseNames.size() && index_PRE < fileBaseNames.size()) {

					// Rename target to source
					std::string nFrom = fileBaseNames[index_PRE];
					std::string nTo = fileBaseNames[index];
					ofFile f;

					if (pathGlobal != "") {
						nTo = pathGlobal + "\\" + kitName + "\\" + nTo + ".json";
						nFrom = pathGlobal + "\\" + kitName + "\\" + nFrom + ".json";
					} else {
						nTo = kitName + "\\" + nTo + ".json";
						nFrom = kitName + "\\" + nFrom + ".json";
					}

					bool bf = f.open(nTo);
					bool bt = f.renameTo(nFrom, true, true);

					// Save current to
					fileBaseName = nTo;
					doSave(false);

					if (bf && bt) {
						ofLogNotice("SurfingPresetsLite") << "Preset Swap!";
						ofLogNotice("SurfingPresetsLite") << nFrom << " <-> " << nTo;
					} else {
						ofLogError("SurfingPresetsLite") << "Wrong Swap!";
					}

					index_PRE = index;
				}
			}
		}
	}

	//--

public:
	ofParameterGroup parameters { "PRESETS MANAGER" };

protected:
	ofParameterGroup paramsPreset { "Preset" };
	ofParameterGroup paramsBrowse { "Browse" };
	ofParameterGroup paramsManager { "Manager" };
	ofParameterGroup paramsKit { "Kit" };
	ofParameterGroup paramsAdvanced { "Advanced" };
	ofParameterGroup paramsInternal { "Internal" };
	//ofParameterGroup paramsUI { "UI" };

protected:
	std::string name = ""; // only for instance naming.

private:
	std::string kitName = "Kit"; // path to put the preset files. bin/data/Kit/
	std::string pathGlobal = ""; // Optional main folder where nested folder goes inside. bin/data/myApp/Kit/

	// Files
protected:
	ofDirectory dir;

	std::string filePath;
	std::string fileBaseName = "NONE";
	vector<std::string> fileBaseNames;

	//--

public:
	std::string getPresetPath() const {
		//fileBaseName must/should be settled before being here!
		//fileBaseName will be updated when moving the index around the Kit presets...
		std::string path;
		if (pathGlobal != "")
			path = ofFilePath::join(ofFilePath::join(pathGlobal, kitName), fileBaseName + ".json");
		else
			path = ofFilePath::join(kitName,fileBaseName + ".json");
		return path;
	}
	std::string getKitPath() const {
		std::string path;
		if (pathGlobal != "")
			path = ofFilePath::join(pathGlobal,kitName);
		else
			path = kitName;
		return path;
	}
	int getNumPresets() const {
		return index.getMax() + 1;
	}
	int getPresetIndexLast() const {
		return index.getMax();
	}
	int getPresetIndex() const {
		return index.get();
	}

protected:
	int getNumFiles() const {
		return fileBaseNames.size();
	}

	bool doReorganizeKitFiles() {
		ofLogNotice("SurfingPresetsLite") << "doReorganizeKitFiles()";

		bool bSomeFileRequiredRename = false;

		ofLogNotice("SurfingPresetsLite") << "doReorganizeKitFiles() Found " << dir.size() << " files.";

		for (int i = 0; i < dir.size(); i++) {
			std::string n = dir[i].getBaseName(); // current name
			std::string n_ = std::string((i < 10) ? "0" : "") + ofToString(i); // expected name
			if (n_ != n) {
				std::string pf; //from
				std::string pt; //to
				if (pathGlobal != "") {
					pf = ofFilePath::join(ofFilePath::join(pathGlobal,kitName),(n + ".json"));
					pt = ofFilePath::join(ofFilePath::join(pathGlobal,kitName),(n_ + ".json"));
				} else {
					pf = ofFilePath::join(kitName, (n + ".json"));
					pt = ofFilePath::join(kitName, (n_ + ".json"));
				}

				ofLogNotice("SurfingPresetsLite") << "Rename file #" << i;
				ofLogNotice("SurfingPresetsLite") << pf << " > " << pt;
				ofFile file(pf);
				if (file.exists()) {
					file.renameTo(pt);

					bSomeFileRequiredRename = true;
				}
			}
		}

		bFlagBuildHelp = true;

		return bSomeFileRequiredRename;
	}

	bool doRefreshKitFiles() {
		// Must call for reading folder, when deleting or creating a new preset.
		ofLogNotice("SurfingPresetsLite") << "doRefreshKitFiles(): " << kitName;

		dir.listDir(getKitPath());
		dir.sort();

		bool bEmpty = (dir.size() == 0);
		if (bEmpty) {
			ofLogWarning("SurfingPresetsLite") << "doRefreshKitFiles(): " << getKitPath() << " directory is empty.";
		} else {
			doReorganizeKitFiles();
		}

		// Log files on folder
		fileBaseNames.clear();
		for (int i = 0; i < dir.size(); i++) {
			ofLogNotice("SurfingPresetsLite") << "file #" << ofToString(i) << ": " << dir.getName(i);

			fileBaseNames.push_back(dir[i].getBaseName());
		}

		refreshIndexRanges();

		// returns true if there's some file/s!
		return (!bEmpty);
	}

protected:
	void doListFiles() {
		for (auto f : dir) {
			ofLogNotice("SurfingPresetsLite") << "doListFiles(): " << f.getAbsolutePath();
		}
	}
	std::string getListFiles() const {
		ofLogNotice("SurfingPresetsLite") << "getListFiles()";

		std::string l;
		size_t i = 0;
		for (auto f : dir) {
			l += f.path();
			if (i == index) l += " *";
			l += "\n";
			i++;
		}
		return l;
	}

	void doDelete(bool bSetIndexToLast = false) {
		if (dir.size() == 0) return;

		ofLogNotice("SurfingPresetsLite") << "doDelete() Index: " << index;

		fileBaseName = fileBaseNames[index];
		//fileBaseName = dir[index].getBaseName();

		ofLogNotice("SurfingPresetsLite") << "doDelete() Filename: " << fileBaseName;

		std::string p = getPresetPath();
		ofLogNotice("SurfingPresetsLite") << "doDelete() Look for: " << p;
		bool b = ofFile::removeFile(p);

		if (b) {
			ofLogNotice("SurfingPresetsLite") << "Successfully deleted: " << p;
			doRefreshKitFiles();
#if 1
			//workflow
			if (bSetIndexToLast) // load last
				index.set(index.getMax());
			else // load current
				doLoad(index); // refresh current selected
#endif
			ofLogNotice("SurfingPresetsLite") << "Done delete";
		} else {
			ofLogError("SurfingPresetsLite") << "Unable to delete: " << p;
		}

		// fix correct list
		doRefreshKitFiles();
	}

	void doNewPreset() {
		ofLogNotice("SurfingPresetsLite") << "doNewPreset()";

		std::string n;
		if (dir.size() == 0) {
			n = "00"; // at first
		} else {
			// at last
			int sz = dir.size();
			n = std::string((sz < 10) ? "0" : "") + ofToString(sz);
		}
		setFileBaseName(n);

		doSave(true);

		//workflow: select last preset
		index.set(dir.size() - 1);
	}

	void doClearKit(bool createOne = true) {
		ofLogNotice("SurfingPresetsLite") << "doClearKit(" << createOne << ")";

		// Remove all files
		for (int i = 0; i < dir.size(); i++) {
			ofFile file = dir[i];
			file.remove();
		}

		doRefreshKitFiles();

		//workflow: create a new preset
		if (createOne) {
			doNewPreset();
		}
	}

	void doPopulateRandomKit() {
		ofLogNotice("SurfingPresetsLite") << "doPopulateRandomKit() " << numPresetsForPopulating;

		doPopulateRandomKit(numPresetsForPopulating);
	}

	void doPopulateRandomKit(int amount) { // pass -1 to overwrite/use the same amount of presets.
		ofLogNotice("SurfingPresetsLite") << "doPopulateRandomKit() " << amount;

		doPopulateKit(amount, true);
	}

	void doPopulateKit(int amount, bool bRandom = false) { // pass -1 to overwrite/use the same amount of presets.
		ofLogNotice("SurfingPresetsLite") << "doPopulateKit() " << amount;

		doClearKit(false);

		// will create the same amount files on folder
		if (amount == -1) {
			amount = dir.size();
		}

		bool b = false;
		for (int i = 0; i < amount; i++) {
			doNewPreset();
			if (bRandom) doRandomize();
			doSave(true);
		}

		if (b) index = 0; //workflow
	}

	//----

	//--------------------------------------------------------------
	void keyPressed(ofKeyEventArgs & eventArgs) {
		if (!bKeys) return;

		const int & key = eventArgs.key;
		ofLogVerbose("SurfingPresetsLite") << "keyPressed: " << (char)key;

		// Modifiers
		//bool bModKeyCommand_ = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool bModKeyControl_ = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool bModKeyAlt_ = eventArgs.hasModifier(OF_KEY_ALT);
		//bool bModKeyShift_ = eventArgs.hasModifier(OF_KEY_SHIFT);

		if (bModKeyControl_) bModKeyControl = true;
		if (bModKeyAlt_) bModKeyAlt = true;

		if (key == OF_KEY_DEL) {
			doDelete();
			return;
		}

		if (key == OF_KEY_LEFT) {
			doLoadPrevious();
			return;
		}

		if (key == OF_KEY_RIGHT) {
			doLoadNext();
			return;
		}

		if (key == OF_KEY_F5) {
			doRefreshKitFiles();
			doRefreshKitFiles(); //TODO: doubled to fix
			return;
		}

#ifdef SURFING__OFX_PRESETS_LITE__USING__IM_GUI
		if (key == OF_KEY_UP) {
			doLoadPreviousRow();
			return;
		}

		if (key == OF_KEY_DOWN) {
			doLoadNextRow();
			return;
		}
#endif

		if (key == OF_KEY_RETURN && !bModKeyControl) {
			doNewPreset();
			return;
		}

		if (key == OF_KEY_RETURN && bModKeyControl) {
			doPopulateRandomKit();
			return;
		}

		if (key == OF_KEY_BACKSPACE && bModKeyControl) {
			doResetInit();
			//doReset();
			return;
		}

		if (key == OF_KEY_BACKSPACE && !bModKeyControl) {
			doRandomize();
			return;
		}

		if (key == 'h') {
			bHelp = !bHelp;
			return;
		}

		if (key == 'g') {
			setToggleGuiVisible();
			return;
		}

		if (key == 'G') {
			bGuiClicker = !bGuiClicker;
			return;
		}

		else {
			//TODO: not working
			for (size_t i = 0; i < numPresetsForPopulating; i++) {
				if ((char)key == keyCommandsChars[i]) {
					//if (key == int(keyCommandsChars[i])) {
					index.set(i);
					return;
				}
			}
		}
	}

	//--------------------------------------------------------------
	void keyReleased(ofKeyEventArgs & eventArgs) {
		if (!bKeys) return;

		const int & key = eventArgs.key;
		ofLogVerbose("SurfingPresetsLite") << "keyReleased: " << (char)key /*<< " [" << key << "]"*/;

		// Modifiers
		//bool bModKeyCommand_ = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool bModControl_ = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool bModAlt_ = eventArgs.hasModifier(OF_KEY_ALT);
		//bool bModKeyShift_ = eventArgs.hasModifier(OF_KEY_SHIFT);

		if (!bModControl_) bModKeyControl = false;
		if (!bModAlt_) bModKeyAlt = false;
	}

	//--------------------------------------------------------------
	void addKeysListeners() {
		ofAddListener(ofEvents().keyPressed, this, &SurfingPresetsLite::keyPressed);
		ofAddListener(ofEvents().keyReleased, this, &SurfingPresetsLite::keyReleased);
	}

	//--------------------------------------------------------------
	void removeKeysListeners() {
		ofRemoveListener(ofEvents().keyPressed, this, &SurfingPresetsLite::keyPressed);
		ofRemoveListener(ofEvents().keyReleased, this, &SurfingPresetsLite::keyReleased);
	}

	// Easy callback to know when the preset index/selector changed from the parent class
private:
	bool bIsChangedIndex = false;

public:
	bool isChangedIndex() {
		if (bIsChangedIndex) {
			bIsChangedIndex = false;
			return true;
		} else
			return false;
	}

	//----

	//--------------------------------------------------------------
	void doResetSetInit() {//set current state as init/default
		ofLogNotice("SurfingPresetsLite") << "doResetSetInit()";

		ofxSurfing::doResetSetInit(paramsPreset);
	}

	//--------------------------------------------------------------
	void doResetInit() {//resetore init state
		ofLogNotice("SurfingPresetsLite") << "doResetInit()";

		ofxSurfing::doResetInit(paramsPreset);
	}

	//--------------------------------------------------------------
	void doReset() {//resets to each params mins
		ofLogNotice("SurfingPresetsLite") << "doReset()";

		ofxSurfing::doReset(paramsPreset);
	}

	//--------------------------------------------------------------
	void doRandomize() {//randomize each params
		ofLogNotice("SurfingPresetsLite") << "doRandomize()";

		ofxSurfing::doRandomize(paramsPreset);
	}
};
