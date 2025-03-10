/*
 * WindchestgroupPanel.h is part of GoOdf.
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

#ifndef WINDCHESTPANEL_H
#define WINDCHESTPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Windchestgroup.h"

class WindchestgroupPanel : public wxPanel {
public:
	WindchestgroupPanel(wxWindow *parent);
	~WindchestgroupPanel();

	void setWindchest(Windchestgroup *windchest);
	void setIsFirstRemoval(bool value);
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxListBox *m_availableEnclosures;
	wxListBox *m_referencedEnclosures;
	wxButton *m_addReferencedEnclosure;
	wxButton *m_removeReferencedEnclosure;
	wxListBox *m_availableTremulants;
	wxListBox *m_referencedTremulants;
	wxButton *m_addReferencedTremulant;
	wxButton *m_removeReferencedTremulant;
	wxSpinCtrlDouble *m_amplitudeLevelSpin;
	wxSpinCtrlDouble *m_gainSpin;
	wxSpinCtrlDouble *m_pitchTuningSpin;
	wxSpinCtrl *m_trackerDelaySpin;
	wxSpinCtrlDouble *m_pitchCorrectionSpin;
	wxRadioButton *m_isPercussiveYes;
	wxRadioButton *m_isPercussiveNo;
	wxRadioButton *m_hasIndependentReleaseYes;
	wxRadioButton *m_hasIndependentReleaseNo;
	wxButton *removeWindchestBtn;

	Windchestgroup *m_windchest;
	bool m_isFirstRemoval;

	void OnNameChange(wxCommandEvent& event);
	void OnAddReferencedEnclosure(wxCommandEvent& event);
	void OnRemoveReferencedEnclosure(wxCommandEvent& event);
	void OnAddReferencedTremulant(wxCommandEvent& event);
	void OnRemoveReferencedTremulant(wxCommandEvent& event);
	void OnRemoveWindchestBtn(wxCommandEvent& event);
	void DoRemoveWindchest();
	void OnEnclosureListboxSelection(wxCommandEvent& event);
	void OnTremulantListboxSelection(wxCommandEvent& event);
	void UpdateReferencedEnclosures();
	void OnReferencedEnclosureSelection(wxCommandEvent& event);
	void OnReferencedTremulantSelection(wxCommandEvent& event);
	void UpdateReferencedTremulants();
	void OnAmplitudeLevelSpin(wxSpinDoubleEvent& event);
	void OnGainSpin(wxSpinDoubleEvent& event);
	void OnPitchTuningSpin(wxSpinDoubleEvent& event);
	void OnTrackerDelaySpin(wxSpinEvent& event);
	void OnPitchCorrectionSpin(wxSpinDoubleEvent& event);
	void OnPercussiveSelection(wxCommandEvent& event);
	void OnIndependentReleaseSelection(wxCommandEvent& event);

};

#endif
