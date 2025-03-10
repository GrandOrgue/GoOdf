/*
 * CmbDialog.h is part of GoOdf.
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

#ifndef CMBDIALOG_H
#define CMBDIALOG_H

#include <wx/wx.h>
#include "CmbOrgan.h"
#include "CmbParser.h"

class CmbDialog : public wxDialog {
	DECLARE_CLASS(CmbDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	CmbDialog();
	CmbDialog(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Extract voicing data from .cmb file"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~CmbDialog();

	// Initialize our variables
	void Init();

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Extract voicing data from .cmb file"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

	// Accessors
	CMB_ORGAN* GetCmbOrgan();
	bool GetImportAmplitude();
	bool GetImportGain();
	bool GetImportPitchTuning();
	bool GetImportPitchCorrection();
	bool GetImportTrackerDelay();

private:
	CMB_ORGAN m_importedCmbOrgan;
	CmbParser *m_cmbParser;
	bool m_importAmplitude;
	bool m_importGain;
	bool m_importPitchTuning;
	bool m_importPitchCorrection;
	bool m_importTrackerDelay;

	wxTextCtrl *m_cmbPathField;
	wxButton *m_browseForCmbBtn;
	wxStaticText *m_cmbChurchAddress;
	wxStaticText *m_cmbOdfPath;
	wxStaticText *m_cmbParserError;
	wxCheckBox *m_importAmplitudeCheck;
	wxCheckBox *m_importGainCheck;
	wxCheckBox *m_importPitchTuningCheck;
	wxCheckBox *m_importPitchCorrectionCheck;
	wxCheckBox *m_importTrackerDelayCheck;

	// Event methods
	void OnBrowseForCmbBtn(wxCommandEvent& event);
	void OnImportAmplitudeCheck(wxCommandEvent& event);
	void OnImportGainCheck(wxCommandEvent& event);
	void OnImportPitchTuningCheck(wxCommandEvent& event);
	void OnImportPitchCorrectionCheck(wxCommandEvent& event);
	void OnImportTrackerDelayCheck(wxCommandEvent& event);

	void DecideStateOfOkButton();
};

#endif
