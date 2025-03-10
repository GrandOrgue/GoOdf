/*
 * OrganPanel.cpp is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * GoOdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General wxLicense as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GoOdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General wxLicense for more details.
 *
 * You should have received a copy of the GNU General wxLicense
 * along with GoOdf. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo DOT se
 */

#include "OrganPanel.h"
#include <wx/dirdlg.h>
#include <wx/button.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/statline.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/filefn.h>
#include <wx/radiobut.h>
#include "GOODFFunctions.h"
#include "GOODF.h"

// Event table
BEGIN_EVENT_TABLE(OrganPanel, wxPanel)
	EVT_BUTTON(ID_BROWSE_FOR_ODF_PATH, OrganPanel::OnBrowseForOrganPath)
	EVT_TEXT(ID_ORGAN_FILE_NAME, OrganPanel::OnOrganFileNameChange)
	EVT_TEXT(ID_CHURCH_NAME_TEXT, OrganPanel::OnChurchNameChange)
	EVT_TEXT(ID_CHURCH_ADDRESS_TEXT, OrganPanel::OnChurchAddressChange)
	EVT_TEXT(ID_ORGAN_BUILDER_TEXT, OrganPanel::OnOrganBuilderChange)
	EVT_TEXT(ID_ORGAN_BUILDDATE_TEXT, OrganPanel::OnOrganBuildDateChange)
	EVT_TEXT(ID_ORGAN_COMMENTS_TEXT ,OrganPanel::OnOrganCommentsChange)
	EVT_TEXT(ID_RECORDING_DETAILS_TEXT, OrganPanel::OnRecordingDetailsChange)
	EVT_BUTTON(ID_BROWSE_FOR_INFO_PATH, OrganPanel::OnBrowseForInfoPath)
	EVT_RADIOBUTTON(ID_INTER_MAN_RB_YES, OrganPanel::OnInterManualRBChange)
	EVT_RADIOBUTTON(ID_INTER_MAN_RB_NO, OrganPanel::OnInterManualRBChange)
	EVT_RADIOBUTTON(ID_INTRA_MAN_RB_YES, OrganPanel::OnIntraManualRBChange)
	EVT_RADIOBUTTON(ID_INTRA_MAN_RB_NO, OrganPanel::OnIntraManualRBChange)
	EVT_RADIOBUTTON(ID_STORE_TREM_YES, OrganPanel::OnStoreTremulantsChange)
	EVT_RADIOBUTTON(ID_STORE_TREM_NO, OrganPanel::OnStoreTremulantsChange)
	EVT_RADIOBUTTON(ID_STORE_DIV_CPLRS_YES, OrganPanel::OnStoreDivCouplersRBChange)
	EVT_RADIOBUTTON(ID_STORE_DIV_CPLRS_NO, OrganPanel::OnStoreDivCouplersRBChange)
	EVT_RADIOBUTTON(ID_STORE_NON_DISP_YES, OrganPanel::OnStoreNonDispRBChange)
	EVT_RADIOBUTTON(ID_STORE_NON_DISP_NO, OrganPanel::OnStoreNonDispRBChange)
	EVT_SPINCTRLDOUBLE(ID_ORGAN_AMP_LVL_SPIN, OrganPanel::OnAmplitudeLevelSpin)
	EVT_SPINCTRLDOUBLE(ID_ORGAN_GAIN_SPIN, OrganPanel::OnGainSpin)
	EVT_SPINCTRLDOUBLE(ID_ORGAN_PITCH_SPIN, OrganPanel::OnPitchTuningSpin)
	EVT_SPINCTRL(ID_ORGAN_TRACKER_DELAY_SPIN, OrganPanel::OnTrackerDelaySpin)
	EVT_SPINCTRLDOUBLE(ID_ORGAN_PITCH_CORRECTION_SPIN, OrganPanel::OnPitchCorrectionSpin)
	EVT_RADIOBUTTON(ID_ORGAN_PERCUSSIVE_YES, OrganPanel::OnPercussiveSelection)
	EVT_RADIOBUTTON(ID_ORGAN_PERCUSSIVE_NO, OrganPanel::OnPercussiveSelection)
	EVT_RADIOBUTTON(ID_ORGAN_INDEPENDENT_RELEASE_YES, OrganPanel::OnIndependentReleaseSelection)
	EVT_RADIOBUTTON(ID_ORGAN_INDEPENDENT_RELEASE_NO, OrganPanel::OnIndependentReleaseSelection)
	EVT_BUTTON(ID_NEW_ORGAN, OrganPanel::OnNewOrganBtn)
