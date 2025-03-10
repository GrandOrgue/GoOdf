/*
 * CouplerPanel.cpp is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * GoOdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GoOdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GoOdf.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#include "CouplerPanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/msgdlg.h>

// Event table
BEGIN_EVENT_TABLE(CouplerPanel, wxPanel)
	EVT_TEXT(ID_COUPLER_NAME_TEXT, CouplerPanel::OnNameChange)
	EVT_CHOICE(ID_COUPLER_FUNCTION_CHOICE, CouplerPanel::OnFunctionChange)
	EVT_RADIOBUTTON(ID_COUPLER_INVERTED_STATE_YES, CouplerPanel::OnDisplayInvertedRadio)
	EVT_RADIOBUTTON(ID_COUPLER_INVERTED_STATE_NO, CouplerPanel::OnDisplayInvertedRadio)
	EVT_CHOICE(ID_COUPLER_GC_STATE_CHOICE, CouplerPanel::OnGcStateChange)
	EVT_RADIOBUTTON(ID_COUPLER_STORE_IN_DIV_YES, CouplerPanel::OnStoreInDivisionalChange)
	EVT_RADIOBUTTON(ID_COUPLER_STORE_IN_DIV_NO, CouplerPanel::OnStoreInDivisionalChange)
	EVT_RADIOBUTTON(ID_COUPLER_STORE_IN_GEN_YES, CouplerPanel::OnStoreInGeneralChange)
	EVT_RADIOBUTTON(ID_COUPLER_STORE_IN_GEN_NO, CouplerPanel::OnStoreInGeneralChange)
	EVT_RADIOBUTTON(ID_COUPLER_DEFAULT_TO_ENGAGED_YES, CouplerPanel::OnDefaultToEngagedChange)
	EVT_RADIOBUTTON(ID_COUPLER_DEFAULT_TO_ENGAGED_NO, CouplerPanel::OnDefaultToEngagedChange)
	EVT_BUTTON(ID_COUPLER_REMOVE_BTN, CouplerPanel::OnRemoveCouplerBtn)
	EVT_BUTTON(ID_COUPLER_ADD_SWITCH_BTN, CouplerPanel::OnAddSwitchReferenceBtn)
	EVT_BUTTON(ID_COUPLER_REMOVE_SWITCH_BTN, CouplerPanel::OnRemoveSwitchReferenceBtn)
	EVT_LISTBOX(ID_COUPLER_AVAILABLE_SWITCHES, CouplerPanel::OnSwitchListboxSelection)
	EVT_LISTBOX(ID_COUPLER_REFERENCED_SWITCHES, CouplerPanel::OnReferencedSwitchSelection)
	EVT_RADIOBUTTON(ID_COUPLER_UNISON_OFF_YES, CouplerPanel::OnUnisonOffRadio)
	EVT_RADIOBUTTON(ID_COUPLER_UNISON_OFF_NO, CouplerPanel::OnUnisonOffRadio)
	EVT_CHOICE(ID_COUPLER_DEST_MAN_CHOICE, CouplerPanel::OnDestinationManualChoice)
	EVT_SPINCTRL(ID_COUPLER_DEST_KEYSHIFT_SPIN, CouplerPanel::OnDestinationKeyShiftSpin)
	EVT_CHOICE(ID_COUPLER_TYPE_CHOICE, CouplerPanel::OnCouplerTypeChoice)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_UNISON_INTER_YES, CouplerPanel::OnCoupleToSubsequentUnisonIntermanualCouplers)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_UNISON_INTER_NO, CouplerPanel::OnCoupleToSubsequentUnisonIntermanualCouplers)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_UPWARD_INTER_YES, CouplerPanel::OnCoupleToSubsequentUpwardIntermanualCouplers)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_UPWARD_INTER_NO, CouplerPanel::OnCoupleToSubsequentUpwardIntermanualCouplers)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_DOWNWARD_INTER_YES, CouplerPanel::OnCoupleToSubsequentDownwardIntermanualCouplers)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_DOWNWARD_INTER_NO, CouplerPanel::OnCoupleToSubsequentDownwardIntermanualCouplers)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_UPWARD_INTRA_YES, CouplerPanel::OnCoupleToSubsequentUpwardIntramanualCouplers)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_UPWARD_INTRA_NO, CouplerPanel::OnCoupleToSubsequentUpwardIntramanualCouplers)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_DOWNWARD_INTRA_YES, CouplerPanel::OnCoupleToSubsequentDownwardIntramanualCouplers)
	EVT_RADIOBUTTON(ID_COUPLER_SUBSEQUENT_DOWNWARD_INTRA_NO, CouplerPanel::OnCoupleToSubsequentDownwardIntramanualCouplers)
	EVT_SPINCTRL(ID_COUPLER_FIRST_MIDI_NOTE_SPIN, CouplerPanel::OnFirstMidiNoteNumberSpin)
	EVT_SPINCTRL(ID_COUPLER_NUMBER_KEYS_SPIN, CouplerPanel::OnNumberOfKeysSpin)
END_EVENT_TABLE()

CouplerPanel::CouplerPanel(wxWindow *parent) : wxPanel(parent) {
	m_coupler = NULL;
	m_isFirstRemoval = true;
	functionChoices.Add(wxT("Input"));
	functionChoices.Add(wxT("And"));
	functionChoices.Add(wxT("Nand"));
	functionChoices.Add(wxT("Not"));
	functionChoices.Add(wxT("Nor"));
	functionChoices.Add(wxT("Or"));
	functionChoices.Add(wxT("Xor"));
	gcStateChoices.Add(wxT("No change"));
	gcStateChoices.Add(wxT("Off"));
	gcStateChoices.Add(wxT("On"));
	couplerTypes.Add(wxT("Normal"));
	couplerTypes.Add(wxT("Bass"));
	couplerTypes.Add(wxT("Melody"));

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *couplerNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Coupler name: ")
	);
	firstRow->Add(couplerNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_COUPLER_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *dispChoiceRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *functionText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Function: ")
	);
	dispChoiceRow->Add(functionText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_functionChoice = new wxChoice(
		this,
		ID_COUPLER_FUNCTION_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		functionChoices
	);
	dispChoiceRow->Add(m_functionChoice, 0, wxALL, 5);
	dispChoiceRow->AddStretchSpacer();
	wxStaticText *dispInvertedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Display in inverted state: ")
	);
	dispChoiceRow->Add(dispInvertedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedYes = new wxRadioButton(
		this,
		ID_COUPLER_INVERTED_STATE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	dispChoiceRow->Add(m_displayInvertedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo = new wxRadioButton(
		this,
		ID_COUPLER_INVERTED_STATE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	dispChoiceRow->Add(m_displayInvertedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo->SetValue(true);
	panelSizer->Add(dispChoiceRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *secondRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available switches")
	);
	secondRow1stCol->Add(availableReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableSwitches = new wxListBox(
		this,
		ID_COUPLER_AVAILABLE_SWITCHES,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	secondRow1stCol->Add(m_availableSwitches, 1, wxEXPAND|wxALL, 5);
	secondRow->Add(secondRow1stCol, 1, wxEXPAND);
	wxBoxSizer *secondRow2ndCol = new wxBoxSizer(wxVERTICAL);
	secondRow2ndCol->AddStretchSpacer();
	m_addReferencedSwitch = new wxButton(
		this,
		ID_COUPLER_ADD_SWITCH_BTN,
		wxT("Add ->")
	);
	secondRow2ndCol->Add(m_addReferencedSwitch, 0, wxGROW|wxALL, 5);
	m_removeReferencedSwitch = new wxButton(
		this,
		ID_COUPLER_REMOVE_SWITCH_BTN,
		wxT("<- Remove")
	);
	secondRow2ndCol->Add(m_removeReferencedSwitch, 0, wxGROW|wxALL, 5);
	secondRow2ndCol->AddStretchSpacer();
	secondRow->Add(secondRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *secondRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced switches")
	);
	secondRow3rdCol->Add(chosenReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedSwitches = new wxListBox(
		this,
		ID_COUPLER_REFERENCED_SWITCHES,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	secondRow3rdCol->Add(m_referencedSwitches, 1, wxEXPAND|wxALL, 5);
	secondRow->Add(secondRow3rdCol, 1, wxEXPAND);
	panelSizer->Add(secondRow, 1, wxEXPAND);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *defaultToEngagedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Default to engaged: ")
	);
	thirdRow->Add(defaultToEngagedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedYes = new wxRadioButton(
		this,
		ID_COUPLER_DEFAULT_TO_ENGAGED_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_defaultToEngagedYes->SetValue(true);
	thirdRow->Add(m_defaultToEngagedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedNo = new wxRadioButton(
		this,
		ID_COUPLER_DEFAULT_TO_ENGAGED_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	thirdRow->Add(m_defaultToEngagedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	thirdRow->AddStretchSpacer();
	wxStaticText *gcStateText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("State when GC is activated: ")
	);
	thirdRow->Add(gcStateText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gcStateChoice = new wxChoice(
		this,
		ID_COUPLER_GC_STATE_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		gcStateChoices
	);
	thirdRow->Add(m_gcStateChoice, 0, wxALL, 5);
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeInDivisionalText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Store in divisional: ")
	);
	fourthRow->Add(storeInDivisionalText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInDivisionalYes = new wxRadioButton(
		this,
		ID_COUPLER_STORE_IN_DIV_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInDivisionalYes->SetValue(true);
	fourthRow->Add(m_storeInDivisionalYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInDivisionalNo = new wxRadioButton(
		this,
		ID_COUPLER_STORE_IN_DIV_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	fourthRow->Add(m_storeInDivisionalNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	fourthRow->AddStretchSpacer();
	wxStaticText *storeInGeneralText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Store in generals: ")
	);
	fourthRow->Add(storeInGeneralText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralYes = new wxRadioButton(
		this,
		ID_COUPLER_STORE_IN_GEN_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInGeneralYes->SetValue(true);
	fourthRow->Add(m_storeInGeneralYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralNo = new wxRadioButton(
		this,
		ID_COUPLER_STORE_IN_GEN_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	fourthRow->Add(m_storeInGeneralNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	fourthRow->AddStretchSpacer();
	wxStaticText *unisonOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Unison off: ")
	);
	fourthRow->Add(unisonOffText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_unisonOffYes = new wxRadioButton(
		this,
		ID_COUPLER_UNISON_OFF_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	fourthRow->Add(m_unisonOffYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_unisonOffNo = new wxRadioButton(
		this,
		ID_COUPLER_UNISON_OFF_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	fourthRow->Add(m_unisonOffNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_unisonOffNo->SetValue(true);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *destinationManualText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Destination manual: ")
	);
	fifthRow->Add(destinationManualText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_destinationManualChoice = new wxChoice(
		this,
		ID_COUPLER_DEST_MAN_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		availableManuals
	);
	fifthRow->Add(m_destinationManualChoice, 0, wxALL, 5);
	fifthRow->AddStretchSpacer();
	wxStaticText *destKeyshiftText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Destination keyshift: ")
	);
	fifthRow->Add(destKeyshiftText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_destinationKeyShiftSpin = new wxSpinCtrl(
		this,
		ID_COUPLER_DEST_KEYSHIFT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-24,
		24,
		0
	);
	fifthRow->Add(m_destinationKeyShiftSpin, 0, wxEXPAND|wxALL, 5);
	fifthRow->AddStretchSpacer();
	wxStaticText *couplerTypeText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Coupler type: ")
	);
	fifthRow->Add(couplerTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_couplerTypeChoice = new wxChoice(
		this,
		ID_COUPLER_TYPE_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		couplerTypes
	);
	fifthRow->Add(m_couplerTypeChoice, 0, wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *cplSubseqUnisInterText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Couple to subsequent unison intermanual couplers: ")
	);
	sixthRow->Add(cplSubseqUnisInterText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentUnisonIntermanualCouplersYes = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_UNISON_INTER_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	sixthRow->Add(m_coupleToSubsequentUnisonIntermanualCouplersYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentUnisonIntermanualCouplersNo = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_UNISON_INTER_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_coupleToSubsequentUnisonIntermanualCouplersNo->SetValue(true);
	sixthRow->Add(m_coupleToSubsequentUnisonIntermanualCouplersNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *cplSubseqUpwardInterText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Couple to subsequent upward intermanual couplers: ")
	);
	seventhRow->Add(cplSubseqUpwardInterText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentUpwardIntermanualCouplersYes = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_UPWARD_INTER_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	seventhRow->Add(m_coupleToSubsequentUpwardIntermanualCouplersYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentUpwardIntermanualCouplersNo = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_UPWARD_INTER_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_coupleToSubsequentUpwardIntermanualCouplersNo->SetValue(true);
	seventhRow->Add(m_coupleToSubsequentUpwardIntermanualCouplersNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(seventhRow, 0, wxGROW);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *cplSubseqDownwardInterText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Couple to subsequent downward intermanual couplers: ")
	);
	eighthRow->Add(cplSubseqDownwardInterText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentDownwardIntermanualCouplersYes = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_DOWNWARD_INTER_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	eighthRow->Add(m_coupleToSubsequentDownwardIntermanualCouplersYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentDownwardIntermanualCouplersNo = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_DOWNWARD_INTER_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_coupleToSubsequentDownwardIntermanualCouplersNo->SetValue(true);
	eighthRow->Add(m_coupleToSubsequentDownwardIntermanualCouplersNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(eighthRow, 0, wxGROW);

	wxBoxSizer *ninthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *cplSubseqUpwardIntraText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Couple to subsequent upward intramanual couplers: ")
	);
	ninthRow->Add(cplSubseqUpwardIntraText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentUpwardIntramanualCouplersYes = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_UPWARD_INTRA_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	ninthRow->Add(m_coupleToSubsequentUpwardIntramanualCouplersYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentUpwardIntramanualCouplersNo = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_UPWARD_INTRA_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_coupleToSubsequentUpwardIntramanualCouplersNo->SetValue(true);
	ninthRow->Add(m_coupleToSubsequentUpwardIntramanualCouplersNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(ninthRow, 0, wxGROW);

	wxBoxSizer *tenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *cplSubseqDownwardIntraText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Couple to subsequent downward intramanual couplers: ")
	);
	tenthRow->Add(cplSubseqDownwardIntraText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentDownwardIntramanualCouplersYes = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_DOWNWARD_INTRA_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	tenthRow->Add(m_coupleToSubsequentDownwardIntramanualCouplersYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_coupleToSubsequentDownwardIntramanualCouplersNo = new wxRadioButton(
		this,
		ID_COUPLER_SUBSEQUENT_DOWNWARD_INTRA_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_coupleToSubsequentDownwardIntramanualCouplersNo->SetValue(true);
	tenthRow->Add(m_coupleToSubsequentDownwardIntramanualCouplersNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(tenthRow, 0, wxGROW);

	wxBoxSizer *eleventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *firstMidiNoteNumberText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("First MIDI note number: ")
	);
	eleventhRow->Add(firstMidiNoteNumberText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstMIDINoteNumberSpin = new wxSpinCtrl(
		this,
		ID_COUPLER_FIRST_MIDI_NOTE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		127,
		0
	);
	eleventhRow->Add(m_firstMIDINoteNumberSpin, 0, wxEXPAND|wxALL, 5);
	eleventhRow->AddStretchSpacer();
	wxStaticText *numberOfKeysText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Number of keys: ")
	);
	eleventhRow->Add(numberOfKeysText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_numberOfKeysSpin = new wxSpinCtrl(
		this,
		ID_COUPLER_NUMBER_KEYS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		127,
		127
	);
	eleventhRow->Add(m_numberOfKeysSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eleventhRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeCouplerBtn = new wxButton(
		this,
		ID_COUPLER_REMOVE_BTN,
		wxT("Remove this coupler")
	);
	bottomRow->Add(removeCouplerBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

CouplerPanel::~CouplerPanel() {

}

void CouplerPanel::setCoupler(Coupler *coupler) {
	m_coupler = coupler;
	m_nameField->ChangeValue(m_coupler->getName());
	if (m_coupler->isDisplayedInverted())
		m_displayInvertedYes->SetValue(true);
	else
		m_displayInvertedNo->SetValue(true);
	m_functionChoice->SetSelection(m_functionChoice->FindString(m_coupler->getFunction()));
	if (m_coupler->getFunction().IsSameAs(wxT("Input"))) {
		m_availableSwitches->Enable(false);
		m_referencedSwitches->Enable(false);
		m_addReferencedSwitch->Enable(false);
		m_removeReferencedSwitch->Enable(false);
		m_defaultToEngagedYes->Enable(true);
		m_defaultToEngagedNo->Enable(true);
		m_gcStateChoice->Enable(true);
	} else {
		m_availableSwitches->Enable(true);
		m_referencedSwitches->Enable(true);
		m_addReferencedSwitch->Enable(true);
		m_removeReferencedSwitch->Enable(true);
		m_defaultToEngagedYes->Enable(false);
		m_defaultToEngagedNo->Enable(false);
		m_gcStateChoice->Enable(false);
	}
	if (m_coupler->isDefaultToEngaged())
		m_defaultToEngagedYes->SetValue(true);
	else
		m_defaultToEngagedNo->SetValue(true);
	m_gcStateChoice->SetSelection((unsigned) (m_coupler->getGcState() + 1));
	if (m_coupler->isStoreInDivisional())
		m_storeInDivisionalYes->SetValue(true);
	else
		m_storeInDivisionalNo->SetValue(true);
	if (m_coupler->isStoreInGeneral())
		m_storeInGeneralYes->SetValue(true);
	else
		m_storeInGeneralNo->SetValue(true);

	// update/populate available switches
	wxArrayString organSwitches;
	unsigned nbSwitches = ::wxGetApp().m_frame->m_organ->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			organSwitches.Add(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(i)->getName());
		}
		m_availableSwitches->Clear();
		m_availableSwitches->InsertItems(organSwitches, 0);
	} else {
		if (m_availableSwitches->GetCount())
			m_availableSwitches->Clear();
	}

	UpdateReferencedSwitches();

	m_addReferencedSwitch->Disable();
	m_removeReferencedSwitch->Disable();

	if (m_coupler->isUnisonOff())
		m_unisonOffYes->SetValue(true);
	else
		m_unisonOffNo->SetValue(true);

	// update/populate available manuals
	if (!availableManuals.IsEmpty())
		availableManuals.Empty();
	unsigned nbManuals = ::wxGetApp().m_frame->m_organ->getNumberOfManuals();
	if (!m_destinationManualChoice->IsEmpty())
		m_destinationManualChoice->Clear();
	if (nbManuals > 0) {
		for (unsigned i = 0; i < nbManuals; i++)
			availableManuals.Add(::wxGetApp().m_frame->m_organ->getOrganManualAt(i)->getName());

		m_destinationManualChoice->Insert(availableManuals, 0);
	}

	if (m_coupler->getDestinationManual()) {
		if (::wxGetApp().m_frame->m_organ->doesHavePedals())
			m_destinationManualChoice->SetSelection(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_coupler->getDestinationManual()));
		else
			m_destinationManualChoice->SetSelection(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_coupler->getDestinationManual()) - 1);
	}

	m_destinationKeyShiftSpin->SetValue(m_coupler->getDestinationKeyshift());
	m_couplerTypeChoice->SetSelection(m_couplerTypeChoice->FindString(m_coupler->getCouplerType()));
	if (m_coupler->isCoupleToSubsequentUnisonIntermanualCouplers())
		m_coupleToSubsequentUnisonIntermanualCouplersYes->SetValue(true);
	else
		m_coupleToSubsequentUnisonIntermanualCouplersNo->SetValue(true);
	if (m_coupler->isCoupleToSubsequentUpwardIntermanualCouplers())
		m_coupleToSubsequentUpwardIntermanualCouplersYes->SetValue(true);
	else
		m_coupleToSubsequentUpwardIntermanualCouplersNo->SetValue(true);
	if (m_coupler->isCoupleToSubsequentDownwardIntermanualCouplers())
		m_coupleToSubsequentDownwardIntermanualCouplersYes->SetValue(true);
	else
		m_coupleToSubsequentDownwardIntermanualCouplersNo->SetValue(true);
	if (m_coupler->isCoupleToSubsequentUpwardIntramanualCouplers())
		m_coupleToSubsequentUpwardIntramanualCouplersYes->SetValue(true);
	else
		m_coupleToSubsequentUpwardIntramanualCouplersNo->SetValue(true);
	if (m_coupler->isCoupleToSubsequentDownwardIntramanualCouplers())
		m_coupleToSubsequentDownwardIntramanualCouplersYes->SetValue(true);
	else
		m_coupleToSubsequentDownwardIntramanualCouplersNo->SetValue(true);

	m_firstMIDINoteNumberSpin->SetValue(m_coupler->getFirstMidiNoteNumber());
	m_numberOfKeysSpin->SetValue(m_coupler->getNumberOfKeys());
}

void CouplerPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void CouplerPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_nameField->SetToolTip(wxT("It's best to make the name unique. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_displayInvertedYes->SetToolTip(wxT("Yes means that the on and off bitmaps are reversed."));
		m_displayInvertedNo->SetToolTip(wxT("No is the standard on/off bitmap display."));
		m_functionChoice->SetToolTip(wxT("Any other function than 'Input' means that the user no longer has direct control over the state of this element, nor can its state be directly changed by a combination, instead it's controlled indirectly by the referenced switch(es).\n\nThe logical function Not can only reference one switch and negates (inverts) the state of the input. And, Xor, Nand, Nor as well as Or has a variable number of inputs.\n\nSee for instance https://en.wikipedia.org/wiki/Logic_gate for further explanations about logical functions."));
		m_defaultToEngagedYes->SetToolTip(wxT("The stop will be active by default. Only works if function is 'Input', otherwise the state is decided by switch(es)."));
		m_defaultToEngagedNo->SetToolTip(wxT("The stop is not active by default."));
		m_availableSwitches->SetToolTip(wxT("Switches available for referencing are listed here. If function is something else than 'Input' and there are switches available one or many can be selected for referencing."));
		m_referencedSwitches->SetToolTip(wxT("The switch(es) that should control the state of this stop should be listed here."));
		m_addReferencedSwitch->SetToolTip(wxT("The selected switch(es) from the left list will be referenced."));
		m_removeReferencedSwitch->SetToolTip(wxT("The selected switch(es) from the right list will be removed from this stop."));
		m_gcStateChoice->SetToolTip(wxT("Decides the state of this stop when 'General Cancel' is pushed. Only works for 'Input' function."));
		m_storeInDivisionalYes->SetToolTip(wxT("This stop is possible to store in divisionals."));
		m_storeInDivisionalNo->SetToolTip(wxT("This stop will not be stored in divisionals unless 'Full' is used to set it."));
		m_storeInGeneralYes->SetToolTip(wxT("This stop is possible to store in generals."));
		m_storeInGeneralNo->SetToolTip(wxT("This stop is not stored in generals unless 'Full' is used to set it."));
		m_unisonOffYes->SetToolTip(wxT("An unison off coupler decouples the manual from the stops (turn it into a floating manual)."));
		m_unisonOffNo->SetToolTip(wxT("This is the option for any 'normal' coupler."));
		m_destinationManualChoice->SetToolTip(wxT("Select the manual to couple to."));
		m_destinationKeyShiftSpin->SetToolTip(wxT("Adjust this value if the key to couple to should not be the same as the source key."));
		m_couplerTypeChoice->SetToolTip(wxT("Here the type of coupler can be changed."));
		m_coupleToSubsequentUnisonIntermanualCouplersYes->SetToolTip(wxT("Triggers further inter-manual coupler(s) with a destination key shift of zero."));
		m_coupleToSubsequentUnisonIntermanualCouplersNo->SetToolTip(wxT("Don't trigger further inter-manual coupler(s)."));
		m_coupleToSubsequentUpwardIntermanualCouplersYes->SetToolTip(wxT("Triggers further inter-manual coupler(s) with a destination key shift greater than zero."));
		m_coupleToSubsequentUpwardIntermanualCouplersNo->SetToolTip(wxT("Don't trigger further inter-manual coupler(s) upwards."));
		m_coupleToSubsequentDownwardIntermanualCouplersYes->SetToolTip(wxT("Triggers further inter-manual coupler(s) with a destination key shift less than zero."));
		m_coupleToSubsequentDownwardIntermanualCouplersNo->SetToolTip(wxT("Don't trigger further inter-manual coupler(s) downwards."));
		m_coupleToSubsequentUpwardIntramanualCouplersYes->SetToolTip(wxT("Triggers further intra-manual coupler(s) with a destination key shift greater than zero."));
		m_coupleToSubsequentUpwardIntramanualCouplersNo->SetToolTip(wxT("Don't trigger further intra-manual coupler(s) upwards."));
		m_coupleToSubsequentDownwardIntramanualCouplersYes->SetToolTip(wxT("Triggers further intra-manual coupler(s) with a destination key shift less than zero."));
		m_coupleToSubsequentDownwardIntramanualCouplersNo->SetToolTip(wxT("Don't trigger further intra-manual coupler(s) downwards."));
		m_firstMIDINoteNumberSpin->SetToolTip(wxT("First MIDI note number to forward."));
		m_numberOfKeysSpin->SetToolTip(wxT("Number of keys to forward starting with FirstMIDINoteNumber."));
	} else {
		m_nameField->SetToolTip(wxEmptyString);
		m_displayInvertedYes->SetToolTip(wxEmptyString);
		m_displayInvertedNo->SetToolTip(wxEmptyString);
		m_functionChoice->SetToolTip(wxEmptyString);
		m_defaultToEngagedYes->SetToolTip(wxEmptyString);
		m_defaultToEngagedNo->SetToolTip(wxEmptyString);
		m_availableSwitches->SetToolTip(wxEmptyString);
		m_referencedSwitches->SetToolTip(wxEmptyString);
		m_addReferencedSwitch->SetToolTip(wxEmptyString);
		m_removeReferencedSwitch->SetToolTip(wxEmptyString);
		m_gcStateChoice->SetToolTip(wxEmptyString);
		m_storeInDivisionalYes->SetToolTip(wxEmptyString);
		m_storeInDivisionalNo->SetToolTip(wxEmptyString);
		m_storeInGeneralYes->SetToolTip(wxEmptyString);
		m_storeInGeneralNo->SetToolTip(wxEmptyString);
		m_unisonOffYes->SetToolTip(wxEmptyString);
		m_unisonOffNo->SetToolTip(wxEmptyString);
		m_destinationManualChoice->SetToolTip(wxEmptyString);
		m_destinationKeyShiftSpin->SetToolTip(wxEmptyString);
		m_couplerTypeChoice->SetToolTip(wxEmptyString);
		m_coupleToSubsequentUnisonIntermanualCouplersYes->SetToolTip(wxEmptyString);
		m_coupleToSubsequentUnisonIntermanualCouplersNo->SetToolTip(wxEmptyString);
		m_coupleToSubsequentUpwardIntermanualCouplersYes->SetToolTip(wxEmptyString);
		m_coupleToSubsequentUpwardIntermanualCouplersNo->SetToolTip(wxEmptyString);
		m_coupleToSubsequentDownwardIntermanualCouplersYes->SetToolTip(wxEmptyString);
		m_coupleToSubsequentDownwardIntermanualCouplersNo->SetToolTip(wxEmptyString);
		m_coupleToSubsequentUpwardIntramanualCouplersYes->SetToolTip(wxEmptyString);
		m_coupleToSubsequentUpwardIntramanualCouplersNo->SetToolTip(wxEmptyString);
		m_coupleToSubsequentDownwardIntramanualCouplersYes->SetToolTip(wxEmptyString);
		m_coupleToSubsequentDownwardIntramanualCouplersNo->SetToolTip(wxEmptyString);
		m_firstMIDINoteNumberSpin->SetToolTip(wxEmptyString);
		m_numberOfKeysSpin->SetToolTip(wxEmptyString);
	}
}

void CouplerPanel::refreshData() {
	m_nameField->ChangeValue(m_coupler->getName());
}

void CouplerPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_coupler->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void CouplerPanel::OnDisplayInvertedRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_INVERTED_STATE_YES) {
		m_displayInvertedYes->SetValue(true);
		m_coupler->setDisplayInverted(true);
	} else {
		m_displayInvertedNo->SetValue(true);
		m_coupler->setDisplayInverted(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnFunctionChange(wxCommandEvent& WXUNUSED(event)) {
	wxString selectedText = m_functionChoice->GetString(m_functionChoice->GetSelection());
	m_coupler->setFunction(selectedText);
	if (m_coupler->getFunction().IsSameAs(wxT("Input"))) {
		if (m_coupler->getNumberOfSwitches()) {
			m_coupler->removeAllSwitchReferences();
			m_referencedSwitches->Clear();
		}
		wxArrayInt existingSelections;
		m_availableSwitches->GetSelections(existingSelections);
		if (!existingSelections.IsEmpty()) {
			for (unsigned i = 0; i < existingSelections.GetCount(); i++) {
				m_availableSwitches->Deselect(existingSelections[i]);
			}
		}
		m_availableSwitches->Enable(false);
		m_referencedSwitches->Enable(false);
		m_addReferencedSwitch->Enable(false);
		m_removeReferencedSwitch->Enable(false);
		m_defaultToEngagedYes->Enable(true);
		m_defaultToEngagedNo->Enable(true);
		m_gcStateChoice->Enable(true);
	} else {
		m_availableSwitches->Enable(true);
		m_referencedSwitches->Enable(true);
		if (m_coupler->getFunction().IsSameAs(wxT("NOT"), false) && m_coupler->getNumberOfSwitches() > 1) {
			while (m_coupler->getNumberOfSwitches() > 1) {
				m_coupler->removeSwitchReferenceAt(m_coupler->getNumberOfSwitches() - 1);
				UpdateReferencedSwitches();
			}
		}
		m_defaultToEngagedYes->Enable(false);
		m_defaultToEngagedNo->Enable(false);
		m_gcStateChoice->Enable(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnDefaultToEngagedChange(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_DEFAULT_TO_ENGAGED_YES) {
		m_defaultToEngagedYes->SetValue(true);
		m_coupler->setDefaultToEngaged(true);
	} else {
		m_defaultToEngagedNo->SetValue(true);
		m_coupler->setDefaultToEngaged(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnGcStateChange(wxCommandEvent& WXUNUSED(event)) {
	int gcValue = (int) m_gcStateChoice->GetSelection() - 1;
	m_coupler->setGcState(gcValue);
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnStoreInDivisionalChange(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_STORE_IN_DIV_YES) {
		m_storeInDivisionalYes->SetValue(true);
		m_coupler->setStoreInDivisional(true);
	} else {
		m_storeInDivisionalNo->SetValue(true);
		m_coupler->setStoreInDivisional(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnStoreInGeneralChange(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_STORE_IN_GEN_YES) {
		m_storeInGeneralYes->SetValue(true);
		m_coupler->setStoreInGeneral(true);
	} else {
		m_storeInGeneralNo->SetValue(true);
		m_coupler->setStoreInGeneral(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnUnisonOffRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_UNISON_OFF_YES) {
		m_unisonOffYes->SetValue(true);
		m_coupler->setUnisonOff(true);
	} else {
		m_unisonOffNo->SetValue(true);
		m_coupler->setUnisonOff(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnDestinationManualChoice(wxCommandEvent& WXUNUSED(event)) {
	m_coupler->setDestinationManual(::wxGetApp().m_frame->m_organ->getOrganManualAt(m_destinationManualChoice->GetCurrentSelection()));
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnDestinationKeyShiftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_coupler->setDestinationKeyshift(m_destinationKeyShiftSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnCouplerTypeChoice(wxCommandEvent& WXUNUSED(event)) {
	m_coupler->setCouplerType(m_couplerTypeChoice->GetString(m_couplerTypeChoice->GetSelection()));
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnCoupleToSubsequentUnisonIntermanualCouplers(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_SUBSEQUENT_UNISON_INTER_YES) {
		m_coupleToSubsequentUnisonIntermanualCouplersYes->SetValue(true);
		m_coupler->setCoupleToSubsequentUnisonIntermanualCouplers(true);
	} else {
		m_coupleToSubsequentUnisonIntermanualCouplersNo->SetValue(true);
		m_coupler->setCoupleToSubsequentUnisonIntermanualCouplers(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnCoupleToSubsequentUpwardIntermanualCouplers(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_SUBSEQUENT_UPWARD_INTER_YES) {
		m_coupleToSubsequentUpwardIntermanualCouplersYes->SetValue(true);
		m_coupler->setCoupleToSubsequentUpwardIntermanualCouplers(true);
	} else {
		m_coupleToSubsequentUpwardIntermanualCouplersNo->SetValue(true);
		m_coupler->setCoupleToSubsequentUpwardIntermanualCouplers(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnCoupleToSubsequentDownwardIntermanualCouplers(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_SUBSEQUENT_DOWNWARD_INTER_YES) {
		m_coupleToSubsequentDownwardIntermanualCouplersYes->SetValue(true);
		m_coupler->setCoupleToSubsequentDownwardIntermanualCouplers(true);
	} else {
		m_coupleToSubsequentDownwardIntermanualCouplersNo->SetValue(true);
		m_coupler->setCoupleToSubsequentDownwardIntermanualCouplers(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnCoupleToSubsequentUpwardIntramanualCouplers(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_SUBSEQUENT_UPWARD_INTRA_YES) {
		m_coupleToSubsequentUpwardIntramanualCouplersYes->SetValue(true);
		m_coupler->setCoupleToSubsequentUpwardIntramanualCouplers(true);
	} else {
		m_coupleToSubsequentUpwardIntramanualCouplersNo->SetValue(true);
		m_coupler->setCoupleToSubsequentUpwardIntramanualCouplers(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnCoupleToSubsequentDownwardIntramanualCouplers(wxCommandEvent& event) {
	if (event.GetId() == ID_COUPLER_SUBSEQUENT_DOWNWARD_INTRA_YES) {
		m_coupleToSubsequentDownwardIntramanualCouplersYes->SetValue(true);
		m_coupler->setCoupleToSubsequentDownwardIntramanualCouplers(true);
	} else {
		m_coupleToSubsequentDownwardIntramanualCouplersNo->SetValue(true);
		m_coupler->setCoupleToSubsequentDownwardIntramanualCouplers(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnFirstMidiNoteNumberSpin(wxSpinEvent& WXUNUSED(event)) {
	m_coupler->setFirstMidiNoteNumber(m_firstMIDINoteNumberSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnNumberOfKeysSpin(wxSpinEvent& WXUNUSED(event)) {
	m_coupler->setNumberOfKeys(m_numberOfKeysSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void CouplerPanel::OnRemoveCouplerBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this coupler?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			// reference to this coupler are removed from the manual and any generals
			// and when the organ remove this coupler any gui representations will be removed too
			::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
			m_isFirstRemoval = false;
		}
	} else {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void CouplerPanel::OnAddSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_availableSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		for (unsigned i = 0; i < selectedSwitches.GetCount(); i++) {
			if (!m_coupler->hasSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selectedSwitches[i])) &&
				(!(m_coupler->getFunction().IsSameAs(wxT("NOT"), false) && m_coupler->getNumberOfSwitches()))
			) {
				m_coupler->addSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selectedSwitches[i]));
			}
		}
		UpdateReferencedSwitches();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void CouplerPanel::OnRemoveSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_referencedSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		std::list<GoSwitch*> switchesToRemove;
		for (unsigned i = 0; i < selectedSwitches.GetCount(); i++) {
			switchesToRemove.push_back(m_coupler->getSwitchAtIndex(selectedSwitches[i]));
		}
		for (GoSwitch *sw : switchesToRemove) {
			m_coupler->removeSwitchReference(sw);
		}
		UpdateReferencedSwitches();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void CouplerPanel::OnSwitchListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_availableSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		m_addReferencedSwitch->Enable(true);
	}
}

void CouplerPanel::OnReferencedSwitchSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_referencedSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		m_removeReferencedSwitch->Enable(true);
	}
}

void CouplerPanel::UpdateReferencedSwitches() {
	wxArrayString referencedSwitches;
	unsigned nbSwitches = m_coupler->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			referencedSwitches.Add(m_coupler->getSwitchAtIndex(i)->getName());
		}
		m_referencedSwitches->Clear();
		m_referencedSwitches->InsertItems(referencedSwitches, 0);
	} else {
		m_referencedSwitches->Clear();
		m_removeReferencedSwitch->Disable();
	}
}
