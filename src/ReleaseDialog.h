/*
 * ReleaseDialog.h is part of GoOdf.
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

#ifndef RELEASEDIALOG_H
#define RELEASEDIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "GOODFDef.h"
#include "Release.h"
#include <list>

class ReleaseDialog : public wxDialog {
	DECLARE_CLASS(ReleaseDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	ReleaseDialog(std::list<Release>& release_list, unsigned selected_release);
	ReleaseDialog(
		std::list<Release>& release_list,
		unsigned selected_release,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("View/edit pipe properties"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~ReleaseDialog();

	// Initialize our variables
	void Init(unsigned selected_release);

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

	// Accessor
	unsigned GetSelectedReleaseIndex();
	Release* GetCurrentRelease();

private:
	std::list<Release>& m_releaselist;
	unsigned m_firstSelectedRelease;
	unsigned m_selectedReleaseIndex;
	Release *m_currentRelease;
	wxArrayString tremChoices;

	wxButton *m_prevReleaseBtn;
	wxStaticText *m_releaseLabel;
	wxButton *m_nextReleaseBtn;
	wxStaticText *m_releaseName;
	wxStaticText *m_releasePath;
	wxChoice *m_isTremulantChoice;
	wxSpinCtrl *m_maxKeyPressTime; // -1 - 100000
	wxSpinCtrl *m_cuePointSpin; // -1 - 158760000
	wxSpinCtrl *m_releaseEndSpin; // -1 - 158760000
	wxSpinCtrl *m_releaseCrossfadeSpin; // 0-3000
	wxButton *m_copyPropertiesBtn;

	// Event methods
	void OnPrevReleaseBtn(wxCommandEvent& event);
	void OnNextReleaseBtn(wxCommandEvent& event);
	void OnTremulantChoice(wxCommandEvent& event);
	void OnMaxKeyPressTimeSpin(wxSpinEvent& event);
	void OnCuePointSpin(wxSpinEvent& event);
	void OnReleaseEndSpin(wxSpinEvent& event);
	void OnReleaseCrossfadeSpin(wxSpinEvent& event);

	Release* GetReleasePointer(unsigned index);
	void SetButtonState();
	void TransferReleaseValuesToWindow();
};

#endif

