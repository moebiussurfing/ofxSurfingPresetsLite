
#pragma once
#include "ofMain.h"

#include "ofxSurfingHelpersLite.h"

//--

/*
	TODO:

	add bGui for params window.
	improve first name.
		new auto set name to be faster
	add rename preset
*/

//----

class ofxSurfingPresetsLite {
public:
	ofxSurfingPresetsLite() {
		ofLogNotice("ofxSurfingPresetsLite") << "Constructor";

		ofAddListener(parameters.parameterChangedE(), this, &ofxSurfingPresetsLite::Changed);
		ofAddListener(ofEvents().update, this, &ofxSurfingPresetsLite::update);
		ofAddListener(ofEvents().exit, this, &ofxSurfingPresetsLite::exit);

		ofSetLogLevel("ofxSurfingPresetsLite", OF_LOG_NOTICE);

		addKeysListeners();
	}

	~ofxSurfingPresetsLite() {
		ofLogNotice("ofxSurfingPresetsLite") << "Destructor";

		ofRemoveListener(parameters.parameterChangedE(), this, &ofxSurfingPresetsLite::Changed);
		ofRemoveListener(ofEvents().update, this, &ofxSurfingPresetsLite::update);
		ofRemoveListener(ofEvents().exit, this, &ofxSurfingPresetsLite::exit);

		removeKeysListeners();
	}

private:
	void exit(ofEventArgs & args) {
		exit();
	}

	void exit() {
		string s;
		if (pathGlobal == "")
			s = pathSettings;
		else
			s = pathGlobal + "/" + pathSettings;

		ofxSurfing::saveGroup(parameters, s);

		doSave();
	}

private:
	//public:
	// 	   //TODO: fix
	// Customize name to avoid window names colliding
	// with other preset manager instances
	//--------------------------------------------------------------
	void setNameUI(string s) {
		bGui.set(ofToString("PRESETS ") + s, true);
	}

private:
	string pathSettings = "Presets_Settings.json";
	bool bDoneStartup = false;

	//----

protected:
	bool bMod_CONTROL = false;
	bool bMod_ALT = false;

	//----

public:
	ofParameter<void> vReset { "Reset" };
	ofParameter<void> vRandom { "Random" };

	ofParameter<int> index { "Index", -1, -1, -1 };
	ofParameter<string> indexName { "Name", "NONE" };

	ofParameter<bool> bCycled { "Cycled", true };
	ofParameter<int> numPresets { "Num presets", 9, 1, 32 };

private:
	int index_PRE = -1; // pre

public:
	ofParameter<bool> bGui { "PRESETS", true };
	ofParameter<bool> bGuiParams { "UI PARAMETERS", true };
	ofParameter<bool> bGuiClicker { "UI CLICKER", false };
	//ofParameter<bool> bGuiClickerMini { "Mini Clicker", false };
	ofParameter<bool> bGuiFloating { "Floating", true };
	ofParameter<bool> bGuiExpand { "Expand", false };
	ofParameter<int> guiNumColumns { "Num columns", 3, 1, 5 };

	void setToggleGuiVisible() { bGui = !bGui; }
	void setGuiVisible(bool b = true) { bGui = b; }
	bool getGuiVisible() { return bGui; }

	//--

public:
	ofParameter<void> vPrevious { "<" };
	ofParameter<void> vNext { ">" };

protected:
	//ofParameter<void> vRename { "Rename" }; //TODO

	ofParameter<void> vNew { "New" };
	ofParameter<void> vDelete { "Delete" };
	ofParameter<void> vSave { "Save" };
	ofParameter<void> vLoad { "Load" };

	ofParameter<void> vScanKit { "Scan" };
	ofParameter<void> vClearKit { "Clear" };
	ofParameter<void> vPopulateKit { "Populate" };
	ofParameter<void> vPopulateRandomKit { "Populate Random" };

	ofParameter<bool> bAutoSave { "AutoSave", true }; // edit mode
	ofParameter<bool> bKeys { "Keys", true };
	ofParameter<bool> bHelp { "Help", true };

