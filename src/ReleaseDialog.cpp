/*
 * ReleaseDialog.cpp is part of GoOdf.
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
 * along with GoOdf. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#include "ReleaseDialog.h"
#include "GOODFFunctions.h"
#include "GOODF.h"
#include <wx/statline.h>
#include "WAVfileParser.h"

IMPLEMENT_CLASS(ReleaseDialog, wxDialog)

BEGIN_EVENT_TABLE(ReleaseDialog, wxDialog)
	EVT_BUTTON(ID_REL_DIALOG_PREV_BTN, ReleaseDialog::OnPrevReleaseBtn)
	EVT_BUTTON(ID_REL_DIALOG_NEXT_BTN, ReleaseDialog::OnNextReleaseBtn)
	EVT_CHOICE(ID_REL_DIALOG_TREMULANT_CHOICE, ReleaseDialog::OnTremulantChoice)
	EVT_SPINCTRL(ID_REL_DIALOG_KEYPRESS_SPIN, ReleaseDialog::OnMaxKeyPressTimeSpin)
	EVT_SPINCTRL(ID_REL_DIALOG_CUE_SPIN, ReleaseDialog::OnCuePointSpin)
	EVT_SPINCTRL(ID_REL_DIALOG_END_SPIN, ReleaseDialog::OnReleaseEndSpin)
	EVT_SPINCTRL(ID_REL_DIALOG_RELEASE_XFADE_SPIN, ReleaseDialog::OnReleaseCrossfadeSpin)
END_EVENT_TABLE()

ReleaseDialog::ReleaseDialog(std::list<Release>& release_list, unsigned selected_release) : m_releaselist(release_list) {
	Init(selected_release);
}

ReleaseDialog::ReleaseDialog(
	std::list<Release>& release_list,
	unsigned selected_release,
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) : m_releaselist(release_list) {
	Init(selected_release);
	Create(parent, id, caption, pos, size, style);
}

ReleaseDialog::~ReleaseDialog() {

}

void ReleaseDialog::Init(unsigned selected_release) {
	m_firstSelectedRelease = selected_release;
	m_selectedReleaseIndex = selected_release;
	m_currentRelease = GetReleasePointer(selected_release);

	tremChoices.Add(wxT("No, not affected by wave tremulants"));
	tremChoices.Add(wxT("Play when wave tremulant is OFF"));
	tremChoices.Add(wxT("Play when wave tremulant is ON"));
}

bool ReleaseDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) {
	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	CreateControls();

	TransferReleaseValuesToWindow();
	SetButtonState();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void ReleaseDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	m_prevReleaseBtn = new wxButton(
		this,
		ID_REL_DIALOG_PREV_BTN,
		wxT("<-- Previous Release")
	);
	firstRow->Add(m_prevReleaseBtn, 0, wxALL, 5);
	firstRow->AddStretchSpacer();
	m_releaseLabel = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Releasename")
	);
	firstRow->Add(m_releaseLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	firstRow->AddStretchSpacer();
	m_nextReleaseBtn = new wxButton(
		this,
		ID_REL_DIALOG_NEXT_BTN,
		wxT("Next release -->")
	);
	firstRow->Add(m_nextReleaseBtn, 0, wxALL, 5);
	mainSizer->Add(firstRow, 0, wxGROW);

	wxStaticLine *topDivider = new wxStaticLine(this);
	mainSizer->Add(topDivider, 0, wxEXPAND);

	wxBoxSizer *nameRow = new wxBoxSizer(wxHORIZONTAL);
	nameRow->AddStretchSpacer();
	m_releaseName = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Displayed releasename")
	);
	nameRow->Add(m_releaseName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	nameRow->AddStretchSpacer();
	mainSizer->Add(nameRow, 0, wxGROW);

	wxBoxSizer *pathRow = new wxBoxSizer(wxHORIZONTAL);
	pathRow->AddStretchSpacer();
	m_releasePath = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Releasepath")
	);
	pathRow->Add(m_releasePath, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pathRow->AddStretchSpacer();
	mainSizer->Add(pathRow, 0, wxGROW);

	wxStaticLine *upperDivider = new wxStaticLine(this);
	mainSizer->Add(upperDivider, 0, wxEXPAND);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *isTremulantText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Is tremulant: ")
	);
	secondRow->Add(isTremulantText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isTremulantChoice = new wxChoice(
		this,
		ID_REL_DIALOG_TREMULANT_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		tremChoices
	);
	secondRow->Add(m_isTremulantChoice, 0, wxALL, 5);
	mainSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *keyPressText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Max key press time: ")
	);
	thirdRow->Add(keyPressText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maxKeyPressTime = new wxSpinCtrl(
		this,
		ID_REL_DIALOG_KEYPRESS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		100000,
		-1
	);
	thirdRow->Add(m_maxKeyPressTime, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *cuePointText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Cuepoint position: ")
	);
	fourthRow->Add(cuePointText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_cuePointSpin = new wxSpinCtrl(
		this,
		ID_REL_DIALOG_CUE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		158760000,
		-1
	);
	fourthRow->Add(m_cuePointSpin, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *relEndText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Release end: ")
	);
	fifthRow->Add(relEndText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_releaseEndSpin = new wxSpinCtrl(
		this,
		ID_REL_DIALOG_END_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		158760000,
		-1
	);
	fifthRow->Add(m_releaseEndSpin, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *xfadeRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *relXfadeText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Release crossfade length: ")
	);
	xfadeRow->Add(relXfadeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_releaseCrossfadeSpin = new wxSpinCtrl(
		this,
		ID_REL_DIALOG_RELEASE_XFADE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		3000,
		0
	);
	xfadeRow->Add(m_releaseCrossfadeSpin, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(xfadeRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	mainSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *lastRow = new wxBoxSizer(wxHORIZONTAL);
	m_copyPropertiesBtn = new wxButton(
		this,
		wxID_OK,
		wxT("Copy properties to other releases coming from same directory")
	);
	m_copyPropertiesBtn->Disable();
	lastRow->Add(m_copyPropertiesBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *warningText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Warning: Clicking the button will close this dialog!")
	);
	lastRow->Add(warningText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxButton *cancelButton = new wxButton(
		this,
		wxID_CANCEL,
		wxT("Exit")
	);
	lastRow->Add(cancelButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mainSizer->Add(lastRow, 0, wxGROW);

	SetSizer(mainSizer);
}

unsigned ReleaseDialog::GetSelectedReleaseIndex() {
	return m_selectedReleaseIndex;
}

Release* ReleaseDialog::GetCurrentRelease() {
	return m_currentRelease;
}

void ReleaseDialog::OnPrevReleaseBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_selectedReleaseIndex > 0) {
		m_selectedReleaseIndex--;

		m_currentRelease = GetReleasePointer(m_selectedReleaseIndex);
		TransferReleaseValuesToWindow();

		SetButtonState();
		m_copyPropertiesBtn->Disable();
	}
}

void ReleaseDialog::OnNextReleaseBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_selectedReleaseIndex < m_releaselist.size()) {
		m_selectedReleaseIndex++;

		m_currentRelease = GetReleasePointer(m_selectedReleaseIndex);
		TransferReleaseValuesToWindow();

		SetButtonState();
		m_copyPropertiesBtn->Disable();
	}
}

void ReleaseDialog::OnTremulantChoice(wxCommandEvent& WXUNUSED(event)) {
	if (m_isTremulantChoice->GetSelection() != wxNOT_FOUND) {
		int tremChoice = m_isTremulantChoice->GetSelection() - 1;
		m_currentRelease->isTremulant = tremChoice;
		m_copyPropertiesBtn->Enable();
	}
}

void ReleaseDialog::OnMaxKeyPressTimeSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentRelease->maxKeyPressTime = m_maxKeyPressTime->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ReleaseDialog::OnCuePointSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentRelease->cuePoint = m_cuePointSpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ReleaseDialog::OnReleaseEndSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentRelease->releaseEnd = m_releaseEndSpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void ReleaseDialog::OnReleaseCrossfadeSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentRelease->releaseCrossfadeLength = m_releaseCrossfadeSpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

Release* ReleaseDialog::GetReleasePointer(unsigned index) {
	auto iterator = std::next(m_releaselist.begin(), index);
	return &(*iterator);
}

void ReleaseDialog::SetButtonState() {
	if (m_selectedReleaseIndex == 0) {
		m_prevReleaseBtn->Disable();
	} else if (m_selectedReleaseIndex > 0) {
		m_prevReleaseBtn->Enable();
	}

	if (m_selectedReleaseIndex == (m_releaselist.size() - 1)) {
		m_nextReleaseBtn->Disable();
	} else if (m_selectedReleaseIndex < (m_releaselist.size() - 1)) {
		m_nextReleaseBtn->Enable();
	}
}

void ReleaseDialog::TransferReleaseValuesToWindow() {
	if (!m_currentRelease->fullPath.IsSameAs(wxT("DUMMY"))) {
		WAVfileParser sample(m_currentRelease->fullPath);
		if (sample.isWavOk()) {
			m_cuePointSpin->SetRange(-1, sample.getNumberOfFrames() - 1);
			m_releaseEndSpin->SetRange(-1, sample.getNumberOfFrames() - 1);
		}
	}
	m_releaseLabel->SetLabel(wxString::Format(wxT("Release%s"), GOODF_functions::number_format(m_selectedReleaseIndex + 1)));
	m_releaseName->SetLabel(m_currentRelease->fileName);
	m_releasePath->SetLabel(m_currentRelease->fullPath);
	m_isTremulantChoice->SetSelection(m_currentRelease->isTremulant + 1);
	m_maxKeyPressTime->SetValue(m_currentRelease->maxKeyPressTime);
	m_cuePointSpin->SetValue(m_currentRelease->cuePoint);
	m_releaseEndSpin->SetValue(m_currentRelease->releaseEnd);
	m_releaseCrossfadeSpin->SetValue(m_currentRelease->releaseCrossfadeLength);
}
