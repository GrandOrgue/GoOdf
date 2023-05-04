/*
 * WindchestgroupPanel.cpp is part of GOODF.
 * Copyright (C) 2023 Lars Palo and contributors (see AUTHORS)
 *
 * GOODF is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GOODF is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GOODF.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#include "WindchestgroupPanel.h"
#include "GOODFDef.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

// Event table
BEGIN_EVENT_TABLE(WindchestgroupPanel, wxPanel)
	EVT_TEXT(ID_WINDCHEST_NAME_TEXT, WindchestgroupPanel::OnNameChange)
	EVT_BUTTON(ID_WC_ADD_ENCLOSURE_BTN, WindchestgroupPanel::OnAddReferencedEnclosure)
	EVT_BUTTON(ID_WC_REMOVE_ENCLOSURE_BTN, WindchestgroupPanel::OnRemoveReferencedEnclosure)
	EVT_BUTTON(ID_WC_ADD_TREMULANT_BTN, WindchestgroupPanel::OnAddReferencedTremulant)
	EVT_BUTTON(ID_WC_REMOVE_TREMULANT_BTN, WindchestgroupPanel::OnRemoveReferencedTremulant)
	EVT_BUTTON(ID_WINDCHEST_REMOVE_BTN, WindchestgroupPanel::OnRemoveWindchestBtn)
	EVT_LISTBOX(ID_WC_AVAILABLE_ENCLOSURES, WindchestgroupPanel::OnEnclosureListboxSelection)
	EVT_LISTBOX(ID_WC_AVAILABLE_TREMULANTS, WindchestgroupPanel::OnTremulantListboxSelection)
	EVT_LISTBOX(ID_WC_REFERENCED_ENCLOSURES, WindchestgroupPanel::OnReferencedEnclosureSelection)
	EVT_LISTBOX(ID_WC_REFERENCED_TREMULANTS, WindchestgroupPanel::OnReferencedTremulantSelection)
END_EVENT_TABLE()

WindchestgroupPanel::WindchestgroupPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY) {
	m_windchest = NULL;
	m_isFirstRemoval = true;

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *windchestNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Windchestgroup name: ")
	);
	firstRow->Add(windchestNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_WINDCHEST_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *secondRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableRefEnclosuresText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available enclosures")
	);
	secondRow1stCol->Add(availableRefEnclosuresText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableEnclosures = new wxListBox(
		this,
		ID_WC_AVAILABLE_ENCLOSURES
	);
	secondRow1stCol->Add(m_availableEnclosures, 1, wxEXPAND|wxALL, 5);
	secondRow->Add(secondRow1stCol, 1, wxEXPAND);
	wxBoxSizer *secondRow2ndCol = new wxBoxSizer(wxVERTICAL);
	secondRow2ndCol->AddStretchSpacer();
	m_addReferencedEnclosure = new wxButton(
		this,
		ID_WC_ADD_ENCLOSURE_BTN,
		wxT("Add ->")
	);
	secondRow2ndCol->Add(m_addReferencedEnclosure, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	m_removeReferencedEnclosure = new wxButton(
		this,
		ID_WC_REMOVE_ENCLOSURE_BTN,
		wxT("<- Remove")
	);
	secondRow2ndCol->Add(m_removeReferencedEnclosure, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	secondRow2ndCol->AddStretchSpacer();
	secondRow->Add(secondRow2ndCol, 0, wxEXPAND|wxALIGN_CENTER);
	wxBoxSizer *secondRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenReferencedEncText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced enclosures")
	);
	secondRow3rdCol->Add(chosenReferencedEncText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedEnclosures = new wxListBox(
		this,
		ID_WC_REFERENCED_ENCLOSURES
	);
	secondRow3rdCol->Add(m_referencedEnclosures, 1, wxEXPAND|wxALL, 5);
	secondRow->Add(secondRow3rdCol, 1, wxEXPAND);
	panelSizer->Add(secondRow, 1, wxEXPAND);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *thirdRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableRefTremText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available tremulants")
	);
	thirdRow1stCol->Add(availableRefTremText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableTremulants = new wxListBox(
		this,
		ID_WC_AVAILABLE_TREMULANTS
	);
	thirdRow1stCol->Add(m_availableTremulants, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow1stCol, 1, wxEXPAND);
	wxBoxSizer *thirdRow2ndCol = new wxBoxSizer(wxVERTICAL);
	thirdRow2ndCol->AddStretchSpacer();
	m_addReferencedTremulant = new wxButton(
		this,
		ID_WC_ADD_TREMULANT_BTN,
		wxT("Add ->")
	);
	thirdRow2ndCol->Add(m_addReferencedTremulant, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	m_removeReferencedTremulant = new wxButton(
		this,
		ID_WC_REMOVE_TREMULANT_BTN,
		wxT("<- Remove")
	);
	thirdRow2ndCol->Add(m_removeReferencedTremulant, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	thirdRow2ndCol->AddStretchSpacer();
	thirdRow->Add(thirdRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *thirdRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced tremulants")
	);
	thirdRow3rdCol->Add(chosenReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedTremulants = new wxListBox(
		this,
		ID_WC_REFERENCED_TREMULANTS
	);
	thirdRow3rdCol->Add(m_referencedTremulants, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow3rdCol, 1, wxEXPAND);
	panelSizer->Add(thirdRow, 1, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeWindchestBtn = new wxButton(
		this,
		ID_WINDCHEST_REMOVE_BTN,
		wxT("Remove this windchestgroup")
	);
	bottomRow->Add(removeWindchestBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

WindchestgroupPanel::~WindchestgroupPanel() {

}

void WindchestgroupPanel::setWindchest(Windchestgroup *windchest) {
	m_windchest = windchest;
	m_nameField->SetValue(m_windchest->getName());

	// Update/populate available enclosures
	wxArrayString organEnclosures;
	unsigned nbEnc = ::wxGetApp().m_frame->m_organ->getNumberOfEnclosures();
	if (nbEnc > 0) {
		for (unsigned i = 0; i < nbEnc; i++) {
			organEnclosures.Add(::wxGetApp().m_frame->m_organ->getOrganEnclosureAt(i)->getName());
		}
		m_availableEnclosures->Clear();
		m_availableEnclosures->InsertItems(organEnclosures, 0);
	} else {
		m_availableEnclosures->Clear();
	}

	// Update/populate available tremulants
	wxArrayString organTremulants;
	unsigned nbTrems = ::wxGetApp().m_frame->m_organ->getNumberOfTremulants();
	if (nbTrems > 0) {
		for (unsigned i = 0; i < nbTrems; i++) {
			organTremulants.Add(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(i)->getName());
		}
		m_availableTremulants->Clear();
		m_availableTremulants->InsertItems(organTremulants, 0);
	} else {
		m_availableTremulants->Clear();
	}

	UpdateReferencedEnclosures();
	UpdateReferencedTremulants();

	m_addReferencedEnclosure->Disable();
	m_removeReferencedEnclosure->Disable();
	m_addReferencedTremulant->Disable();
	m_removeReferencedTremulant->Disable();
}

void WindchestgroupPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void WindchestgroupPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_windchest->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
}

void WindchestgroupPanel::OnAddReferencedEnclosure(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableEnclosures->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableEnclosures->GetSelection();
		if (!m_windchest->hasEnclosureReference(::wxGetApp().m_frame->m_organ->getOrganEnclosureAt(selected))) {
			m_windchest->addEnclosureReference(::wxGetApp().m_frame->m_organ->getOrganEnclosureAt(selected));
			UpdateReferencedEnclosures();
		}
	}
}

void WindchestgroupPanel::OnRemoveReferencedEnclosure(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedEnclosures->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedEnclosures->GetSelection();
		m_windchest->removeEnclosureReference(m_windchest->getEnclosureAt(selected));
		UpdateReferencedEnclosures();
	}
}

void WindchestgroupPanel::OnAddReferencedTremulant(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableTremulants->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableTremulants->GetSelection();
		if (!m_windchest->hasTremulantReference(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(selected))) {
			m_windchest->addTremulantReference(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(selected));
			UpdateReferencedTremulants();
		}
	}
}

void WindchestgroupPanel::OnRemoveReferencedTremulant(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedTremulants->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedTremulants->GetSelection();
		m_windchest->removeTremulantReference(m_windchest->getTremulantAt(selected));
		UpdateReferencedTremulants();
	}
}

void WindchestgroupPanel::OnRemoveWindchestBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this windchestgroup?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			DoRemoveWindchest();
			m_isFirstRemoval = false;
		}
	} else {
		DoRemoveWindchest();
	}
}

void WindchestgroupPanel::DoRemoveWindchest() {
	// first remove all possible references to this windchest from ranks
	unsigned numberOfRanks = ::wxGetApp().m_frame->m_organ->getNumberOfRanks();
	for (unsigned i = 0; i < numberOfRanks; i++) {
		if (::wxGetApp().m_frame->m_organ->getOrganRankAt(i)->getWindchest() == m_windchest) {
			::wxGetApp().m_frame->m_organ->getOrganRankAt(i)->setWindchest(NULL);
		}
		// also remove any possible reference in any pipe of this rank
		for (Pipe p : ::wxGetApp().m_frame->m_organ->getOrganRankAt(i)->m_pipes) {
			if (p.windchest == m_windchest)
				p.windchest = NULL;
		}
	}
	::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
}

void WindchestgroupPanel::OnEnclosureListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableEnclosures->GetSelection() != wxNOT_FOUND) {
		m_addReferencedEnclosure->Enable(true);
	}
}

void WindchestgroupPanel::OnTremulantListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableTremulants->GetSelection() != wxNOT_FOUND) {
		m_addReferencedTremulant->Enable(true);
	}
}

void WindchestgroupPanel::UpdateReferencedEnclosures() {
	wxArrayString referencedEnclosures;
	unsigned nbEnc = m_windchest->getNumberOfEnclosures();
	if (nbEnc > 0) {
		for (unsigned i = 0; i < nbEnc; i++) {
			referencedEnclosures.Add(m_windchest->getEnclosureAt(i)->getName());
		}
		m_referencedEnclosures->Clear();
		m_referencedEnclosures->InsertItems(referencedEnclosures, 0);
	} else {
		m_referencedEnclosures->Clear();
		m_removeReferencedEnclosure->Disable();
	}
}

void WindchestgroupPanel::OnReferencedEnclosureSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedEnclosures->GetSelection() != wxNOT_FOUND) {
		m_removeReferencedEnclosure->Enable(true);
	}
}

void WindchestgroupPanel::OnReferencedTremulantSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedTremulants->GetSelection() != wxNOT_FOUND) {
		m_removeReferencedTremulant->Enable(true);
	}
}

void WindchestgroupPanel::UpdateReferencedTremulants() {
	wxArrayString referencedTremulants;
	unsigned nbTrems = m_windchest->getNumberOfTremulants();
	if (nbTrems > 0) {
		for (unsigned i = 0; i < nbTrems; i++) {
			referencedTremulants.Add(m_windchest->getTremulantAt(i)->getName());
		}
		m_referencedTremulants->Clear();
		m_referencedTremulants->InsertItems(referencedTremulants, 0);
	} else {
		m_referencedTremulants->Clear();
		m_removeReferencedTremulant->Disable();
	}
}
