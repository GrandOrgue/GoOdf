/*
 * DisplayMetricsPanel.h is part of GoOdf.
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

#ifndef DISPLAYMETRICSPANEL_H
#define DISPLAYMETRICSPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/bmpcbox.h>
#include "DisplayMetrics.h"
#include <wx/clrpicker.h>
#include <wx/fontpicker.h>

class DisplayMetricsPanel : public wxPanel {
public:
	DisplayMetricsPanel(wxWindow *parent);
	~DisplayMetricsPanel();

	void setDisplayMetrics(DisplayMetrics *displayMetrics);
	void setTooltipsEnabled(bool isEnabled);

private:
	DECLARE_EVENT_TABLE()

	wxChoice *m_screenSizeHorizChoice;
	wxSpinCtrl *m_screenSizeHorizSpin;
	wxChoice *m_screenSizeVertChoice;
	wxSpinCtrl *m_screenSizeVertSpin;
	wxBitmapComboBox *m_drawstopBackground;
	wxBitmapComboBox *m_consoleBackground;
	wxBitmapComboBox *m_keyHorizBackground;
	wxBitmapComboBox *m_keyVertBackground;
	wxBitmapComboBox *m_drawstopInsetBackground;
	wxFontPickerCtrl *m_controlLabelFont;
	wxFontPickerCtrl *m_shortcutKeyLabelFont;
	wxChoice *m_shortcutKeyLabelColour;
	wxColourPickerCtrl *m_shortcutKeyColourPick;
	wxFontPickerCtrl *m_groupLabelFont;
	wxSpinCtrl *m_drawstopCols; // 2-12 must be even or multiple of 4 if pairDrawstopCols
	wxSpinCtrl *m_drawstopRows; // 1-20
	wxRadioButton *m_drawstopColsOffsetYes;
	wxRadioButton *m_drawstopColsOffsetNo;
	wxRadioButton *m_drawstopOuterColOffsetUpYes; // (required if m_dispDrawstopColsOffset is true)
	wxRadioButton *m_drawstopOuterColOffsetUpNo;
	wxRadioButton *m_pairDrawstopColsYes; // m_dispDrawstopCols must be multiple of 4
	wxRadioButton *m_pairDrawstopColsNo;
	wxSpinCtrl *m_extraDrawstopRows; // 0-99, numbering starts at 100
	wxSpinCtrl *m_extraDrawstopCols; // 0-40
	wxSpinCtrl *m_buttonCols; // 1-32
	wxSpinCtrl *m_extraButtonRows; // 0-99
	wxRadioButton *m_extraPedalButtonRowYes; // extra piston row (number 9)
	wxRadioButton *m_extraPedalButtonRowNo;
	wxRadioButton *m_extraPedalButtonRowOffsetYes; // (required if m_extraPedalButtonRowYes)
	wxRadioButton *m_extraPedalButtonRowOffsetNo; // (required if m_extraPedalButtonRowYes)
	wxRadioButton *m_extraPedalButtonRowOffsetRightYes; // (required if m_extraPedalButtonRowYes)
	wxRadioButton *m_extraPedalButtonRowOffsetRightNo; // (required if m_extraPedalButtonRowYes)
	wxRadioButton *m_buttonsAboveManualsYes;
	wxRadioButton *m_buttonsAboveManualsNo;
	wxRadioButton *m_trimAboveManualsYes;
	wxRadioButton *m_trimAboveManualsNo;
	wxRadioButton *m_trimBelowManualsYes;
	wxRadioButton *m_trimBelowManualsNo;
	wxRadioButton *m_trimAboveExtraRowsYes;
	wxRadioButton *m_trimAboveExtraRowsNo;
	wxRadioButton *m_extraDrawstopRowsAboveExtraButtonRowsYes;
	wxRadioButton *m_extraDrawstopRowsAboveExtraButtonRowsNo;
	wxSpinCtrl *m_drawstopWidth; // (integer 1-150, default: 78)
	wxSpinCtrl *m_drawstopHeight; // (integer 1-150, default: 69)
	wxSpinCtrl *m_pistonWidth; // (integer 1-150, default: 44)
	wxSpinCtrl *m_pistonHeight; // (integer 1-150, default: 40)
	wxSpinCtrl *m_enclosureWidth; // (integer 1-150, default: 52)
	wxSpinCtrl *m_enclosureHeight; // (integer 1-150, default: 63)
	wxSpinCtrl *m_pedalHeight; // (integer 1-500, default: 40)
	wxSpinCtrl *m_pedalKeyWidth; // (integer 1-500, default: 7)
	wxSpinCtrl *m_manualHeight; // (integer 1-500, default: 32)
	wxSpinCtrl *m_manualKeyWidth; // (integer 1-500, default: 12)

	DisplayMetrics *m_displayMetrics;
	wxArrayString m_panelSizes;
	wxArrayString m_colors;

	void OnHorizontalSizeChoice(wxCommandEvent& event);
	void OnHorizontalSizeChange(wxSpinEvent& event);
	void OnVerticalSizeChoice(wxCommandEvent& event);
	void OnVerticalSizeChange(wxSpinEvent& event);
	void OnDrawstopBackgroundChange(wxCommandEvent& event);
	void OnConsoleBackgroundChange(wxCommandEvent& event);
	void OnKeyHorizontalChange(wxCommandEvent& event);
	void OnKeyVerticalChange(wxCommandEvent& event);
	void OnDrawstopInsetBgChange(wxCommandEvent& event);
	void OnControlLabelFontChange(wxFontPickerEvent& event);
	void OnShortcutKeyLabelFontChange(wxFontPickerEvent& event);
	void OnShortcutKeyLabelColourChoice(wxCommandEvent& event);
	void OnShortcutKeyLabelColourPick(wxColourPickerEvent& event);
	void OnGroupLabelFontChange(wxFontPickerEvent& event);
	void OnDrawstopColSpin(wxSpinEvent& event);
	void OnDrawstopRowSpin(wxSpinEvent& event);
	void OnDrawstopColsOffsetRadio(wxCommandEvent& event);
	void OnDrawstopOuterColOffsetUpRadio(wxCommandEvent& event);
	void OnPairDrawstopColsRadio(wxCommandEvent& event);
	void OnExtraDrawstopRowSpin(wxSpinEvent& event);
	void OnExtraDrawstopColSpin(wxSpinEvent& event);
	void OnButtonColSpin(wxSpinEvent& event);
	void OnExtraButtonRowSpin(wxSpinEvent& event);
	void OnExtraPedalButtonRowRadio(wxCommandEvent& event);
	void OnExtraPedalButtonRowOffsetRadio(wxCommandEvent& event);
	void OnExtraPedalButtonRowOffsetRightRadio(wxCommandEvent& event);
	void OnButtonsAboveManualsRadio(wxCommandEvent& event);
	void OnTrimAboveManualsRadio(wxCommandEvent& event);
	void OnTrimBelowManualsRadio(wxCommandEvent& event);
	void OnTrimAboveExtraRowsRadio(wxCommandEvent& event);
	void OnExtraDrawstopRowsAboveExtraButtonRowsRadio(wxCommandEvent& event);
	void OnDrawstopWidthSpin(wxSpinEvent& event);
	void OnDrawstopHeightSpin(wxSpinEvent& event);
	void OnPistonWidthSpin(wxSpinEvent& event);
	void OnPistonHeightSpin(wxSpinEvent& event);
	void OnEnclosureWidthSpin(wxSpinEvent& event);
	void OnEnclosureHeightSpin(wxSpinEvent& event);
	void OnPedalHeightSpin(wxSpinEvent& event);
	void OnPedalKeyWidthSpin(wxSpinEvent& event);
	void OnManualHeightSpin(wxSpinEvent& event);
	void OnManualKeyWidthSpin(wxSpinEvent& event);

	void SetupWoodBitmapBoxes();

};

#endif
