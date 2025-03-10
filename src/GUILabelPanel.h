/*
 * GUILabelPanel.h is part of GoOdf.
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

#ifndef GUILABELPANEL_H
#define GUILABELPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include <wx/bmpcbox.h>
#include "GUILabel.h"

class GUILabelPanel : public wxPanel {
public:
	GUILabelPanel(wxWindow *parent);
	~GUILabelPanel();

	void setLabel(GUILabel *label);
	void updatePositionValues();
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_labelTextField; // name of label == text that it will display
	wxButton *m_labelFont; // this control will activate a wxfontdialog that handle both font size and font name
	wxChoice *m_labelColourChoice;
	wxColourPickerCtrl *m_labelColourPick;
	wxRadioButton *m_freeXposYes;
	wxRadioButton *m_freeXposNo;
	wxRadioButton *m_freeYposYes;
	wxRadioButton *m_freeYposNo;
	wxSpinCtrl *m_dispXposSpin;
	wxSpinCtrl *m_dispYposSpin;
	wxSpinCtrl *m_elementPosXSpin;
	wxSpinCtrl *m_elementPosYSpin;
	wxBitmapComboBox *m_dispImageNbrBox; // this is filled with different images for label styles default 1
	wxRadioButton *m_atTopOfDrawstopColYes;
	wxRadioButton *m_atTopOfDrawstopColNo;
	wxRadioButton *m_spanDrawstopColToRightYes;
	wxRadioButton *m_spanDrawstopColToRightNo;
	wxSpinCtrl *m_drawstopColSpin;
	wxTextCtrl *m_imagePathField;
	wxButton *m_addImageBtn;
	wxTextCtrl *m_maskPathField;
	wxButton *m_addMaskBtn;
	wxSpinCtrl *m_widthSpin; // 0 - panel width, default bitmap width
	wxSpinCtrl *m_heightSpin; // 0 - panel height, default bitmap width
	wxSpinCtrl *m_tileOffsetXSpin; // (integer 0 - bitmap width, default: 0)
	wxSpinCtrl *m_tileOffsetYSpin; // (integer 0 - bitmap height, default: 0)
	wxSpinCtrl *m_textRectLeftSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_textRectTopSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_textRectWidthSpin; // (integer 0 - Width, default: Width)
	wxSpinCtrl *m_textRectHeightSpin; // (integer 0 - Height, default: Height)
	wxSpinCtrl *m_textBreakWidthSpin; // (integer 0 - text rectangle width, default: slightly smaller than Width)
	wxButton *m_removeLabelBtn;

	GUILabel *m_label;

	wxArrayString m_colors;
	wxString m_lastUsedImagePath;

	void OnLabelTextChange(wxCommandEvent& event);
	void OnLabelFontChange(wxCommandEvent& event);
	void OnLabelColourChoice(wxCommandEvent& event);
	void OnLabelColourPick(wxColourPickerEvent& event);
	void OnFreeXposRadio(wxCommandEvent& event);
	void OnFreeYposRadio(wxCommandEvent& event);
	void OnPositionXSpin(wxSpinEvent& event);
	void OnPositionYSpin(wxSpinEvent& event);
	void OnImageNumberChoice(wxCommandEvent& event);
	void OnDispXposSpin(wxSpinEvent& event);
	void OnDispYposSpin(wxSpinEvent& event);
	void OnAtTopOfDrawstopColRadio(wxCommandEvent& event);
	void OnSpanDrawstopColRightRadio(wxCommandEvent& event);
	void OnDrawstopColSpin(wxSpinEvent& event);
	void OnAddImageBtn(wxCommandEvent& event);
	void OnAddMaskBtn(wxCommandEvent& event);
	void OnWidthSpin(wxSpinEvent& event);
	void OnHeightSpin(wxSpinEvent& event);
	void OnTileOffsetXSpin(wxSpinEvent& event);
	void OnTileOffsetYSpin(wxSpinEvent& event);
	void OnTextRectLeftSpin(wxSpinEvent& event);
	void OnTextRectTopSpin(wxSpinEvent& event);
	void OnTextRectWidthSpin(wxSpinEvent& event);
	void OnTextRectHeightSpin(wxSpinEvent& event);
	void OnTextBreakWidthSpin(wxSpinEvent& event);
	void OnRemoveLabelBtn(wxCommandEvent& event);

	void UpdateSpinRanges();
	void UpdateDefaultSpinValues();
	void UpdateDefaultImageValues();
	wxString GetPathForImageFile();
};

#endif
