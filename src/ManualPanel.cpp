/*
 * ManualPanel.cpp is part of GoOdf.
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

#include "ManualPanel.h"
#include <wx/statline.h>
#include <wx/button.h>
#include "GOODFDef.h"
#include "GOODF.h"
#include "GOODFFunctions.h"

// Event table
BEGIN_EVENT_TABLE(ManualPanel, wxPanel)
	EVT_TEXT(ID_MANUAL_NAME_TEXT, ManualPanel::OnNameChange)
	EVT_CHECKBOX(ID_MANUAL_IS_PEDAL_OPTION, ManualPanel::OnPedalCheckbox)
	EVT_SPINCTRL(ID_MANUAL_NBR_LOGICAL_KEYS_SPIN, ManualPanel::OnLogicalKeysSpin)
	EVT_SPINCTRL(ID_MANUAL_FIRST_ACC_LOGICAL_KEY_SPIN, ManualPanel::OnFirstLogicalKeySpin)
	EVT_SPINCTRL(ID_MANUAL_FIRST_MIDI_NOTE_SPIN, ManualPanel::OnFirstMidiNoteSpin)
	EVT_SPINCTRL(ID_MANUAL_NBR_ACC_KEYS_SPIN, ManualPanel::OnAccessibleKeysSpin)
	EVT_SPINCTRL(ID_MANUAL_MIDI_INPUT_NBR_SPIN, ManualPanel::OnMidiInputNbrSpin)
	EVT_BUTTON(ID_MANUAL_ADD_STOP_BTN, ManualPanel::OnNewStopBtn)
	EVT_BUTTON(ID_MANUAL_ADD_COUPLER_BTN, ManualPanel::OnNewCouplerBtn)
	EVT_BUTTON(ID_MANUAL_ADD_DIVISIONAL_BTN, ManualPanel::OnNewDivisionalBtn)
	EVT_LISTBOX(ID_MANUAL_AVAILABLE_TREMULANTS, ManualPanel::OnAvailableTremSelection)
	EVT_BUTTON(ID_MANUAL_ADD_TREMULANT_BTN, ManualPanel::OnAddReferencedTremBtn)
	EVT_BUTTON(ID_MANUAL_REMOVE_TREMULANT_BTN, ManualPanel::OnRemoveReferencedTremBtn)
	EVT_LISTBOX(ID_MANUAL_REFERENCED_TREMULANTS, ManualPanel::OnReferencedTremSelection)
	EVT_LISTBOX(ID_MANUAL_AVAILABLE_SWITCHES, ManualPanel::OnAvailableSwitchSelection)
	EVT_BUTTON(ID_MANUAL_ADD_SWITCH_BTN, ManualPanel::OnAddReferencedSwitchBtn)
	EVT_BUTTON(ID_MANUAL_REMOVE_SWITCH_BTN, ManualPanel::OnRemoveReferencedSwitchBtn)
	EVT_LISTBOX(ID_MANUAL_REFERENCED_SWITCHES, ManualPanel::OnReferencedSwitchSelection)
	EVT_CHOICE(ID_MANUAL_MIDI_KEY_MAP_CHOICE, ManualPanel::OnMidiKeyMapKeyChoice)
	EVT_SPINCTRL(ID_MANUAL_MIDI_KEYMAP_VALUE_SPIN, ManualPanel::OnMidiKeyMapValueSpin)
	EVT_BUTTON(ID_MANUAL_REMOVE_BTN, ManualPanel::OnRemoveManualBtn)
END_EVENT_TABLE()

ManualPanel::ManualPanel(wxWindow *parent) : wxPanel(parent) {
	m_manual = NULL;
	m_isFirstRemoval = true;
	for (unsigned i = 0; i < 127; i++) {
		wxString key = wxT("MIDIKey") + GOODF_functions::number_format(i);
		m_mapKeys.Add(key);
	}

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *manualNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Manual name: ")
	);
	firstRow->Add(manualNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_MANUAL_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	m_thisIsThePedalCheckbox = new wxCheckBox(
		this,
		ID_MANUAL_IS_PEDAL_OPTION,
		wxT("This manual is the pedal"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_thisIsThePedalCheckbox->SetValue(false);
	m_thisIsThePedalCheckbox->Disable();
	firstRow->Add(m_thisIsThePedalCheckbox, 0, wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *numberOfLogicalKeysText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Number of logical keys: ")
	);
	secondRow->Add(numberOfLogicalKeysText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_numberOfLogicalKeysSpin = new wxSpinCtrl(
		this,
		ID_MANUAL_NBR_LOGICAL_KEYS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		192,
		61
	);
	secondRow->Add(m_numberOfLogicalKeysSpin, 0, wxEXPAND|wxALL, 5);
	wxStaticText *firstAccLogicalKeyNbrText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("First accessible key logical key number: ")
	);
	secondRow->Add(firstAccLogicalKeyNbrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstAccessibleKeyLogicalKeyNumberSpin = new wxSpinCtrl(
		this,
		ID_MANUAL_FIRST_ACC_LOGICAL_KEY_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		61,
		1
	);
	secondRow->Add(m_firstAccessibleKeyLogicalKeyNumberSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *firstKeyMidiNoteText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("First accessible key MIDI note number: ")
	);
	thirdRow->Add(firstKeyMidiNoteText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstAccessibleKeyMIDINoteNumberSpin = new wxSpinCtrl(
		this,
		ID_MANUAL_FIRST_MIDI_NOTE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		127,
		36
	);
	thirdRow->Add(m_firstAccessibleKeyMIDINoteNumberSpin, 0, wxEXPAND|wxALL, 5);
	wxStaticText *nbrAccessibleKeysText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Number of accessible keys: ")
	);
	thirdRow->Add(nbrAccessibleKeysText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_numberOfAccessibleKeysSpin = new wxSpinCtrl(
		this,
		ID_MANUAL_NBR_ACC_KEYS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		61,
		1
	);
	thirdRow->Add(m_numberOfAccessibleKeysSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *midiInputNbrText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MIDI input number: ")
	);
	fourthRow->Add(midiInputNbrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_midiInputNumberSpin = new wxSpinCtrl(
		this,
		ID_MANUAL_MIDI_INPUT_NBR_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		200,
		0
	);
	fourthRow->Add(m_midiInputNumberSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	fifthRow->AddStretchSpacer();
	wxStaticText *stopText = new wxStaticText(this, wxID_STATIC, wxT("Add"));
	fifthRow->Add(stopText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_addStopSpin = new wxSpinCtrl(
		this,
		ID_ADD_STOP_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		999,
		1
	);
	fifthRow->Add(m_addStopSpin, 0, wxALL, 5);
	m_addNewStop = new wxButton(
		this,
		ID_MANUAL_ADD_STOP_BTN,
		wxT("new stop(s)")
	);
	fifthRow->Add(m_addNewStop, 0, wxALL, 5);
	fifthRow->AddStretchSpacer();
	m_addNewCoupler = new wxButton(
		this,
		ID_MANUAL_ADD_COUPLER_BTN,
		wxT("Add new coupler")
	);
	fifthRow->Add(m_addNewCoupler, 0, wxALIGN_CENTER|wxALL, 5);
	fifthRow->AddStretchSpacer();
	m_addNewDivisional = new wxButton(
		this,
		ID_MANUAL_ADD_DIVISIONAL_BTN,
		wxT("Add new divisional")
	);
	fifthRow->Add(m_addNewDivisional, 0, wxALIGN_CENTER|wxALL, 5);
	fifthRow->AddStretchSpacer();
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sixthRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableRefTremText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available tremulants")
	);
	sixthRow1stCol->Add(availableRefTremText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableTremulants = new wxListBox(
		this,
		ID_MANUAL_AVAILABLE_TREMULANTS,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	sixthRow1stCol->Add(m_availableTremulants, 1, wxEXPAND|wxALL, 5);
	sixthRow->Add(sixthRow1stCol, 1, wxEXPAND);
	wxBoxSizer *sixthRow2ndCol = new wxBoxSizer(wxVERTICAL);
	sixthRow2ndCol->AddStretchSpacer();
	m_addReferencedTremulant = new wxButton(
		this,
		ID_MANUAL_ADD_TREMULANT_BTN,
		wxT("Add ->")
	);
	sixthRow2ndCol->Add(m_addReferencedTremulant, 0, wxGROW|wxALL, 5);
	m_removeReferencedTremulant = new wxButton(
		this,
		ID_MANUAL_REMOVE_TREMULANT_BTN,
		wxT("<- Remove")
	);
	sixthRow2ndCol->Add(m_removeReferencedTremulant, 0, wxGROW|wxALL, 5);
	sixthRow2ndCol->AddStretchSpacer();
	sixthRow->Add(sixthRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *sixthRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced tremulants")
	);
	sixthRow3rdCol->Add(chosenReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedTremulants = new wxListBox(
		this,
		ID_MANUAL_REFERENCED_TREMULANTS,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	sixthRow3rdCol->Add(m_referencedTremulants, 1, wxEXPAND|wxALL, 5);
	sixthRow->Add(sixthRow3rdCol, 1, wxEXPAND);
	panelSizer->Add(sixthRow, 1, wxEXPAND);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *seventhRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available switches")
	);
	seventhRow1stCol->Add(availableReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableSwitches = new wxListBox(
		this,
		ID_MANUAL_AVAILABLE_SWITCHES,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	seventhRow1stCol->Add(m_availableSwitches, 1, wxEXPAND|wxALL, 5);
	seventhRow->Add(seventhRow1stCol, 1, wxEXPAND);
	wxBoxSizer *seventhRow2ndCol = new wxBoxSizer(wxVERTICAL);
	seventhRow2ndCol->AddStretchSpacer();
	m_addReferencedSwitch = new wxButton(
		this,
		ID_MANUAL_ADD_SWITCH_BTN,
		wxT("Add ->")
	);
	seventhRow2ndCol->Add(m_addReferencedSwitch, 0, wxGROW|wxALL, 5);
	m_removeReferencedSwitch = new wxButton(
		this,
		ID_MANUAL_REMOVE_SWITCH_BTN,
		wxT("<- Remove")
	);
	seventhRow2ndCol->Add(m_removeReferencedSwitch, 0, wxGROW|wxALL, 5);
	seventhRow2ndCol->AddStretchSpacer();
	seventhRow->Add(seventhRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *seventhRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenSwitchRefText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced switches")
	);
	seventhRow3rdCol->Add(chosenSwitchRefText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedSwitches = new wxListBox(
		this,
		ID_MANUAL_REFERENCED_SWITCHES,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	seventhRow3rdCol->Add(m_referencedSwitches, 1, wxEXPAND|wxALL, 5);
	seventhRow->Add(seventhRow3rdCol, 1, wxEXPAND);
	panelSizer->Add(seventhRow, 1, wxEXPAND);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *midiKeyMapKeyText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MIDI key map key: ")
	);
	eighthRow->Add(midiKeyMapKeyText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_midiKeyMapKeys = new wxChoice(
		this,
		ID_MANUAL_MIDI_KEY_MAP_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_mapKeys
	);
	eighthRow->Add(m_midiKeyMapKeys, 0, wxALL, 5);
	wxStaticText *midiKeyMapValueText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT(" maps to value: ")
	);
	eighthRow->Add(midiKeyMapValueText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_midiKeyMapValue = new wxSpinCtrl(
		this,
		ID_MANUAL_MIDI_KEYMAP_VALUE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		127,
		0
	);
	eighthRow->Add(m_midiKeyMapValue, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eighthRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeManualBtn = new wxButton(
		this,
		ID_MANUAL_REMOVE_BTN,
		wxT("Remove this manual")
	);
	bottomRow->Add(removeManualBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

ManualPanel::~ManualPanel() {

}

void ManualPanel::setManual(Manual *manual) {
	m_manual = manual;
	m_nameField->ChangeValue(m_manual->getName());
	if (m_manual == ::wxGetApp().m_frame->m_organ->getOrganManualAt(0))
		m_thisIsThePedalCheckbox->Enable(true);
	else
		m_thisIsThePedalCheckbox->Disable();
	if (m_manual->isThePedal())
		m_thisIsThePedalCheckbox->SetValue(true);
	else
		m_thisIsThePedalCheckbox->SetValue(false);
	m_numberOfLogicalKeysSpin->SetValue(m_manual->getNumberOfLogicalKeys());

	// adjust range of m_firstAccessibleKeyLogicalKeyNumberSpin
	m_firstAccessibleKeyLogicalKeyNumberSpin->SetRange(1, m_manual->getNumberOfLogicalKeys());

	// and accessible keys
	if (m_manual->getNumberOfLogicalKeys() > 85)
		m_numberOfAccessibleKeysSpin->SetRange(0, 85);
	else
		m_numberOfAccessibleKeysSpin->SetRange(0, m_manual->getNumberOfLogicalKeys());

	m_firstAccessibleKeyLogicalKeyNumberSpin->SetValue(m_manual->getFirstAccessibleKeyLogicalKeyNumber());
	m_firstAccessibleKeyMIDINoteNumberSpin->SetValue(m_manual->getFirstAccessibleKeyMIDINoteNumber());
	m_numberOfAccessibleKeysSpin->SetValue(m_manual->getNumberOfAccessibleKeys());
	m_midiInputNumberSpin->SetValue(m_manual->getMidiInputNumber());

	int totalNbrStops = ::wxGetApp().m_frame->m_organ->getNumberOfStops();
	if (totalNbrStops < 999) {
		m_addStopSpin->SetRange(1, 999 - totalNbrStops);
		m_addStopSpin->SetValue(1);
		m_addStopSpin->Enable();
		m_addNewStop->Enable();
	} else {
		m_addStopSpin->SetRange(0, 1);
		m_addStopSpin->SetValue(0);
		m_addStopSpin->Disable();
		m_addNewStop->Disable();
	}
	// Update/populate available tremulants
	wxArrayString organTremulants;
	if (!m_availableTremulants->IsEmpty())
		m_availableTremulants->Clear();
	unsigned nbTrems = ::wxGetApp().m_frame->m_organ->getNumberOfTremulants();
	if (nbTrems > 0) {
		for (unsigned i = 0; i < nbTrems; i++) {
			organTremulants.Add(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(i)->getName());
		}

		m_availableTremulants->InsertItems(organTremulants, 0);
	}

	UpdateReferencedTremulants();

	m_addReferencedTremulant->Disable();
	m_removeReferencedTremulant->Disable();

	// update/populate available switches
	wxArrayString organSwitches;
	if (!m_availableSwitches->IsEmpty())
		m_availableSwitches->Clear();
	unsigned nbSwitches = ::wxGetApp().m_frame->m_organ->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			organSwitches.Add(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(i)->getName());
		}
		m_availableSwitches->InsertItems(organSwitches, 0);
	}

	UpdateReferencedSwitches();

	m_addReferencedSwitch->Disable();
	m_removeReferencedSwitch->Disable();

	m_midiKeyMapKeys->SetSelection(36);
	m_midiKeyMapValue->SetValue(m_manual->getMidiKeyMapValue(m_midiKeyMapKeys->GetString(m_midiKeyMapKeys->GetCurrentSelection())));
}

void ManualPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void ManualPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_nameField->SetToolTip(wxT("It's best to make the name unique. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_thisIsThePedalCheckbox->SetToolTip(wxT("If checked this manual is written as [Manual000] and assumed to be the pedal. Only the very first manual can be set to be the pedal as [Manual000]."));
		m_numberOfLogicalKeysSpin->SetToolTip(wxT("This is the total number of keys on a manual, including not directly playable ones (for instance not by physical keys - but reachable by octave couplers!). Normally, if you want 61 keys for a manual then the value for this property should also be set to 61."));
		m_firstAccessibleKeyLogicalKeyNumberSpin->SetToolTip(wxT("The number of the first usable key."));
		m_firstAccessibleKeyMIDINoteNumberSpin->SetToolTip(wxT("The MIDI number of the first accessible key (by means of MIDI keys). For a normal organ compass this is usually set to 36."));
		m_numberOfAccessibleKeysSpin->SetToolTip(wxT("Number of MIDI accessible keys."));
		m_midiInputNumberSpin->SetToolTip(wxT("This number is used while building the initial MIDI configuration. 0 means no association. 1 maps to pedal, 2 to first manual, 3 to second manual etc."));
		m_addNewDivisional->SetToolTip(wxT("This button adds an old style ODF based divisional that can have a customized content. If there's no special reason to use this feature it's generally better to use the built in setter versions of the divisionals that GrandOrgue provide (this is available on any Panel as SetterXXXDivisionalYYY)."));
		m_availableTremulants->SetToolTip(wxT("Here all available tremulants of the organ are listed."));
		m_referencedTremulants->SetToolTip(wxT("This list contain the tremulants that are associated with this manual. NOTE: This association mainly matters for the built in divisionals as it decides what tremulants can be stored in them."));
		m_addReferencedTremulant->SetToolTip(wxT("Add the selected tremulant as associated with this manual."));
		m_removeReferencedTremulant->SetToolTip(wxT("Remove the association with the selected tremulant."));
		m_availableSwitches->SetToolTip(wxT("Here all the available switches of the organ are listed."));
		m_referencedSwitches->SetToolTip(wxT("This list contain the switches that are associated with this manual. NOTE: This association mainly matters for the built in divisionals as it decides what switches can be stored in them."));
		m_addReferencedSwitch->SetToolTip(wxT("Add the selected switch(es) as associated with this manual."));
		m_removeReferencedSwitch->SetToolTip(wxT("Remove the association with the selected switch(es)."));
		m_midiKeyMapKeys->SetToolTip(wxT("Allows to map the MIDI note in MIDIKey999 to a different number. NOTE: If there's no special reason to change this, the mapping should be left 'as is' by default!"));
		m_midiKeyMapValue->SetToolTip(wxT("This is the number that the chosen MIDIKey will map to. NOTE: If there's no special reason to change this, the mapping should be left 'as is' by default!"));
	} else {
		m_nameField->SetToolTip(wxEmptyString);
		m_thisIsThePedalCheckbox->SetToolTip(wxEmptyString);
		m_numberOfLogicalKeysSpin->SetToolTip(wxEmptyString);
		m_firstAccessibleKeyLogicalKeyNumberSpin->SetToolTip(wxEmptyString);
		m_firstAccessibleKeyMIDINoteNumberSpin->SetToolTip(wxEmptyString);
		m_numberOfAccessibleKeysSpin->SetToolTip(wxEmptyString);
		m_midiInputNumberSpin->SetToolTip(wxEmptyString);
		m_addNewDivisional->SetToolTip(wxEmptyString);
		m_availableTremulants->SetToolTip(wxEmptyString);
		m_referencedTremulants->SetToolTip(wxEmptyString);
		m_addReferencedTremulant->SetToolTip(wxEmptyString);
		m_removeReferencedTremulant->SetToolTip(wxEmptyString);
		m_availableSwitches->SetToolTip(wxEmptyString);
		m_referencedSwitches->SetToolTip(wxEmptyString);
		m_addReferencedSwitch->SetToolTip(wxEmptyString);
		m_removeReferencedSwitch->SetToolTip(wxEmptyString);
		m_midiKeyMapKeys->SetToolTip(wxEmptyString);
		m_midiKeyMapValue->SetToolTip(wxEmptyString);
	}
}

void ManualPanel::refreshData() {
	m_nameField->ChangeValue(m_manual->getName());
}

void ManualPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_manual->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void ManualPanel::OnPedalCheckbox(wxCommandEvent& WXUNUSED(event)) {
	if (m_thisIsThePedalCheckbox->GetValue())
		m_manual->setIsPedal(true);
	else
		m_manual->setIsPedal(false);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void ManualPanel::OnLogicalKeysSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setNumberOfLogicalKeys(m_numberOfLogicalKeysSpin->GetValue());

	// adjust range of m_firstAccessibleKeyLogicalKeyNumberSpin
	m_firstAccessibleKeyLogicalKeyNumberSpin->SetRange(1, m_manual->getNumberOfLogicalKeys());

	// and accessible keys
	if (m_manual->getNumberOfLogicalKeys() > 85) {
		m_numberOfAccessibleKeysSpin->SetRange(0, 85);
	} else {
		m_numberOfAccessibleKeysSpin->SetRange(0, m_manual->getNumberOfLogicalKeys());
		// also possibly update actual value in manual
		if (m_manual->getNumberOfAccessibleKeys() > m_manual->getNumberOfLogicalKeys())
			m_manual->setNumberOfAccessibleKeys(m_manual->getNumberOfLogicalKeys());
	}

	UpdateAnyGuiVersion();

	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ManualPanel::OnFirstLogicalKeySpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setFirstAccessibleKeyLogicalKeyNumber(m_firstAccessibleKeyLogicalKeyNumberSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ManualPanel::OnFirstMidiNoteSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setFirstAccessibleKeyMIDINoteNumber(m_firstAccessibleKeyMIDINoteNumberSpin->GetValue());
	UpdateAnyGuiVersion();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ManualPanel::OnAccessibleKeysSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setNumberOfAccessibleKeys(m_numberOfAccessibleKeysSpin->GetValue());
	UpdateAnyGuiVersion();

	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ManualPanel::OnMidiInputNbrSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setMidiInputNumber(m_midiInputNumberSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ManualPanel::OnNewStopBtn(wxCommandEvent& WXUNUSED(event)) {
	if (::wxGetApp().m_frame->m_organ->getNumberOfStops() < 999) {
		int nbrToAdd = m_addStopSpin->GetValue();
		for (int i = 0; i < nbrToAdd; i++) {
			Stop stop;
			stop.setOwningManual(m_manual);
			stop.getInternalRank()->setFirstMidiNoteNumber(m_manual->getFirstAccessibleKeyMIDINoteNumber());
			::wxGetApp().m_frame->m_organ->addStop(stop);
			unsigned nbStops = ::wxGetApp().m_frame->m_organ->getNumberOfStops();
			if (nbStops > 0)
				m_manual->addStop(::wxGetApp().m_frame->m_organ->getOrganStopAt(nbStops - 1));

			::wxGetApp().m_frame->AddStopItemToTree();
		}
		::wxGetApp().m_frame->SelectStopItemInTree(nbrToAdd);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 stops!"), wxT("Too many stops"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void ManualPanel::OnNewCouplerBtn(wxCommandEvent& WXUNUSED(event)) {
	if (::wxGetApp().m_frame->m_organ->getNumberOfCouplers() < 999) {
		Coupler coupler;
		coupler.setOwningManual(m_manual);
		::wxGetApp().m_frame->m_organ->addCoupler(coupler);
		unsigned nbCouplers = ::wxGetApp().m_frame->m_organ->getNumberOfCouplers();
		if (nbCouplers > 0)
			m_manual->addCoupler(::wxGetApp().m_frame->m_organ->getOrganCouplerAt(nbCouplers - 1));

		::wxGetApp().m_frame->AddCouplerItemToTree();

	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 couplers!"), wxT("Too many couplers"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void ManualPanel::OnNewDivisionalBtn(wxCommandEvent& WXUNUSED(event)) {
	if (::wxGetApp().m_frame->m_organ->getNumberOfDivisionals() < 999) {
		Divisional div;
		div.setOwningManual(m_manual);
		::wxGetApp().m_frame->m_organ->addDivisional(div);
		unsigned nbDivisionals = ::wxGetApp().m_frame->m_organ->getNumberOfDivisionals();
		if (nbDivisionals > 0)
			m_manual->addDivisional(::wxGetApp().m_frame->m_organ->getOrganDivisionalAt(nbDivisionals - 1));

		::wxGetApp().m_frame->AddDivisionalItemToTree();

	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 divisionals!"), wxT("Too many divisionals"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void ManualPanel::OnAvailableTremSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedTremulants;
	m_availableTremulants->GetSelections(selectedTremulants);
	if (!selectedTremulants.IsEmpty())
		m_addReferencedTremulant->Enable(true);
}

void ManualPanel::OnReferencedTremSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedTremulants;
	m_referencedTremulants->GetSelections(selectedTremulants);
	if (!selectedTremulants.IsEmpty())
		m_removeReferencedTremulant->Enable(true);
}

void ManualPanel::OnAddReferencedTremBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedTremulants;
	m_availableTremulants->GetSelections(selectedTremulants);
	bool isValidAdd = false;
	if (!selectedTremulants.IsEmpty()) {
		for (unsigned i = 0; i < selectedTremulants.GetCount(); i++) {
			if (!m_manual->hasTremulantReference(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(selectedTremulants[i]))) {
				m_manual->addTremulant(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(selectedTremulants[i]));
				isValidAdd = true;
			}
		}
	}
	if (isValidAdd) {
		UpdateReferencedTremulants();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void ManualPanel::OnRemoveReferencedTremBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedTremulants;
	m_referencedTremulants->GetSelections(selectedTremulants);
	if (!selectedTremulants.IsEmpty()) {
		std::list<Tremulant*> tremulantsToRemove;
		for (unsigned i = 0; i < selectedTremulants.GetCount(); i++) {
			tremulantsToRemove.push_back(m_manual->getTremulantAt(selectedTremulants[i]));
		}
		for (Tremulant *t : tremulantsToRemove) {
			m_manual->removeTremulant(t);
		}
		UpdateReferencedTremulants();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void ManualPanel::OnAvailableSwitchSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_availableSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty())
		m_addReferencedSwitch->Enable(true);
}

void ManualPanel::OnReferencedSwitchSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_referencedSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty())
		m_removeReferencedSwitch->Enable(true);
}

void ManualPanel::OnAddReferencedSwitchBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_availableSwitches->GetSelections(selectedSwitches);
	bool isValidAdd = false;
	if (!selectedSwitches.IsEmpty()) {
		for (unsigned i = 0; i < selectedSwitches.GetCount(); i++) {
			if (!m_manual->hasGoSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selectedSwitches[i]))) {
				m_manual->addGoSwitch(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selectedSwitches[i]));
				isValidAdd = true;
			}
		}
	}
	if (isValidAdd) {
		UpdateReferencedSwitches();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void ManualPanel::OnRemoveReferencedSwitchBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedSwitches;
	m_referencedSwitches->GetSelections(selectedSwitches);
	if (!selectedSwitches.IsEmpty()) {
		std::list<GoSwitch*> switchesToRemove;
		for (unsigned i = 0; i < selectedSwitches.GetCount(); i++) {
			switchesToRemove.push_back(m_manual->getGoSwitchAt(selectedSwitches[i]));
		}
		for (GoSwitch *sw : switchesToRemove) {
			m_manual->removeGoSwitch(sw);
		}
		UpdateReferencedSwitches();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void ManualPanel::OnMidiKeyMapKeyChoice(wxCommandEvent& WXUNUSED(event)) {
	if (m_midiKeyMapKeys->GetCurrentSelection() != wxNOT_FOUND)
		m_midiKeyMapValue->SetValue(m_manual->getMidiKeyMapValue(m_midiKeyMapKeys->GetString(m_midiKeyMapKeys->GetCurrentSelection())));
}

void ManualPanel::OnMidiKeyMapValueSpin(wxSpinEvent& WXUNUSED(event)) {
	if (m_midiKeyMapKeys->GetCurrentSelection() != wxNOT_FOUND) {
		m_manual->setMidiKeyMapValue(m_midiKeyMapKeys->GetString(m_midiKeyMapKeys->GetCurrentSelection()), m_midiKeyMapValue->GetValue());
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void ManualPanel::OnRemoveManualBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this manual?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			DoRemoveManual();
			m_isFirstRemoval = false;
		}
	} else {
		DoRemoveManual();
	}
}

void ManualPanel::DoRemoveManual() {
	// remove all stops, couplers and divisionals that this manual "own" from the organ first
	// the remove methods for stops, couplers and divisionals will also remove any existing gui representations of them
	for (unsigned i = 0; i < m_manual->getNumberOfStops(); i++) {
		::wxGetApp().m_frame->m_organ->removeStop(m_manual->getStopAt(i));
	}
	for (unsigned i = 0; i < m_manual->getNumberOfCouplers(); i++) {
		::wxGetApp().m_frame->m_organ->removeCoupler(m_manual->getCouplerAt(i));
	}
	for (unsigned i = 0; i < m_manual->getNumberOfDivisionals(); i++) {
		::wxGetApp().m_frame->m_organ->removeDivisional(m_manual->getDivisionalAt(i));
	}
	// if any gui element is referencing this manual it should be removed too
	unsigned numberOfPanels = ::wxGetApp().m_frame->m_organ->getNumberOfPanels();
	for (unsigned i = 0; i < numberOfPanels; i++) {
		if (::wxGetApp().m_frame->m_organ->getOrganPanelAt(i)->hasItemAsGuiElement(m_manual)) {
			::wxGetApp().m_frame->m_organ->getOrganPanelAt(i)->removeItemFromPanel(m_manual);
			::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
		}
	}
	// check if this manual was the pedal
	if (m_manual->isThePedal())
		::wxGetApp().m_frame->m_organ->setHasPedals(false);
	// then finally remove this manual
	::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
}

void ManualPanel::UpdateReferencedTremulants() {
	wxArrayString referencedTremulants;
	unsigned nbTrems = m_manual->getNumberOfTremulants();
	if (nbTrems > 0) {
		for (unsigned i = 0; i < nbTrems; i++) {
			referencedTremulants.Add(m_manual->getTremulantAt(i)->getName());
		}
		m_referencedTremulants->Clear();
		m_referencedTremulants->InsertItems(referencedTremulants, 0);
	} else {
		m_referencedTremulants->Clear();
		m_removeReferencedTremulant->Disable();
	}
}

void ManualPanel::UpdateReferencedSwitches() {
	wxArrayString referencedSwitches;
	unsigned nbSwitches = m_manual->getNumberOfGoSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			referencedSwitches.Add(m_manual->getGoSwitchAt(i)->getName());
		}
		m_referencedSwitches->Clear();
		m_referencedSwitches->InsertItems(referencedSwitches, 0);
	} else {
		m_referencedSwitches->Clear();
		m_removeReferencedSwitch->Disable();
	}
}

void ManualPanel::UpdateAnyGuiVersion() {
	// notify any gui version of the manual that the number of keys might have changed
	for (unsigned i = 0; i < ::wxGetApp().m_frame->m_organ->getNumberOfPanels(); i++) {
		for (unsigned j = 0; j < ::wxGetApp().m_frame->m_organ->getOrganPanelAt(i)->getNumberOfManuals(); j++) {
			GUIManual *guiMan = ::wxGetApp().m_frame->m_organ->getOrganPanelAt(i)->getGuiManualAt(j);
			if (guiMan->isReferencing(m_manual)) {
				guiMan->setNumberOfDisplayKeys(m_manual->getNumberOfAccessibleKeys());
				guiMan->setDisplayFirstNote(m_manual->getFirstAccessibleKeyMIDINoteNumber());
			}
		}
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}
