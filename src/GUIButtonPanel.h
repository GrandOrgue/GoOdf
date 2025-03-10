/*
 * GUIButtonPanel.h is part of GoOdf.
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

#ifndef GUIBUTTONPANEL_H
#define GUIBUTTONPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/clrpicker.h>
#include <wx/bmpcbox.h>
#include "GUIButton.h"

class GUIButtonPanel : public wxPanel {
public:
	GUIButtonPanel(wxWindow *parent);
	~GUIButtonPanel();

	void setButton(GUIButton *button);
	void updatePositionValues();
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_labelTextField;
	wxButton *m_labelFont; // this control will trigger a font dialog that handle both font size and font name
	wxChoice *m_labelColourChoice;
	wxColourPickerCtrl *m_labelColourPick;
	wxRadioButton *m_displayAsPistonYes;
	wxRadioButton *m_displayAsPistonNo;
	wxRadioButton *m_displayKeyLabelLeftYes;
	wxRadioButton *m_displayKeyLabelLeftNo;
	wxSpinCtrl *m_elementPosXSpin;
	wxSpinCtrl *m_elementPosYSpin;
	wxBitmapComboBox *m_dispImageNbrBox; // this should be filled with different images depending on display as piston value, default 1
	wxSpinCtrl *m_buttonRowSpin; // used if display as piston yes
	wxSpinCtrl *m_buttonColSpin;
	wxSpinCtrl *m_drawstopRowSpin; // used if display as piston no
	wxSpinCtrl *m_drawstopColSpin;
	wxTextCtrl *m_imageOnPathField;
	wxButton *m_addImageOnBtn;
	wxTextCtrl *m_imageOffPathField;
	wxButton *m_addImageOffBtn;
	wxTextCtrl *m_maskOnPathField;
	wxButton *m_addMaskOnBtn;
	wxTextCtrl *m_maskOffPathField;
	wxButton *m_addMaskOffBtn;
	wxSpinCtrl *m_widthSpin; // 0 - panel width, default bitmap width
	wxSpinCtrl *m_heightSpin; // 0 - panel height, default bitmap width
	wxSpinCtrl *m_tileOffsetXSpin; // (integer 0 - bitmap width, default: 0)
	wxSpinCtrl *m_tileOffsetYSpin; // (integer 0 - bitmap height, default: 0)
	wxSpinCtrl *m_mouseRectLeftSpin; // (integer 0 - Width, default: 0)
	wxSpinCtrl *m_mouseRectTopSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_mouseRectWidthSpin; // (integer 0 - Width, default: Width)
	wxSpinCtrl *m_mouseRectHeightSpin; // (integer 0 - Height, default: Height)
	wxSpinCtrl *m_mouseRadiusSpin; // (integer 0 - max(MouseRectHeight, MouseRectWidth), default: max(MouseRectHeight, MouseRectWidth) / 2)
	wxSpinCtrl *m_textRectLeftSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_textRectTopSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_textRectWidthSpin; // (integer 0 - Width, default: Width)
	wxSpinCtrl *m_textRectHeightSpin; // (integer 0 - Height, default: Height)
	wxSpinCtrl *m_textBreakWidthSpin; // (integer 0 - text rectangle width, default: slightly smaller than Width)

	wxButton *m_removeButtonBtn;

	GUIButton *m_button;

	wxArrayString m_colors;
	wxString m_lastUsedImagePath;

	void OnLabelTextChange(wxCommandEvent& event);
	void OnLabelFontChange(wxCommandEvent& event);
	void OnLabelColourChoice(wxCommandEvent& event);
	void OnLabelColourPick(wxColourPickerEvent& event);
	void OnDisplayAsPistonRadio(wxCommandEvent& event);
	void OnDisplayKeyLabelLeftRadio(wxCommandEvent& event);
	void OnImageNumberChoice(wxCommandEvent& event);
	void OnButtonRowSpin(wxSpinEvent& event);
	void OnButtonColSpin(wxSpinEvent& event);
	void OnDrawstopRowSpin(wxSpinEvent& event);
	void OnDrawstopColSpin(wxSpinEvent& event);
	void OnAddImageOnBtn(wxCommandEvent& event);
	void OnAddImageOffBtn(wxCommandEvent& event);
	void OnAddMaskOnBtn(wxCommandEvent& event);
	void OnAddMaskOffBtn(wxCommandEvent& event);
	void OnWidthSpin(wxSpinEvent& event);
	void OnHeightSpin(wxSpinEvent& event);
	void OnTileOffsetXSpin(wxSpinEvent& event);
	void OnTileOffsetYSpin(wxSpinEvent& event);
	void OnMouseRectLeftSpin(wxSpinEvent& event);
	void OnMouseRectTopSpin(wxSpinEvent& event);
	void OnMouseRectWidthSpin(wxSpinEvent& event);
	void OnMouseRectHeightSpin(wxSpinEvent& event);
	void OnMouseRadiusSpin(wxSpinEvent& event);
	void OnTextRectLeftSpin(wxSpinEvent& event);
	void OnTextRectTopSpin(wxSpinEvent& event);
	void OnTextRectWidthSpin(wxSpinEvent& event);
	void OnTextRectHeightSpin(wxSpinEvent& event);
	void OnTextBreakWidthSpin(wxSpinEvent& event);
	void OnPositionXSpin(wxSpinEvent& event);
	void OnPositionYSpin(wxSpinEvent& event);
	void OnRemoveButtonBtn(wxCommandEvent& event);

	void SetupImageNbrBoxContent();
	void UpdateSpinRanges();
	void UpdateDefaultSpinValues();
	wxString GetPathForImageFile();
};

#endif
