/*
 * DoubleEntryDialog.h is part of GoOdf.
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

#ifndef DOUBLEENTRYDIALOG_H
#define DOUBLEENTRYDIALOG_H

#include <wx/wx.h>
#include <wx/spinctrl.h>

class DoubleEntryDialog : public wxDialog {
	DECLARE_CLASS(DoubleEntryDialog)
	DECLARE_EVENT_TABLE()

public:
	// Constructors
	DoubleEntryDialog(wxString message, wxString prompt, double value, double min, double max);
	DoubleEntryDialog(
		wxWindow* parent,
		wxString message,
		wxString prompt,
		double value,
		double min,
		double max,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Double Number Entry Dialog"),
		const wxPoint& pos = wxDefaultPosition
	);

	~DoubleEntryDialog();

	// Initialize our variables
	void Init(wxString message, wxString prompt, double value, double min, double max);

	// Creation
	bool Create(
		wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxString& caption = wxT("Double Number Entry Dialog"),
		const wxPoint& pos = wxDefaultPosition
	);

	// Creates the controls and sizers
	void CreateControls();

	// Accessors
	double GetValue();
	bool TransferDataFromWindow();

private:
	wxString m_messageString;
	wxString m_promptString;
	double m_doubleValue;
	double m_minValue;
	double m_maxValue;

	wxSpinCtrlDouble *m_doubleSpin;

	// Event methods
	void OnDoubleValueSpin(wxSpinDoubleEvent& event);

};

#endif
