
#pragma once
#include "ofMain.h"

#include "ofxSurfingHelpersLite.h"

//--

/*
	TODO: 
        add auto saver.
	  avoid exit save.
	add multiple ofParameterGroup/s
	in the matrix 
		fix colors on flipping 
		fix direction
	add rename/custom names presets ?
*/

//----

class SurfingPresetsLite {
public:
	SurfingPresetsLite() {
		ofLogNotice("SurfingPresetsLite") << "Constructor()";
		addListeners();

		dir.allowExt("JSON");
		dir.allowExt("json");
	};

	~SurfingPresetsLite() {
		ofLogNotice("SurfingPresetsLite") << "Destructor()";

		removeListeners();

		destructUI();

		if (!bDoneExit) {
			ofLogWarning("SurfingPresetsLite") << "Call exit() forced!";
			exit();
		}
	};

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
		string s;
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
	void setNameUI(string s) {
		bGui.set(ofToString("PRESETS ") + s, true);
	}

private:
	string pathSettings = "Presets.json";

	bool bDoneStartup = false;
	bool bDoneExit = false;

	//----

protected:
	bool bMod_CONTROL = false;
	bool bMod_ALT = false;

	//----

public:
	ofParameter<void> vReset { "Reset" };
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

	//ofParameter<string> indexName { "Name", "NONE" };//TODO
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

		parameters.add(bGuiParams);
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

		string s;
		if (pathGlobal == "")
			s = pathSettings;
		else
			s = pathGlobal + "\\" + pathSettings;

		// load settings
		ofxSurfing::loadGroup(parameters, s);

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
	string sHelp = "";
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

