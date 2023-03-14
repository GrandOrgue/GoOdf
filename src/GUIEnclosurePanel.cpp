/*
 * GUIEnclosurePanel.cpp is part of GOODF.
 * Copyright (C) 2023 Lars Palo
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
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_WIDTH_SPIN, GUIEnclosurePanel::OnWidthSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_HEIGHT_SPIN, GUIEnclosurePanel::OnHeightSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_TILE_X_SPIN, GUIEnclosurePanel::OnTileOffsetXSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_TILE_Y_SPIN, GUIEnclosurePanel::OnTileOffsetYSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_RECT_LEFT_SPIN, GUIEnclosurePanel::OnMouseRectLeftSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_RECT_TOP_SPIN, GUIEnclosurePanel::OnMouseRectTopSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_RECT_WIDTH_SPIN, GUIEnclosurePanel::OnMouseRectWidthSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_RECT_HEIGHT_SPIN, GUIEnclosurePanel::OnMouseRectHeightSpin)
	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_AXIS_START_SPIN, GUIEnclosurePanel::OnMouseAxisStartSpin)
//	EVT_SPINCTRL(ID_GUIENCLOSUREPANEL_MOUSE_AXIS_END_SPIN, GUIEnclosurePanel::OnMouseAxisEndSpin)
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
	colorRow->Add(labelColourText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_labelColourChoice = new wxChoice(
		this,
		ID_GUIENCLOSUREPANEL_COLOR_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_colors
	);
	colorRow->Add(m_labelColourChoice, 1, wxEXPAND, 0);
	m_labelColourPick = new wxColourPickerCtrl(
		this,
		ID_GUIENCLOSUREPANEL_COLOR_PICKER
	);
	colorRow->Add(m_labelColourPick, 1, wxEXPAND, 0);
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
	positionRow->Add(m_enclosureStyleBox, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(positionRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	m_bitmapBox = new wxListBox(
		this,
		ID_GUIENCLOSUREPANEL_BITMAP_BOX,
		wxDefaultPosition,
		wxDefaultSize
	);
	thirdRow->Add(m_bitmapBox, 1, wxGROW|wxALL, 5);
	wxBoxSizer *thirdVertical = new wxBoxSizer(wxVERTICAL);
	m_addBitmapBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_ADD_BITMAP_BTN,
		wxT("Add a new bitmap"),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	thirdVertical->Add(m_addBitmapBtn, 0, wxGROW|wxALL, 5);
	wxBoxSizer *verticalSecond = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imagePathText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Image: ")
	);
	verticalSecond->Add(imagePathText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_imagePathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	verticalSecond->Add(m_imagePathField, 1, wxEXPAND|wxALL, 5);
	m_addImagePathBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_IMAGE_PATH_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	verticalSecond->Add(m_addImagePathBtn, 0, wxALL, 5);
	thirdVertical->Add(verticalSecond, 0, wxGROW);
	wxBoxSizer *verticalThird = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *maskPathText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Mask: ")
	);
	verticalThird->Add(maskPathText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maskPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	verticalThird->Add(m_maskPathField, 1, wxEXPAND|wxALL, 5);
	m_addMaskPathBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_MASK_PATH_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	verticalThird->Add(m_addMaskPathBtn, 0, wxALL, 5);
	thirdVertical->Add(verticalThird, 0, wxGROW);
	m_removeBitmapBtn = new wxButton(
		this,
		ID_GUIENCLOSUREPANEL_REMOVE_BITMAP_BTN,
		wxT("Revmove selected bitmap"),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	thirdVertical->Add(m_removeBitmapBtn, 0, wxGROW|wxALL, 5);
	thirdRow->Add(thirdVertical, 2, wxGROW);
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
	eighthRow->Add(tileOffsetXText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
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
	eighthRow->Add(m_tileOffsetXSpin, 0, wxEXPAND|wxALL, 5);
	eighthRow->AddStretchSpacer();
	wxStaticText *tileOffsetYText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TileOffsetY: ")
	);
	eighthRow->Add(tileOffsetYText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
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
	eighthRow->Add(m_tileOffsetYSpin, 0, wxEXPAND|wxALL, 5);
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
	tenthRow->Add(mouseRectWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
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
	tenthRow->Add(m_mouseRectWidthSpin, 0, wxEXPAND|wxALL, 5);
	tenthRow->AddStretchSpacer();
	wxStaticText *mouseRectHeightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectHeight: ")
	);
	tenthRow->Add(mouseRectHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
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
	tenthRow->Add(m_mouseRectHeightSpin, 0, wxEXPAND|wxALL, 5);
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
/*
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
*/
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
	twelfthRow->Add(textRectLeftText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
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
	twelfthRow->Add(m_textRectLeftSpin, 0, wxEXPAND|wxALL, 5);
	twelfthRow->AddStretchSpacer();
	wxStaticText *textRectTopText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextRectTop: ")
	);
	twelfthRow->Add(textRectTopText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
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
	twelfthRow->Add(m_textRectTopSpin, 0, wxEXPAND|wxALL, 5);
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
	m_labelTextField->SetValue(m_enclosure->getDispLabelText());
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
		// m_enclosureStyleBox->Disable();
	} else {
		// no bitmaps existing so enclosure style is enabled and remove bitmap button disabled
		if (!m_bitmapBox->IsEmpty()) {
			m_bitmapBox->Clear();
		}
		// m_enclosureStyleBox->Enable();
		m_addImagePathBtn->Disable();
		m_addMaskPathBtn->Disable();
		m_removeBitmapBtn->Disable();
		UpdateBuiltinBitmapValues();
	}
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
}

