/*
 * EnclosurePanel.cpp is part of GOODF.
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

#include "EnclosurePanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"

// Event table
BEGIN_EVENT_TABLE(EnclosurePanel, wxPanel)
	EVT_TEXT(ID_ENCLOSURE_NAME_TEXT, EnclosurePanel::OnNameChange)
	EVT_SPINCTRL(ID_ENCLOSURE_AMP_MIN_LVL_SPIN, EnclosurePanel::OnAmpMinLvlChange)
	EVT_BUTTON(ID_ENCLOSURE_REMOVE_BTN, EnclosurePanel::OnRemoveEnclosureBtn)
END_EVENT_TABLE()

EnclosurePanel::EnclosurePanel(wxWindow *parent) : wxPanel (
		parent,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxTAB_TRAVERSAL|wxNO_BORDER
	) {

	m_enclosure = NULL;
	m_isFirstRemoval = true;

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *enclosureNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Enclosure name: ")
	);
	firstRow->Add(enclosureNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_ENCLOSURE_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *ampMinLvlText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Amplitude minimum level: ")
	);
	secondRow->Add(ampMinLvlText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_ampMinLvlSpin = new wxSpinCtrl(
		this,
		ID_ENCLOSURE_AMP_MIN_LVL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		100,
		0
	);
	secondRow->Add(m_ampMinLvlSpin, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeEnclosureBtn = new wxButton(
		this,
		ID_ENCLOSURE_REMOVE_BTN,
		wxT("Remove this enclosure")
	);
	bottomRow->Add(removeEnclosureBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

EnclosurePanel::~EnclosurePanel() {

}

void EnclosurePanel::setEnclosure(Enclosure *enclosure) {
	m_enclosure = enclosure;
	m_nameField->SetValue(m_enclosure->getName());
	m_ampMinLvlSpin->SetValue(m_enclosure->getAmpMinimumLevel());
}

void EnclosurePanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void EnclosurePanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_enclosure->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void EnclosurePanel::OnAmpMinLvlChange(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setAmpMinimumLevel(m_ampMinLvlSpin->GetValue());
}

void EnclosurePanel::OnRemoveEnclosureBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this enclosure?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			DoRemoveEnclosure();
			m_isFirstRemoval = false;
		}
	} else {
		DoRemoveEnclosure();
	}
}

void EnclosurePanel::DoRemoveEnclosure() {
	// remove all possible references to this enclosure in any windchest first
	unsigned numberOfWindchests = ::wxGetApp().m_frame->m_organ->getNumberOfWindchestgroups();
	for (unsigned i = 0; i < numberOfWindchests; i++) {
		if (::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(i)->hasEnclosureReference(m_enclosure)) {
			::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(i)->removeEnclosureReference(m_enclosure);
		}
	}
	// remove any gui element that reference this enclosure
	unsigned numberOfPanels = ::wxGetApp().m_frame->m_organ->getNumberOfPanels();
	for (unsigned i = 0; i < numberOfPanels; i++) {
		if (::wxGetApp().m_frame->m_organ->getOrganPanelAt(i)->hasItemAsGuiElement(m_enclosure)) {
			::wxGetApp().m_frame->m_organ->getOrganPanelAt(i)->removeItemFromPanel(m_enclosure);
			::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
		}
	}
	// then remove this item from the organ
	::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
}
