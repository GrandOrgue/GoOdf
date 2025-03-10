/*
 * GUIEnclosurePanel.cpp is part of GoOdf.
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

#include "GUIEnclosurePanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>
#include <algorithm>
#include <wx/msgdlg.h>
#include <wx/filename.h>

// Event table
BEGIN_EVENT_TABLE(GUIEnclosurePanel, wxPanel)
	EVT_TEXT(ID_GUIENCLOSUREPANEL_LABEL_TEXT, GUIEnclosurePanel::OnLabelTextChange)
	EVT_FONTPICKER_CHANGED(ID_GUIENCLOSUREPANEL_FONT_PICKER, GUIEnclosurePanel::OnLabelFontChange)
	EVT_CHOICE(ID_GUIENCLOSUREPANEL_COLOR_CHOICE, GUIEnclosurePanel::OnLabelColourChoice)
	EVT_COLOURPICKER_CHANGED(ID_GUIENCLOSUREPANEL_COLOR_PICKER, GUIEnclosurePanel::OnLabelColourPick)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_X_SPIN, GUIEnclosurePanel::OnPositionXSpin)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_Y_SPIN, GUIEnclosurePanel::OnPositionYSpin)
	EVT_COMBOBOX(ID_GUIENCLOSUREPANEL_STYLE_CHOICE, GUIEnclosurePanel::OnEnclosureStyleChoice)
	EVT_LISTBOX(ID_GUIENCLOSUREPANEL_BITMAP_BOX, GUIEnclosurePanel::OnBitmapChoice)
	EVT_BUTTON(ID_GUIENCLOSUREPANEL_ADD_BITMAP_BTN, GUIEnclosurePanel::OnAddBitmapBtn)
	EVT_BUTTON(ID_GUIENCLOSUREPANEL_IMAGE_PATH_BTN, GUIEnclosurePanel::OnAddImagePathBtn)
	EVT_BUTTON(ID_GUIENCLOSUREPANEL_MASK_PATH_BTN, GUIEnclosurePanel::OnAddMaskPathBtn)
	EVT_BUTTON(ID_GUIENCLOSUREPANEL_REMOVE_BITMAP_BTN, GUIEnclosurePanel::OnRemoveBitmapBtn)
	EVT_BUTTON(ID_GUIENCLOSUREPANEL_ADD_IMAGE_BITMAPS_BTN, GUIEnclosurePanel::OnAddImageBitmapsBtn)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_WIDTH_SPIN, GUIEnclosurePanel::OnWidthSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_HEIGHT_SPIN, GUIEnclosurePanel::OnHeightSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_TILE_X_SPIN, GUIEnclosurePanel::OnTileOffsetXSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_TILE_Y_SPIN, GUIEnclosurePanel::OnTileOffsetYSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_RECT_LEFT_SPIN, GUIEnclosurePanel::OnMouseRectLeftSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_RECT_TOP_SPIN, GUIEnclosurePanel::OnMouseRectTopSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_RECT_WIDTH_SPIN, GUIEnclosurePanel::OnMouseRectWidthSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_RECT_HEIGHT_SPIN, GUIEnclosurePanel::OnMouseRectHeightSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_AXIS_START_SPIN, GUIEnclosurePanel::OnMouseAxisStartSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_AXIS_END_SPIN, GUIEnclosurePanel::OnMouseAxisEndSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_TEXT_RECT_LEFT_SPIN, GUIEnclosurePanel::OnTextRectLeftSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_TEXT_RECT_TOP_SPIN, GUIEnclosurePanel::OnTextRectTopSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_TEXT_RECT_WIDTH_SPIN, GUIEnclosurePanel::OnTextRectWidthSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_TEXT_RECT_HEIGHT_SPIN, GUIEnclosurePanel::OnTextRectHeightSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_TEXT_BREAK_SPIN, GUIEnclosurePanel::OnTextBreakWidthSpin)
	EVT_BUTTON(ID_GUIENCLOSUREPANEL_REMOVE_BTN, GUIEnclosurePanel::OnRemoveEnclosureBtn)
END_EVENT_TABLE()

GUIEnclosurePanel::GUIEnclosurePanel(wxWindow *parent) : wxPanel(parent) {
	m_enclosure = NULL;
	GoColor col;
	m_colors = col.getColorNames();
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *labelText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispLabelText: ")
	);
	firstRow->Add(labelText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_labelTextField = new wxTextCtrl(
		this,
		ID_GUIENCLOSUREPANEL_LABEL_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_labelTextField, 1, wxEXPAND|wxALL, 5);
	wxStaticText *labelFontText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Label font: ")
	);
	firstRow->Add(labelFontText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_labelFont = new wxFontPickerCtrl(
		this,
		ID_GUIENCLOSUREPANEL_FONT_PICKER
	);
	firstRow->Add(m_labelFont, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *colorRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *labelColourText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("DispLabelColour: ")
	);
	colorRow->Add(labelColourText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
	m_labelColourChoice = new wxChoice(
		this,
		ID_GUIENCLOSUREPANEL_COLOR_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_colors
	);
	colorRow->Add(m_labelColourChoice, 1, wxEXPAND|wxLEFT|wxRIGHT, 5);
	m_labelColourPick = new wxColourPickerCtrl(
		this,
		ID_GUIENCLOSUREPANEL_COLOR_PICKER
	);
	colorRow->Add(m_labelColourPick, 1, wxEXPAND|wxLEFT|wxRIGHT, 5);
	panelSizer->Add(colorRow, 0, wxGROW);

	wxBoxSizer *positionRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *posXText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("PositionX: ")
	);
	positionRow->Add(posXText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_elementPosXSpin = new wxSpinCtrl(
		this,
		ID_GUIELEMENT_POS_X_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		1,
		-1
	);
	positionRow->Add(m_elementPosXSpin, 0, wxEXPAND|wxALL, 5);
	// positionRow->AddStretchSpacer();
	wxStaticText *posYText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("PositionY: ")
	);
	positionRow->Add(posYText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_elementPosYSpin = new wxSpinCtrl(
		this,
		ID_GUIELEMENT_POS_Y_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-1,
		1,
		-1
	);
	positionRow->Add(m_elementPosYSpin, 0, wxEXPAND|wxALL, 5);
	positionRow->AddStretchSpacer();
	wxStaticText *enclosureStyleText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("EnclosureStyle: ")
	);
	positionRow->Add(enclosureStyleText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_enclosureStyleBox = new wxBitmapComboBox(
		this,
		ID_GUIENCLOSUREPANEL_STYLE_CHOICE,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxCB_READONLY
	);
	for (unsigned i = 0; i < ::wxGetApp().m_enclosureStyleBitmaps.size(); i++) {
		wxString imgNumber = wxString::Format(wxT("Image %d"), i + 1);
		m_enclosureStyleBox->Append(imgNumber, ::wxGetApp().m_enclosureStyleBitmaps[i]);
	}
	positionRow->Add(m_enclosureStyleBox, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(positionRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *thirdFirstVertical = new wxBoxSizer(wxVERTICAL);
	m_bitmapBox = new wxListBox(
		this,
		ID_GUIENCLOSUREPANEL_BITMAP_BOX,
		wxDefaultPosition,
		wxDefaultSize
	);
	thirdFirstVertical->Add(m_bitmapBox, 1, wxGROW);
	thirdRow->Add(thirdFirstVertical, 1, wxGROW|wxALL, 5);
	wxBoxSizer *thirdVertical = new wxBoxSizer(wxVERTICAL);
	m_addBitmapBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_ADD_BITMAP_BTN,
		wxT("Add a new bitmap"),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	thirdVertical->Add(m_addBitmapBtn, 0, wxGROW);
	wxBoxSizer *verticalSecond = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imagePathText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Image: ")
	);
	verticalSecond->Add(imagePathText, 0, wxALIGN_CENTER_VERTICAL);
	m_imagePathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	verticalSecond->Add(m_imagePathField, 1, wxEXPAND, 0);
	m_addImagePathBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_IMAGE_PATH_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	verticalSecond->Add(m_addImagePathBtn, 0, wxLEFT, 5);
	thirdVertical->Add(verticalSecond, 0, wxGROW|wxTOP|wxBOTTOM, 5);
	wxBoxSizer *verticalThird = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *maskPathText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Mask: ")
	);
	verticalThird->Add(maskPathText, 0, wxALIGN_CENTER_VERTICAL);
	m_maskPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	verticalThird->Add(m_maskPathField, 1, wxEXPAND, 0);
	m_addMaskPathBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_MASK_PATH_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	verticalThird->Add(m_addMaskPathBtn, 0, wxLEFT, 5);
	thirdVertical->Add(verticalThird, 0, wxGROW);
	m_removeBitmapBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_REMOVE_BITMAP_BTN,
		wxT("Revmove selected bitmap"),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	thirdVertical->Add(m_removeBitmapBtn, 0, wxGROW|wxTOP|wxBOTTOM, 5);
	m_addImageBitmapsBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_ADD_IMAGE_BITMAPS_BTN,
		wxT("Browse for image(s) and add them as new bitmap(s)..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	thirdVertical->Add(m_addImageBitmapsBtn, 0, wxGROW);
	thirdRow->Add(thirdVertical, 2, wxGROW|wxALL, 5);
	panelSizer->Add(thirdRow, 1, wxGROW);

	wxBoxSizer *seventhRow  = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *widthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Width: ")
	);
	seventhRow->Add(widthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_widthSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	seventhRow->Add(m_widthSpin, 0, wxEXPAND|wxALL, 5);
	seventhRow->AddStretchSpacer();
	wxStaticText *heightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Height: ")
	);
	seventhRow->Add(heightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_heightSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	seventhRow->Add(m_heightSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(seventhRow, 0, wxEXPAND);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *tileOffsetXText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TileOffsetX: ")
	);
	eighthRow->Add(tileOffsetXText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
	m_tileOffsetXSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_TILE_X_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eighthRow->Add(m_tileOffsetXSpin, 0, wxEXPAND|wxLEFT, 5);
	eighthRow->AddStretchSpacer();
	wxStaticText *tileOffsetYText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TileOffsetY: ")
	);
	eighthRow->Add(tileOffsetYText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT, 5);
	m_tileOffsetYSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_TILE_Y_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eighthRow->Add(m_tileOffsetYSpin, 0, wxEXPAND|wxLEFT|wxRIGHT, 5);
	panelSizer->Add(eighthRow, 0, wxEXPAND);

	wxBoxSizer *ninthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *mouseRectLeftText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectLeft: ")
	);
	ninthRow->Add(mouseRectLeftText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectLeftSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_MOUSE_RECT_LEFT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	ninthRow->Add(m_mouseRectLeftSpin, 0, wxEXPAND|wxALL, 5);
	ninthRow->AddStretchSpacer();
	wxStaticText *mouseRectTopText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectTop: ")
	);
	ninthRow->Add(mouseRectTopText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectTopSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_MOUSE_RECT_TOP_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	ninthRow->Add(m_mouseRectTopSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(ninthRow, 0, wxEXPAND);

	wxBoxSizer *tenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *mouseRectWidthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectWidth: ")
	);
	tenthRow->Add(mouseRectWidthText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
	m_mouseRectWidthSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_MOUSE_RECT_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	tenthRow->Add(m_mouseRectWidthSpin, 0, wxEXPAND|wxLEFT|wxRIGHT, 5);
	tenthRow->AddStretchSpacer();
	wxStaticText *mouseRectHeightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectHeight: ")
	);
	tenthRow->Add(mouseRectHeightText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
	m_mouseRectHeightSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_MOUSE_RECT_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	tenthRow->Add(m_mouseRectHeightSpin, 0, wxEXPAND|wxLEFT|wxRIGHT, 5);
	panelSizer->Add(tenthRow, 0, wxEXPAND);

	wxBoxSizer *eleventh = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *mouseAxisStartText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseAxisStart: ")
	);
	eleventh->Add(mouseAxisStartText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseAxisStartSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_MOUSE_AXIS_START_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eleventh->Add(m_mouseAxisStartSpin, 0, wxEXPAND|wxALL, 5);
	eleventh->AddStretchSpacer();
	wxStaticText *mouseAxisEndText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseAxisEnd: ")
	);
	eleventh->Add(mouseAxisEndText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseAxisEndSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_MOUSE_AXIS_END_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eleventh->Add(m_mouseAxisEndSpin, 0, wxEXPAND|wxALL, 5);

	eleventh->AddStretchSpacer();
	wxStaticText *textBreakText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextBreakWidth: ")
	);
	eleventh->Add(textBreakText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textBreakWidthSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_TEXT_BREAK_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eleventh->Add(m_textBreakWidthSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eleventh, 0, wxEXPAND);

	wxBoxSizer *twelfthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textRectLeftText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextRectLeft: ")
	);
	twelfthRow->Add(textRectLeftText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
	m_textRectLeftSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_TEXT_RECT_LEFT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	twelfthRow->Add(m_textRectLeftSpin, 0, wxEXPAND|wxLEFT|wxRIGHT, 5);
	twelfthRow->AddStretchSpacer();
	wxStaticText *textRectTopText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextRectTop: ")
	);
	twelfthRow->Add(textRectTopText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
	m_textRectTopSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_TEXT_RECT_TOP_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	twelfthRow->Add(m_textRectTopSpin, 0, wxEXPAND|wxLEFT|wxRIGHT, 5);
	panelSizer->Add(twelfthRow, 0, wxEXPAND);

	wxBoxSizer *thirteenthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *textRectWidthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextRectWidth: ")
	);
	thirteenthRow->Add(textRectWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textRectWidthSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_TEXT_RECT_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	thirteenthRow->Add(m_textRectWidthSpin, 0, wxEXPAND|wxALL, 5);
	thirteenthRow->AddStretchSpacer();
	wxStaticText *textRectHeightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextRectHeight: ")
	);
	thirteenthRow->Add(textRectHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textRectHeightSpin = new wxSpinCtrl(
		this,
		ID_GUIENCLOSUREPANEL_TEXT_RECT_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	thirteenthRow->Add(m_textRectHeightSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(thirteenthRow, 0, wxEXPAND);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	m_removeEnclosureBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_REMOVE_BTN,
		wxT("Remove this element")
	);
	bottomRow->Add(m_removeEnclosureBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}
GUIEnclosurePanel::~GUIEnclosurePanel() {

}

void GUIEnclosurePanel::GUIEnclosurePanel::setEnclosure(GUIEnclosure *enclosure) {
	m_enclosure = enclosure;
	m_labelTextField->ChangeValue(m_enclosure->getDispLabelText());
	m_labelFont->SetSelectedFont(m_enclosure->getDispLabelFont());
	if (m_enclosure->getDispLabelColour()->getSelectedColorIndex() == 0) {
		// it's a custom color
		m_labelColourChoice->SetSelection(0);
		m_labelColourPick->Enable();
		m_labelColourPick->SetColour(m_enclosure->getDispLabelColour()->getColor());
	} else {
		// the color is text specified from the available ones in GO
		m_labelColourChoice->SetSelection(m_enclosure->getDispLabelColour()->getSelectedColorIndex());
		m_labelColourPick->SetColour(m_enclosure->getDispLabelColour()->getColor());
		m_labelColourPick->Disable();
	}
	m_enclosureStyleBox->Clear();
	for (unsigned i = 0; i < ::wxGetApp().m_enclosureStyleBitmaps.size(); i++) {
		wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
		m_enclosureStyleBox->Append(imgNumber, ::wxGetApp().m_enclosureStyleBitmaps[i]);
	}
	m_enclosureStyleBox->SetSelection(m_enclosure->getEnclosureStyle() - 1);
	if (m_enclosure->getNumberOfBitmaps() != 0) {
		// if enclosure has own bitmaps the enclosure style should be disabled and reset to 1
		// the bitmap listbox should be filled and first bitmap selected
		m_bitmapBox->Clear();
		wxArrayString bitmaps;
		for (unsigned i = 0; i < m_enclosure->getNumberOfBitmaps(); i++) {
			bitmaps.Add(wxT("Bitmap ") + wxString::Format(wxT("%i"), i + 1));
		}
		m_bitmapBox->InsertItems(bitmaps, 0);
		m_bitmapBox->SetSelection(0);
		m_removeBitmapBtn->Enable();
		m_enclosure->setEnclosureStyle(1);
		m_enclosureStyleBox->SetSelection(0);
		m_imagePathField->SetValue(m_enclosure->getBitmapAtIndex(0)->getRelativeImagePath());
		m_maskPathField->SetValue(m_enclosure->getBitmapAtIndex(0)->getRelativeMaskPath());
	} else {
		// no bitmaps existing so enclosure style is enabled and remove bitmap button disabled
		if (!m_bitmapBox->IsEmpty()) {
			m_bitmapBox->Clear();
		}
		m_addImagePathBtn->Disable();
		m_addMaskPathBtn->Disable();
		m_removeBitmapBtn->Disable();
	}
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
}

void GUIEnclosurePanel::updatePositionValues() {
	if (m_enclosure) {
		m_elementPosXSpin->SetValue(m_enclosure->getPosX());
		m_elementPosYSpin->SetValue(m_enclosure->getPosY());
	}
}

void GUIEnclosurePanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_labelTextField->SetToolTip(wxT("If another string is desired than the button name (or a shorter one for display reasons) it can be overridden here. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_labelFont->SetToolTip(wxT("Font face and size can be set here. The default value is derived from the display metrics control label font of the panel."));
		m_labelColourChoice->SetToolTip(wxT("Colour of the font can be chosen here. If a custom value is chosen the picker will be activated."));
		m_labelColourPick->SetToolTip(wxT("A custom colour can be picked here."));
		m_elementPosXSpin->SetToolTip(wxT("Absolute positioning x coordinate value. If set to -1 the built-in layout with rows/columns will be used instead."));
		m_elementPosYSpin->SetToolTip(wxT("Absolute positioning x coordinate value. If set to -1 the built-in layout with rows/columns will be used instead."));
		m_enclosureStyleBox->SetToolTip(wxT("Here a built-in enclosure bitmap style that GrandOrgue provides can be chosen."));
		m_bitmapBox->SetToolTip(wxT("Here the added bitmaps are listed. Select one to display/set/modify its paths."));
		m_addBitmapBtn->SetToolTip(wxT("Add an empty bitmap for the enclosure."));
		m_imagePathField->SetToolTip(wxT("Relative path to the image file for selected bitmap is shown here."));
		m_addImagePathBtn->SetToolTip(wxT("Opens a file dialog where the image file for this bitmap can be selected. NOTE: If this dialog is cancelled the image file for this bitmap can be removed!"));
		m_maskPathField->SetToolTip(wxT("Relative path to the mask file corresponding to the image file for selected bitmap is shown here."));
		m_addMaskPathBtn->SetToolTip(wxT("Opens a file dialog where the mask file for this bitmap can be selected. NOTE: If this dialog is cancelled the mask file for this bitmap can be removed!"));
		m_removeBitmapBtn->SetToolTip(wxT("Removes the bitmap and its associated image and mask."));
		m_addImageBitmapsBtn->SetToolTip(wxT("This button opens a file dialog where multiple images can be chosen and added as bitmaps for the enclosure. For this to work they have to be in correct sorting order (starting with the closed image and end with the open) and not use or depend on any mask files that exist in the same directory!"));
		m_widthSpin->SetToolTip(wxT("Width of bitmap to use. NOTE: Changing this value does not scale the image! If set greater than real image width - the shown image in GrandOrgue will be tiled. If set smaller than real image width - the shown image in GrandOrgue will be cropped."));
		m_heightSpin->SetToolTip(wxT("Height of bitmap to use. NOTE: Changing this value does not scale the image! If set greater than real image height - the shown image in GrandOrgue will be tiled. If set smaller than real image height - the shown image in GrandOrgue will be cropped."));
		m_tileOffsetXSpin->SetToolTip(wxT("Offset any tiling for the x axis."));
		m_tileOffsetYSpin->SetToolTip(wxT("Offset any tiling for the y axis."));
		m_mouseRectLeftSpin->SetToolTip(wxT("Controls the left coordinate for where the area of mouse interaction begin."));
		m_mouseRectTopSpin->SetToolTip(wxT("Controls the upper coordinate for where the area of mouse interaction begin."));
		m_mouseRectWidthSpin->SetToolTip(wxT("Set the width of the area of where mouse interaction is possible. NOTE: To get full rectangle area interaction the mouse radius must be set to 0, otherwise the interaction area is limited to a circle created with the mouse radius value."));
		m_mouseRectHeightSpin->SetToolTip(wxT("Set the height of the area of where mouse interaction is possible. NOTE: To get full rectangle area interaction the mouse radius must be set to 0, otherwise the interaction area is limited to a circle created with the mouse radius value."));
		m_mouseAxisStartSpin->SetToolTip(wxT("Set the top Y coordinate of the movement axis."));
		m_mouseAxisEndSpin->SetToolTip(wxT("Set the bottom Y coordinate of the movement axis."));
		m_textRectLeftSpin->SetToolTip(wxT("Controls to left coordinate for where text can be written on the element."));
		m_textRectTopSpin->SetToolTip(wxT("Controls to upper coordinate for where text can be written on the element."));
		m_textRectWidthSpin->SetToolTip(wxT("Set the total width available to write text on the element."));
		m_textRectHeightSpin->SetToolTip(wxT("Set the total height available to write text on the element."));
		m_textBreakWidthSpin->SetToolTip(wxT("Control line length of text written on the element. NOTE: Set this value to 0 (zero) to completely remove text and not write anything at all on the bitmap!"));
	} else {
		m_labelTextField->SetToolTip(wxEmptyString);
		m_labelFont->SetToolTip(wxEmptyString);
		m_labelColourChoice->SetToolTip(wxEmptyString);
		m_labelColourPick->SetToolTip(wxEmptyString);
		m_elementPosXSpin->SetToolTip(wxEmptyString);
		m_elementPosYSpin->SetToolTip(wxEmptyString);
		m_enclosureStyleBox->SetToolTip(wxEmptyString);
		m_bitmapBox->SetToolTip(wxEmptyString);
		m_addBitmapBtn->SetToolTip(wxEmptyString);
		m_imagePathField->SetToolTip(wxEmptyString);
		m_addImagePathBtn->SetToolTip(wxEmptyString);
		m_maskPathField->SetToolTip(wxEmptyString);
		m_addMaskPathBtn->SetToolTip(wxEmptyString);
		m_removeBitmapBtn->SetToolTip(wxEmptyString);
		m_addImageBitmapsBtn->SetToolTip(wxEmptyString);
		m_widthSpin->SetToolTip(wxEmptyString);
		m_heightSpin->SetToolTip(wxEmptyString);
		m_tileOffsetXSpin->SetToolTip(wxEmptyString);
		m_tileOffsetYSpin->SetToolTip(wxEmptyString);
		m_mouseRectLeftSpin->SetToolTip(wxEmptyString);
		m_mouseRectTopSpin->SetToolTip(wxEmptyString);
		m_mouseRectWidthSpin->SetToolTip(wxEmptyString);
		m_mouseRectHeightSpin->SetToolTip(wxEmptyString);
		m_mouseAxisStartSpin->SetToolTip(wxEmptyString);
		m_mouseAxisEndSpin->SetToolTip(wxEmptyString);
		m_textRectLeftSpin->SetToolTip(wxEmptyString);
		m_textRectTopSpin->SetToolTip(wxEmptyString);
		m_textRectWidthSpin->SetToolTip(wxEmptyString);
		m_textRectHeightSpin->SetToolTip(wxEmptyString);
		m_textBreakWidthSpin->SetToolTip(wxEmptyString);
	}
}

void GUIEnclosurePanel::refreshData() {
	m_labelTextField->ChangeValue(m_enclosure->getDispLabelText());
}

void GUIEnclosurePanel::OnLabelTextChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_labelTextField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_labelTextField);
	m_enclosure->setDispLabelText(m_labelTextField->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIEnclosurePanel::OnLabelFontChange(wxFontPickerEvent& WXUNUSED(event)) {
	m_enclosure->setDispLabelFont(m_labelFont->GetSelectedFont());
	m_enclosure->setDispLabelFontSize(m_labelFont->GetSelectedFont().GetPointSize());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIEnclosurePanel::OnLabelColourChoice(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIENCLOSUREPANEL_COLOR_CHOICE) {
		if (m_labelColourChoice->GetSelection() == 0) {
			m_labelColourPick->Enable();
		} else {
			m_enclosure->getDispLabelColour()->setSelectedColorIndex(m_labelColourChoice->GetSelection());
			m_labelColourPick->SetColour(m_enclosure->getDispLabelColour()->getColor());
			m_labelColourPick->Disable();
			::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
		}
	}
}

void GUIEnclosurePanel::OnLabelColourPick(wxColourPickerEvent& event) {
	if (event.GetId() == ID_GUIENCLOSUREPANEL_COLOR_PICKER) {
		m_enclosure->getDispLabelColour()->setColorValue(m_labelColourPick->GetColour());
		::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
	}
}

void GUIEnclosurePanel::OnPositionXSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that default display metric positioning is used
	int value = m_elementPosXSpin->GetValue();
	m_enclosure->setPosX(value);
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIEnclosurePanel::OnPositionYSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that default display metric positioning is used
	int value = m_elementPosYSpin->GetValue();
	m_enclosure->setPosY(value);
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIEnclosurePanel::OnEnclosureStyleChoice(wxCommandEvent& WXUNUSED(event)) {
	m_enclosure->setEnclosureStyle(m_enclosureStyleBox->GetSelection() + 1);
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIEnclosurePanel::OnBitmapChoice(wxCommandEvent& WXUNUSED(event)) {
	// when a bitmap is selected the image and mask fields should fill and the remove bitmap button be enabled
	int selectedIndex = m_bitmapBox->GetSelection();
	if (m_enclosure->getBitmapAtIndex(selectedIndex)->getImage() != wxEmptyString)
		m_imagePathField->SetValue(m_enclosure->getBitmapAtIndex(selectedIndex)->getRelativeImagePath());
	else
		m_imagePathField->SetValue(wxEmptyString);
	if (m_enclosure->getBitmapAtIndex(selectedIndex)->getMask() != wxEmptyString)
		m_maskPathField->SetValue(m_enclosure->getBitmapAtIndex(selectedIndex)->getRelativeMaskPath());
	else
		m_maskPathField->SetValue(wxEmptyString);
	m_addImagePathBtn->Enable();
	m_addMaskPathBtn->Enable();
	m_removeBitmapBtn->Enable();
}

void GUIEnclosurePanel::OnAddBitmapBtn(wxCommandEvent& WXUNUSED(event)) {
	GoImage bitmap;
	m_enclosure->addBitmap(bitmap);
	// add a new item to bitmapBox and select it
	unsigned existingBitmaps = m_enclosure->getNumberOfBitmaps();
	m_bitmapBox->Append(wxT("Bitmap ") + wxString::Format(wxT("%u"), existingBitmaps));
	m_bitmapBox->SetSelection(existingBitmaps - 1);
	// we need to notify the bitmapBox that selection has changed
	wxCommandEvent evt(wxEVT_LISTBOX, ID_GUIENCLOSUREPANEL_BITMAP_BOX);
	wxPostEvent(this, evt);
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnAddImagePathBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->setImage(path);
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (m_bitmapBox->GetSelection() == 0) {
				m_enclosure->setBitmapWidth(width);
				m_enclosure->setBitmapHeight(height);
				m_enclosure->setWidth(width);
				m_enclosure->setHeight(height);
				m_enclosure->setMouseRectWidth(width - m_enclosure->getMouseRectLeft());
				m_enclosure->setMouseRectHeight(height - m_enclosure->getMouseRectTop() - 3);
				m_enclosure->setMouseAxisStart(m_enclosure->getMouseRectHeight() / 3);
				m_enclosure->setMouseAxisEnd(m_enclosure->getMouseRectHeight() / 3 * 2);
				m_enclosure->setTextRectWidth(width - m_enclosure->getTextRectLeft());
				m_enclosure->setTextRectHeight(height - m_enclosure->getTextRectTop());
				m_enclosure->setTextBreakWidth(m_enclosure->getTextRectWidth());
				UpdateSpinRanges();
				UpdateDefaultSpinValues();
				m_imagePathField->SetValue(m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->getRelativeImagePath());
				::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
			} else {
				if (width == m_enclosure->getBitmapWidth() && height == m_enclosure->getBitmapHeight()) {
					m_imagePathField->SetValue(m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->getRelativeImagePath());
				} else {
					wxMessageDialog msg(this, wxT("Current bitmap size doesn't match the first bitmap! Please review/set first bitmap size correctly."), wxT("All bitmaps must be of same size!"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
					msg.ShowModal();
				}
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->getImage() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Image value is not empty! Do you want to empty it?"), wxT("Empty image value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->setImage(wxEmptyString);
				m_imagePathField->SetValue(wxEmptyString);
			}
		}
	}
}

void GUIEnclosurePanel::OnAddMaskPathBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (width == m_enclosure->getBitmapWidth() && height == m_enclosure->getBitmapHeight()) {
				m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->setMask(path);
				m_maskPathField->SetValue(m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->getMaskNameOnly());
				::wxGetApp().m_frame->m_organ->setModified(true);
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->getMask() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Mask value is not empty! Do you want to empty it?"), wxT("Empty mask value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->setMask(wxEmptyString);
				m_maskPathField->SetValue(wxEmptyString);
				::wxGetApp().m_frame->m_organ->setModified(true);
			}
		}
	}
}

void GUIEnclosurePanel::OnRemoveBitmapBtn(wxCommandEvent& WXUNUSED(event)) {
	int selectedIndex = m_bitmapBox->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		m_enclosure->removeBitmapAt(selectedIndex);
		m_bitmapBox->Delete(selectedIndex);
		if (m_enclosure->getNumberOfBitmaps() != 0 && !m_bitmapBox->IsEmpty()) {
			// there's still a bitmap to select
			if (selectedIndex > 0) {
				// we can push the selection to a lower index
				selectedIndex -= 1;
				m_bitmapBox->SetSelection(selectedIndex);
			} else {
				// we must select index 0 as it might be the only left
				m_bitmapBox->SetSelection(0);
			}
			// then we notify the box that selection has changed
			wxCommandEvent evt(wxEVT_LISTBOX, ID_GUIENCLOSUREPANEL_BITMAP_BOX);
			wxPostEvent(this, evt);
			::wxGetApp().m_frame->m_organ->setModified(true);
		} else {
			m_removeBitmapBtn->Disable();
			m_addImagePathBtn->Disable();
			m_addMaskPathBtn->Disable();
			UpdateBuiltinBitmapValues();
			UpdateSpinRanges();
			UpdateDefaultSpinValues();
			::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
		}
	}
}

void GUIEnclosurePanel::OnAddImageBitmapsBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayString paths;
	GetPathsForImageBitmaps(paths);
	if (!paths.IsEmpty()) {
		unsigned nbrBmpsBefore = m_enclosure->getNumberOfBitmaps();
		for (unsigned i = 0; i < paths.GetCount(); i++) {
			wxImage img = wxImage(paths[i]);
			if (img.IsOk()) {
				int width = img.GetWidth();
				int height = img.GetHeight();
				GoImage bitmap;
				bitmap.setImage(paths[i]);
				if (m_enclosure->getNumberOfBitmaps() != 0) {
					// bitmaps already exist so size of each new must match the first
					if (width == m_enclosure->getBitmapWidth() && height == m_enclosure->getBitmapHeight()) {
						m_enclosure->addBitmap(bitmap);
						unsigned existingBitmaps = m_enclosure->getNumberOfBitmaps();
						m_bitmapBox->Append(wxT("Bitmap ") + wxString::Format(wxT("%u"), existingBitmaps));
					} else {
						wxMessageDialog msg(this, wxT("Current bitmap size doesn't match the first bitmap! Please review/set first bitmap size correctly."), wxT("All bitmaps must be of same size!"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
						msg.ShowModal();
					}
				} else {
					// no bitmaps exist so we can just add this one and set default values from it
					m_enclosure->addBitmap(bitmap);
					m_enclosure->setBitmapWidth(width);
					m_enclosure->setBitmapHeight(height);
					m_enclosure->setWidth(width);
					m_enclosure->setHeight(height);
					m_enclosure->setMouseRectWidth(width - m_enclosure->getMouseRectLeft());
					m_enclosure->setMouseRectHeight(height - m_enclosure->getMouseRectTop() - 3);
					m_enclosure->setMouseAxisStart(m_enclosure->getMouseRectHeight() / 3);
					m_enclosure->setMouseAxisEnd(m_enclosure->getMouseRectHeight() / 3 * 2);
					m_enclosure->setTextRectWidth(width - m_enclosure->getTextRectLeft());
					m_enclosure->setTextRectHeight(height - m_enclosure->getTextRectTop());
					m_enclosure->setTextBreakWidth(m_enclosure->getTextRectWidth());
					unsigned existingBitmaps = m_enclosure->getNumberOfBitmaps();
					m_bitmapBox->Append(wxT("Bitmap ") + wxString::Format(wxT("%u"), existingBitmaps));
					UpdateSpinRanges();
					UpdateDefaultSpinValues();
				}
			}
		}
		if (m_enclosure->getNumberOfBitmaps() > nbrBmpsBefore) {
			// New bitmaps were added, we select the first new
			m_bitmapBox->SetSelection(nbrBmpsBefore);
			// we need to notify the bitmapBox that selection has changed
			wxCommandEvent evt(wxEVT_LISTBOX, ID_GUIENCLOSUREPANEL_BITMAP_BOX);
			wxPostEvent(this, evt);
			::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
		}
	}
}

void GUIEnclosurePanel::OnWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setWidth(m_widthSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setHeight(m_heightSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnTileOffsetXSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTileOffsetX(m_tileOffsetXSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnTileOffsetYSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTileOffsetY(m_tileOffsetYSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnMouseRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseRectLeft(m_mouseRectLeftSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnMouseRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseRectTop(m_mouseRectTopSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnMouseRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseRectWidth(m_mouseRectWidthSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnMouseRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseRectHeight(m_mouseRectHeightSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnMouseAxisStartSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseAxisStart(m_mouseAxisStartSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnMouseAxisEndSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseAxisEnd(m_mouseAxisEndSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnTextRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextRectLeft(m_textRectLeftSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnTextRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextRectTop(m_textRectTopSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnTextRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextRectWidth(m_textRectWidthSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnTextRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextRectHeight(m_textRectHeightSpin->GetValue());
	UpdateSpinRanges();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnTextBreakWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextBreakWidth(m_textBreakWidthSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIEnclosurePanel::OnRemoveEnclosureBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_enclosure->getOwningPanel()->getIsGuiElementFirstRemoval()) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this element?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			m_enclosure->getOwningPanel()->setIsGuiElementFirstRemoval(false);
			// then remove this item from the owning panel
			::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
		}
	} else {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIEnclosurePanel::UpdateSpinRanges() {
	if (m_enclosure->getPosX() > m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue())
		m_enclosure->setPosX(m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue());
	m_elementPosXSpin->SetRange(-1, m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue());
	if (m_enclosure->getPosY() > m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue())
		m_enclosure->setPosY(m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	m_elementPosYSpin->SetRange(-1, m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	if (m_enclosure->getWidth() > m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue())
		m_enclosure->setWidth(m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue());
	m_widthSpin->SetRange(0, m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue()); // panel width!
	if (m_enclosure->getHeight() > m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue())
		m_enclosure->setHeight(m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	m_heightSpin->SetRange(0, m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue()); // panel height!
	if (m_enclosure->getTileOffsetX() > (m_enclosure->getBitmapWidth() - 1))
		m_enclosure->setTileOffsetX(m_enclosure->getBitmapWidth() - 1);
	m_tileOffsetXSpin->SetRange(0, m_enclosure->getBitmapWidth() - 1);
	if (m_enclosure->getTileOffsetY() > (m_enclosure->getBitmapHeight() - 1))
		m_enclosure->setTileOffsetY(m_enclosure->getBitmapHeight() - 1);
	m_tileOffsetYSpin->SetRange(0, m_enclosure->getBitmapHeight() - 1);
	if (m_enclosure->getMouseRectLeft() > (m_enclosure->getWidth() - 1))
		m_enclosure->setMouseRectLeft(m_enclosure->getWidth() - 1);
	m_mouseRectLeftSpin->SetRange(0, m_enclosure->getWidth() - 1);
	if (m_enclosure->getMouseRectTop() > (m_enclosure->getHeight() - 1))
		m_enclosure->setMouseRectTop(m_enclosure->getHeight() - 1);
	m_mouseRectTopSpin->SetRange(0, m_enclosure->getHeight() - 1);
	if (m_enclosure->getMouseRectWidth() > (m_enclosure->getWidth() - m_enclosure->getMouseRectLeft()))
		m_enclosure->setMouseRectWidth(m_enclosure->getWidth() - m_enclosure->getMouseRectLeft());
	m_mouseRectWidthSpin->SetRange(1, m_enclosure->getWidth() - m_enclosure->getMouseRectLeft());
	if (m_enclosure->getMouseRectHeight() > (m_enclosure->getHeight() - m_enclosure->getMouseRectTop()))
		m_enclosure->setMouseRectHeight(m_enclosure->getHeight() - m_enclosure->getMouseRectTop());
	m_mouseRectHeightSpin->SetRange(1, m_enclosure->getHeight() - m_enclosure->getMouseRectTop());
	if (m_enclosure->getMouseAxisStart() > m_enclosure->getMouseRectHeight())
		m_enclosure->setMouseAxisStart(m_enclosure->getMouseRectHeight());
	m_mouseAxisStartSpin->SetRange(0, m_enclosure->getMouseRectHeight());
	if (m_enclosure->getMouseAxisEnd() > m_enclosure->getMouseRectHeight())
		m_enclosure->setMouseAxisEnd(m_enclosure->getMouseRectHeight());
	m_mouseAxisEndSpin->SetRange(m_enclosure->getMouseAxisStart(), m_enclosure->getMouseRectHeight());
	if (m_enclosure->getTextRectLeft() > (m_enclosure->getWidth() - 1))
		m_enclosure->setTextRectLeft(m_enclosure->getWidth() - 1);
	m_textRectLeftSpin->SetRange(0, m_enclosure->getWidth() - 1);
	if (m_enclosure->getTextRectTop() > (m_enclosure->getHeight() - 1))
		m_enclosure->setTextRectTop(m_enclosure->getHeight() - 1);
	m_textRectTopSpin->SetRange(0, m_enclosure->getHeight() - 1);
	if (m_enclosure->getTextRectWidth() > (m_enclosure->getWidth() - m_enclosure->getTextRectLeft()))
		m_enclosure->setTextRectWidth(m_enclosure->getWidth() - m_enclosure->getTextRectLeft());
	m_textRectWidthSpin->SetRange(1, m_enclosure->getWidth() - m_enclosure->getTextRectLeft());
	if (m_enclosure->getTextRectHeight() > (m_enclosure->getHeight() - m_enclosure->getTextRectTop()))
		m_enclosure->setTextRectHeight(m_enclosure->getHeight() - m_enclosure->getTextRectTop());
	m_textRectHeightSpin->SetRange(1, m_enclosure->getHeight() - m_enclosure->getTextRectTop());
	if (m_enclosure->getTextBreakWidth() > m_enclosure->getTextRectWidth())
		m_enclosure->setTextBreakWidth(m_enclosure->getTextRectWidth());
	m_textBreakWidthSpin->SetRange(0, m_enclosure->getTextRectWidth());
}

void GUIEnclosurePanel::UpdateDefaultSpinValues() {
	m_elementPosXSpin->SetValue(m_enclosure->getPosX());
	m_elementPosYSpin->SetValue(m_enclosure->getPosY());
	m_widthSpin->SetValue(m_enclosure->getWidth());
	m_heightSpin->SetValue(m_enclosure->getHeight());
	m_tileOffsetXSpin->SetValue(m_enclosure->getTileOffsetX());
	m_tileOffsetYSpin->SetValue(m_enclosure->getTileOffsetY());
	m_mouseRectLeftSpin->SetValue(m_enclosure->getMouseRectLeft());
	m_mouseRectTopSpin->SetValue(m_enclosure->getMouseRectTop());
	m_mouseRectWidthSpin->SetValue(m_enclosure->getMouseRectWidth());
	m_mouseRectHeightSpin->SetValue(m_enclosure->getMouseRectHeight());
	m_mouseAxisStartSpin->SetValue(m_enclosure->getMouseAxisStart());
	m_mouseAxisEndSpin->SetValue(m_enclosure->getMouseAxisEnd());
	m_textRectLeftSpin->SetValue(m_enclosure->getTextRectLeft());
	m_textRectTopSpin->SetValue(m_enclosure->getTextRectTop());
	m_textRectWidthSpin->SetValue(m_enclosure->getTextRectWidth());
	m_textRectHeightSpin->SetValue(m_enclosure->getTextRectHeight());
	m_textBreakWidthSpin->SetValue(m_enclosure->getTextBreakWidth());
}

wxString GUIEnclosurePanel::GetPathForImageFile() {
	wxString imageFilePath;
	wxString defaultPath = wxEmptyString;
	if (m_enclosure->getNumberOfBitmaps() > 0 && m_enclosure->getBitmapAtIndex(0)->getImage() != wxEmptyString) {
		wxFileName filePath = wxFileName(m_enclosure->getBitmapAtIndex(0)->getImage());
		if (filePath.FileExists())
			defaultPath = filePath.GetPath();
	} else {
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
	}
	if (defaultPath == wxEmptyString) {
		if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
			defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
		else
			defaultPath = wxStandardPaths::Get().GetDocumentsDir();
	}

	wxFileDialog fileDialog(
		this,
		wxT("Select image file (all image pixel sizes for this element must match)"),
		defaultPath,
		"",
		"Image files (*.png;*.bmp;*.gif;*.jpg;*.ico)|*.png;*.bmp;*.gif;*.jpg;*.ico;*.PNG;*.BMP;*.GIF;*.JPG;*.ICO;*.Png;*.Bmp;*.Gif;*.Jpg;*.Ico",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return wxEmptyString;

	imageFilePath = fileDialog.GetPath();
	return imageFilePath;
}

void GUIEnclosurePanel::GetPathsForImageBitmaps(wxArrayString &paths) {
	wxString defaultPath = wxEmptyString;
	if (m_enclosure->getNumberOfBitmaps() > 0 && m_enclosure->getBitmapAtIndex(0)->getImage() != wxEmptyString) {
		wxFileName filePath = wxFileName(m_enclosure->getBitmapAtIndex(0)->getImage());
		if (filePath.FileExists())
			defaultPath = filePath.GetPath();
	} else {
		defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
	}
	if (defaultPath == wxEmptyString)
		defaultPath = wxStandardPaths::Get().GetDocumentsDir();

	wxFileDialog fileDialog(
		this,
		wxT("Select image file(s), all pixel sizes for element(s) must match!"),
		defaultPath,
		"",
		"Image files (*.png;*.bmp;*.gif;*.jpg;*.ico)|*.png;*.bmp;*.gif;*.jpg;*.ico;*.PNG;*.BMP;*.GIF;*.JPG;*.ICO;*.Png;*.Bmp;*.Gif;*.Jpg;*.Ico",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE
	);

	if (fileDialog.ShowModal() == wxID_OK) {
		fileDialog.GetPaths(paths);
	}
}

void GUIEnclosurePanel::UpdateBuiltinBitmapValues() {
	int width = 46;
	int height = 61;
	m_enclosure->setBitmapWidth(width);
	m_enclosure->setBitmapHeight(height);
	m_enclosure->setWidth(width);
	m_enclosure->setHeight(height);
	m_enclosure->setMouseRectLeft(0);
	m_enclosure->setMouseRectTop(13);
	m_enclosure->setMouseRectWidth(width - m_enclosure->getMouseRectLeft());
	m_enclosure->setMouseRectHeight(height - m_enclosure->getMouseRectTop() - 3);
	m_enclosure->setMouseAxisStart(m_enclosure->getMouseRectHeight() / 3);
	m_enclosure->setMouseAxisEnd(m_enclosure->getMouseRectHeight() / 3 * 2);
	m_enclosure->setTextRectLeft(0);
	m_enclosure->setTextRectTop(0);
	m_enclosure->setTextRectWidth(width);
	m_enclosure->setTextRectHeight(height);
	m_enclosure->setTextBreakWidth(width);
}
