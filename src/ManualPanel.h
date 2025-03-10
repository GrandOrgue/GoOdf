/*
 * ManualPanel.h is part of GoOdf.
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

#ifndef MANUALPANEL_H
#define MANUALPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Manual.h"
#include <wx/checkbox.h>

class ManualPanel : public wxPanel {
public:
	ManualPanel(wxWindow *parent);
	~ManualPanel();

	void setManual(Manual *manual);
	void setIsFirstRemoval(bool value);
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxCheckBox *m_thisIsThePedalCheckbox;
	wxSpinCtrl *m_numberOfLogicalKeysSpin;
	wxSpinCtrl *m_firstAccessibleKeyLogicalKeyNumberSpin;
	wxSpinCtrl *m_firstAccessibleKeyMIDINoteNumberSpin;
	wxSpinCtrl *m_numberOfAccessibleKeysSpin;
	wxSpinCtrl *m_midiInputNumberSpin;
	wxButton *m_addNewStop;
	wxSpinCtrl *m_addStopSpin;
	wxButton *m_addNewCoupler;
	wxButton *m_addNewDivisional;
	wxListBox *m_availableTremulants;
	wxListBox *m_referencedTremulants;
	wxButton *m_addReferencedTremulant;
	wxButton *m_removeReferencedTremulant;
	wxListBox *m_availableSwitches;
	wxListBox *m_referencedSwitches;
	wxButton *m_addReferencedSwitch;
	wxButton *m_removeReferencedSwitch;
	wxChoice *m_midiKeyMapKeys;
	wxSpinCtrl *m_midiKeyMapValue;
	wxButton *removeManualBtn;

	Manual *m_manual;
	wxArrayString m_mapKeys;
	bool m_isFirstRemoval;

	void OnNameChange(wxCommandEvent& event);
	void OnPedalCheckbox(wxCommandEvent& event);
	void OnLogicalKeysSpin(wxSpinEvent& event);
	void OnFirstLogicalKeySpin(wxSpinEvent& event);
	void OnFirstMidiNoteSpin(wxSpinEvent& event);
	void OnAccessibleKeysSpin(wxSpinEvent& event);
	void OnMidiInputNbrSpin(wxSpinEvent& event);
	void OnNewStopBtn(wxCommandEvent& event);
	void OnNewCouplerBtn(wxCommandEvent& event);
	void OnNewDivisionalBtn(wxCommandEvent& event);
	void OnAvailableTremSelection(wxCommandEvent& event);
	void OnReferencedTremSelection(wxCommandEvent& event);
	void OnAddReferencedTremBtn(wxCommandEvent& event);
	void OnRemoveReferencedTremBtn(wxCommandEvent& event);
	void OnAvailableSwitchSelection(wxCommandEvent& event);
	void OnReferencedSwitchSelection(wxCommandEvent& event);
	void OnAddReferencedSwitchBtn(wxCommandEvent& event);
	void OnRemoveReferencedSwitchBtn(wxCommandEvent& event);
	void OnMidiKeyMapKeyChoice(wxCommandEvent& event);
	void OnMidiKeyMapValueSpin(wxSpinEvent& event);
	void OnRemoveManualBtn(wxCommandEvent& event);
	void DoRemoveManual();
	void UpdateReferencedTremulants();
	void UpdateReferencedSwitches();
	void UpdateAnyGuiVersion();

};

#endif