void GUIEnclosurePanel::OnLabelTextChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_labelTextField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_labelTextField);
	m_enclosure->setDispLabelText(m_labelTextField->GetValue());
}

void GUIEnclosurePanel::OnLabelFontChange(wxFontPickerEvent& WXUNUSED(event)) {
	m_enclosure->setDispLabelFont(m_labelFont->GetSelectedFont());
	m_enclosure->setDispLabelFontSize(m_labelFont->GetFont().GetPointSize());
}

void GUIEnclosurePanel::OnLabelColourChoice(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIENCLOSUREPANEL_COLOR_CHOICE) {
		if (m_labelColourChoice->GetSelection() == 0) {
			m_labelColourPick->Enable();
		} else {
			m_enclosure->getDispLabelColour()->setSelectedColorIndex(m_labelColourChoice->GetSelection());
			m_labelColourPick->SetColour(m_enclosure->getDispLabelColour()->getColor());
			m_labelColourPick->Disable();
		}
	}
}

void GUIEnclosurePanel::OnLabelColourPick(wxColourPickerEvent& event) {
	if (event.GetId() == ID_GUIENCLOSUREPANEL_COLOR_PICKER) {
		m_enclosure->getDispLabelColour()->setColorValue(m_labelColourPick->GetColour());
	}
}

void GUIEnclosurePanel::OnPositionXSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that default display metric positioning is used
	int value = m_elementPosXSpin->GetValue();
	m_enclosure->setPosX(value);
}

void GUIEnclosurePanel::OnPositionYSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that default display metric positioning is used
	int value = m_elementPosYSpin->GetValue();
	m_enclosure->setPosY(value);
}

void GUIEnclosurePanel::OnEnclosureStyleChoice(wxCommandEvent& WXUNUSED(event)) {
	m_enclosure->setEnclosureStyle(m_enclosureStyleBox->GetSelection() + 1);
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
}

void GUIEnclosurePanel::OnAddImagePathBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->setImage(path);
			int width = img.GetWidth();
			int height = img.GetHeight();
			m_enclosure->setBitmapWidth(width);
			m_enclosure->setBitmapHeight(height);
			m_enclosure->setWidth(width);
			m_enclosure->setHeight(height);
			m_enclosure->setMouseRectWidth(width);
			m_enclosure->setMouseRectHeight(height);
			m_enclosure->setMouseAxisStart(height);
			m_enclosure->setMouseAxisEnd(0);
			m_enclosure->setTextRectWidth(width);
			m_enclosure->setTextRectHeight(height);
			m_enclosure->setTextBreakWidth(width);
			UpdateSpinRanges();
			UpdateDefaultSpinValues();
			m_imagePathField->SetValue(m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->getRelativeImagePath());
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->getImage() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Image value is not empty! Do you want to empty it?"), wxT("Empty image value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_enclosure->getBitmapAtIndex(m_bitmapBox->GetSelection())->setImage(wxEmptyString);
				int width = 0;
				int height = 0;
				m_enclosure->setBitmapWidth(width);
				m_enclosure->setBitmapHeight(height);
				m_enclosure->setWidth(width);
				m_enclosure->setHeight(height);
				m_enclosure->setMouseRectWidth(width);
				m_enclosure->setMouseRectHeight(height);
				m_enclosure->setMouseAxisStart(height);
				m_enclosure->setMouseAxisEnd(0);
				m_enclosure->setTextRectWidth(width);
				m_enclosure->setTextRectHeight(height);
				m_enclosure->setTextBreakWidth(width);
				UpdateSpinRanges();
				UpdateDefaultSpinValues();
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
		} else {
			m_removeBitmapBtn->Disable();
			m_addImagePathBtn->Disable();
			m_addMaskPathBtn->Disable();
			UpdateBuiltinBitmapValues();
			UpdateSpinRanges();
			UpdateDefaultSpinValues();
		}
	}
}

