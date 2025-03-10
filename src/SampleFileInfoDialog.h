/*
 * SampleFileInfoDialog.h is part of GoOdf.
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

#ifndef SAMPLEFILEINFODIALOG_H
#define SAMPLEFILEINFODIALOG_H

#include <wx/wx.h>
#include "WAVfileParser.h"

class SampleFileInfoDialog : public wxDialog {
	DECLARE_CLASS(SampleFileInfoDialog)

public:
	SampleFileInfoDialog(wxString sampleFile);
	SampleFileInfoDialog(
		wxString sampleFile,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Sample File Information"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~SampleFileInfoDialog();

	// Initialize our variables
	void Init(wxString sampleFile);

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Sample File Information"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

private:
	wxString m_relativeFilePath;
	WAVfileParser *m_sampleFile;
};

#endif