END_EVENT_TABLE()

OrganPanel::OrganPanel(Organ *organ, wxWindow *parent) : wxPanel (
		parent,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxTAB_TRAVERSAL|wxNO_BORDER
	) {
	m_odfPath = wxEmptyString;
	m_odfName = wxEmptyString;
	m_currentOrgan = organ;

	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *topRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *odfPathText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Location of the .organ file: ")
	);
	topRow->Add(odfPathText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_odfPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		m_odfPath,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	topRow->Add(m_odfPathField, 1, wxEXPAND|wxALL, 5);
	wxButton *selectPath = new wxButton(
		this,
		ID_BROWSE_FOR_ODF_PATH,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	topRow->Add(selectPath, 0, wxALL, 5);
	mainSizer->Add(topRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *organNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Name of .organ file (without the extension): ")
	);
	secondRow->Add(organNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_organNameField = new wxTextCtrl(
		this,
		ID_ORGAN_FILE_NAME,
		m_odfName,
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_organNameField, 1, wxEXPAND|wxALL, 5);
	mainSizer->Add(secondRow, 0, wxGROW);

	wxStaticLine *topDivider = new wxStaticLine(this);
	mainSizer->Add(topDivider, 0, wxEXPAND);

	wxStaticBoxSizer *organProperties = new wxStaticBoxSizer(wxVERTICAL, this, wxT("Organ properties"));

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *churchNameText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Church name: ")
	);
	thirdRow->Add(churchNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_churchNameField = new wxTextCtrl(
		organProperties->GetStaticBox(),
		ID_CHURCH_NAME_TEXT,
		m_currentOrgan->getChurchName(),
		wxDefaultPosition,
		wxDefaultSize
	);
	thirdRow->Add(m_churchNameField, 1, wxEXPAND|wxALL, 5);
	organProperties->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *churchAddressText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Church address: ")
	);
	fourthRow->Add(churchAddressText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_churchAddressField = new wxTextCtrl(
		organProperties->GetStaticBox(),
		ID_CHURCH_ADDRESS_TEXT,
		m_currentOrgan->getChurchAddress(),
		wxDefaultPosition,
		wxDefaultSize
	);
	fourthRow->Add(m_churchAddressField, 1, wxEXPAND|wxALL, 5);
	organProperties->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *organBuilderText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Organ builder: ")
	);
	fifthRow->Add(organBuilderText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_organBuilderField = new wxTextCtrl(
		organProperties->GetStaticBox(),
		ID_ORGAN_BUILDER_TEXT,
		m_currentOrgan->getOrganBuilder(),
		wxDefaultPosition,
		wxDefaultSize
	);
	fifthRow->Add(m_organBuilderField, 1, wxEXPAND|wxALL, 5);
	organProperties->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *organBuildDateText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Organ build date: ")
	);
	sixthRow->Add(organBuildDateText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_organBuildDateField = new wxTextCtrl(
		organProperties->GetStaticBox(),
		ID_ORGAN_BUILDDATE_TEXT,
		m_currentOrgan->getOrganBuildDate(),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_organBuildDateField, 1, wxEXPAND|wxALL, 5);
	organProperties->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *organCommentsText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Organ comments: ")
	);
	seventhRow->Add(organCommentsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_organCommentsField = new wxTextCtrl(
		organProperties->GetStaticBox(),
		ID_ORGAN_COMMENTS_TEXT,
		m_currentOrgan->getOrganComments(),
		wxDefaultPosition,
		wxDefaultSize
	);
	seventhRow->Add(m_organCommentsField, 1, wxEXPAND|wxALL, 5);
	organProperties->Add(seventhRow, 0, wxGROW);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *recordingDetailsText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Recording details: ")
	);
	eighthRow->Add(recordingDetailsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_recordingDetailsField = new wxTextCtrl(
		organProperties->GetStaticBox(),
		ID_RECORDING_DETAILS_TEXT,
		m_currentOrgan->getRecordingDetails(),
		wxDefaultPosition,
		wxDefaultSize
	);
	eighthRow->Add(m_recordingDetailsField, 1, wxEXPAND|wxALL, 5);
	organProperties->Add(eighthRow, 0, wxGROW);

	wxBoxSizer *ninthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *infoPathText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Organinfo file: ")
	);
	ninthRow->Add(infoPathText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_infoPathField = new wxTextCtrl(
		organProperties->GetStaticBox(),
		wxID_ANY,
		m_currentOrgan->getInfoFilename(),
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	ninthRow->Add(m_infoPathField, 1, wxEXPAND|wxALL, 5);
	wxButton *infoPath = new wxButton(
		organProperties->GetStaticBox(),
		ID_BROWSE_FOR_INFO_PATH,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	ninthRow->Add(infoPath, 0, wxALL, 5);
	organProperties->Add(ninthRow, 0, wxGROW);

	wxBoxSizer *tenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *interManualText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Divisionals store intermanual couplers: ")
	);
	tenthRow->Add(interManualText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_interManualYes = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_INTER_MAN_RB_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_interManualYes->SetValue(true);
	tenthRow->Add(m_interManualYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_interManualNo = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_INTER_MAN_RB_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	tenthRow->Add(m_interManualNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	tenthRow->AddStretchSpacer();
	wxStaticText *intraManualText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Divisionals store intramanual couplers: ")
	);
	tenthRow->Add(intraManualText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_intraManualYes = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_INTRA_MAN_RB_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_intraManualYes->SetValue(true);
	tenthRow->Add(m_intraManualYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_intraManualNo = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_INTRA_MAN_RB_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	tenthRow->Add(m_intraManualNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	organProperties->Add(tenthRow, 0, wxGROW);

	wxBoxSizer *eleventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeTremulantsText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Divisionals store tremulants: ")
	);
	eleventhRow->Add(storeTremulantsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeTremulantsYes = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_STORE_TREM_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeTremulantsYes->SetValue(true);
	eleventhRow->Add(m_storeTremulantsYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeTremulantsNo = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_STORE_TREM_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	eleventhRow->Add(m_storeTremulantsNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	organProperties->Add(eleventhRow, 0, wxGROW);

	wxBoxSizer *twelfthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeDivCouplersText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Generals store divisional couplers: ")
	);
	twelfthRow->Add(storeDivCouplersText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeDivCouplersYes = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_STORE_DIV_CPLRS_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeDivCouplersYes->SetValue(true);
	twelfthRow->Add(m_storeDivCouplersYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeDivCouplersNo = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_STORE_DIV_CPLRS_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	twelfthRow->Add(m_storeDivCouplersNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	organProperties->Add(twelfthRow, 0, wxGROW);

	wxBoxSizer *thirteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeNonDisplayedText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Combinations store non displayed drawstops: ")
	);
	thirteenthRow->Add(storeNonDisplayedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeNonDisplayedYes = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_STORE_NON_DISP_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeNonDisplayedYes->SetValue(true);
	thirteenthRow->Add(m_storeNonDisplayedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeNonDisplayedNo = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_STORE_NON_DISP_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	thirteenthRow->Add(m_storeNonDisplayedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	organProperties->Add(thirteenthRow, 0, wxGROW);

	wxBoxSizer *fourteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *isPercussiveText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Percussive: ")
	);
	fourteenthRow->Add(isPercussiveText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveYes = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_ORGAN_PERCUSSIVE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	fourteenthRow->Add(m_isPercussiveYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveNo = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_ORGAN_PERCUSSIVE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	fourteenthRow->Add(m_isPercussiveNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isPercussiveNo->SetValue(true);
	fourteenthRow->AddStretchSpacer();
	wxStaticText *independentReleaseText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Has independent release: ")
	);
	fourteenthRow->Add(independentReleaseText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasIndependentReleaseYes = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_ORGAN_INDEPENDENT_RELEASE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	fourteenthRow->Add(m_hasIndependentReleaseYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasIndependentReleaseNo = new wxRadioButton(
		organProperties->GetStaticBox(),
		ID_ORGAN_INDEPENDENT_RELEASE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	fourteenthRow->Add(m_hasIndependentReleaseNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasIndependentReleaseNo->SetValue(true);
	m_hasIndependentReleaseYes->Enable(false);
	m_hasIndependentReleaseNo->Enable(false);
	organProperties->Add(fourteenthRow, 0, wxGROW);

	wxBoxSizer *fifteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *ampLvlText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("AmplitudeLevel: ")
	);
	fifteenthRow->Add(ampLvlText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_amplitudeLevelSpin = new wxSpinCtrlDouble(
		organProperties->GetStaticBox(),
		ID_ORGAN_AMP_LVL_SPIN,
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
	fifteenthRow->Add(m_amplitudeLevelSpin, 0, wxEXPAND|wxALL, 5);
	wxStaticText *gainText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("Gain: ")
	);
	fifteenthRow->Add(gainText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gainSpin = new wxSpinCtrlDouble(
		organProperties->GetStaticBox(),
		ID_ORGAN_GAIN_SPIN,
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
	fifteenthRow->Add(m_gainSpin, 0, wxEXPAND|wxALL, 5);
	fifteenthRow->AddStretchSpacer();
	wxStaticText *pitchText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("PitchTuning: ")
	);
	fifteenthRow->Add(pitchText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pitchTuningSpin = new wxSpinCtrlDouble(
		organProperties->GetStaticBox(),
		ID_ORGAN_PITCH_SPIN,
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
	fifteenthRow->Add(m_pitchTuningSpin, 0, wxEXPAND|wxALL, 5);
	organProperties->Add(fifteenthRow, 0, wxGROW);

	wxBoxSizer *sixteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *trackerDelayText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("TrackerDelay: ")
	);
	sixteenthRow->Add(trackerDelayText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trackerDelaySpin = new wxSpinCtrl(
		organProperties->GetStaticBox(),
		ID_ORGAN_TRACKER_DELAY_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		10000,
		0
	);
	sixteenthRow->Add(m_trackerDelaySpin, 0, wxGROW|wxALL, 5);
	sixteenthRow->AddStretchSpacer();
	wxStaticText *pitchCorrText = new wxStaticText (
		organProperties->GetStaticBox(),
		wxID_STATIC,
		wxT("PitchCorrection: ")
	);
	sixteenthRow->Add(pitchCorrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pitchCorrectionSpin = new wxSpinCtrlDouble(
		organProperties->GetStaticBox(),
		ID_ORGAN_PITCH_CORRECTION_SPIN,
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
	sixteenthRow->Add(m_pitchCorrectionSpin, 0, wxGROW|wxALL, 5);
	organProperties->Add(sixteenthRow, 0, wxGROW);

	mainSizer->Add(organProperties, 1, wxEXPAND|wxALL, 5);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	mainSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	m_newOrganBtn = new wxButton(
		this,
		ID_NEW_ORGAN,
		wxT("New organ (reset everything)")
	);
	bottomRow->Add(m_newOrganBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	mainSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizerAndFit(mainSizer);
}

OrganPanel::~OrganPanel() {

}

void OrganPanel::setCurrentOrgan(Organ *organ) {
	m_currentOrgan = organ;
	getDataFromOrgan();
}

wxString OrganPanel::getOdfPath() {
	return m_odfPath;
}

wxString OrganPanel::getOdfName() {
	return m_odfName;
}

void OrganPanel::setOdfPath(wxString path) {
	m_odfPath = path;
	m_odfPathField->SetValue(m_odfPath);
}

void OrganPanel::setOdfName(wxString name) {
	m_odfName = name;
	m_organNameField->ChangeValue(m_odfName);
}

void OrganPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		wxButton *selectPath = (wxButton*) FindWindow(ID_BROWSE_FOR_ODF_PATH);
		selectPath->SetToolTip(wxT("Begin with setting the location for the .organ file in the file system here."));
		wxButton *infoPath = (wxButton*) FindWindow(ID_BROWSE_FOR_INFO_PATH);
		infoPath->SetToolTip(wxT("An organinfo file can be selected here but it is not necessary and not even supported for organ packages!"));
		m_odfPathField->SetToolTip(wxT("The full path to where the .organ file will be written is shown here. Set it with the 'Browse' button."));
		m_organNameField->SetToolTip(wxT("This is the file name of the .organ file. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved! To 'Save as...' just change the name here and save."));
		m_churchNameField->SetToolTip(wxT("The curch name is an important property as it's the name of the organ as shown in GrandOrgue. Make it unique for every different organ! NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_churchAddressField->SetToolTip(wxT("This value is shown in GrandOrgue. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_organBuilderField->SetToolTip(wxT("This value is shown in GrandOrgue. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_organBuildDateField->SetToolTip(wxT("This value is shown in GrandOrgue. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_organCommentsField->SetToolTip(wxT("This value is shown in GrandOrgue. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_recordingDetailsField->SetToolTip(wxT("This value is shown in GrandOrgue. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_infoPathField->SetToolTip(wxT("NOTE: This value is shown in GrandOrgue, but is not supported for the new organ package format! To clear any file here, click the 'Browse' button to the right and then 'Cancel'!"));
		m_amplitudeLevelSpin->SetToolTip(wxT("Linear volume (expressed as a percentage). A value of 0 (zero) will mute sound output!"));
		m_gainSpin->SetToolTip(wxT("Logarithmic volume. A value of 0 gain is equivalent to an amplitude level of 100."));
		m_pitchTuningSpin->SetToolTip(wxT("Set overall pitch adjustment for 'Original temperament' of the organ here."));
		m_trackerDelaySpin->SetToolTip(wxT("Set overall tracker delay of the organ here."));
		m_pitchCorrectionSpin->SetToolTip(wxT("Set overall pitch adjustment for any other temperament than 'Original' for the organ here."));
		m_isPercussiveYes->SetToolTip(wxT("A percussive samples are played once from start to end. The value can be passed down to any [Rank] and pipes placed on this windchest."));
		m_isPercussiveNo->SetToolTip(wxT("Non-percussive samples have loops and release(s). The value can be inherited down to any [WindchestGroup], [Rank] and [Pipe]."));
		m_hasIndependentReleaseYes->SetToolTip(wxT("Requires that Percussive is set to yes. When this also is set to yes, percussive pipes should have separately defined releases that are played independently of the attacks. The value can be inherited down to any [WindchestGroup], [Rank] and [Pipe]."));
		m_hasIndependentReleaseNo->SetToolTip(wxT("Percussive pipes are played 'as is' without any loop or release handling. The value can be inherited down to any [WindchestGroup], [Rank] and [Pipe]."));
	} else {
		wxButton *selectPath = (wxButton*) FindWindow(ID_BROWSE_FOR_ODF_PATH);
		selectPath->SetToolTip(wxEmptyString);
		wxButton *infoPath = (wxButton*) FindWindow(ID_BROWSE_FOR_INFO_PATH);
		infoPath->SetToolTip(wxEmptyString);
		m_odfPathField->SetToolTip(wxEmptyString);
		m_organNameField->SetToolTip(wxEmptyString);
		m_churchNameField->SetToolTip(wxEmptyString);
		m_churchAddressField->SetToolTip(wxEmptyString);
		m_organBuilderField->SetToolTip(wxEmptyString);
		m_organBuildDateField->SetToolTip(wxEmptyString);
		m_organCommentsField->SetToolTip(wxEmptyString);
		m_recordingDetailsField->SetToolTip(wxEmptyString);
		m_infoPathField->SetToolTip(wxEmptyString);
		m_amplitudeLevelSpin->SetToolTip(wxEmptyString);
		m_gainSpin->SetToolTip(wxEmptyString);
		m_pitchTuningSpin->SetToolTip(wxEmptyString);
		m_trackerDelaySpin->SetToolTip(wxEmptyString);
		m_pitchCorrectionSpin->SetToolTip(wxEmptyString);
		m_hasIndependentReleaseYes->SetToolTip(wxEmptyString);
		m_hasIndependentReleaseNo->SetToolTip(wxEmptyString);
		m_isPercussiveYes->SetToolTip(wxEmptyString);
		m_isPercussiveNo->SetToolTip(wxEmptyString);
	}
}

void OrganPanel::fixTrailingSpacesInStrings() {
	GOODF_functions::RemoveTrailingWhitespace(&m_odfName);
}

void OrganPanel::refreshData() {
	m_organNameField->ChangeValue(m_odfName);
	getDataFromOrgan();
}

void OrganPanel::getDataFromOrgan() {
	m_churchNameField->ChangeValue(m_currentOrgan->getChurchName());
	m_churchAddressField->ChangeValue(m_currentOrgan->getChurchAddress());
	m_organBuilderField->ChangeValue(m_currentOrgan->getOrganBuilder());
	m_organBuildDateField->ChangeValue(m_currentOrgan->getOrganBuildDate());
	m_organCommentsField->ChangeValue(m_currentOrgan->getOrganComments());
	m_recordingDetailsField->ChangeValue(m_currentOrgan->getRecordingDetails());
	m_infoPathField->ChangeValue(GOODF_functions::removeBaseOdfPath(m_currentOrgan->getInfoFilename()));
	if (m_currentOrgan->doesDivisionalsStoreIntermanualCouplers())
		m_interManualYes->SetValue(true);
	else
		m_interManualNo->SetValue(true);
	if (m_currentOrgan->doesDivisionalsStoreIntramanualCouplers())
		m_intraManualYes->SetValue(true);
	else
		m_intraManualNo->SetValue(true);
	if (m_currentOrgan->doesDivisionalsStoreTremulants())
		m_storeTremulantsYes->SetValue(true);
	else
		m_storeTremulantsNo->SetValue(true);
	if (m_currentOrgan->doesGeneralsStoreDivisionalCouplers())
		m_storeDivCouplersYes->SetValue(true);
	else
		m_storeDivCouplersNo->SetValue(true);
	if (m_currentOrgan->doesCombinationsStoreNonDisplayedDrawstops())
		m_storeNonDisplayedYes->SetValue(true);
	else
		m_storeNonDisplayedNo->SetValue(true);
	if (m_currentOrgan->getHasIndependentRelease())
		m_hasIndependentReleaseYes->SetValue(true);
	else
		m_hasIndependentReleaseNo->SetValue(true);
	if (m_currentOrgan->getIsPercussive()) {
		m_isPercussiveYes->SetValue(true);
		m_hasIndependentReleaseYes->Enable();
		m_hasIndependentReleaseNo->Enable();
	} else {
		m_isPercussiveNo->SetValue(true);
		m_hasIndependentReleaseYes->Enable(false);
		m_hasIndependentReleaseNo->Enable(false);
	}
	m_amplitudeLevelSpin->SetValue(m_currentOrgan->getAmplitudeLevel());
	m_gainSpin->SetValue(m_currentOrgan->getGain());
	m_pitchTuningSpin->SetValue(m_currentOrgan->getPitchTuning());
	m_trackerDelaySpin->SetValue(m_currentOrgan->getTrackerDelay());
	m_pitchCorrectionSpin->SetValue(m_currentOrgan->getPitchCorrection());
}

void OrganPanel::OnBrowseForOrganPath(wxCommandEvent& WXUNUSED(event)) {
	wxString oldOdfPath = m_odfPath;
	m_odfPath = GetDirectoryPath();
	m_odfPathField->SetValue(m_odfPath);
	m_currentOrgan->setOdfRoot(m_odfPath);
	m_currentOrgan->updateRelativePipePaths();
	if (!m_infoPathField->IsEmpty()) {
		m_infoPathField->SetValue(GOODF_functions::removeBaseOdfPath(m_currentOrgan->getInfoFilename()));
	}
	if (!m_odfPath.IsSameAs(oldOdfPath))
		::wxGetApp().m_frame->m_organ->setModified(true);
}

wxString OrganPanel::GetDirectoryPath() {
	wxString pathToReturn;
	wxString defaultPath;
	if (m_odfPath != wxEmptyString)
		defaultPath = m_odfPath;
	else {
		if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
			defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
		else
			defaultPath = wxStandardPaths::Get().GetDocumentsDir();
	}

	wxDirDialog dirDialog(
		this,
		wxT("Pick a directory (Cancel will empty the field!)"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (dirDialog.ShowModal() == wxID_OK) {
		pathToReturn = dirDialog.GetPath();
		return pathToReturn;
	} else {
		return wxEmptyString;
	}
}

void OrganPanel::OnOrganFileNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_organNameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_organNameField);
	if (!m_odfName.IsSameAs(m_organNameField->GetValue())) {
		m_odfName = m_organNameField->GetValue();
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void OrganPanel::OnChurchNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_churchNameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_churchNameField);
	m_currentOrgan->setChurchName(m_churchNameField->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnChurchAddressChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_churchAddressField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_churchAddressField);
	m_currentOrgan->setChurchAddress(m_churchAddressField->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnOrganBuilderChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_organBuilderField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_organBuilderField);
	m_currentOrgan->setOrganBuilder(m_organBuilderField->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnOrganBuildDateChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_organBuildDateField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_organBuildDateField);
	m_currentOrgan->setOrganBuildDate(m_organBuildDateField->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnOrganCommentsChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_organCommentsField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_organCommentsField);
	m_currentOrgan->setOrganComments(m_organCommentsField->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnRecordingDetailsChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_recordingDetailsField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_recordingDetailsField);
	m_currentOrgan->setRecordingDetails(m_recordingDetailsField->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnBrowseForInfoPath(wxCommandEvent& WXUNUSED(event)) {
	wxString infoFilePath;
	wxString defaultPath;
	if (m_odfPath != wxEmptyString)
		defaultPath = m_odfPath;
	else
		defaultPath = wxStandardPaths::Get().GetDocumentsDir();

	wxFileDialog fileDialog(
		this,
		wxT("Select organinfo file"),
		defaultPath,
		"",
		"HTML files (*.html;*.htm)|*.html;*.htm",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL) {
		if (m_infoPathField->GetValue() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Info file value is not empty! Do you want to empty it?"), wxT("Empty info file value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				m_currentOrgan->setInfoFilename(wxEmptyString);
				m_infoPathField->SetValue(wxEmptyString);
			}
		}
		return;
	}

	infoFilePath = fileDialog.GetPath();
	m_currentOrgan->setInfoFilename(infoFilePath);
	m_infoPathField->SetValue(GOODF_functions::removeBaseOdfPath(infoFilePath));
	::wxGetApp().m_frame->m_organ->setModified(true);

}

void OrganPanel::OnInterManualRBChange(wxCommandEvent& event) {
	if (event.GetId() == ID_INTER_MAN_RB_YES) {
		m_interManualYes->SetValue(true);
		m_currentOrgan->setDivisionalsStoreIntermanualCouplers(true);
	} else {
		m_interManualNo->SetValue(true);
		m_currentOrgan->setDivisionalsStoreIntermanualCouplers(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnIntraManualRBChange(wxCommandEvent& event) {
	if (event.GetId() == ID_INTRA_MAN_RB_YES) {
		m_intraManualYes->SetValue(true);
		m_currentOrgan->setDivisionalsStoreIntramanualCouplers(true);
	} else {
		m_intraManualNo->SetValue(true);
		m_currentOrgan->setDivisionalsStoreIntramanualCouplers(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnStoreTremulantsChange(wxCommandEvent& event) {
	if (event.GetId() == ID_STORE_TREM_YES) {
		m_storeTremulantsYes->SetValue(true);
		m_currentOrgan->setDivisionalsStoreTremulants(true);
	} else {
		m_storeTremulantsNo->SetValue(true);
		m_currentOrgan->setDivisionalsStoreTremulants(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnStoreDivCouplersRBChange(wxCommandEvent& event) {
	if (event.GetId() == ID_STORE_DIV_CPLRS_YES) {
		m_storeDivCouplersYes->SetValue(true);
		m_currentOrgan->setGeneralsStoreDivisionalCouplers(true);
	} else {
		m_storeDivCouplersNo->SetValue(true);
		m_currentOrgan->setGeneralsStoreDivisionalCouplers(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnStoreNonDispRBChange(wxCommandEvent& event) {
	if (event.GetId() == ID_STORE_NON_DISP_YES) {
		m_storeNonDisplayedYes->SetValue(true);
		m_currentOrgan->setCombinationsStoreNonDisplayedDrawstops(true);
	} else {
		m_storeNonDisplayedNo->SetValue(true);
		m_currentOrgan->setCombinationsStoreNonDisplayedDrawstops(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void OrganPanel::OnNewOrganBtn(wxCommandEvent& WXUNUSED(event)) {
	wxCommandEvent evt(wxEVT_MENU, ID_NEW_ORGAN);
	wxPostEvent(this, evt);
}

void OrganPanel::OnAmplitudeLevelSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentOrgan->setAmplitudeLevel(m_amplitudeLevelSpin->GetValue());
	m_currentOrgan->setModified(true);
}

void OrganPanel::OnGainSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentOrgan->setGain(m_gainSpin->GetValue());
	m_currentOrgan->setModified(true);
}

void OrganPanel::OnPitchTuningSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentOrgan->setPitchTuning(m_pitchTuningSpin->GetValue());
	m_currentOrgan->setModified(true);
}

void OrganPanel::OnTrackerDelaySpin(wxSpinEvent& WXUNUSED(event)) {
	m_currentOrgan->setTrackerDelay(m_trackerDelaySpin->GetValue());
	m_currentOrgan->setModified(true);
}

void OrganPanel::OnPitchCorrectionSpin(wxSpinDoubleEvent& WXUNUSED(event)) {
	m_currentOrgan->setPitchCorrection(m_pitchCorrectionSpin->GetValue());
	m_currentOrgan->setModified(true);
}

void OrganPanel::OnPercussiveSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_ORGAN_PERCUSSIVE_YES) {
		m_currentOrgan->setIsPercussive(true);
		m_hasIndependentReleaseYes->Enable();
		m_hasIndependentReleaseNo->Enable();

		if (m_currentOrgan->getNumberOfWindchestgroups()) {
			// ask if this should be applied to existing windchests
			wxMessageDialog msg(this, wxT("Do you want the Percussive=Y value to be applied to existing windchestgroups recursively?\n\nWARNING! Purely percussive pipes cannot have any releases, so any existing releases in a pipe on this windchest will be removed!"), wxT("Apply to existing windchestgroups recursively?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// apply Percussive value to all windchestgroups
				for (unsigned i = 0; i < m_currentOrgan->getNumberOfWindchestgroups(); i++) {
					Windchestgroup *w = m_currentOrgan->getOrganWindchestgroupAt(i);
					w->setIsPercussive(true);
					if (w->isPipesOnThisWindchest())
						w->applyPercussiveRecursively();
				}
			}
		}
	} else {
		m_currentOrgan->setIsPercussive(false);
		// HasIndependentRelease requires that Percussive=Y so it must now be set to false
		m_currentOrgan->setHasIndependentRelease(false);
		m_hasIndependentReleaseNo->SetValue(true);
		m_hasIndependentReleaseYes->Enable(false);
		m_hasIndependentReleaseNo->Enable(false);

		if (m_currentOrgan->getNumberOfWindchestgroups()) {
			// ask if this should be applied to existing windchests
			wxMessageDialog msg(this, wxT("Do you want the Percussive=N value to be applied to existing windchestgroups recursively?"), wxT("Apply to existing windchestgroups recursively?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// apply Percussive value to all windchestgroups
				for (unsigned i = 0; i < m_currentOrgan->getNumberOfWindchestgroups(); i++) {
					Windchestgroup *w = m_currentOrgan->getOrganWindchestgroupAt(i);
					w->setIsPercussive(true);
					if (w->isPipesOnThisWindchest())
						w->applyPercussiveRecursively();
				}
			}
		}
	}
	m_currentOrgan->setModified(true);
}

void OrganPanel::OnIndependentReleaseSelection(wxCommandEvent& event) {
	if (event.GetId() == ID_ORGAN_INDEPENDENT_RELEASE_YES) {
		m_currentOrgan->setHasIndependentRelease(true);
	} else {
		m_currentOrgan->setHasIndependentRelease(false);
	}
	m_currentOrgan->setModified(true);
}
