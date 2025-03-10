/*
 * PipeCopyOffsetDialog.h is part of GoOdf.
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

#ifndef PIPECOPYOFFSETDIALOG_H
#define PIPECOPYOFFSETDIALOG_H

#include <wx/wx.h>

class PipeCopyOffsetDialog : public wxDialog {
	DECLARE_CLASS(PipeCopyOffsetDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	PipeCopyOffsetDialog(int nbrOfPipes, int sourcePipeIndex);
	PipeCopyOffsetDialog(
		int nbrOfPipes,
		int sourcePipeIndex,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Copy pipe with offset pitch"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~PipeCopyOffsetDialog();

	// Initialize our variables
	void Init(int nbrOfPipes, int sourcePipeIndex);

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Copy pipe with offset pitch"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

	// Accessors
	bool GetSelectedIndices(wxArrayInt& selectedPipes);

private:
	int m_nbrOfPipes;
	int m_sourcePipeIndex;
	wxListBox *m_pipeListbox;

	wxArrayString m_pipeList;

	// Event methods
	void OnPipeSelection(wxCommandEvent& event);

};

#endif
