/*
 * ReversiblePistonPanel.h is part of GoOdf.
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

#ifndef REVERSIBLEPISTONPANEL_H
#define REVERSIBLEPISTONPANEL_H

#include <wx/wx.h>
#include "ReversiblePiston.h"

class ReversiblePistonPanel : public wxPanel {
public:
	ReversiblePistonPanel(wxWindow *parent);
	~ReversiblePistonPanel();

	void setReversiblePiston(ReversiblePiston *piston);
	void setIsFirstRemoval(bool value);
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxRadioButton *m_displayInvertedYes;
	wxRadioButton *m_displayInvertedNo;
	wxListBox *m_availableStops;
	wxListBox *m_availableCouplers;
	wxListBox *m_availableSwitches;
	wxListBox *m_availableTremulants;
	wxButton *useSelectedBtn;
	wxTextCtrl *m_objecTypeField;
	wxTextCtrl *m_manualField;
	wxButton *removePistonBtn;

	ReversiblePiston *m_piston;
	bool m_isFirstRemoval;

	void OnNameChange(wxCommandEvent& event);
	void OnDisplayInvertedRadio(wxCommandEvent& event);
	void OnStopListboxSelection(wxCommandEvent& event);
	void OnCouplerListboxSelection(wxCommandEvent& event);
	void OnSwitchListboxSelection(wxCommandEvent& event);
	void OnTremulantListboxSelection(wxCommandEvent& event);
	void OnUseSelectedBtn(wxCommandEvent& event);
	void OnRemovePistonBtn(wxCommandEvent& event);
	void UpdateInfoFields();

};

#endif

