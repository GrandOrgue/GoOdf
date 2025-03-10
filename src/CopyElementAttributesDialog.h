/*
 * CopyElementAttributesDialog.h is part of GoOdf.
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

#ifndef COPYELEMENTATTRIBUTESDIALOG_H
#define COPYELEMENTATTRIBUTESDIALOG_H

#include <wx/wx.h>
#include "GoPanel.h"
#include <vector>

class CopyElementAttributesDialog : public wxDialog {
	DECLARE_CLASS(CopyElementAttributesDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	CopyElementAttributesDialog(GoPanel *sourcePanel, int sourceElementIndex);
	CopyElementAttributesDialog(
		GoPanel *sourcePanel,
		int sourceElementIndex,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Copy GUI Element Attributes Dialog"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~CopyElementAttributesDialog();

	// Initialize our variables
	void Init(GoPanel *sourcePanel, int sourceElementIndex);

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Copy GUI Element Attributes Dialog"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

	// Accessors
	GoPanel* GetSelectedTargetPanel();
	bool GetSelectedElementIndices(wxArrayInt& selectedElements);

private:
	int m_sourceElementIndex;
	GoPanel *m_sourcePanel;
	wxChoice *m_panelChoice;
	wxListBox *m_availableElements;

	// m_panelList contain all available panels in the organ to choose as target container
	wxArrayString m_panelList;

	// the GUI Elements that match the source element type will be stored as string (display name)
	// and the actual index that element has on the panel in the vector
	wxArrayString m_matchingElementsList;
	std::vector<int> m_referenceElementIdx;

	// Event methods
	void OnPanelChoice(wxCommandEvent& event);
	void OnElementChoice(wxCommandEvent& event);

	void updateAvailableGuiElements();
};

#endif
