/*
 * DivisionalCouplerPanel.h is part of GoOdf.
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

#ifndef DIVISIONAL_CPLR_PANEL_H
#define DIVISIONAL_CPLR_PANEL_H

#include <wx/wx.h>
#include "DivisionalCoupler.h"

class DivisionalCouplerPanel : public wxPanel {
public:
	DivisionalCouplerPanel(wxWindow *parent);
	~DivisionalCouplerPanel();

	void setDivisionalCoupler(DivisionalCoupler *divCplr);
	void setIsFirstRemoval(bool value);
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxRadioButton *m_displayInvertedYes;
	wxRadioButton *m_displayInvertedNo;
	wxChoice *m_functionChoice;
	wxRadioButton *m_defaultToEngagedYes;
	wxRadioButton *m_defaultToEngagedNo;
	wxListBox *m_availableSwitches;
	wxListBox *m_referencedSwitches;
	wxButton *m_addReferencedSwitch;
	wxButton *m_removeReferencedSwitch;
	wxChoice *m_gcStateChoice;
	wxRadioButton *m_storeInGeneralYes;
	wxRadioButton *m_storeInGeneralNo;
	wxRadioButton *m_isBiDirectionalYes;
	wxRadioButton *m_isBiDirectionalNo;
	wxListBox *m_availableManuals;
	wxListBox *m_referencedManuals;
	wxButton *m_addReferencedManual;
	wxButton *m_removeReferencedManual;
	wxButton *removeDivisionalCouplerBtn;

	DivisionalCoupler *m_divCplr;
	wxArrayString functionChoices;
	wxArrayString gcStateChoices;
	bool m_isFirstRemoval;

	void OnNameChange(wxCommandEvent& event);
	void OnDisplayInvertedRadio(wxCommandEvent& event);
	void OnFunctionChange(wxCommandEvent& event);
	void OnDefaultToEngagedChange(wxCommandEvent& event);
	void OnGcStateChange(wxCommandEvent& event);
	void OnStoreInGeneralChange(wxCommandEvent& event);
	void OnBidirectionalChange(wxCommandEvent& event);
	void OnAddReferencedManual(wxCommandEvent& event);
	void OnRemoveReferencedManual(wxCommandEvent& event);
	void OnRemoveDivisionalCouplerBtn(wxCommandEvent& event);
	void OnManualListboxSelection(wxCommandEvent& event);
	void UpdateReferencedManuals();
	void OnReferencedManualSelection(wxCommandEvent& event);
	void OnAddSwitchReferenceBtn(wxCommandEvent& event);
	void OnRemoveSwitchReferenceBtn(wxCommandEvent& event);
	void OnSwitchListboxSelection(wxCommandEvent& event);
	void OnReferencedSwitchSelection(wxCommandEvent& event);
	void UpdateReferencedSwitches();

};

#endif
