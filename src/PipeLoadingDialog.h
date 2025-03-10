/*
 * PipeLoadingDialog.h is part of GoOdf.
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

#ifndef PIPELOADINGDIALOG_H
#define PIPELOADINGDIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>

class PipeLoadingDialog : public wxDialog {
	DECLARE_CLASS(PipeLoadingDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	PipeLoadingDialog(
		int nbrOfPipes,
		unsigned startPipe = 1,
		unsigned firstMidiNbr = 36,
		wxString baseDirectory = wxEmptyString,
		wxString attackSubfolder = wxEmptyString,
		bool onlyOneAttack = false,
		bool loadReleaseInAttack = true,
		wxString releaseFolderPrefix = wxT("rel"),
		bool extractKeyPressTime = true,
		wxString tremulantFolderPrefix = wxT("trem"),
		bool loadPipesTremOff = false
	);
	PipeLoadingDialog(
		wxWindow* parent,
		int nbrOfPipes,
		unsigned startPipe = 1,
		unsigned firstMidiNbr = 36,
		wxString baseDirectory = wxEmptyString,
		wxString attackSubfolder = wxEmptyString,
		bool onlyOneAttack = false,
		bool loadReleaseInAttack = true,
		wxString releaseFolderPrefix = wxT("rel"),
		bool extractKeyPressTime = true,
		wxString tremulantFolderPrefix = wxT("trem"),
		bool loadPipesTremOff = false,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Flexible Pipe Loading Options"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~PipeLoadingDialog();

	// Initialize our variables
	void Init(
		int nbrOfPipes,
		unsigned startPipe,
		unsigned firstMidiNbr,
		wxString baseDirectory,
		wxString attackSubfolder,
		bool onlyOneAttack,
		bool loadReleaseInAttack,
		wxString releaseFolderPrefix,
		bool extractKeyPressTime ,
		wxString tremulantFolderPrefix,
		bool loadPipesTremOff
	);

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Flexible Pipe Loading Options"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

	// Accessors
	unsigned GetStartingPipeNumber();
	unsigned GetNbrPipesToLoad();
	unsigned GetFirstMidiMatchingNbr();
	wxString GetBaseDirectory();
	wxString GetSubDirectory();
	bool GetLoadOnlyOneAttack();
	bool GetLoadReleaseInAttack();
	wxString GetReleaseFolderPrefix();
	wxString GetTremFolderPrefix();
	bool GetExtractKeyPressTime();
	int GetSelectedStrategy();
	bool GetLoadPipesTremOff();

private:
	int m_nbrPipesInRank;
	unsigned m_startingPipe;
	unsigned m_pipesToLoad;
	unsigned m_firstMidiNbr;
	wxString m_baseDirectory;
	wxString m_attackSubfolder;
	bool m_onlyOneAttack;
	bool m_loadReleaseInAttack;
	wxString m_releaseFolderPrefix;
	bool m_extractKeyPressTime;
	wxString m_tremulantFolderPrefix;
	wxArrayString m_pipeLoadingStrategies;
	int m_loadingStrategy;
	bool m_loadPipesTremOff;

	wxTextCtrl *m_basePathField;
	wxButton *m_browseForPathBtn;
	wxSpinCtrl *m_startPipeSpin;
	wxSpinCtrl *m_pipesToLoadSpin;
	wxSpinCtrl *m_firstMidiNbrSpin;
	wxTextCtrl *m_subfolderField;
	wxTextCtrl *m_releasePrefixField;
	wxTextCtrl *m_tremulantPrefixField;
	wxCheckBox *m_addOnlyOneAttack;
	wxCheckBox *m_addReleaseInAttack;
	wxCheckBox *m_tryForKeyPressTime;
	wxCheckBox *m_loadPipesTremulantOffCheck;
	wxChoice *m_loadingStrategyChoice;

	// Event methods
	void OnBrowseForPathBtn(wxCommandEvent& event);
	void OnFirstPipeSpin(wxSpinEvent& event);
	void OnNumberOfPipesSpin(wxSpinEvent& event);
	void OnFirstMidiNumberSpin(wxSpinEvent& event);
	void OnAttackSubDirectoryText(wxCommandEvent& event);
	void OnOneAttackCheck(wxCommandEvent& event);
	void OnReleaseInAttackCheck(wxCommandEvent& event);
	void OnReleaseSubDirectoryText(wxCommandEvent& event);
	void OnExtractKeyPressTimeCheck(wxCommandEvent& event);
	void OnTremSubDirectoryText(wxCommandEvent& event);
	void OnLoadingStrategyChoice(wxCommandEvent& event);
	void OnLoadPipesTremOffCheck(wxCommandEvent& event);

	void DecideStateOfOkButton();
};

#endif
