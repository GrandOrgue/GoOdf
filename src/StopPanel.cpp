/*
 * StopPanel.cpp is part of GoOdf.
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

#include "StopPanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/msgdlg.h>

// Event table
BEGIN_EVENT_TABLE(StopPanel, wxPanel)
	EVT_TEXT(ID_STOP_NAME_TEXT, StopPanel::OnNameChange)
	EVT_CHOICE(ID_STOP_FUNCTION_CHOICE, StopPanel::OnFunctionChange)
	EVT_RADIOBUTTON(ID_STOP_INVERTED_STATE_YES, StopPanel::OnDisplayInvertedRadio)
	EVT_RADIOBUTTON(ID_STOP_INVERTED_STATE_NO, StopPanel::OnDisplayInvertedRadio)
	EVT_CHOICE(ID_STOP_GC_STATE_CHOICE, StopPanel::OnGcStateChange)
	EVT_RADIOBUTTON(ID_STOP_STORE_IN_DIV_YES, StopPanel::OnStoreInDivisionalChange)
	EVT_RADIOBUTTON(ID_STOP_STORE_IN_DIV_NO, StopPanel::OnStoreInDivisionalChange)
	EVT_RADIOBUTTON(ID_STOP_STORE_IN_GEN_YES, StopPanel::OnStoreInGeneralChange)
	EVT_RADIOBUTTON(ID_STOP_STORE_IN_GEN_NO, StopPanel::OnStoreInGeneralChange)
	EVT_RADIOBUTTON(ID_STOP_DEFAULT_TO_ENGAGED_YES, StopPanel::OnDefaultToEngagedChange)
	EVT_RADIOBUTTON(ID_STOP_DEFAULT_TO_ENGAGED_NO, StopPanel::OnDefaultToEngagedChange)
	EVT_BUTTON(ID_STOP_REMOVE_BTN, StopPanel::OnRemoveStopBtn)
	EVT_BUTTON(ID_STOP_ADD_SWITCH_BTN, StopPanel::OnAddSwitchReferenceBtn)
	EVT_BUTTON(ID_STOP_REMOVE_SWITCH_BTN, StopPanel::OnRemoveSwitchReferenceBtn)
	EVT_LISTBOX(ID_STOP_AVAILABLE_SWITCHES, StopPanel::OnSwitchListboxSelection)
	EVT_LISTBOX(ID_STOP_REFERENCED_SWITCHES, StopPanel::OnReferencedSwitchSelection)
	EVT_SPINCTRL(ID_STOP_FIRST_PIPE_LOGICAL_KEY_SPIN, StopPanel::OnFirstAccPipeLogKeyNbrChange)
	EVT_SPINCTRL(ID_STOP_ACCESSIBLE_PIPES_SPIN, StopPanel::OnNbrOfAccPipesChange)
	EVT_SPINCTRL(ID_STOP_FIRST_PIPE_LOGICAL_PIPE_SPIN, StopPanel::OnFirstAccPipeLogPipeNbrChange)
	EVT_RADIOBUTTON(ID_STOP_USE_INTERNAL_RANK_YES, StopPanel::OnUseInternalRankSelection)
	EVT_RADIOBUTTON(ID_STOP_USE_INTERNAL_RANK_NO, StopPanel::OnUseInternalRankSelection)
	EVT_NOTEBOOK_PAGE_CHANGED(ID_STOP_NOTEBOOK, StopPanel::OnNotebookPageChanged)
	EVT_LISTBOX(ID_STOP_AVAILABLE_RANKS, StopPanel::OnAvailableRanksSelection)
	EVT_BUTTON(ID_STOP_ADD_RANK_BTN, StopPanel::OnAddRankReferenceBtn)
	EVT_BUTTON(ID_STOP_REMOVE_RANK_BTN, StopPanel::OnRemoveRankReferenceBtn)
	EVT_LISTBOX(ID_STOP_REFERENCED_RANKS, StopPanel::OnReferencedRanksSelection)
	EVT_SPINCTRL(ID_STOP_FIRST_PIPE_NBR_SPIN, StopPanel::OnRankFirstPipeNumberSpin)
	EVT_SPINCTRL(ID_STOP_PIPE_COUNT_SPIN, StopPanel::OnRankPipeCountSpin)
	EVT_SPINCTRL(ID_STOP_FIRST_KEY_NBR_SPIN, StopPanel::OnRankFirstKeyNbrSpin)
END_EVENT_TABLE()

StopPanel::StopPanel(wxWindow *parent) : wxPanel(parent) {
	m_stop = NULL;
	m_isFirstRemoval = true;
	m_lastUsedPage = 0;
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

	wxBoxSizer *mainPanelSizer = new wxBoxSizer(wxVERTICAL);

	m_notebook = new wxNotebook(
		this,
		ID_STOP_NOTEBOOK
	);

	m_stopPanel = new wxPanel(m_notebook, wxID_ANY);
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *stopNameText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Stop name: ")
	);
	firstRow->Add(stopNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		m_stopPanel,
		ID_STOP_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *functionText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Function: ")
	);
	secondRow->Add(functionText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_functionChoice = new wxChoice(
		m_stopPanel,
		ID_STOP_FUNCTION_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		functionChoices
	);
	secondRow->Add(m_functionChoice, 0, wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *dispInvertedText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Display in inverted state: ")
	);
	secondRow->Add(dispInvertedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedYes = new wxRadioButton(
		m_stopPanel,
		ID_STOP_INVERTED_STATE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	secondRow->Add(m_displayInvertedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo = new wxRadioButton(
		m_stopPanel,
		ID_STOP_INVERTED_STATE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_displayInvertedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo->SetValue(true);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *thirdRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableReferencesText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Available switches")
	);
	thirdRow1stCol->Add(availableReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableSwitches = new wxListBox(
		m_stopPanel,
		ID_STOP_AVAILABLE_SWITCHES,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	thirdRow1stCol->Add(m_availableSwitches, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow1stCol, 1, wxEXPAND);
	wxBoxSizer *thirdRow2ndCol = new wxBoxSizer(wxVERTICAL);
	thirdRow2ndCol->AddStretchSpacer();
	m_addReferencedSwitch = new wxButton(
		m_stopPanel,
		ID_STOP_ADD_SWITCH_BTN,
		wxT("Add ->")
	);
	thirdRow2ndCol->Add(m_addReferencedSwitch, 0, wxGROW|wxALL, 5);
	m_removeReferencedSwitch = new wxButton(
		m_stopPanel,
		ID_STOP_REMOVE_SWITCH_BTN,
		wxT("<- Remove")
	);
	thirdRow2ndCol->Add(m_removeReferencedSwitch, 0, wxGROW|wxALL, 5);
	thirdRow2ndCol->AddStretchSpacer();
	thirdRow->Add(thirdRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *thirdRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenReferencesText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Referenced switches")
	);
	thirdRow3rdCol->Add(chosenReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedSwitches = new wxListBox(
		m_stopPanel,
		ID_STOP_REFERENCED_SWITCHES,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	thirdRow3rdCol->Add(m_referencedSwitches, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow3rdCol, 1, wxEXPAND);

	panelSizer->Add(thirdRow, 1, wxEXPAND);

	wxBoxSizer *forgottenRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *defaultToEngagedText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Default to engaged: ")
	);
	forgottenRow->Add(defaultToEngagedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedYes = new wxRadioButton(
		m_stopPanel,
		ID_STOP_DEFAULT_TO_ENGAGED_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_defaultToEngagedYes->SetValue(true);
	forgottenRow->Add(m_defaultToEngagedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedNo = new wxRadioButton(
		m_stopPanel,
		ID_STOP_DEFAULT_TO_ENGAGED_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	forgottenRow->Add(m_defaultToEngagedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(forgottenRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *gcStateText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("State when GC is activated: ")
	);
	fourthRow->Add(gcStateText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gcStateChoice = new wxChoice(
		m_stopPanel,
		ID_STOP_GC_STATE_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		gcStateChoices
	);
	fourthRow->Add(m_gcStateChoice, 0, wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeInDivisionalText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Store in divisional: ")
	);
	fifthRow->Add(storeInDivisionalText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInDivisionalYes = new wxRadioButton(
		m_stopPanel,
		ID_STOP_STORE_IN_DIV_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInDivisionalYes->SetValue(true);
	fifthRow->Add(m_storeInDivisionalYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInDivisionalNo = new wxRadioButton(
		m_stopPanel,
		ID_STOP_STORE_IN_DIV_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	fifthRow->Add(m_storeInDivisionalNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeInGeneralText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Store in generals: ")
	);
	sixthRow->Add(storeInGeneralText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralYes = new wxRadioButton(
		m_stopPanel,
		ID_STOP_STORE_IN_GEN_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInGeneralYes->SetValue(true);
	sixthRow->Add(m_storeInGeneralYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralNo = new wxRadioButton(
		m_stopPanel,
		ID_STOP_STORE_IN_GEN_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_storeInGeneralNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *firstLogicalKeyText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("First accessible pipe logical key number: ")
	);
	seventhRow->Add(firstLogicalKeyText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstAccessiblePipeLogicalKeyNumberSpin = new wxSpinCtrl(
		m_stopPanel,
		ID_STOP_FIRST_PIPE_LOGICAL_KEY_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		128,
		1
	);
	seventhRow->Add(m_firstAccessiblePipeLogicalKeyNumberSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(seventhRow, 0, wxGROW);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *accessiblePipesText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Number of accessible pipes: ")
	);
	eighthRow->Add(accessiblePipesText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_numberOfAccessiblePipesSpin = new wxSpinCtrl(
		m_stopPanel,
		ID_STOP_ACCESSIBLE_PIPES_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		192,
		1
	);
	eighthRow->Add(m_numberOfAccessiblePipesSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eighthRow, 0, wxGROW);

	wxBoxSizer *ninthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *firstLogicalPipeText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("First accessible pipe logical pipe number: ")
	);
	ninthRow->Add(firstLogicalPipeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstAccessiblePipeLogicalPipeNumberSpin = new wxSpinCtrl(
		m_stopPanel,
		ID_STOP_FIRST_PIPE_LOGICAL_PIPE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		192,
		1
	);
	ninthRow->Add(m_firstAccessiblePipeLogicalPipeNumberSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(ninthRow, 0, wxGROW);

	wxBoxSizer *tenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *useInternalRankText = new wxStaticText (
		m_stopPanel,
		wxID_STATIC,
		wxT("Use internal rank or references? ")
	);
	tenthRow->Add(useInternalRankText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_useInternalRankYes = new wxRadioButton(
		m_stopPanel,
		ID_STOP_USE_INTERNAL_RANK_YES,
		wxT("Internal rank"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInGeneralYes->SetValue(true);
	tenthRow->Add(m_useInternalRankYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_useInternalRankNo = new wxRadioButton(
		m_stopPanel,
		ID_STOP_USE_INTERNAL_RANK_NO,
		wxT("Referenced ranks"),
		wxDefaultPosition,
		wxDefaultSize
	);
	tenthRow->Add(m_useInternalRankNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(tenthRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(m_stopPanel);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeStopBtn = new wxButton(
		m_stopPanel,
		ID_STOP_REMOVE_BTN,
		wxT("Remove this stop")
	);
	bottomRow->Add(removeStopBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);
	m_stopPanel->SetSizer(panelSizer);

	m_notebook->AddPage(m_stopPanel, wxT("Stop properties"));

	m_internalRankPanel = new RankPanel(m_notebook);
	m_notebook->AddPage(m_internalRankPanel, wxT("Internal rank"));

	m_referencedRanksPanel = new wxPanel(m_notebook, wxID_ANY);
	wxBoxSizer *refPanelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *refP1stRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *refP1stRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableRanksText = new wxStaticText (
		m_referencedRanksPanel,
		wxID_STATIC,
		wxT("Available ranks")
	);
	refP1stRow1stCol->Add(availableRanksText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableRanks = new wxListBox(
		m_referencedRanksPanel,
		ID_STOP_AVAILABLE_RANKS
	);
	refP1stRow1stCol->Add(m_availableRanks, 1, wxEXPAND|wxALL, 5);
	refP1stRow->Add(refP1stRow1stCol, 1, wxEXPAND);
	wxBoxSizer *refP1stRow2ndCol = new wxBoxSizer(wxVERTICAL);
	refP1stRow2ndCol->AddStretchSpacer();
	m_addReferencedRank = new wxButton(
		m_referencedRanksPanel,
		ID_STOP_ADD_RANK_BTN,
		wxT("Add ->")
	);
	m_addReferencedRank->Disable();
	refP1stRow2ndCol->Add(m_addReferencedRank, 0, wxGROW|wxALL, 5);
	m_removeReferencedRank = new wxButton(
		m_referencedRanksPanel,
		ID_STOP_REMOVE_RANK_BTN,
		wxT("<- Remove")
	);
	m_removeReferencedRank->Disable();
	refP1stRow2ndCol->Add(m_removeReferencedRank, 0, wxGROW|wxALL, 5);
	refP1stRow2ndCol->AddStretchSpacer();
	refP1stRow->Add(refP1stRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *refP1stRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenRefRanksText = new wxStaticText (
		m_referencedRanksPanel,
		wxID_STATIC,
		wxT("Referenced ranks")
	);
	refP1stRow3rdCol->Add(chosenRefRanksText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedRanks = new wxListBox(
		m_referencedRanksPanel,
		ID_STOP_REFERENCED_RANKS
	);
	refP1stRow3rdCol->Add(m_referencedRanks, 1, wxEXPAND|wxALL, 5);
	refP1stRow->Add(refP1stRow3rdCol, 1, wxEXPAND);
	refPanelSizer->Add(refP1stRow, 1, wxEXPAND|wxALL, 5);

	wxBoxSizer *refP2ndRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *firstPipeNbrText = new wxStaticText (
		m_referencedRanksPanel,
		wxID_STATIC,
		wxT("First pipe number: ")
	);
	refP2ndRow->Add(firstPipeNbrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstPipeNumberSpin = new wxSpinCtrl(
		m_referencedRanksPanel,
		ID_STOP_FIRST_PIPE_NBR_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	m_firstPipeNumberSpin->Disable();
	refP2ndRow->Add(m_firstPipeNumberSpin, 0, wxEXPAND|wxALL, 5);
	refPanelSizer->Add(refP2ndRow, 0, wxGROW);

	wxBoxSizer *refP3rdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *pipeCountText = new wxStaticText (
		m_referencedRanksPanel,
		wxID_STATIC,
		wxT("Pipe count: ")
	);
	refP3rdRow->Add(pipeCountText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pipeCountSpin = new wxSpinCtrl(
		m_referencedRanksPanel,
		ID_STOP_PIPE_COUNT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		1
	);
	m_pipeCountSpin->Disable();
	refP3rdRow->Add(m_pipeCountSpin, 0, wxEXPAND|wxALL, 5);
	refPanelSizer->Add(refP3rdRow, 0, wxGROW);

	wxBoxSizer *refP4thRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *firstAccKeyNbrText = new wxStaticText (
		m_referencedRanksPanel,
		wxID_STATIC,
		wxT("First accessible key number: ")
	);
	refP4thRow->Add(firstAccKeyNbrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstAccessibleKeyNumberSpin = new wxSpinCtrl(
		m_referencedRanksPanel,
		ID_STOP_FIRST_KEY_NBR_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	m_firstAccessibleKeyNumberSpin->Disable();
	refP4thRow->Add(m_firstAccessibleKeyNumberSpin, 0, wxEXPAND|wxALL, 5);
	refPanelSizer->Add(refP4thRow, 0, wxGROW);

	m_referencedRanksPanel->SetSizer(refPanelSizer);
	m_notebook->AddPage(m_referencedRanksPanel, wxT("Referenced ranks"));

	mainPanelSizer->Add(m_notebook, 1, wxEXPAND);

	SetSizer(mainPanelSizer);
}

StopPanel::~StopPanel() {

}

void StopPanel::setStop(Stop *stop) {
	m_stop = stop;
	m_internalRankPanel->setRank(m_stop->getInternalRank());
	m_internalRankPanel->setNameFieldValue(m_stop->getName());
	m_internalRankPanel->disableNameFieldInput();
	m_nameField->ChangeValue(m_stop->getName());
	if (m_stop->isDisplayedInverted())
		m_displayInvertedYes->SetValue(true);
	else
		m_displayInvertedNo->SetValue(true);
	m_functionChoice->SetSelection(m_functionChoice->FindString(m_stop->getFunction()));
	if (m_stop->getFunction().IsSameAs(wxT("Input"))) {
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
	if (m_stop->isDefaultToEngaged())
		m_defaultToEngagedYes->SetValue(true);
	else
		m_defaultToEngagedNo->SetValue(true);
	m_gcStateChoice->SetSelection((unsigned) (m_stop->getGcState() + 1));
	if (m_stop->isStoreInDivisional())
		m_storeInDivisionalYes->SetValue(true);
	else
		m_storeInDivisionalNo->SetValue(true);
	if (m_stop->isStoreInGeneral())
		m_storeInGeneralYes->SetValue(true);
	else
		m_storeInGeneralNo->SetValue(true);
	m_firstAccessiblePipeLogicalKeyNumberSpin->SetValue(m_stop->getFirstPipeLogicalKeyNbr());
	m_numberOfAccessiblePipesSpin->SetValue(m_stop->getNumberOfAccessiblePipes());
	m_firstAccessiblePipeLogicalPipeNumberSpin->SetValue(m_stop->getFirstPipeLogicalPipeNbr());
	if (m_stop->isUsingInternalRank()) {
		m_useInternalRankYes->SetValue(true);
		m_useInternalRankNo->SetValue(false);
	} else {
		m_useInternalRankYes->SetValue(false);
		m_useInternalRankNo->SetValue(true);
	}

	// update/populate available switches
	wxArrayString organSwitches;
	unsigned nbSwitches = ::wxGetApp().m_frame->m_organ->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++)
			organSwitches.Add(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(i)->getName());

		m_availableSwitches->Clear();
		m_availableSwitches->InsertItems(organSwitches, 0);
	} else {
		if (m_availableSwitches->GetCount())
			m_availableSwitches->Clear();
	}

	UpdateReferencedSwitches();

	m_addReferencedSwitch->Disable();
	m_removeReferencedSwitch->Disable();

	// update/populate available ranks
	wxArrayString organRanks;
	unsigned nbRanks = ::wxGetApp().m_frame->m_organ->getNumberOfRanks();
	if (nbRanks > 0) {
		for (unsigned i = 0; i < nbRanks; i++)
			organRanks.Add(::wxGetApp().m_frame->m_organ->getOrganRankAt(i)->getName());

		m_availableRanks->Clear();
		m_availableRanks->InsertItems(organRanks, 0);
	} else {
		if (m_availableRanks->GetCount())
			m_availableRanks->Clear();
	}

	UpdateReferencedRanks();

	m_availableRanks->SetSelection(wxNOT_FOUND);
	m_referencedRanks->SetSelection(wxNOT_FOUND);
	m_addReferencedRank->Disable();
	m_removeReferencedRank->Disable();
	m_firstPipeNumberSpin->Disable();
	m_pipeCountSpin->Disable();
	m_firstAccessibleKeyNumberSpin->Disable();

	if ((m_stop->isUsingInternalRank() && m_lastUsedPage == 1) || (!m_stop->isUsingInternalRank() && m_lastUsedPage == 2)) {
		m_notebook->SetSelection(m_lastUsedPage);
	} else {
		m_notebook->SetSelection(0);
		m_lastUsedPage = m_notebook->GetSelection();
	}
}

Stop* StopPanel::getCurrentStop() {
	return m_stop;
}

void StopPanel::setTooltipsEnabled(bool isEnabled) {
	m_internalRankPanel->setTooltipsEnabled(isEnabled);

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
		m_firstAccessiblePipeLogicalKeyNumberSpin->SetToolTip(wxT("The first (directly accessible) pipe is played from this key number on the manual."));
		m_numberOfAccessiblePipesSpin->SetToolTip(wxT("Number of pipes accessible to play from a manual. Note that this value is calculated from the pipes added and rarely needs to be manually changed except for certain cases!"));
		m_firstAccessiblePipeLogicalPipeNumberSpin->SetToolTip(wxT("The number of the first pipe accessible from the manual."));
		m_useInternalRankYes->SetToolTip(wxT("This stop has a rank defined directly in itself."));
		m_useInternalRankNo->SetToolTip(wxT("This stop references external ranks."));
		m_availableRanks->SetToolTip(wxT("List of ranks possible to reference."));
		m_referencedRanks->SetToolTip(wxT("List of the rank(s) this stop actually references."));
		m_addReferencedRank->SetToolTip(wxT("The selected rank in the left list will be referenced by this stop."));
		m_removeReferencedRank->SetToolTip(wxT("The selected rank that the stop references will be removed from this stop."));
		m_firstPipeNumberSpin->SetToolTip(wxT("Number of the first pipe to use from the rank."));
		m_pipeCountSpin->SetToolTip(wxT("Number of pipes to use from the rank."));
		m_firstAccessibleKeyNumberSpin->SetToolTip(wxT("Key number offset (from FirstAccessiblePipeLogicalKeyNumber) for the pipe referenced by FirstPipeNumber."));
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
		m_firstAccessiblePipeLogicalKeyNumberSpin->SetToolTip(wxEmptyString);
		m_numberOfAccessiblePipesSpin->SetToolTip(wxEmptyString);
		m_firstAccessiblePipeLogicalPipeNumberSpin->SetToolTip(wxEmptyString);
		m_useInternalRankYes->SetToolTip(wxEmptyString);
		m_useInternalRankNo->SetToolTip(wxEmptyString);
		m_availableRanks->SetToolTip(wxEmptyString);
		m_referencedRanks->SetToolTip(wxEmptyString);
		m_addReferencedRank->SetToolTip(wxEmptyString);
		m_removeReferencedRank->SetToolTip(wxEmptyString);
		m_firstPipeNumberSpin->SetToolTip(wxEmptyString);
		m_pipeCountSpin->SetToolTip(wxEmptyString);
		m_firstAccessibleKeyNumberSpin->SetToolTip(wxEmptyString);
	}
}

void StopPanel::refreshData() {
	m_nameField->ChangeValue(m_stop->getName());
}

RankPanel* StopPanel::GetInternalRankPanel() {
	return m_internalRankPanel;
}

void StopPanel::internalRankLogicalPipesChanged(int value) {
	m_numberOfAccessiblePipesSpin->SetValue(value);
	m_stop->setNumberOfAccessiblePipes(value);
}

void StopPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void StopPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_stop->setName(m_nameField->GetValue());
	m_stop->getInternalRank()->setName(m_nameField->GetValue());
	m_internalRankPanel->setNameFieldValue(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void StopPanel::OnDisplayInvertedRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_STOP_INVERTED_STATE_YES) {
		m_displayInvertedYes->SetValue(true);
		m_stop->setDisplayInverted(true);
	} else {
		m_displayInvertedNo->SetValue(true);
		m_stop->setDisplayInverted(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnFunctionChange(wxCommandEvent& WXUNUSED(event)) {
	wxString selectedText = m_functionChoice->GetString(m_functionChoice->GetSelection());
	m_stop->setFunction(selectedText);
	if (m_stop->getFunction().IsSameAs(wxT("Input"))) {
		if (m_stop->getNumberOfSwitches()) {
			m_stop->removeAllSwitchReferences();
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
		if (m_stop->getFunction().IsSameAs(wxT("NOT"), false) && m_stop->getNumberOfSwitches() > 1) {
			while (m_stop->getNumberOfSwitches() > 1) {
				m_stop->removeSwitchReferenceAt(m_stop->getNumberOfSwitches() - 1);
				UpdateReferencedSwitches();
			}
		}
		m_defaultToEngagedYes->Enable(false);
		m_defaultToEngagedNo->Enable(false);
		m_gcStateChoice->Enable(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnDefaultToEngagedChange(wxCommandEvent& event) {
	if (event.GetId() == ID_STOP_DEFAULT_TO_ENGAGED_YES) {
		m_defaultToEngagedYes->SetValue(true);
		m_stop->setDefaultToEngaged(true);
	} else {
		m_defaultToEngagedNo->SetValue(true);
		m_stop->setDefaultToEngaged(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnGcStateChange(wxCommandEvent& WXUNUSED(event)) {
	int gcValue = (int) m_gcStateChoice->GetSelection() - 1;
	m_stop->setGcState(gcValue);
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnStoreInDivisionalChange(wxCommandEvent& event) {
	if (event.GetId() == ID_STOP_STORE_IN_DIV_YES) {
		m_storeInDivisionalYes->SetValue(true);
		m_stop->setStoreInDivisional(true);
	} else {
		m_storeInDivisionalNo->SetValue(true);
		m_stop->setStoreInDivisional(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnStoreInGeneralChange(wxCommandEvent& event) {
	if (event.GetId() == ID_STOP_STORE_IN_GEN_YES) {
		m_storeInGeneralYes->SetValue(true);
		m_stop->setStoreInGeneral(true);
	} else {
		m_storeInGeneralNo->SetValue(true);
		m_stop->setStoreInGeneral(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnFirstAccPipeLogKeyNbrChange(wxSpinEvent& WXUNUSED(event)) {
	m_stop->setFirstPipeLogicalKeyNbr(m_firstAccessiblePipeLogicalKeyNumberSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnNbrOfAccPipesChange(wxSpinEvent& WXUNUSED(event)) {
	m_stop->setNumberOfAccessiblePipes(m_numberOfAccessiblePipesSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnFirstAccPipeLogPipeNbrChange(wxSpinEvent& WXUNUSED(event)) {
	m_stop->setFirstPipeLogicalPipeNbr(m_firstAccessiblePipeLogicalPipeNumberSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnUseInternalRankSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_STOP_USE_INTERNAL_RANK_YES) {
		m_stop->setUsingInternalRank(true);
	} else {
		m_stop->setUsingInternalRank(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void StopPanel::OnRemoveStopBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this stop?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			// references to this stop are removed in manual and in any generals
			// and when the organ removes this stop any gui representations are removed too from panels
			::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
			m_isFirstRemoval = false;
		}
	} else {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void StopPanel::OnAddSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_availableSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		for (unsigned i = 0; i < selectedSwitches.GetCount(); i++) {
			if (!m_stop->hasSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selectedSwitches[i])) &&
				(!(m_stop->getFunction().IsSameAs(wxT("NOT"), false) && m_stop->getNumberOfSwitches()))
			) {
				m_stop->addSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selectedSwitches[i]));
			}
		}
		UpdateReferencedSwitches();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void StopPanel::OnRemoveSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_referencedSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		std::list<GoSwitch*> switchesToRemove;
		for (unsigned i = 0; i < selectedSwitches.GetCount(); i++) {
			switchesToRemove.push_back(m_stop->getSwitchAtIndex(selectedSwitches[i]));
		}
		for (GoSwitch *sw : switchesToRemove) {
			m_stop->removeSwitchReference(sw);
		}
		UpdateReferencedSwitches();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void StopPanel::OnSwitchListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_availableSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		m_addReferencedSwitch->Enable(true);
	}
}

void StopPanel::OnReferencedSwitchSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_referencedSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		m_removeReferencedSwitch->Enable(true);
	}
}

void StopPanel::UpdateReferencedSwitches() {
	wxArrayString referencedSwitches;
	unsigned nbSwitches = m_stop->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			referencedSwitches.Add(m_stop->getSwitchAtIndex(i)->getName());
		}
		m_referencedSwitches->Clear();
		m_referencedSwitches->InsertItems(referencedSwitches, 0);
	} else {
		m_referencedSwitches->Clear();
		m_removeReferencedSwitch->Disable();
	}
}

void StopPanel::OnNotebookPageChanged(wxBookCtrlEvent& event) {
	if ((m_useInternalRankYes->GetValue() && event.GetSelection() == 2) ||
		(m_useInternalRankNo->GetValue() && event.GetSelection() == 1)) {
		m_notebook->SetSelection(event.GetOldSelection());
	} else {
		m_lastUsedPage = m_notebook->GetSelection();
	}
	// Update stop FirstAccessiblePipeLogicalKeyNumber if necessary
	if (m_useInternalRankYes->GetValue() && m_stop) {
		int internalRankMidiNoteNbr = m_stop->getInternalRank()->getFirstMidiNoteNumber();
		int manualMidiNoteNbr = m_stop->getOwningManual()->getFirstAccessibleKeyMIDINoteNumber();
		int stopFirstPipeKeyNbr = m_stop->getFirstPipeLogicalKeyNbr();
		if (internalRankMidiNoteNbr > manualMidiNoteNbr) {
			int expectedFirstPipeKeyNbr = internalRankMidiNoteNbr - manualMidiNoteNbr + 1;
			if (expectedFirstPipeKeyNbr != stopFirstPipeKeyNbr) {
				m_stop->setFirstPipeLogicalKeyNbr(expectedFirstPipeKeyNbr);
				m_firstAccessiblePipeLogicalKeyNumberSpin->SetValue(expectedFirstPipeKeyNbr);
			}
		}
	}
}

void StopPanel::OnAvailableRanksSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableRanks->GetSelection() != wxNOT_FOUND)
		m_addReferencedRank->Enable(true);
}

void StopPanel::OnAddRankReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableRanks->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableRanks->GetSelection();
		if (!m_stop->hasRankReference(::wxGetApp().m_frame->m_organ->getOrganRankAt(selected))) {
			m_stop->addRankReference(::wxGetApp().m_frame->m_organ->getOrganRankAt(selected));

			// set pipe count from the rank logical pipes
			unsigned rankRefIdx = m_stop->getIndexOfRankReference(::wxGetApp().m_frame->m_organ->getOrganRankAt(selected));
			m_stop->getRankReferenceAt(rankRefIdx)->m_pipeCount = ::wxGetApp().m_frame->m_organ->getOrganRankAt(selected)->getNumberOfLogicalPipes();
			// also update accessible pipes if it's greater than before
			if (m_stop->getRankReferenceAt(rankRefIdx)->m_pipeCount > m_stop->getNumberOfAccessiblePipes()) {
				m_stop->setNumberOfAccessiblePipes(m_stop->getRankReferenceAt(rankRefIdx)->m_pipeCount);
				m_firstAccessibleKeyNumberSpin->SetRange(1, m_stop->getNumberOfAccessiblePipes());
				m_numberOfAccessiblePipesSpin->SetValue(m_stop->getNumberOfAccessiblePipes());
			}

			UpdateReferencedRanks();
			m_referencedRanks->SetSelection(m_stop->getNumberOfRanks() - 1);
			wxCommandEvent evt(wxEVT_LISTBOX, ID_STOP_REFERENCED_RANKS);
			wxPostEvent(this, evt);
			::wxGetApp().m_frame->m_organ->setModified(true);
		}
	}
}

void StopPanel::OnRemoveRankReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedRanks->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedRanks->GetSelection();
		m_stop->removeRankReference(m_stop->getRankAt(selected));
		UpdateReferencedRanks();

		// disable spin controls
		m_firstPipeNumberSpin->SetValue(1);
		m_pipeCountSpin->SetValue(1);
		m_firstAccessibleKeyNumberSpin->SetValue(1);
		m_firstPipeNumberSpin->Disable();
		m_pipeCountSpin->Disable();
		m_firstAccessibleKeyNumberSpin->Disable();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void StopPanel::OnReferencedRanksSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedRanks->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedRanks->GetSelection();
		m_removeReferencedRank->Enable(true);

		// first update range of spin controls
		m_firstPipeNumberSpin->SetRange(1, m_stop->getRankAt(selected)->getNumberOfLogicalPipes());
		m_pipeCountSpin->SetRange(0, m_stop->getRankAt(selected)->getNumberOfLogicalPipes() - (m_stop->getRankReferenceAt(selected)->m_firstPipeNumber - 1));
		m_firstAccessibleKeyNumberSpin->SetRange(1, m_stop->getNumberOfAccessiblePipes());

		// then set values and enable the spins
		m_firstPipeNumberSpin->SetValue(m_stop->getRankReferenceAt(selected)->m_firstPipeNumber);
		m_firstPipeNumberSpin->Enable(true);
		m_pipeCountSpin->SetValue(m_stop->getRankReferenceAt(selected)->m_pipeCount);
		m_pipeCountSpin->Enable(true);
		m_firstAccessibleKeyNumberSpin->SetValue(m_stop->getRankReferenceAt(selected)->m_firstAccessibleKeyNumber);
		m_firstAccessibleKeyNumberSpin->Enable(true);
	}
}

void StopPanel::OnRankFirstPipeNumberSpin(wxSpinEvent& WXUNUSED(event)) {
	if (m_referencedRanks->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedRanks->GetSelection();
		m_stop->getRankReferenceAt(selected)->m_firstPipeNumber = m_firstPipeNumberSpin->GetValue();

		// if first pipe is changed the available pipe count might have changed and needs to be adjusted
		int maxPipes = m_stop->getRankAt(selected)->getNumberOfLogicalPipes() - (m_stop->getRankReferenceAt(selected)->m_firstPipeNumber - 1);
		if (m_stop->getRankReferenceAt(selected)->m_pipeCount > maxPipes)
			m_stop->getRankReferenceAt(selected)->m_pipeCount = maxPipes;
		m_pipeCountSpin->SetRange(0, maxPipes);
		m_pipeCountSpin->SetValue(m_stop->getRankReferenceAt(selected)->m_pipeCount);
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void StopPanel::OnRankPipeCountSpin(wxSpinEvent& WXUNUSED(event)) {
	if (m_referencedRanks->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedRanks->GetSelection();
		m_stop->getRankReferenceAt(selected)->m_pipeCount = m_pipeCountSpin->GetValue();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void StopPanel::OnRankFirstKeyNbrSpin(wxSpinEvent& WXUNUSED(event)) {
	if (m_referencedRanks->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedRanks->GetSelection();
		m_stop->getRankReferenceAt(selected)->m_firstAccessibleKeyNumber = m_firstAccessibleKeyNumberSpin->GetValue();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void StopPanel::UpdateReferencedRanks() {
	wxArrayString referencedRanks;
	unsigned nbRanks = m_stop->getNumberOfRanks();
	if (nbRanks > 0) {
		for (unsigned i = 0; i < nbRanks; i++) {
			referencedRanks.Add(m_stop->getRankAt(i)->getName());
		}
		m_referencedRanks->Clear();
		m_referencedRanks->InsertItems(referencedRanks, 0);
	} else {
		m_referencedRanks->Clear();
		m_removeReferencedRank->Disable();
	}
}