	vector<char> keyCommandsChars;

private:
	void setup() {
		ofLogNotice("ofxSurfingPresetsLite") << "setup()";

		doRefreshKitFiles();

		setupParameters();
		setupGui();
	}

	virtual void refreshGui() {
		ofLogNotice("ofxSurfingPresetsLite") << "refreshGui()";
	}

	void setupParameters() {
		ofLogNotice("ofxSurfingPresetsLite") << "setupParameters()";

		indexName.setSerializable(false);
		paramsKit.setSerializable(false);

		//paramsUI.add(bGuiClicker);
		//params.add(paramsUI);
		//paramsBrowse.add(indexName);
		//paramsManager.add(vRename);

		paramsBrowse.add(index);
		paramsBrowse.add(vNext);
		paramsBrowse.add(vPrevious);

		paramsManager.add(vSave);
		paramsManager.add(vLoad);
		paramsManager.add(vNew);
		paramsManager.add(vDelete);
		paramsManager.add(vRandom);
		paramsManager.add(vReset);
		paramsManager.add(bAutoSave);

		paramsKit.add(vScanKit);
		paramsKit.add(vClearKit);
		paramsKit.add(vPopulateKit);
		paramsKit.add(vPopulateRandomKit);

		paramsInternal.add(bGuiParams);
		paramsInternal.add(bGuiClicker);
		paramsInternal.add(bGuiFloating);
		paramsInternal.add(bGuiExpand);
		paramsInternal.add(guiNumColumns);
		paramsInternal.add(bCycled);
		paramsInternal.add(numPresets);
		paramsInternal.add(bGui);

		parameters.add(paramsBrowse);
		parameters.add(paramsManager);
		parameters.add(paramsKit);
		parameters.add(paramsInternal);
		parameters.add(bHelp);
		parameters.add(bKeys);
	}

	virtual void setupGui() {
		ofLogNotice("ofxSurfingPresetsLite") << "setupGui()";
	}

	void startup() {
		ofLogNotice("ofxSurfingPresetsLite") << "startup()";

		doRefreshKitFiles();

		index_PRE = -1; // pre

		string s;
		if (pathGlobal == "")
			s = pathSettings;
		else
			s = pathGlobal + "\\" + pathSettings;

		ofxSurfing::loadGroup(parameters, s);

		bFlagBuildHelp = true;

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
		numPresets = ofClamp(numPresets.get(), 0, numPresets.getMax());
		keyCommandsChars.clear();
		for (size_t i = 0; i < numPresets; i++) {
			keyCommandsChars.push_back(pChars[i]);
		}

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
		sHelp += "LEFT/RIGHT  Browse\n";
		sHelp += "UP/DOWN\n";
		sHelp += "\n";
		sHelp += "BACKSPACE   Random\n";
		sHelp += "+Ctrl       Reset\n";
		sHelp += "\n";
		sHelp += "RETURN      New add\n";
		sHelp += "+Ctrl       New name\n";
		sHelp += "\n";
		sHelp += "Preset\n";
		sHelp += ofToString(getPresetIndex()) + "/" + ofToString(getPresetIndexLast()) + "\n";
		sHelp += "\n";
		sHelp += "Path:\n";
		sHelp += pathPresets;
	}

	virtual void drawHelp() {
		if (!bHelp) return;

		ofxSurfing::ofDrawBitmapStringBox(sHelp, ofxSurfing::SURFING_LAYOUT_BOTTOM_RIGHT);
	}

	//--

private:
	void doSave() {
		// note that saves the previously settled file name when picking the index!
		ofLogNotice("ofxSurfingPresetsLite") << "doSave(): " << filename;

		if (bDoingNew) bDoingNew = false;

		ofxSurfing::checkFolderOrCreate(pathPresets);

		// Save Settings
		ofxSurfing::saveGroup(paramsPreset, pathPresets + "\\" + filename + ".json");
	}

