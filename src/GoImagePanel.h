/*
 * GoImagePanel.h is part of GoOdf.
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

#ifndef IMAGEPANEL_H
#define IMAGEPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "GoImage.h"

class GoImagePanel : public wxPanel {
public:
	GoImagePanel(wxWindow *parent);
	~GoImagePanel();

	void setImage(GoImage *image);
	void setIsFirstRemoval(bool value);
	void updatePositionValues();
	void setTooltipsEnabled(bool isEnabled);

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_imagePathField;
	wxButton *m_addImageBtn;
	wxTextCtrl *m_maskPathField;
	wxButton *m_addMaskBtn;

	wxSpinCtrl *m_posXSpin;
	wxSpinCtrl *m_posYSpin;
	wxSpinCtrl *m_widthSpin;
	wxSpinCtrl *m_heightSpin;
	wxSpinCtrl *m_tileOffsetXSpin;
	wxSpinCtrl *m_tileOffsetYSpin;
	wxButton *removeImageBtn;

	GoImage *m_image;
	bool m_isFirstRemoval;

	void OnAddImageBtn(wxCommandEvent& event);
	void OnAddMaskBtn(wxCommandEvent& event);
	void OnPosXSpin(wxSpinEvent& event);
	void OnPosYSpin(wxSpinEvent& event);
	void OnWidthSpin(wxSpinEvent& event);
	void OnHeightSpin(wxSpinEvent& event);
	void OnTileOffsetXSpin(wxSpinEvent& event);
	void OnTileOffsetYSpin(wxSpinEvent& event);
	void OnRemoveImageBtn(wxCommandEvent& event);

	void UpdateControlValues();

};

#endif
