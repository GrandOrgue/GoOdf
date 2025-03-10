/*
 * TremulantPanel.h is part of GoOdf.
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

#ifndef TREMULANTPANEL_H
#define TREMULANTPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Tremulant.h"

class TremulantPanel : public wxPanel {
public:
	TremulantPanel(wxWindow *parent);
	~TremulantPanel();

	void setTremulant(Tremulant *tremulant);
	void setIsFirstRemoval(bool value);
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxChoice *m_functionChoice;
	wxRadioButton *m_displayInvertedYes;
	wxRadioButton *m_displayInvertedNo;
	wxRadioButton *m_defaultToEngagedYes;
	wxRadioButton *m_defaultToEngagedNo;
	wxListBox *m_availableSwitches;
	wxListBox *m_referencedSwitches;
	wxButton *m_addReferencedSwitch;
	wxButton *m_removeReferencedSwitch;
	wxChoice *m_gcStateChoice;
	wxRadioButton *m_storeInDivisionalYes;
	wxRadioButton *m_storeInDivisionalNo;
	wxRadioButton *m_storeInGeneralYes;
	wxRadioButton *m_storeInGeneralNo;
	wxChoice *m_tremTypeChoice;
	wxSpinCtrl *m_periodSpin;
	wxSpinCtrl *m_startRateSpin;
	wxSpinCtrl *m_stopRateSpin;
	wxSpinCtrl *m_ampModDepthSpin;
	wxButton *removeTremulantBtn;

	Tremulant *m_tremulant;
	wxArrayString functionChoices;
	wxArrayString gcStateChoices;
	wxArrayString tremTypeChoices;
	bool m_isFirstRemoval;

	void OnNameChange(wxCommandEvent& event);
	void OnDisplayInvertedRadio(wxCommandEvent& event);
	void OnFunctionChange(wxCommandEvent& event);
	void OnDefaultToEngagedChange(wxCommandEvent& event);
	void OnGcStateChange(wxCommandEvent& event);
	void OnStoreInDivisionalChange(wxCommandEvent& event);
	void OnStoreInGeneralChange(wxCommandEvent& event);
	void OnTremTypeChange(wxCommandEvent& event);
	void OnPeriodChange(wxSpinEvent& event);
	void OnStartRateChange(wxSpinEvent& event);
	void OnStopRateChange(wxSpinEvent& event);
	void OnAmpModDepthChange(wxSpinEvent& event);
	void OnRemoveTremulantBtn(wxCommandEvent& event);
	void DoRemoveTremulant();
	void OnAddSwitchReferenceBtn(wxCommandEvent& event);
	void OnRemoveSwitchReferenceBtn(wxCommandEvent& event);
	void OnSwitchListboxSelection(wxCommandEvent& event);
	void OnReferencedSwitchSelection(wxCommandEvent& event);
	void UpdateReferencedSwitches();

};

#endif
