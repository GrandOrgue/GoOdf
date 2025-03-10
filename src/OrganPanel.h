/*
 * OrganPanel.h is part of GoOdf.
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

#ifndef ORGANPANEL_H
#define ORGANPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Organ.h"
#include "GOODFDef.h"

class OrganPanel : public wxPanel {
public:
	OrganPanel(Organ *organ, wxWindow *parent);
	~OrganPanel();

	void setCurrentOrgan(Organ *organ);
	wxString getOdfPath();
	wxString getOdfName();
	void setOdfPath(wxString path);
	void setOdfName(wxString name);
	void setTooltipsEnabled(bool isEnabled);
	void fixTrailingSpacesInStrings();
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxString m_odfPath;
	wxString m_odfName;
	Organ *m_currentOrgan;

	wxTextCtrl *m_odfPathField;
	wxTextCtrl *m_organNameField;
	wxTextCtrl *m_churchNameField;
	wxTextCtrl *m_churchAddressField;
	wxTextCtrl *m_organBuilderField;
	wxTextCtrl *m_organBuildDateField;
	wxTextCtrl *m_organCommentsField;
	wxTextCtrl *m_recordingDetailsField;
	wxTextCtrl *m_infoPathField;
	wxRadioButton *m_interManualYes;
	wxRadioButton *m_interManualNo;
	wxRadioButton *m_intraManualYes;
	wxRadioButton *m_intraManualNo;
	wxRadioButton *m_storeTremulantsYes;
	wxRadioButton *m_storeTremulantsNo;
	wxRadioButton *m_storeDivCouplersYes;
	wxRadioButton *m_storeDivCouplersNo;
	wxRadioButton *m_storeNonDisplayedYes;
	wxRadioButton *m_storeNonDisplayedNo;
	wxSpinCtrlDouble *m_amplitudeLevelSpin;
	wxSpinCtrlDouble *m_gainSpin;
	wxSpinCtrlDouble *m_pitchTuningSpin;
	wxSpinCtrl *m_trackerDelaySpin;
	wxSpinCtrlDouble *m_pitchCorrectionSpin;
	wxRadioButton *m_isPercussiveYes;
	wxRadioButton *m_isPercussiveNo;
	wxRadioButton *m_hasIndependentReleaseYes;
	wxRadioButton *m_hasIndependentReleaseNo;
	wxButton *m_newOrganBtn;

	void getDataFromOrgan();
	void OnBrowseForOrganPath(wxCommandEvent& event);
	wxString GetDirectoryPath();
	void OnOrganFileNameChange(wxCommandEvent& event);
	void OnChurchNameChange(wxCommandEvent& event);
	void OnChurchAddressChange(wxCommandEvent& event);
	void OnOrganBuilderChange(wxCommandEvent& event);
	void OnOrganBuildDateChange(wxCommandEvent& event);
	void OnOrganCommentsChange(wxCommandEvent& event);
	void OnRecordingDetailsChange(wxCommandEvent& event);
	void OnBrowseForInfoPath(wxCommandEvent& event);
	void OnInterManualRBChange(wxCommandEvent& event);
	void OnIntraManualRBChange(wxCommandEvent& event);
	void OnStoreTremulantsChange(wxCommandEvent& event);
	void OnStoreDivCouplersRBChange(wxCommandEvent& event);
	void OnStoreNonDispRBChange(wxCommandEvent& event);
	void OnAmplitudeLevelSpin(wxSpinDoubleEvent& event);
	void OnGainSpin(wxSpinDoubleEvent& event);
	void OnPitchTuningSpin(wxSpinDoubleEvent& event);
	void OnTrackerDelaySpin(wxSpinEvent& event);
	void OnPitchCorrectionSpin(wxSpinDoubleEvent& event);
	void OnPercussiveSelection(wxCommandEvent& event);
	void OnIndependentReleaseSelection(wxCommandEvent& event);
	void OnNewOrganBtn(wxCommandEvent& event);
};

#endif
