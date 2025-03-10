/*
 * TremulantPanel.cpp is part of GoOdf.
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

#include "TremulantPanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/msgdlg.h>

// Event table
BEGIN_EVENT_TABLE(TremulantPanel, wxPanel)
	EVT_TEXT(ID_TREMULANT_NAME_TEXT, TremulantPanel::OnNameChange)
	EVT_RADIOBUTTON(ID_TREMULANT_INVERTED_STATE_YES, TremulantPanel::OnDisplayInvertedRadio)
	EVT_RADIOBUTTON(ID_TREMULANT_INVERTED_STATE_NO, TremulantPanel::OnDisplayInvertedRadio)
	EVT_CHOICE(ID_TREMULANT_FUNCTION_CHOICE, TremulantPanel::OnFunctionChange)
	EVT_CHOICE(ID_TREM_GC_STATE_CHOICE, TremulantPanel::OnGcStateChange)
	EVT_RADIOBUTTON(ID_TREM_STORE_IN_DIV_YES, TremulantPanel::OnStoreInDivisionalChange)
	EVT_RADIOBUTTON(ID_TREM_STORE_IN_DIV_NO, TremulantPanel::OnStoreInDivisionalChange)
	EVT_RADIOBUTTON(ID_TREM_STORE_IN_GEN_YES, TremulantPanel::OnStoreInGeneralChange)
	EVT_RADIOBUTTON(ID_TREM_STORE_IN_GEN_NO, TremulantPanel::OnStoreInGeneralChange)
	EVT_CHOICE(ID_TREM_TYPE_CHOICE, TremulantPanel::OnTremTypeChange)
	EVT_SPINCTRL(ID_TREM_PERIOD_SPIN, TremulantPanel::OnPeriodChange)
	EVT_SPINCTRL(ID_TREM_START_RATE_SPIN, TremulantPanel::OnStartRateChange)
	EVT_SPINCTRL(ID_TREM_STOP_RATE_SPIN, TremulantPanel::OnStopRateChange)
	EVT_SPINCTRL(ID_TREM_AMP_MOD_DEPTH_SPIN, TremulantPanel::OnAmpModDepthChange)
	EVT_RADIOBUTTON(ID_TREM_DEFAULT_TO_ENGAGED_YES, TremulantPanel::OnDefaultToEngagedChange)
	EVT_RADIOBUTTON(ID_TREM_DEFAULT_TO_ENGAGED_NO, TremulantPanel::OnDefaultToEngagedChange)
	EVT_BUTTON(ID_TREM_REMOVE_BTN, TremulantPanel::OnRemoveTremulantBtn)
	EVT_BUTTON(ID_TREM_ADD_SWITCH_BTN, TremulantPanel::OnAddSwitchReferenceBtn)
	EVT_BUTTON(ID_TREM_REMOVE_SWITCH_BTN, TremulantPanel::OnRemoveSwitchReferenceBtn)
	EVT_LISTBOX(ID_TREM_AVAILABLE_SWITCHES, TremulantPanel::OnSwitchListboxSelection)
	EVT_LISTBOX(ID_TREM_REFERENCED_SWITCHES, TremulantPanel::OnReferencedSwitchSelection)
END_EVENT_TABLE()

TremulantPanel::TremulantPanel(wxWindow *parent) : wxPanel(parent) {
	m_tremulant = NULL;
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
	tremTypeChoices.Add(wxT("Synth"));
	tremTypeChoices.Add(wxT("Wave"));

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *tremulantNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Tremulant name: ")
	);
	firstRow->Add(tremulantNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_TREMULANT_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *functionText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Function: ")
	);
	secondRow->Add(functionText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_functionChoice = new wxChoice(
		this,
		ID_TREMULANT_FUNCTION_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		functionChoices
	);
	secondRow->Add(m_functionChoice, 0, wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *dispInvertedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Display in inverted state: ")
	);
	secondRow->Add(dispInvertedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedYes = new wxRadioButton(
		this,
		ID_TREMULANT_INVERTED_STATE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	secondRow->Add(m_displayInvertedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo = new wxRadioButton(
		this,
		ID_TREMULANT_INVERTED_STATE_NO,
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
		this,
		wxID_STATIC,
		wxT("Available switches")
	);
	thirdRow1stCol->Add(availableReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableSwitches = new wxListBox(
		this,
		ID_TREM_AVAILABLE_SWITCHES,
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
		this,
		ID_TREM_ADD_SWITCH_BTN,
		wxT("Add ->")
	);
	thirdRow2ndCol->Add(m_addReferencedSwitch, 0, wxGROW|wxALL, 5);
	m_removeReferencedSwitch = new wxButton(
		this,
		ID_TREM_REMOVE_SWITCH_BTN,
		wxT("<- Remove")
	);
	thirdRow2ndCol->Add(m_removeReferencedSwitch, 0, wxGROW|wxALL, 5);
	thirdRow2ndCol->AddStretchSpacer();
	thirdRow->Add(thirdRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *thirdRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced switches")
	);
	thirdRow3rdCol->Add(chosenReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedSwitches = new wxListBox(
		this,
		ID_TREM_REFERENCED_SWITCHES,
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
		this,
		wxID_STATIC,
		wxT("Default to engaged: ")
	);
	forgottenRow->Add(defaultToEngagedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedYes = new wxRadioButton(
		this,
		ID_TREM_DEFAULT_TO_ENGAGED_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_defaultToEngagedYes->SetValue(true);
	forgottenRow->Add(m_defaultToEngagedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedNo = new wxRadioButton(
		this,
		ID_TREM_DEFAULT_TO_ENGAGED_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	forgottenRow->Add(m_defaultToEngagedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(forgottenRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *gcStateText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("State when GC is activated: ")
	);
	fourthRow->Add(gcStateText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gcStateChoice = new wxChoice(
		this,
		ID_TREM_GC_STATE_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		gcStateChoices
	);
	fourthRow->Add(m_gcStateChoice, 0, wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeInDivisionalText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Store in divisional: ")
	);
	fifthRow->Add(storeInDivisionalText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInDivisionalYes = new wxRadioButton(
		this,
		ID_TREM_STORE_IN_DIV_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInDivisionalYes->SetValue(true);
	fifthRow->Add(m_storeInDivisionalYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInDivisionalNo = new wxRadioButton(
		this,
		ID_TREM_STORE_IN_DIV_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	fifthRow->Add(m_storeInDivisionalNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeInGeneralText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Store in generals: ")
	);
	sixthRow->Add(storeInGeneralText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralYes = new wxRadioButton(
		this,
		ID_TREM_STORE_IN_GEN_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInGeneralYes->SetValue(true);
	sixthRow->Add(m_storeInGeneralYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralNo = new wxRadioButton(
		this,
		ID_TREM_STORE_IN_GEN_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_storeInGeneralNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *tremTypeText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Type of tremulant: ")
	);
	seventhRow->Add(tremTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_tremTypeChoice = new wxChoice(
		this,
		ID_TREM_TYPE_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		tremTypeChoices
	);
	seventhRow->Add(m_tremTypeChoice, 0, wxALL, 5);
	panelSizer->Add(seventhRow, 0, wxGROW);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *periodText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Period: ")
	);
	eighthRow->Add(periodText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_periodSpin = new wxSpinCtrl(
		this,
		ID_TREM_PERIOD_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		32,
		44100,
		160
	);
	eighthRow->Add(m_periodSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eighthRow, 0, wxGROW);

	wxBoxSizer *ninthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *startRateText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Start rate: ")
	);
	ninthRow->Add(startRateText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_startRateSpin = new wxSpinCtrl(
		this,
		ID_TREM_START_RATE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		100,
		8
	);
	ninthRow->Add(m_startRateSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(ninthRow, 0, wxGROW);

	wxBoxSizer *tenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *stopRateText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Stop rate: ")
	);
	tenthRow->Add(stopRateText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_stopRateSpin = new wxSpinCtrl(
		this,
		ID_TREM_STOP_RATE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		100,
		8
	);
	tenthRow->Add(m_stopRateSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(tenthRow, 0, wxGROW);

	wxBoxSizer *eleventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *ampModDepthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Amplitude modulation depth: ")
	);
	eleventhRow->Add(ampModDepthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_ampModDepthSpin = new wxSpinCtrl(
		this,
		ID_TREM_AMP_MOD_DEPTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		100,
		18
	);
	eleventhRow->Add(m_ampModDepthSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eleventhRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeTremulantBtn = new wxButton(
		this,
		ID_TREM_REMOVE_BTN,
		wxT("Remove this tremulant")
	);
	bottomRow->Add(removeTremulantBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

TremulantPanel::~TremulantPanel() {

}

void TremulantPanel::setTremulant(Tremulant *tremulant) {
	m_tremulant = tremulant;
	m_nameField->ChangeValue(m_tremulant->getName());
	if (m_tremulant->isDisplayedInverted())
		m_displayInvertedYes->SetValue(true);
	else
		m_displayInvertedNo->SetValue(true);
	m_functionChoice->SetSelection(m_functionChoice->FindString(m_tremulant->getFunction()));
	if (m_tremulant->getFunction().IsSameAs(wxT("Input"))) {
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
	if (m_tremulant->isDefaultToEngaged())
		m_defaultToEngagedYes->SetValue(true);
	else
		m_defaultToEngagedNo->SetValue(true);
	m_gcStateChoice->SetSelection((unsigned) (m_tremulant->getGcState() + 1));
	if (m_tremulant->isStoreInDivisional())
		m_storeInDivisionalYes->SetValue(true);
	else
		m_storeInDivisionalNo->SetValue(true);
	if (m_tremulant->isStoreInGeneral())
		m_storeInGeneralYes->SetValue(true);
	else
		m_storeInGeneralNo->SetValue(true);
	m_tremTypeChoice->SetSelection(m_tremTypeChoice->FindString(m_tremulant->getTremType()));
	m_periodSpin->SetValue(m_tremulant->getPeriod());
	m_startRateSpin->SetValue(m_tremulant->getStartRate());
	m_stopRateSpin->SetValue(m_tremulant->getStopRate());
	m_ampModDepthSpin->SetValue(m_tremulant->getAmpModDepth());
	if (m_tremTypeChoice->GetSelection() == 1) {
		m_periodSpin->Enable(false);
		m_startRateSpin->Enable(false);
		m_stopRateSpin->Enable(false);
		m_ampModDepthSpin->Enable(false);
	} else {
		m_periodSpin->Enable(true);
		m_startRateSpin->Enable(true);
		m_stopRateSpin->Enable(true);
		m_ampModDepthSpin->Enable(true);
	}

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
}

void TremulantPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void TremulantPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_nameField->SetToolTip(wxT("It's best to make the name unique. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_displayInvertedYes->SetToolTip(wxT("Yes means that the on and off bitmaps are reversed."));
		m_displayInvertedNo->SetToolTip(wxT("No is the standard on/off bitmap display."));
		m_functionChoice->SetToolTip(wxT("Any other function than 'Input' means that the user no longer has direct control over the state of this element, nor can its state be directly changed by a combination, instead it's controlled indirectly by the referenced switch(es).\n\nThe logical function Not can only reference one switch and negates (inverts) the state of the input. And, Xor, Nand, Nor as well as Or has a variable number of inputs.\n\nSee for instance https://en.wikipedia.org/wiki/Logic_gate for further explanations about logical functions."));
		m_defaultToEngagedYes->SetToolTip(wxT("The tremulant will be active by default. Only works if function is 'Input', otherwise the state is decided by switch(es)."));
		m_defaultToEngagedNo->SetToolTip(wxT("The tremulant is not active by default."));
		m_availableSwitches->SetToolTip(wxT("Switches available for referencing are listed here. If function is something else than 'Input' and there are switches available one or many can be selected for referencing."));
		m_referencedSwitches->SetToolTip(wxT("The switch(es) that should control the state of this tremulant should be listed here."));
		m_addReferencedSwitch->SetToolTip(wxT("The selected switch(es) from the left list will be referenced."));
		m_removeReferencedSwitch->SetToolTip(wxT("The selected switch(es) from the right list will be removed from this tremulant."));
		m_gcStateChoice->SetToolTip(wxT("Decides the state of this tremulant when 'General Cancel' is pushed. Only works for 'Input' function."));
		m_storeInDivisionalYes->SetToolTip(wxT("This tremulant is possible to store in divisionals."));
		m_storeInDivisionalNo->SetToolTip(wxT("This tremulant will not be stored in divisionals unless 'Full' is used to set it."));
		m_storeInGeneralYes->SetToolTip(wxT("This tremulant is possible to store in generals."));
		m_storeInGeneralNo->SetToolTip(wxT("This tremulant is not stored in generals unless 'Full' is used to set it."));
		m_tremTypeChoice->SetToolTip(wxT("The type of the tremulant is selected here. Synth (synthesized tremulant) and Wave (tremulant based on different wave samples that needs to be provided for the pipe(s))."));
		m_periodSpin->SetToolTip(wxT("Period in ms. Only used for a 'Synth' tremulant. Lower value means a faster tremulant speed."));
		m_startRateSpin->SetToolTip(wxT("Startup time of the tremulant. Only used for a 'Synth' tremulant."));
		m_stopRateSpin->SetToolTip(wxT("Stop time of the tremulant. Only used for a 'Synth' tremulant."));
		m_ampModDepthSpin->SetToolTip(wxT("How much the amplitude will be modulated. Only used for a 'Synth' tremulant. A higher value means greater modulation."));
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
		m_tremTypeChoice->SetToolTip(wxEmptyString);
		m_periodSpin->SetToolTip(wxEmptyString);
		m_startRateSpin->SetToolTip(wxEmptyString);
		m_stopRateSpin->SetToolTip(wxEmptyString);
		m_ampModDepthSpin->SetToolTip(wxEmptyString);
	}
}

void TremulantPanel::refreshData() {
	m_nameField->ChangeValue(m_tremulant->getName());
}

void TremulantPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_tremulant->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void TremulantPanel::OnDisplayInvertedRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_TREMULANT_INVERTED_STATE_YES) {
		m_displayInvertedYes->SetValue(true);
		m_tremulant->setDisplayInverted(true);
	} else {
		m_displayInvertedNo->SetValue(true);
		m_tremulant->setDisplayInverted(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnFunctionChange(wxCommandEvent& WXUNUSED(event)) {
	wxString selectedText = m_functionChoice->GetString(m_functionChoice->GetSelection());
	m_tremulant->setFunction(selectedText);
	if (m_tremulant->getFunction().IsSameAs(wxT("Input"))) {
		if (m_tremulant->getNumberOfSwitches()) {
			m_tremulant->removeAllSwitchReferences();
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
		if (m_tremulant->getFunction().IsSameAs(wxT("NOT"), false) && m_tremulant->getNumberOfSwitches() > 1) {
			while (m_tremulant->getNumberOfSwitches() > 1) {
				m_tremulant->removeSwitchReferenceAt(m_tremulant->getNumberOfSwitches() - 1);
				UpdateReferencedSwitches();
			}
		}
		m_defaultToEngagedYes->Enable(false);
		m_defaultToEngagedNo->Enable(false);
		m_gcStateChoice->Enable(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnDefaultToEngagedChange(wxCommandEvent& event) {
	if (event.GetId() == ID_TREM_DEFAULT_TO_ENGAGED_YES) {
		m_defaultToEngagedYes->SetValue(true);
		m_tremulant->setDefaultToEngaged(true);
	} else {
		m_defaultToEngagedNo->SetValue(true);
		m_tremulant->setDefaultToEngaged(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnGcStateChange(wxCommandEvent& WXUNUSED(event)) {
	int gcValue = (int) m_gcStateChoice->GetSelection() - 1;
	m_tremulant->setGcState(gcValue);
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnStoreInDivisionalChange(wxCommandEvent& event) {
	if (event.GetId() == ID_TREM_STORE_IN_DIV_YES) {
		m_storeInDivisionalYes->SetValue(true);
		m_tremulant->setStoreInDivisional(true);
	} else {
		m_storeInDivisionalNo->SetValue(true);
		m_tremulant->setStoreInDivisional(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnStoreInGeneralChange(wxCommandEvent& event) {
	if (event.GetId() == ID_TREM_STORE_IN_GEN_YES) {
		m_storeInGeneralYes->SetValue(true);
		m_tremulant->setStoreInGeneral(true);
	} else {
		m_storeInGeneralNo->SetValue(true);
		m_tremulant->setStoreInGeneral(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnTremTypeChange(wxCommandEvent& WXUNUSED(event)) {
	wxString selectedText = m_tremTypeChoice->GetString(m_tremTypeChoice->GetSelection());
	m_tremulant->setTremType(selectedText);
	if (selectedText.IsSameAs(wxT("Wave"))) {
		m_periodSpin->Enable(false);
		m_startRateSpin->Enable(false);
		m_stopRateSpin->Enable(false);
		m_ampModDepthSpin->Enable(false);
	} else {
		m_periodSpin->Enable(true);
		m_startRateSpin->Enable(true);
		m_stopRateSpin->Enable(true);
		m_ampModDepthSpin->Enable(true);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnPeriodChange(wxSpinEvent& WXUNUSED(event)) {
	m_tremulant->setPeriod(m_periodSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnStartRateChange(wxSpinEvent& WXUNUSED(event)) {
	m_tremulant->setStartRate(m_startRateSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnStopRateChange(wxSpinEvent& WXUNUSED(event)) {
	m_tremulant->setStopRate(m_stopRateSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnAmpModDepthChange(wxSpinEvent& WXUNUSED(event)) {
	m_tremulant->setAmpModDepth(m_ampModDepthSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void TremulantPanel::OnRemoveTremulantBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this tremulant?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			DoRemoveTremulant();
			m_isFirstRemoval = false;
		}
	} else {
		DoRemoveTremulant();
	}
}

void TremulantPanel::DoRemoveTremulant() {
	// remove all possible references to this tremulant in any windchest first
	unsigned numberOfWindchests = ::wxGetApp().m_frame->m_organ->getNumberOfWindchestgroups();
	for (unsigned i = 0; i < numberOfWindchests; i++) {
		if (::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(i)->hasTremulantReference(m_tremulant))
			::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(i)->removeTremulantReference(m_tremulant);
	}
	// also remove any references from the manuals
	unsigned numberOfManuals = ::wxGetApp().m_frame->m_organ->getNumberOfManuals();
	for (unsigned i = 0; i < numberOfManuals; i++) {
		if (::wxGetApp().m_frame->m_organ->getOrganManualAt(i)->hasTremulantReference(m_tremulant)) {
			::wxGetApp().m_frame->m_organ->getOrganManualAt(i)->removeTremulant(m_tremulant);
		}
	}
	// if any panel has a GUI element for this tremulant it should be removed too
	unsigned numberOfPanels = ::wxGetApp().m_frame->m_organ->getNumberOfPanels();
	for (unsigned i = 0; i < numberOfPanels; i++) {
		if (::wxGetApp().m_frame->m_organ->getOrganPanelAt(i)->hasItemAsGuiElement(m_tremulant)) {
			::wxGetApp().m_frame->m_organ->getOrganPanelAt(i)->removeItemFromPanel(m_tremulant);
			::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
		}
	}
	// the tremulant can also exist in generals
	unsigned numberOfGenerals = ::wxGetApp().m_frame->m_organ->getNumberOfGenerals();
	for (unsigned i = 0; i < numberOfGenerals; i++) {
		if (::wxGetApp().m_frame->m_organ->getOrganGeneralAt(i)->hasTremulant(m_tremulant)) {
			::wxGetApp().m_frame->m_organ->getOrganGeneralAt(i)->removeTremulant(m_tremulant);
		}
	}
	// then remove this tremulant
	::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
}

void TremulantPanel::OnAddSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_availableSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		for (unsigned i = 0; i < selectedSwitches.GetCount(); i++) {
			if (!m_tremulant->hasSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selectedSwitches[i])) &&
				(!(m_tremulant->getFunction().IsSameAs(wxT("NOT"), false) && m_tremulant->getNumberOfSwitches()))
			) {
				m_tremulant->addSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selectedSwitches[i]));
			}
		}
		UpdateReferencedSwitches();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void TremulantPanel::OnRemoveSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_referencedSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		std::list<GoSwitch*> switchesToRemove;
		for (unsigned i = 0; i < selectedSwitches.GetCount(); i++) {
			switchesToRemove.push_back(m_tremulant->getSwitchAtIndex(selectedSwitches[i]));
		}
		for (GoSwitch *sw : switchesToRemove) {
			m_tremulant->removeSwitchReference(sw);
		}
		UpdateReferencedSwitches();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void TremulantPanel::OnSwitchListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_availableSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		m_addReferencedSwitch->Enable(true);
	}
}

void TremulantPanel::OnReferencedSwitchSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_referencedSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		m_removeReferencedSwitch->Enable(true);
	}
}

void TremulantPanel::UpdateReferencedSwitches() {
	wxArrayString referencedSwitches;
	unsigned nbSwitches = m_tremulant->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			referencedSwitches.Add(m_tremulant->getSwitchAtIndex(i)->getName());
		}
		m_referencedSwitches->Clear();
		m_referencedSwitches->InsertItems(referencedSwitches, 0);
	} else {
		m_referencedSwitches->Clear();
		m_removeReferencedSwitch->Disable();
	}
}
