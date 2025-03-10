/*
 * ManualKeyCopyDialog.cpp is part of GoOdf.
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

#include "ManualKeyCopyDialog.h"
#include "GOODFDef.h"
#include <wx/statline.h>

IMPLEMENT_CLASS(ManualKeyCopyDialog, wxDialog)

BEGIN_EVENT_TABLE(ManualKeyCopyDialog, wxDialog)
	EVT_LISTBOX(ID_MANUAL_KEY_COPY_DIALOG_BOX, ManualKeyCopyDialog::OnKeyChoice)
END_EVENT_TABLE()

ManualKeyCopyDialog::ManualKeyCopyDialog(GUIManual *man, int sourceIndex) {
	Init(man, sourceIndex);
}

ManualKeyCopyDialog::ManualKeyCopyDialog(
	GUIManual *man,
	int sourceIndex,
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style) {
	Init(man, sourceIndex);
	Create(parent, id, caption, pos, size, style);
}

ManualKeyCopyDialog::~ManualKeyCopyDialog() {

}

void ManualKeyCopyDialog::Init(GUIManual *man, int sourceIndex) {
	m_manual = man;
	m_sourceKeyIndex = sourceIndex;
	for (int i = 0; i < (int) m_manual->getNumberOfKeytypes(); i++) {
		if (i != m_sourceKeyIndex)
			m_keyStrings.Add(m_manual->getKeytypeAt(i)->KeytypeIdentifier);
		else
			m_keyStrings.Add(m_manual->getKeytypeAt(i)->KeytypeIdentifier + wxT(" (selected source key)"));
	}
}

bool ManualKeyCopyDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style ) {
	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	CreateControls();

	if (!m_keyStrings.IsEmpty()) {
		m_availableKeys->InsertItems(m_keyStrings, 0);
	}

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void ManualKeyCopyDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *infoText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Select the key(s) you want to copy to: ")
	);
	firstRow->Add(infoText, 0, wxALIGN_CENTER|wxALL, 5);
	mainSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	m_availableKeys = new wxListBox(
		this,
		ID_MANUAL_KEY_COPY_DIALOG_BOX,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	secondRow->Add(m_availableKeys, 1, wxGROW|wxALL, 5);
	mainSizer->Add(secondRow, 0, wxGROW);

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

bool ManualKeyCopyDialog::GetSelectedKeys(wxArrayInt& selectedKeys) {
	if (!selectedKeys.IsEmpty()) {
		selectedKeys.Empty();
	}
	int nbrSelectedKeys = m_availableKeys->GetSelections(selectedKeys);
	if (nbrSelectedKeys > 0) {
		int currentIndex = selectedKeys.GetCount() - 1;
		while (currentIndex >= 0) {
			if (selectedKeys.Item(currentIndex) == m_sourceKeyIndex) {
				selectedKeys.RemoveAt(currentIndex);
				m_availableKeys->Deselect(m_sourceKeyIndex);
			}
			currentIndex--;
		}
	}
	if (selectedKeys.GetCount() > 0)
		return true;
	else
		return false;
}

void ManualKeyCopyDialog::OnKeyChoice(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedTargetKeys;
	if (GetSelectedKeys(selectedTargetKeys)) {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Enable();
	} else {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Disable();
	}
}
