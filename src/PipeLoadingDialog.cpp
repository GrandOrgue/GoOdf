/*
 * PipeLoadingDialog.cpp is part of GoOdf.
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

#include "PipeLoadingDialog.h"
#include "GOODFFunctions.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>

IMPLEMENT_CLASS(PipeLoadingDialog, wxDialog)

BEGIN_EVENT_TABLE(PipeLoadingDialog, wxDialog)
	EVT_BUTTON(ID_BROWSE_FOR_BASE_PATH, PipeLoadingDialog::OnBrowseForPathBtn)
	EVT_SPINCTRL(ID_START_FROM_PIPE_SPIN, PipeLoadingDialog::OnFirstPipeSpin)
	EVT_SPINCTRL(ID_FIRST_MATCH_NUMBER_SPIN, PipeLoadingDialog::OnFirstMidiNumberSpin)
	EVT_SPINCTRL(ID_NUMBER_OF_PIPES_SPIN, PipeLoadingDialog::OnNumberOfPipesSpin)
	EVT_CHOICE(ID_LOADING_STRATEGY_CHOICE, PipeLoadingDialog::OnLoadingStrategyChoice)
	EVT_TEXT(ID_ATTACK_SUBDIRECTORY_PREFIX_TEXT, PipeLoadingDialog::OnAttackSubDirectoryText)
	EVT_CHECKBOX(ID_ADD_ONLY_ONE_ATTACK_CHECK, PipeLoadingDialog::OnOneAttackCheck)
	EVT_CHECKBOX(ID_LOAD_RELEASE_IN_ATTACKS_CHECK, PipeLoadingDialog::OnReleaseInAttackCheck)
	EVT_TEXT(ID_RELEASE_SUBDIRECTORY_PREFIX_TEXT, PipeLoadingDialog::OnReleaseSubDirectoryText)
	EVT_CHECKBOX(ID_EXTRACT_KEY_PRESS_TIME_CHECK, PipeLoadingDialog::OnExtractKeyPressTimeCheck)
	EVT_TEXT(ID_TREMULANT_SUBDIRECTORY_PREFIX_TEXT, PipeLoadingDialog::OnTremSubDirectoryText)
	EVT_CHECKBOX(ID_LOAD_PIPES_AS_TREMULANT_OFF_CHECK, PipeLoadingDialog::OnLoadPipesTremOffCheck)
END_EVENT_TABLE()

PipeLoadingDialog::PipeLoadingDialog(
	int nbrOfPipes,
	unsigned startPipe,
	unsigned firstMidiNbr,
	wxString baseDirectory,
	wxString attackSubfolder,
	bool onlyOneAttack,
	bool loadReleaseInAttack,
	wxString releaseFolderPrefix,
	bool extractKeyPressTime,
	wxString tremulantFolderPrefix,
	bool loadPipesTremOff) {
	Init(
		nbrOfPipes,
		startPipe,
		firstMidiNbr,
		baseDirectory,
		attackSubfolder,
		onlyOneAttack,
		loadReleaseInAttack,
		releaseFolderPrefix,
		extractKeyPressTime ,
		tremulantFolderPrefix,
		loadPipesTremOff
	);
}

PipeLoadingDialog::PipeLoadingDialog(
	wxWindow* parent,
	int nbrOfPipes,
	unsigned startPipe,
	unsigned firstMidiNbr,
	wxString baseDirectory,
	wxString attackSubfolder,
	bool onlyOneAttack,
	bool loadReleaseInAttack,
	wxString releaseFolderPrefix,
	bool extractKeyPressTime ,
	wxString tremulantFolderPrefix,
	bool loadPipesTremOff,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style) {
	Init(
		nbrOfPipes,
		startPipe,
		firstMidiNbr,
		baseDirectory,
		attackSubfolder,
		onlyOneAttack,
		loadReleaseInAttack,
		releaseFolderPrefix,
		extractKeyPressTime ,
		tremulantFolderPrefix,
		loadPipesTremOff
	);
	Create(parent, id, caption, pos, size, style);
}

PipeLoadingDialog::~PipeLoadingDialog() {

}

void PipeLoadingDialog::Init(
	int nbrOfPipes,
	unsigned startPipe,
	unsigned firstMidiNbr,
	wxString baseDirectory,
	wxString attackSubfolder,
	bool onlyOneAttack,
	bool loadReleaseInAttack,
	wxString releaseFolderPrefix,
	bool extractKeyPressTime ,
	wxString tremulantFolderPrefix,
	bool loadPipesTremOff) {
	m_nbrPipesInRank = nbrOfPipes;
	if (startPipe > 0)
		m_startingPipe = startPipe;
	else
		m_startingPipe = 1;
	m_pipesToLoad = m_nbrPipesInRank - (m_startingPipe - 1);
	m_firstMidiNbr = firstMidiNbr;
	if (baseDirectory != wxEmptyString)
		m_baseDirectory = baseDirectory;
	else {
		m_baseDirectory = ::wxGetApp().m_frame->m_organ->getOdfRoot();
		if (m_baseDirectory == wxEmptyString) {
			if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
				m_baseDirectory = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
			else
				m_baseDirectory = wxStandardPaths::Get().GetDocumentsDir();
		}
	}
	m_attackSubfolder = attackSubfolder;
	m_onlyOneAttack = onlyOneAttack;
	m_loadReleaseInAttack = loadReleaseInAttack;
	if (releaseFolderPrefix != wxEmptyString)
		m_releaseFolderPrefix = releaseFolderPrefix;
	else
		m_releaseFolderPrefix = wxT("rel");
	m_extractKeyPressTime = extractKeyPressTime;
	if (tremulantFolderPrefix != wxEmptyString)
		m_tremulantFolderPrefix = tremulantFolderPrefix;
	else
		m_tremulantFolderPrefix = wxT("trem");
	m_pipeLoadingStrategies.Add(wxT("Create new pipe(s) by number matching"));
	m_pipeLoadingStrategies.Add(wxT("Add more samples to pipe(s)"));
	m_pipeLoadingStrategies.Add(wxT("Add tremulant samples to pipe(s)"));
	m_pipeLoadingStrategies.Add(wxT("Add release samples to pipe(s)"));
	m_loadingStrategy = 0;
	m_loadPipesTremOff = loadPipesTremOff;
}

bool PipeLoadingDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style ) {
	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	CreateControls();
	DecideStateOfOkButton();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void PipeLoadingDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *infoText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Select base directory: ")
	);
	firstRow->Add(infoText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_basePathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	firstRow->Add(m_basePathField, 1, wxEXPAND|wxALL, 5);
	if (m_baseDirectory != wxEmptyString)
		m_basePathField->ChangeValue(m_baseDirectory);
	m_browseForPathBtn = new wxButton(
		this,
		ID_BROWSE_FOR_BASE_PATH,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	firstRow->Add(m_browseForPathBtn, 0, wxALL, 5);
	mainSizer->Add(firstRow, 1, wxGROW|wxALL, 5);

	wxStaticLine *topDivider = new wxStaticLine(this);
	mainSizer->Add(topDivider, 0, wxEXPAND);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *firstPipeText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Start loading from pipe nbr: ")
	);
	secondRow->Add(firstPipeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_startPipeSpin = new wxSpinCtrl(
		this,
		ID_START_FROM_PIPE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		m_nbrPipesInRank,
		1
	);
	secondRow->Add(m_startPipeSpin, 0, wxEXPAND|wxALL, 5);
	m_startPipeSpin->SetValue(m_startingPipe);
	secondRow->AddStretchSpacer();
	wxStaticText *matchMidiText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Matching (MIDI) number: ")
	);
	secondRow->Add(matchMidiText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstMidiNbrSpin = new wxSpinCtrl(
		this,
		ID_FIRST_MATCH_NUMBER_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		127,
		36
	);
	secondRow->Add(m_firstMidiNbrSpin, 0, wxEXPAND|wxALL, 5);
	m_firstMidiNbrSpin->SetValue(m_firstMidiNbr);
	mainSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *totalPipesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Total number of pipes to load: ")
	);
	thirdRow->Add(totalPipesText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pipesToLoadSpin = new wxSpinCtrl(
		this,
		ID_NUMBER_OF_PIPES_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		m_nbrPipesInRank - (m_startingPipe - 1),
		m_pipesToLoad
	);
	thirdRow->Add(m_pipesToLoadSpin, 0, wxEXPAND|wxALL, 5);
	thirdRow->AddStretchSpacer();
	wxStaticText *loadStrategyText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Loading strategy: ")
	);
	thirdRow->Add(loadStrategyText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_loadingStrategyChoice = new wxChoice(
		this,
		ID_LOADING_STRATEGY_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_pipeLoadingStrategies
	);
	thirdRow->Add(m_loadingStrategyChoice, 0, wxALL, 5);
	m_loadingStrategyChoice->SetSelection(0);
	mainSizer->Add(thirdRow, 0, wxGROW);

	wxStaticLine *optionsDivider = new wxStaticLine(this);
	mainSizer->Add(optionsDivider, 0, wxEXPAND);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *extraAttackFolderText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Attack sub-directory prefix: ")
	);
	fourthRow->Add(extraAttackFolderText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_subfolderField = new wxTextCtrl(
		this,
		ID_ATTACK_SUBDIRECTORY_PREFIX_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	fourthRow->Add(m_subfolderField, 1, wxEXPAND|wxALL, 5);
	if (m_attackSubfolder != wxEmptyString)
		m_subfolderField->ChangeValue(m_attackSubfolder);
	mainSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	m_addOnlyOneAttack = new wxCheckBox(
		this,
		ID_ADD_ONLY_ONE_ATTACK_CHECK,
		wxT("Load only one attack"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_addOnlyOneAttack->SetValue(m_onlyOneAttack);
	fifthRow->Add(m_addOnlyOneAttack, 0, wxALL, 5);
	fifthRow->AddStretchSpacer();
	m_addReleaseInAttack = new wxCheckBox(
		this,
		ID_LOAD_RELEASE_IN_ATTACKS_CHECK,
		wxT("Load release in attack(s)"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_addReleaseInAttack->SetValue(m_loadReleaseInAttack);
	fifthRow->Add(m_addReleaseInAttack, 0, wxALL, 5);
	mainSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *releaseFolderText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Release sub-directory prefix: ")
	);
	sixthRow->Add(releaseFolderText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_releasePrefixField = new wxTextCtrl(
		this,
		ID_RELEASE_SUBDIRECTORY_PREFIX_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_releasePrefixField, 1, wxEXPAND|wxALL, 5);
	m_releasePrefixField->ChangeValue(m_releaseFolderPrefix);
	mainSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	m_tryForKeyPressTime = new wxCheckBox(
		this,
		ID_EXTRACT_KEY_PRESS_TIME_CHECK,
		wxT("Extract MaxKeyPressTime from foldername"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_tryForKeyPressTime->SetValue(m_extractKeyPressTime);
	seventhRow->Add(m_tryForKeyPressTime, 0, wxALL, 5);
	mainSizer->Add(seventhRow, 0, wxGROW);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *tremulantFolderText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Tremulant sub-directory prefix: ")
	);
	eighthRow->Add(tremulantFolderText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_tremulantPrefixField = new wxTextCtrl(
		this,
		ID_TREMULANT_SUBDIRECTORY_PREFIX_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	eighthRow->Add(m_tremulantPrefixField, 1, wxEXPAND|wxALL, 5);
	m_tremulantPrefixField->ChangeValue(m_tremulantFolderPrefix);
	mainSizer->Add (eighthRow, 0, wxGROW);

	wxBoxSizer *ninthRow = new wxBoxSizer(wxHORIZONTAL);
	m_loadPipesTremulantOffCheck = new wxCheckBox(
		this,
		ID_LOAD_PIPES_AS_TREMULANT_OFF_CHECK,
		wxT("Load pipes to play when wave tremulant is off"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_loadPipesTremulantOffCheck->SetValue(m_loadPipesTremOff);
	ninthRow->Add(m_loadPipesTremulantOffCheck, 0, wxALL, 5);
	mainSizer->Add(ninthRow, 0, wxGROW);

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
		wxT("Load pipes")
	);
	theOkButton->Disable();
	bottomRow->Add(theOkButton, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	mainSizer->Add(bottomRow, 0, wxGROW);

	SetSizer(mainSizer);
}

unsigned PipeLoadingDialog::GetStartingPipeNumber() {
	return m_startingPipe;
}

unsigned PipeLoadingDialog::GetNbrPipesToLoad() {
	return m_pipesToLoad;
}

unsigned PipeLoadingDialog::GetFirstMidiMatchingNbr() {
	return m_firstMidiNbr;
}

wxString PipeLoadingDialog::GetBaseDirectory() {
	return m_baseDirectory;
}

wxString PipeLoadingDialog::GetSubDirectory() {
	return m_attackSubfolder;
}

bool PipeLoadingDialog::GetLoadOnlyOneAttack() {
	return m_onlyOneAttack;
}

bool PipeLoadingDialog::GetLoadReleaseInAttack() {
	return m_loadReleaseInAttack;
}

wxString PipeLoadingDialog::GetReleaseFolderPrefix() {
	return m_releaseFolderPrefix;
}

wxString PipeLoadingDialog::GetTremFolderPrefix() {
	return m_tremulantFolderPrefix;
}

bool PipeLoadingDialog::GetExtractKeyPressTime() {
	return m_extractKeyPressTime;
}

int PipeLoadingDialog::GetSelectedStrategy() {
	return m_loadingStrategy;
}

bool PipeLoadingDialog::GetLoadPipesTremOff() {
	return m_loadPipesTremOff;
}

void PipeLoadingDialog::OnBrowseForPathBtn(wxCommandEvent& WXUNUSED(event)) {
	wxDirDialog basePathDialog(
		this,
		wxT("Pick a directory to add pipe attack/releases from"),
		m_baseDirectory,
		wxDD_DIR_MUST_EXIST
	);

	if (basePathDialog.ShowModal() == wxID_OK) {
		m_baseDirectory = basePathDialog.GetPath();
		m_basePathField->ChangeValue(m_baseDirectory);
	} else {
		m_baseDirectory = wxEmptyString;
		m_basePathField->ChangeValue(m_baseDirectory);
	}

	DecideStateOfOkButton();
}

void PipeLoadingDialog::OnFirstPipeSpin(wxSpinEvent& WXUNUSED(event)) {
	m_startingPipe = m_startPipeSpin->GetValue();
	int newTotalPipeRange = m_nbrPipesInRank - (m_startingPipe - 1);
	if ((int) m_pipesToLoad > newTotalPipeRange) {
		m_pipesToLoad = newTotalPipeRange;
		m_pipesToLoadSpin->SetValue(m_pipesToLoad);
	}
	m_pipesToLoadSpin->SetRange(1, newTotalPipeRange);
}

void PipeLoadingDialog::OnNumberOfPipesSpin(wxSpinEvent& WXUNUSED(event)) {
	m_pipesToLoad = m_pipesToLoadSpin->GetValue();
}

void PipeLoadingDialog::OnFirstMidiNumberSpin(wxSpinEvent& WXUNUSED(event)) {
	m_firstMidiNbr = m_firstMidiNbrSpin->GetValue();
}

void PipeLoadingDialog::OnAttackSubDirectoryText(wxCommandEvent& WXUNUSED(event)) {
	m_attackSubfolder = m_subfolderField->GetValue();
}

void PipeLoadingDialog::OnOneAttackCheck(wxCommandEvent& WXUNUSED(event)) {
	m_onlyOneAttack = m_addOnlyOneAttack->IsChecked();
}

void PipeLoadingDialog::OnReleaseInAttackCheck(wxCommandEvent& WXUNUSED(event)) {
	m_loadReleaseInAttack = m_addReleaseInAttack->IsChecked();
}

void PipeLoadingDialog::OnReleaseSubDirectoryText(wxCommandEvent& WXUNUSED(event)) {
	m_releaseFolderPrefix = m_releasePrefixField->GetValue();
}

void PipeLoadingDialog::OnExtractKeyPressTimeCheck(wxCommandEvent& WXUNUSED(event)) {
	m_extractKeyPressTime = m_tryForKeyPressTime->IsChecked();
}

void PipeLoadingDialog::OnTremSubDirectoryText(wxCommandEvent& WXUNUSED(event)) {
	m_tremulantFolderPrefix = m_tremulantPrefixField->GetValue();
}

void PipeLoadingDialog::OnLoadingStrategyChoice(wxCommandEvent& WXUNUSED(event)) {
	m_loadingStrategy = m_loadingStrategyChoice->GetSelection();
}

void PipeLoadingDialog::OnLoadPipesTremOffCheck(wxCommandEvent& WXUNUSED(event)) {
	m_loadPipesTremOff = m_loadPipesTremulantOffCheck->IsChecked();
}

void PipeLoadingDialog::DecideStateOfOkButton() {
	if (m_baseDirectory != wxEmptyString) {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Enable();
	} else {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Disable();
	}

}
