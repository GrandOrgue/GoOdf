/*
 * PipeDialog.cpp is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * GoOdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GoOdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GoOdf. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#include "PipeDialog.h"
#include "GOODFFunctions.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/accel.h>

IMPLEMENT_CLASS(PipeDialog, wxDialog)

BEGIN_EVENT_TABLE(PipeDialog, wxDialog)
	EVT_BUTTON(ID_PIPE_DIALOG_PREV_BTN, PipeDialog::OnPrevPipeBtn)
	EVT_BUTTON(ID_PIPE_DIALOG_NEXT_BTN, PipeDialog::OnNextPipeBtn)
	EVT_RADIOBUTTON(ID_PIPE_DIALOG_PERCUSSIVE_YES, PipeDialog::OnPercussiveSelection)
	EVT_RADIOBUTTON(ID_PIPE_DIALOG_PERCUSSIVE_NO, PipeDialog::OnPercussiveSelection)
	EVT_RADIOBUTTON(ID_PIPE_INDEPENDENT_RELEASE_YES, PipeDialog::OnIndependentReleaseSelection)
	EVT_RADIOBUTTON(ID_PIPE_INDEPENDENT_RELEASE_NO, PipeDialog::OnIndependentReleaseSelection)
	EVT_SPINCTRL(ID_PIPE_DIALOG_HARMONIC_SPIN, PipeDialog::OnHarmonicNbrSpin)
	EVT_SPINCTRL(ID_PIPE_DIALOG_MIDI_NOTE_SPIN, PipeDialog::OnMidiNoteSpin)
	EVT_SPINCTRLDOUBLE(ID_PIPE_DIALOG_PITCH_FRACTION_SPIN, PipeDialog::OnMidiPitchFractionSpin)
	EVT_SPINCTRLDOUBLE(ID_PIPE_DIALOG_PITCH_CORR_SPIN, PipeDialog::OnPitchCorrectionSpin)
	EVT_RADIOBUTTON(ID_PIPE_DIALOG_ACCEPTS_RETUNING_YES, PipeDialog::OnAcceptsRetuningSelection)
	EVT_RADIOBUTTON(ID_PIPE_DIALOG_ACCEPTS_RETUNING_NO, PipeDialog::OnAcceptsRetuningSelection)
	EVT_CHOICE(ID_PIPE_DIALOG_WINDCHEST_CHOICE, PipeDialog::OnWindchestChoice)
	EVT_SPINCTRLDOUBLE(ID_PIPE_DIALOG_MIN_VEL_SPIN, PipeDialog::OnMinVelocitySpin)
	EVT_SPINCTRLDOUBLE(ID_PIPE_DIALOG_MAX_VEL_SPIN, PipeDialog::OnMaxVelocitySpin)
	EVT_SPINCTRL(ID_PIPE_DIALOG_COPY_TO_SPIN, PipeDialog::OnCopyPipesSpin)
	EVT_BUTTON(ID_PIPE_DIALOG_COPY_BTN, PipeDialog::OnCopyPropertiesBtn)
	EVT_BUTTON(ID_PIPE_DIALOG_COPY_ATK_REL_BTN, PipeDialog::OnCopyAtkRelBtn)
	EVT_SPINCTRLDOUBLE(ID_PIPE_AMP_LVL_SPIN, PipeDialog::OnAmplitudeLevelSpin)
	EVT_SPINCTRLDOUBLE(ID_PIPE_GAIN_SPIN, PipeDialog::OnGainSpin)
	EVT_SPINCTRLDOUBLE(ID_PIPE_PITCH_SPIN, PipeDialog::OnPitchTuningSpin)
	EVT_SPINCTRL(ID_PIPE_TRACKER_DELAY_SPIN, PipeDialog::OnTrackerDelaySpin)
END_EVENT_TABLE()

PipeDialog::PipeDialog(std::list<Pipe>& pipe_list, unsigned selected_pipe) : m_rank_pipelist(pipe_list) {
	Init(selected_pipe);
}

PipeDialog::PipeDialog(
	std::list<Pipe>& pipe_list,
	unsigned selected_pipe,
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) : m_rank_pipelist(pipe_list) {
	Init(selected_pipe);
	Create(parent, id, caption, pos, size, style);
}

PipeDialog::~PipeDialog() {

}

void PipeDialog::Init(unsigned selected_pipe) {
	m_firstSelectedPipe = selected_pipe;
	m_selectedPipeIndex = selected_pipe;
	m_currentPipe = GetPipePointer(selected_pipe);
}

bool PipeDialog::Create(
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

	// Accelerator table for keyboard navigations
	wxAcceleratorEntry entries[2];
	entries[0].Set(wxACCEL_NORMAL, WXK_LEFT, ID_PIPE_DIALOG_PREV_BTN);
	entries[1].Set(wxACCEL_NORMAL, WXK_RIGHT, ID_PIPE_DIALOG_NEXT_BTN);
	wxAcceleratorTable accel(2, entries);
	this->SetAcceleratorTable(accel);

	unsigned nbWc = ::wxGetApp().m_frame->m_organ->getNumberOfWindchestgroups();
	if (nbWc > 0) {
		for (unsigned i = 0; i < nbWc; i++) {
			windchestChoices.Add(::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(i)->getName());
		}
		m_windchestChoice->Insert(windchestChoices, 0);
	}

	TransferPipeValuesToWindow();
	SetButtonState();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void PipeDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	m_prevPipeBtn = new wxButton(
		this,
		ID_PIPE_DIALOG_PREV_BTN,
		wxT("<-- Previous pipe")
	);
	firstRow->Add(m_prevPipeBtn, 0, wxALL, 5);
	firstRow->AddStretchSpacer();
	m_pipeLabel = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Pipename")
	);
	firstRow->Add(m_pipeLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	firstRow->AddStretchSpacer();
	m_nextPipeBtn = new wxButton(
		this,
		ID_PIPE_DIALOG_NEXT_BTN,
		wxT("Next pipe -->")
	);
	firstRow->Add(m_nextPipeBtn, 0, wxALL, 5);
	mainSizer->Add(firstRow, 0, wxGROW);

	wxStaticLine *topDivider = new wxStaticLine(this);
	mainSizer->Add(topDivider, 0, wxEXPAND);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *isPercussiveText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Percussive: ")
	);
	secondRow->Add(isPercussiveText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveYes = new wxRadioButton(
		this,
		ID_PIPE_DIALOG_PERCUSSIVE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_isPercussiveYes->SetValue(false);
	secondRow->Add(m_isPercussiveYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveNo = new wxRadioButton(
		this,
		ID_PIPE_DIALOG_PERCUSSIVE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_isPercussiveNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveNo->SetValue(true);
	secondRow->AddStretchSpacer();
	wxStaticText *independentReleaseText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Independent release: ")
	);
	secondRow->Add(independentReleaseText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasIndependentReleaseYes = new wxRadioButton(
		this,
		ID_PIPE_INDEPENDENT_RELEASE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_hasIndependentReleaseYes->SetValue(false);
	secondRow->Add(m_hasIndependentReleaseYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasIndependentReleaseNo = new wxRadioButton(
		this,
		ID_PIPE_INDEPENDENT_RELEASE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_hasIndependentReleaseNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasIndependentReleaseNo->SetValue(true);
	secondRow->AddStretchSpacer();
	wxStaticText *acceptRetuningText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Accepts retuning: ")
	);
	secondRow->Add(acceptRetuningText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_acceptsRetuningYes = new wxRadioButton(
		this,
		ID_PIPE_DIALOG_ACCEPTS_RETUNING_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_acceptsRetuningYes->SetValue(true);
	secondRow->Add(m_acceptsRetuningYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_acceptsRetuningNo = new wxRadioButton(
		this,
		ID_PIPE_DIALOG_ACCEPTS_RETUNING_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_acceptsRetuningNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_acceptsRetuningNo->SetValue(false);
	mainSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *harmonicNumberText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Harmonic number: ")
	);
	thirdRow->Add(harmonicNumberText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_harmonicNbrSpin = new wxSpinCtrl(
		this,
		ID_PIPE_DIALOG_HARMONIC_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1024,
		8
	);
	thirdRow->Add(m_harmonicNbrSpin, 0, wxEXPAND|wxALL, 5);
	m_calculatedLength = new wxStaticText(this, wxID_STATIC, wxEmptyString);
	thirdRow->Add(m_calculatedLength, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	thirdRow->AddStretchSpacer();
	wxStaticText *windchestText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Windchestgroup: ")
	);
	thirdRow->Add(windchestText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_windchestChoice = new wxChoice(
		this,
		ID_PIPE_DIALOG_WINDCHEST_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		windchestChoices
	);
	thirdRow->Add(m_windchestChoice, 0, wxALL, 5);
	mainSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *midiNoteText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MIDI key number: ")
	);
	fourthRow->Add(midiNoteText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_midiKeyNbrSpin = new wxSpinCtrl(
		this,
		ID_PIPE_DIALOG_MIDI_NOTE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		127,
		-1
	);
	fourthRow->Add(m_midiKeyNbrSpin, 0, wxEXPAND|wxALL, 5);
	fourthRow->AddStretchSpacer();
	wxStaticText *pitchFractionText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MIDI Pitch fraction: ")
	);
	fourthRow->Add(pitchFractionText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_midiPitchFractionSpin = new wxSpinCtrlDouble(
		this,
		ID_PIPE_DIALOG_PITCH_FRACTION_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		100,
		-1,
		0.1
	);
	m_midiPitchFractionSpin->SetDigits(1);
	m_midiPitchFractionSpin->SetIncrement(1.0f);
	fourthRow->Add(m_midiPitchFractionSpin, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *ampLvlText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("AmplitudeLevel: ")
	);
	fifthRow->Add(ampLvlText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_amplitudeLevelSpin = new wxSpinCtrlDouble(
		this,
		ID_PIPE_AMP_LVL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1000,
		100,
		0.1
	);
	m_amplitudeLevelSpin->SetDigits(1);
	m_amplitudeLevelSpin->SetIncrement(1.0f);
	fifthRow->Add(m_amplitudeLevelSpin, 0, wxEXPAND|wxALL, 5);
	wxStaticText *gainText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Gain: ")
	);
	fifthRow->Add(gainText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gainSpin = new wxSpinCtrlDouble(
		this,
		ID_PIPE_GAIN_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-120,
		40,
		0,
		0.1
	);
	m_gainSpin->SetDigits(1);
	m_gainSpin->SetIncrement(1.0f);
	fifthRow->Add(m_gainSpin, 0, wxEXPAND|wxALL, 5);
	wxStaticText *pitchText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("PitchTuning: ")
	);
	fifthRow->Add(pitchText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pitchTuningSpin = new wxSpinCtrlDouble(
		this,
		ID_PIPE_PITCH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1800,
		1800,
		0,
		0.1
	);
	m_pitchTuningSpin->SetDigits(1);
	m_pitchTuningSpin->SetIncrement(1.0f);
	fifthRow->Add(m_pitchTuningSpin, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *trackerDelayText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TrackerDelay: ")
	);
	sixthRow->Add(trackerDelayText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trackerDelaySpin = new wxSpinCtrl(
		this,
		ID_PIPE_TRACKER_DELAY_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		10000,
		0
	);
	sixthRow->Add(m_trackerDelaySpin, 0, wxGROW|wxALL, 5);
	sixthRow->AddStretchSpacer();
	wxStaticText *pitchCorrectionText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Pitch correction: ")
	);
	sixthRow->Add(pitchCorrectionText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pitchCorrectionSpin = new wxSpinCtrlDouble(
		this,
		ID_PIPE_DIALOG_PITCH_CORR_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1800,
		1800,
		0,
		0.1
	);
	m_pitchCorrectionSpin->SetDigits(1);
	m_pitchCorrectionSpin->SetIncrement(1.0f);
	sixthRow->Add(m_pitchCorrectionSpin, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *minVelocityVolText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Min velocity volume: ")
	);
	seventhRow->Add(minVelocityVolText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_minVelocitySpin = new wxSpinCtrlDouble(
		this,
		ID_PIPE_DIALOG_MIN_VEL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1000,
		100,
		0.1
	);
	m_minVelocitySpin->SetDigits(1);
	m_minVelocitySpin->SetIncrement(1.0f);
	seventhRow->Add(m_minVelocitySpin, 0, wxEXPAND|wxALL, 5);
	seventhRow->AddStretchSpacer();
	wxStaticText *maxVelocityVolText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Max velocity volume: ")
	);
	seventhRow->Add(maxVelocityVolText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maxVelocitySpin = new wxSpinCtrlDouble(
		this,
		ID_PIPE_DIALOG_MAX_VEL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1000,
		100,
		0.1
	);
	m_maxVelocitySpin->SetDigits(1);
	m_maxVelocitySpin->SetIncrement(1.0f);
	seventhRow->Add(m_maxVelocitySpin, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(seventhRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	mainSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *tenthRow = new wxBoxSizer(wxHORIZONTAL);
	m_copyAtkRelBtn = new wxButton(
		this,
		ID_PIPE_DIALOG_COPY_ATK_REL_BTN,
		wxT("Copy attacks/releases to next")
	);
	m_copyAtkRelBtn->Disable();
	tenthRow->Add(m_copyAtkRelBtn, 0, wxALL, 5);
	m_copyPropertiesBtn = new wxButton(
		this,
		ID_PIPE_DIALOG_COPY_BTN,
		wxT("Copy current properties to next")
	);
	m_copyPropertiesBtn->Disable();
	tenthRow->Add(m_copyPropertiesBtn, 0, wxALL, 5);
	m_copyToNbrPipesSpin = new wxSpinCtrl(
		this,
		ID_PIPE_DIALOG_COPY_TO_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		m_rank_pipelist.size(),
		0
	);
	tenthRow->Add(m_copyToNbrPipesSpin, 0, wxEXPAND|wxALL, 5);
	wxStaticText *copyPipeText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT(" pipe(s).")
	);
	tenthRow->Add(copyPipeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_copyInfo = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("")
	);
	tenthRow->Add(m_copyInfo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mainSizer->Add(tenthRow, 0, wxGROW);

	SetSizer(mainSizer);
}

unsigned PipeDialog::GetSelectedPipeIndex() {
	return m_selectedPipeIndex;
}

void PipeDialog::OnPrevPipeBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_selectedPipeIndex > 0) {
		m_selectedPipeIndex--;

		m_currentPipe = GetPipePointer(m_selectedPipeIndex);
		TransferPipeValuesToWindow();

		SetButtonState();
		SetCopyState();
	}
}

void PipeDialog::OnNextPipeBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_selectedPipeIndex < m_rank_pipelist.size() - 1) {
		m_selectedPipeIndex++;

		m_currentPipe = GetPipePointer(m_selectedPipeIndex);
		TransferPipeValuesToWindow();

		SetButtonState();
		SetCopyState();
	}
}

void PipeDialog::OnPercussiveSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_PIPE_DIALOG_PERCUSSIVE_YES) {
		m_currentPipe->isPercussive = true;
		m_hasIndependentReleaseYes->Enable();
		m_hasIndependentReleaseNo->Enable();
	} else {
		m_currentPipe->isPercussive = false;
		// HasIndependentRelease requires that Percussive=Y
		m_currentPipe->setIndependentRelease(false);
		m_hasIndependentReleaseNo->SetValue(true);
		m_hasIndependentReleaseYes->Enable(false);
		m_hasIndependentReleaseNo->Enable(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnIndependentReleaseSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_PIPE_INDEPENDENT_RELEASE_YES) {
		m_currentPipe->setIndependentRelease(true);
	} else {
		m_currentPipe->setIndependentRelease(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnHarmonicNbrSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentPipe->harmonicNumber = m_harmonicNbrSpin->GetValue();
	m_calculatedLength->SetLabelText(GOODF_functions::getFootLengthSize(m_currentPipe->harmonicNumber));
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnMidiNoteSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentPipe->midiKeyNumber = m_midiKeyNbrSpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnMidiPitchFractionSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	float value = m_midiPitchFractionSpin->GetValue();
	if (value < 0 && m_currentPipe->midiPitchFraction > value) {
		value = -1.0f;
	} else if (value > m_currentPipe->midiPitchFraction && value < 0 && m_currentPipe->midiPitchFraction < 0){
		value = 0.0f;
	}
	m_midiPitchFractionSpin->SetValue(value);
	m_currentPipe->midiPitchFraction = value;
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnPitchCorrectionSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentPipe->pitchCorrection = m_pitchCorrectionSpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnAcceptsRetuningSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_PIPE_DIALOG_ACCEPTS_RETUNING_YES) {
		m_currentPipe->acceptsRetuning = true;
	} else {
		m_currentPipe->acceptsRetuning = false;
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnWindchestChoice(wxCommandEvent& WXUNUSED(event)) {
	if (m_windchestChoice->GetSelection() != wxNOT_FOUND) {
		unsigned selectedIndex = m_windchestChoice->GetSelection();
		m_currentPipe->windchest = ::wxGetApp().m_frame->m_organ->getOrganWindchestgroupAt(selectedIndex);
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void PipeDialog::OnMinVelocitySpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentPipe->minVelocityVolume = m_minVelocitySpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnMaxVelocitySpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentPipe->maxVelocityVolume = m_maxVelocitySpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

Pipe* PipeDialog::GetPipePointer(unsigned index) {
	auto iterator = std::next(m_rank_pipelist.begin(), index);
	return &(*iterator);
}

void PipeDialog::SetButtonState() {
	if (m_selectedPipeIndex == 0) {
		if (m_prevPipeBtn->HasFocus())
			m_nextPipeBtn->SetFocus();
		m_prevPipeBtn->Disable();
	} else if (m_selectedPipeIndex > 0) {
		m_prevPipeBtn->Enable();
	}

	if (m_selectedPipeIndex == (m_rank_pipelist.size() - 1)) {
		if (m_nextPipeBtn->HasFocus())
			m_prevPipeBtn->SetFocus();
		m_nextPipeBtn->Disable();
	} else if (m_selectedPipeIndex < (m_rank_pipelist.size() - 1)) {
		m_nextPipeBtn->Enable();
	}
}

void PipeDialog::TransferPipeValuesToWindow() {
	m_pipeLabel->SetLabel(wxString::Format(wxT("Pipe%s"), GOODF_functions::number_format(m_selectedPipeIndex + 1)));
	if (m_currentPipe->hasIndependentRelease) {
		m_hasIndependentReleaseYes->SetValue(true);
	} else {
		m_hasIndependentReleaseNo->SetValue(true);
	}
	if (m_currentPipe->isPercussive) {
		m_isPercussiveYes->SetValue(true);
		m_hasIndependentReleaseYes->Enable();
		m_hasIndependentReleaseNo->Enable();
	} else {
		m_isPercussiveNo->SetValue(true);
		m_hasIndependentReleaseYes->Enable(false);
		m_hasIndependentReleaseNo->Enable(false);
	}
	m_harmonicNbrSpin->SetValue(m_currentPipe->harmonicNumber);
	m_calculatedLength->SetLabelText(GOODF_functions::getFootLengthSize(m_currentPipe->harmonicNumber));
	m_midiKeyNbrSpin->SetValue(m_currentPipe->midiKeyNumber);
	float pitchFractionValue = m_currentPipe->midiPitchFraction;
	if (pitchFractionValue < 0) {
		pitchFractionValue = -1.0f;
		m_currentPipe->midiPitchFraction = pitchFractionValue;
	}
	m_midiPitchFractionSpin->SetValue(pitchFractionValue);
	m_pitchCorrectionSpin->SetValue(m_currentPipe->pitchCorrection);
	if (m_currentPipe->acceptsRetuning) {
		m_acceptsRetuningYes->SetValue(true);
		m_acceptsRetuningNo->SetValue(false);
	} else {
		m_acceptsRetuningYes->SetValue(false);
		m_acceptsRetuningNo->SetValue(true);
	}
	m_windchestChoice->SetSelection(::wxGetApp().m_frame->m_organ->getIndexOfOrganWindchest(m_currentPipe->windchest) - 1);
	m_minVelocitySpin->SetValue(m_currentPipe->minVelocityVolume);
	m_maxVelocitySpin->SetValue(m_currentPipe->maxVelocityVolume);
	m_amplitudeLevelSpin->SetValue(m_currentPipe->amplitudeLevel);
	m_gainSpin->SetValue(m_currentPipe->gain);
	m_pitchTuningSpin->SetValue(m_currentPipe->pitchTuning);
	m_trackerDelaySpin->SetValue(m_currentPipe->trackerDelay);
}

void PipeDialog::OnCopyPipesSpin(wxSpinEvent& WXUNUSED(event)) {
	int spinValue = m_copyToNbrPipesSpin->GetValue();
	if (m_rank_pipelist.size() > 1) {
		if ((spinValue + m_selectedPipeIndex) > (m_rank_pipelist.size() - 2)) {
			spinValue = (m_rank_pipelist.size() - 1) - m_selectedPipeIndex;
			m_copyToNbrPipesSpin->SetValue(spinValue);
		}
	} else {
		spinValue = 0;
		m_copyToNbrPipesSpin->SetValue(spinValue);
	}
	if (spinValue > 0) {
		m_copyPropertiesBtn->Enable();
		m_copyAtkRelBtn->Enable();
	} else {
		m_copyPropertiesBtn->Disable();
		m_copyAtkRelBtn->Disable();
	}
}

void PipeDialog::OnCopyPropertiesBtn(wxCommandEvent& WXUNUSED(event)) {
	int numberOfPipes = m_copyToNbrPipesSpin->GetValue();
	for (int i = 0; i < numberOfPipes; i++) {
		std::list<Pipe>::iterator pipe = m_rank_pipelist.begin();
		std::advance(pipe, (m_selectedPipeIndex + 1 + i));
		pipe->acceptsRetuning = m_currentPipe->acceptsRetuning;
		pipe->harmonicNumber = m_currentPipe->harmonicNumber;
		pipe->isPercussive = m_currentPipe->isPercussive;
		pipe->hasIndependentRelease = m_currentPipe->hasIndependentRelease;
		pipe->maxVelocityVolume = m_currentPipe->maxVelocityVolume;
		pipe->midiKeyNumber = m_currentPipe->midiKeyNumber;
		pipe->midiPitchFraction = m_currentPipe->midiPitchFraction;
		pipe->minVelocityVolume = m_currentPipe->minVelocityVolume;
		pipe->pitchCorrection = m_currentPipe->pitchCorrection;
		pipe->windchest = m_currentPipe->windchest;
		pipe->amplitudeLevel = m_currentPipe->amplitudeLevel;
		pipe->gain = m_currentPipe->gain;
		pipe->pitchTuning = m_currentPipe->pitchTuning;
		pipe->trackerDelay = m_currentPipe->trackerDelay;
	}
	m_copyInfo->SetLabel(wxString::Format(wxT("Done!"), numberOfPipes));
	m_copyToNbrPipesSpin->SetValue(0);
	m_copyPropertiesBtn->Disable();
	m_copyAtkRelBtn->Disable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnCopyAtkRelBtn(wxCommandEvent& WXUNUSED(event)) {
	int numberOfPipes = m_copyToNbrPipesSpin->GetValue();
	for (int i = 0; i < numberOfPipes; i++) {
		std::list<Pipe>::iterator pipe = m_rank_pipelist.begin();
		std::advance(pipe, (m_selectedPipeIndex + 1 + i));
		// first remove any DUMMY already present in target, then copy attacks and releases from current pipe
		if (pipe->m_attacks.front().fullPath == wxT("DUMMY"))
			pipe->m_attacks.pop_front();
		std::copy(m_currentPipe->m_attacks.begin(), m_currentPipe->m_attacks.end(), std::back_inserter(pipe->m_attacks));
		std::copy(m_currentPipe->m_releases.begin(), m_currentPipe->m_releases.end(), std::back_inserter(pipe->m_releases));
	}
	m_copyInfo->SetLabel(wxString::Format(wxT("Done!"), numberOfPipes));
	m_copyToNbrPipesSpin->SetValue(0);
	m_copyPropertiesBtn->Disable();
	m_copyAtkRelBtn->Disable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::SetCopyState() {
	int maxNumber = m_rank_pipelist.size() - (m_selectedPipeIndex + 1);
	m_copyToNbrPipesSpin->SetRange(0, maxNumber);
	m_copyToNbrPipesSpin->SetValue(0);

	m_copyInfo->SetLabel(wxT(""));
}

void PipeDialog::OnAmplitudeLevelSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentPipe->amplitudeLevel = m_amplitudeLevelSpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnGainSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentPipe->gain = m_gainSpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnPitchTuningSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentPipe->pitchTuning = m_pitchTuningSpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void PipeDialog::OnTrackerDelaySpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentPipe->trackerDelay = m_trackerDelaySpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}
