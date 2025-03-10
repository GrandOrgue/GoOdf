/*
 * StopPanel.h is part of GoOdf.
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

#ifndef STOPPANEL_H
#define STOPPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Stop.h"
#include <wx/notebook.h>
#include "RankPanel.h"

class StopPanel : public wxPanel {
public:
	StopPanel(wxWindow *parent);
	~StopPanel();

	void setStop(Stop *stop);
	void internalRankLogicalPipesChanged(int value);
	void setIsFirstRemoval(bool value);
	Stop* getCurrentStop();
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();
	RankPanel* GetInternalRankPanel();

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
	wxRadioButton *m_storeInDivisionalYes;
	wxRadioButton *m_storeInDivisionalNo;
	wxRadioButton *m_storeInGeneralYes;
	wxRadioButton *m_storeInGeneralNo;
	wxSpinCtrl *m_firstAccessiblePipeLogicalKeyNumberSpin; // 1 - 128
	wxSpinCtrl *m_numberOfAccessiblePipesSpin; // 1 - 192
	wxSpinCtrl *m_firstAccessiblePipeLogicalPipeNumberSpin; // 1 - 192
	wxRadioButton *m_useInternalRankYes;
	wxRadioButton *m_useInternalRankNo;
	wxButton *removeStopBtn;
	wxListBox *m_availableRanks;
	wxListBox *m_referencedRanks;
	wxButton *m_addReferencedRank;
	wxButton *m_removeReferencedRank;
	wxSpinCtrl *m_firstPipeNumberSpin; // 1 - nbr pipes in rank, default 1
	wxSpinCtrl *m_pipeCountSpin; // 0 - remaining pipes in rank, default pipes in rank
	wxSpinCtrl *m_firstAccessibleKeyNumberSpin; // 1 - nbr accessible pipes in rank, default 1

	Stop *m_stop;
	wxArrayString functionChoices;
	wxArrayString gcStateChoices;
	// wxArrayString availableRanks;
	wxNotebook *m_notebook;
	wxPanel *m_stopPanel;
	RankPanel *m_internalRankPanel;
	wxPanel *m_referencedRanksPanel;
	bool m_isFirstRemoval;
	size_t m_lastUsedPage;

	void OnNameChange(wxCommandEvent& event);
	void OnDisplayInvertedRadio(wxCommandEvent& event);
	void OnFunctionChange(wxCommandEvent& event);
	void OnDefaultToEngagedChange(wxCommandEvent& event);
	void OnGcStateChange(wxCommandEvent& event);
	void OnStoreInDivisionalChange(wxCommandEvent& event);
	void OnStoreInGeneralChange(wxCommandEvent& event);
	void OnFirstAccPipeLogKeyNbrChange(wxSpinEvent& event);
	void OnNbrOfAccPipesChange(wxSpinEvent& event);
	void OnFirstAccPipeLogPipeNbrChange(wxSpinEvent& event);
	void OnUseInternalRankSelection(wxCommandEvent& event);
	void OnRemoveStopBtn(wxCommandEvent& event);
	void OnAddSwitchReferenceBtn(wxCommandEvent& event);
	void OnRemoveSwitchReferenceBtn(wxCommandEvent& event);
	void OnSwitchListboxSelection(wxCommandEvent& event);
	void OnReferencedSwitchSelection(wxCommandEvent& event);
	void UpdateReferencedSwitches();
	void OnNotebookPageChanged(wxBookCtrlEvent& event);
	void OnAvailableRanksSelection(wxCommandEvent& event);
	void OnAddRankReferenceBtn(wxCommandEvent& event);
	void OnRemoveRankReferenceBtn(wxCommandEvent& event);
	void OnReferencedRanksSelection(wxCommandEvent& event);
	void OnRankFirstPipeNumberSpin(wxSpinEvent& event);
	void OnRankPipeCountSpin(wxSpinEvent& event);
	void OnRankFirstKeyNbrSpin(wxSpinEvent& event);
	void UpdateReferencedRanks();

};

#endif
