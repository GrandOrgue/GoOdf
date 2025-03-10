/*
 * DefaultPathsDialog.cpp is part of GoOdf.
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
#include "DefaultPathsDialog.h"
#include "GOODFDef.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>

IMPLEMENT_CLASS(DefaultPathsDialog, wxDialog)

// Event table
BEGIN_EVENT_TABLE(DefaultPathsDialog, wxDialog)
	EVT_BUTTON(ID_DEFAULT_ORGAN_DIRECTORY_BTN, DefaultPathsDialog::OnBrowseForOrganDirectoryBtn)
	EVT_BUTTON(ID_DEFAULT_CMB_DIRECTORY_BTN, DefaultPathsDialog::OnBrowseForCmbDirectoryBtn)
END_EVENT_TABLE()

DefaultPathsDialog::DefaultPathsDialog(wxString defaultOrganDir, wxString defaultCmbDir) {
	Init(defaultOrganDir, defaultCmbDir);
}

DefaultPathsDialog::DefaultPathsDialog(
	wxString defaultOrganDir,
	wxString defaultCmbDir,
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) {
	Init(defaultOrganDir, defaultCmbDir);
	Create(parent, id, caption, pos, size, style);
}

DefaultPathsDialog::~DefaultPathsDialog() {

}

void DefaultPathsDialog::Init(wxString defaultOrganDir, wxString defaultCmbDir) {
	m_organDirectory = defaultOrganDir;
	m_cmbDirectory = defaultCmbDir;
}

bool DefaultPathsDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) {
	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	CreateControls();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	wxSize calculated = GetSize();
	if (calculated.GetWidth() < 640)
		SetMinSize(wxSize(640, calculated.GetHeight()));
	Centre();

	return true;
}

void DefaultPathsDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *organPathText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Default organ directory: ")
	);
	firstRow->Add(organPathText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_organPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		m_organDirectory,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	firstRow->Add(m_organPathField, 1, wxEXPAND|wxALL, 5);
	m_browseForOrganDirectoryBtn = new wxButton(
		this,
		ID_DEFAULT_ORGAN_DIRECTORY_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	firstRow->Add(m_browseForOrganDirectoryBtn, 0, wxALL, 5);
	mainSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *cmbPathText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Default cmb directory: ")
	);
	secondRow->Add(cmbPathText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_cmbPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		m_cmbDirectory,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	secondRow->Add(m_cmbPathField, 1, wxEXPAND|wxALL, 5);
	m_browseForCmbDirectoryBtn = new wxButton(
		this,
		ID_DEFAULT_CMB_DIRECTORY_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	secondRow->Add(m_browseForCmbDirectoryBtn, 0, wxALL, 5);
	mainSizer->Add(secondRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	mainSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	wxButton *theCancelButton = new wxButton(
		this,
		wxID_CANCEL,
		wxT("Cancel")
	);
	bottomRow->Add(theCancelButton, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	wxButton *theOkButton = new wxButton(
		this,
		wxID_OK,
		wxT("Apply Default Paths")
	);
	bottomRow->Add(theOkButton, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	mainSizer->Add(bottomRow, 0, wxGROW);

	SetSizer(mainSizer);
}

wxString DefaultPathsDialog::GetSelectedOrganDirectory() {
	return m_organDirectory;
}

wxString DefaultPathsDialog::GetSelectedCmbDirectory() {
	return m_cmbDirectory;
}

void DefaultPathsDialog::OnBrowseForOrganDirectoryBtn(wxCommandEvent& WXUNUSED(event)) {
	m_organDirectory = GetPathToDirectory(m_organDirectory);
	m_organPathField->ChangeValue(m_organDirectory);
}

void DefaultPathsDialog::OnBrowseForCmbDirectoryBtn(wxCommandEvent& WXUNUSED(event)) {
	m_cmbDirectory = GetPathToDirectory(m_cmbDirectory);
	m_cmbPathField->ChangeValue(m_cmbDirectory);
}

wxString DefaultPathsDialog::GetPathToDirectory(wxString defaultPath) {
	wxString pathToReturn;
	wxString pathSuggestion;
	if (defaultPath != wxEmptyString)
		pathSuggestion = defaultPath;
	else
		defaultPath = wxStandardPaths::Get().GetDocumentsDir();

	wxDirDialog dirDialog(
		this,
		wxT("Pick a directory (Cancel will empty the field!)"),
		defaultPath,
		wxDD_DIR_MUST_EXIST
	);

	if (dirDialog.ShowModal() == wxID_OK) {
		pathToReturn = dirDialog.GetPath();
		return pathToReturn;
	} else {
		return wxEmptyString;
	}
}
