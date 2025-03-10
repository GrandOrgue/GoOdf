/*
 * GUIEnclosurePanel.h is part of GoOdf.
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

#ifndef GUIENCLOSUREPANEL_H
#define GUIENCLOSUREPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/fontpicker.h>
#include <wx/clrpicker.h>
#include <wx/bmpcbox.h>
#include "GUIEnclosure.h"

class GUIEnclosurePanel : public wxPanel {
public:
	GUIEnclosurePanel(wxWindow *parent);
	~GUIEnclosurePanel();

	void setEnclosure(GUIEnclosure *enclosure);
	void updatePositionValues();
	void setTooltipsEnabled(bool isEnabled);
	void refreshData();

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_labelTextField;
	wxFontPickerCtrl *m_labelFont; // this control will handle both font size and font name
	wxChoice *m_labelColourChoice;
	wxColourPickerCtrl *m_labelColourPick;
	wxSpinCtrl *m_elementPosXSpin;
	wxSpinCtrl *m_elementPosYSpin;
	wxBitmapComboBox *m_enclosureStyleBox; // this should be filled with built in enclosure images, default 1
	wxListBox *m_bitmapBox;
	wxButton *m_addBitmapBtn;
	wxTextCtrl *m_imagePathField;
	wxButton *m_addImagePathBtn;
	wxTextCtrl *m_maskPathField;
	wxButton *m_addMaskPathBtn;
	wxButton *m_removeBitmapBtn;
	wxButton *m_addImageBitmapsBtn;
	wxSpinCtrl *m_widthSpin; // 0 - panel width, default bitmap width
	wxSpinCtrl *m_heightSpin; // 0 - panel height, default bitmap width
	wxSpinCtrl *m_tileOffsetXSpin; // (integer 0 - bitmap width, default: 0)
	wxSpinCtrl *m_tileOffsetYSpin; // (integer 0 - bitmap height, default: 0)
	wxSpinCtrl *m_mouseRectLeftSpin; // (integer 0 - Width, default: 0)
	wxSpinCtrl *m_mouseRectTopSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_mouseRectWidthSpin; // (integer 0 - Width, default: Width)
	wxSpinCtrl *m_mouseRectHeightSpin; // (integer 0 - Height, default: Height)
	wxSpinCtrl *m_mouseAxisStartSpin;
	wxSpinCtrl *m_mouseAxisEndSpin;
	wxSpinCtrl *m_textRectLeftSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_textRectTopSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_textRectWidthSpin; // (integer 0 - Width, default: Width)
	wxSpinCtrl *m_textRectHeightSpin; // (integer 0 - Height, default: Height)
	wxSpinCtrl *m_textBreakWidthSpin; // (integer 0 - text rectangle width, default: slightly smaller than Width)

	wxButton *m_removeEnclosureBtn;

	GUIEnclosure *m_enclosure;

	wxArrayString m_colors;

	void OnLabelTextChange(wxCommandEvent& event);
	void OnLabelFontChange(wxFontPickerEvent& event);
	void OnLabelColourChoice(wxCommandEvent& event);
	void OnLabelColourPick(wxColourPickerEvent& event);
	void OnPositionXSpin(wxSpinEvent& event);
	void OnPositionYSpin(wxSpinEvent& event);
	void OnEnclosureStyleChoice(wxCommandEvent& event);
	void OnBitmapChoice(wxCommandEvent& event);
	void OnAddBitmapBtn(wxCommandEvent& event);
	void OnAddImagePathBtn(wxCommandEvent& event);
	void OnAddMaskPathBtn(wxCommandEvent& event);
	void OnRemoveBitmapBtn(wxCommandEvent& event);
	void OnAddImageBitmapsBtn(wxCommandEvent& event);
	void OnWidthSpin(wxSpinEvent& event);
	void OnHeightSpin(wxSpinEvent& event);
	void OnTileOffsetXSpin(wxSpinEvent& event);
	void OnTileOffsetYSpin(wxSpinEvent& event);
	void OnMouseRectLeftSpin(wxSpinEvent& event);
	void OnMouseRectTopSpin(wxSpinEvent& event);
	void OnMouseRectWidthSpin(wxSpinEvent& event);
	void OnMouseRectHeightSpin(wxSpinEvent& event);
	void OnMouseAxisStartSpin(wxSpinEvent& event);
	void OnMouseAxisEndSpin(wxSpinEvent& event);
	void OnTextRectLeftSpin(wxSpinEvent& event);
	void OnTextRectTopSpin(wxSpinEvent& event);
	void OnTextRectWidthSpin(wxSpinEvent& event);
	void OnTextRectHeightSpin(wxSpinEvent& event);
	void OnTextBreakWidthSpin(wxSpinEvent& event);
	void OnRemoveEnclosureBtn(wxCommandEvent& event);

	void UpdateSpinRanges();
	void UpdateDefaultSpinValues();
	wxString GetPathForImageFile();
	void GetPathsForImageBitmaps(wxArrayString &paths);
	void UpdateBuiltinBitmapValues();
};

#endif
