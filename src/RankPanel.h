/*
 * RankPanel.h is part of GoOdf.
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

#ifndef RANKPANEL_H
#define RANKPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Rank.h"
#include <wx/treectrl.h>
#include <wx/checkbox.h>

class RankPanel : public wxPanel {
public:
	RankPanel(wxWindow *parent);
	~RankPanel();

	void setRank(Rank *rank);
	void setNameFieldValue(wxString name);
	void disableNameFieldInput();
	void setIsFirstRemoval(bool value);
	Rank* getCurrentRank();
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();
	void SetPipeReadingOptions(wxString atkFolder, bool oneAttack, bool loadRelease, wxString releaseFolder, bool extractTime, wxString tremFolder, bool loadAsTremOff);
	wxString GetAttackFolderOption();
	bool GetOneAttackOption();
	bool GetLoadReleaseOption();
	wxString GetReleaseFolderOption();
	bool GetExtractTimeOption();
	wxString GetTremFolderOption();
	bool GetLoadPipesAsTremOffOption();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxSpinCtrl *m_firstMidiNoteNumberSpin; // 0 - 256
	wxSpinCtrl *m_numberOfLogicalPipesSpin; // 1 - 192
	wxSpinCtrl *m_harmonicNumberSpin; // 1 - 1024, default 8, calculates 64 / rank_size
	wxStaticText *m_calculatedLength;
	wxButton *setHarmonicNbrFromEmbeddedPitchBtn;
	wxSpinCtrlDouble *m_pitchCorrectionSpin; // -1800 - 1800
	wxChoice *m_windchestChoice;
	wxRadioButton *m_isPercussiveYes;
	wxRadioButton *m_isPercussiveNo;
	wxRadioButton *m_hasIndependentReleaseYes;
	wxRadioButton *m_hasIndependentReleaseNo;
	wxSpinCtrlDouble *m_minVelocityVolumeSpin; // 0 - 1000, 100 == no change
	wxSpinCtrlDouble *m_maxVelocityVolumeSpin; // 0 - 1000, 100 == no change
	wxRadioButton *m_acceptsRetuningYes;
	wxRadioButton *m_acceptsRetuningNo;
	wxSpinCtrlDouble *m_amplitudeLevelSpin;
	wxSpinCtrlDouble *m_gainSpin;
	wxSpinCtrlDouble *m_pitchTuningSpin;
	wxSpinCtrl *m_trackerDelaySpin;

	wxTreeCtrl *m_pipeTreeCtrl;
	wxButton *readPipesFromFolderBtn;
	wxTextCtrl *m_optionsAttackField;
	wxCheckBox *m_optionsOnlyOneAttack;
	wxCheckBox *m_optionsLoadReleaseInAttack;
	wxTextCtrl *m_optionsReleaseField;
	wxCheckBox *m_optionsKeyPressTime;
	wxTextCtrl *m_optionsTremulantField;
	wxButton *m_addPipesFromFolderBtn;
	wxButton *m_addTremulantPipesBtn;
	wxCheckBox *m_loadPipesAsTremOffCheck;
	wxButton *m_expandTreeBtn;
	wxButton *m_addReleaseSamplesBtn;
	wxButton *m_flexiblePipeLoadingBtn;

	wxButton *removeRankBtn;

	Rank *m_rank;
	wxArrayString availableWindchests;
	wxTreeItemId m_tree_rank_root;
	bool m_isFirstRemoval;
	int m_lastReferencedManual;
	int m_lastReferencedStop;
	wxString m_attackFolder;
	bool m_loadOnlyOneAttack;
	bool m_loadReleaseInAttack;
	wxString m_releaseFolder;
	bool m_extractTime;
	wxString m_tremFolder;
	bool m_loadPipesAsTremOff;

	void OnNameChange(wxCommandEvent& event);
	void OnWindchestChoice(wxCommandEvent& event);
	void OnMidiNoteSpin(wxSpinEvent& event);
	void OnLogicalPipeSpin(wxSpinEvent& event);
	void OnHarmonicNbrSpin(wxSpinEvent& event);
	void OnSetHarmonicNbrBtn(wxCommandEvent& event);
	void OnPitchCorrectionSpin(wxSpinDoubleEvent& event);
	void OnPercussiveSelection(wxCommandEvent& event);
	void OnIndependentReleaseSelection(wxCommandEvent& event);
	void OnMinVelocitySpin(wxSpinDoubleEvent& event);
	void OnMaxVelocitySpin(wxSpinDoubleEvent& event);
	void OnRetuningSelection(wxCommandEvent& event);
	void OnReadPipesBtn(wxCommandEvent& event);
	void OnRemoveRankBtn(wxCommandEvent& event);
	void DoRemoveRank();
	void OnClearPipesBtn(wxCommandEvent& event);
	void OnPipeTreeItemRightClick(wxTreeEvent &evt);
	void OnPopupMenuClick(wxCommandEvent &evt);
	void OnAmplitudeLevelSpin(wxSpinDoubleEvent& event);
	void OnGainSpin(wxSpinDoubleEvent& event);
	void OnPitchTuningSpin(wxSpinDoubleEvent& event);
	void OnTrackerDelaySpin(wxSpinEvent& event);
	void OnAddPipesBtn(wxCommandEvent& event);
	void OnAddTremulantPipesBtn(wxCommandEvent& event);
	void OnExpandTreeBtn(wxCommandEvent& event);
	void OnAddReleaseSamplesBtn(wxCommandEvent& event);
	void OnFlexiblePipeLoadingBtn(wxCommandEvent& event);
	void OnTreeKeyboardInput(wxTreeEvent& event);
	void OnAttackFolderText(wxCommandEvent& event);
	void OnOnlyOneAttachCheck(wxCommandEvent& event);
	void OnLoadReleaseCheck(wxCommandEvent& event);
	void OnReleaseFolderText(wxCommandEvent& event);
	void OnExtractTimeCheck(wxCommandEvent& event);
	void OnTremulantFolderText(wxCommandEvent& event);
	void OnLoadPipesAsTremOffCheck(wxCommandEvent& event);

	void UpdatePipeTree();
	void RebuildPipeTree();

	int GetSelectedItemIndexRelativeParent();
	int GetItemIndexRelativeParent(wxTreeItemId item);
	void OnAddNewAttack();
	void OnAddNewRelease();
	void OnClearPipe();
	void OnEditPipe();
	void OnCreateReference();
	void OnEditAttack();
	void OnViewAttackSample();
	void OnEditRelease();
	void OnViewReleaseSample();
	void OnRemoveSelectedAttack();
	void OnRemoveSelectedRelease();
	void OnCopyPipeOffset();
	void ViewSampleDetails(wxString fullPathToSample);
	wxTreeItemId GetPipeTreeItemAt(int index);
	wxTreeItemId GetPipeOfSelection();
	void OnPipeReadingOptionsChanged();

};

#endif
