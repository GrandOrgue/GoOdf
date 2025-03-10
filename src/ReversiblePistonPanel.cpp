/*
 * ReversiblePistonPanel.cpp is part of GoOdf.
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

#include "ReversiblePistonPanel.h"
#include "GOODFDef.h"
#include "GOODFFunctions.h"
#include "GOODF.h"
#include <wx/statline.h>

// Event table
BEGIN_EVENT_TABLE(ReversiblePistonPanel, wxPanel)
	EVT_TEXT(ID_PISTON_NAME_TEXT, ReversiblePistonPanel::OnNameChange)
	EVT_RADIOBUTTON(ID_PISTON_INVERTED_STATE_YES, ReversiblePistonPanel::OnDisplayInvertedRadio)
	EVT_RADIOBUTTON(ID_PISTON_INVERTED_STATE_NO, ReversiblePistonPanel::OnDisplayInvertedRadio)
	EVT_LISTBOX(ID_PISTON_AVAILABLE_STOPS, ReversiblePistonPanel::OnStopListboxSelection)
	EVT_LISTBOX(ID_PISTON_AVAILABLE_COUPLERS, ReversiblePistonPanel::OnCouplerListboxSelection)
	EVT_LISTBOX(ID_PISTON_AVAILABLE_TREMS, ReversiblePistonPanel::OnTremulantListboxSelection)
	EVT_LISTBOX(ID_PISTON_AVAILABLE_SWITCHES, ReversiblePistonPanel::OnSwitchListboxSelection)
	EVT_BUTTON(ID_PISTON_USE_SELECTED_BTN, ReversiblePistonPanel::OnUseSelectedBtn)
	EVT_BUTTON(ID_PISTON_REMOVE_BTN, ReversiblePistonPanel::OnRemovePistonBtn)
END_EVENT_TABLE()

ReversiblePistonPanel::ReversiblePistonPanel(wxWindow *parent) : wxPanel(parent) {
	m_piston = NULL;
	m_isFirstRemoval = true;

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *pistonNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Piston name: ")
	);
	firstRow->Add(pistonNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_PISTON_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *invertedRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *dispInvertedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Display in inverted state: ")
	);
	invertedRow->Add(dispInvertedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedYes = new wxRadioButton(
		this,
		ID_PISTON_INVERTED_STATE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	invertedRow->Add(m_displayInvertedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo = new wxRadioButton(
		this,
		ID_PISTON_INVERTED_STATE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	invertedRow->Add(m_displayInvertedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo->SetValue(true);
	panelSizer->Add(invertedRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *secondRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableStopsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available stops")
	);
	secondRow1stCol->Add(availableStopsText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableStops = new wxListBox(
		this,
		ID_PISTON_AVAILABLE_STOPS
	);
	secondRow1stCol->Add(m_availableStops, 1, wxEXPAND|wxALL, 5);
	secondRow->Add(secondRow1stCol, 1, wxEXPAND);

	wxBoxSizer *secondRow2ndCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableCouplersText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available couplers")
	);
	secondRow2ndCol->Add(availableCouplersText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableCouplers = new wxListBox(
		this,
		ID_PISTON_AVAILABLE_COUPLERS
	);
	secondRow2ndCol->Add(m_availableCouplers, 1, wxEXPAND|wxALL, 5);
	secondRow->Add(secondRow2ndCol, 1, wxEXPAND);
	wxBoxSizer *secondRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available switches")
	);
	secondRow3rdCol->Add(availableReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableSwitches = new wxListBox(
		this,
		ID_PISTON_AVAILABLE_SWITCHES
	);
	secondRow3rdCol->Add(m_availableSwitches, 1, wxEXPAND|wxALL, 5);
	secondRow->Add(secondRow3rdCol, 1, wxEXPAND);
	wxBoxSizer *secondRow4thCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableTremulantsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available tremulants")
	);
	secondRow4thCol->Add(availableTremulantsText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableTremulants = new wxListBox(
		this,
		ID_PISTON_AVAILABLE_TREMS
	);
	secondRow4thCol->Add(m_availableTremulants, 1, wxEXPAND|wxALL, 5);
	secondRow->Add(secondRow4thCol, 1, wxEXPAND);
	panelSizer->Add(secondRow, 1, wxEXPAND);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	thirdRow->AddStretchSpacer();
	useSelectedBtn = new wxButton(
		this,
		ID_PISTON_USE_SELECTED_BTN,
		wxT("Use selected item for this reversible piston")
	);
	thirdRow->Add(useSelectedBtn, 0, wxALIGN_CENTER|wxALL, 10);
	thirdRow->AddStretchSpacer();
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *objectTypeText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Object type: ")
	);
	fourthRow->Add(objectTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_objecTypeField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fourthRow->Add(m_objecTypeField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *manualText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Manual: ")
	);
	fifthRow->Add(manualText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_manualField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fifthRow->Add(m_manualField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removePistonBtn = new wxButton(
		this,
		ID_PISTON_REMOVE_BTN,
		wxT("Remove this reversible piston")
	);
	bottomRow->Add(removePistonBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

ReversiblePistonPanel::~ReversiblePistonPanel() {

}

void ReversiblePistonPanel::setReversiblePiston(ReversiblePiston *piston) {
	m_piston = piston;
	m_nameField->ChangeValue(m_piston->getName());
	if (m_piston->isDisplayedInverted())
		m_displayInvertedYes->SetValue(true);
	else
		m_displayInvertedNo->SetValue(true);
	// update/populate available stops
	wxArrayString organStops;
	unsigned nbStops = ::wxGetApp().m_frame->m_organ->getNumberOfStops();
	if (nbStops > 0) {
		for (unsigned i = 0; i < nbStops; i++) {
			organStops.Add(::wxGetApp().m_frame->m_organ->getOrganStopAt(i)->getName());
		}
		m_availableStops->Clear();
		m_availableStops->InsertItems(organStops, 0);
	} else {
		m_availableStops->Clear();
	}

	// update/populate available couplers
	wxArrayString organCouplers;
	unsigned nbCouplers = ::wxGetApp().m_frame->m_organ->getNumberOfCouplers();
	if (nbCouplers > 0) {
		for (unsigned i = 0; i < nbCouplers; i++) {
			organCouplers.Add(::wxGetApp().m_frame->m_organ->getOrganCouplerAt(i)->getName());
		}
		m_availableCouplers->Clear();
		m_availableCouplers->InsertItems(organCouplers, 0);
	} else {
		m_availableCouplers->Clear();
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
		m_availableSwitches->Clear();
	}

	// update/populate available tremulants
	wxArrayString organTrems;
	unsigned nbTrems = ::wxGetApp().m_frame->m_organ->getNumberOfTremulants();
	if (nbTrems > 0) {
		for (unsigned i = 0; i < nbTrems; i++) {
			organTrems.Add(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(i)->getName());
		}
		m_availableTremulants->Clear();
		m_availableTremulants->InsertItems(organTrems, 0);
	} else {
		m_availableTremulants->Clear();
	}

	// from the piston a selection is made and the fields will be populated
	if (m_piston->getStop()) {
		m_availableStops->Select(::wxGetApp().m_frame->m_organ->getIndexOfOrganStop(m_piston->getStop()) - 1);
	} else if (m_piston->getCoupler()) {
		m_availableCouplers->Select(::wxGetApp().m_frame->m_organ->getIndexOfOrganCoupler(m_piston->getCoupler()) - 1);
	} else if (m_piston->getSwitch()) {
		m_availableSwitches->Select(::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(m_piston->getSwitch()) - 1);
	} else if (m_piston->getTremulant()) {
		m_availableTremulants->Select(::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(m_piston->getTremulant()) - 1);
	} else {
		if (m_availableStops->GetSelection() != wxNOT_FOUND)
			m_availableStops->Deselect(m_availableStops->GetSelection());
		if (m_availableCouplers->GetSelection() != wxNOT_FOUND)
			m_availableCouplers->Deselect(m_availableCouplers->GetSelection());
		if (m_availableSwitches->GetSelection() != wxNOT_FOUND)
			m_availableSwitches->Deselect(m_availableSwitches->GetSelection());
		if (m_availableTremulants->GetSelection() != wxNOT_FOUND)
			m_availableTremulants->Deselect(m_availableTremulants->GetSelection());
	}
	UpdateInfoFields();

	useSelectedBtn->Disable();
}

void ReversiblePistonPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void ReversiblePistonPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_nameField->SetToolTip(wxT("It's best to make the name unique. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_displayInvertedYes->SetToolTip(wxT("Yes means that the on and off bitmaps are reversed."));
		m_displayInvertedNo->SetToolTip(wxT("No is the standard on/off bitmap display."));
		m_availableStops->SetToolTip(wxT("Here all available elements are listed that the reversible piston can target. Select one element and click the button below to use it as target. NOTE: Only one element can be targeted and make sure that the target element can receive 'Input'! This is a legacy feature of GrandOrgue, the same result can be achieved with just creating another GUI Element for the target element and style it appropriately!"));
		m_availableCouplers->SetToolTip(wxT("Here all available elements are listed that the reversible piston can target. Select one element and click the button below to use it as target. NOTE: Only one element can be targeted and make sure that the target element can receive 'Input'! This is a legacy feature of GrandOrgue, the same result can be achieved with just creating another GUI Element for the target element and style it appropriately!"));
		m_availableSwitches->SetToolTip(wxT("Here all available elements are listed that the reversible piston can target. Select one element and click the button below to use it as target. NOTE: Only one element can be targeted and make sure that the target element can receive 'Input'! This is a legacy feature of GrandOrgue, the same result can be achieved with just creating another GUI Element for the target element and style it appropriately!"));
		m_availableTremulants->SetToolTip(wxT("Here all available elements are listed that the reversible piston can target. Select one element and click the button below to use it as target. NOTE: Only one element can be targeted and make sure that the target element can receive 'Input'! This is a legacy feature of GrandOrgue, the same result can be achieved with just creating another GUI Element for the target element and style it appropriately!"));
		m_objecTypeField->SetToolTip(wxT("This textfield for object type is automatically populated depending on what element is chosen."));
		m_manualField->SetToolTip(wxT("This textfield for manual is automatically populated depending on what element is chosen."));
	} else {
		m_nameField->SetToolTip(wxEmptyString);
		m_displayInvertedYes->SetToolTip(wxEmptyString);
		m_displayInvertedNo->SetToolTip(wxEmptyString);
		m_availableStops->SetToolTip(wxEmptyString);
		m_availableCouplers->SetToolTip(wxEmptyString);
		m_availableSwitches->SetToolTip(wxEmptyString);
		m_availableTremulants->SetToolTip(wxEmptyString);
		m_objecTypeField->SetToolTip(wxEmptyString);
		m_manualField->SetToolTip(wxEmptyString);
	}
}

void ReversiblePistonPanel::refreshData() {
	m_nameField->ChangeValue(m_piston->getName());
}

void ReversiblePistonPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_piston->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void ReversiblePistonPanel::OnDisplayInvertedRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_PISTON_INVERTED_STATE_YES) {
		m_displayInvertedYes->SetValue(true);
		m_piston->setDisplayInverted(true);
	} else {
		m_displayInvertedNo->SetValue(true);
		m_piston->setDisplayInverted(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ReversiblePistonPanel::OnStopListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableStops->GetSelection() != wxNOT_FOUND) {
		m_availableCouplers->SetSelection(wxNOT_FOUND);
		m_availableSwitches->SetSelection(wxNOT_FOUND);
		m_availableTremulants->SetSelection(wxNOT_FOUND);

		useSelectedBtn->Enable(true);
	}
}

void ReversiblePistonPanel::OnCouplerListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableCouplers->GetSelection() != wxNOT_FOUND) {
		m_availableStops->SetSelection(wxNOT_FOUND);
		m_availableSwitches->SetSelection(wxNOT_FOUND);
		m_availableTremulants->SetSelection(wxNOT_FOUND);

		useSelectedBtn->Enable(true);
	}
}

void ReversiblePistonPanel::OnSwitchListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableSwitches->GetSelection() != wxNOT_FOUND) {
		m_availableCouplers->SetSelection(wxNOT_FOUND);
		m_availableStops->SetSelection(wxNOT_FOUND);
		m_availableTremulants->SetSelection(wxNOT_FOUND);

		useSelectedBtn->Enable(true);
	}
}

void ReversiblePistonPanel::OnTremulantListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableTremulants->GetSelection() != wxNOT_FOUND) {
		m_availableStops->SetSelection(wxNOT_FOUND);
		m_availableCouplers->SetSelection(wxNOT_FOUND);
		m_availableSwitches->SetSelection(wxNOT_FOUND);

		useSelectedBtn->Enable(true);
	}
}

void ReversiblePistonPanel::OnUseSelectedBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableStops->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableStops->GetSelection();
		m_piston->setStop(::wxGetApp().m_frame->m_organ->getOrganStopAt(selected));
	} else if (m_availableCouplers->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableCouplers->GetSelection();
		m_piston->setCoupler(::wxGetApp().m_frame->m_organ->getOrganCouplerAt(selected));
	} else if (m_availableSwitches->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableSwitches->GetSelection();
		m_piston->setSwitch(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selected));
	} else if (m_availableTremulants->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableTremulants->GetSelection();
		m_piston->setTremulant(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(selected));
	}
	UpdateInfoFields();
	useSelectedBtn->Disable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ReversiblePistonPanel::OnRemovePistonBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this reversible piston?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
			m_isFirstRemoval = false;
		}
	} else {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void ReversiblePistonPanel::UpdateInfoFields() {
	m_objecTypeField->SetValue(m_piston->getObjecType());
	m_manualField->SetValue(m_piston->getManual());
}