	void doLoad() {
		ofLogNotice("ofxSurfingPresetsLite") << "doLoad(): " << filename;

		if (bDoingNew) bDoingNew = false;

		// Load Settings
		ofxSurfing::loadGroup(paramsPreset, pathPresets + "\\" + filename + ".json");

		//TODO
		//indexName.set(filename);
	}

	//-

public:
	void addGroup(ofParameterGroup & group) {
		// make a pointer, bc maybe that = no works well..

		auto ptype = group.type();
		bool isGroup = ptype == typeid(ofParameterGroup).name();
		if (isGroup) {
			paramsPreset = group;
		}

		name = group.getName();
		setNameUI(name);
		parameters.setName(ofToString("PRESETS ") + name);

		pathSettings = name + ofToString("_Presets.json");

		setup();
	}

	/*
	* Example:
	* Two ways for setting this path:
	// data\TEXT\Presets\xx.json
	//
	// 1.
	//presetsManager.setPathGlobal("TEXT");//parent folder
	//presetsManager.setPathPresets("Presets");//presets folder
	//
	// 2.
	presetsManager.setPathPresets("TEXT\\Presets");//you can call only this too!
	*/

	// must be settled before setting pathPresets. Must call setPathPresets after this too.
	void setPathGlobal(string p) {
		pathGlobal = p;
		ofxSurfing::checkFolderOrCreate(pathGlobal);
	}

public:
	// related to inside pathGlobal. Must call setPathGlobal before this too.
	void setPathPresets(string p) {
		//pathPresets = p + "\\Presets";
		if (pathGlobal != "")
			pathPresets = pathGlobal + "\\" + p + "\\";
		else
			pathPresets = p + "\\";

		ofxSurfing::checkFolderOrCreate(pathPresets);
		ofLogNotice("ofxSurfingPresetsLite") << "setPathPresets(): " << pathPresets;
	}

	void setFilename(string p) {
		filename = p;
		ofLogNotice("ofxSurfingPresetsLite") << "setFilename(): " << filename;
	}

	void doLoadPrevious() {
		ofLogNotice("ofxSurfingPresetsLite") << "doLoadPrevious()";
		if (index.getMax() == -1) return;
		int i = index;

		if (index > index.getMin())
			i--;
		else if (i == index.getMin())
			if (bCycled)
				i = index.getMax();
			else
				i = index.getMin();
		index.set(i);
	}

	void doLoadNext() {
		ofLogNotice("ofxSurfingPresetsLite") << "doLoadNext()";
		if (index.getMax() == -1) return;
		int i = index;
		if (i < index.getMax())
			i++;
		else if (i == index.getMax())
			if (bCycled)
				i = index.getMin();
			else
				i = index.getMax();
		index.set(i);
	}

	void doLoadNextRow() {
		ofLogNotice("ofxSurfingPresetsLite") << "doLoadNextRow()";

		if (index > index.getMax() - guiNumColumns) return;
		int i = index;
		i = i + guiNumColumns;
		if (bCycled)
			if (i > index.getMax()) i = index.getMin() + i;
		index = ofClamp(i, index.getMin(), index.getMax());
	}

