/*
 * DoubleEntryDialog.cpp is part of GoOdf.
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

#include "DoubleEntryDialog.h"
#include "GOODFDef.h"
#include <wx/statline.h>

IMPLEMENT_CLASS(DoubleEntryDialog, wxDialog)

// Event table
BEGIN_EVENT_TABLE(DoubleEntryDialog, wxDialog)
	EVT_SPINCTRLDOUBLE(ID_DOUBLE_ENTRY_DIALOG_SPIN, DoubleEntryDialog::OnDoubleValueSpin)
END_EVENT_TABLE()

DoubleEntryDialog::DoubleEntryDialog(wxString message, wxString prompt, double value, double min, double max) {
	Init(message, prompt, value, min, max);
}

DoubleEntryDialog::DoubleEntryDialog(
	wxWindow* parent,
	wxString message,
	wxString prompt,
	double value,
	double min,
	double max,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos
) {
	Init(message, prompt, value, min, max);
	Create(parent, id, caption, pos);
}

DoubleEntryDialog::~DoubleEntryDialog() {

}

void DoubleEntryDialog::Init(wxString message, wxString prompt, double value, double min, double max) {
	m_messageString = message;
	m_promptString = prompt;
	if (min < max) {
		m_minValue = min;
		m_maxValue = max;
	} else {
		m_minValue = max;
		m_maxValue = min;
	}
	if (m_minValue <= value && m_maxValue >= value) {
		m_doubleValue = value;
	} else {
		m_doubleValue = (m_minValue + m_maxValue) / 2.0f;
	}
}

bool DoubleEntryDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos
) {
	if (!wxDialog::Create(parent, id, caption, pos, wxDefaultSize, wxCAPTION|wxSYSTEM_MENU|wxCLOSE_BOX))
		return false;

	CreateControls();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);

	return true;
}

void DoubleEntryDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *innerSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *messageText = new wxStaticText (
		this,
		wxID_STATIC,
		m_messageString
	);
	firstRow->Add(messageText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	innerSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *promptText = new wxStaticText (
		this,
		wxID_STATIC,
		m_promptString
	);
	secondRow->Add(promptText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_doubleSpin = new wxSpinCtrlDouble(
		this,
		ID_DOUBLE_ENTRY_DIALOG_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		m_minValue,
		m_maxValue,
		m_doubleValue,
		0.1
	);
	m_doubleSpin->SetDigits(1);
	m_doubleSpin->SetIncrement(1.0f);
	secondRow->Add(m_doubleSpin, 0, wxEXPAND|wxALL, 5);
	innerSizer->Add(secondRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	innerSizer->Add(bottomDivider, 0, wxEXPAND|wxALL, 5);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	wxButton *theCancelButton = new wxButton(
		this,
		wxID_CANCEL,
		wxT("Cancel")
	);
	bottomRow->Add(theCancelButton, 0, wxALIGN_CENTER|wxALL, 5);
	bottomRow->AddStretchSpacer();
	wxButton *theOkButton = new wxButton(
		this,
		wxID_OK,
		wxT("OK")
	);
	bottomRow->Add(theOkButton, 0, wxALIGN_CENTER|wxALL, 5);
	bottomRow->AddStretchSpacer();
	innerSizer->Add(bottomRow, 0, wxGROW);

	mainSizer->Add(innerSizer, 0, wxGROW|wxALL, 5);

	SetSizer(mainSizer);
}

double DoubleEntryDialog::GetValue() {
	return m_doubleValue;
}

bool DoubleEntryDialog::TransferDataFromWindow() {
	double theValue = m_doubleSpin->GetValue();
	if (theValue >= m_minValue && theValue <= m_maxValue)
		m_doubleValue = m_doubleSpin->GetValue();
	else
		m_doubleSpin->SetValue(m_doubleValue);

	return true;
}

void DoubleEntryDialog::OnDoubleValueSpin(wxSpinDoubleEvent& event) {
	double theValue = event.GetValue();
	if (theValue >= m_minValue && theValue <= m_maxValue)
		m_doubleValue = m_doubleSpin->GetValue();
	else
		m_doubleSpin->SetValue(m_doubleValue);
}
