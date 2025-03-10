/*
 * PipeDialog.h is part of GoOdf.
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

#ifndef PIPEDIALOG_H
#define PIPEDIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "GOODFDef.h"
#include "Pipe.h"

class PipeDialog : public wxDialog {
	DECLARE_CLASS(PipeDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	PipeDialog(std::list<Pipe>& pipe_list, unsigned selected_pipe);
	PipeDialog(
		std::list<Pipe>& pipe_list,
		unsigned selected_pipe,
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("View/edit pipe properties"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	~PipeDialog();

	// Initialize our variables
	void Init(unsigned selected_pipe);

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Pipe properties"),
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
	);

	// Creates the controls and sizers
	void CreateControls();

	// Accessor
	unsigned GetSelectedPipeIndex();

private:
	std::list<Pipe>& m_rank_pipelist;
	unsigned m_firstSelectedPipe;
	unsigned m_selectedPipeIndex;
	Pipe *m_currentPipe;
	wxArrayString windchestChoices;

	wxButton *m_prevPipeBtn;
	wxStaticText *m_pipeLabel;
	wxButton *m_nextPipeBtn;
	wxRadioButton *m_isPercussiveYes;
	wxRadioButton *m_isPercussiveNo;
	wxRadioButton *m_hasIndependentReleaseYes;
	wxRadioButton *m_hasIndependentReleaseNo;
	wxSpinCtrl *m_harmonicNbrSpin;
	wxStaticText *m_calculatedLength;
	wxSpinCtrl *m_midiKeyNbrSpin; // -1 - 127
	wxSpinCtrlDouble *m_midiPitchFractionSpin; // -1 as default (use value from sample), 0 - 100 as specified value in cents
	wxSpinCtrlDouble *m_pitchCorrectionSpin;
	wxRadioButton *m_acceptsRetuningYes;
	wxRadioButton *m_acceptsRetuningNo;
	wxChoice *m_windchestChoice;
	wxSpinCtrlDouble *m_minVelocitySpin;
	wxSpinCtrlDouble *m_maxVelocitySpin;
	wxButton *m_copyPropertiesBtn;
	wxSpinCtrl *m_copyToNbrPipesSpin;
	wxStaticText *m_copyInfo;
	wxButton *m_copyAtkRelBtn;
	wxSpinCtrlDouble *m_amplitudeLevelSpin;
	wxSpinCtrlDouble *m_gainSpin;
	wxSpinCtrlDouble *m_pitchTuningSpin;
	wxSpinCtrl *m_trackerDelaySpin;

	// Event methods
	void OnPrevPipeBtn(wxCommandEvent& event);
	void OnNextPipeBtn(wxCommandEvent& event);
	void OnPercussiveSelection(wxCommandEvent& event);
	void OnIndependentReleaseSelection(wxCommandEvent& event);
	void OnHarmonicNbrSpin(wxSpinEvent& event);
	void OnMidiNoteSpin(wxSpinEvent& event);
	void OnMidiPitchFractionSpin(wxSpinDoubleEvent& event);
	void OnPitchCorrectionSpin(wxSpinDoubleEvent& event);
	void OnAcceptsRetuningSelection(wxCommandEvent& event);
	void OnWindchestChoice(wxCommandEvent& event);
	void OnMinVelocitySpin(wxSpinDoubleEvent& event);
	void OnMaxVelocitySpin(wxSpinDoubleEvent& event);
	void OnCopyPipesSpin(wxSpinEvent& event);
	void OnCopyPropertiesBtn(wxCommandEvent& event);
	void OnCopyAtkRelBtn(wxCommandEvent& event);
	void OnAmplitudeLevelSpin(wxSpinDoubleEvent& event);
	void OnGainSpin(wxSpinDoubleEvent& event);
	void OnPitchTuningSpin(wxSpinDoubleEvent& event);
	void OnTrackerDelaySpin(wxSpinEvent& event);

	Pipe* GetPipePointer(unsigned index);
	void SetButtonState();
	void TransferPipeValuesToWindow();
	void SetCopyState();
};

#endif