	void doLoadPreviousRow() {
		ofLogNotice("ofxSurfingPresetsLite") << "doLoadPreviousRow()";

		if (index < guiNumColumns) return;
		int i = index;
		i = i - guiNumColumns;
		if (bCycled)
			if (i < index.getMin()) i = index.getMax() - i;
		index = ofClamp(i, index.getMin(), index.getMax());
	}

private:
	void Changed(ofAbstractParameter & e) {
		string name = e.getName();
		ofLogNotice("ofxSurfingPresetsLite") << "Changed: " << name << ": " << e;

		bFlagBuildHelp = true;

		if (name == index.getName()) {
			// clamp inside dir files amount limits
			index = ofClamp(index.get(), index.getMin(), index.getMax());
			if (filenames.size() == 0) return;

			// Changed?
			if (index.get() != index_PRE) {
				bIsChangedIndex = true;

				if (index_PRE != -1) {
					if (index_PRE < filenames.size() && index < filenames.size()) {
						ofLogNotice("ofxSurfingPresetsLite") << "Changed: Preset Index: " << ofToString(index_PRE) << " > " << ofToString(index);
						ofLogNotice("ofxSurfingPresetsLite") << "\t(" << ofToString(filenames[index_PRE]) << " > " << ofToString(filenames[index]) << ")\n";
					}
				}

				//--

				// 1. Common Load but AutoSave

				if (!bMod_CONTROL && !bMod_ALT) // Ctrl nor Alt not pressed
				{
					// Autosave
					if (bAutoSave) {
						if (dir.size() > 0 && index_PRE < dir.size() && index_PRE < filenames.size()) {
							filename = filenames[index_PRE];
							doSave();
						} else {
							ofLogError("ofxSurfingPresetsLite") << "Preset Index points an out of range file!";
						}
					}

					index_PRE = index;

					//--

					// Load
					ofLogNotice("ofxSurfingPresetsLite") << index.getName() + " : " << ofToString(index);

					if (dir.size() > 0 && index < dir.size() && index_PRE < filenames.size()) {
						filename = filenames[index_PRE];
						//indexName = filename;//crashes..
						doLoad();
					} else {
						ofLogError("ofxSurfingPresetsLite") << "File out of range";
					}
				}

				//--

				// 2. Save / Copy

				// Save to clicked preset index
				// Ctrl pressed. Alt not pressed

				else if (bMod_CONTROL && !bMod_ALT) {
					if (index < filenames.size()) {
						filename = filenames[index];
						doSave();
						ofLogNotice("ofxSurfingPresetsLite") << "PRESET COPY!";

						index_PRE = index;
					}
				}

				//--

				//TODO:

				// 3. Swap

				// (from/to) pre/current index
				// Ctrl not pressed. Alt pressed

				else if (!bMod_CONTROL && bMod_ALT) {
					if (dir.size() > 0 && index < filenames.size() && index_PRE < filenames.size()) {

						// Rename target to source
						string _fFrom = filenames[index_PRE];
						string _fTo = filenames[index];
						ofFile f;

						_fTo = pathPresets + "\\" + _fTo + ".json";
						_fFrom = pathPresets + "\\" + _fFrom + ".json";

						bool bf = f.open(_fTo);
						bool bt = f.renameTo(_fFrom, true, true);

						// Save current to
						filename = _fTo;
						doSave();

						if (bf && bt) {
							ofLogNotice("ofxSurfingPresetsLite") << "PRESET SWAP!";
							ofLogNotice("ofxSurfingPresetsLite") << _fFrom << " <-> " << _fTo;
						} else {
							ofLogError("ofxSurfingPresetsLite") << "WRONG SWAP!";
						}

						index_PRE = index;
					}
				}
			}

			//----

			//TODO:
			// changed
			//if (index_PRE != index) {
			//	if (bAutoSave)
			//	{
			//		if (index_PRE < filenames.size() && index_PRE >= 0) {
			//			filename = filenames[index_PRE];
			//			doSave();
			//		}
			//	}
			//	index_PRE = index;//refresh
			//}

			//if (index < filenames.size() && index >= 0) {
			//	filename = filenames[index];
			//	doLoad();
			//}
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
			int num = getNumFiles();
			if (num == 0) filename = "0";

			// filename is already named
			doSave();

			// scan
			string filename_ = filename;
			doRefreshKitFiles();

			//if (filename_ != filename)
			if (num != getNumFiles()) {
				ofLogNotice("ofxSurfingPresetsLite") << "Reorganize " << dir.size() << " files.";
				for (int i = 0; i < dir.size(); i++) {
					string n = dir.getName(i);
					if (n == filename_ + ".json") {
						index = i;
						break;
					}
				}
			}
		}

		else if (name == vDelete.getName()) {
			doDelete();
		}

		else if (name == vNew.getName()) {
			if (bMod_CONTROL)
				doNewPreset();
			else
				doAddNewPreset();
		}

		//else if (name == vRename.getName()) {
		//	doRename();
		//}

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
			doClearKit();
		}

