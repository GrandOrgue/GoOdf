/*
 * PipeBorrowingDialog.h is part of GoOdf.
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

#ifndef PIPEBORROWINGDIALOG_H
#define PIPEBORROWINGDIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>

class PipeBorrowingDialog : public wxDialog {
	DECLARE_CLASS(PipeBorrowingDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	PipeBorrowingDialog();
	PipeBorrowingDialog(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Pipe Borrowing Dialog"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~PipeBorrowingDialog();

	// Initialize our variables
	void Init();

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Pipe Borrowing Dialog"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

	// Accessors
	int GetSelectedManual();
	int GetSelectedStop();
	int GetSelectedPipe();
	int GetFollowingPipes();
	bool IsSelectionOk();
	void SetDefaultSelections(int manIdx, int stopIdx, int pipeNbr);

private:
	int m_selectedManualIndex;
	int m_selectedStopIndex;
	int m_selectedPipeNbr;
	int m_followingPipes;
	wxArrayString m_manualList;
	bool m_manIsOk;
	bool m_stopIsOk;

	wxChoice *m_manualChoice;
	wxChoice *m_stopChoice; // the first rank of this stop will be referenced
	wxSpinCtrl *m_pipeSpin;
	wxSpinCtrl *m_followingPipesToRefSpin;
	wxStaticText *m_infoText;

	// Event methods
	void OnManualChoice(wxCommandEvent& event);
	void OnStopChoice(wxCommandEvent& event);
	void OnPipeSpin(wxSpinEvent& event);
	void OnFollowingPipesSpin(wxSpinEvent& event);

	void UpdateStopChoice();
	void UpdatePipeSpin();
	void UpdateFollowingPipesSpin();
};

#endif
