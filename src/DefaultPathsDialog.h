/*
 * DefaultPathsDialog.h is part of GoOdf.
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

#ifndef DEFAULTPATHSDIALOG_H
#define DEFAULTPATHSDIALOG_H

#include <wx/wx.h>

class DefaultPathsDialog : public wxDialog {
	DECLARE_CLASS(DefaultPathsDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	DefaultPathsDialog(wxString defaultOrganDir, wxString defaultCmbDir);
	DefaultPathsDialog(
		wxString defaultOrganDir,
		wxString defaultCmbDir,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Default Paths"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~DefaultPathsDialog();

	// Initialize our variables
	void Init(wxString defaultOrganDir, wxString defaultCmbDir);

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Default Paths"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

	// Accessors
	wxString GetSelectedOrganDirectory();
	wxString GetSelectedCmbDirectory();

private:
	wxString m_organDirectory;
	wxString m_cmbDirectory;

	wxTextCtrl *m_organPathField;
	wxButton *m_browseForOrganDirectoryBtn;
	wxTextCtrl *m_cmbPathField;
	wxButton *m_browseForCmbDirectoryBtn;

	// Event methods
	void OnBrowseForOrganDirectoryBtn(wxCommandEvent& event);
	void OnBrowseForCmbDirectoryBtn(wxCommandEvent& event);

	wxString GetPathToDirectory(wxString defaultPath);
};

#endif