void GUIEnclosurePanel::OnWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setWidth(m_widthSpin->GetValue());
}

void GUIEnclosurePanel::OnHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setHeight(m_heightSpin->GetValue());
}

void GUIEnclosurePanel::OnTileOffsetXSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTileOffsetX(m_tileOffsetXSpin->GetValue());
}

void GUIEnclosurePanel::OnTileOffsetYSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTileOffsetY(m_tileOffsetYSpin->GetValue());
}

void GUIEnclosurePanel::OnMouseRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseRectLeft(m_mouseRectLeftSpin->GetValue());
}

void GUIEnclosurePanel::OnMouseRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseRectTop(m_mouseRectTopSpin->GetValue());
}

void GUIEnclosurePanel::OnMouseRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseRectWidth(m_mouseRectWidthSpin->GetValue());
}

void GUIEnclosurePanel::OnMouseRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseRectHeight(m_mouseRectHeightSpin->GetValue());
}

void GUIEnclosurePanel::OnMouseAxisStartSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setMouseAxisStart(m_mouseAxisStartSpin->GetValue());
}

/*
void GUIEnclosurePanel::OnMouseAxisEndSpin(wxSpinEvent& event) {

} */

void GUIEnclosurePanel::OnTextRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextRectLeft(m_textRectLeftSpin->GetValue());
}

void GUIEnclosurePanel::OnTextRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextRectTop(m_textRectTopSpin->GetValue());
}

void GUIEnclosurePanel::OnTextRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextRectWidth(m_textRectWidthSpin->GetValue());
}

void GUIEnclosurePanel::OnTextRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextRectHeight(m_textRectHeightSpin->GetValue());
}

void GUIEnclosurePanel::OnTextBreakWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_enclosure->setTextBreakWidth(m_textBreakWidthSpin->GetValue());
}

void GUIEnclosurePanel::OnRemoveEnclosureBtn(wxCommandEvent& WXUNUSED(event)) {
	wxMessageDialog msg(this, wxT("Are you really sure you want to delete this element?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (msg.ShowModal() == wxID_YES) {
		// then remove this item from the owning panel
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void GUIEnclosurePanel::UpdateSpinRanges() {
	m_elementPosXSpin->SetRange(-1, m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - m_enclosure->getWidth());
	m_elementPosYSpin->SetRange(-1, m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() - m_enclosure->getHeight());
	m_widthSpin->SetRange(0, m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue()); // panel width!
	m_heightSpin->SetRange(0, m_enclosure->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue()); // panel height!
	m_tileOffsetXSpin->SetRange(0, m_enclosure->getBitmapWidth());
	m_tileOffsetYSpin->SetRange(0, m_enclosure->getBitmapHeight());
	m_mouseRectLeftSpin->SetRange(0, m_enclosure->getWidth());
	m_mouseRectTopSpin->SetRange(0, m_enclosure->getHeight());
	m_mouseRectWidthSpin->SetRange(0, m_enclosure->getWidth());
	m_mouseRectHeightSpin->SetRange(0, m_enclosure->getHeight());
	m_mouseAxisStartSpin->SetRange(0, m_enclosure->getMouseRectHeight());
	//m_mouseAxisEndSpin->SetRange(0, (m_enclosure->getMouseAxisStart() - m_enclosure->getMouseRectHeight()));
	m_textRectLeftSpin->SetRange(0, m_enclosure->getWidth());
	m_textRectTopSpin->SetRange(0, m_enclosure->getHeight());
	m_textRectWidthSpin->SetRange(0, m_enclosure->getWidth());
	m_textRectHeightSpin->SetRange(0, m_enclosure->getHeight());
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
	m_enclosure->setMouseAxisEnd(m_enclosure->getMouseAxisStart() - m_enclosure->getMouseRectHeight());
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
	if (defaultPath == wxEmptyString)
		defaultPath = wxStandardPaths::Get().GetDocumentsDir();

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

void GUIEnclosurePanel::UpdateBuiltinBitmapValues() {
	int width = 46;
	int height = 61;
	m_enclosure->setBitmapWidth(width);
	m_enclosure->setBitmapHeight(height);
	m_enclosure->setWidth(width);
	m_enclosure->setHeight(height);
	m_enclosure->setMouseRectWidth(width);
	m_enclosure->setMouseRectHeight(height);
	m_enclosure->setMouseAxisStart(height);
	m_enclosure->setMouseAxisEnd(0);
	m_enclosure->setTextRectWidth(width);
	m_enclosure->setTextRectHeight(height);
	m_enclosure->setTextBreakWidth(width);
}
