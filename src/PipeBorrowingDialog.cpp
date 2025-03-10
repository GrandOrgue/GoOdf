/*
 * PipeBorrowingDialog.cpp is part of GoOdf.
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

#include "PipeBorrowingDialog.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>

IMPLEMENT_CLASS(PipeBorrowingDialog, wxDialog)

BEGIN_EVENT_TABLE(PipeBorrowingDialog, wxDialog)
	EVT_CHOICE(ID_BORROWING_DIALOG_MANUAL_CHOICE, PipeBorrowingDialog::OnManualChoice)
	EVT_CHOICE(ID_BORROWING_DIALOG_STOP_CHOICE, PipeBorrowingDialog::OnStopChoice)
	EVT_SPINCTRL(ID_BORROWING_DIALOG_PIPE_SPIN, PipeBorrowingDialog::OnPipeSpin)
	EVT_SPINCTRL(ID_BORROWING_DIALOG_FOLLOWING_PIPES_SPIN, PipeBorrowingDialog::OnFollowingPipesSpin)
END_EVENT_TABLE()

PipeBorrowingDialog::PipeBorrowingDialog() {
	Init();
}

PipeBorrowingDialog::PipeBorrowingDialog(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style) {
	Init();
	Create(parent, id, caption, pos, size, style);
}

PipeBorrowingDialog::~PipeBorrowingDialog() {

}

void PipeBorrowingDialog::Init() {
	m_selectedManualIndex = 0;
	m_selectedStopIndex = 0;
	m_selectedPipeNbr = -1;
	m_followingPipes = 0;

	unsigned nbrManuals = ::wxGetApp().m_frame->m_organ->getNumberOfManuals();
	if (nbrManuals > 0) {
		for (unsigned i = 0; i < nbrManuals; i++) {
			m_manualList.Add(::wxGetApp().m_frame->m_organ->getOrganManualAt(i)->getName());
		}
	}

	if (!m_manualList.IsEmpty()) {
		m_manIsOk = true;
	} else {
		m_manualList.Add(wxT("No manuals exist in organ!"));
		m_infoText->SetLabelText(wxT("No manuals exist!"));
		m_manIsOk = false;
	}
}

bool PipeBorrowingDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style ) {
	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	CreateControls();

	if (!m_manualList.IsEmpty()) {
		m_selectedManualIndex = 0;
		m_manualChoice->SetSelection(0);
		UpdateStopChoice();
	}

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void PipeBorrowingDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *manualText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Manual: ")
	);
	firstRow->Add(manualText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_manualChoice = new wxChoice(
		this,
		ID_BORROWING_DIALOG_MANUAL_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_manualList
	);
	firstRow->Add(m_manualChoice, 1, wxGROW|wxALL, 5);
	mainSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *stopText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Stop: ")
	);
	secondRow->Add(stopText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_stopChoice = new wxChoice(
		this,
		ID_BORROWING_DIALOG_STOP_CHOICE
	);
	secondRow->Add(m_stopChoice, 1, wxGROW|wxALL, 5);
	mainSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *pipeNumberText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Pipe number: ")
	);
	thirdRow->Add(pipeNumberText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pipeSpin = new wxSpinCtrl(
		this,
		ID_BORROWING_DIALOG_PIPE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		-1,
		-1
	);
	thirdRow->Add(m_pipeSpin, 1, wxGROW|wxALL, 5);
	mainSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *morePipesRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *morePipesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Also borrow next ")
	);
	morePipesRow->Add(morePipesText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_followingPipesToRefSpin = new wxSpinCtrl(
		this,
		ID_BORROWING_DIALOG_FOLLOWING_PIPES_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		0,
		0
	);
	morePipesRow->Add(m_followingPipesToRefSpin, 1, wxGROW|wxALL, 5);
	wxStaticText *lastText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT(" pipes.")
	);
	morePipesRow->Add(lastText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mainSizer->Add(morePipesRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	fourthRow->AddStretchSpacer();
	m_infoText = new wxStaticText (
		this,
		wxID_STATIC,
		wxEmptyString
	);
	fourthRow->Add(m_infoText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	fourthRow->AddStretchSpacer();
	mainSizer->Add(fourthRow, 0, wxGROW);

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
		wxT("Create reference")
	);
	bottomRow->Add(theOkButton, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	mainSizer->Add(bottomRow, 0, wxGROW);

	SetSizer(mainSizer);
}

int PipeBorrowingDialog::GetSelectedManual() {
	return m_selectedManualIndex;
}

int PipeBorrowingDialog::GetSelectedStop() {
	return m_selectedStopIndex;
}

int PipeBorrowingDialog::GetSelectedPipe() {
	return m_selectedPipeNbr;
}

int PipeBorrowingDialog::GetFollowingPipes() {
	return m_followingPipes;
}

bool PipeBorrowingDialog::IsSelectionOk() {
	return m_manIsOk && m_stopIsOk;
}

void PipeBorrowingDialog::SetDefaultSelections(int manIdx, int stopIdx, int pipeNbr) {
	if (manIdx < (int) m_manualChoice->GetCount()) {
		m_selectedManualIndex = manIdx;
		m_manualChoice->SetSelection(m_selectedManualIndex);
		UpdateStopChoice();

		if (stopIdx < (int) m_stopChoice->GetCount()) {
			m_selectedStopIndex = stopIdx;
			m_stopChoice->SetSelection(m_selectedStopIndex);
			UpdatePipeSpin();

			if (pipeNbr > 0 && pipeNbr <= m_pipeSpin->GetMax()) {
				m_selectedPipeNbr = pipeNbr;
				m_pipeSpin->SetValue(m_selectedPipeNbr);
			}
		}
	}
}

void PipeBorrowingDialog::OnManualChoice(wxCommandEvent& WXUNUSED(event)) {
	m_selectedManualIndex = m_manualChoice->GetSelection();
	UpdateStopChoice();
}

void PipeBorrowingDialog::OnStopChoice(wxCommandEvent& WXUNUSED(event)) {
	m_selectedStopIndex = m_stopChoice->GetSelection();
	UpdatePipeSpin();
}

void PipeBorrowingDialog::OnPipeSpin(wxSpinEvent& WXUNUSED(event)) {
	m_selectedPipeNbr = m_pipeSpin->GetValue();
}

void PipeBorrowingDialog::OnFollowingPipesSpin(wxSpinEvent& WXUNUSED(event)) {
	m_followingPipes = m_followingPipesToRefSpin->GetValue();
}

void PipeBorrowingDialog::UpdateStopChoice() {
	if (!m_stopChoice->IsEmpty())
		m_stopChoice->Clear();

	wxArrayString stops;
	unsigned nbrStops = ::wxGetApp().m_frame->m_organ->getOrganManualAt(m_selectedManualIndex)->getNumberOfStops();
	if (nbrStops > 0 && m_manIsOk) {
		for (unsigned i = 0; i < nbrStops; i++) {
			stops.Add(::wxGetApp().m_frame->m_organ->getOrganManualAt(m_selectedManualIndex)->getStopAt(i)->getName());
		}
		m_infoText->SetLabelText(wxEmptyString);
		m_stopIsOk = true;
	} else {
		stops.Add(wxT("No stops to choose from!"));
		m_infoText->SetLabelText(m_manualChoice->GetString(m_manualChoice->GetSelection()) + wxT(" has no stops!"));
		m_stopIsOk = false;
	}

	m_stopChoice->Insert(stops, 0);
	m_selectedStopIndex = 0;
	m_stopChoice->SetSelection(m_selectedStopIndex);
	UpdatePipeSpin();
}

void PipeBorrowingDialog::UpdatePipeSpin() {
	if (m_manIsOk && m_stopIsOk) {
		m_pipeSpin->SetRange(1, ::wxGetApp().m_frame->m_organ->getOrganManualAt(m_selectedManualIndex)->getStopAt(m_selectedStopIndex)->getNumberOfAccessiblePipes());
		m_selectedPipeNbr = 1;
		m_pipeSpin->SetValue(1);
	} else {
		m_pipeSpin->SetRange(-1, -1);
		m_selectedPipeNbr = -1;
		m_pipeSpin->SetValue(m_selectedPipeNbr);
	}
	UpdateFollowingPipesSpin();
}

void PipeBorrowingDialog::UpdateFollowingPipesSpin() {
	if (m_manIsOk && m_stopIsOk) {
		int maxPipes = ::wxGetApp().m_frame->m_organ->getOrganManualAt(m_selectedManualIndex)->getStopAt(m_selectedStopIndex)->getNumberOfAccessiblePipes();
		int maxPossibleRefPipes = maxPipes - m_selectedPipeNbr;
		if (m_followingPipes > maxPossibleRefPipes)
			m_followingPipes = maxPossibleRefPipes;
		m_followingPipesToRefSpin->SetRange(0, maxPossibleRefPipes);
		m_followingPipesToRefSpin->SetValue(m_followingPipes);
	} else {
		m_followingPipes = 0;
		m_followingPipesToRefSpin->SetRange(0, 0);
		m_followingPipesToRefSpin->SetValue(0);
	}
}
