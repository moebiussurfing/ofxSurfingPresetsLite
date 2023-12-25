#pragma once
#include "ofMain.h"

//--

#include "ofxSurfingPresetsLite.h"

#include "ofxSurfingImGui.h"

//----

class ofxSurfingPresetsLiteImGui : public ofxSurfingPresetsLite {
private:
	ofxSurfingGui * ui = nullptr;

public:
	void setUiPtr(ofxSurfingGui * _ui) {
		ui = _ui;

		//TODO:
		// matrix colors
		if (bGuiColorized) {
			colors.clear();
			for (size_t i = 0; i < 9; i++) {
				ofColor c;
				if (i < 3)
					c = ofColor::green;
				else if (i < 6)
					c = ofColor::yellow;
				else if (i < 10)
					c = ofColor::red;

				colors.push_back(c);
			}
		}
	}

	//----

	// 1. Main window
	// using different layouts.
public:
	void drawImGui(bool bWindowed = false, bool bMinimized_ = false, bool bFoldered_ = false, bool bOpen = true) {
		if (!bGui) return;

		bool bOpen_ = true;

		if (bWindowed) {
			IMGUI_SUGAR__WINDOWS_CONSTRAINTSW_SMALL_LOCKED_RESIZE;
			bOpen_ = ui->BeginWindow(bGui);
		}

		else if (bFoldered_) {
			bOpen_ = true;
		}

		if (bOpen_) {
			string s = filename;
			string sn = "Presets";

			bool b;
			if (!bFoldered_) {
				b = true;
				if (!bWindowed && ui->isMaximized()) ui->AddLabelBig(sn);
			} else {
				b = ui->BeginTree(sn, bOpen);
				if (b) ui->AddSpacing();
			}

			ui->Add(ui->bMinimize, OFX_IM_TOGGLE_ROUNDED_MINI);
			ui->AddSpacing();

			if (b) {
				if (ui->isMaximized()) {
					ui->Add(bGuiParams, OFX_IM_TOGGLE_ROUNDED_MINI);
					ui->Add(bGuiExpand, OFX_IM_TOGGLE_ROUNDED_MINI);
					ui->AddSpacing();
				}

				//if (bMinimized_ || !bGuiFloating || !bGuiParams)
				//	ui->Add(ui->bMinimize, OFX_IM_TOGGLE_ROUNDED_MINI);

				// expanded
				if (bGuiExpand) {
					// maximized
					if (ui->isMaximized()) {
						ui->Add(vLoad, OFX_IM_BUTTON_SMALL, 2, true);
						ui->AddTooltip("Discard changes\nand reload file");
						if (ui->Add(vSave,
								(bAutoSave ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_SMALL_BORDER_BLINK), 2)) {
							bOverInputText = false;
							indexName = s;
						}
						ui->AddTooltip("Save changes\nto file");

						ui->Add(bAutoSave, OFX_IM_TOGGLE_SMALL_BORDER_BLINK);
						//ui->Add(vRename, OFX_IM_BUTTON_SMALL, 2, true);
						//ui->AddTooltip("Change file name");
						//ui->Add(bAutoSave, OFX_IM_TOGGLE_SMALL_BORDER_BLINK, 2);
						string s_;
						if (bAutoSave) {
							s_ = "Edit Mode";
						} else {
							s_ = "Manual save Mode";
						}
						ui->AddTooltip(s_);

						if (!bOverInputText) {
							ui->Add(vNew, OFX_IM_BUTTON_SMALL, 2);
							string s_;
							if (bMod_CONTROL) {
								s_ = "Release control\nto rename";
							} else {
								s_ = "Press control\nto create";
							}
							ui->AddTooltip(s_);
						} else {
							if (ui->AddButton("Cancel", OFX_IM_BUTTON_SMALL_BORDER_BLINK, 2)) {
								bOverInputText = false;
								bDoingNew = false;
							}
						}
						ui->SameLine();
						ui->Add(vDelete, OFX_IM_BUTTON_SMALL, 2);
						ui->AddTooltip("Remove preset file");

						ui->Add(vReset, OFX_IM_BUTTON_SMALL, 2, true);
						ui->AddTooltip("Preset reset");
						ui->Add(vRandom, OFX_IM_BUTTON_SMALL, 2);
						ui->AddTooltip("Preset random");

						//--

						ui->refreshLayout();
						float _w2 = getWidgetsWidth(2);
						float _h = getWidgetsHeightUnit();

						//ui->AddLabel("KIT");

						if (ImGui::Button("Clear", ImVec2(_w2, _h))) {
							ImGui::OpenPopup("CLEAR KIT?");
						}
						ui->AddTooltip("Remove all \nfile presets!");

						if (ImGui::BeginPopupModal("CLEAR KIT?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
							ui->AddLabelBig("Presets kit \nwill be erased.", true, true);
							ui->AddSpacing();
							ui->AddLabelBig("This operation \ncannot be undone!", true, true);
							ui->AddSpacingBig();

							static bool dont_ask_me_next_time = false;
							ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
							ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
							ImGui::PopStyleVar();

							ui->AddSpacing();

							if (!dont_ask_me_next_time) {
								if (ImGui::Button("OK", ImVec2(120, 2 * _h))) {
									ofLogNotice("ofxSurfingPresetsLiteImGui") << "CLEAR";
									doClearKit();
									ImGui::CloseCurrentPopup();
								}
								ImGui::SetItemDefaultFocus();
								ImGui::SameLine();
								if (ImGui::Button("Cancel", ImVec2(120, 2 * _h))) {
									ImGui::CloseCurrentPopup();
								}
							} else {
								ofLogNotice("ofxSurfingPresetsLiteImGui") << "CLEAR";
								doClearKit(false);
								ImGui::CloseCurrentPopup();

								if (bOverInputText) bOverInputText = false;
							}

							ImGui::EndPopup();
						}

						ui->SameLine();
						ui->Add(vScanKit, OFX_IM_BUTTON_SMALL, 2);
						ui->AddTooltip("Reload folder files");

						if (ui->Add(vPopulateKit, OFX_IM_BUTTON_SMALL, 2)) {
							if (bOverInputText) bOverInputText = false;
						}
						ui->AddTooltip("Clear kit and create \nNew presets copying current");
						ui->SameLine();
						ui->Add(vPopulateRandomKit, OFX_IM_BUTTON_SMALL, 2);
						ui->AddTooltip("Clear kit and create \nNew randomized presets ");
					}

					// minimized
					else {
						if (ui->Add(vSave,
								(bOverInputText ? OFX_IM_BUTTON_SMALL_BORDER_BLINK : OFX_IM_BUTTON_SMALL), 2, true)) {
							bOverInputText = false;
							indexName = s;
						}

						if (!bOverInputText) {
							ui->Add(vNew, OFX_IM_BUTTON_SMALL, 2);
						} else {
							if (ui->AddButton("Cancel", OFX_IM_BUTTON_SMALL_BORDER_BLINK, 2)) {
								bOverInputText = false;
								bDoingNew = false;
							}
						}
					}

					//--

					if (bOverInputText) {
						if (!ui->bMinimize) ui->AddSpacing();

						int _w = ui->getWidgetsWidth() * 0.9f;
						ImGui::PushItemWidth(_w);
						{
							bool b = ImGui::InputText("##NAME", &s);
							if (b) {
								ofLogNotice("ofxSurfingPresetsLiteImGui") << "InputText:" << s.c_str();
								setFilename(s);
							}
							if (ImGui::IsItemDeactivated() && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
								bOverInputText = false;
						}
						ImGui::PopItemWidth();
					}
				}

				if (ui->bMinimize && !bGuiExpand) {
					ui->Add(vPrevious, OFX_IM_BUTTON_MEDIUM, 2, true);
					ui->Add(vNext, OFX_IM_BUTTON_MEDIUM, 2);
				}

				//--

				if (ui->isMaximized() && index.getMax() > -1) ui->AddSpacingSeparated();

				if (bGuiExpand) // expanded
				{
					ui->AddCombo(index, filenames);
				} else // not expanded
				{
					if (!ui->bMinimize) {
						ui->Add(vPrevious, OFX_IM_BUTTON_MEDIUM, 2, true);
						ui->Add(vNext, OFX_IM_BUTTON_MEDIUM, 2);
						ui->AddCombo(index, filenames);
						ui->Add(vLoad, OFX_IM_BUTTON_SMALL, 2, true);
						ui->Add(vSave, (bAutoSave ? OFX_IM_BUTTON_SMALL : OFX_IM_BUTTON_SMALL_BORDER_BLINK), 2);
					}
				}

				//--

				if (!ui->bMinimize) ui->AddSpacingSeparated();

				//--

				drawImGuiClicker();

				//if (!bGuiClickerMini) {
				//	drawImGuiClicker();
				//} else {
				//	drawImGuiClicker(bWindowed, true);
				//}

				//--

				if (b && bFoldered_) ui->EndTree();
			}
		}

		if (bWindowed && bOpen_) {
			ui->EndWindow();
		}
	}

	//----

	// 2. Presets Clicker

	void drawImGuiClicker(bool bWindowed = false, bool bMinimal = false) {

		bool bOpen_ = false;
		if (bWindowed) {
			bOpen_ = ui->BeginWindow(bGui);
			if (!bOpen_) {
				ui->EndWindow();
				return;
			}
		}

		//--

		if (!bMinimal) {
			if (!ui->bMinimize) {
				ui->Add(bGuiClicker, OFX_IM_TOGGLE_BUTTON_ROUNDED_MINI);
				//ui->Add(bGuiClickerMini, OFX_IM_TOGGLE_ROUNDED_MINI_XS);
				ui->AddSpacing();
			}
		} else {
			if (!bGuiClicker) bGuiClicker = true;
		}

		if (bGuiClicker) {
			if (ui->isMinimized() && !bMinimal) ui->AddSpacingSeparated();

			float _h2 = ui->getWidgetsHeightUnit();
			_h2 *= 1.5f;

			bool bResponsiveButtonsClicker = true;

			//TODO: add a public var
			bool bFlip = false;

			string sTip = "";
			if (bMod_CONTROL)
				sTip = "Copy to...";
			else if (bMod_ALT)
				sTip = "Swap between..";
			else if (bHelp)
				sTip = "Press Ctrl/Alt\nto copy/swap";

			if (bGuiColorized) {
				ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, colors, bResponsiveButtonsClicker,
					guiNumColumns, true, _h2, sTip, bFlip);
			} else {
				ofxImGuiSurfing::AddMatrixClickerLabels(index, keyCommandsChars, bResponsiveButtonsClicker,
					guiNumColumns, true, _h2, sTip, bFlip);
			}
			if (bGuiExpand) {
				if (index.getMax() > -1) {
					if (!bMinimal && ui->isMaximized()) {
						ui->AddSpacing();
						ui->Add(ui->bAdvanced, OFX_IM_TOGGLE_ROUNDED_MINI);
						if (ui->bAdvanced) {
							ui->AddSpacing();
							ui->AddLabel(guiNumColumns.getName());
							ui->Add(guiNumColumns, OFX_IM_STEPPER_NO_LABEL);
							ui->AddTooltip("Buttons per row.");
							ui->AddLabel(numPresets.getName());
							ui->Add(numPresets, OFX_IM_STEPPER_NO_LABEL);
							ui->AddTooltip("Amount populating presets.");
							ui->Add(bCycled);
							ui->Add(bKeys);
							ui->Add(bHelp);
						}
					}
				}
			}
		}

		//--

		if (bWindowed) {
			if (bOpen_) {
				ui->EndWindow();
			}
		}
	}

	//----

	// 3. Only draw UI

	void drawGui() {
		if (!bGui) return;
		drawHelp();

		if (ui == nullptr) return;
		ui->Begin();
		{
			if (bGuiParams) {

				string n;
				if (bGuiFloating)
					n = "PARAMETERS " + name;
				else
					n = "PRESETS " + name;
				if (ui->BeginWindow(n)) {

				//if (ui->BeginWindow(bGuiParams)) {
					ui->Add(ui->bMinimize, OFX_IM_TOGGLE_ROUNDED_SMALL);
					ui->Add(bGuiFloating, OFX_IM_TOGGLE_ROUNDED_MINI);

					ui->AddSpacingSeparated();

					if (!bGuiFloating && ui->isMaximized())
						ui->AddLabelBig("Parameters");
					ui->AddSpacing();

					ui->AddGroup(paramsPreset);

					// Integrated inside another panel
					if (!bGuiFloating) {
						ui->AddSpacingBigSeparated();

						drawImGui();
					}

					ui->EndWindow();
				};
			}

			// C.
			// Separated on another floating window
			if (bGuiFloating) drawImGui(true);
		}
		ui->End();
	}

protected:
	vector<ofColor> colors;
	bool bGuiColorized = false;

public:
	void setGuiColorized(bool b) { bGuiColorized = b; }
};
