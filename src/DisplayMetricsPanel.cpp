/*
 * DisplayMetricsPanel.cpp is part of GOODF.
 * Copyright (C) 2023 Lars Palo and contributors (see AUTHORS)
 *
 * GOODF is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GOODF is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GOODF.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#include "DisplayMetricsPanel.h"
#include "GOODF.h"
#include "GOODFDef.h"
#include <wx/msgdlg.h>

// Event table
BEGIN_EVENT_TABLE(DisplayMetricsPanel, wxPanel)
	EVT_CHOICE(ID_HORIZONTAL_SIZE_CHOICE, DisplayMetricsPanel::OnHorizontalSizeChoice)
	EVT_SPINCTRL(ID_HORIZONTAL_SIZE_SPIN, DisplayMetricsPanel::OnHorizontalSizeChange)
	EVT_CHOICE(ID_VERTICAL_SIZE_CHOICE, DisplayMetricsPanel::OnVerticalSizeChoice)
	EVT_SPINCTRL(ID_VERTICAL_SIZE_SPIN, DisplayMetricsPanel::OnVerticalSizeChange)
	EVT_COMBOBOX(ID_DRAWSTOP_BACKGROUND, DisplayMetricsPanel::OnDrawstopBackgroundChange)
	EVT_COMBOBOX(ID_CONSOLE_BACKGROUND, DisplayMetricsPanel::OnConsoleBackgroundChange)
	EVT_COMBOBOX(ID_KEYBOARD_HORIZONTAL, DisplayMetricsPanel::OnKeyHorizontalChange)
	EVT_COMBOBOX(ID_KEYBOARD_VERTICAL, DisplayMetricsPanel::OnKeyVerticalChange)
	EVT_COMBOBOX(ID_DRAWSTOP_INSET, DisplayMetricsPanel::OnDrawstopInsetBgChange)
	EVT_FONTPICKER_CHANGED(ID_CTRL_LABEL_FONT, DisplayMetricsPanel::OnControlLabelFontChange)
	EVT_FONTPICKER_CHANGED(ID_GROUP_LABEL_FONT, DisplayMetricsPanel::OnGroupLabelFontChange)
	EVT_FONTPICKER_CHANGED(ID_SHORTCUT_KEY_LABEL_FONT, DisplayMetricsPanel::OnShortcutKeyLabelFontChange)
	EVT_CHOICE(ID_SHORTCUT_COLOUR_CHOICE, DisplayMetricsPanel::OnShortcutKeyLabelColourChoice)
	EVT_COLOURPICKER_CHANGED(ID_SHORTCUT_COLOUR_PICKER, DisplayMetricsPanel::OnShortcutKeyLabelColourPick)
	EVT_SPINCTRL(ID_DRAWSTOP_COL_SPIN, DisplayMetricsPanel::OnDrawstopColSpin)
	EVT_RADIOBUTTON(ID_PAIR_DRAWSTOP_COLS_YES, DisplayMetricsPanel::OnPairDrawstopColsRadio)
	EVT_RADIOBUTTON(ID_PAIR_DRAWSTOP_COLS_NO, DisplayMetricsPanel::OnPairDrawstopColsRadio)
	EVT_SPINCTRL(ID_DRAWSTOP_ROW_SPIN, DisplayMetricsPanel::OnDrawstopRowSpin)
	EVT_RADIOBUTTON(ID_DRAWSTOP_COLS_OFFSET_YES, DisplayMetricsPanel::OnDrawstopColsOffsetRadio)
	EVT_RADIOBUTTON(ID_DRAWSTOP_COLS_OFFSET_NO, DisplayMetricsPanel::OnDrawstopColsOffsetRadio)
	EVT_RADIOBUTTON(ID_OUTER_COL_UP_YES, DisplayMetricsPanel::OnDrawstopOuterColOffsetUpRadio)
	EVT_RADIOBUTTON(ID_OUTER_COL_UP_NO, DisplayMetricsPanel::OnDrawstopOuterColOffsetUpRadio)
	EVT_SPINCTRL(ID_EXTRA_DRAWSTOP_COL_SPIN, DisplayMetricsPanel::OnExtraDrawstopColSpin)
	EVT_SPINCTRL(ID_EXTRA_DRAWSTOP_ROW_SPIN, DisplayMetricsPanel::OnExtraDrawstopRowSpin)
	EVT_SPINCTRL(ID_BUTTON_COLS_SPIN, DisplayMetricsPanel::OnButtonColSpin)
	EVT_SPINCTRL(ID_EXTRA_BUTTON_ROW_SPIN, DisplayMetricsPanel::OnExtraButtonRowSpin)
	EVT_RADIOBUTTON(ID_EXTRA_PED_BTN_ROW_YES, DisplayMetricsPanel::OnExtraPedalButtonRowRadio)
	EVT_RADIOBUTTON(ID_EXTRA_PED_BTN_ROW_NO, DisplayMetricsPanel::OnExtraPedalButtonRowRadio)
	EVT_RADIOBUTTON(ID_EXTRA_PED_ROW_OFFSET_YES, DisplayMetricsPanel::OnExtraPedalButtonRowOffsetRadio)
	EVT_RADIOBUTTON(ID_EXTRA_PED_ROW_OFFSET_NO, DisplayMetricsPanel::OnExtraPedalButtonRowOffsetRadio)
	EVT_RADIOBUTTON(ID_EXTRA_PED_ROW_RIGHT_YES, DisplayMetricsPanel::OnExtraPedalButtonRowOffsetRightRadio)
	EVT_RADIOBUTTON(ID_EXTRA_PED_ROW_RIGHT_NO, DisplayMetricsPanel::OnExtraPedalButtonRowOffsetRightRadio)
	EVT_RADIOBUTTON(ID_BTNS_ABOVE_MAN_YES, DisplayMetricsPanel::OnButtonsAboveManualsRadio)
	EVT_RADIOBUTTON(ID_BTNS_ABOVE_MAN_NO, DisplayMetricsPanel::OnButtonsAboveManualsRadio)
	EVT_RADIOBUTTON(ID_EXTRA_STOPS_ABOVE_BTNS_YES, DisplayMetricsPanel::OnExtraDrawstopRowsAboveExtraButtonRowsRadio)
	EVT_RADIOBUTTON(ID_EXTRA_STOPS_ABOVE_BTNS_NO, DisplayMetricsPanel::OnExtraDrawstopRowsAboveExtraButtonRowsRadio)
	EVT_RADIOBUTTON(ID_TRIM_ABOVE_MAN_YES, DisplayMetricsPanel::OnTrimAboveManualsRadio)
	EVT_RADIOBUTTON(ID_TRIM_ABOVE_MAN_NO, DisplayMetricsPanel::OnTrimAboveManualsRadio)
	EVT_RADIOBUTTON(ID_TRIM_BELOW_MAN_YES, DisplayMetricsPanel::OnTrimBelowManualsRadio)
	EVT_RADIOBUTTON(ID_TRIM_BELOW_MAN_NO, DisplayMetricsPanel::OnTrimBelowManualsRadio)
	EVT_RADIOBUTTON(ID_TRIM_ABOVE_EXTRA_ROWS_YES, DisplayMetricsPanel::OnTrimAboveExtraRowsRadio)
	EVT_RADIOBUTTON(ID_TRIM_ABOVE_EXTRA_ROWS_NO, DisplayMetricsPanel::OnTrimAboveExtraRowsRadio)
	EVT_SPINCTRL(ID_DRAWSTOP_WIDTH_SPIN, DisplayMetricsPanel::OnDrawstopWidthSpin)
	EVT_SPINCTRL(ID_DRAWSTOP_HEIGHT_SPIN, DisplayMetricsPanel::OnDrawstopHeightSpin)
	EVT_SPINCTRL(ID_PISTON_WIDTH_SPIN, DisplayMetricsPanel::OnPistonWidthSpin)
	EVT_SPINCTRL(ID_PISTON_HEIGHT_SPIN, DisplayMetricsPanel::OnPistonHeightSpin)
	EVT_SPINCTRL(ID_ENCLOSURE_WIDTH_SPIN, DisplayMetricsPanel::OnEnclosureWidthSpin)
	EVT_SPINCTRL(ID_PEDAL_KEY_WIDTH_SPIN, DisplayMetricsPanel::OnPedalKeyWidthSpin)
	EVT_SPINCTRL(ID_MANUAL_KEY_WIDTH_SPIN, DisplayMetricsPanel::OnManualKeyWidthSpin)
	EVT_SPINCTRL(ID_ENCLOSURE_HEIGHT_SPIN , DisplayMetricsPanel::OnEnclosureHeightSpin)
	EVT_SPINCTRL(ID_PEDAL_HEIGHT_SPIN, DisplayMetricsPanel::OnPedalHeightSpin)
	EVT_SPINCTRL(ID_MANUAL_HEIGHT_SPIN, DisplayMetricsPanel::OnManualHeightSpin)
END_EVENT_TABLE()

DisplayMetricsPanel::DisplayMetricsPanel(wxWindow *parent) : wxPanel(parent) {
	GoPanelSize pSize;
	m_panelSizes = pSize.getPanelSizeNames();
	GoColor col;
	m_colors = col.getColorNames();
	m_displayMetrics = NULL;

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *horizSizeText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Horizontal panelsize: ")
	);
	firstRow->Add(horizSizeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_screenSizeHorizChoice = new wxChoice(
		this,
		ID_HORIZONTAL_SIZE_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_panelSizes
	);
	firstRow->Add(m_screenSizeHorizChoice, 1, wxEXPAND|wxALL, 5);
	m_screenSizeHorizSpin = new wxSpinCtrl(
		this,
		ID_HORIZONTAL_SIZE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		100,
		4000,
		800
	);
	firstRow->Add(m_screenSizeHorizSpin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *firstHalfRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *vertSizeText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Vertical panelsize: ")
	);
	firstHalfRow->Add(vertSizeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_screenSizeVertChoice = new wxChoice(
		this,
		ID_VERTICAL_SIZE_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_panelSizes
	);
	firstHalfRow->Add(m_screenSizeVertChoice, 1, wxEXPAND|wxALL, 5);
	m_screenSizeVertSpin = new wxSpinCtrl(
		this,
		ID_VERTICAL_SIZE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		100,
		4000,
		500
	);
	firstHalfRow->Add(m_screenSizeVertSpin, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(firstHalfRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *drawstopBgText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Drawstop background: ")
	);
	secondRow->Add(drawstopBgText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopBackground = new wxBitmapComboBox(
		this,
		ID_DRAWSTOP_BACKGROUND
	);
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_drawstopBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}
	secondRow->Add(m_drawstopBackground, 1, wxEXPAND, 0);
	wxStaticText *consoleBgText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Console background: ")
	);
	secondRow->Add(consoleBgText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_consoleBackground = new wxBitmapComboBox(
		this,
		ID_CONSOLE_BACKGROUND
	);
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_consoleBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}
	secondRow->Add(m_consoleBackground, 1, wxEXPAND, 0);
	panelSizer->Add(secondRow, 1, wxEXPAND);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *keyHorizBgText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Keyboard horizontal bg.: ")
	);
	thirdRow->Add(keyHorizBgText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_keyHorizBackground = new wxBitmapComboBox(
		this,
		ID_KEYBOARD_HORIZONTAL
	);
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_keyHorizBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}
	thirdRow->Add(m_keyHorizBackground, 1, wxEXPAND, 0);
	wxStaticText *keyVertBgText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Keyboard vertical bg.: ")
	);
	thirdRow->Add(keyVertBgText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_keyVertBackground = new wxBitmapComboBox(
		this,
		ID_KEYBOARD_VERTICAL
	);
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_keyVertBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}
	thirdRow->Add(m_keyVertBackground, 1, wxEXPAND, 0);
	wxStaticText *drawstopInsetBgText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Drawstop inset bg.: ")
	);
	thirdRow->Add(drawstopInsetBgText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopInsetBackground = new wxBitmapComboBox(
		this,
		ID_DRAWSTOP_INSET
	);
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_drawstopInsetBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}
	thirdRow->Add(m_drawstopInsetBackground, 1, wxEXPAND, 0);
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *controlFontText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Control label font: ")
	);
	fourthRow->Add(controlFontText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_controlLabelFont = new wxFontPickerCtrl(
		this,
		ID_CTRL_LABEL_FONT
	);
	fourthRow->Add(m_controlLabelFont, 1, wxEXPAND, 0);
	wxStaticText *groupFontText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Group label font: ")
	);
	fourthRow->Add(groupFontText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_groupLabelFont = new wxFontPickerCtrl(
		this,
		ID_GROUP_LABEL_FONT
	);
	fourthRow->Add(m_groupLabelFont, 1, wxEXPAND, 0);
	panelSizer->Add(fourthRow, 1, wxEXPAND);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *shortcutFontText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Shortcut key label font: ")
	);
	fifthRow->Add(shortcutFontText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_shortcutKeyLabelFont = new wxFontPickerCtrl(
		this,
		ID_SHORTCUT_KEY_LABEL_FONT
	);
	fifthRow->Add(m_shortcutKeyLabelFont, 1, wxEXPAND, 0);
	wxStaticText *shortcutColourText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Shortcut key label colour: ")
	);
	fifthRow->Add(shortcutColourText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_shortcutKeyLabelColour = new wxChoice(
		this,
		ID_SHORTCUT_COLOUR_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_colors
	);
	fifthRow->Add(m_shortcutKeyLabelColour, 1, wxEXPAND, 0);
	m_shortcutKeyColourPick = new wxColourPickerCtrl(
		this,
		ID_SHORTCUT_COLOUR_PICKER
	);
	fifthRow->Add(m_shortcutKeyColourPick, 1, wxEXPAND, 0);
	panelSizer->Add(fifthRow, 1, wxEXPAND);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *drawstopColText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Drawstop columns: ")
	);
	sixthRow->Add(drawstopColText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopCols = new wxSpinCtrl(
		this,
		ID_DRAWSTOP_COL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		2,
		12,
		2
	);
	sixthRow->Add(m_drawstopCols, 0, wxEXPAND|wxALL, 5);
	wxStaticText *pairColsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Pair columns: ")
	);
	sixthRow->Add(pairColsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pairDrawstopColsYes = new wxRadioButton(
		this,
		ID_PAIR_DRAWSTOP_COLS_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	sixthRow->Add(m_pairDrawstopColsYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pairDrawstopColsNo = new wxRadioButton(
		this,
		ID_PAIR_DRAWSTOP_COLS_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_pairDrawstopColsNo->SetValue(true);
	sixthRow->Add(m_pairDrawstopColsNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *drawstopRowText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Drawstop rows: ")
	);
	sixthRow->Add(drawstopRowText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopRows = new wxSpinCtrl(
		this,
		ID_DRAWSTOP_ROW_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		20,
		1
	);
	sixthRow->Add(m_drawstopRows, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *offsetColsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Drawstop Columns Offset: ")
	);
	seventhRow->Add(offsetColsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopColsOffsetYes = new wxRadioButton(
		this,
		ID_DRAWSTOP_COLS_OFFSET_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	seventhRow->Add(m_drawstopColsOffsetYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopColsOffsetNo = new wxRadioButton(
		this,
		ID_DRAWSTOP_COLS_OFFSET_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_drawstopColsOffsetNo->SetValue(true);
	seventhRow->Add(m_drawstopColsOffsetNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *outerColUpText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("With Outer Up: ")
	);
	seventhRow->Add(outerColUpText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopOuterColOffsetUpYes = new wxRadioButton(
		this,
		ID_OUTER_COL_UP_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	seventhRow->Add(m_drawstopOuterColOffsetUpYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopOuterColOffsetUpNo = new wxRadioButton(
		this,
		ID_OUTER_COL_UP_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_drawstopOuterColOffsetUpNo->SetValue(true);
	seventhRow->Add(m_drawstopOuterColOffsetUpNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(seventhRow, 0, wxGROW);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *extraDrawstopColsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Extra drawstop columns: ")
	);
	eighthRow->Add(extraDrawstopColsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraDrawstopCols = new wxSpinCtrl(
		this,
		ID_EXTRA_DRAWSTOP_COL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		40,
		0
	);
	eighthRow->Add(m_extraDrawstopCols, 0, wxEXPAND|wxALL, 5);
	wxStaticText *extraDrawstopRowsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Extra drawstop rows: ")
	);
	eighthRow->Add(extraDrawstopRowsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraDrawstopRows = new wxSpinCtrl(
		this,
		ID_EXTRA_DRAWSTOP_ROW_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		99,
		0
	);
	eighthRow->Add(m_extraDrawstopRows, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eighthRow, 0, wxGROW);

	wxBoxSizer *ninthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *buttonColsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Button columns: ")
	);
	ninthRow->Add(buttonColsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_buttonCols = new wxSpinCtrl(
		this,
		ID_BUTTON_COLS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		32,
		10
	);
	ninthRow->Add(m_buttonCols, 0, wxEXPAND|wxALL, 5);
	wxStaticText *extraButtonRowsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Extra button rows: ")
	);
	ninthRow->Add(extraButtonRowsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraButtonRows = new wxSpinCtrl(
		this,
		ID_EXTRA_BUTTON_ROW_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		99,
		0
	);
	ninthRow->Add(m_extraButtonRows, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(ninthRow, 0, wxGROW);

	wxBoxSizer *tenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *extraPedBtnRowText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Extra Ped button row: ")
	);
	tenthRow->Add(extraPedBtnRowText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraPedalButtonRowYes = new wxRadioButton(
		this,
		ID_EXTRA_PED_BTN_ROW_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	tenthRow->Add(m_extraPedalButtonRowYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraPedalButtonRowNo = new wxRadioButton(
		this,
		ID_EXTRA_PED_BTN_ROW_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_extraPedalButtonRowNo->SetValue(true);
	tenthRow->Add(m_extraPedalButtonRowNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *extraPedBtnRowOffsetText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Offset: ")
	);
	tenthRow->Add(extraPedBtnRowOffsetText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraPedalButtonRowOffsetYes = new wxRadioButton(
		this,
		ID_EXTRA_PED_ROW_OFFSET_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	tenthRow->Add(m_extraPedalButtonRowOffsetYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraPedalButtonRowOffsetNo = new wxRadioButton(
		this,
		ID_EXTRA_PED_ROW_OFFSET_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_extraPedalButtonRowOffsetNo->SetValue(true);
	tenthRow->Add(m_extraPedalButtonRowOffsetNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *offsetRightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Right: ")
	);
	tenthRow->Add(offsetRightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraPedalButtonRowOffsetRightYes = new wxRadioButton(
		this,
		ID_EXTRA_PED_ROW_RIGHT_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	tenthRow->Add(m_extraPedalButtonRowOffsetRightYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraPedalButtonRowOffsetRightNo = new wxRadioButton(
		this,
		ID_EXTRA_PED_ROW_RIGHT_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_extraPedalButtonRowOffsetRightNo->SetValue(true);
	tenthRow->Add(m_extraPedalButtonRowOffsetRightNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(tenthRow, 0, wxGROW);

	wxBoxSizer *eleventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *btnsAboveManText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Buttons above manuals: ")
	);
	eleventhRow->Add(btnsAboveManText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_buttonsAboveManualsYes = new wxRadioButton(
		this,
		ID_BTNS_ABOVE_MAN_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	eleventhRow->Add(m_buttonsAboveManualsYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_buttonsAboveManualsNo = new wxRadioButton(
		this,
		ID_BTNS_ABOVE_MAN_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_buttonsAboveManualsNo->SetValue(true);
	eleventhRow->Add(m_buttonsAboveManualsNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *extraDrawstopsAboveButtonsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Extra drawstops above buttons: ")
	);
	eleventhRow->Add(extraDrawstopsAboveButtonsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraDrawstopRowsAboveExtraButtonRowsYes = new wxRadioButton(
		this,
		ID_EXTRA_STOPS_ABOVE_BTNS_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	eleventhRow->Add(m_extraDrawstopRowsAboveExtraButtonRowsYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_extraDrawstopRowsAboveExtraButtonRowsNo = new wxRadioButton(
		this,
		ID_EXTRA_STOPS_ABOVE_BTNS_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_extraDrawstopRowsAboveExtraButtonRowsNo->SetValue(true);
	eleventhRow->Add(m_extraDrawstopRowsAboveExtraButtonRowsNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(eleventhRow, 0, wxGROW);

	wxBoxSizer *twelfthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *trimAboveManText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Trim above manuals: ")
	);
	twelfthRow->Add(trimAboveManText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trimAboveManualsYes = new wxRadioButton(
		this,
		ID_TRIM_ABOVE_MAN_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	twelfthRow->Add(m_trimAboveManualsYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trimAboveManualsNo = new wxRadioButton(
		this,
		ID_TRIM_ABOVE_MAN_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_trimAboveManualsNo->SetValue(true);
	twelfthRow->Add(m_trimAboveManualsNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *trimBelowManText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Trim below manuals: ")
	);
	twelfthRow->Add(trimBelowManText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trimBelowManualsYes = new wxRadioButton(
		this,
		ID_TRIM_BELOW_MAN_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	twelfthRow->Add(m_trimBelowManualsYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trimBelowManualsNo = new wxRadioButton(
		this,
		ID_TRIM_BELOW_MAN_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_trimBelowManualsNo->SetValue(true);
	twelfthRow->Add(m_trimBelowManualsNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *trimAboveExtraRowsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Trim above extra rows: ")
	);
	twelfthRow->Add(trimAboveExtraRowsText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trimAboveExtraRowsYes = new wxRadioButton(
		this,
		ID_TRIM_ABOVE_EXTRA_ROWS_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	twelfthRow->Add(m_trimAboveExtraRowsYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_trimAboveExtraRowsNo = new wxRadioButton(
		this,
		ID_TRIM_ABOVE_EXTRA_ROWS_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_trimAboveExtraRowsNo->SetValue(true);
	twelfthRow->Add(m_trimAboveExtraRowsNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(twelfthRow, 0, wxGROW);

	wxBoxSizer *thirteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *drawstopWidthText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Drawstop width: ")
	);
	thirteenthRow->Add(drawstopWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopWidth = new wxSpinCtrl(
		this,
		ID_DRAWSTOP_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		150,
		78
	);
	thirteenthRow->Add(m_drawstopWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *drawstopHeightText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Drawstop height: ")
	);
	thirteenthRow->Add(drawstopHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopHeight = new wxSpinCtrl(
		this,
		ID_DRAWSTOP_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		150,
		69
	);
	thirteenthRow->Add(m_drawstopHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(thirteenthRow, 0, wxGROW);

	wxBoxSizer *thirteenthHalfRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *pistonWidthText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Piston width: ")
	);
	thirteenthHalfRow->Add(pistonWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pistonWidth = new wxSpinCtrl(
		this,
		ID_PISTON_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		150,
		44
	);
	thirteenthHalfRow->Add(m_pistonWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *pistonHeightText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Piston height: ")
	);
	thirteenthHalfRow->Add(pistonHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pistonHeight = new wxSpinCtrl(
		this,
		ID_PISTON_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		150,
		40
	);
	thirteenthHalfRow->Add(m_pistonHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(thirteenthHalfRow, 0, wxGROW);

	wxBoxSizer *fourteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *manualKeyWidthText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Manual key width: ")
	);
	fourteenthRow->Add(manualKeyWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_manualKeyWidth = new wxSpinCtrl(
		this,
		ID_MANUAL_KEY_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		500,
		12
	);
	fourteenthRow->Add(m_manualKeyWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *manualHeightText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Manual height: ")
	);
	fourteenthRow->Add(manualHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_manualHeight = new wxSpinCtrl(
		this,
		ID_MANUAL_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		500,
		32
	);
	fourteenthRow->Add(m_manualHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(fourteenthRow, 0, wxGROW);

	wxBoxSizer *fifthteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *pedalKeyWidthText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Pedal key width: ")
	);
	fifthteenthRow->Add(pedalKeyWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pedalKeyWidth = new wxSpinCtrl(
		this,
		ID_PEDAL_KEY_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		500,
		7
	);
	fifthteenthRow->Add(m_pedalKeyWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *pedalHeightText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Pedal height: ")
	);
	fifthteenthRow->Add(pedalHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_pedalHeight = new wxSpinCtrl(
		this,
		ID_PEDAL_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		500,
		40
	);
	fifthteenthRow->Add(m_pedalHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(fifthteenthRow, 0, wxGROW);

	wxBoxSizer *sixteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *enclosureWidthText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Enclosure width: ")
	);
	sixteenthRow->Add(enclosureWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_enclosureWidth = new wxSpinCtrl(
		this,
		ID_ENCLOSURE_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		150,
		52
	);
	sixteenthRow->Add(m_enclosureWidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	wxStaticText *enclosureHeightText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("Enclosure height: ")
	);
	sixteenthRow->Add(enclosureHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_enclosureHeight = new wxSpinCtrl(
		this,
		ID_ENCLOSURE_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		150,
		63
	);
	sixteenthRow->Add(m_enclosureHeight, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(sixteenthRow, 0, wxGROW);

	SetSizer(panelSizer);
}

DisplayMetricsPanel::~DisplayMetricsPanel() {

}

void DisplayMetricsPanel::setDisplayMetrics(DisplayMetrics *displayMetrics) {
	SetupWoodBitmapBoxes();
	m_displayMetrics = displayMetrics;
	m_screenSizeHorizChoice->SetSelection(m_displayMetrics->m_dispScreenSizeHoriz.getSelectedNameIndex());
	m_screenSizeHorizSpin->SetValue(m_displayMetrics->m_dispScreenSizeHoriz.getNumericalValue());
	if (m_displayMetrics->m_dispScreenSizeHoriz.getSelectedNameIndex() != 4)
		m_screenSizeHorizSpin->Disable();
	m_screenSizeVertChoice->SetSelection(m_displayMetrics->m_dispScreenSizeVert.getSelectedNameIndex());
	m_screenSizeVertSpin->SetValue(m_displayMetrics->m_dispScreenSizeVert.getNumericalValue());
	if (m_displayMetrics->m_dispScreenSizeVert.getSelectedNameIndex() != 4)
		m_screenSizeVertSpin->Disable();

	m_drawstopBackground->SetSelection(m_displayMetrics->m_dispDrawstopBackgroundImageNum - 1);
	m_consoleBackground->SetSelection(m_displayMetrics->m_dispConsoleBackgroundImageNum - 1);
	m_keyHorizBackground->SetSelection(m_displayMetrics->m_dispKeyHorizBackgroundImageNum - 1);
	m_keyVertBackground->SetSelection(m_displayMetrics->m_dispKeyVertBackgroundImageNum - 1);
	m_drawstopInsetBackground->SetSelection(m_displayMetrics->m_dispDrawstopInsetBackgroundImageNum - 1);

	if (m_displayMetrics->m_dispControlLabelFont.IsOk())
		m_controlLabelFont->SetSelectedFont(m_displayMetrics->m_dispControlLabelFont);
	if (m_displayMetrics->m_dispShortcutKeyLabelFont.IsOk())
		m_shortcutKeyLabelFont->SetSelectedFont(m_displayMetrics->m_dispShortcutKeyLabelFont);
	if (m_displayMetrics->m_dispShortcutKeyLabelColour.getSelectedColorIndex() == 0) {
		// it's a custom color
		m_shortcutKeyLabelColour->SetSelection(0);
		m_shortcutKeyColourPick->Enable();
		m_shortcutKeyColourPick->SetColour(m_displayMetrics->m_dispShortcutKeyLabelColour.getColor());
	} else {
		// the color is text specified from the available ones in GO
		m_shortcutKeyLabelColour->SetSelection(m_displayMetrics->m_dispShortcutKeyLabelColour.getSelectedColorIndex());
		m_shortcutKeyColourPick->SetColour(m_displayMetrics->m_dispShortcutKeyLabelColour.getColor());
		m_shortcutKeyColourPick->Disable();
	}
	if (m_displayMetrics->m_dispGroupLabelFont.IsOk())
		m_groupLabelFont->SetSelectedFont(m_displayMetrics->m_dispGroupLabelFont);

	m_drawstopCols->SetValue(m_displayMetrics->m_dispDrawstopCols);
	m_drawstopRows->SetValue(m_displayMetrics->m_dispDrawstopRows);
	if (m_displayMetrics->m_dispDrawstopColsOffset) {
		m_drawstopColsOffsetYes->SetValue(true);
		m_drawstopColsOffsetNo->SetValue(false);
		m_drawstopOuterColOffsetUpYes->Enable();
		m_drawstopOuterColOffsetUpNo->Enable();
	} else {
		m_drawstopColsOffsetYes->SetValue(false);
		m_drawstopColsOffsetNo->SetValue(true);
		m_drawstopOuterColOffsetUpYes->Disable();
		m_drawstopOuterColOffsetUpNo->Disable();
	}
	if (m_displayMetrics->m_dispDrawstopOuterColOffsetUp) {
		m_drawstopOuterColOffsetUpYes->SetValue(true);
		m_drawstopOuterColOffsetUpNo->SetValue(false);
	} else {
		m_drawstopOuterColOffsetUpYes->SetValue(false);
		m_drawstopOuterColOffsetUpNo->SetValue(true);
	}
	if (m_displayMetrics->m_dispPairDrawstopCols) {
		m_pairDrawstopColsYes->SetValue(true);
		m_pairDrawstopColsNo->SetValue(false);
	} else {
		m_pairDrawstopColsYes->SetValue(false);
		m_pairDrawstopColsNo->SetValue(true);
	}
	m_extraDrawstopRows->SetValue(m_displayMetrics->m_dispExtraDrawstopRows);
	m_extraDrawstopCols->SetValue(m_displayMetrics->m_dispExtraDrawstopCols);
	m_buttonCols->SetValue(m_displayMetrics->m_dispButtonCols);
	m_extraButtonRows->SetValue(m_displayMetrics->m_dispExtraButtonRows);
	if (m_displayMetrics->m_dispExtraPedalButtonRow) {
		m_extraPedalButtonRowYes->SetValue(true);
		m_extraPedalButtonRowNo->SetValue(false);
		m_extraPedalButtonRowOffsetYes->Enable();
		m_extraPedalButtonRowOffsetNo->Enable();
		m_extraPedalButtonRowOffsetRightYes->Enable();
		m_extraPedalButtonRowOffsetRightNo->Enable();
	} else {
		m_extraPedalButtonRowYes->SetValue(false);
		m_extraPedalButtonRowNo->SetValue(true);
		m_extraPedalButtonRowOffsetYes->Enable(false);
		m_extraPedalButtonRowOffsetNo->Enable(false);
		m_extraPedalButtonRowOffsetRightYes->Enable(false);
		m_extraPedalButtonRowOffsetRightNo->Enable(false);
	}
	if (m_displayMetrics->m_dispExtraPedalButtonRowOffset) {
		m_extraPedalButtonRowOffsetYes->SetValue(true);
		m_extraPedalButtonRowOffsetNo->SetValue(false);
	} else {
		m_extraPedalButtonRowOffsetYes->SetValue(false);
		m_extraPedalButtonRowOffsetNo->SetValue(true);
	}
	if (m_displayMetrics->m_dispExtraPedalButtonRowOffsetRight) {
		m_extraPedalButtonRowOffsetRightYes->SetValue(true);
		m_extraPedalButtonRowOffsetRightNo->SetValue(false);
	} else {
		m_extraPedalButtonRowOffsetRightYes->SetValue(false);
		m_extraPedalButtonRowOffsetRightNo->SetValue(true);
	}
	if (m_displayMetrics->m_dispButtonsAboveManuals) {
		m_buttonsAboveManualsYes->SetValue(true);
		m_buttonsAboveManualsNo->SetValue(false);
	} else {
		m_buttonsAboveManualsYes->SetValue(false);
		m_buttonsAboveManualsNo->SetValue(true);
	}
	if (m_displayMetrics->m_dispTrimAboveManuals) {
		m_trimAboveManualsYes->SetValue(true);
		m_trimAboveManualsNo->SetValue(false);
	} else {
		m_trimAboveManualsYes->SetValue(false);
		m_trimAboveManualsNo->SetValue(true);
	}
	if (m_displayMetrics->m_dispTrimBelowManuals) {
		m_trimBelowManualsYes->SetValue(true);
		m_trimBelowManualsNo->SetValue(false);
	} else {
		m_trimBelowManualsYes->SetValue(false);
		m_trimBelowManualsNo->SetValue(true);
	}
	if (m_displayMetrics->m_dispTrimAboveExtraRows) {
		m_trimAboveExtraRowsYes->SetValue(true);
		m_trimAboveExtraRowsNo->SetValue(false);
	} else {
		m_trimAboveExtraRowsYes->SetValue(false);
		m_trimAboveExtraRowsNo->SetValue(true);
	}
	if (m_displayMetrics->m_dispExtraDrawstopRowsAboveExtraButtonRows) {
		m_extraDrawstopRowsAboveExtraButtonRowsYes->SetValue(true);
		m_extraDrawstopRowsAboveExtraButtonRowsNo->SetValue(false);
	} else {
		m_extraDrawstopRowsAboveExtraButtonRowsYes->SetValue(false);
		m_extraDrawstopRowsAboveExtraButtonRowsNo->SetValue(true);
	}
	m_drawstopWidth->SetValue(m_displayMetrics->m_dispDrawstopWidth);
	m_drawstopHeight->SetValue(m_displayMetrics->m_dispDrawstopHeight);
	m_pistonWidth->SetValue(m_displayMetrics->m_dispPistonWidth);
	m_pistonHeight->SetValue(m_displayMetrics->m_dispPistonHeight);
	m_enclosureWidth->SetValue(m_displayMetrics->m_dispEnclosureWidth);
	m_enclosureHeight->SetValue(m_displayMetrics->m_dispEnclosureHeight);
	m_pedalHeight->SetValue(m_displayMetrics->m_dispPedalHeight);
	m_pedalKeyWidth->SetValue(m_displayMetrics->m_dispPedalKeyWidth);
	m_manualHeight->SetValue(m_displayMetrics->m_dispManualHeight);
	m_manualKeyWidth->SetValue(m_displayMetrics->m_dispManualKeyWidth);

}

void DisplayMetricsPanel::OnHorizontalSizeChoice(wxCommandEvent& event) {
	if (event.GetId() == ID_HORIZONTAL_SIZE_CHOICE) {
		m_displayMetrics->m_dispScreenSizeHoriz.setSelectedNameIndex(m_screenSizeHorizChoice->GetSelection(), true);
		if (m_screenSizeHorizChoice->GetSelection() == 4) {
			m_screenSizeHorizSpin->Enable();
		} else {
			m_screenSizeHorizSpin->SetValue(m_displayMetrics->m_dispScreenSizeHoriz.getNumericalValue());
			m_screenSizeHorizSpin->Disable();
		}
	}
}

void DisplayMetricsPanel::OnHorizontalSizeChange(wxSpinEvent& event) {
	if (event.GetId() == ID_HORIZONTAL_SIZE_SPIN) {
		m_displayMetrics->m_dispScreenSizeHoriz.setNumericalValue(m_screenSizeHorizSpin->GetValue());
	}
}

void DisplayMetricsPanel::OnVerticalSizeChoice(wxCommandEvent& event) {
	if (event.GetId() == ID_VERTICAL_SIZE_CHOICE) {
		m_displayMetrics->m_dispScreenSizeVert.setSelectedNameIndex(m_screenSizeVertChoice->GetSelection(), false);
		if (m_screenSizeVertChoice->GetSelection() == 4) {
			m_screenSizeVertSpin->Enable();
		} else {
			m_screenSizeVertSpin->SetValue(m_displayMetrics->m_dispScreenSizeVert.getNumericalValue());
			m_screenSizeVertSpin->Disable();
		}
	}
}

void DisplayMetricsPanel::OnVerticalSizeChange(wxSpinEvent& event) {
	if (event.GetId() == ID_VERTICAL_SIZE_SPIN) {
		m_displayMetrics->m_dispScreenSizeVert.setNumericalValue(m_screenSizeVertSpin->GetValue());
	}
}

void DisplayMetricsPanel::OnDrawstopBackgroundChange(wxCommandEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispDrawstopBackgroundImageNum = m_drawstopBackground->GetSelection() + 1;
}

void DisplayMetricsPanel::OnConsoleBackgroundChange(wxCommandEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispConsoleBackgroundImageNum = m_consoleBackground->GetSelection() + 1;
}

void DisplayMetricsPanel::OnKeyHorizontalChange(wxCommandEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispKeyHorizBackgroundImageNum = m_keyHorizBackground->GetSelection() + 1;
}

void DisplayMetricsPanel::OnKeyVerticalChange(wxCommandEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispKeyVertBackgroundImageNum = m_keyVertBackground->GetSelection() + 1;
}

void DisplayMetricsPanel::OnDrawstopInsetBgChange(wxCommandEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispDrawstopInsetBackgroundImageNum = m_drawstopInsetBackground->GetSelection() + 1;
}

void DisplayMetricsPanel::OnControlLabelFontChange(wxFontPickerEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispControlLabelFont = m_controlLabelFont->GetSelectedFont();
	GoPanel *owning_panel = ::wxGetApp().m_frame->m_organ->getPanelOwningDisplayMetrics(m_displayMetrics);
	if (owning_panel) {
		if (owning_panel->getNumberOfGuiElements() > 0) {
			wxMessageDialog fontNameMsg(this, wxT("Do you want to apply this font name to all existing GUI Elements of button types?"), wxT("Apply font name to elements?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (fontNameMsg.ShowModal() == wxID_YES) {
				::wxGetApp().m_frame->m_organ->panelApplyButtonFontName(m_displayMetrics);
			}
			wxMessageDialog fontSizeMsg(this, wxT("Do you want to apply this font size to all existing GUI Elements of button types?"), wxT("Apply font size to elements?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (fontSizeMsg.ShowModal() == wxID_YES) {
				::wxGetApp().m_frame->m_organ->panelApplyButtonFontSize(m_displayMetrics);
			}
		}
	}
}

void DisplayMetricsPanel::OnShortcutKeyLabelFontChange(wxFontPickerEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispShortcutKeyLabelFont = m_shortcutKeyLabelFont->GetSelectedFont();
}

void DisplayMetricsPanel::OnShortcutKeyLabelColourChoice(wxCommandEvent& event) {
	if (event.GetId() == ID_SHORTCUT_COLOUR_CHOICE) {
		if (m_shortcutKeyLabelColour->GetSelection() == 0) {
			m_shortcutKeyColourPick->Enable();
		} else {
			m_displayMetrics->m_dispShortcutKeyLabelColour.setSelectedColorIndex(m_shortcutKeyLabelColour->GetSelection());
			m_shortcutKeyColourPick->SetColour(m_displayMetrics->m_dispShortcutKeyLabelColour.getColor());
			m_shortcutKeyColourPick->Disable();
		}
	}
}

void DisplayMetricsPanel::OnShortcutKeyLabelColourPick(wxColourPickerEvent& event) {
	if (event.GetId() == ID_SHORTCUT_COLOUR_PICKER) {
		m_displayMetrics->m_dispShortcutKeyLabelColour.setColorValue(m_shortcutKeyColourPick->GetColour());
	}
}

void DisplayMetricsPanel::OnGroupLabelFontChange(wxFontPickerEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispGroupLabelFont = m_groupLabelFont->GetSelectedFont();
	GoPanel *owning_panel = ::wxGetApp().m_frame->m_organ->getPanelOwningDisplayMetrics(m_displayMetrics);
	if (owning_panel) {
		if (owning_panel->getNumberOfGuiElements() > 0) {
			wxMessageDialog fontNameMsg(this, wxT("Do you want to apply this font name to existing GUI Elements of label types?"), wxT("Apply font name to elements?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (fontNameMsg.ShowModal() == wxID_YES) {
				::wxGetApp().m_frame->m_organ->panelApplyLabelFontName(m_displayMetrics);
			}
			wxMessageDialog fontSizeMsg(this, wxT("Do you want to apply this font size to existing GUI Elements of label types?"), wxT("Apply font size to elements?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (fontSizeMsg.ShowModal() == wxID_YES) {
				::wxGetApp().m_frame->m_organ->panelApplyLabelFontSize(m_displayMetrics);
			}
		}
	}
}

void DisplayMetricsPanel::OnDrawstopColSpin(wxSpinEvent& WXUNUSED(event)) {
	int value = m_drawstopCols->GetValue();
	if (!m_displayMetrics->m_dispPairDrawstopCols) {
		// we don't pair columns, but value must be even
		if (value < m_displayMetrics->m_dispDrawstopCols && value % 2 != 0) {
			value -= 1;
		} else if (value % 2 != 0) {
			value += 1;
		}
		if (value > 1 && value < 13) {
			m_displayMetrics->m_dispDrawstopCols = value;
			m_drawstopCols->SetValue(value);
		} else {
			m_drawstopCols->SetValue(m_displayMetrics->m_dispDrawstopCols);
		}
	} else {
		if (value < m_displayMetrics->m_dispDrawstopCols && value % 4 != 0) {
			value -= 3;
		} else if (value %4 != 0) {
			value += 3;
		}
		if (value > 3 && value < 13) {
			m_displayMetrics->m_dispDrawstopCols = value;
			m_drawstopCols->SetValue(value);
		} else {
			m_drawstopCols->SetValue(m_displayMetrics->m_dispDrawstopCols);
		}
	}
	::wxGetApp().m_frame->m_organ->panelDisplayMetricsUpdate(m_displayMetrics);
}

void DisplayMetricsPanel::OnDrawstopRowSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispDrawstopRows = m_drawstopRows->GetValue();
	::wxGetApp().m_frame->m_organ->panelDisplayMetricsUpdate(m_displayMetrics);
}

void DisplayMetricsPanel::OnDrawstopColsOffsetRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_DRAWSTOP_COLS_OFFSET_YES) {
		m_displayMetrics->m_dispDrawstopColsOffset = true;
		m_drawstopOuterColOffsetUpYes->Enable();
		m_drawstopOuterColOffsetUpNo->Enable();
	} else {
		m_displayMetrics->m_dispDrawstopColsOffset = false;
		m_drawstopOuterColOffsetUpYes->Disable();
		m_drawstopOuterColOffsetUpNo->Disable();
	}
}

void DisplayMetricsPanel::OnDrawstopOuterColOffsetUpRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_OUTER_COL_UP_YES) {
		m_displayMetrics->m_dispDrawstopOuterColOffsetUp = true;
	} else {
		m_displayMetrics->m_dispDrawstopOuterColOffsetUp = false;
	}
}

void DisplayMetricsPanel::OnPairDrawstopColsRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_PAIR_DRAWSTOP_COLS_YES) {
		m_displayMetrics->m_dispPairDrawstopCols = true;
		// if true, then the number of cols must be a multiple of 4
		int rest = m_displayMetrics->m_dispDrawstopCols % 4;
		if (rest != 0) {
			int nbrCols = m_displayMetrics->m_dispDrawstopCols + rest;
			m_displayMetrics->m_dispDrawstopCols = nbrCols;
			m_drawstopCols->SetValue(nbrCols);
		}
	} else {
		m_displayMetrics->m_dispPairDrawstopCols = false;
	}
}

void DisplayMetricsPanel::OnExtraDrawstopRowSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispExtraDrawstopRows = m_extraDrawstopRows->GetValue();
	::wxGetApp().m_frame->m_organ->panelDisplayMetricsUpdate(m_displayMetrics);
}

void DisplayMetricsPanel::OnExtraDrawstopColSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispExtraDrawstopCols = m_extraDrawstopCols->GetValue();
	::wxGetApp().m_frame->m_organ->panelDisplayMetricsUpdate(m_displayMetrics);
}

void DisplayMetricsPanel::OnButtonColSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispButtonCols = m_buttonCols->GetValue();
	::wxGetApp().m_frame->m_organ->panelDisplayMetricsUpdate(m_displayMetrics);
}

void DisplayMetricsPanel::OnExtraButtonRowSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispExtraButtonRows = m_extraButtonRows->GetValue();
	::wxGetApp().m_frame->m_organ->panelDisplayMetricsUpdate(m_displayMetrics);
}

void DisplayMetricsPanel::OnExtraPedalButtonRowRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_EXTRA_PED_BTN_ROW_YES) {
		m_displayMetrics->m_dispExtraPedalButtonRow = true;
		m_extraPedalButtonRowOffsetYes->Enable();
		m_extraPedalButtonRowOffsetNo->Enable();
		m_extraPedalButtonRowOffsetRightYes->Enable();
		m_extraPedalButtonRowOffsetRightNo->Enable();
	} else {
		m_displayMetrics->m_dispExtraPedalButtonRow = false;
		m_extraPedalButtonRowOffsetYes->Disable();
		m_extraPedalButtonRowOffsetNo->Disable();
		m_extraPedalButtonRowOffsetRightYes->Disable();
		m_extraPedalButtonRowOffsetRightNo->Disable();
	}
}

void DisplayMetricsPanel::OnExtraPedalButtonRowOffsetRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_EXTRA_PED_ROW_OFFSET_YES) {
		m_displayMetrics->m_dispExtraPedalButtonRowOffset = true;
	} else {
		m_displayMetrics->m_dispExtraPedalButtonRowOffset = false;
	}
}

void DisplayMetricsPanel::OnExtraPedalButtonRowOffsetRightRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_EXTRA_PED_ROW_RIGHT_YES) {
		m_displayMetrics->m_dispExtraPedalButtonRowOffsetRight = true;
	} else {
		m_displayMetrics->m_dispExtraPedalButtonRowOffsetRight = false;
	}
}

void DisplayMetricsPanel::OnButtonsAboveManualsRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_BTNS_ABOVE_MAN_YES) {
		m_displayMetrics->m_dispButtonsAboveManuals = true;
	} else {
		m_displayMetrics->m_dispButtonsAboveManuals = false;
	}
}

void DisplayMetricsPanel::OnTrimAboveManualsRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_TRIM_ABOVE_MAN_YES) {
		m_displayMetrics->m_dispTrimAboveManuals = true;
	} else {
		m_displayMetrics->m_dispTrimAboveManuals = false;
	}
}

void DisplayMetricsPanel::OnTrimBelowManualsRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_TRIM_BELOW_MAN_YES) {
		m_displayMetrics->m_dispTrimBelowManuals = true;
	} else {
		m_displayMetrics->m_dispTrimBelowManuals = false;
	}
}

void DisplayMetricsPanel::OnTrimAboveExtraRowsRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_TRIM_ABOVE_EXTRA_ROWS_YES) {
		m_displayMetrics->m_dispTrimAboveExtraRows = true;
	} else {
		m_displayMetrics->m_dispTrimAboveExtraRows = false;
	}
}

void DisplayMetricsPanel::OnExtraDrawstopRowsAboveExtraButtonRowsRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_EXTRA_STOPS_ABOVE_BTNS_YES) {
		m_displayMetrics->m_dispExtraDrawstopRowsAboveExtraButtonRows = true;
	} else {
		m_displayMetrics->m_dispExtraDrawstopRowsAboveExtraButtonRows = false;
	}
}

void DisplayMetricsPanel::OnDrawstopWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispDrawstopWidth = m_drawstopWidth->GetValue();
}

void DisplayMetricsPanel::OnDrawstopHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispDrawstopHeight = m_drawstopHeight->GetValue();
}

void DisplayMetricsPanel::OnPistonWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispPistonWidth = m_pistonWidth->GetValue();
}

void DisplayMetricsPanel::OnPistonHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispPistonHeight = m_pistonHeight->GetValue();
}

void DisplayMetricsPanel::OnEnclosureWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispEnclosureWidth = m_enclosureWidth->GetValue();
}

void DisplayMetricsPanel::OnEnclosureHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispEnclosureHeight = m_enclosureHeight->GetValue();
}

void DisplayMetricsPanel::OnPedalHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispPedalHeight = m_pedalHeight->GetValue();
}

void DisplayMetricsPanel::OnPedalKeyWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispPedalKeyWidth = m_pedalKeyWidth->GetValue();
}

void DisplayMetricsPanel::OnManualHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispManualHeight = m_manualHeight->GetValue();
}

void DisplayMetricsPanel::OnManualKeyWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_displayMetrics->m_dispManualKeyWidth = m_manualKeyWidth->GetValue();
}

void DisplayMetricsPanel::SetupWoodBitmapBoxes() {
	m_drawstopBackground->Clear();
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_drawstopBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}
	m_consoleBackground->Clear();
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_consoleBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}
	m_keyHorizBackground->Clear();
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_keyHorizBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}
	m_keyVertBackground->Clear();
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_keyVertBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}
	m_drawstopInsetBackground->Clear();
	for (unsigned i = 0; i < ::wxGetApp().m_scaledWoodBitmaps.size(); i++) {
		wxString woodNumber = wxString::Format(wxT("%d"), i + 1);
		m_drawstopInsetBackground->Append(woodNumber, ::wxGetApp().m_scaledWoodBitmaps[i]);
	}

}