		else if (name == vPopulateKit.getName()) {
			doPopulateKit(numPresets);
		}

		else if (name == vPopulateRandomKit.getName()) {
			doPopulateRandomKit(numPresets);
		}
	}

	//--

public:
	ofParameterGroup parameters { "PRESETS MANAGER" };
	ofParameterGroup paramsPreset { "Preset" };
	ofParameterGroup paramsBrowse { "Browse" };
	ofParameterGroup paramsManager { "Manager" };
	ofParameterGroup paramsKit { "Kit" };
	ofParameterGroup paramsInternal { "Internal" };
	//ofParameterGroup paramsUI { "UI" };

protected:
	string name = ""; // only for instance naming.

private:
	string pathPresets = "Presets"; // path to put the preset files.
	string pathGlobal = ""; // Optional main folder where nested folder goes inside.

	// Files Browser
protected:
	ofDirectory dir;
	string fileName;
	string filePath;

public:
	string filename = "1";
	vector<string> filenames;

	//--

	int getNumPresets() {
		return index.getMax() + 1;
	}
	int getPresetIndexLast() {
		return index.getMax();
	}
	int getPresetIndex() {
		return index.get();
	}

protected:
	int getNumFiles() {
		return filenames.size();
	}

	bool doRefreshKitFiles() {
		// Load dragged images folder
		ofLogNotice("ofxSurfingPresetsLite") << "doRefreshKitFiles(): " << pathPresets;

		bool b = false;

		dir.listDir(pathPresets);
		dir.allowExt("JSON");
		dir.allowExt("json");
		dir.sort();

		// Log files on folder
		filenames.clear();
		for (int i = 0; i < dir.size(); i++) {
			ofLogNotice("ofxSurfingPresetsLite") << "file "
												 << "[" << ofToString(i) << "] " << dir.getName(i);

			string _name = "NoName"; // without ext
			auto _names = ofSplitString(dir.getName(i), ".");
			if (_names.size() > 0) {
				_name = _names[0];
			}
			filenames.push_back(_name);
		}

		b = (dir.size() > 0);
		if (b) {
			index.setMin(0);
			index.setMax(dir.size() - 1);
		} else {
			index.setMin(-1);
			index.setMax(-1);
			index.set(-1);
			indexName.set("NONE");
		}

		// returns true if there's some file/s!
		return b;
	}

protected:
	//TODO:
	bool bOverInputText = false;
	bool bDoingNew = false;

