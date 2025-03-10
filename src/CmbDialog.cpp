/*
 * CmbDialog.cpp is part of GoOdf.
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

#include "CmbDialog.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/stdpaths.h>
#include <wx/statline.h>

IMPLEMENT_CLASS(CmbDialog, wxDialog)

BEGIN_EVENT_TABLE(CmbDialog, wxDialog)
	EVT_BUTTON(ID_BROWSE_FOR_CMB_PATH_BTN, CmbDialog::OnBrowseForCmbBtn)
	EVT_CHECKBOX(ID_IMPORT_AMPLITUDE_CHECK, CmbDialog::OnImportAmplitudeCheck)
	EVT_CHECKBOX(ID_IMPORT_GAIN_CHECK, CmbDialog::OnImportGainCheck)
	EVT_CHECKBOX(ID_IMPORT_PITCH_TUNING_CHECK, CmbDialog::OnImportPitchTuningCheck)
	EVT_CHECKBOX(ID_IMPORT_PITCH_CORRECTION_CHECK, CmbDialog::OnImportPitchCorrectionCheck)
	EVT_CHECKBOX(ID_IMPORT_TRACKER_DELAY_CHECK, CmbDialog::OnImportTrackerDelayCheck)
END_EVENT_TABLE()

CmbDialog::CmbDialog() {
	Init();
}

CmbDialog::CmbDialog(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) {
	Init();
	Create(parent, id, caption, pos, size, style);
}

CmbDialog::~CmbDialog() {
	if (m_cmbParser) {
		delete m_cmbParser;
		m_cmbParser = NULL;
	}
}

void CmbDialog::Init() {
	m_cmbParser = NULL;
	m_importAmplitude = true;
	m_importGain = true;
	m_importPitchTuning = true;
	m_importPitchCorrection = true;
	m_importTrackerDelay = true;
}

bool CmbDialog::Create(
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
	DecideStateOfOkButton();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void CmbDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *warningRow = new wxBoxSizer(wxVERTICAL);
	wxStaticText *warningHeader = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("WARNING!!!")
	);
	warningHeader->SetForegroundColour(*wxRED);
	warningHeader->SetFont(wxFont(wxFontInfo(18).Bold()));
	warningRow->Add(warningHeader, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText *warningText1 = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Only use this feature to import voicing data from a .cmb settings file that actually was created by GrandOrgue (with the 'File->Export Settings' menu option) for exactly this .organ file!")
	);
	warningText1->Wrap(640);
	warningRow->Add(warningText1, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	wxStaticText *warningText2 = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("In particular, if the number of ranks/stops/pipes have changed in the .organ file, there's no guarantee that the voicing data from the .cmb file will be imported correctly!")
	);
	warningText2->Wrap(640);
	warningRow->Add(warningText2, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	mainSizer->Add(warningRow, 0, wxGROW|wxALL, 5);

	wxStaticLine *warningDivider = new wxStaticLine(this);
	mainSizer->Add(warningDivider, 0, wxEXPAND);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *infoText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Select .cmb file: ")
	);
	firstRow->Add(infoText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_cmbPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	firstRow->Add(m_cmbPathField, 1, wxEXPAND|wxALL, 5);
	m_browseForCmbBtn = new wxButton(
		this,
		ID_BROWSE_FOR_CMB_PATH_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	firstRow->Add(m_browseForCmbBtn, 0, wxALL, 5);
	mainSizer->Add(firstRow, 1, wxGROW|wxALL, 5);

	wxStaticLine *topDivider = new wxStaticLine(this);
	mainSizer->Add(topDivider, 0, wxEXPAND);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	m_cmbParserError = new wxStaticText (
		this,
		wxID_STATIC,
		wxEmptyString
	);
	m_cmbParserError->SetForegroundColour(*wxRED);
	secondRow->Add(m_cmbParserError, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mainSizer->Add(secondRow, 0, wxALIGN_CENTER_HORIZONTAL);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *churchNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("CMB was created for: ")
	);
	thirdRow->Add(churchNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_cmbChurchAddress = new wxStaticText (
		this,
		wxID_STATIC,
		wxEmptyString
	);
	thirdRow->Add(m_cmbChurchAddress, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mainSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *originalOdfText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("The ODF path: ")
	);
	fourthRow->Add(originalOdfText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_cmbOdfPath = new wxStaticText (
		this,
		wxID_STATIC,
		wxEmptyString
	);
	fourthRow->Add(m_cmbOdfPath, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	mainSizer->Add(fourthRow, 0, wxGROW);

	wxStaticLine *optionsDivider = new wxStaticLine(this);
	mainSizer->Add(optionsDivider, 0, wxEXPAND);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	fifthRow->AddStretchSpacer();
	m_importAmplitudeCheck = new wxCheckBox(
		this,
		ID_IMPORT_AMPLITUDE_CHECK,
		wxT("Import all existing Amplitude data"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_importAmplitudeCheck->SetValue(m_importAmplitude);
	fifthRow->Add(m_importAmplitudeCheck, 0, wxALL, 5);
	m_importGainCheck = new wxCheckBox(
		this,
		ID_IMPORT_GAIN_CHECK,
		wxT("Import all existing Gain data"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_importGainCheck->SetValue(m_importGain);
	fifthRow->Add(m_importGainCheck, 0, wxALL, 5);
	fifthRow->AddStretchSpacer();
	mainSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	sixthRow->AddStretchSpacer();
	m_importPitchTuningCheck = new wxCheckBox(
		this,
		ID_IMPORT_PITCH_TUNING_CHECK,
		wxT("Import all existing PitchTuning data"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_importPitchTuningCheck->SetValue(m_importPitchTuning);
	sixthRow->Add(m_importPitchTuningCheck, 0, wxALL, 5);
	m_importPitchCorrectionCheck = new wxCheckBox(
		this,
		ID_IMPORT_PITCH_CORRECTION_CHECK,
		wxT("Import all existing PitchCorrection data"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_importPitchCorrectionCheck->SetValue(m_importPitchCorrection);
	sixthRow->Add(m_importPitchCorrectionCheck, 0, wxALL, 5);
	sixthRow->AddStretchSpacer();
	mainSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	seventhRow->AddStretchSpacer();
	m_importTrackerDelayCheck = new wxCheckBox(
		this,
		ID_IMPORT_TRACKER_DELAY_CHECK,
		wxT("Import all existing TrackerDelay data"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_importTrackerDelayCheck->SetValue(m_importTrackerDelay);
	seventhRow->Add(m_importTrackerDelayCheck, 0, wxALL, 5);
	seventhRow->AddStretchSpacer();
	mainSizer->Add(seventhRow, 0, wxGROW);

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
		wxT("Import selected .cmb data")
	);
	theOkButton->Disable();
	bottomRow->Add(theOkButton, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	mainSizer->Add(bottomRow, 0, wxGROW);

	SetSizer(mainSizer);
}

CMB_ORGAN* CmbDialog::GetCmbOrgan() {
	return &m_importedCmbOrgan;
}

bool CmbDialog::GetImportAmplitude() {
	return m_importAmplitude;
}

bool CmbDialog::GetImportGain() {
	return m_importGain;
}

bool CmbDialog::GetImportPitchTuning() {
	return m_importPitchTuning;
}

bool CmbDialog::GetImportPitchCorrection() {
	return m_importPitchCorrection;
}

bool CmbDialog::GetImportTrackerDelay() {
	return m_importTrackerDelay;
}

void CmbDialog::OnBrowseForCmbBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString defaultPath = ::wxGetApp().m_frame->GetDefaultCmbDirectory();
	if (defaultPath == wxEmptyString)
		defaultPath = wxStandardPaths::Get().GetDocumentsDir();
	wxFileDialog fileDialog(
		this,
		wxT("Select .cmb file to import"),
		defaultPath,
		"",
		"GrandOrgue CMB files (*.cmb)|*.cmb;*.CMB",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_OK) {
		if (m_cmbParser) {
			delete m_cmbParser;
			m_cmbParser = NULL;
		}
		if (m_importedCmbOrgan.cmbRanks.size() > 0 || m_importedCmbOrgan.cmbStops.size() > 0) {
			// Any previously added information that could be left over must now be removed
			m_importedCmbOrgan.churchName = wxEmptyString;
			m_importedCmbOrgan.odfPath = wxEmptyString;
			m_importedCmbOrgan.cmbRanks.clear();
			m_importedCmbOrgan.cmbStops.clear();
		}
		m_cmbParser = new CmbParser(fileDialog.GetPath(), GetCmbOrgan());
		if (m_cmbParser->IsParsedOk()) {
			// The m_importedCmbOrgan should now hold all the voicing information from the .cmb file
			m_cmbPathField->ChangeValue(fileDialog.GetPath());
			m_cmbParserError->SetLabelText(wxEmptyString);
		} else {
			m_cmbParserError->SetLabelText(m_cmbParser->GetErrorText());
		}
		m_cmbChurchAddress->SetLabelText(m_importedCmbOrgan.churchName);
		m_cmbOdfPath->SetLabelText(m_importedCmbOrgan.odfPath);
	}
	DecideStateOfOkButton();
}

void CmbDialog::OnImportAmplitudeCheck(wxCommandEvent& WXUNUSED(event)) {
	m_importAmplitude = m_importAmplitudeCheck->IsChecked();
}

void CmbDialog::OnImportGainCheck(wxCommandEvent& WXUNUSED(event)) {
	m_importGain = m_importGainCheck->IsChecked();
}

void CmbDialog::OnImportPitchTuningCheck(wxCommandEvent& WXUNUSED(event)) {
	m_importPitchTuning = m_importPitchTuningCheck->IsChecked();
}

void CmbDialog::OnImportPitchCorrectionCheck(wxCommandEvent& WXUNUSED(event)) {
	m_importPitchCorrection = m_importPitchCorrectionCheck->IsChecked();
}

void CmbDialog::OnImportTrackerDelayCheck(wxCommandEvent& WXUNUSED(event)) {
	m_importTrackerDelay = m_importTrackerDelayCheck->IsChecked();
}

void CmbDialog::DecideStateOfOkButton() {
	bool shouldBeEnabled = false;
	if (m_cmbParser) {
		if (m_cmbParser->IsParsedOk())
			shouldBeEnabled = true;
	}
	if (shouldBeEnabled) {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Enable();
	} else {
		wxButton *okBtn = (wxButton*) FindWindow(wxID_OK);
		okBtn->Disable();
	}
}
