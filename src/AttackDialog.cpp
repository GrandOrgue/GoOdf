/*
 * AttackDialog.cpp is part of GoOdf.
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

#include "AttackDialog.h"
#include "GOODFFunctions.h"
#include "GOODF.h"
#include <wx/statline.h>
#include "WAVfileParser.h"

IMPLEMENT_CLASS(AttackDialog, wxDialog)

BEGIN_EVENT_TABLE(AttackDialog, wxDialog)
	EVT_BUTTON(ID_ATK_DIALOG_PREV_BTN, AttackDialog::OnPrevAttackBtn)
	EVT_BUTTON(ID_ATK_DIALOG_NEXT_BTN, AttackDialog::OnNextAttackBtn)
	EVT_RADIOBUTTON(ID_ATK_DIALOG_LOAD_REL_YES, AttackDialog::OnLoadReleaseSelection)
	EVT_RADIOBUTTON(ID_ATK_DIALOG_LOAD_REL_NO, AttackDialog::OnLoadReleaseSelection)
	EVT_SPINCTRL(ID_ATK_DIALOG_ATK_VEL_SPIN, AttackDialog::OnAttackVelocitySpin)
	EVT_SPINCTRL(ID_ATK_DIALOG_MAX_TIME_SINCE_SPIN, AttackDialog::OnMaxTimeSinceLastSpin)
	EVT_CHOICE(ID_ATK_DIALOG_TREMULANT_CHOICE, AttackDialog::OnTremulantChoice)
	EVT_SPINCTRL(ID_ATK_DIALOG_KEYPRESS_SPIN, AttackDialog::OnMaxKeyPressTimeSpin)
	EVT_SPINCTRL(ID_ATK_DIALOG_ATK_START_SPIN, AttackDialog::OnAttackStartSpin)
	EVT_SPINCTRL(ID_ATK_DIALOG_CUE_SPIN, AttackDialog::OnCuePointSpin)
	EVT_SPINCTRL(ID_ATK_DIALOG_END_SPIN, AttackDialog::OnReleaseEndSpin)
	EVT_SPINCTRL(ID_ATK_DIALOG_LOOP_XFADE_SPIN, AttackDialog::OnLoopCrossfadeSpin)
	EVT_SPINCTRL(ID_ATK_DIALOG_RELEASE_XFADE_SPIN, AttackDialog::OnReleaseCrossfadeSpin)
	EVT_LISTBOX(ID_ATK_DIALOG_LOOP_LIST, AttackDialog::OnLoopListSelection)
	EVT_BUTTON(ID_ATK_DIALOG_ADD_LOOP_BTN, AttackDialog::OnAddLoopBtn)
	EVT_BUTTON(ID_ATK_DIALOG_DELETE_LOOP_BTN, AttackDialog::OnRemoveLoopBtn)
	EVT_SPINCTRL(ID_ATK_DIALOG_LOOP_START_SPIN, AttackDialog::OnLoopStartSpin)
	EVT_SPINCTRL(ID_ATK_DIALOG_LOOP_END_SPIN, AttackDialog::OnLoopEndSpin)
	EVT_CHECKBOX(ID_RANK_COPY_REPLACE_ODF_LOOPS, AttackDialog::OnCopyReplaceLoopCheck)
END_EVENT_TABLE()

AttackDialog::AttackDialog(std::list<Attack>& attack_list, unsigned selected_attack) : m_attacklist(attack_list) {
	Init(selected_attack);
}

AttackDialog::AttackDialog(
	std::list<Attack>& attack_list,
	unsigned selected_attack,
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) : m_attacklist(attack_list) {
	Init(selected_attack);
	Create(parent, id, caption, pos, size, style);
}

AttackDialog::~AttackDialog() {

}

void AttackDialog::Init(unsigned selected_attack) {
	m_firstSelectedAttack = selected_attack;
	m_selectedAttackIndex = selected_attack;
	m_currentAttack = GetAttackIterator(selected_attack);
	m_selectedLoop = NULL;

	tremChoices.Add(wxT("No, not affected by wave tremulants"));
	tremChoices.Add(wxT("Play when wave tremulant is OFF"));
	tremChoices.Add(wxT("Play when wave tremulant is ON"));
}

bool AttackDialog::Create(
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

	TransferAttackValuesToWindow();
	SetButtonState();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void AttackDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	m_prevAttackBtn = new wxButton(
		this,
		ID_ATK_DIALOG_PREV_BTN,
		wxT("<-- Previous attack")
	);
	firstRow->Add(m_prevAttackBtn, 0, wxALL, 5);
	firstRow->AddStretchSpacer();
	m_attackLabel = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Attackname")
	);
	firstRow->Add(m_attackLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	firstRow->AddStretchSpacer();
	m_nextAttackBtn = new wxButton(
		this,
		ID_ATK_DIALOG_NEXT_BTN,
		wxT("Next attack -->")
	);
	firstRow->Add(m_nextAttackBtn, 0, wxALL, 5);
	mainSizer->Add(firstRow, 0, wxGROW);

	wxStaticLine *topDivider = new wxStaticLine(this);
	mainSizer->Add(topDivider, 0, wxEXPAND);

	wxBoxSizer *nameRow = new wxBoxSizer(wxHORIZONTAL);
	nameRow->AddStretchSpacer();
	m_attackName = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Displayed attackname")
	);
	nameRow->Add(m_attackName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	nameRow->AddStretchSpacer();
	mainSizer->Add(nameRow, 0, wxGROW);

	wxBoxSizer *pathRow = new wxBoxSizer(wxHORIZONTAL);
	pathRow->AddStretchSpacer();
	m_attackPath = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Attackpath")
	);
	pathRow->Add(m_attackPath, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	pathRow->AddStretchSpacer();
	mainSizer->Add(pathRow, 0, wxGROW);

	wxStaticLine *upperDivider = new wxStaticLine(this);
	mainSizer->Add(upperDivider, 0, wxEXPAND);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *loadReleaseText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Load release: ")
	);
	secondRow->Add(loadReleaseText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_loadReleaseYes = new wxRadioButton(
		this,
		ID_ATK_DIALOG_LOAD_REL_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_loadReleaseYes->SetValue(true);
	secondRow->Add(m_loadReleaseYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_loadReleaseNo = new wxRadioButton(
		this,
		ID_ATK_DIALOG_LOAD_REL_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_loadReleaseNo->SetValue(false);
	secondRow->Add(m_loadReleaseNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *keyPressText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Max key press time: ")
	);
	secondRow->Add(keyPressText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maxKeyPressTime = new wxSpinCtrl(
		this,
		ID_ATK_DIALOG_KEYPRESS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		100000,
		-1
	);
	secondRow->Add(m_maxKeyPressTime, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *attackVelocityText = new wxStaticText (
			this,
			wxID_STATIC,
			wxT("Attack velocity: ")
		);
	thirdRow->Add(attackVelocityText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_attackVelocitySpin = new wxSpinCtrl(
		this,
		ID_ATK_DIALOG_ATK_VEL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		127,
		0
	);
	thirdRow->Add(m_attackVelocitySpin, 0, wxEXPAND|wxALL, 5);
	thirdRow->AddStretchSpacer();
	wxStaticText *maxTimeSinceText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Max time since last release: ")
	);
	thirdRow->Add(maxTimeSinceText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maxTimeSinceLastSpin = new wxSpinCtrl(
		this,
		ID_ATK_DIALOG_MAX_TIME_SINCE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		100000,
		-1
	);
	thirdRow->Add(m_maxTimeSinceLastSpin, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *isTremulantText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Is tremulant: ")
	);
	fourthRow->Add(isTremulantText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isTremulantChoice = new wxChoice(
		this,
		ID_ATK_DIALOG_TREMULANT_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		tremChoices
	);
	fourthRow->Add(m_isTremulantChoice, 0, wxALL, 5);
	fourthRow->AddStretchSpacer();
	wxStaticText *atkStartText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Attack start: ")
	);
	fourthRow->Add(atkStartText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_attackStartSpin = new wxSpinCtrl(
		this,
		ID_ATK_DIALOG_ATK_START_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		158760000,
		0
	);
	fourthRow->Add(m_attackStartSpin, 0, wxEXPAND|wxALL, 5);
	mainSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *cuePointText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Cuepoint position: ")
	);
	fifthRow->Add(cuePointText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_cuePointSpin = new wxSpinCtrl(
		this,
		ID_ATK_DIALOG_CUE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		158760000,
		-1
	);
	fifthRow->Add(m_cuePointSpin, 0, wxEXPAND|wxALL, 5);
	fifthRow->AddStretchSpacer();
	wxStaticText *relEndText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Release end: ")
	);
	fifthRow->Add(relEndText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_releaseEndSpin = new wxSpinCtrl(
		this,
		ID_ATK_DIALOG_END_SPIN,
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
	wxStaticText *loopXfadeText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Loop crossfade length: ")
	);
	xfadeRow->Add(loopXfadeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_loopCrossfadeSpin = new wxSpinCtrl(
		this,
		ID_ATK_DIALOG_LOOP_XFADE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		3000,
		0
	);
	xfadeRow->Add(m_loopCrossfadeSpin, 0, wxEXPAND|wxALL, 5);
	xfadeRow->AddStretchSpacer();
	wxStaticText *relXfadeText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Release crossfade length: ")
	);
	xfadeRow->Add(relXfadeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_releaseCrossfadeSpin = new wxSpinCtrl(
		this,
		ID_ATK_DIALOG_RELEASE_XFADE_SPIN,
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

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *loopListContainer = new wxBoxSizer(wxVERTICAL);
	wxStaticText *loopListText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Loops in ODF (not embedded)")
	);
	loopListContainer->Add(loopListText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_loopsList = new wxListBox(
		this,
		ID_ATK_DIALOG_LOOP_LIST
	);
	loopListContainer->Add(m_loopsList, 1, wxEXPAND|wxALL, 5);
	sixthRow->Add(loopListContainer, 1, wxGROW|wxALL, 5);
	wxBoxSizer *loopBtnContainer = new wxBoxSizer(wxVERTICAL);
	loopBtnContainer->AddStretchSpacer();
	m_addNewLoopBtn = new wxButton(
		this,
		ID_ATK_DIALOG_ADD_LOOP_BTN,
		wxT("Create a new loop")
	);
	loopBtnContainer->Add(m_addNewLoopBtn, 0, wxGROW|wxALL, 5);
	m_deleteLoopBtn = new wxButton(
		this,
		ID_ATK_DIALOG_DELETE_LOOP_BTN,
		wxT("Delete selected loop")
	);
	loopBtnContainer->Add(m_deleteLoopBtn, 0, wxGROW|wxALL, 5);
	loopBtnContainer->AddStretchSpacer();
	sixthRow->Add(loopBtnContainer, 0, wxGROW);
	wxStaticBoxSizer *loopPropertiesContainer = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Selected loop properties"));
	wxBoxSizer *startRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *startText = new wxStaticText (
		loopPropertiesContainer->GetStaticBox(),
		wxID_STATIC,
		wxT("Loop start: ")
	);
	startRow->Add(startText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_loopStartSpin = new wxSpinCtrl(
		loopPropertiesContainer->GetStaticBox(),
		ID_ATK_DIALOG_LOOP_START_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		158760000,
		0
	);
	startRow->Add(m_loopStartSpin, 0, wxEXPAND|wxALL, 5);
	loopPropertiesContainer->Add(startRow, 0, wxGROW);
	wxBoxSizer *endRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *endText = new wxStaticText (
		loopPropertiesContainer->GetStaticBox(),
		wxID_STATIC,
		wxT("Loop end: ")
	);
	endRow->Add(endText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_loopEndSpin = new wxSpinCtrl(
		loopPropertiesContainer->GetStaticBox(),
		ID_ATK_DIALOG_LOOP_END_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		158760000,
		1
	);
	endRow->Add(m_loopEndSpin, 0, wxEXPAND|wxALL, 5);
	loopPropertiesContainer->Add(endRow, 0, wxGROW);
	wxStaticText *commentText1 = new wxStaticText (
		loopPropertiesContainer->GetStaticBox(),
		wxID_STATIC,
		wxT("A loop must be selected in the listbox to the left to enable.")
	);
	loopPropertiesContainer->Add(commentText1, 0, wxGROW|wxALL, 5);
	m_copyReplaceLoopsCheck = new wxCheckBox(
		loopPropertiesContainer->GetStaticBox(),
		ID_RANK_COPY_REPLACE_ODF_LOOPS,
		wxT("Copy/replace loops to other attacks too!")
	);
	m_copyReplaceLoopsCheck->SetValue(false);
	loopPropertiesContainer->Add(m_copyReplaceLoopsCheck, 0, wxGROW|wxALL, 5);
	sixthRow->Add(loopPropertiesContainer, 1, wxEXPAND|wxALL, 5);
	mainSizer->Add(sixthRow, 1, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	mainSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *lastRow = new wxBoxSizer(wxHORIZONTAL);
	m_copyPropertiesBtn = new wxButton(
		this,
		wxID_OK,
		wxT("Copy properties to other attacks coming from same directory")
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

unsigned AttackDialog::GetSelectedAttackIndex() {
	return m_selectedAttackIndex;
}

bool AttackDialog::GetCopyReplaceLoops() {
	return m_copyReplaceLoopsCheck->IsChecked();
}

std::list<Attack>::iterator AttackDialog::GetAttackIterator(unsigned index) {
	auto iterator = std::next(m_attacklist.begin(), index);
	return iterator;
}

void AttackDialog::OnPrevAttackBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_selectedAttackIndex > 0) {
		m_selectedAttackIndex--;

		m_currentAttack = GetAttackIterator(m_selectedAttackIndex);
		TransferAttackValuesToWindow();

		SetButtonState();
		m_copyPropertiesBtn->Disable();
	}
}

void AttackDialog::OnNextAttackBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_selectedAttackIndex < m_attacklist.size()) {
		m_selectedAttackIndex++;

		m_currentAttack = GetAttackIterator(m_selectedAttackIndex);
		TransferAttackValuesToWindow();

		SetButtonState();
		m_copyPropertiesBtn->Disable();
	}
}

void AttackDialog::OnLoadReleaseSelection(wxCommandEvent& event) {
	auto m_currentAttack = GetAttackIterator(m_selectedAttackIndex);
	if (event.GetId() == ID_ATK_DIALOG_LOAD_REL_YES) {
		m_currentAttack->loadRelease = true;
		m_releaseCrossfadeSpin->Enable();
	} else {
		m_currentAttack->loadRelease = false;
		m_releaseCrossfadeSpin->Disable();
	}
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void AttackDialog::OnAttackVelocitySpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentAttack->attackVelocity = m_attackVelocitySpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void AttackDialog::OnMaxTimeSinceLastSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentAttack->maxTimeSinceLastRelease = m_maxTimeSinceLastSpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void AttackDialog::OnTremulantChoice(wxCommandEvent& WXUNUSED(event)) {
	if (m_isTremulantChoice->GetSelection() != wxNOT_FOUND) {
		int tremChoice = m_isTremulantChoice->GetSelection() - 1;
		m_currentAttack->isTremulant = tremChoice;
		m_copyPropertiesBtn->Enable();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void AttackDialog::OnMaxKeyPressTimeSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentAttack->maxKeyPressTime = m_maxKeyPressTime->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void AttackDialog::OnAttackStartSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentAttack->attackStart = m_attackStartSpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void AttackDialog::OnCuePointSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentAttack->cuePoint = m_cuePointSpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}


void AttackDialog::OnLoopCrossfadeSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentAttack->loopCrossfadeLength = m_loopCrossfadeSpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void AttackDialog::OnReleaseCrossfadeSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentAttack->releaseCrossfadeLength = m_releaseCrossfadeSpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void AttackDialog::OnReleaseEndSpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentAttack->releaseEnd = m_releaseEndSpin->GetValue();
	m_copyPropertiesBtn->Enable();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void AttackDialog::OnLoopListSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_loopsList->GetSelection() != wxNOT_FOUND) {
		m_loopStartSpin->Enable();
		m_loopEndSpin->Enable();
		m_deleteLoopBtn->Enable();
		unsigned selectedIndex = m_loopsList->GetSelection();
		m_selectedLoop = m_currentAttack->getLoopAt(selectedIndex);
		LoopInListSelected();
	} else {
		m_deleteLoopBtn->Disable();
		m_loopStartSpin->Disable();
		m_loopEndSpin->Disable();
	}
}

void AttackDialog::OnAddLoopBtn(wxCommandEvent& WXUNUSED(event)) {
	Loop l;
	l.start = 0;
	l.end = m_maxSampleFrames;
	m_currentAttack->addNewLoop(l);
	UpdateLoopChoices();
	unsigned lastLoopIndex = m_loopsList->GetCount() - 1;
	m_loopsList->SetSelection(lastLoopIndex);
	m_selectedLoop = m_currentAttack->getLoopAt(lastLoopIndex);
	LoopInListSelected();
	if (GetCopyReplaceLoops())
		m_copyPropertiesBtn->Enable();
}

void AttackDialog::OnRemoveLoopBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_loopsList->GetSelection() != wxNOT_FOUND) {
		unsigned indexToRemove = m_loopsList->GetSelection();
		m_currentAttack->removeLoopAt(indexToRemove);
		UpdateLoopChoices();
		if (loopChoices.IsEmpty()) {
			m_deleteLoopBtn->Disable();
			m_loopStartSpin->Disable();
			m_loopEndSpin->Disable();
			m_selectedLoop = NULL;
		} else {
			unsigned lastLoopIndex = m_loopsList->GetCount() - 1;
			m_loopsList->SetSelection(lastLoopIndex);
			m_selectedLoop = m_currentAttack->getLoopAt(lastLoopIndex);
			LoopInListSelected();
		}
		if (GetCopyReplaceLoops())
			m_copyPropertiesBtn->Enable();
	}
}

void AttackDialog::OnLoopStartSpin(wxSpinEvent& WXUNUSED(event)) {
	int value = m_loopStartSpin->GetValue();
	if (value > m_loopEndSpin->GetValue() - 1) {
		value = m_loopEndSpin->GetValue() - 1;
		m_loopStartSpin->SetValue(value);
	}
	m_selectedLoop->start = value;
	SetLoopStartAndEndRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
	if (GetCopyReplaceLoops())
		m_copyPropertiesBtn->Enable();
}

void AttackDialog::OnLoopEndSpin(wxSpinEvent& WXUNUSED(event)) {
	int value = m_loopEndSpin->GetValue();
	if (value < m_loopStartSpin->GetValue() + 1) {
		value = m_loopStartSpin->GetValue() + 1;
		m_loopEndSpin->SetValue(value);
	}
	m_selectedLoop->end = value;
	SetLoopStartAndEndRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
	if (GetCopyReplaceLoops())
		m_copyPropertiesBtn->Enable();
}

void AttackDialog::OnCopyReplaceLoopCheck(wxCommandEvent& WXUNUSED(event)) {
	if (GetCopyReplaceLoops())
		m_copyPropertiesBtn->Enable();
}

void AttackDialog::SetButtonState() {
	if (m_selectedAttackIndex == 0) {
		m_prevAttackBtn->Disable();
	} else if (m_selectedAttackIndex > 0) {
		m_prevAttackBtn->Enable();
	}

	if (m_selectedAttackIndex == (m_attacklist.size() - 1)) {
		m_nextAttackBtn->Disable();
	} else if (m_selectedAttackIndex < (m_attacklist.size() - 1)) {
		m_nextAttackBtn->Enable();
	}
}

void AttackDialog::TransferAttackValuesToWindow() {
	m_attackLabel->SetLabel(wxString::Format(wxT("Attack%s"), GOODF_functions::number_format(m_selectedAttackIndex + 1)));
	m_attackName->SetLabel(m_currentAttack->fileName);
	m_attackPath->SetLabel(m_currentAttack->fullPath);
	if (m_currentAttack->fullPath.IsSameAs(wxT("DUMMY"))) {
		// almost everything should be set to default and disabled
		m_loadReleaseYes->SetValue(true);
		m_loadReleaseNo->SetValue(false);
		m_attackVelocitySpin->SetValue(m_currentAttack->attackVelocity);
		m_maxTimeSinceLastSpin->SetValue(m_currentAttack->maxTimeSinceLastRelease);
		m_isTremulantChoice->SetSelection(m_currentAttack->isTremulant + 1);
		m_maxKeyPressTime->SetValue(m_currentAttack->maxKeyPressTime);
		m_attackStartSpin->SetValue(m_currentAttack->attackStart);
		m_cuePointSpin->SetValue(m_currentAttack->cuePoint);
		m_releaseEndSpin->SetValue(m_currentAttack->releaseEnd);
		m_loopCrossfadeSpin->SetValue(m_currentAttack->loopCrossfadeLength);
		m_releaseCrossfadeSpin->SetValue(m_currentAttack->releaseCrossfadeLength);

		m_loadReleaseYes->Disable();
		m_loadReleaseNo->Disable();
		m_attackVelocitySpin->Disable();
		m_maxTimeSinceLastSpin->Disable();
		m_isTremulantChoice->Disable();
		m_maxKeyPressTime->Disable();
		m_attackStartSpin->Disable();
		m_cuePointSpin->Disable();
		m_releaseEndSpin->Disable();
		m_addNewLoopBtn->Disable();
		m_deleteLoopBtn->Disable();
		m_loopStartSpin->Disable();
		m_loopEndSpin->Disable();
		m_loopCrossfadeSpin->Disable();
		m_releaseCrossfadeSpin->Disable();
	} else {
		WAVfileParser sample(m_currentAttack->fullPath);
		if (sample.isWavOk()) {
			m_maxSampleFrames = sample.getNumberOfFrames() - 1;
			m_attackStartSpin->SetRange(0, m_maxSampleFrames);
			m_cuePointSpin->SetRange(-1, m_maxSampleFrames);
			m_releaseEndSpin->SetRange(-1, m_maxSampleFrames);
			m_loopStartSpin->SetRange(0, m_maxSampleFrames - 1);
			m_loopStartSpin->SetValue(0);
			m_loopEndSpin->SetRange(1, m_maxSampleFrames);
			m_loopEndSpin->SetValue(m_maxSampleFrames);
		} else {
			m_maxSampleFrames = 158760000;
		}

		if (m_currentAttack->loadRelease) {
			m_loadReleaseYes->SetValue(true);
			m_loadReleaseNo->SetValue(false);
		} else {
			m_loadReleaseYes->SetValue(false);
			m_loadReleaseNo->SetValue(true);
		}
		m_attackVelocitySpin->SetValue(m_currentAttack->attackVelocity);
		m_maxTimeSinceLastSpin->SetValue(m_currentAttack->maxTimeSinceLastRelease);
		m_isTremulantChoice->SetSelection(m_currentAttack->isTremulant + 1);
		m_maxKeyPressTime->SetValue(m_currentAttack->maxKeyPressTime);
		m_attackStartSpin->SetValue(m_currentAttack->attackStart);
		m_cuePointSpin->SetValue(m_currentAttack->cuePoint);
		m_releaseEndSpin->SetValue(m_currentAttack->releaseEnd);
		m_loopCrossfadeSpin->SetValue(m_currentAttack->loopCrossfadeLength);
		m_releaseCrossfadeSpin->SetValue(m_currentAttack->releaseCrossfadeLength);
		if (m_currentAttack->loadRelease)
			m_releaseCrossfadeSpin->Enable();
		else
			m_releaseCrossfadeSpin->Disable();

		UpdateLoopChoices();
	}
}

void AttackDialog::SetLoopStartAndEndRanges() {
	m_loopStartSpin->SetRange(0, m_loopEndSpin->GetValue() - 1);
	m_loopEndSpin->SetRange(m_loopStartSpin->GetValue() + 1, m_maxSampleFrames);
}

void AttackDialog::UpdateLoopChoices() {
	m_currentAttack = GetAttackIterator(m_selectedAttackIndex);
	loopChoices.Empty();
	unsigned counter = 0;
	for (Loop l : m_currentAttack->m_loops) {
		counter++;
		loopChoices.Add(wxString::Format(wxT("Loop%s"), GOODF_functions::number_format(counter)));
	}
	m_loopsList->Clear();
	if (!loopChoices.IsEmpty()) {
		m_loopsList->InsertItems(loopChoices, 0);
		if (m_loopsList->GetSelection() == wxNOT_FOUND) {
			m_loopsList->SetSelection(0);
			m_selectedLoop = m_currentAttack->getLoopAt(0);
			LoopInListSelected();
		}
	} else {
		m_deleteLoopBtn->Disable();
		m_loopStartSpin->Disable();
		m_loopEndSpin->Disable();
	}
}

void AttackDialog::LoopInListSelected() {
	if (m_loopsList->GetSelection() != wxNOT_FOUND) {
		m_deleteLoopBtn->Enable();
		m_loopStartSpin->Enable();
		m_loopEndSpin->Enable();
		m_loopStartSpin->SetValue(m_selectedLoop->start);
		m_loopEndSpin->SetValue(m_selectedLoop->end);
		SetLoopStartAndEndRanges();
	}
}
