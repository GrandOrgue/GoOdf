/*
 * DivisionalPanel.h is part of GoOdf.
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

#ifndef DIVISIONALPANEL_H
#define DIVISIONALPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Divisional.h"

class DivisionalPanel : public wxPanel {
public:
	DivisionalPanel(wxWindow *parent);
	~DivisionalPanel();

	void setDivisional(Divisional *divisional);
	void setIsFirstRemoval(bool value);
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxRadioButton *m_displayInvertedYes;
	wxRadioButton *m_displayInvertedNo;
	wxRadioButton *m_isProtectedYes;
	wxRadioButton *m_isProtectedNo;
	
	wxListBox *m_availableStops;
	wxListBox *m_referencedStops;
	wxButton *m_addReferencedStop;
	wxButton *m_addAllAvailableStops;
	wxButton *m_removeReferencedStop;
	wxRadioButton *m_currentStopIsOn;
	wxRadioButton *m_currentStopIsOff;
	
	wxListBox *m_availableCouplers;
	wxListBox *m_referencedCouplers;
	wxButton *m_addReferencedCoupler;
	wxButton *m_addAllAvailableCouplers;
	wxButton *m_removeReferencedCoupler;
	wxRadioButton *m_currentCouplerIsOn;
	wxRadioButton *m_currentCouplerIsOff;
	
	wxListBox *m_availableTremulants;
	wxListBox *m_referencedTremulants;
	wxButton *m_addReferencedTremulant;
	wxButton *m_addAllAvailableTremulants;
	wxButton *m_removeReferencedTremulant;
	wxRadioButton *m_currentTremIsOn;
	wxRadioButton *m_currentTremIsOff;
	
	wxListBox *m_availableSwitches;
	wxListBox *m_referencedSwitches;
	wxButton *m_addReferencedSwitch;
	wxButton *m_addAllAvailableSwitches;
	wxButton *m_removeReferencedSwitch;
	wxRadioButton *m_currentSwitchIsOn;
	wxRadioButton *m_currentSwitchIsOff;
	
	wxButton *removeDivisionalBtn;

	Divisional *m_divisional;
	bool m_isFirstRemoval;
	
	void OnNameChange(wxCommandEvent& event);
	void OnDisplayInvertedRadio(wxCommandEvent& event);
	void OnProtectedSelection(wxCommandEvent& event);
	void OnAddStopReferenceBtn(wxCommandEvent& event);
	void OnAddAllAvailableStopsBtn(wxCommandEvent& event);
	void OnRemoveStopReferenceBtn(wxCommandEvent& event);
	void OnStopListboxSelection(wxCommandEvent& event);
	void OnReferencedStopSelection(wxCommandEvent& event);
	void OnStopOnOffRadioBtns(wxCommandEvent& event);
	void UpdateReferencedStops();
	void OnAddCouplerReferenceBtn(wxCommandEvent& event);
	void OnAddAllAvailableCouplersBtn(wxCommandEvent& event);
	void OnRemoveCouplerReferenceBtn(wxCommandEvent& event);
	void OnCouplerListboxSelection(wxCommandEvent& event);
	void OnReferencedCouplerSelection(wxCommandEvent& event);
	void OnCouplerOnOffRadioBtns(wxCommandEvent& event);
	void UpdateReferencedCouplers();
	void OnAddTremulantReferenceBtn(wxCommandEvent& event);
	void OnAddAllAvailableTremulantsBtn(wxCommandEvent& event);
	void OnRemoveTremulantReferenceBtn(wxCommandEvent& event);
	void OnTremulantListboxSelection(wxCommandEvent& event);
	void OnReferencedTremulantSelection(wxCommandEvent& event);
	void OnTremOnOffRadioBtns(wxCommandEvent& event);
	void UpdateReferencedTremulants();
	void OnAddSwitchReferenceBtn(wxCommandEvent& event);
	void OnAddAllAvailableSwitchesBtn(wxCommandEvent& event);
	void OnRemoveSwitchReferenceBtn(wxCommandEvent& event);
	void OnSwitchListboxSelection(wxCommandEvent& event);
	void OnReferencedSwitchSelection(wxCommandEvent& event);
	void OnSwitchOnOffRadioBtns(wxCommandEvent& event);
	void UpdateReferencedSwitches();
	void OnRemoveDivisionalBtn(wxCommandEvent& event);

};

#endif
