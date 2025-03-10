/*
 * CopyElementAttributesDialog.cpp is part of GoOdf.
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

#include "CopyElementAttributesDialog.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>

IMPLEMENT_CLASS(CopyElementAttributesDialog, wxDialog)

BEGIN_EVENT_TABLE(CopyElementAttributesDialog, wxDialog)
	EVT_CHOICE(ID_ELEMENT_COPY_DIALOG_PANELCHOICE, CopyElementAttributesDialog::OnPanelChoice)
	EVT_LISTBOX(ID_ELEMENT_COPY_DIALOG_BOX, CopyElementAttributesDialog::OnElementChoice)
END_EVENT_TABLE()

CopyElementAttributesDialog::CopyElementAttributesDialog(GoPanel *sourcePanel, int sourceElementIndex) {
	Init(sourcePanel, sourceElementIndex);
}

CopyElementAttributesDialog::CopyElementAttributesDialog(
	GoPanel *sourcePanel,
	int sourceElementIndex,
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style) {
	Init(sourcePanel, sourceElementIndex);
	Create(parent, id, caption, pos, size, style);
}

CopyElementAttributesDialog::~CopyElementAttributesDialog() {

}

void CopyElementAttributesDialog::Init(GoPanel *sourcePanel, int sourceElementIndex) {
	m_sourceElementIndex = sourceElementIndex;
	m_sourcePanel = sourcePanel;
	for (unsigned i = 0; i < ::wxGetApp().m_frame->m_organ->getNumberOfPanels(); i++) {
		m_panelList.Add(::wxGetApp().m_frame->m_organ->getOrganPanelAt(i)->getName());
	}
}

bool CopyElementAttributesDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style ) {
	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	CreateControls();

	m_panelChoice->Select(m_panelList.Index(m_sourcePanel->getName()));
	updateAvailableGuiElements();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void CopyElementAttributesDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *panelText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Target panel: ")
	);
	firstRow->Add(panelText, 0, wxALIGN_CENTER|wxALL, 5);
	m_panelChoice = new wxChoice(
		this,
		ID_ELEMENT_COPY_DIALOG_PANELCHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_panelList
	);
	firstRow->Add(m_panelChoice, 1, wxGROW|wxALL, 5);
	mainSizer->Add(firstRow, 0, wxGROW|wxALL, 5);

	wxBoxSizer *secondRow = new wxBoxSizer(wxVERTICAL);
	wxStaticText *elementText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Select GUI elements to copy to:")
	);
	secondRow->Add(elementText, 0, wxALL, 5);
	m_availableElements = new wxListBox(
		this,
		ID_ELEMENT_COPY_DIALOG_BOX,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	secondRow->Add(m_availableElements, 1, wxGROW|wxALL, 5);
	mainSizer->Add(secondRow, 1, wxGROW|wxALL, 5);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	mainSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	wxButton *theCancelButton = new wxButton(
		this,
		wxID_CANCEL,
		wxT("Cancel")
	);
	bottomRow->Add(theCancelButton, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	wxButton *theOkButton = new wxButton(
		this,
		wxID_OK,
		wxT("Copy to selected")
	);
	theOkButton->Disable();
	bottomRow->Add(theOkButton, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	mainSizer->Add(bottomRow, 0, wxGROW);

	SetSizer(mainSizer);
}

GoPanel* CopyElementAttributesDialog::GetSelectedTargetPanel() {
	return ::wxGetApp().m_frame->m_organ->getOrganPanelAt((unsigned) m_panelChoice->GetSelection());
}

bool CopyElementAttributesDialog::GetSelectedElementIndices(wxArrayInt& selectedElements) {
	if (!selectedElements.IsEmpty()) {
		selectedElements.Empty();
	}
	wxArrayInt selectedItemsInList;
	int nbrSelected = m_availableElements->GetSelections(selectedItemsInList);
	if (nbrSelected > 0) {
		// now we need to translate the selected items to what indices they actually refer to
		for (int i = 0; i < nbrSelected; i++) {
			selectedElements.Add(m_referenceElementIdx[selectedItemsInList[i]]);
		}
		return true;
	} else {
		return false;
	}
}

void CopyElementAttributesDialog::OnPanelChoice(wxCommandEvent& WXUNUSED(event)) {
	updateAvailableGuiElements();
}

void CopyElementAttributesDialog::OnElementChoice(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selections;
	int count = m_availableElements->GetSelections(selections);
	if (count > 0) {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Enable();
	} else {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Disable();
	}
}

void CopyElementAttributesDialog::updateAvailableGuiElements() {
	// This method will populate the list of available elements for this panel
	if (!m_matchingElementsList.IsEmpty()) {
		m_matchingElementsList.Empty();
	}

	if (!m_referenceElementIdx.empty()) {
		m_referenceElementIdx.clear();
	}

	if (!m_availableElements->IsEmpty()) {
		m_availableElements->Clear();
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Disable();
	}

	GoPanel *currentTargetPanel = GetSelectedTargetPanel();
	GUIElement *sourceElement = m_sourcePanel->getGuiElementAt(m_sourceElementIndex);

	int nbrGuiElements = currentTargetPanel->getNumberOfGuiElements();
	for (int i = 0; i < nbrGuiElements; i++) {
		if (currentTargetPanel == m_sourcePanel && i == m_sourceElementIndex)
			continue;

		GUIElement *candidateElement = currentTargetPanel->getGuiElementAt(i);

		if ((dynamic_cast<GUIButton*>(sourceElement) != nullptr && dynamic_cast<GUIButton*>(candidateElement) != nullptr) ||
			(dynamic_cast<GUIEnclosure*>(sourceElement) != nullptr && dynamic_cast<GUIEnclosure*>(candidateElement) != nullptr) ||
			(dynamic_cast<GUILabel*>(sourceElement) != nullptr && dynamic_cast<GUILabel*>(candidateElement) != nullptr) ||
			(dynamic_cast<GUIManual*>(sourceElement) != nullptr && dynamic_cast<GUIManual*>(candidateElement) != nullptr)
		) {
			m_matchingElementsList.Add(currentTargetPanel->getGuiElementAt(i)->getDisplayName());
			m_referenceElementIdx.push_back(i);
		}

	}

	if (!m_matchingElementsList.IsEmpty()) {
		m_availableElements->InsertItems(m_matchingElementsList, 0);
	}
}