protected:
	void doAddNewPreset() {
		ofLogNotice("ofxSurfingPresetsLite") << "doAddNewPreset()";

		doNewPreset();
		doSave();
		bool b = doRefreshKitFiles();
		if (b) index = getNumPresets() - 1; //workflow
	}

	void doDelete() {
		ofLogNotice("ofxSurfingPresetsLite") << "doDelete()";

		if (filenames.size() == 0) return;

		ofFile::removeFile(pathPresets + "\\" + filename + ".json");
		doRefreshKitFiles();

		//nameIndex = filenames[index];

		index.set(index.get());
		//doLoad();
	}

	void doNewPreset() {
		ofLogNotice("ofxSurfingPresetsLite") << "doNewPreset()";

		// Only sets a name,
		// Correlated to the amount of files

		if (!bOverInputText) bOverInputText = true;

		bDoingNew = true;

		// default name
		indexName = "";

		// auto name counting the amount of files
		string _n = ofToString(dir.size());
		bool bAvoidOverWrite = false;
		for (int i = 0; i < dir.size(); i++) {
			if (_n == dir.getName(i)) bAvoidOverWrite = true;
		}
		if (!bAvoidOverWrite) indexName = _n;

		//TODO:
		// should rename if file is already there!

		setFilename(indexName);
	}

	//void doRename() {
	//	ofLogNotice("ofxSurfingPresetsLite") << "doRename()";

	//	if (filenames.size() == 0) return;

	//	/*
 //       // remove
 //       //filename = filenames[index];
 //       ofFile::removeFile(pathPresets + "\\" + filename + ".json");
 //       doRefreshKitFiles();
 //       // make new
 //       */

	//	// delete
	//	doDelete();

	//	// create new
	//	if (!bOverInputText) bOverInputText = true;
	//	indexName = "";

	//	setFilename("");
	//}

	void doClearKit(bool createOne = true) {
		ofLogNotice("ofxSurfingPresetsLite") << "doClearKit";

		// Remove all files
		for (int i = 0; i < dir.size(); i++) {
			ofFile file = dir[i];
			file.remove();
		}

		bool b = doRefreshKitFiles();

		if (createOne) doNewPreset();
		if (bOverInputText) bOverInputText = false;

		if (b) index.set(0);
	}

	void doPopulateRandomKit(int amount) { //pass -1 to overwrite/use the same amount of presets
		ofLogNotice("ofxSurfingPresetsLite") << "doPopulateRandomKit() " << amount;

		doPopulateKit(amount, true);
	}

	void doPopulateKit(int amount, bool bRandom = false) { //pass -1 to overwrite/use the same amount of presets
		ofLogNotice("ofxSurfingPresetsLite") << "doPopulateKit() " << amount;

		doClearKit(false);

		// will create the same amount files on folder
		if (amount == -1) {
			amount = dir.size();
		}

		bool b = false;
		for (int i = 0; i < amount; i++) {
			doNewPreset();
			if (bRandom) doRandomizeParams();
			doSave();
			b = doRefreshKitFiles();
		}

		if (b) index = 0; //workflow
	}

	//----

	//--------------------------------------------------------------
	void keyPressed(ofKeyEventArgs & eventArgs) {
		if (!bKeys) return;

		const int & key = eventArgs.key;
		ofLogVerbose("ofxSurfingPresetsLite") << "keyPressed: " << (char)key /*<< " [" << key << "]"*/;

		// Modifiers
		bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
		bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

		if (mod_CONTROL) bMod_CONTROL = true;
		if (mod_ALT) bMod_ALT = true;

		if (key == OF_KEY_LEFT)
			doLoadPrevious();
		else if (key == OF_KEY_RIGHT)
			doLoadNext();
		else if (key == OF_KEY_UP)
			doLoadPreviousRow();
		else if (key == OF_KEY_DOWN)
			doLoadNextRow();

		else if (key == OF_KEY_BACKSPACE && bMod_CONTROL)
			doResetParams();
		else if (key == OF_KEY_BACKSPACE && !bMod_CONTROL)
			doRandomizeParams();

		else if (key == OF_KEY_RETURN && bMod_CONTROL)
			doNewPreset();
		else if (key == OF_KEY_RETURN && !bMod_CONTROL)
			doAddNewPreset();

		else {
			//TODO: not working
			for (size_t i = 0; i < numPresets; i++) {
				if ((char)key == keyCommandsChars[i]) {
					//if (key == int(keyCommandsChars[i])) {
					index.set(i);
					return;
				}
			}
		}

		if (key == 'g')
			setToggleGuiVisible();
		else if (key == 'G')
			bGuiClicker = !bGuiClicker;
	}

	//--------------------------------------------------------------
	void keyReleased(ofKeyEventArgs & eventArgs) {
		if (!bKeys) return;

		const int & key = eventArgs.key;
		ofLogVerbose("ofxSurfingPresetsLite") << "keyReleased: " << (char)key /*<< " [" << key << "]"*/;

		// Modifiers
		bool mod_COMMAND = eventArgs.hasModifier(OF_KEY_COMMAND);
		bool mod_CONTROL = eventArgs.hasModifier(OF_KEY_CONTROL);
		bool mod_ALT = eventArgs.hasModifier(OF_KEY_ALT);
		bool mod_SHIFT = eventArgs.hasModifier(OF_KEY_SHIFT);

		if (!mod_CONTROL) bMod_CONTROL = false;
		if (!mod_ALT) bMod_ALT = false;
	}

	//--------------------------------------------------------------
	void addKeysListeners() {
		ofAddListener(ofEvents().keyPressed, this, &ofxSurfingPresetsLite::keyPressed);
		ofAddListener(ofEvents().keyReleased, this, &ofxSurfingPresetsLite::keyReleased);
	}

	//--------------------------------------------------------------
	void removeKeysListeners() {
		ofRemoveListener(ofEvents().keyPressed, this, &ofxSurfingPresetsLite::keyPressed);
		ofRemoveListener(ofEvents().keyReleased, this, &ofxSurfingPresetsLite::keyReleased);
	}

	// easy callback to know when the preset index/selector changed
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
		ofLogNotice("ofxSurfingPresetsLite") << "doRandomizeParams";

		//TODO:
		// this is not recursive inside the nested groups content!
		// get solution from ImHelpers from addGroup to iterate groups.

		for (int i = 0; i < paramsPreset.size(); i++) {
			auto & p = paramsPreset[i];

			if (p.type() == typeid(ofParameter<float>).name()) {
				ofParameter<float> pr = p.cast<float>();
				float v = ofRandom(pr.getMin(), pr.getMax());
				if (bSilent)
					pr.setWithoutEventNotifications(v);
				else
					pr.set(v);
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			else if (p.type() == typeid(ofParameter<int>).name()) {
				ofParameter<int> pr = p.cast<int>();
				int v = ofRandom(pr.getMin(), pr.getMax());
				if (bSilent)
					pr.setWithoutEventNotifications(v);
				else
					pr.set(v);
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			// include booleans
			else if (p.type() == typeid(ofParameter<bool>).name()) {
				ofParameter<bool> pr = p.cast<bool>();
				bool b = (ofRandom(1.f) >= 0.5f);
				if (bSilent)
					pr.setWithoutEventNotifications(b);
				else
					pr.set(b);
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			// colors
			else if (p.type() == typeid(ofParameter<ofColor>).name()) {
				ofParameter<ofColor> pr = p.cast<ofColor>();
				ofColor c;
				c.setBrightness(255);
				c.setSaturation(255);
				c.setHue(ofRandom(255));

				if (bSilent)
					pr.setWithoutEventNotifications(c);
				else
					pr.set(c);
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}
		}
	}

	// Reset Simple
	//--------------------------------------------------------------
	void doResetParams(bool bSilent = false) {
		ofLogNotice("ofxSurfingPresetsLite") << "doResetParams";

		for (int i = 0; i < paramsPreset.size(); i++) {
			auto & p = paramsPreset[i];

			if (p.type() == typeid(ofParameter<float>).name()) {
				ofParameter<float> pr = p.cast<float>();
				if (bSilent)
					pr.setWithoutEventNotifications(pr.getMin());
				else
					pr.set(pr.getMin());
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			else if (p.type() == typeid(ofParameter<int>).name()) {
				ofParameter<int> pr = p.cast<int>();
				if (bSilent)
					pr.setWithoutEventNotifications(pr.getMin());
				else
					pr.set(pr.getMin());
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}

			// include booleans
			else if (p.type() == typeid(ofParameter<bool>).name()) {
				ofParameter<bool> pr = p.cast<bool>();
				bool b = false;
				if (bSilent)
					pr.setWithoutEventNotifications(b);
				else
					pr.set(b);
				ofLogNotice("ofxSurfingPresetsLite") << pr.getName() << " = " << pr.get();
			}
		}

		//if (!bSilent) bIsRetrigged = true;
	}

	//--

	//TODO:
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
        ofLogNotice("ofxSurfingPresetsLite") << (__FUNCTION__);


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
