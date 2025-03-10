/*
 * AttackDialog.h is part of GoOdf.
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
 * along with GoOdf. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#ifndef ATTACKDIALOG_H
#define ATTACKDIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/checkbox.h>
#include "GOODFDef.h"
#include "Attack.h"

class AttackDialog : public wxDialog {
	DECLARE_CLASS(AttackDialog)
	DECLARE_EVENT_TABLE()

public:
	std::list<Attack>& m_attacklist;
	unsigned m_selectedAttackIndex;
	// Constructors
	AttackDialog(std::list<Attack>& attack_list, unsigned selected_attack);
	AttackDialog(
		std::list<Attack>& attack_list,
		unsigned selected_attack,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("View/edit pipe properties"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~AttackDialog();

	// Initialize our variables
	void Init(unsigned selected_attack);

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Release properties"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

	// Accessors
	unsigned GetSelectedAttackIndex();
	bool GetCopyReplaceLoops();

private:
	unsigned m_firstSelectedAttack;
	unsigned m_maxSampleFrames;
	std::list<Attack>::iterator m_currentAttack;
	Loop *m_selectedLoop;
	wxArrayString tremChoices;
	wxArrayString loopChoices;

	wxButton *m_prevAttackBtn;
	wxStaticText *m_attackLabel;
	wxButton *m_nextAttackBtn;
	wxStaticText *m_attackName;
	wxStaticText *m_attackPath;
	wxRadioButton *m_loadReleaseYes;
	wxRadioButton *m_loadReleaseNo;
	wxSpinCtrl *m_attackVelocitySpin; // 0 - 127
	wxSpinCtrl *m_maxTimeSinceLastSpin; // -1 - 100000
	wxChoice *m_isTremulantChoice;
	wxSpinCtrl *m_maxKeyPressTime; // -1 - 100000
	wxSpinCtrl *m_attackStartSpin; // 0 - 158760000
	wxSpinCtrl *m_cuePointSpin; // -1 - 158760000
	wxSpinCtrl *m_releaseEndSpin; // -1 - 158760000
	wxSpinCtrl *m_loopCrossfadeSpin; // 0-3000
	wxSpinCtrl *m_releaseCrossfadeSpin; // 0-3000
	wxListBox *m_loopsList;
	wxButton *m_addNewLoopBtn;
	wxButton *m_deleteLoopBtn;
	wxSpinCtrl *m_loopStartSpin;
	wxSpinCtrl *m_loopEndSpin;
	wxButton *m_copyPropertiesBtn;
	wxCheckBox *m_copyReplaceLoopsCheck;

	// Event methods
	void OnPrevAttackBtn(wxCommandEvent& event);
	void OnNextAttackBtn(wxCommandEvent& event);
	void OnLoadReleaseSelection(wxCommandEvent& event);
	void OnAttackVelocitySpin(wxSpinEvent& event);
	void OnMaxTimeSinceLastSpin(wxSpinEvent& event);
	void OnTremulantChoice(wxCommandEvent& event);
	void OnMaxKeyPressTimeSpin(wxSpinEvent& event);
	void OnAttackStartSpin(wxSpinEvent& event);
	void OnCuePointSpin(wxSpinEvent& event);
	void OnReleaseEndSpin(wxSpinEvent& event);
	void OnLoopCrossfadeSpin(wxSpinEvent& event);
	void OnReleaseCrossfadeSpin(wxSpinEvent& event);
	void OnLoopListSelection(wxCommandEvent& event);
	void OnAddLoopBtn(wxCommandEvent& event);
	void OnRemoveLoopBtn(wxCommandEvent& event);
	void OnLoopStartSpin(wxSpinEvent& event);
	void OnLoopEndSpin(wxSpinEvent& event);
	void OnCopyReplaceLoopCheck(wxCommandEvent& event);

	std::list<Attack>::iterator GetAttackIterator(unsigned index);
	void SetButtonState();
	void TransferAttackValuesToWindow();
	void SetLoopStartAndEndRanges();
	void UpdateLoopChoices();
	void LoopInListSelected();
};

#endif
