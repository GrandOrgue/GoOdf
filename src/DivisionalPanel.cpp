/*
 * DivisionalPanel.cpp is part of GoOdf.
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

#include "DivisionalPanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/msgdlg.h>

// Event table
BEGIN_EVENT_TABLE(DivisionalPanel, wxPanel)
	EVT_TEXT(ID_DIVISIONAL_NAME_TEXT, DivisionalPanel::OnNameChange)
	EVT_RADIOBUTTON(ID_DIVISIONAL_INVERTED_STATE_YES, DivisionalPanel::OnDisplayInvertedRadio)
	EVT_RADIOBUTTON(ID_DIVISIONAL_INVERTED_STATE_NO, DivisionalPanel::OnDisplayInvertedRadio)
	EVT_RADIOBUTTON(ID_DIVISIONAL_PROTECTED_YES, DivisionalPanel::OnProtectedSelection)
	EVT_RADIOBUTTON(ID_DIVISIONAL_PROTECTED_NO, DivisionalPanel::OnProtectedSelection)
	EVT_BUTTON(ID_DIVISIONAL_ADD_STOP_BTN, DivisionalPanel::OnAddStopReferenceBtn)
	EVT_BUTTON(ID_DIVISIONAL_ADD_ALL_STOPS_BTN, DivisionalPanel::OnAddAllAvailableStopsBtn)
	EVT_BUTTON(ID_DIVISIONAL_REMOVE_STOP_BTN, DivisionalPanel::OnRemoveStopReferenceBtn)
	EVT_LISTBOX(ID_DIVISIONAL_AVAILABLE_STOPS, DivisionalPanel::OnStopListboxSelection)
	EVT_LISTBOX(ID_DIVISIONAL_REFERENCED_STOPS, DivisionalPanel::OnReferencedStopSelection)
	EVT_BUTTON(ID_DIVISIONAL_ADD_COUPLER_BTN, DivisionalPanel::OnAddCouplerReferenceBtn)
	EVT_BUTTON(ID_DIVISIONAL_ADD_ALL_COUPLERS_BTN, DivisionalPanel::OnAddAllAvailableCouplersBtn)
	EVT_BUTTON(ID_DIVISIONAL_REMOVE_COUPLER_BTN, DivisionalPanel::OnRemoveCouplerReferenceBtn)
	EVT_LISTBOX(ID_DIVISIONAL_AVAILABLE_COUPLERS, DivisionalPanel::OnCouplerListboxSelection)
	EVT_LISTBOX(ID_DIVISIONAL_REFERENCED_COUPLERS, DivisionalPanel::OnReferencedCouplerSelection)
	EVT_BUTTON(ID_DIVISIONAL_ADD_TREM_BTN, DivisionalPanel::OnAddTremulantReferenceBtn)
	EVT_BUTTON(ID_DIVISIONAL_ADD_ALL_TREMS_BTN, DivisionalPanel::OnAddAllAvailableTremulantsBtn)
	EVT_BUTTON(ID_DIVISIONAL_REMOVE_TREM_BTN, DivisionalPanel::OnRemoveTremulantReferenceBtn)
	EVT_LISTBOX(ID_DIVISIONAL_AVAILABLE_TREMS, DivisionalPanel::OnTremulantListboxSelection)
	EVT_LISTBOX(ID_DIVISIONAL_REFERENCED_TREMS, DivisionalPanel::OnReferencedTremulantSelection)
	EVT_BUTTON(ID_DIVISIONAL_ADD_SWITCH_BTN, DivisionalPanel::OnAddSwitchReferenceBtn)
	EVT_BUTTON(ID_DIVISIONAL_ADD_ALL_SWITCHES_BTN, DivisionalPanel::OnAddAllAvailableSwitchesBtn)
	EVT_BUTTON(ID_DIVISIONAL_REMOVE_SWITCH_BTN, DivisionalPanel::OnRemoveSwitchReferenceBtn)
	EVT_LISTBOX(ID_DIVISIONAL_AVAILABLE_SWITCHES, DivisionalPanel::OnSwitchListboxSelection)
	EVT_LISTBOX(ID_DIVISIONAL_REFERENCED_SWITCHES, DivisionalPanel::OnReferencedSwitchSelection)
	EVT_RADIOBUTTON(ID_DIVISIONAL_STOP_ON_RADIO, DivisionalPanel::OnStopOnOffRadioBtns)
	EVT_RADIOBUTTON(ID_DIVISIONAL_STOP_OFF_RADIO, DivisionalPanel::OnStopOnOffRadioBtns)
	EVT_RADIOBUTTON(ID_DIVISIONAL_COUPLER_ON_RADIO, DivisionalPanel::OnCouplerOnOffRadioBtns)
	EVT_RADIOBUTTON(ID_DIVISIONAL_COUPLER_OFF_RADIO, DivisionalPanel::OnCouplerOnOffRadioBtns)
	EVT_RADIOBUTTON(ID_DIVISIONAL_TREM_ON_RADIO, DivisionalPanel::OnTremOnOffRadioBtns)
	EVT_RADIOBUTTON(ID_DIVISIONAL_TREM_OFF_RADIO, DivisionalPanel::OnTremOnOffRadioBtns)
	EVT_RADIOBUTTON(ID_DIVISIONAL_SWITCH_ON_RADIO, DivisionalPanel::OnSwitchOnOffRadioBtns)
	EVT_RADIOBUTTON(ID_DIVISIONAL_SWITCH_OFF_RADIO, DivisionalPanel::OnSwitchOnOffRadioBtns)
	EVT_BUTTON(ID_DIVISIONAL_REMOVE_BTN, DivisionalPanel::OnRemoveDivisionalBtn)
END_EVENT_TABLE()

DivisionalPanel::DivisionalPanel(wxWindow *parent) : wxPanel(parent) {
	m_divisional = NULL;
	m_isFirstRemoval = true;

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *divisionalNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Divisional name: ")
	);
	firstRow->Add(divisionalNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_DIVISIONAL_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *divIsProtectedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Divisional combination is protected: ")
	);
	secondRow->Add(divIsProtectedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isProtectedYes = new wxRadioButton(
		this,
		ID_DIVISIONAL_PROTECTED_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	secondRow->Add(m_isProtectedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isProtectedNo = new wxRadioButton(
		this,
		ID_DIVISIONAL_PROTECTED_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_isProtectedNo->SetValue(true);
	secondRow->Add(m_isProtectedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *dispInvertedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Display in inverted state: ")
	);
	secondRow->Add(dispInvertedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedYes = new wxRadioButton(
		this,
		ID_DIVISIONAL_INVERTED_STATE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	secondRow->Add(m_displayInvertedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo = new wxRadioButton(
		this,
		ID_DIVISIONAL_INVERTED_STATE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_displayInvertedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo->SetValue(true);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *thirdRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableStopsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available stops")
	);
	thirdRow1stCol->Add(availableStopsText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableStops = new wxListBox(
		this,
		ID_DIVISIONAL_AVAILABLE_STOPS
	);
	thirdRow1stCol->Add(m_availableStops, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow1stCol, 1, wxEXPAND);
	wxBoxSizer *thirdRow2ndCol = new wxBoxSizer(wxVERTICAL);
	thirdRow2ndCol->AddStretchSpacer();
	m_addReferencedStop = new wxButton(
		this,
		ID_DIVISIONAL_ADD_STOP_BTN,
		wxT("Add ->")
	);
	thirdRow2ndCol->Add(m_addReferencedStop, 0, wxGROW|wxALL, 5);
	m_addAllAvailableStops = new wxButton(
		this,
		ID_DIVISIONAL_ADD_ALL_STOPS_BTN,
		wxT("Add all")
	);
	thirdRow2ndCol->Add(m_addAllAvailableStops, 0, wxGROW|wxALL, 5);
	m_removeReferencedStop = new wxButton(
		this,
		ID_DIVISIONAL_REMOVE_STOP_BTN,
		wxT("<- Remove")
	);
	thirdRow2ndCol->Add(m_removeReferencedStop, 0, wxGROW|wxALL, 5);
	thirdRow2ndCol->AddStretchSpacer();
	thirdRow->Add(thirdRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *thirdRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenStopsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced stops")
	);
	thirdRow3rdCol->Add(chosenStopsText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedStops = new wxListBox(
		this,
		ID_DIVISIONAL_REFERENCED_STOPS
	);
	thirdRow3rdCol->Add(m_referencedStops, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow3rdCol, 1, wxEXPAND);
	wxBoxSizer *thirdRow4thCol = new wxBoxSizer(wxVERTICAL);
	thirdRow4thCol->AddStretchSpacer();
	wxStaticText *stopOnOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Selected stop is:")
	);
	thirdRow4thCol->Add(stopOnOffText, 0, wxALL, 5);
	wxBoxSizer *thirdRow4thColRadios = new wxBoxSizer(wxHORIZONTAL);
	m_currentStopIsOn = new wxRadioButton(
		this,
		ID_DIVISIONAL_STOP_ON_RADIO,
		wxT("On"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	thirdRow4thColRadios->Add(m_currentStopIsOn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_currentStopIsOff = new wxRadioButton(
		this,
		ID_DIVISIONAL_STOP_OFF_RADIO,
		wxT("Off"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_currentStopIsOff->SetValue(true);
	thirdRow4thColRadios->Add(m_currentStopIsOff, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	thirdRow4thCol->Add(thirdRow4thColRadios, 0, wxEXPAND);
	thirdRow4thCol->AddStretchSpacer();
	thirdRow->Add(thirdRow4thCol, 0, wxEXPAND);
	panelSizer->Add(thirdRow, 1, wxEXPAND);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *fourthRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableCouplersText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available couplers")
	);
	fourthRow1stCol->Add(availableCouplersText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableCouplers = new wxListBox(
		this,
		ID_DIVISIONAL_AVAILABLE_COUPLERS
	);
	fourthRow1stCol->Add(m_availableCouplers, 1, wxEXPAND|wxALL, 5);
	fourthRow->Add(fourthRow1stCol, 1, wxEXPAND);
	wxBoxSizer *fourthRow2ndCol = new wxBoxSizer(wxVERTICAL);
	fourthRow2ndCol->AddStretchSpacer();
	m_addReferencedCoupler = new wxButton(
		this,
		ID_DIVISIONAL_ADD_COUPLER_BTN,
		wxT("Add ->")
	);
	fourthRow2ndCol->Add(m_addReferencedCoupler, 0, wxGROW|wxALL, 5);
	m_addAllAvailableCouplers = new wxButton(
		this,
		ID_DIVISIONAL_ADD_ALL_COUPLERS_BTN,
		wxT("Add all")
	);
	fourthRow2ndCol->Add(m_addAllAvailableCouplers, 0, wxGROW|wxALL, 5);
	m_removeReferencedCoupler = new wxButton(
		this,
		ID_DIVISIONAL_REMOVE_COUPLER_BTN,
		wxT("<- Remove")
	);
	fourthRow2ndCol->Add(m_removeReferencedCoupler, 0, wxGROW|wxALL, 5);
	fourthRow2ndCol->AddStretchSpacer();
	fourthRow->Add(fourthRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *fourthRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenCouplersText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced couplers")
	);
	fourthRow3rdCol->Add(chosenCouplersText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedCouplers = new wxListBox(
		this,
		ID_DIVISIONAL_REFERENCED_COUPLERS
	);
	fourthRow3rdCol->Add(m_referencedCouplers, 1, wxEXPAND|wxALL, 5);
	fourthRow->Add(fourthRow3rdCol, 1, wxEXPAND);
	wxBoxSizer *fourthRow4thCol = new wxBoxSizer(wxVERTICAL);
	fourthRow4thCol->AddStretchSpacer();
	wxStaticText *couplerOnOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Selected coupler is:")
	);
	fourthRow4thCol->Add(couplerOnOffText, 0, wxALL, 5);
	wxBoxSizer *fourthRow4thColRadios = new wxBoxSizer(wxHORIZONTAL);
	m_currentCouplerIsOn = new wxRadioButton(
		this,
		ID_DIVISIONAL_COUPLER_ON_RADIO,
		wxT("On"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	fourthRow4thColRadios->Add(m_currentCouplerIsOn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_currentCouplerIsOff = new wxRadioButton(
		this,
		ID_DIVISIONAL_COUPLER_OFF_RADIO,
		wxT("Off"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_currentCouplerIsOff->SetValue(true);
	fourthRow4thColRadios->Add(m_currentCouplerIsOff, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	fourthRow4thCol->Add(fourthRow4thColRadios, 0, wxEXPAND);
	fourthRow4thCol->AddStretchSpacer();
	fourthRow->Add(fourthRow4thCol, 0, wxEXPAND);
	panelSizer->Add(fourthRow, 1, wxEXPAND);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *fifthRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableTremulantsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available tremulants")
	);
	fifthRow1stCol->Add(availableTremulantsText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableTremulants = new wxListBox(
		this,
		ID_DIVISIONAL_AVAILABLE_TREMS
	);
	fifthRow1stCol->Add(m_availableTremulants, 1, wxEXPAND|wxALL, 5);
	fifthRow->Add(fifthRow1stCol, 1, wxEXPAND);
	wxBoxSizer *fifthRow2ndCol = new wxBoxSizer(wxVERTICAL);
	fifthRow2ndCol->AddStretchSpacer();
	m_addReferencedTremulant = new wxButton(
		this,
		ID_DIVISIONAL_ADD_TREM_BTN,
		wxT("Add ->")
	);
	fifthRow2ndCol->Add(m_addReferencedTremulant, 0, wxGROW|wxALL, 5);
	m_addAllAvailableTremulants = new wxButton(
		this,
		ID_DIVISIONAL_ADD_ALL_TREMS_BTN,
		wxT("Add all")
	);
	fifthRow2ndCol->Add(m_addAllAvailableTremulants, 0, wxGROW|wxALL, 5);
	m_removeReferencedTremulant = new wxButton(
		this,
		ID_DIVISIONAL_REMOVE_TREM_BTN,
		wxT("<- Remove")
	);
	fifthRow2ndCol->Add(m_removeReferencedTremulant, 0, wxGROW|wxALL, 5);
	fifthRow2ndCol->AddStretchSpacer();
	fifthRow->Add(fifthRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *fifthRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenTremulantsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced tremulants")
	);
	fifthRow3rdCol->Add(chosenTremulantsText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedTremulants = new wxListBox(
		this,
		ID_DIVISIONAL_REFERENCED_TREMS
	);
	fifthRow3rdCol->Add(m_referencedTremulants, 1, wxEXPAND|wxALL, 5);
	fifthRow->Add(fifthRow3rdCol, 1, wxEXPAND);
	wxBoxSizer *fifthRow4thCol = new wxBoxSizer(wxVERTICAL);
	fifthRow4thCol->AddStretchSpacer();
	wxStaticText *tremOnOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Selected tremulant is:")
	);
	fifthRow4thCol->Add(tremOnOffText, 0, wxALL, 5);
	wxBoxSizer *fifthRow4thColRadios = new wxBoxSizer(wxHORIZONTAL);
	m_currentTremIsOn = new wxRadioButton(
		this,
		ID_DIVISIONAL_TREM_ON_RADIO,
		wxT("On"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	fifthRow4thColRadios->Add(m_currentTremIsOn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_currentTremIsOff = new wxRadioButton(
		this,
		ID_DIVISIONAL_TREM_OFF_RADIO,
		wxT("Off"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_currentTremIsOff->SetValue(true);
	fifthRow4thColRadios->Add(m_currentTremIsOff, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	fifthRow4thCol->Add(fifthRow4thColRadios, 0, wxEXPAND);
	fifthRow4thCol->AddStretchSpacer();
	fifthRow->Add(fifthRow4thCol, 0, wxEXPAND);
	panelSizer->Add(fifthRow, 1, wxEXPAND);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sixthRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available switches")
	);
	sixthRow1stCol->Add(availableReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableSwitches = new wxListBox(
		this,
		ID_DIVISIONAL_AVAILABLE_SWITCHES
	);
	sixthRow1stCol->Add(m_availableSwitches, 1, wxEXPAND|wxALL, 5);
	sixthRow->Add(sixthRow1stCol, 1, wxEXPAND);
	wxBoxSizer *sixthRow2ndCol = new wxBoxSizer(wxVERTICAL);
	sixthRow2ndCol->AddStretchSpacer();
	m_addReferencedSwitch = new wxButton(
		this,
		ID_DIVISIONAL_ADD_SWITCH_BTN,
		wxT("Add ->")
	);
	sixthRow2ndCol->Add(m_addReferencedSwitch, 0, wxGROW|wxALL, 5);
	m_addAllAvailableSwitches = new wxButton(
		this,
		ID_DIVISIONAL_ADD_ALL_SWITCHES_BTN,
		wxT("Add all")
	);
	sixthRow2ndCol->Add(m_addAllAvailableSwitches, 0, wxGROW|wxALL, 5);
	m_removeReferencedSwitch = new wxButton(
		this,
		ID_DIVISIONAL_REMOVE_SWITCH_BTN,
		wxT("<- Remove")
	);
	sixthRow2ndCol->Add(m_removeReferencedSwitch, 0, wxGROW|wxALL, 5);
	sixthRow2ndCol->AddStretchSpacer();
	sixthRow->Add(sixthRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *sixthRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced switches")
	);
	sixthRow3rdCol->Add(chosenReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedSwitches = new wxListBox(
		this,
		ID_DIVISIONAL_REFERENCED_SWITCHES
	);
	sixthRow3rdCol->Add(m_referencedSwitches, 1, wxEXPAND|wxALL, 5);
	sixthRow->Add(sixthRow3rdCol, 1, wxEXPAND);

	wxBoxSizer *sixthRow4thCol = new wxBoxSizer(wxVERTICAL);
	sixthRow4thCol->AddStretchSpacer();
	wxStaticText *switchOnOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Selected switch is:")
	);
	sixthRow4thCol->Add(switchOnOffText, 0, wxALL, 5);
	wxBoxSizer *sixthRow4thColRadios = new wxBoxSizer(wxHORIZONTAL);
	m_currentSwitchIsOn = new wxRadioButton(
		this,
		ID_DIVISIONAL_SWITCH_ON_RADIO,
		wxT("On"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	sixthRow4thColRadios->Add(m_currentSwitchIsOn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_currentSwitchIsOff = new wxRadioButton(
		this,
		ID_DIVISIONAL_SWITCH_OFF_RADIO,
		wxT("Off"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_currentSwitchIsOff->SetValue(true);
	sixthRow4thColRadios->Add(m_currentSwitchIsOff, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	sixthRow4thCol->Add(sixthRow4thColRadios, 0, wxEXPAND);
	sixthRow4thCol->AddStretchSpacer();
	sixthRow->Add(sixthRow4thCol, 0, wxEXPAND);
	panelSizer->Add(sixthRow, 1, wxEXPAND);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeDivisionalBtn = new wxButton(
		this,
		ID_DIVISIONAL_REMOVE_BTN,
		wxT("Remove this divisional")
	);
	bottomRow->Add(removeDivisionalBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

DivisionalPanel::~DivisionalPanel() {
	
}

void DivisionalPanel::setDivisional(Divisional *divisional) {
	m_divisional = divisional;
	m_nameField->ChangeValue(m_divisional->getName());
	if (m_divisional->isDisplayedInverted())
		m_displayInvertedYes->SetValue(true);
	else
		m_displayInvertedNo->SetValue(true);
	if (m_divisional->isProtected())
		m_isProtectedYes->SetValue(true);
	else
		m_isProtectedNo->SetValue(true);
	
	// update/populate available stops
	wxArrayString manualStops;
	unsigned nbStops = m_divisional->getOwningManual()->getNumberOfStops();
	if (nbStops > 0) {
		for (unsigned i = 0; i < nbStops; i++) {
			manualStops.Add(m_divisional->getOwningManual()->getStopAt(i)->getName());
		}
		m_availableStops->Clear();
		m_availableStops->InsertItems(manualStops, 0);
		m_addAllAvailableStops->Enable();
	} else {
		m_availableStops->Clear();
		m_addAllAvailableStops->Disable();
	}
	
	UpdateReferencedStops();
	
	m_addReferencedStop->Disable();
	m_removeReferencedStop->Disable();
	
	// update/populate available couplers
	wxArrayString manualCouplers;
	unsigned nbCouplers = m_divisional->getOwningManual()->getNumberOfCouplers();
	if (nbCouplers > 0) {
		for (unsigned i = 0; i < nbCouplers; i++) {
			manualCouplers.Add(m_divisional->getOwningManual()->getCouplerAt(i)->getName());
		}
		m_availableCouplers->Clear();
		m_availableCouplers->InsertItems(manualCouplers, 0);
		m_addAllAvailableCouplers->Enable();
	} else {
		m_availableCouplers->Clear();
		m_addAllAvailableCouplers->Disable();
	}

	UpdateReferencedCouplers();

	m_addReferencedCoupler->Disable();
	m_removeReferencedCoupler->Disable();

	// update/populate available tremulants
	wxArrayString manualTrems;
	unsigned nbTrems = m_divisional->getOwningManual()->getNumberOfTremulants();
	if (nbTrems > 0) {
		for (unsigned i = 0; i < nbTrems; i++) {
			manualTrems.Add(m_divisional->getOwningManual()->getTremulantAt(i)->getName());
		}
		m_availableTremulants->Clear();
		m_availableTremulants->InsertItems(manualTrems, 0);
		m_addAllAvailableTremulants->Enable();
	} else {
		m_availableTremulants->Clear();
		m_addAllAvailableTremulants->Disable();
	}

	UpdateReferencedTremulants();

	m_addReferencedTremulant->Disable();
	m_removeReferencedTremulant->Disable();

	// update/populate available switches
	wxArrayString manualSwitches;
	unsigned nbSwitches = m_divisional->getOwningManual()->getNumberOfGoSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			manualSwitches.Add(m_divisional->getOwningManual()->getGoSwitchAt(i)->getName());
		}
		m_availableSwitches->Clear();
		m_availableSwitches->InsertItems(manualSwitches, 0);
		m_addAllAvailableSwitches->Enable();
	} else {
		m_availableSwitches->Clear();
		m_addAllAvailableSwitches->Disable();
	}

	UpdateReferencedSwitches();

	m_addReferencedSwitch->Disable();
	m_removeReferencedSwitch->Disable();

	m_currentStopIsOn->Disable();
	m_currentStopIsOff->Disable();
	m_currentCouplerIsOn->Disable();
	m_currentCouplerIsOff->Disable();
	m_currentTremIsOn->Disable();
	m_currentTremIsOff->Disable();
	m_currentSwitchIsOn->Disable();
	m_currentSwitchIsOff->Disable();
}

void DivisionalPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void DivisionalPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_nameField->SetToolTip(wxT("It's best to make the name unique. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_isProtectedYes->SetToolTip(wxT("Yes means that the content of the divisional cannot be modified by the user."));
		m_isProtectedNo->SetToolTip(wxT("No means that the user can modify the state of added elements."));
		m_displayInvertedYes->SetToolTip(wxT("Yes means that the on and off bitmaps are reversed."));
		m_displayInvertedNo->SetToolTip(wxT("No is the standard on/off bitmap display."));
	} else {
		m_nameField->SetToolTip(wxEmptyString);
		m_isProtectedYes->SetToolTip(wxEmptyString);
		m_isProtectedNo->SetToolTip(wxEmptyString);
		m_displayInvertedYes->SetToolTip(wxEmptyString);
		m_displayInvertedNo->SetToolTip(wxEmptyString);
	}
}

void DivisionalPanel::refreshData() {
	m_nameField->ChangeValue(m_divisional->getName());
}

void DivisionalPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_divisional->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void DivisionalPanel::OnDisplayInvertedRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_DIVISIONAL_INVERTED_STATE_YES) {
		m_displayInvertedYes->SetValue(true);
		m_divisional->setDisplayInverted(true);
	} else {
		m_displayInvertedNo->SetValue(true);
		m_divisional->setDisplayInverted(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnProtectedSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_DIVISIONAL_PROTECTED_YES) {
		m_isProtectedYes->SetValue(true);
		m_divisional->setProtected(true);
	} else {
		m_isProtectedNo->SetValue(true);
		m_divisional->setProtected(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnAddStopReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableStops->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableStops->GetSelection();
		if (!m_divisional->hasStop(m_divisional->getOwningManual()->getStopAt(selected))) {
			m_divisional->addStop(m_divisional->getOwningManual()->getStopAt(selected), false);
			UpdateReferencedStops();
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnAddAllAvailableStopsBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableStops->IsEmpty())
		return;
	else {
		for (unsigned i = 0; i < m_availableStops->GetCount(); i++) {
			if (!m_divisional->hasStop(m_divisional->getOwningManual()->getStopAt(i)))
				m_divisional->addStop(m_divisional->getOwningManual()->getStopAt(i), false);
		}
		UpdateReferencedStops();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnRemoveStopReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedStops->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedStops->GetSelection();
		m_divisional->removeStopAt(selected);
		UpdateReferencedStops();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnStopListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableStops->GetSelection() != wxNOT_FOUND) {
		m_addReferencedStop->Enable(true);
	}
}

void DivisionalPanel::OnReferencedStopSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedStops->GetSelection() != wxNOT_FOUND) {
		m_removeReferencedStop->Enable(true);
		m_currentStopIsOn->Enable();
		m_currentStopIsOff->Enable();

		// also update on/off radiobuttons for this stop
		unsigned selected = (unsigned) m_referencedStops->GetSelection();
		if (m_divisional->getStopPairAt(selected)->second)
			m_currentStopIsOn->SetValue(true);
		else
			m_currentStopIsOff->SetValue(true);
	}
}

void DivisionalPanel::OnStopOnOffRadioBtns(wxCommandEvent& event) {
	if (m_referencedStops->GetSelection() != wxNOT_FOUND) {
		bool onOffValue = false;
		if (event.GetId() == ID_DIVISIONAL_STOP_ON_RADIO)
			onOffValue = true;

		unsigned selected = (unsigned) m_referencedStops->GetSelection();
		m_divisional->getStopPairAt(selected)->second = onOffValue;
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::UpdateReferencedStops() {
	m_currentStopIsOff->SetValue(true);
	m_currentStopIsOn->Disable();
	m_currentStopIsOff->Disable();
	wxArrayString referencedStops;
	unsigned nbStops = m_divisional->getNumberOfStops();
	if (nbStops > 0) {
		for (unsigned i = 0; i < nbStops; i++) {
			// stored as a pair, first is stop pointer, second is bool on/off
			referencedStops.Add(m_divisional->getStopPairAt(i)->first->getName());
		}
		m_referencedStops->Clear();
		m_referencedStops->InsertItems(referencedStops, 0);
	} else {
		m_referencedStops->Clear();
		m_removeReferencedStop->Disable();
	}
}

void DivisionalPanel::OnAddCouplerReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableCouplers->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableCouplers->GetSelection();
		if (!m_divisional->hasCoupler(m_divisional->getOwningManual()->getCouplerAt(selected))) {
			m_divisional->addCoupler(m_divisional->getOwningManual()->getCouplerAt(selected), false);
			UpdateReferencedCouplers();
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnAddAllAvailableCouplersBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableCouplers->IsEmpty())
		return;
	else {
		for (unsigned i = 0; i < m_availableCouplers->GetCount(); i++) {
			if (!m_divisional->hasCoupler(m_divisional->getOwningManual()->getCouplerAt(i)))
				m_divisional->addCoupler(m_divisional->getOwningManual()->getCouplerAt(i), false);
		}
		UpdateReferencedCouplers();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnRemoveCouplerReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedCouplers->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedCouplers->GetSelection();
		m_divisional->removeCouplerAt(selected);
		UpdateReferencedCouplers();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnCouplerListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableCouplers->GetSelection() != wxNOT_FOUND) {
		m_addReferencedCoupler->Enable(true);
	}
}

void DivisionalPanel::OnReferencedCouplerSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedCouplers->GetSelection() != wxNOT_FOUND) {
		m_removeReferencedCoupler->Enable(true);
		m_currentCouplerIsOn->Enable();
		m_currentCouplerIsOff->Enable();

		// also update on/off radiobuttons for this coupler
		unsigned selected = (unsigned) m_referencedCouplers->GetSelection();
		if (m_divisional->getCouplerPairAt(selected)->second)
			m_currentCouplerIsOn->SetValue(true);
		else
			m_currentCouplerIsOff->SetValue(true);
	}
}

void DivisionalPanel::OnCouplerOnOffRadioBtns(wxCommandEvent& event) {
	if (m_referencedCouplers->GetSelection() != wxNOT_FOUND) {
		bool onOffValue = false;
		if (event.GetId() == ID_DIVISIONAL_COUPLER_ON_RADIO)
			onOffValue = true;

		unsigned selected = (unsigned) m_referencedCouplers->GetSelection();
		m_divisional->getCouplerPairAt(selected)->second = onOffValue;
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::UpdateReferencedCouplers() {
	m_currentCouplerIsOff->SetValue(true);
	m_currentCouplerIsOn->Disable();
	m_currentCouplerIsOff->Disable();
	wxArrayString referencedCouplers;
	unsigned nbCouplers = m_divisional->getNumberOfCouplers();
	if (nbCouplers > 0) {
		for (unsigned i = 0; i < nbCouplers; i++) {
			// stored as a pair, first is coupler pointer, second is bool on/off
			referencedCouplers.Add(m_divisional->getCouplerPairAt(i)->first->getName());
		}
		m_referencedCouplers->Clear();
		m_referencedCouplers->InsertItems(referencedCouplers, 0);
	} else {
		m_referencedCouplers->Clear();
		m_removeReferencedCoupler->Disable();
	}
}

void DivisionalPanel::OnAddTremulantReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableTremulants->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableTremulants->GetSelection();
		if (!m_divisional->hasTremulant(m_divisional->getOwningManual()->getTremulantAt(selected))) {
			m_divisional->addTremulant(m_divisional->getOwningManual()->getTremulantAt(selected), false);
			UpdateReferencedTremulants();
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnAddAllAvailableTremulantsBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableTremulants->IsEmpty())
		return;
	else {
		for (unsigned i = 0; i < m_availableTremulants->GetCount(); i++) {
			if (!m_divisional->hasTremulant(m_divisional->getOwningManual()->getTremulantAt(i)))
				m_divisional->addTremulant(m_divisional->getOwningManual()->getTremulantAt(i), false);
		}
		UpdateReferencedTremulants();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnRemoveTremulantReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedTremulants->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedTremulants->GetSelection();
		m_divisional->removeTremulantAt(selected);
		UpdateReferencedTremulants();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnTremulantListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableTremulants->GetSelection() != wxNOT_FOUND) {
		m_addReferencedTremulant->Enable(true);
	}
}

void DivisionalPanel::OnReferencedTremulantSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedTremulants->GetSelection() != wxNOT_FOUND) {
		m_removeReferencedTremulant->Enable(true);
		m_currentTremIsOn->Enable();
		m_currentTremIsOff->Enable();

		// also update on/off radiobuttons for this tremulant
		unsigned selected = (unsigned) m_referencedTremulants->GetSelection();
		if (m_divisional->getTremulantPairAt(selected)->second)
			m_currentTremIsOn->SetValue(true);
		else
			m_currentTremIsOff->SetValue(true);
	}
}

void DivisionalPanel::OnTremOnOffRadioBtns(wxCommandEvent& event) {
	if (m_referencedTremulants->GetSelection() != wxNOT_FOUND) {
		bool onOffValue = false;
		if (event.GetId() == ID_DIVISIONAL_TREM_ON_RADIO)
			onOffValue = true;

		unsigned selected = (unsigned) m_referencedTremulants->GetSelection();
		m_divisional->getTremulantPairAt(selected)->second = onOffValue;
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::UpdateReferencedTremulants() {
	m_currentTremIsOff->SetValue(true);
	m_currentTremIsOn->Disable();
	m_currentTremIsOff->Disable();
	wxArrayString referencedTrems;
	unsigned nbTrems = m_divisional->getNumberOfTremulants();
	if (nbTrems > 0) {
		for (unsigned i = 0; i < nbTrems; i++) {
			// stored as a pair, first is trem pointer, second is bool on/off
			referencedTrems.Add(m_divisional->getTremulantPairAt(i)->first->getName());
		}
		m_referencedTremulants->Clear();
		m_referencedTremulants->InsertItems(referencedTrems, 0);
	} else {
		m_referencedTremulants->Clear();
		m_removeReferencedTremulant->Disable();
	}
}

void DivisionalPanel::OnAddSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableSwitches->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableSwitches->GetSelection();
		if (!m_divisional->hasSwitch(m_divisional->getOwningManual()->getGoSwitchAt(selected))) {
			m_divisional->addSwitch(m_divisional->getOwningManual()->getGoSwitchAt(selected), false);
			UpdateReferencedSwitches();
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnAddAllAvailableSwitchesBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableSwitches->IsEmpty())
		return;
	else {
		for (unsigned i = 0; i < m_availableSwitches->GetCount(); i++) {
			if (!m_divisional->hasSwitch(m_divisional->getOwningManual()->getGoSwitchAt(i)))
				m_divisional->addSwitch(m_divisional->getOwningManual()->getGoSwitchAt(i), false);
		}
		UpdateReferencedSwitches();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnRemoveSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedSwitches->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedSwitches->GetSelection();
		m_divisional->removeSwitchAt(selected);
		UpdateReferencedSwitches();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::OnSwitchListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableSwitches->GetSelection() != wxNOT_FOUND) {
		m_addReferencedSwitch->Enable(true);
	}
}

void DivisionalPanel::OnReferencedSwitchSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedSwitches->GetSelection() != wxNOT_FOUND) {
		m_removeReferencedSwitch->Enable(true);
		m_currentSwitchIsOn->Enable();
		m_currentSwitchIsOff->Enable();

		// also update on/off radiobuttons for this switch
		unsigned selected = (unsigned) m_referencedSwitches->GetSelection();
		if (m_divisional->getSwitchPairAt(selected)->second)
			m_currentSwitchIsOn->SetValue(true);
		else
			m_currentSwitchIsOff->SetValue(true);
	}
}

void DivisionalPanel::OnSwitchOnOffRadioBtns(wxCommandEvent& event) {
	if (m_referencedSwitches->GetSelection() != wxNOT_FOUND) {
		bool onOffValue = false;
		if (event.GetId() == ID_DIVISIONAL_SWITCH_ON_RADIO)
			onOffValue = true;

		unsigned selected = (unsigned) m_referencedSwitches->GetSelection();
		m_divisional->getSwitchPairAt(selected)->second = onOffValue;
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalPanel::UpdateReferencedSwitches() {
	m_currentSwitchIsOff->SetValue(true);
	m_currentSwitchIsOn->Disable();
	m_currentSwitchIsOff->Disable();
	wxArrayString referencedSwitches;
	unsigned nbSwitches = m_divisional->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			// stored as a pair first is switch pointer, second is bool on/off
			referencedSwitches.Add(m_divisional->getSwitchPairAt(i)->first->getName());
		}
		m_referencedSwitches->Clear();
		m_referencedSwitches->InsertItems(referencedSwitches, 0);
	} else {
		m_referencedSwitches->Clear();
		m_removeReferencedSwitch->Disable();
	}
}

void DivisionalPanel::OnRemoveDivisionalBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this divisional?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			// the divisional is removed from manual and organ and any gui representation is removed from any panel
			::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
			m_isFirstRemoval = false;
		}
	} else {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}
