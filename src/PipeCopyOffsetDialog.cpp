/*
 * PipeCopyOffsetDialog.cpp is part of GoOdf.
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

#include "PipeCopyOffsetDialog.h"
#include "GOODFDef.h"
#include <wx/statline.h>

IMPLEMENT_CLASS(PipeCopyOffsetDialog, wxDialog)

BEGIN_EVENT_TABLE(PipeCopyOffsetDialog, wxDialog)
	EVT_LISTBOX(ID_PIPE_COPY_OFFSET_DIALOG_BOX, PipeCopyOffsetDialog::OnPipeSelection)
END_EVENT_TABLE()

PipeCopyOffsetDialog::PipeCopyOffsetDialog(int nbrOfPipes, int sourcePipeIndex) {
	Init(nbrOfPipes, sourcePipeIndex);
}

PipeCopyOffsetDialog::PipeCopyOffsetDialog(
	int nbrOfPipes,
	int sourcePipeIndex,
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style) {
	Init(nbrOfPipes, sourcePipeIndex);
	Create(parent, id, caption, pos, size, style);
}

PipeCopyOffsetDialog::~PipeCopyOffsetDialog() {

}

void PipeCopyOffsetDialog::Init(int nbrOfPipes, int sourcePipeIndex) {
	m_nbrOfPipes = nbrOfPipes;
	m_sourcePipeIndex = sourcePipeIndex;
	for (int i = 0; i < m_nbrOfPipes; i++) {
		int offset = i - sourcePipeIndex;
		if (i != sourcePipeIndex) {
			if (offset < -18 || offset > 18)
				m_pipeList.Add(wxString::Format(wxT("Pipe%0.3d (out of range)"), i + 1));
			else
				m_pipeList.Add(wxString::Format(wxT("Pipe%0.3d"), i + 1));
		} else
			m_pipeList.Add(wxString::Format(wxT("Pipe%0.3d (source pipe)"), i + 1));
	}
}

bool PipeCopyOffsetDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style ) {
	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	CreateControls();

	if (!m_pipeList.IsEmpty()) {
		m_pipeListbox->InsertItems(m_pipeList, 0);
	}
	m_pipeListbox->EnsureVisible(m_sourcePipeIndex);

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void PipeCopyOffsetDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxVERTICAL);
	wxStaticText *infoText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Select target pipes to copy to with offset:")
	);
	firstRow->Add(infoText, 0, wxALL, 5);
	m_pipeListbox = new wxListBox(
		this,
		ID_PIPE_COPY_OFFSET_DIALOG_BOX,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	firstRow->Add(m_pipeListbox, 1, wxGROW|wxALL, 5);
	mainSizer->Add(firstRow, 1, wxGROW|wxALL, 5);

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

bool PipeCopyOffsetDialog::GetSelectedIndices(wxArrayInt& selectedPipes) {
	if (!selectedPipes.IsEmpty()) {
		selectedPipes.Empty();
	}

	int nbrSelected = m_pipeListbox->GetSelections(selectedPipes);
	if (nbrSelected > 0)
		return true;
	else
		return false;
}

void PipeCopyOffsetDialog::OnPipeSelection(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selections;
	int count = m_pipeListbox->GetSelections(selections);

	for (int i = 0; i < count; i++) {
		int pipeIndex = selections[i];
		int offsetToSourcePipe = pipeIndex - m_sourcePipeIndex;
		if (pipeIndex == m_sourcePipeIndex || offsetToSourcePipe < -18 || offsetToSourcePipe > 18) {
			m_pipeListbox->Deselect(pipeIndex);
		}
	}

	wxArrayInt validSelections;
	int validCount = m_pipeListbox->GetSelections(validSelections);
	if (validCount > 0) {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Enable();
	} else {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Disable();
	}
}
