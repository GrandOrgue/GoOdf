/*
 * StopRankImportDialog.h is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (See AUTHORS)
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

#ifndef STOPRANKIMPORTDIALOG_H
#define STOPRANKIMPORTDIALOG_H

#include <wx/wx.h>
#include "Organ.h"

class StopRankImportDialog : public wxDialog {
	DECLARE_CLASS(StopRankImportDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	StopRankImportDialog(Organ *source, Organ *target);
	StopRankImportDialog(
		Organ *source,
		Organ *target,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Import stop(s)/rank(s) from .organ file"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~StopRankImportDialog();

	// Initialize our variables
	void Init(Organ *source, Organ *target);

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Import stop(s)/rank(s) from .organ file"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

private:
	Organ *m_targetOrgan;
	Organ *m_sourceOrgan;
	wxArrayString m_targetManualList;
	wxArrayString m_importStopList;
	wxArrayString m_importRankList;
	wxArrayString m_availableWindchests;

	wxListBox *m_availableStopList;
	wxListBox *m_availableRankList;
	wxChoice *m_manualChoice;
	wxChoice *m_windchestChoice;
	wxButton *m_doImportBtn;
	wxTextCtrl *m_infoText;
	wxTextCtrl *m_logText;

	// Event methods
	void OnAvailableStopRankSelection(wxCommandEvent& event);
	void OnDoImportBtn(wxCommandEvent& event);
	void OnManualChoice(wxCommandEvent& event);

	bool CanImportButtonBeEnabled();
};

#endif