	void doLoad(string baseName = "") {
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
	void setPathGlobal(const string & p) {
		pathGlobal = p;
		ofLogNotice("SurfingPresetsLite") << "setPathGlobal(): " << pathGlobal;
	}

public:
	// related to inside pathGlobal. Must call setPathGlobal before this too.
	void setKitName(const string & p) {
		kitName = p;
		ofLogNotice("SurfingPresetsLite") << "setKitName(): " << kitName;
	}

private:
	void setFileBaseName(const string & p) {
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
		string name = e.getName();
		ofLogNotice("SurfingPresetsLite") << "Changed: " << name << ": " << e;

		bFlagBuildHelp = true;

		if (name == index.getName()) {
			doProcessIndexCallback();
		}

		else if (name == vLoad.getName()) {
			doLoad();
		}

		else if (name == vReset.getName()) {
			doResetParams();
		}

		else if (name == vRandom.getName()) {
			doRandomizeParams();
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

		//else if (name == vRename.getName()) {
		//	doRename();
		//}
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

			if (!bMod_CONTROL && !bMod_ALT) // Ctrl nor Alt not pressed.
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

			else if (bMod_CONTROL && !bMod_ALT) {
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

			else if (!bMod_CONTROL && bMod_ALT) {
				if (index < fileBaseNames.size() && index_PRE < fileBaseNames.size()) {

					// Rename target to source
					string nFrom = fileBaseNames[index_PRE];
					string nTo = fileBaseNames[index];
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
	string name = ""; // only for instance naming.

private:
	string kitName = "Kit"; // path to put the preset files. bin/data/Kit/
	string pathGlobal = ""; // Optional main folder where nested folder goes inside. bin/data/myApp/Kit/

	// Files
protected:
	ofDirectory dir;

	string filePath;
	string fileBaseName = "NONE";
	vector<string> fileBaseNames;

	//--

public:
	string getPresetPath() const {
		//fileBaseName must/should be settled before being here!
		//fileBaseName will be updated when moving the index around the Kit presets...
		string path;
		if (pathGlobal != "")
			path = pathGlobal + "\\" + kitName + "\\" + fileBaseName + ".json";
		else
			path = kitName + "\\" + fileBaseName + ".json";
		return path;
	}
	string getKitPath() const {
		string path;
		if (pathGlobal != "")
			path = pathGlobal + "\\" + kitName;
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
			string n = dir[i].getBaseName(); // current name
			string n_ = string((i < 10) ? "0" : "") + ofToString(i); // expected name
			if (n_ != n) {
				string pf; //from
				string pt; //to
				if (pathGlobal != "") {
					pf = pathGlobal + "\\" + kitName + "\\" + n + ".json";
					pt = pathGlobal + "\\" + kitName + "\\" + n_ + ".json";
				} else {
					pf = kitName + "\\" + n + ".json";
					pt = kitName + "\\" + n_ + ".json";
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
	string getListFiles() const {
		ofLogNotice("SurfingPresetsLite") << "getListFiles()";

		string l;
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

		string p = getPresetPath();
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

		string n;
		if (dir.size() == 0) {
			n = "00"; // at first
		} else {
			// at last
			int sz = dir.size();
			n = string((sz < 10) ? "0" : "") + ofToString(sz);
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
			if (bRandom) doRandomizeParams();
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
		//bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
		//bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

		if (mod_CONTROL) bMod_CONTROL = true;
		if (mod_ALT) bMod_ALT = true;

		if (key == OF_KEY_DEL)
			doDelete();

		else if (key == OF_KEY_LEFT)
			doLoadPrevious();
		else if (key == OF_KEY_RIGHT)
			doLoadNext();

		else if (key == OF_KEY_F5) {
			doRefreshKitFiles();
			doRefreshKitFiles(); //TODO: fix
		}

#ifdef SURFING__OFX_PRESETS_LITE__USING__IM_GUI
		else if (key == OF_KEY_UP)
			doLoadPreviousRow();
		else if (key == OF_KEY_DOWN)
			doLoadNextRow();
#endif

		else if (key == OF_KEY_BACKSPACE && bMod_CONTROL)
			doResetParams();
		else if (key == OF_KEY_BACKSPACE && !bMod_CONTROL)
			doRandomizeParams();

		else if (key == OF_KEY_RETURN && !bMod_CONTROL)
			doNewPreset();

		else if (key == OF_KEY_RETURN && bMod_CONTROL)
			doPopulateRandomKit();

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

		if (key == 'h') bHelp = !bHelp;

		if (key == 'g')
			setToggleGuiVisible();

		else if (key == 'G')
			bGuiClicker = !bGuiClicker;
	}

	//--------------------------------------------------------------
	void keyReleased(ofKeyEventArgs & eventArgs) {
		if (!bKeys) return;

		const int & key = eventArgs.key;
		ofLogVerbose("SurfingPresetsLite") << "keyReleased: " << (char)key /*<< " [" << key << "]"*/;

		// Modifiers
		//bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
		//bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

		if (!mod_CONTROL) bMod_CONTROL = false;
		if (!mod_ALT) bMod_ALT = false;
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

	//--

	// Resets Engine

private:
	// Random simple
	//--------------------------------------------------------------
	void doRandomizeParams(bool bSilent = false) {
		ofLogNotice("SurfingPresetsLite") << "doRandomizeParams";

		//TODO:
		// this is not recursive inside the nested groups content!
		// get solution from ImHelpers from addGroup to iterate groups.

		for (int i = 0; i < paramsPreset.size(); i++) {
			auto & p = paramsPreset[i];

			if (p.type() == typeid(ofParameter<float>).name()) {
				ofParameter<float> pr = p.cast<float>();
				if (!pr.isSerializable()) continue;
				float v = ofRandom(pr.getMin(), pr.getMax());
				if (bSilent)
					pr.setWithoutEventNotifications(v);
				else
					pr.set(v);
				ofLogNotice("SurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			else if (p.type() == typeid(ofParameter<int>).name()) {
				ofParameter<int> pr = p.cast<int>();
				if (!pr.isSerializable()) continue;
				int v = ofRandom(pr.getMin(), pr.getMax());
				if (bSilent)
					pr.setWithoutEventNotifications(v);
				else
					pr.set(v);
				ofLogNotice("SurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			// include booleans
			else if (p.type() == typeid(ofParameter<bool>).name()) {
				ofParameter<bool> pr = p.cast<bool>();
				if (!pr.isSerializable()) continue;
				bool b = (ofRandom(1.f) >= 0.5f);
				if (bSilent)
					pr.setWithoutEventNotifications(b);
				else
					pr.set(b);
				ofLogNotice("SurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			// colors
			else if (p.type() == typeid(ofParameter<ofColor>).name()) {
				ofParameter<ofColor> pr = p.cast<ofColor>();
				if (!pr.isSerializable()) continue;
				ofColor c;
				c.setBrightness(255);
				c.setSaturation(255);
				c.setHue(ofRandom(255));

				if (bSilent)
					pr.setWithoutEventNotifications(c);
				else
					pr.set(c);
				ofLogNotice("SurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}
		}
	}

	// Reset Simple
	//--------------------------------------------------------------
	void doResetParams(bool bSilent = false) {
		ofLogNotice("SurfingPresetsLite") << "doResetParams";

		for (int i = 0; i < paramsPreset.size(); i++) {
			auto & p = paramsPreset[i];

			if (p.type() == typeid(ofParameter<float>).name()) {
				ofParameter<float> pr = p.cast<float>();
				if (!pr.isSerializable()) continue;
				if (bSilent)
					pr.setWithoutEventNotifications(pr.getMin());
				else
					pr.set(pr.getMin());
				ofLogNotice("SurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			else if (p.type() == typeid(ofParameter<int>).name()) {
				ofParameter<int> pr = p.cast<int>();
				if (!pr.isSerializable()) continue;
				if (bSilent)
					pr.setWithoutEventNotifications(pr.getMin());
				else
					pr.set(pr.getMin());
				ofLogNotice("SurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			// include booleans
			else if (p.type() == typeid(ofParameter<bool>).name()) {
				ofParameter<bool> pr = p.cast<bool>();
				if (!pr.isSerializable()) continue;
				bool b = false;
				if (bSilent)
					pr.setWithoutEventNotifications(b);
				else
					pr.set(b);
				ofLogNotice("SurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

#if 0 // Disable to avoid both color same.
	// colors
			else if (p.type() == typeid(ofParameter<ofColor>).name()) {
				ofParameter<ofColor> pr = p.cast<ofColor>();
				if (!pr.isSerializable()) continue;
				ofColor c;
				c.setBrightness(64);
				c.setSaturation(0);
				c.setHue(0);

				if (bSilent)
					pr.setWithoutEventNotifications(c);
				else
					pr.set(c);
				ofLogNotice("SurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}
#endif
		}

		//if (!bSilent) bIsRetrigged = true;
	}

	//void doRename() {
	//	ofLogNotice("SurfingPresetsLite") << "doRename()";
	//}

	//--

	//TODO:
	// An improved randomizer. center value + power %
	/*
    enum ResetPramsType
    {
        RESET_PARAM_MIN = 0,
        RESET_PARAM_FULL_CENTER,
        RESET_PARAM_RANGE_CENTER,
        RESET_PARAM_MAX,
        RESET_RANGE_MIN,
        RESET_RANGE_MAX,
    };

    //--------------------------------------------------------------
    void doResetParamsFull(ResetPramsType MS_type) {
        ofLogNotice("SurfingPresetsLite") << (__FUNCTION__);


        for (int i = 0; i < paramsPreset.size(); i++)
            //for (auto p : editorEnablers)
        {
            auto p = true;//only reset this iterated param if it's enabled

            //-

            //string name = p.getName();//name
            string name = paramsPreset[i].getName();//name
            //auto& g = paramsPreset.getGroup(name);//ofParameterGroup
            //auto& g = paramsPreset.getGroup(name);//ofParameterGroup
            auto& g = paramsPreset;//ofParameterGroup
            auto& e = g.get(name);//ofAbstractParameter
            //auto& e = paramsPreset.get(name);//ofAbstractParameter

            auto type = e.type();
            bool isFloat = type == typeid(ofParameter<float>).name();
            bool isInt = type == typeid(ofParameter<int>).name();

            bool isVec2 = type == typeid(ofParameter<glm::vec2>).name();
            bool isVec3 = type == typeid(ofParameter<glm::vec3>).name();
            bool isVec4 = type == typeid(ofParameter<glm::vec4>).name();

            if (isFloat)
            {
                auto pmin = g.getFloat("Min").get();
                auto pmax = g.getFloat("Max").get();
                ofParameter<float> p0 = e.cast<float>();

                if (0) {}
                else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
                else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
                else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
                else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
                else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
                else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
            }

            else if (isInt)
            {
                auto pmin = g.getInt("Min").get();
                auto pmax = g.getInt("Max").get();
                ofParameter<int> p0 = e.cast<int>();

                if (0) {}
                else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
                else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
                else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
                else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
                else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
                else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max
            }

            else if (isVec2)
            {
                auto pmin = g.getVec2f("Min").get();
                auto pmax = g.getVec2f("Max").get();
                ofParameter<glm::vec2> p0 = e.cast<glm::vec2>();

                if (0) {}
                else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
                else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
                else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
                else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
                else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
                else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

                //for (int dim = 0; dim < 2; dim++) {
                //	if (dim == 0) {
                //		if (0) {}
                //		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec2(p0.getMin().x, p0.get().y));//reset to param min
                //		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec2(p0.getMax().x - p0.getMin().x / 2, p0.get().y));
                //		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec2(pmax.x - pmin.x / 2, p0.get().y));
                //		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec2(p0.getMax().x, p0.get().y));//reset to param max
                //		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec2(pmin.x, p0.get().y));//reset to range min
                //		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec2(pmax.x, p0.get().y));//reset to range max
                //	}
                //	else if (dim == 1) {
                //		if (0) {}
                //		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec2(p0.get().x, p0.getMin().y));//reset to param min
                //		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec2(p0.get().x,p0.getMax().y - p0.getMin().y / 2));
                //		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec2(p0.get().x,pmax.y - pmin.y / 2));
                //		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec2(p0.getMax().x, p0.get().y));//reset to param max
                //		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec2(p0.get().x, pmin.y));//reset to range min
                //		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec2(p0.get().x, pmax.y));//reset to range max
                //	}
                //}
            }
            else if (isVec3)
            {
                auto pmin = g.getVec3f("Min").get();
                auto pmax = g.getVec3f("Max").get();
                ofParameter<glm::vec3> p0 = e.cast<glm::vec3>();

                if (0) {}
                else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
                else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
                else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
                else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
                else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
                else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

                //for (int dim = 0; dim < 3; dim++) {
                //	if (dim == 0) {
                //		if (0) {}
                //		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.getMin().x, p0.get().y, p0.get().z));//reset to param min
                //		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.getMax().x - p0.getMin().x / 2, p0.get().y, p0.get().z));
                //		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(pmax.x - pmin.x / 2, p0.get().y, p0.get().z));
                //		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.get().z));//reset to param max
                //		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(pmin.x, p0.get().y, p0.get().z));//reset to range min
                //		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(pmax.x, p0.get().y, p0.get().z));//reset to range max
                //	}
                //	else if (dim == 1) {
                //		if (0) {}
                //		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.get().x, p0.getMin().y, p0.getMin().z));//reset to param min
                //		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.get().x, p0.getMax().y - p0.getMin().y / 2, p0.getMin().z));
                //		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(p0.get().x, pmax.y - pmin.y / 2, p0.getMin().z));
                //		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.getMin().z));//reset to param max
                //		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(p0.get().x, pmin.y, p0.getMin().z));//reset to range min
                //		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(p0.get().x, pmax.y, p0.getMin().z));//reset to range max
                //	}
                //	else if (dim == 2) {
                //		if (0) {}
                //		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec3(p0.get().x, p0.get().y, p0.getMin().z));//reset to param min
                //		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec3(p0.get().x, p0.get().y, p0.getMax().z - p0.getMin().z / 2));
                //		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec3(p0.get().x, p0.get().y, pmax.z - pmin.z / 2));
                //		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec3(p0.getMax().x, p0.get().y, p0.get().z));//reset to param max
                //		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec3(p0.get().x, p0.get().y, pmin.z));//reset to range min
                //		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec3(p0.get().x, p0.get().y, pmax.z));//reset to range max
                //	}
                //}
            }
            else if (isVec4)
            {
                auto pmin = g.getVec4f("Min").get();
                auto pmax = g.getVec4f("Max").get();
                ofParameter<glm::vec4> p0 = e.cast<glm::vec4>();

                if (0) {}
                else if (MS_type == RESET_PARAM_MIN) p0.set(p0.getMin());//reset to param min
                else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set((p0.getMax() - p0.getMin()) / 2);
                else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set((pmax - pmin) / 2);
                else if (MS_type == RESET_PARAM_MAX) p0.set(p0.getMax());//reset to param max
                else if (MS_type == RESET_RANGE_MIN) p0.set(pmin);//reset to range min
                else if (MS_type == RESET_RANGE_MAX) p0.set(pmax);//reset to range max

                //for (int dim = 0; dim < 4; dim++) {
                //	if (dim == 0) {
                //		if (0) {}
                //		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.getMin().x, p0.get().y, p0.get().z, p0.get().w));//reset to param min
                //		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4((p0.getMax().x - p0.getMin().x) / 2, p0.get().y, p0.get().z, p0.get().w));
                //		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4((pmax.x - pmin.x) / 2, p0.get().y, p0.get().z, p0.get().w));
                //		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.getMax().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
                //		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(pmin.x, p0.get().y, p0.get().z, p0.get().w));//reset to range min
                //		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(pmax.x, p0.get().y, p0.get().z, p0.get().w));//reset to range max
                //	}
                //	else if (dim == 1) {
                //		if (0) {}
                //		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.getMin().y, p0.get().z, p0.get().w));//reset to param min
                //		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, (p0.getMax().y - p0.getMin().y) / 2, p0.get().z, p0.get().w));
                //		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, (pmax.y - pmin.y) / 2, p0.get().z, p0.get().w));
                //		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.getMax().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
                //		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, pmin.y, p0.get().z, p0.get().w));//reset to range min
                //		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, pmax.y, p0.get().z, p0.get().w));//reset to range max
                //	}
                //	else if (dim == 2) {
                //		if (0) {}
                //		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.getMin().z, p0.get().w));//reset to param min
                //		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, (p0.getMax().z - p0.getMin().z) / 2, p0.get().w));
                //		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, (pmax.z - pmin.z) / 2, p0.get().w));
                //		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.get().w));//reset to param max
                //		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, pmin.z, p0.get().w));//reset to range min
                //		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, pmax.z, p0.get().w));//reset to range max
                //	}
                //	else if (dim == 3) {
                //		if (0) {}
                //		else if (MS_type == RESET_PARAM_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.getMin()));//reset to param min
                //		else if (MS_type == RESET_PARAM_FULL_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, (p0.getMax().w - p0.getMin().w) / 2));
                //		else if (MS_type == RESET_PARAM_RANGE_CENTER) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, (pmax.w - pmin.w) / 2));
                //		else if (MS_type == RESET_PARAM_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, p0.getMax().w));//reset to param max
                //		else if (MS_type == RESET_RANGE_MIN) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, pmin));//reset to range min
                //		else if (MS_type == RESET_RANGE_MAX) p0.set(glm::vec4(p0.get().x, p0.get().y, p0.get().z, pmax));//reset to range max
                //	}
                //}
            }
        }
    }
    */
};
