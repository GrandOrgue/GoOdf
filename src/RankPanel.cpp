/*
 * RankPanel.cpp is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * GoOdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GoOdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GoOdf. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#include "RankPanel.h"
#include <wx/statline.h>
#include <wx/button.h>
#include "GOODFFunctions.h"
#include "GOODF.h"
#include <wx/msgdlg.h>
#include <wx/dirdlg.h>
#include <wx/stdpaths.h>
#include "PipeDialog.h"
#include "ReleaseDialog.h"
#include "AttackDialog.h"
#include "StopPanel.h"
#include "PipeBorrowingDialog.h"
#include "PipeCopyOffsetDialog.h"
#include "PipeLoadingDialog.h"
#include <algorithm>
#include "WAVfileParser.h"
#include "SampleFileInfoDialog.h"
#include "DoubleEntryDialog.h"
#include <cmath>

// Event table
BEGIN_EVENT_TABLE(RankPanel, wxPanel)
	EVT_TEXT(ID_RANK_NAME_TEXT, RankPanel::OnNameChange)
	EVT_CHOICE(ID_RANK_WINDCHEST_CHOICE, RankPanel::OnWindchestChoice)
	EVT_SPINCTRL(ID_RANK_FIRST_MIDI_NOTE_SPIN, RankPanel::OnMidiNoteSpin)
	EVT_SPINCTRL(ID_RANK_LOGICAL_PIPES_SPIN, RankPanel::OnLogicalPipeSpin)
	EVT_SPINCTRL(ID_RANK_HARMONIC_NBR_SPIN, RankPanel::OnHarmonicNbrSpin)
	EVT_BUTTON(ID_RANK_SET_HN_FROM_PITCH_BTN, RankPanel::OnSetHarmonicNbrBtn)
	EVT_SPINCTRLDOUBLE(ID_RANK_PITCH_CORR_SPIN, RankPanel::OnPitchCorrectionSpin)
	EVT_RADIOBUTTON(ID_RANK_PERCUSSIVE_YES, RankPanel::OnPercussiveSelection)
	EVT_RADIOBUTTON(ID_RANK_PERCUSSIVE_NO, RankPanel::OnPercussiveSelection)
	EVT_RADIOBUTTON(ID_RANK_INDEPENDENT_RELEASE_YES, RankPanel::OnIndependentReleaseSelection)
	EVT_RADIOBUTTON(ID_RANK_INDEPENDENT_RELEASE_NO, RankPanel::OnIndependentReleaseSelection)
	EVT_SPINCTRLDOUBLE(ID_RANK_MIN_VEL_VOL_SPIN, RankPanel::OnMinVelocitySpin)
	EVT_SPINCTRLDOUBLE(ID_RANK_MAX_VEL_VOL_SPIN, RankPanel::OnMaxVelocitySpin)
	EVT_RADIOBUTTON(ID_RANK_ACC_RETUNING_YES, RankPanel::OnRetuningSelection)
	EVT_RADIOBUTTON(ID_RANK_ACC_RETUNING_NO, RankPanel::OnRetuningSelection)
	EVT_BUTTON(ID_RANK_READ_PIPES_BTN, RankPanel::OnReadPipesBtn)
	EVT_BUTTON(ID_RANK_REMOVE_BTN, RankPanel::OnRemoveRankBtn)
	EVT_BUTTON(ID_RANK_CLEAR_PIPES, RankPanel::OnClearPipesBtn)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_RANK_PIPE_TREE, RankPanel::OnPipeTreeItemRightClick)
	EVT_SPINCTRLDOUBLE(ID_RANK_AMP_LVL_SPIN, RankPanel::OnAmplitudeLevelSpin)
	EVT_SPINCTRLDOUBLE(ID_RANK_GAIN_SPIN, RankPanel::OnGainSpin)
	EVT_SPINCTRLDOUBLE(ID_RANK_PITCH_SPIN, RankPanel::OnPitchTuningSpin)
	EVT_SPINCTRL(ID_RANK_TRACKER_DELAY_SPIN, RankPanel::OnTrackerDelaySpin)
	EVT_BUTTON(ID_RANK_ADD_PIPES_BTN, RankPanel::OnAddPipesBtn)
	EVT_BUTTON(ID_RANK_ADD_TREMULANT_PIPES_BTN, RankPanel::OnAddTremulantPipesBtn)
	EVT_BUTTON(ID_RANK_EXPAND_TREE_BTN, RankPanel::OnExpandTreeBtn)
	EVT_BUTTON(ID_RANK_ADD_RELEASES_BTN, RankPanel::OnAddReleaseSamplesBtn)
	EVT_TREE_KEY_DOWN(ID_RANK_PIPE_TREE, RankPanel::OnTreeKeyboardInput)
	EVT_BUTTON(ID_RANK_FLEXIBLE_PIPE_LOADING_BTN, RankPanel::OnFlexiblePipeLoadingBtn)
	EVT_TEXT(ID_RANK_ATTACK_SUBFOLDER_TEXT, RankPanel::OnAttackFolderText)
	EVT_TEXT(ID_RANK_RELEASE_SUBFOLDER_TEXT, RankPanel::OnReleaseFolderText)
	EVT_TEXT(ID_RANK_TREMULANT_SUBFOLDER_TEXT, RankPanel::OnTremulantFolderText)
	EVT_CHECKBOX(ID_RANK_KEY_PRESS_TIME_OPTION, RankPanel::OnExtractTimeCheck)
	EVT_CHECKBOX(ID_RANK_ONLY_ONE_ATK_OPTION, RankPanel::OnOnlyOneAttachCheck)
	EVT_CHECKBOX(ID_RANK_LOAD_RELEASE_OPTION, RankPanel::OnLoadReleaseCheck)
	EVT_CHECKBOX(ID_RANK_LOAD_PIPES_TREM_OFF_OPTION, RankPanel::OnLoadPipesAsTremOffCheck)
END_EVENT_TABLE()

RankPanel::RankPanel(wxWindow *parent) : wxPanel(parent) {
	m_rank = NULL;
	m_isFirstRemoval = true;
	m_lastReferencedManual = -1;
	m_lastReferencedStop = -1;
	m_attackFolder = wxEmptyString;
	m_loadOnlyOneAttack = false;
	m_loadReleaseInAttack = true;
	m_releaseFolder = wxT("rel");
	m_extractTime = true;
	m_tremFolder = wxT("trem");
	m_loadPipesAsTremOff = false;

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *rankNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Rank name: ")
	);
	firstRow->Add(rankNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_RANK_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	wxStaticText *windchestText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Windchestgroup: ")
	);
	firstRow->Add(windchestText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_windchestChoice = new wxChoice(
		this,
		ID_RANK_WINDCHEST_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		availableWindchests
	);
	firstRow->Add(m_windchestChoice, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *firstMidiNoteText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("First Midi note: ")
	);
	secondRow->Add(firstMidiNoteText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstMidiNoteNumberSpin = new wxSpinCtrl(
		this,
		ID_RANK_FIRST_MIDI_NOTE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		256,
		36
	);
	secondRow->Add(m_firstMidiNoteNumberSpin, 0, wxEXPAND|wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *logicalPipeNbrText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Number of logical pipes: ")
	);
	secondRow->Add(logicalPipeNbrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_numberOfLogicalPipesSpin = new wxSpinCtrl(
		this,
		ID_RANK_LOGICAL_PIPES_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		192,
		1
	);
	secondRow->Add(m_numberOfLogicalPipesSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	setHarmonicNbrFromEmbeddedPitchBtn = new wxButton(
		this,
		ID_RANK_SET_HN_FROM_PITCH_BTN,
		wxT("Calculate HN")
	);
	thirdRow->Add(setHarmonicNbrFromEmbeddedPitchBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *harmonicNumberText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Harmonic number: ")
	);
	thirdRow->Add(harmonicNumberText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_harmonicNumberSpin = new wxSpinCtrl(
		this,
		ID_RANK_HARMONIC_NBR_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1024,
		8
	);
	thirdRow->Add(m_harmonicNumberSpin, 0, wxEXPAND|wxALL, 5);
	m_calculatedLength = new wxStaticText(this, wxID_STATIC, wxEmptyString);
	thirdRow->Add(m_calculatedLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	thirdRow->AddStretchSpacer();
	wxStaticText *trackerDelayText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TrackerDelay: ")
	);
	thirdRow->Add(trackerDelayText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trackerDelaySpin = new wxSpinCtrl(
		this,
		ID_RANK_TRACKER_DELAY_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		10000,
		0
	);
	thirdRow->Add(m_trackerDelaySpin, 0, wxGROW|wxALL, 5);
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *pitchRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *pitchText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("PitchTuning: ")
	);
	pitchRow->Add(pitchText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pitchTuningSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_PITCH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1800,
		1800,
		0,
		0.1
	);
	m_pitchTuningSpin->SetDigits(1);
	m_pitchTuningSpin->SetIncrement(1.0f);
	pitchRow->Add(m_pitchTuningSpin, 0, wxEXPAND|wxALL, 5);
	pitchRow->AddStretchSpacer();
	wxStaticText *pitchCorrectionText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Pitch correction: ")
	);
	pitchRow->Add(pitchCorrectionText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pitchCorrectionSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_PITCH_CORR_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1800,
		1800,
		0,
		0.1
	);
	m_pitchCorrectionSpin->SetDigits(1);
	m_pitchCorrectionSpin->SetIncrement(1.0f);
	pitchRow->Add(m_pitchCorrectionSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(pitchRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *ampLvlText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("AmplitudeLevel: ")
	);
	fourthRow->Add(ampLvlText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_amplitudeLevelSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_AMP_LVL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1000,
		100,
		0.1
	);
	m_amplitudeLevelSpin->SetDigits(1);
	m_amplitudeLevelSpin->SetIncrement(1.0f);
	fourthRow->Add(m_amplitudeLevelSpin, 0, wxEXPAND|wxALL, 5);
	fourthRow->AddStretchSpacer();
	wxStaticText *gainText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Gain: ")
	);
	fourthRow->Add(gainText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gainSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_GAIN_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-120,
		40,
		0,
		0.1
	);
	m_gainSpin->SetDigits(1);
	m_gainSpin->SetIncrement(1.0f);
	fourthRow->Add(m_gainSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *minVelocityVolText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Min velocity volume: ")
	);
	fifthRow->Add(minVelocityVolText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_minVelocityVolumeSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_MIN_VEL_VOL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1000,
		100,
		0.1
	);
	m_minVelocityVolumeSpin->SetDigits(1);
	m_minVelocityVolumeSpin->SetIncrement(1.0f);
	fifthRow->Add(m_minVelocityVolumeSpin, 0, wxEXPAND|wxALL, 5);
	fifthRow->AddStretchSpacer();
	wxStaticText *maxVelocityVolText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Max velocity volume: ")
	);
	fifthRow->Add(maxVelocityVolText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maxVelocityVolumeSpin = new wxSpinCtrlDouble(
		this,
		ID_RANK_MAX_VEL_VOL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1000,
		100,
		0.11
	);
	m_maxVelocityVolumeSpin->SetDigits(1);
	m_maxVelocityVolumeSpin->SetIncrement(1.0f);
	fifthRow->Add(m_maxVelocityVolumeSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	m_expandTreeBtn = new wxButton(
		this,
		ID_RANK_EXPAND_TREE_BTN,
		wxT("Expand the pipe tree")
	);
	sixthRow->Add(m_expandTreeBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sixthRow->AddStretchSpacer();
	wxStaticText *isPercussiveText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Percussive: ")
	);
	sixthRow->Add(isPercussiveText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveYes = new wxRadioButton(
		this,
		ID_RANK_PERCUSSIVE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_isPercussiveYes->SetValue(false);
	sixthRow->Add(m_isPercussiveYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveNo = new wxRadioButton(
		this,
		ID_RANK_PERCUSSIVE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_isPercussiveNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveNo->SetValue(true);
	sixthRow->AddStretchSpacer();
	wxStaticText *independentReleaseText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Independent release: ")
	);
	sixthRow->Add(independentReleaseText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasIndependentReleaseYes = new wxRadioButton(
		this,
		ID_RANK_INDEPENDENT_RELEASE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_hasIndependentReleaseYes->SetValue(false);
	sixthRow->Add(m_hasIndependentReleaseYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasIndependentReleaseNo = new wxRadioButton(
		this,
		ID_RANK_INDEPENDENT_RELEASE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_hasIndependentReleaseNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasIndependentReleaseNo->SetValue(true);
	sixthRow->AddStretchSpacer();
	wxStaticText *acceptsRetuningText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Accepts retuning: ")
	);
	sixthRow->Add(acceptsRetuningText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_acceptsRetuningYes = new wxRadioButton(
		this,
		ID_RANK_ACC_RETUNING_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_acceptsRetuningYes->SetValue(true);
	sixthRow->Add(m_acceptsRetuningYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_acceptsRetuningNo = new wxRadioButton(
		this,
		ID_RANK_ACC_RETUNING_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_acceptsRetuningNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	m_pipeTreeCtrl = new wxTreeCtrl(
		this,
		ID_RANK_PIPE_TREE,
		wxDefaultPosition,
		wxDefaultSize,
		wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT
	);
	m_tree_rank_root = m_pipeTreeCtrl->AddRoot("Rank");
	seventhRow->Add(m_pipeTreeCtrl, 1, wxEXPAND);
	wxBoxSizer *pipeReadingOptions = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *actionButtons = new wxBoxSizer(wxHORIZONTAL);
	readPipesFromFolderBtn = new wxButton(
		this,
		ID_RANK_READ_PIPES_BTN,
		wxT("Create new pipes by MIDI nbr from...")
	);
	actionButtons->Add(readPipesFromFolderBtn, 0, wxALL, 5);
	actionButtons->AddStretchSpacer();
	wxButton *clearPipesBtn = new wxButton(
		this,
		ID_RANK_CLEAR_PIPES,
		wxT("Reset/Clear all pipes")
	);
	actionButtons->Add(clearPipesBtn, 0, wxALL, 5);
	pipeReadingOptions->Add(actionButtons, 0, wxGROW);
	wxStaticBoxSizer *readingOptions = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Pipe reading options"));
	wxBoxSizer *optionsRow1 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *extraAttackText = new wxStaticText (
		readingOptions->GetStaticBox(),
		wxID_STATIC,
		wxT("Attack subfolder prefix: ")
	);
	optionsRow1->Add(extraAttackText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);
	m_optionsAttackField = new wxTextCtrl(
		readingOptions->GetStaticBox(),
		ID_RANK_ATTACK_SUBFOLDER_TEXT
	);
	optionsRow1->Add(m_optionsAttackField, 1, wxEXPAND|wxALL, 4);
	readingOptions->Add(optionsRow1, 0, wxGROW);
	wxBoxSizer *optionsRowOnlyOne = new wxBoxSizer(wxHORIZONTAL);
	m_optionsOnlyOneAttack = new wxCheckBox(
		readingOptions->GetStaticBox(),
		ID_RANK_ONLY_ONE_ATK_OPTION,
		wxT("Load only one attack"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_optionsOnlyOneAttack->SetValue(m_loadOnlyOneAttack);
	optionsRowOnlyOne->Add(m_optionsOnlyOneAttack, 0, wxALL, 2);
	optionsRowOnlyOne->AddStretchSpacer();
	m_optionsLoadReleaseInAttack = new wxCheckBox(
		readingOptions->GetStaticBox(),
		ID_RANK_LOAD_RELEASE_OPTION,
		wxT("Load release in attack"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_optionsLoadReleaseInAttack->SetValue(m_loadReleaseInAttack);
	optionsRowOnlyOne->Add(m_optionsLoadReleaseInAttack, 0, wxALL, 2);
	readingOptions->Add(optionsRowOnlyOne,0, wxGROW);
	wxBoxSizer *optionsRow2 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *releaseFolderText = new wxStaticText (
		readingOptions->GetStaticBox(),
		wxID_STATIC,
		wxT("Release subfolder prefix: ")
	);
	optionsRow2->Add(releaseFolderText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);
	m_optionsReleaseField = new wxTextCtrl(
		readingOptions->GetStaticBox(),
		ID_RANK_RELEASE_SUBFOLDER_TEXT
	);
	optionsRow2->Add(m_optionsReleaseField, 1, wxEXPAND|wxALL, 4);
	readingOptions->Add(optionsRow2, 0, wxGROW);
	wxBoxSizer *optionsRow3 = new wxBoxSizer(wxHORIZONTAL);
	m_optionsKeyPressTime = new wxCheckBox(
		readingOptions->GetStaticBox(),
		ID_RANK_KEY_PRESS_TIME_OPTION,
		wxT("Extract MaxKeyPressTime from foldername"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_optionsKeyPressTime->SetValue(m_extractTime);
	optionsRow3->Add(m_optionsKeyPressTime, 0, wxALL, 2);
	readingOptions->Add(optionsRow3, 0, wxGROW);
	wxBoxSizer *optionsRow4 = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *tremulantFolderText = new wxStaticText (
		readingOptions->GetStaticBox(),
		wxID_STATIC,
		wxT("Tremulant folder prefix: ")
	);
	optionsRow4->Add(tremulantFolderText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2);
	m_optionsTremulantField = new wxTextCtrl(
		readingOptions->GetStaticBox(),
		ID_RANK_TREMULANT_SUBFOLDER_TEXT
	);
	optionsRow4->Add(m_optionsTremulantField, 1, wxEXPAND|wxALL, 4);
	readingOptions->Add(optionsRow4, 0, wxGROW);

	wxBoxSizer *optionsRow4b = new wxBoxSizer(wxHORIZONTAL);
	m_loadPipesAsTremOffCheck = new wxCheckBox(
		readingOptions->GetStaticBox(),
		ID_RANK_LOAD_PIPES_TREM_OFF_OPTION,
		wxT("Load pipes to play when wave tremulant is off"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_loadPipesAsTremOffCheck->SetValue(m_loadPipesAsTremOff);
	optionsRow4b->Add(m_loadPipesAsTremOffCheck, 0, wxALL, 2);
	readingOptions->Add(optionsRow4b, 0, wxGROW);

	wxBoxSizer *optionsRow5 = new wxBoxSizer(wxHORIZONTAL);
	m_addPipesFromFolderBtn = new wxButton(
		readingOptions->GetStaticBox(),
		ID_RANK_ADD_PIPES_BTN,
		wxT("Add more samples from...")
	);
	optionsRow5->Add(m_addPipesFromFolderBtn, 1, wxALL, 2);
	m_addTremulantPipesBtn = new wxButton(
		readingOptions->GetStaticBox(),
		ID_RANK_ADD_TREMULANT_PIPES_BTN,
		wxT("Add tremulant samples from...")
	);
	optionsRow5->Add(m_addTremulantPipesBtn, 1, wxALL, 2);
	readingOptions->Add(optionsRow5, 0, wxGROW);
	wxBoxSizer *optionsRow6 = new wxBoxSizer(wxHORIZONTAL);
	m_addReleaseSamplesBtn = new wxButton(
		readingOptions->GetStaticBox(),
		ID_RANK_ADD_RELEASES_BTN,
		wxT("Add (only) releases from...")
	);
	optionsRow6->Add(m_addReleaseSamplesBtn, 1, wxALL, 2);
	m_flexiblePipeLoadingBtn = new wxButton(
		readingOptions->GetStaticBox(),
		ID_RANK_FLEXIBLE_PIPE_LOADING_BTN,
		wxT("Flexible pipe loading...")
	);
	optionsRow6->Add(m_flexiblePipeLoadingBtn, 1, wxALL, 2);
	readingOptions->Add(optionsRow6, 0, wxGROW);
	pipeReadingOptions->Add(readingOptions, 1, wxEXPAND);
	seventhRow->Add(pipeReadingOptions, 1, wxEXPAND);
	panelSizer->Add(seventhRow, 1, wxEXPAND|wxALL, 5);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeRankBtn = new wxButton(
		this,
		ID_RANK_REMOVE_BTN,
		wxT("Remove this rank")
	);
	bottomRow->Add(removeRankBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

RankPanel::~RankPanel() {

}

void RankPanel::setRank(Rank *rank) {
	m_rank = rank;
	m_lastReferencedManual = -1;
	m_lastReferencedStop = -1;

	// update/populate available windchests
	if (!availableWindchests.IsEmpty())
		availableWindchests.Empty();
	unsigned nbWc = ::wxGetApp().m_frame->m_organ->getNumberOfWindchestgroups();
	if (nbWc > 0) {
		for (unsigned i = 0; i < nbWc; i++) {
			availableWindchests.Add(::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(i)->getName());
		}
		m_windchestChoice->Clear();
		m_windchestChoice->Insert(availableWindchests, 0);
	}

	m_nameField->ChangeValue(m_rank->getName());
	if (m_rank->getWindchest() != NULL)
		m_windchestChoice->SetSelection((int) (::wxGetApp().m_frame->m_organ->getIndexOfOrganWindchest(m_rank->getWindchest()) - 1));
	m_firstMidiNoteNumberSpin->SetValue(m_rank->getFirstMidiNoteNumber());
	m_numberOfLogicalPipesSpin->SetValue(m_rank->getNumberOfLogicalPipes());

	// create the necessary pipes in tree
	RebuildPipeTree();
	UpdatePipeTree();

	m_harmonicNumberSpin->SetValue(m_rank->getHarmonicNumber());
	m_calculatedLength->SetLabelText(GOODF_functions::getFootLengthSize(m_rank->getHarmonicNumber()));
	m_pitchCorrectionSpin->SetValue(m_rank->getPitchCorrection());
	if (m_rank->isIndependentRelease()) {
		m_hasIndependentReleaseYes->SetValue(true);
	} else {
		m_hasIndependentReleaseNo->SetValue(true);
	}
	if (m_rank->isPercussive()) {
		m_isPercussiveYes->SetValue(true);
		m_hasIndependentReleaseYes->Enable();
		m_hasIndependentReleaseNo->Enable();
	} else {
		m_isPercussiveNo->SetValue(true);
		m_hasIndependentReleaseYes->Enable(false);
		m_hasIndependentReleaseNo->Enable(false);
	}
	m_minVelocityVolumeSpin->SetValue(m_rank->getMinVelocityVolume());
	m_maxVelocityVolumeSpin->SetValue(m_rank->getMaxVelocityVolume());
	if (m_rank->doesAcceptsRetuning()) {
		m_acceptsRetuningYes->SetValue(true);
		m_acceptsRetuningNo->SetValue(false);
	} else {
		m_acceptsRetuningYes->SetValue(false);
		m_acceptsRetuningNo->SetValue(true);
	}
	m_amplitudeLevelSpin->SetValue(m_rank->getAmplitudeLevel());
	m_gainSpin->SetValue(m_rank->getGain());
	m_pitchTuningSpin->SetValue(m_rank->getPitchTuning());
	m_trackerDelaySpin->SetValue(m_rank->getTrackerDelay());

	m_optionsAttackField->ChangeValue(m_attackFolder);
	m_optionsOnlyOneAttack->SetValue(m_loadOnlyOneAttack);
	m_optionsLoadReleaseInAttack->SetValue(m_loadReleaseInAttack);
	m_optionsReleaseField->ChangeValue(m_releaseFolder);
	m_optionsKeyPressTime->SetValue(m_extractTime);
	m_optionsTremulantField->ChangeValue(m_tremFolder);
	m_loadPipesAsTremOffCheck->SetValue(m_loadPipesAsTremOff);

	// Force updating layout
	Layout();
}

Rank* RankPanel::getCurrentRank() {
	return m_rank;
}

void RankPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_nameField->SetToolTip(wxT("It's best to make the name unique. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_firstMidiNoteNumberSpin->SetToolTip(wxT("The MIDI note number for the first pipe decides the number matching when auto loading pipes with many of the buttons below."));
		m_numberOfLogicalPipesSpin->SetToolTip(wxT("Set the desired number of pipes for the rank here."));
		m_harmonicNumberSpin->SetToolTip(wxT("This value together with MIDI note and PitchFraction affects the automatic re-tuning in different temperaments."));
		m_pitchCorrectionSpin->SetToolTip(wxT("Adjust the tuning in other temperaments than Original with this value."));
		m_windchestChoice->SetToolTip(wxT("Place the rank on a windchest here."));
		m_isPercussiveYes->SetToolTip(wxT("A percussive rank has attack samples that is played once from start to end."));
		m_isPercussiveNo->SetToolTip(wxT("A non-percussive rank has samples with loops and release(s)."));
		m_hasIndependentReleaseYes->SetToolTip(wxT("Requires that Percussive is set to yes. When this also is set to yes, percussive pipes should now have separately defined releases that are played independently of the attacks. The value can be inherited down to any [Pipe] in this rank."));
		m_hasIndependentReleaseNo->SetToolTip(wxT("Percussive pipes are played 'as is' without any loop or release handling. The value can be inherited down to any [Pipe] in this rank."));
		m_minVelocityVolumeSpin->SetToolTip(wxT("If the rank should be velocity sensitive this value should be something else than 100 and decide the lower range."));
		m_maxVelocityVolumeSpin->SetToolTip(wxT("If the rank should be velocity sensitive this value should be something else than 100 and decide the upper range."));
		m_acceptsRetuningYes->SetToolTip(wxT("Enable automatic re-tuning when changing temperaments."));
		m_acceptsRetuningNo->SetToolTip(wxT("Disable automatic re-tuning. Should only be used for different noise effects."));
		m_amplitudeLevelSpin->SetToolTip(wxT("Linear volume adjustment for the rank. The value of 100 means that the sample is played 'as is'."));
		m_gainSpin->SetToolTip(wxT("Logarithmic volume adjustment for the rank. The value of 0 means that the sample is played 'as is'."));
		m_pitchTuningSpin->SetToolTip(wxT("Adjust the overall tuning of the rank in 'Original temperament'."));
		m_trackerDelaySpin->SetToolTip(wxT("Introduce latency (delay the attack) in ms. for the whole rank."));
		readPipesFromFolderBtn->SetToolTip(wxT("Clear and auto load new samples for all the pipes with MIDI number matching and using pipe reading options below."));
		m_optionsAttackField->SetToolTip(wxT("If attacks are in a sub directory of the main rank it can be specified here."));
		m_optionsOnlyOneAttack->SetToolTip(wxT("Check this to only load one attack for each pipe even if many are available."));
		m_optionsLoadReleaseInAttack->SetToolTip(wxT("Un-check to not try adding releases from the attack sample files. For samples coming from SP this should generally be un-checked!"));
		m_optionsReleaseField->SetToolTip(wxT("Set a string to match release sample folders here."));
		m_optionsKeyPressTime->SetToolTip(wxT("If checked, try to set MaxKeyPressTime from numerical values in the release folder name."));
		m_optionsTremulantField->SetToolTip(wxT("Set a string to match tremulant sample folders here."));
		m_loadPipesAsTremOffCheck->SetToolTip(wxT("If checked loaded pipe attacks/releases will have IsTremulant=0 set, which means that they will play when the associated wave tremulant is off. This value is not used when loading tremulant samples specifically."));
		m_addPipesFromFolderBtn->SetToolTip(wxT("Use this button to add more samples from selected folder to the rank. This operation only adds more samples, it doesn't remove existing samples."));
		m_addTremulantPipesBtn->SetToolTip(wxT("Use this button to add separate tremulant samples to the rank. Especially useful if the tremulant samples for the rank is not a sub directory to the other samples."));
		m_addReleaseSamplesBtn->SetToolTip(wxT("Use this button to add separate releases to the rank, it doesn't remove existing samples. This can be useful if the release samples are placed somewhere else than the other samples."));
		m_flexiblePipeLoadingBtn->SetToolTip(wxT("Use this button to auto load samples for the rank with more fexibility than the other buttons allow."));
		m_pipeTreeCtrl->SetToolTip(wxT("The pipe tree pipe(s), attacks and releases can be right clicked to bring up a pop-up menu."));
	} else {
		m_nameField->SetToolTip(wxEmptyString);
		m_firstMidiNoteNumberSpin->SetToolTip(wxEmptyString);
		m_numberOfLogicalPipesSpin->SetToolTip(wxEmptyString);
		m_harmonicNumberSpin->SetToolTip(wxEmptyString);
		m_pitchCorrectionSpin->SetToolTip(wxEmptyString);
		m_windchestChoice->SetToolTip(wxEmptyString);
		m_isPercussiveYes->SetToolTip(wxEmptyString);
		m_isPercussiveNo->SetToolTip(wxEmptyString);
		m_hasIndependentReleaseYes->SetToolTip(wxEmptyString);
		m_hasIndependentReleaseNo->SetToolTip(wxEmptyString);
		m_minVelocityVolumeSpin->SetToolTip(wxEmptyString);
		m_maxVelocityVolumeSpin->SetToolTip(wxEmptyString);
		m_acceptsRetuningYes->SetToolTip(wxEmptyString);
		m_acceptsRetuningNo->SetToolTip(wxEmptyString);
		m_amplitudeLevelSpin->SetToolTip(wxEmptyString);
		m_gainSpin->SetToolTip(wxEmptyString);
		m_pitchTuningSpin->SetToolTip(wxEmptyString);
		m_trackerDelaySpin->SetToolTip(wxEmptyString);
		readPipesFromFolderBtn->SetToolTip(wxEmptyString);
		m_optionsAttackField->SetToolTip(wxEmptyString);
		m_optionsOnlyOneAttack->SetToolTip(wxEmptyString);
		m_optionsLoadReleaseInAttack->SetToolTip(wxEmptyString);
		m_optionsReleaseField->SetToolTip(wxEmptyString);
		m_optionsKeyPressTime->SetToolTip(wxEmptyString);
		m_optionsTremulantField->SetToolTip(wxEmptyString);
		m_loadPipesAsTremOffCheck->SetToolTip(wxEmptyString);
		m_addPipesFromFolderBtn->SetToolTip(wxEmptyString);
		m_addTremulantPipesBtn->SetToolTip(wxEmptyString);
		m_addReleaseSamplesBtn->SetToolTip(wxEmptyString);
		m_flexiblePipeLoadingBtn->SetToolTip(wxEmptyString);
		m_pipeTreeCtrl->SetToolTip(wxEmptyString);
	}
}

void RankPanel::refreshData() {
	m_nameField->ChangeValue(m_rank->getName());
}

void RankPanel::SetPipeReadingOptions(wxString atkFolder, bool oneAttack, bool loadRelease, wxString releaseFolder, bool extractTime, wxString tremFolder, bool loadAsTremOff) {
	m_attackFolder = atkFolder;
	m_loadOnlyOneAttack = oneAttack;
	m_loadReleaseInAttack = loadRelease;
	m_releaseFolder = releaseFolder;
	m_extractTime = extractTime;
	m_tremFolder = tremFolder;
	m_loadPipesAsTremOff = loadAsTremOff;
}

wxString RankPanel::GetAttackFolderOption() {
	return m_attackFolder;
}

bool RankPanel::GetOneAttackOption() {
	return m_loadOnlyOneAttack;
}

bool RankPanel::GetLoadReleaseOption() {
	return m_loadReleaseInAttack;
}

wxString RankPanel::GetReleaseFolderOption() {
	return m_releaseFolder;
}

bool RankPanel::GetExtractTimeOption() {
	return m_extractTime;
}

wxString RankPanel::GetTremFolderOption() {
	return m_tremFolder;
}

bool RankPanel::GetLoadPipesAsTremOffOption() {
	return m_loadPipesAsTremOff;
}

void RankPanel::setNameFieldValue(wxString name) {
	m_nameField->ChangeValue(name);
}

void RankPanel::disableNameFieldInput() {
	m_nameField->Disable();
}

void RankPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void RankPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_rank->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnWindchestChoice(wxCommandEvent& WXUNUSED(event)) {
	if (m_windchestChoice->GetSelection() != wxNOT_FOUND) {
		unsigned selectedIndex = m_windchestChoice->GetSelection();
		m_rank->setWindchest(::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(selectedIndex));

		// Should Percussive and HasIndependentRelease values be applied to the rank recursively from the windchest?
		// Only bother to ask if Percussive for the windchest is set to true
		if (m_rank->getWindchest()->getIsPercussive()) {
			wxString windchestGrp = wxString::Format(wxT("[WindchestGroup%0.3d] with name "), selectedIndex + 1);
			wxString windchestName = m_rank->getWindchest()->getName();
			wxString wPercussive = wxT(" has Percussive=");
			if (m_rank->getWindchest()->getIsPercussive())
				wPercussive += wxT("Y");
			else
				wPercussive += wxT("N");
			wxString wIndepRel = wxT(" and HasIndependentRelease=");
			if (m_rank->getWindchest()->getHasIndependentRelease())
				wIndepRel += wxT("Y");
			else
				wIndepRel += wxT("N");
			wxString question = wxT("\n\nDo you want to apply these values to this rank and it's pipes?");
			wxString fullText = windchestGrp + windchestName + wPercussive + wIndepRel + question;
			wxMessageDialog msg(this, fullText, wxT("Apply windchest values recursively?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// apply values to rank and all pipes of the rank
				bool percussiveValue = m_rank->getWindchest()->getIsPercussive();
				bool indepRelValue = m_rank->getWindchest()->getHasIndependentRelease();
				m_rank->setPercussive(percussiveValue);
				m_rank->setIndependentRelease(indepRelValue);
				for (Pipe &p : m_rank->m_pipes) {
					p.isPercussive = percussiveValue;
					p.setIndependentRelease(indepRelValue);
				}

				// now the panel radiobuttons should also be updated
				if (m_rank->isPercussive())
					m_isPercussiveYes->SetValue(true);
				else
					m_isPercussiveNo->SetValue(true);
				if (m_rank->isIndependentRelease())
					m_hasIndependentReleaseYes->SetValue(true);
				else
					m_hasIndependentReleaseNo->SetValue(true);
			}
		}

		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void RankPanel::OnMidiNoteSpin(wxSpinEvent& WXUNUSED(event)) {
	if (m_rank->hasOnlyDummyPipes()) {
		m_rank->setFirstMidiNoteNumber(m_firstMidiNoteNumberSpin->GetValue());
	} else {
		wxMessageDialog msg(this, wxT("Pipes already exist in rank! Changing this value now causes changes in logical placement and removal of pipes!"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			int spinValue = m_firstMidiNoteNumberSpin->GetValue();
			int inRankValue = m_rank->getFirstMidiNoteNumber();
			m_rank->setFirstMidiNoteNumber(spinValue);

			if (spinValue > inRankValue) {
				// this means that the pipes need to be removed from the bottom
				// and new ones will be created in the top
				int pipesToAddTop = spinValue - inRankValue;

				while (pipesToAddTop > 0) {
					m_rank->addDummyPipeBack();
					m_rank->removePipeFront();
					pipesToAddTop--;
				}

			} else if (inRankValue > spinValue) {
				// pipes need to be removed from the top and new ones created below
				int pipesToAddBottom = inRankValue - spinValue;

				while (pipesToAddBottom > 0) {
					m_rank->addDummyPipeFront();
					m_rank->removePipeBack();
					pipesToAddBottom--;
				}

			}
			RebuildPipeTree();
			UpdatePipeTree();
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnLogicalPipeSpin(wxSpinEvent& WXUNUSED(event)) {
	int pipeSpinValue = m_numberOfLogicalPipesSpin->GetValue();
	int pipesAlreadyInRank = m_rank->getNumberOfLogicalPipes();
	if (pipeSpinValue > pipesAlreadyInRank) {
		m_rank->setNumberOfLogicalPipes(pipeSpinValue);
		int pipesToAdd = pipeSpinValue - pipesAlreadyInRank;
		for (int i = 0; i < pipesToAdd; i++) {
			m_rank->addDummyPipeBack();
		}
		RebuildPipeTree();
		UpdatePipeTree();
	} else if (pipesAlreadyInRank > pipeSpinValue) {
		wxMessageDialog msg(this, wxT("Pipes will be deleted! Are you really sure you want to delete them?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			// we must remove all pipes lost
			int pipesToRemove = pipesAlreadyInRank - pipeSpinValue;
			m_rank->setNumberOfLogicalPipes(pipeSpinValue);

			// then adjust the tree view
			while (pipesToRemove > 0) {
				m_rank->removePipeBack();
				m_pipeTreeCtrl->Delete(m_pipeTreeCtrl->GetLastChild(m_tree_rank_root));
				pipesToRemove--;
			}
			RebuildPipeTree();
			UpdatePipeTree();
		} else {
			m_numberOfLogicalPipesSpin->SetValue(pipesAlreadyInRank);
		}
	}
	// try to notify parent if it's a StopPanel
	StopPanel *theParent = wxDynamicCast(this->GetGrandParent(), StopPanel);
	if (theParent) {
		theParent->internalRankLogicalPipesChanged(m_rank->getNumberOfLogicalPipes());
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnHarmonicNbrSpin(wxSpinEvent& WXUNUSED(event)) {
	int harmonicNbr = m_harmonicNumberSpin->GetValue();
	m_rank->setHarmonicNumber(harmonicNbr);
	m_calculatedLength->SetLabelText(GOODF_functions::getFootLengthSize(harmonicNbr));
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnSetHarmonicNbrBtn(wxCommandEvent& WXUNUSED(event)) {
	DoubleEntryDialog referencePitchDlg(
		this,
		wxT("Embedded pitch info in the first attack plus any PitchTuning for every pipe in this rank\nwill be compared against reference pitch and expected MIDI number\nto set the HarmonicNumber for each pipe in this rank (and the rank itself).\n\nThus you should make sure to have adjusted PitchTuning properly for the pipes\nespecially if samples are re-used for extension of compass."),
		wxT("8' reference pitch for a1 (A4) in Hz"),
		440,
		220,
		880,
		wxID_ANY,
		wxT("Choose reference pitch")
	);
	if (referencePitchDlg.ShowModal() == wxID_OK && referencePitchDlg.TransferDataFromWindow()) {
		double referencePitch = referencePitchDlg.GetValue();
		int pipeMIDInote = m_rank->getFirstMidiNoteNumber();
		bool foundFirstHarmonicNbr = false;
		for (auto& p : m_rank->m_pipes) {
			if (p.isFirstAttackRefPath()) {
				pipeMIDInote++;
				continue;
			}
			for (auto& atk : p.m_attacks) {
				if (atk.fullPath.IsSameAs(wxT("DUMMY"))) {
					pipeMIDInote++;
					continue;
				} else {
					WAVfileParser atkFile(atk.fullPath);
					if (atkFile.isWavOk()) {
						double embeddedPitch = atkFile.getPitchInHz();
						double effectivePitch = embeddedPitch * pow(2, (p.pitchTuning / 1200.0));
						double expectedEightFootPitch = referencePitch * pow(2, ((double)(pipeMIDInote - 69) / 12.0));
						double pitchRatio = effectivePitch / expectedEightFootPitch;
						int harmonicNbr = round(8.0f * pitchRatio);
						p.harmonicNumber = harmonicNbr;
						if (!foundFirstHarmonicNbr) {
							m_rank->setHarmonicNumber(harmonicNbr);
							foundFirstHarmonicNbr = true;
						}
						break;
					}
				}
			}
			pipeMIDInote++;
		}
		if (foundFirstHarmonicNbr) {
			m_harmonicNumberSpin->SetValue(m_rank->getHarmonicNumber());
			m_calculatedLength->SetLabelText(GOODF_functions::getFootLengthSize(m_rank->getHarmonicNumber()));
			::wxGetApp().m_frame->m_organ->setModified(true);
		}
	}
}

void RankPanel::OnPitchCorrectionSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setPitchCorrection((float) m_pitchCorrectionSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnPercussiveSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_RANK_PERCUSSIVE_YES) {
		m_rank->setPercussive(true);
		m_hasIndependentReleaseYes->Enable();
		m_hasIndependentReleaseNo->Enable();

		if (!m_rank->hasOnlyDummyPipes()) {
			// ask if this should be applied to existing pipes
			wxMessageDialog msg(this, wxT("Do you want the Percussive value to be applied recursively to existing pipes?"), wxT("Apply to existing pipes?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// apply Percussive=Y value to all pipes of the rank
				for (Pipe &p : m_rank->m_pipes) {
					p.isPercussive = true;
				}
			}

			// a purely percussive rank should not have any releases!
			for (Pipe &p : m_rank->m_pipes) {

				if (p.isPercussive && !p.hasIndependentRelease) {
					if (!p.m_releases.empty())
						p.m_releases.clear();
				}
			}
		}

	} else {
		m_rank->setPercussive(false);
		// HasIndependentRelease requires that Percussive=Y
		m_rank->setIndependentRelease(false);
		m_hasIndependentReleaseNo->SetValue(true);
		m_hasIndependentReleaseYes->Enable(false);
		m_hasIndependentReleaseNo->Enable(false);

		if (!m_rank->hasOnlyDummyPipes()) {
			// ask if this should be applied to existing pipes
			wxMessageDialog msg(this, wxT("Do you want the Percussive (and also HasIndependentRelease) value to be applied recursively to existing pipes?"), wxT("Apply to existing pipes?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// apply false value for all pipes of the rank
				for (Pipe &p : m_rank->m_pipes) {
					p.isPercussive = false;
					p.setIndependentRelease(false);
				}
			}
		}
	}
	RebuildPipeTree();
	UpdatePipeTree();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnIndependentReleaseSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_RANK_INDEPENDENT_RELEASE_YES) {
		m_rank->setIndependentRelease(true);

		if (!m_rank->hasOnlyDummyPipes()) {
			// ask if this should be applied to existing pipes
			wxMessageDialog msg(this, wxT("Do you want the HasIndependentRelease value to be applied recursively to existing pipes?"), wxT("Apply to existing pipes?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// apply Percussive=Y value to all pipes of the rank
				for (Pipe &p : m_rank->m_pipes) {
					p.setIndependentRelease(true);
				}
			}
		}
	} else {
		m_rank->setIndependentRelease(false);
		if (!m_rank->hasOnlyDummyPipes()) {
			// ask if this should be applied to existing pipes
			wxMessageDialog msg(this, wxT("Do you want the HasIndependentRelease value to be applied recursively to existing pipes?"), wxT("Apply to existing pipes?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// apply false value for all pipes of the rank
				for (Pipe &p : m_rank->m_pipes) {
					p.setIndependentRelease(false);
				}
			}
		}
	}
	RebuildPipeTree();
	UpdatePipeTree();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnMinVelocitySpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setMinVelocityVolume((float) m_minVelocityVolumeSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnMaxVelocitySpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setMaxVelocityVolume((float) m_maxVelocityVolumeSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnRetuningSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_RANK_ACC_RETUNING_YES) {
		m_acceptsRetuningYes->SetValue(true);
		m_rank->setAcceptsRetuning(true);
	} else {
		m_acceptsRetuningNo->SetValue(true);
		m_rank->setAcceptsRetuning(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnReadPipesBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else {
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
		if (defaultPath == wxEmptyString) {
			if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
				defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
			else
				defaultPath = wxStandardPaths::Get().GetDocumentsDir();
		}
	}

	wxDirDialog rankPipesPathDialog(
		this,
		wxT("Pick a root directory to read new pipes from"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (rankPipesPathDialog.ShowModal() == wxID_OK) {
		m_rank->setPipesRootPath(rankPipesPathDialog.GetPath());
		wxString extraAttackFolderPrefix = m_optionsAttackField->GetValue();
		bool loadOnlyOneAttack = m_optionsOnlyOneAttack->GetValue();
		bool loadRelease = m_optionsLoadReleaseInAttack->GetValue();
		wxString releaseFolderPrefix = m_optionsReleaseField->GetValue();
		bool extractKeyPressTime = m_optionsKeyPressTime->GetValue();
		wxString tremulantFolderPrefix = m_optionsTremulantField->GetValue();
		bool loadPipesTremOff = m_loadPipesAsTremOffCheck->GetValue();

		m_rank->readPipes(
			extraAttackFolderPrefix,
			loadOnlyOneAttack,
			loadRelease,
			releaseFolderPrefix,
			extractKeyPressTime,
			tremulantFolderPrefix,
			loadPipesTremOff,
			0,
			m_rank->getFirstMidiNoteNumber(),
			m_rank->getNumberOfLogicalPipes()
		);

		RebuildPipeTree();
		UpdatePipeTree();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnRemoveRankBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this rank?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			DoRemoveRank();
			m_isFirstRemoval = false;
		}
	} else {
		DoRemoveRank();
	}
}

void RankPanel::DoRemoveRank() {
	// remove all possible references in stops first
	::wxGetApp().m_frame->m_organ->removeReferenceToRankInStops(m_rank);

	// then remove this rank
	::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
}

void RankPanel::OnClearPipesBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_rank->hasOnlyDummyPipes())
		return;

	wxMessageDialog msg(this, wxT("Are you really sure you want to clear all pipes?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (msg.ShowModal() == wxID_YES) {
		m_rank->createDummyPipes();
		RebuildPipeTree();
		UpdatePipeTree();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void RankPanel::OnPipeTreeItemRightClick(wxTreeEvent &evt) {
	wxTreeItemId selectedItem = evt.GetItem();
	if (m_pipeTreeCtrl->GetSelection() != selectedItem)
		m_pipeTreeCtrl->SetFocusedItem(selectedItem);
	wxMenu mnu;

	bool showMenu = false;

	if (m_pipeTreeCtrl->GetItemParent(selectedItem) == m_tree_rank_root) {
		// for a pipe selection
		mnu.Append(ID_PIPE_MENU_ADD_ATTACK, "Add new attack from...\tCtrl+A");
		if (!m_rank->getPipeAt(GetSelectedItemIndexRelativeParent())->isPercussive ||
			(m_rank->getPipeAt(GetSelectedItemIndexRelativeParent())->isPercussive && m_rank->getPipeAt(GetSelectedItemIndexRelativeParent())->hasIndependentRelease)
		) {
			mnu.Append(ID_PIPE_MENU_ADD_RELEASE, "Add new release from...\tCtrl+R");
		}
		mnu.Append(ID_PIPE_MENU_CREATE_REFERENCE, "Borrow pipe...\tCtrl+B");
		mnu.Append(ID_PIPE_MENU_COPY_PIPE_OFFSET, "Copy this pipe offset to...\tCtrl+C");
		mnu.Append(ID_PIPE_MENU_EDIT_PIPE, "Edit pipe properties\tCtrl+E");
		mnu.Append(ID_PIPE_MENU_CLEAR_PIPE, "Reset this pipe\tDel");
		showMenu = true;
	} else if (m_pipeTreeCtrl->GetItemText(m_pipeTreeCtrl->GetItemParent(selectedItem)) == wxT("Attack(s)") ||
		(m_pipeTreeCtrl->GetItemParent(m_pipeTreeCtrl->GetItemParent(selectedItem)) == m_tree_rank_root &&
			m_pipeTreeCtrl->GetItemText(selectedItem) != wxT("Attack(s)") &&
			m_pipeTreeCtrl->GetItemText(selectedItem) != wxT("Release(s)")
		)
	) {
		// for an attack
		mnu.Append(ID_PIPE_MENU_EDIT_ATTACK, "Edit attack properties\tCtrl+E");
		mnu.Append(ID_PIPE_MENU_VIEW_ATTACK_SAMPLE_DETAILS, "View sample file details\tCtrl+D");
		mnu.Append(ID_PIPE_MENU_REMOVE_SELECTED_ATTACK, "Delete attack\tDel");
		showMenu = true;
	} else if (m_pipeTreeCtrl->GetItemText(m_pipeTreeCtrl->GetItemParent(selectedItem)) == wxT("Release(s)")) {
		// for a release
		mnu.Append(ID_PIPE_MENU_EDIT_RELEASE, "Edit release properties\tCtrl+E");
		mnu.Append(ID_PIPE_MENU_VIEW_RELEASE_SAMPLE_DETAILS, "View sample file details\tCtrl+D");
		mnu.Append(ID_PIPE_MENU_REMOVE_SELECTED_RELEASE, "Delete release\tDel");
		showMenu = true;
	}

	if (showMenu) {
		mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(RankPanel::OnPopupMenuClick), NULL, this);
		PopupMenu(&mnu);
	}
}

void RankPanel::OnPopupMenuClick(wxCommandEvent &evt) {
	switch(evt.GetId()) {
		case ID_PIPE_MENU_ADD_ATTACK:
			OnAddNewAttack();
			break;
		case ID_PIPE_MENU_ADD_RELEASE:
			OnAddNewRelease();
			break;
		case ID_PIPE_MENU_CLEAR_PIPE:
			OnClearPipe();
			break;
		case ID_PIPE_MENU_CREATE_REFERENCE:
			OnCreateReference();
			break;
		case ID_PIPE_MENU_EDIT_PIPE:
			OnEditPipe();
			break;
		case ID_PIPE_MENU_EDIT_ATTACK:
			OnEditAttack();
			break;
		case ID_PIPE_MENU_EDIT_RELEASE:
			OnEditRelease();
			break;
		case ID_PIPE_MENU_REMOVE_SELECTED_ATTACK:
			OnRemoveSelectedAttack();
			break;
		case ID_PIPE_MENU_REMOVE_SELECTED_RELEASE:
			OnRemoveSelectedRelease();
			break;
		case ID_PIPE_MENU_COPY_PIPE_OFFSET:
			OnCopyPipeOffset();
			break;
		case ID_PIPE_MENU_VIEW_ATTACK_SAMPLE_DETAILS:
			OnViewAttackSample();
			break;
		case ID_PIPE_MENU_VIEW_RELEASE_SAMPLE_DETAILS:
			OnViewReleaseSample();
			break;
	}
}

void RankPanel::UpdatePipeTree() {
	wxTreeItemIdValue cookie;
	bool firstItem = true;
	for (Pipe p : m_rank->m_pipes) {
		wxTreeItemId currentPipe;
		if (firstItem) {
			currentPipe = m_pipeTreeCtrl->GetFirstChild(m_tree_rank_root, cookie);
			firstItem = false;
		} else {
			currentPipe = m_pipeTreeCtrl->GetNextChild(m_tree_rank_root, cookie);
		}

		wxTreeItemId releases = m_pipeTreeCtrl->GetLastChild(currentPipe);
		wxTreeItemId attacks = m_pipeTreeCtrl->GetPrevSibling(releases);

		for (Attack atk : p.m_attacks) {
			m_pipeTreeCtrl->AppendItem(attacks, atk.fileName);
		}

		if (!p.m_releases.empty()) {
			for (Release rel : p.m_releases) {
				m_pipeTreeCtrl->AppendItem(releases, rel.fileName);
			}
		}

	}
}

void RankPanel::RebuildPipeTree() {
	m_pipeTreeCtrl->DeleteChildren(m_tree_rank_root);

	for (int i = 0; i < m_rank->getNumberOfLogicalPipes(); i++) {
		wxString pipeName = wxT("Pipe") + GOODF_functions::number_format(i + 1);
		wxTreeItemId thisPipe = m_pipeTreeCtrl->AppendItem(m_tree_rank_root, pipeName);

		m_pipeTreeCtrl->AppendItem(thisPipe, wxT("Attack(s)"));
		m_pipeTreeCtrl->AppendItem(thisPipe, wxT("Release(s)"));
	}
}

int RankPanel::GetSelectedItemIndexRelativeParent() {
	// return a valid index or -1 if it's not
	int selectedIndex = -1;
	wxTreeItemId selectedItem = m_pipeTreeCtrl->GetSelection();
	wxTreeItemId parent = m_pipeTreeCtrl->GetItemParent(selectedItem);

	if (selectedItem.IsOk() && parent.IsOk()) {

		int numberOfItems = m_pipeTreeCtrl->GetChildrenCount(parent, false);
		wxTreeItemIdValue cookie;

		for (int i = 0; i < numberOfItems; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_pipeTreeCtrl->GetFirstChild(parent, cookie);
			else
				currentId = m_pipeTreeCtrl->GetNextChild(parent, cookie);

			if (selectedItem == currentId) {
				selectedIndex = i;
				break;
			}
		}
		return selectedIndex;
	}

	return selectedIndex;
}

int RankPanel::GetItemIndexRelativeParent(wxTreeItemId item) {
	// return a valid index or -1 if it's not
	int index = -1;

	wxTreeItemId parent = m_pipeTreeCtrl->GetItemParent(item);

	if (item.IsOk() && parent.IsOk()) {

		int numberOfItems = m_pipeTreeCtrl->GetChildrenCount(parent, false);
		wxTreeItemIdValue cookie;

		for (int i = 0; i < numberOfItems; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_pipeTreeCtrl->GetFirstChild(parent, cookie);
			else
				currentId = m_pipeTreeCtrl->GetNextChild(parent, cookie);

			if (item == currentId) {
				index = i;
				break;
			}
		}
		return index;
	}

	return index;
}

void RankPanel::OnAddNewAttack() {
	wxString attackPath;
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else {
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
		if (defaultPath == wxEmptyString) {
			if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
				defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
			else
				defaultPath = wxStandardPaths::Get().GetDocumentsDir();
		}
	}

	wxFileDialog fileDialog(
		this,
		wxT("Select wave sample file as attack"),
		defaultPath,
		"",
		"WAVE files (*.wav;*.wv)|*.wav;*.wv",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	attackPath = fileDialog.GetPath();

	int pipeIndex = GetSelectedItemIndexRelativeParent();
	if  (pipeIndex < 0)
		return;

	bool loadRelease = m_optionsLoadReleaseInAttack->GetValue();
	m_rank->createNewAttackInPipe(pipeIndex, attackPath, loadRelease);

	RebuildPipeTree();
	UpdatePipeTree();

	wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		m_pipeTreeCtrl->ExpandAllChildren(toSelect);
	}

	m_rank->setPipesRootPath(fileDialog.GetDirectory());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnAddNewRelease() {
	wxString releasePath;
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else {
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
		if (defaultPath == wxEmptyString) {
			if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
				defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
			else
				defaultPath = wxStandardPaths::Get().GetDocumentsDir();
		}
	}

	wxFileDialog fileDialog(
		this,
		wxT("Select wave sample file as release"),
		defaultPath,
		"",
		"WAVE files (*.wav;*.wv)|*.wav;*.wv",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	releasePath = fileDialog.GetPath();

	int pipeIndex = GetSelectedItemIndexRelativeParent();
	if  (pipeIndex < 0)
		return;

	bool extractKeyPressTime = m_optionsKeyPressTime->GetValue();
	m_rank->createNewReleaseInPipe(pipeIndex, releasePath, extractKeyPressTime);

	RebuildPipeTree();
	UpdatePipeTree();

	wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		m_pipeTreeCtrl->ExpandAllChildren(toSelect);
	}

	m_rank->setPipesRootPath(fileDialog.GetDirectory());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnClearPipe() {
	int pipeIndex = GetSelectedItemIndexRelativeParent();
	if (pipeIndex < 0)
		return;

	m_rank->clearPipeAt((unsigned) pipeIndex);

	RebuildPipeTree();
	UpdatePipeTree();

	wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		m_pipeTreeCtrl->ExpandAllChildren(toSelect);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnEditPipe() {
	wxTreeItemId firstSelected = GetPipeTreeItemAt(GetSelectedItemIndexRelativeParent());
	bool isItemExpanded = false;
	if (firstSelected.IsOk() && m_pipeTreeCtrl->IsExpanded(firstSelected))
		isItemExpanded = true;

	PipeDialog dlg(m_rank->m_pipes, (unsigned) GetSelectedItemIndexRelativeParent(), this);
	dlg.ShowModal();

	RebuildPipeTree();
	UpdatePipeTree();

	wxTreeItemId toSelect = GetPipeTreeItemAt(dlg.GetSelectedPipeIndex());
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		if (isItemExpanded)
			m_pipeTreeCtrl->ExpandAllChildren(toSelect);

		if (m_pipeTreeCtrl->GetChildrenCount(toSelect) > 0) {
			wxTreeItemId lastChild = m_pipeTreeCtrl->GetLastChild(toSelect);
			if (lastChild.IsOk()) {
				if (m_pipeTreeCtrl->GetChildrenCount(lastChild) > 0) {
					wxTreeItemId veryLastChild = m_pipeTreeCtrl->GetLastChild(lastChild);
					if (veryLastChild.IsOk())
						m_pipeTreeCtrl->EnsureVisible(veryLastChild);
				} else {
					m_pipeTreeCtrl->EnsureVisible(lastChild);
				}
			}
		}
	}
}

void RankPanel::OnCreateReference() {
	int pipeIndex = GetSelectedItemIndexRelativeParent();
	int highestPipeIdx = m_rank->m_pipes.size() - 1;
	int pipesAboveThis = highestPipeIdx - pipeIndex;
	PipeBorrowingDialog dlg(this);

	if (m_lastReferencedManual > -1 && m_lastReferencedStop > -1) {
		dlg.SetDefaultSelections(m_lastReferencedManual, m_lastReferencedStop, pipeIndex + 1);
	}

	if (dlg.ShowModal() == wxID_OK) {
		if(dlg.IsSelectionOk()) {
			int manId = dlg.GetSelectedManual();
			m_lastReferencedManual = manId;
			if (!::wxGetApp().m_frame->m_organ->doesHavePedals())
				manId += 1;
			m_lastReferencedStop = dlg.GetSelectedStop();
			int stopId = m_lastReferencedStop + 1;
			int pipeId = dlg.GetSelectedPipe();
			int pipesToRef = 1; // At least one REF should be made
			int followingPipesFromDialog = dlg.GetFollowingPipes();
			if (followingPipesFromDialog > 0) {
				// limit of max pipes to reference can be either from how many were available in source
				// or how many pipes we actually have in this target rank
				pipesToRef += std::min(followingPipesFromDialog, pipesAboveThis);
			}
			for (int i = 0; i < pipesToRef; i++) {
				wxString refString = wxT("REF:") + GOODF_functions::number_format(manId) + wxT(":") + GOODF_functions::number_format(stopId) + wxT(":") + GOODF_functions::number_format(pipeId + i);
				m_rank->clearPipeAt(pipeIndex + i);
				m_rank->getPipeAt(pipeIndex + i)->m_attacks.front().fileName = refString;
				m_rank->getPipeAt(pipeIndex + i)->m_attacks.front().fullPath = refString;
			}

			RebuildPipeTree();
			UpdatePipeTree();
			::wxGetApp().m_frame->m_organ->setModified(true);
		}
	}
}

void RankPanel::OnEditAttack() {
	wxTreeItemId selectedPipe = GetPipeOfSelection();
	int selectedPipeIndex = GetItemIndexRelativeParent(selectedPipe);
	Pipe *currentPipe = m_rank->getPipeAt(selectedPipeIndex);
	AttackDialog atk_dlg(currentPipe->m_attacks, (unsigned) GetSelectedItemIndexRelativeParent(), this);

	if (atk_dlg.ShowModal() == wxID_OK) {
		// the user wants to copy properties of the selected attack to other
		// attacks in the same directory
		auto sourceAttack = std::next(atk_dlg.m_attacklist.begin(), atk_dlg.m_selectedAttackIndex);
		wxString sourceDir = sourceAttack->fullPath.BeforeLast(wxFILE_SEP_PATH);
		bool hadUnusualTremulants = false;

		for (Pipe &p : m_rank->m_pipes) {
			for (std::list<Attack>::iterator atk = p.m_attacks.begin(); atk != p.m_attacks.end(); ++atk) {
				if (atk->fullPath.BeforeLast(wxFILE_SEP_PATH).IsSameAs(sourceDir) && atk != sourceAttack) {
					atk->attackStart = sourceAttack->attackStart;
					atk->attackVelocity = sourceAttack->attackVelocity;
					atk->cuePoint = sourceAttack->cuePoint;
					atk->isTremulant = sourceAttack->isTremulant;
					atk->loadRelease = sourceAttack->loadRelease;
					atk->maxKeyPressTime = sourceAttack->maxKeyPressTime;
					atk->maxTimeSinceLastRelease = sourceAttack->maxTimeSinceLastRelease;
					atk->releaseEnd = sourceAttack->releaseEnd;
					atk->loopCrossfadeLength = sourceAttack->loopCrossfadeLength;
					atk->releaseCrossfadeLength = sourceAttack->releaseCrossfadeLength;

					if (atk_dlg.GetCopyReplaceLoops()) {
						atk->m_loops.clear();

						// need a way to check that loop end point won't be larger than actual attack samples
						unsigned maxSampleFrames = 0;
						WAVfileParser sample(atk->fullPath);
						if (sample.isWavOk()) {
							maxSampleFrames = sample.getNumberOfFrames();
						}
						if (sourceAttack->m_loops.size()) {
							for (Loop l: sourceAttack->m_loops) {
								if ((unsigned) l.end < maxSampleFrames)
									atk->m_loops.push_back(l);
							}
						}
					}
				}
			}
			if (p.hasUnusualTremulants()) {
				hadUnusualTremulants = true;
			}
		}
		if (hadUnusualTremulants) {
			GOODF_functions::logTremulantMessage(m_rank->getName());
		}
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void RankPanel::OnViewAttackSample() {
	wxTreeItemId selectedPipe = GetPipeOfSelection();
	int selectedPipeIndex = GetItemIndexRelativeParent(selectedPipe);
	Pipe *currentPipe = m_rank->getPipeAt(selectedPipeIndex);
	int selectedAttack = GetSelectedItemIndexRelativeParent();

	if (selectedAttack < 0)
		return;

	if ((unsigned) selectedAttack >= currentPipe->m_attacks.size())
		return;

	auto attackIterator = std::next(currentPipe->m_attacks.begin(), selectedAttack);
	ViewSampleDetails(attackIterator->fullPath);
}

void RankPanel::OnEditRelease() {
	wxTreeItemId selectedPipe = GetPipeOfSelection();
	int selectedPipeIndex = GetItemIndexRelativeParent(selectedPipe);
	Pipe *currentPipe = m_rank->getPipeAt(selectedPipeIndex);
	ReleaseDialog dlg(currentPipe->m_releases, (unsigned) GetSelectedItemIndexRelativeParent(), this);
	bool hadUnusualTremulants = false;

	if (dlg.ShowModal() == wxID_OK) {
		// the user wants to copy properties of the selected release to other
		// releases from the same directory
		Release *sourceRelease = dlg.GetCurrentRelease();
		wxString sourceDir = sourceRelease->fullPath.BeforeLast(wxFILE_SEP_PATH);
		for (Pipe &p : m_rank->m_pipes) {
			for (std::list<Release>::iterator rel = p.m_releases.begin(); rel != p.m_releases.end(); ++rel) {
				if (rel->fullPath.BeforeLast(wxFILE_SEP_PATH).IsSameAs(sourceDir) && &(*rel) != sourceRelease) {
					rel->cuePoint = sourceRelease->cuePoint;
					rel->isTremulant = sourceRelease->isTremulant;
					rel->maxKeyPressTime = sourceRelease->maxKeyPressTime;
					rel->releaseEnd = sourceRelease->releaseEnd;
					rel->releaseCrossfadeLength = sourceRelease->releaseCrossfadeLength;
				}
			}
			if (p.hasUnusualTremulants()) {
				hadUnusualTremulants = true;
			}
		}
		if (hadUnusualTremulants) {
			GOODF_functions::logTremulantMessage(m_rank->getName());
		}
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void RankPanel::OnViewReleaseSample() {
	wxTreeItemId selectedPipe = GetPipeOfSelection();
	int selectedPipeIndex = GetItemIndexRelativeParent(selectedPipe);
	Pipe *currentPipe = m_rank->getPipeAt(selectedPipeIndex);
	int selectedRelease = GetSelectedItemIndexRelativeParent();

	if (selectedRelease < 0)
		return;

	if ((unsigned) selectedRelease >= currentPipe->m_releases.size())
		return;

	auto releaseIterator = std::next(currentPipe->m_releases.begin(), selectedRelease);
	ViewSampleDetails(releaseIterator->fullPath);
}

void RankPanel::OnRemoveSelectedAttack() {
	int attackIndex = GetSelectedItemIndexRelativeParent();
	int pipeIndex = -1;

	wxTreeItemId pipe = GetPipeOfSelection();

	pipeIndex = GetItemIndexRelativeParent(pipe);

	if (attackIndex > -1 && pipeIndex > -1) {
		bool sucessfullyDeleted = m_rank->deleteAttackInPipe((unsigned) pipeIndex, (unsigned) attackIndex);

		if (!sucessfullyDeleted) {
			wxMessageDialog msg(this, wxT("Last attack for a pipe cannot be deleted!"), wxT("Couldn't delete attack!"), wxOK|wxCENTRE);
			msg.ShowModal();
		} else {
			RebuildPipeTree();
			UpdatePipeTree();
			wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
			if (toSelect.IsOk()) {
				m_pipeTreeCtrl->SelectItem(toSelect);
				m_pipeTreeCtrl->ExpandAllChildren(toSelect);
			}
			::wxGetApp().m_frame->m_organ->setModified(true);
		}
	}
}

void RankPanel::OnRemoveSelectedRelease() {
	int releaseIndex = GetSelectedItemIndexRelativeParent();
	int pipeIndex = -1;

	wxTreeItemId pipe = GetPipeOfSelection();

	pipeIndex = GetItemIndexRelativeParent(pipe);

	if (releaseIndex > -1 && pipeIndex > -1)
		m_rank->deleteReleaseInPipe((unsigned) pipeIndex, (unsigned) releaseIndex);

	RebuildPipeTree();
	UpdatePipeTree();
	wxTreeItemId toSelect = GetPipeTreeItemAt(pipeIndex);
	if (toSelect.IsOk()) {
		m_pipeTreeCtrl->SelectItem(toSelect);
		m_pipeTreeCtrl->ExpandAllChildren(toSelect);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnCopyPipeOffset() {
	int selectedPipeIndex = GetSelectedItemIndexRelativeParent();
	if (selectedPipeIndex < 0)
		return;

	PipeCopyOffsetDialog copyDlg(m_pipeTreeCtrl->GetChildrenCount(m_tree_rank_root, false), selectedPipeIndex, this);
	if (copyDlg.ShowModal() == wxID_OK) {
		wxArrayInt targetPipes;
		if (copyDlg.GetSelectedIndices(targetPipes)) {
			Pipe *source = m_rank->getPipeAt(selectedPipeIndex);
			// there are valid target pipes to copy to with offset
			for (unsigned i = 0; i < targetPipes.size(); i++) {
				int targetIdx = targetPipes[i];
				int offset = targetIdx - selectedPipeIndex;
				Pipe *target = m_rank->getPipeAt(targetIdx);

				target->isPercussive = source->isPercussive;
				target->hasIndependentRelease = source->hasIndependentRelease;
				target->amplitudeLevel = source->amplitudeLevel;
				target->gain = source->gain;
				float newPitchTuning = source->pitchTuning + (offset * 100);
				if (newPitchTuning < -1800)
					newPitchTuning = -1800;
				if (newPitchTuning > 1800)
					newPitchTuning = 1800;
				target->pitchTuning = newPitchTuning;
				target->trackerDelay = source->trackerDelay;
				target->harmonicNumber = source->harmonicNumber;
				target->midiKeyNumber = source->midiKeyNumber;
				target->midiPitchFraction = source->midiPitchFraction;
				target->pitchCorrection = source->pitchCorrection;
				target->acceptsRetuning = source->acceptsRetuning;
				target->windchest = source->windchest;
				target->minVelocityVolume = source->minVelocityVolume;
				target->maxVelocityVolume = source->maxVelocityVolume;

				target->m_attacks.clear();
				for (Attack atk : source->m_attacks) {
					target->m_attacks.push_back(atk);
				}

				target->m_releases.clear();
				for (Release rel : source->m_releases) {
					target->m_releases.push_back(rel);
				}
			}

			RebuildPipeTree();
			UpdatePipeTree();

			wxTreeItemId toSelect = GetPipeTreeItemAt(selectedPipeIndex);
			if (toSelect.IsOk()) {
				m_pipeTreeCtrl->SelectItem(toSelect);
				m_pipeTreeCtrl->ExpandAllChildren(toSelect);
			}
			::wxGetApp().m_frame->m_organ->setModified(true);
		}
	}
}

void RankPanel::ViewSampleDetails(wxString fullPathToSample) {
	SampleFileInfoDialog infoDialog(fullPathToSample, this);
	infoDialog.ShowModal();
}

wxTreeItemId RankPanel::GetPipeTreeItemAt(int index) {
	int pipesInTree = m_pipeTreeCtrl->GetChildrenCount(m_tree_rank_root, false);
	wxTreeItemIdValue cookie;
	for (int i = 0; i < pipesInTree; i++) {
		wxTreeItemId currentId;
		if (i == 0)
			currentId = m_pipeTreeCtrl->GetFirstChild(m_tree_rank_root, cookie);
		else
			currentId = m_pipeTreeCtrl->GetNextChild(m_tree_rank_root, cookie);

		if (i == index) {
			return currentId;
		}
	}
	return wxTreeItemId();
}

wxTreeItemId RankPanel::GetPipeOfSelection() {
	wxTreeItemId item = m_pipeTreeCtrl->GetSelection();
	wxTreeItemId parent;
	bool cont = item.IsOk() && item != m_tree_rank_root;
	while (cont) {
		parent = m_pipeTreeCtrl->GetItemParent(item);

		if (parent == m_tree_rank_root) {
			cont = false;
		} else {
			item = parent;
		}
	}
	return item;
}

void RankPanel::OnAmplitudeLevelSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setAmplitudeLevel(m_amplitudeLevelSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnGainSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setGain(m_gainSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnPitchTuningSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_rank->setPitchTuning(m_pitchTuningSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnTrackerDelaySpin(wxSpinEvent& WXUNUSED(event)) {
	m_rank->setTrackerDelay(m_trackerDelaySpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void RankPanel::OnAddPipesBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else {
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
		if (defaultPath == wxEmptyString) {
			if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
				defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
			else
				defaultPath = wxStandardPaths::Get().GetDocumentsDir();
		}
	}

	wxDirDialog rankPipesPathDialog(
		this,
		wxT("Pick a directory to add pipe attack/releases from"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (rankPipesPathDialog.ShowModal() == wxID_OK) {
		m_rank->setPipesRootPath(rankPipesPathDialog.GetPath());
		wxString extraAttackFolderPrefix = m_optionsAttackField->GetValue();
		bool loadOnlyOneAttack = m_optionsOnlyOneAttack->GetValue();
		bool loadRelease = m_optionsLoadReleaseInAttack->GetValue();
		wxString releaseFolderPrefix = m_optionsReleaseField->GetValue();
		bool extractKeyPressTime = m_optionsKeyPressTime->GetValue();
		wxString tremulantFolderPrefix = m_optionsTremulantField->GetValue();
		bool loadPipesTremOff = m_loadPipesAsTremOffCheck->GetValue();

		m_rank->addToPipes(
			extraAttackFolderPrefix,
			loadOnlyOneAttack,
			loadRelease,
			releaseFolderPrefix,
			extractKeyPressTime,
			tremulantFolderPrefix,
			loadPipesTremOff,
			0,
			m_rank->getFirstMidiNoteNumber(),
			m_rank->getNumberOfLogicalPipes()
		);

		RebuildPipeTree();
		UpdatePipeTree();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void RankPanel::OnAddTremulantPipesBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else {
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
		if (defaultPath == wxEmptyString) {
			if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
				defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
			else
				defaultPath = wxStandardPaths::Get().GetDocumentsDir();
		}
	}

	wxDirDialog rankPipesPathDialog(
		this,
		wxT("Pick a directory to add (wave) tremulant attack/releases from"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (rankPipesPathDialog.ShowModal() == wxID_OK) {
		m_rank->setPipesRootPath(rankPipesPathDialog.GetPath());
		wxString extraAttackFolderPrefix = m_optionsAttackField->GetValue();
		bool loadOnlyOneAttack = m_optionsOnlyOneAttack->GetValue();
		bool loadRelease = m_optionsLoadReleaseInAttack->GetValue();
		wxString releaseFolderPrefix = m_optionsReleaseField->GetValue();
		bool extractKeyPressTime = m_optionsKeyPressTime->GetValue();

		m_rank->addTremulantToPipes(
			extraAttackFolderPrefix,
			loadOnlyOneAttack,
			loadRelease,
			releaseFolderPrefix,
			extractKeyPressTime,
			0,
			m_rank->getFirstMidiNoteNumber(),
			m_rank->getNumberOfLogicalPipes()
		);

		RebuildPipeTree();
		UpdatePipeTree();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void RankPanel::OnExpandTreeBtn(wxCommandEvent& WXUNUSED(event)) {
	m_pipeTreeCtrl->ExpandAll();
	if (m_pipeTreeCtrl->GetSelection().IsOk()) {
		m_pipeTreeCtrl->EnsureVisible(m_pipeTreeCtrl->GetSelection());
	}
}

void RankPanel::OnAddReleaseSamplesBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString defaultPath;
	if (m_rank->getPipesRootPath() != wxEmptyString)
		defaultPath = m_rank->getPipesRootPath();
	else {
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
		if (defaultPath == wxEmptyString) {
			if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
				defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
			else
				defaultPath = wxStandardPaths::Get().GetDocumentsDir();
		}
	}

	wxDirDialog rankPipesPathDialog(
		this,
		wxT("Pick a directory to add release samples from"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (rankPipesPathDialog.ShowModal() == wxID_OK) {
		m_rank->setPipesRootPath(rankPipesPathDialog.GetPath());
		bool loadPipesTremOff = m_loadPipesAsTremOffCheck->GetValue();

		m_rank->addReleasesToPipes(
			loadPipesTremOff,
			0,
			m_rank->getFirstMidiNoteNumber(),
			m_rank->getNumberOfLogicalPipes()
		);

		RebuildPipeTree();
		UpdatePipeTree();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void RankPanel::OnFlexiblePipeLoadingBtn(wxCommandEvent& WXUNUSED(event)) {
	int selectedPipeIdx = GetItemIndexRelativeParent(GetPipeOfSelection());
	bool pipeWasSelected = true;
	if (selectedPipeIdx < 0) {
		selectedPipeIdx = 0;
		pipeWasSelected = false;
	}
	PipeLoadingDialog loadingDialog(
		this,
		m_rank->getNumberOfLogicalPipes(),
		selectedPipeIdx + 1,
		m_rank->getFirstMidiNoteNumber() + selectedPipeIdx,
		m_rank->getPipesRootPath(),
		m_optionsAttackField->GetValue(),
		m_optionsOnlyOneAttack->IsChecked(),
		m_optionsLoadReleaseInAttack->IsChecked(),
		m_optionsReleaseField->GetValue(),
		m_optionsKeyPressTime->IsChecked(),
		m_optionsTremulantField->GetValue(),
		m_loadPipesAsTremOffCheck->IsChecked()
	);

	if (loadingDialog.ShowModal() == wxID_OK) {
		m_rank->setPipesRootPath(loadingDialog.GetBaseDirectory());
		switch (loadingDialog.GetSelectedStrategy()) {
			case 0:
				// Create new pipe(s) by number matching
				m_rank->readPipes(
					loadingDialog.GetSubDirectory(),
					loadingDialog.GetLoadOnlyOneAttack(),
					loadingDialog.GetLoadReleaseInAttack(),
					loadingDialog.GetReleaseFolderPrefix(),
					loadingDialog.GetExtractKeyPressTime(),
					loadingDialog.GetTremFolderPrefix(),
					loadingDialog.GetLoadPipesTremOff(),
					loadingDialog.GetStartingPipeNumber() - 1,
					loadingDialog.GetFirstMidiMatchingNbr(),
					loadingDialog.GetNbrPipesToLoad()
				);
				break;
			case 1:
				// Add more samples to pipe(s)
				m_rank->addToPipes(
					loadingDialog.GetSubDirectory(),
					loadingDialog.GetLoadOnlyOneAttack(),
					loadingDialog.GetLoadReleaseInAttack(),
					loadingDialog.GetReleaseFolderPrefix(),
					loadingDialog.GetExtractKeyPressTime(),
					loadingDialog.GetTremFolderPrefix(),
					loadingDialog.GetLoadPipesTremOff(),
					loadingDialog.GetStartingPipeNumber() - 1,
					loadingDialog.GetFirstMidiMatchingNbr(),
					loadingDialog.GetNbrPipesToLoad()
				);
				break;
			case 2:
				// Add tremulant samples to pipe(s)
				m_rank->addTremulantToPipes(
					loadingDialog.GetSubDirectory(),
					loadingDialog.GetLoadOnlyOneAttack(),
					loadingDialog.GetLoadReleaseInAttack(),
					loadingDialog.GetReleaseFolderPrefix(),
					loadingDialog.GetExtractKeyPressTime(),
					loadingDialog.GetStartingPipeNumber() - 1,
					loadingDialog.GetFirstMidiMatchingNbr(),
					loadingDialog.GetNbrPipesToLoad()
				);
				break;
			case 3:
				// Add release samples to pipe(s)
				m_rank->addReleasesToPipes(
					loadingDialog.GetLoadPipesTremOff(),
					loadingDialog.GetStartingPipeNumber() - 1,
					loadingDialog.GetFirstMidiMatchingNbr(),
					loadingDialog.GetNbrPipesToLoad()
				);
				break;
			default:
				break;
		}

		RebuildPipeTree();
		UpdatePipeTree();

		if (pipeWasSelected) {
			wxTreeItemId toSelect = GetPipeTreeItemAt(selectedPipeIdx);
			if (toSelect.IsOk()) {
				m_pipeTreeCtrl->SelectItem(toSelect);
				m_pipeTreeCtrl->ExpandAllChildren(toSelect);
				m_pipeTreeCtrl->EnsureVisible(toSelect);
			}
		}

		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void RankPanel::OnTreeKeyboardInput(wxTreeEvent& event) {
	wxTreeItemId selectedItem = m_pipeTreeCtrl->GetSelection();
	if (m_pipeTreeCtrl->GetItemParent(selectedItem) == m_tree_rank_root) {
		// for a pipe selection
		switch(event.GetKeyCode()) {
			case 'A':
				if (event.GetKeyEvent().GetModifiers() == wxMOD_CONTROL)
					OnAddNewAttack();
				else
					event.Skip();
				break;
			case 'R':
				if (event.GetKeyEvent().GetModifiers() == wxMOD_CONTROL)
					OnAddNewRelease();
				else
					event.Skip();
				break;
			case WXK_DELETE:
				OnClearPipe();
				break;
			case 'B':
				if (event.GetKeyEvent().GetModifiers() == wxMOD_CONTROL)
					OnCreateReference();
				else
					event.Skip();
				break;
			case 'E':
				if (event.GetKeyEvent().GetModifiers() == wxMOD_CONTROL)
					OnEditPipe();
				else
					event.Skip();
				break;
			case 'C':
				if (event.GetKeyEvent().GetModifiers() == wxMOD_CONTROL)
					OnCopyPipeOffset();
				else
					event.Skip();
				break;
			default:
				event.Skip();
		}
	} else if (m_pipeTreeCtrl->GetItemText(m_pipeTreeCtrl->GetItemParent(selectedItem)) == wxT("Attack(s)") ||
		(m_pipeTreeCtrl->GetItemParent(m_pipeTreeCtrl->GetItemParent(selectedItem)) == m_tree_rank_root &&
			m_pipeTreeCtrl->GetItemText(selectedItem) != wxT("Attack(s)") &&
			m_pipeTreeCtrl->GetItemText(selectedItem) != wxT("Release(s)")
		)
	) {
		// for an attack
		switch(event.GetKeyCode()) {
			case WXK_DELETE:
				OnRemoveSelectedAttack();
				break;
			case 'E':
				if (event.GetKeyEvent().GetModifiers() == wxMOD_CONTROL)
					OnEditAttack();
				else
					event.Skip();
				break;
			case 'D':
				if (event.GetKeyEvent().GetModifiers() == wxMOD_CONTROL)
					OnViewAttackSample();
				else
					event.Skip();
				break;
			default:
				event.Skip();
		}
	} else if (m_pipeTreeCtrl->GetItemText(m_pipeTreeCtrl->GetItemParent(selectedItem)) == wxT("Release(s)")) {
		// for a release
		switch(event.GetKeyCode()) {
			case WXK_DELETE:
				OnRemoveSelectedRelease();
				break;
			case 'E':
				if (event.GetKeyEvent().GetModifiers() == wxMOD_CONTROL)
					OnEditRelease();
				else
					event.Skip();
				break;
			case 'D':
				if (event.GetKeyEvent().GetModifiers() == wxMOD_CONTROL)
					OnViewReleaseSample();
				else
					event.Skip();
				break;
			default:
				event.Skip();
		}
	} else {
		event.Skip();
	}
}

void RankPanel::OnAttackFolderText(wxCommandEvent& WXUNUSED(event)) {
	m_attackFolder = m_optionsAttackField->GetValue();
	OnPipeReadingOptionsChanged();
}

void RankPanel::OnOnlyOneAttachCheck(wxCommandEvent& WXUNUSED(event)) {
	m_loadOnlyOneAttack = m_optionsOnlyOneAttack->GetValue();
	OnPipeReadingOptionsChanged();
}

void RankPanel::OnLoadReleaseCheck(wxCommandEvent& WXUNUSED(event)) {
	m_loadReleaseInAttack = m_optionsLoadReleaseInAttack->GetValue();
	OnPipeReadingOptionsChanged();
}

void RankPanel::OnReleaseFolderText(wxCommandEvent& WXUNUSED(event)) {
	m_releaseFolder = m_optionsReleaseField->GetValue();
	OnPipeReadingOptionsChanged();
}

void RankPanel::OnExtractTimeCheck(wxCommandEvent& WXUNUSED(event)) {
	m_extractTime = m_optionsKeyPressTime->GetValue();
	OnPipeReadingOptionsChanged();
}

void RankPanel::OnTremulantFolderText(wxCommandEvent& WXUNUSED(event)) {
	m_tremFolder = m_optionsTremulantField->GetValue();
	OnPipeReadingOptionsChanged();
}

void RankPanel::OnLoadPipesAsTremOffCheck(wxCommandEvent& WXUNUSED(event)) {
	m_loadPipesAsTremOff = m_loadPipesAsTremOffCheck->GetValue();
	OnPipeReadingOptionsChanged();
}

void RankPanel::OnPipeReadingOptionsChanged() {
	::wxGetApp().m_frame->SynchronizePipeReadingOptions(
		this,
		GetAttackFolderOption(),
		GetOneAttackOption(),
		GetLoadReleaseOption(),
		GetReleaseFolderOption(),
		GetExtractTimeOption(),
		GetTremFolderOption(),
		GetLoadPipesAsTremOffOption()
	);
}
