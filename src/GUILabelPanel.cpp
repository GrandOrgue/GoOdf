/*
 * GUILabelPanel.cpp is part of GOODF.
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

#include "GUILabelPanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>
#include <algorithm>
#include <wx/msgdlg.h>
#include <wx/filename.h>

// Event table
BEGIN_EVENT_TABLE(GUILabelPanel, wxPanel)
	EVT_TEXT(ID_GUILABELPANEL_LABEL_TEXT, GUILabelPanel::OnLabelTextChange)
	EVT_FONTPICKER_CHANGED(ID_GUILABELPANEL_FONT_PICKER, GUILabelPanel::OnLabelFontChange)
	EVT_CHOICE(ID_GUILABELPANEL_COLOR_CHOICE, GUILabelPanel::OnLabelColourChoice)
	EVT_COLOURPICKER_CHANGED(ID_GUILABELPANEL_COLOR_PICKER, GUILabelPanel::OnLabelColourPick)
	EVT_RADIOBUTTON(ID_GUILABELPANEL_FREE_X_POS_YES, GUILabelPanel::OnFreeXposRadio)
	EVT_RADIOBUTTON(ID_GUILABELPANEL_FREE_X_POS_NO, GUILabelPanel::OnFreeXposRadio)
	EVT_RADIOBUTTON(ID_GUILABELPANEL_FREE_Y_POS_YES, GUILabelPanel::OnFreeYposRadio)
	EVT_RADIOBUTTON(ID_GUILABELPANEL_FREE_Y_POS_NO, GUILabelPanel::OnFreeYposRadio)
	EVT_SPINCTRL(ID_GUILABELPANEL_DISP_X_POS_SPIN, GUILabelPanel::OnDispXposSpin)
	EVT_SPINCTRL(ID_GUILABELPANEL_DISP_Y_POS_SPIN, GUILabelPanel::OnDispYposSpin)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_X_SPIN, GUILabelPanel::OnPositionXSpin)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_Y_SPIN, GUILabelPanel::OnPositionYSpin)
	EVT_COMBOBOX(ID_GUILABELPANEL_IMAGE_NBR_BOX, GUILabelPanel::OnImageNumberChoice)
	EVT_SPINCTRL(ID_GUILABELPANEL_DRAWSTOPCOL_SPIN, GUILabelPanel::OnDrawstopColSpin)
	EVT_RADIOBUTTON(ID_GUILABELPANEL_AT_TOP_COL_YES, GUILabelPanel::OnAtTopOfDrawstopColRadio)
	EVT_RADIOBUTTON(ID_GUILABELPANEL_AT_TOP_COL_NO, GUILabelPanel::OnAtTopOfDrawstopColRadio)
	EVT_RADIOBUTTON(ID_GUILABELPANEL_SPAN_RIGHT_YES, GUILabelPanel::OnSpanDrawstopColRightRadio)
	EVT_RADIOBUTTON(ID_GUILABELPANEL_SPAN_RIGHT_NO, GUILabelPanel::OnSpanDrawstopColRightRadio)
	EVT_BUTTON(ID_GUILABELPANEL_IMAGE_BTN, GUILabelPanel::OnAddImageBtn)
	EVT_BUTTON(ID_GUILABELPANEL_MASK_BTN, GUILabelPanel::OnAddMaskBtn)
	EVT_SPINCTRL(ID_GUILABELPANEL_WIDTH_SPIN, GUILabelPanel::OnWidthSpin)
	EVT_SPINCTRL(ID_GUILABELPANEL_HEIGHT_SPIN, GUILabelPanel::OnHeightSpin)
	EVT_SPINCTRL(ID_GUILABELPANEL_TILE_X_SPIN, GUILabelPanel::OnTileOffsetXSpin)
	EVT_SPINCTRL(ID_GUILABELPANEL_TILE_Y_SPIN, GUILabelPanel::OnTileOffsetYSpin)
	EVT_SPINCTRL(ID_GUILABELPANEL_TEXT_RECT_LEFT_SPIN, GUILabelPanel::OnTextRectLeftSpin)
	EVT_SPINCTRL(ID_GUILABELPANEL_TEXT_RECT_TOP_SPIN, GUILabelPanel::OnTextRectTopSpin)
	EVT_SPINCTRL(ID_GUILABELPANEL_TEXT_RECT_WIDTH_SPIN, GUILabelPanel::OnTextRectWidthSpin)
	EVT_SPINCTRL(ID_GUILABELPANEL_TEXT_RECT_HEIGHT_SPIN, GUILabelPanel::OnTextRectHeightSpin)
	EVT_SPINCTRL(ID_GUILABELPANEL_TEXT_BREAK_SPIN, GUILabelPanel::OnTextBreakWidthSpin)
	EVT_BUTTON(ID_GUILABELPANEL_REMOVE_BTN, GUILabelPanel::OnRemoveLabelBtn)
END_EVENT_TABLE()

GUILabelPanel::GUILabelPanel(wxWindow *parent) : wxPanel(parent) {
	m_label = NULL;
	GoColor col;
	m_colors = col.getColorNames();
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *labelText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Text: ")
	);
	firstRow->Add(labelText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_labelTextField = new wxTextCtrl(
		this,
		ID_GUILABELPANEL_LABEL_TEXT,
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
		ID_GUILABELPANEL_FONT_PICKER
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
		ID_GUILABELPANEL_COLOR_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_colors
	);
	colorRow->Add(m_labelColourChoice, 1, wxEXPAND, 0);
	m_labelColourPick = new wxColourPickerCtrl(
		this,
		ID_GUILABELPANEL_COLOR_PICKER
	);
	colorRow->Add(m_labelColourPick, 1, wxEXPAND, 0);
	panelSizer->Add(colorRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *freeXText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Free X pos: ")
	);
	secondRow->Add(freeXText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_freeXposYes = new wxRadioButton(
		this,
		ID_GUILABELPANEL_FREE_X_POS_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_freeXposYes->SetValue(true);
	secondRow->Add(m_freeXposYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_freeXposNo = new wxRadioButton(
		this,
		ID_GUILABELPANEL_FREE_X_POS_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_freeXposNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *freeYText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Free Y pos: ")
	);
	secondRow->Add(freeYText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_freeYposYes = new wxRadioButton(
		this,
		ID_GUILABELPANEL_FREE_Y_POS_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_freeYposYes->SetValue(true);
	secondRow->Add(m_freeYposYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_freeYposNo = new wxRadioButton(
		this,
		ID_GUILABELPANEL_FREE_Y_POS_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_freeYposNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *dispPosRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *dispXText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispXpos: ")
	);
	dispPosRow->Add(dispXText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_dispXposSpin = new wxSpinCtrl(
		this,
		ID_GUILABELPANEL_DISP_X_POS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	dispPosRow->Add(m_dispXposSpin, 0, wxEXPAND|wxALL, 5);
	dispPosRow->AddStretchSpacer();
	wxStaticText *dispYText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispYpos: ")
	);
	dispPosRow->Add(dispYText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_dispYposSpin = new wxSpinCtrl(
		this,
		ID_GUILABELPANEL_DISP_Y_POS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	dispPosRow->Add(m_dispYposSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(dispPosRow, 0, wxGROW);

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
	positionRow->AddStretchSpacer();
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
	panelSizer->Add(positionRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imageNbrText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("DispImageNum: ")
	);
	thirdRow->Add(imageNbrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_dispImageNbrBox = new wxBitmapComboBox(
		this,
		ID_GUILABELPANEL_IMAGE_NBR_BOX
	);
	for (unsigned i = 0; i < ::wxGetApp().m_labelBitmaps.size(); i++) {
		wxString imgNumber = wxString::Format(wxT("Label %d"), i);
		m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_labelBitmaps[i]);
	}
	thirdRow->Add(m_dispImageNbrBox, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	thirdRow->AddStretchSpacer();
	wxStaticText *stopColText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispDrawstopCol: ")
	);
	thirdRow->Add(stopColText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopColSpin = new wxSpinCtrl(
		this,
		ID_GUILABELPANEL_DRAWSTOPCOL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	thirdRow->Add(m_drawstopColSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *atTopText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("At top of drawstop columns: ")
	);
	fourthRow->Add(atTopText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_atTopOfDrawstopColYes = new wxRadioButton(
		this,
		ID_GUILABELPANEL_AT_TOP_COL_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	fourthRow->Add(m_atTopOfDrawstopColYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_atTopOfDrawstopColNo = new wxRadioButton(
		this,
		ID_GUILABELPANEL_AT_TOP_COL_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_atTopOfDrawstopColNo->SetValue(true);
	fourthRow->Add(m_atTopOfDrawstopColNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	fourthRow->AddStretchSpacer();
	wxStaticText *spanRightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Span column to right: ")
	);
	fourthRow->Add(spanRightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_spanDrawstopColToRightYes = new wxRadioButton(
		this,
		ID_GUILABELPANEL_SPAN_RIGHT_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	fourthRow->Add(m_spanDrawstopColToRightYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_spanDrawstopColToRightNo = new wxRadioButton(
		this,
		ID_GUILABELPANEL_SPAN_RIGHT_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_spanDrawstopColToRightNo->SetValue(true);
	fourthRow->Add(m_spanDrawstopColToRightNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxEXPAND);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imageOnText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Image: ")
	);
	fifthRow->Add(imageOnText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_imagePathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fifthRow->Add(m_imagePathField, 1, wxEXPAND|wxALL, 5);
	m_addImageBtn = new wxButton(
		this,
		ID_GUILABELPANEL_IMAGE_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	fifthRow->Add(m_addImageBtn, 0, wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxEXPAND);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *maskText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Mask: ")
	);
	sixthRow->Add(maskText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maskPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	sixthRow->Add(m_maskPathField, 1, wxEXPAND|wxALL, 5);
	m_addMaskBtn = new wxButton(
		this,
		ID_GUILABELPANEL_MASK_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	sixthRow->Add(m_addMaskBtn, 0, wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxEXPAND);

	wxBoxSizer *seventhRow  = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *widthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Width: ")
	);
	seventhRow->Add(widthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_widthSpin = new wxSpinCtrl(
		this,
		ID_GUILABELPANEL_WIDTH_SPIN,
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
		ID_GUILABELPANEL_HEIGHT_SPIN,
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
		ID_GUILABELPANEL_TILE_X_SPIN,
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
		ID_GUILABELPANEL_TILE_Y_SPIN,
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

	wxBoxSizer *eleventh = new wxBoxSizer(wxHORIZONTAL);
	eleventh->AddStretchSpacer();
	wxStaticText *textBreakText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextBreakWidth: ")
	);
	eleventh->Add(textBreakText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textBreakWidthSpin = new wxSpinCtrl(
		this,
		ID_GUILABELPANEL_TEXT_BREAK_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eleventh->Add(m_textBreakWidthSpin, 0, wxEXPAND|wxALL, 5);
	eleventh->AddStretchSpacer();
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
		ID_GUILABELPANEL_TEXT_RECT_LEFT_SPIN,
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
		ID_GUILABELPANEL_TEXT_RECT_TOP_SPIN,
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
		ID_GUILABELPANEL_TEXT_RECT_WIDTH_SPIN,
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
		ID_GUILABELPANEL_TEXT_RECT_HEIGHT_SPIN,
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
	m_removeLabelBtn = new wxButton(
		this,
		ID_GUILABELPANEL_REMOVE_BTN,
		wxT("Remove this element")
	);
	bottomRow->Add(m_removeLabelBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

GUILabelPanel::~GUILabelPanel() {

}

void GUILabelPanel::setLabel(GUILabel *label) {
	m_label = label;
	m_labelTextField->SetValue(m_label->getName());
	m_labelFont->SetSelectedFont(m_label->getDispLabelFont());
	if (m_label->getDispLabelColour()->getSelectedColorIndex() == 0) {
		// it's a custom color
		m_labelColourChoice->SetSelection(0);
		m_labelColourPick->Enable();
		m_labelColourPick->SetColour(m_label->getDispLabelColour()->getColor());
	} else {
		// the color is text specified from the available ones in GO
		m_labelColourChoice->SetSelection(m_label->getDispLabelColour()->getSelectedColorIndex());
		m_labelColourPick->SetColour(m_label->getDispLabelColour()->getColor());
		m_labelColourPick->Disable();
	}
	if (m_label->isFreeXPlacement()) {
		m_dispXposSpin->Enable();
		m_elementPosXSpin->Enable();
		m_spanDrawstopColToRightYes->Disable();
		m_spanDrawstopColToRightNo->Disable();
		m_drawstopColSpin->Disable();
	} else {
		m_dispXposSpin->Disable();
		m_elementPosXSpin->Disable();
		m_spanDrawstopColToRightYes->Enable();
		m_spanDrawstopColToRightNo->Enable();
		m_drawstopColSpin->Enable();
	}

	if (m_label->isFreeYPlacement()) {
		m_dispYposSpin->Enable();
		m_elementPosYSpin->Enable();
		m_atTopOfDrawstopColYes->Disable();
		m_atTopOfDrawstopColNo->Disable();
	} else {
		m_dispYposSpin->Disable();
		m_elementPosYSpin->Disable();
		m_atTopOfDrawstopColYes->Enable();
		m_atTopOfDrawstopColNo->Enable();
	}
	m_dispImageNbrBox->Clear();
	for (unsigned i = 0; i < ::wxGetApp().m_labelBitmaps.size(); i++) {
		wxString imgNumber = wxString::Format(wxT(" %d "), i);
		m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_labelBitmaps[i]);
	}
	m_dispImageNbrBox->SetSelection(m_label->getDispImageNum());
	if (m_label->isDispAtTopOfDrawstopCol()) {
		m_atTopOfDrawstopColYes->SetValue(true);
	} else {
		m_atTopOfDrawstopColNo->SetValue(true);
	}

	if (m_label->isDispSpanDrawstopColToRight()) {
		m_spanDrawstopColToRightYes->SetValue(true);
	} else {
		m_spanDrawstopColToRightNo->SetValue(true);
	}
	if (m_label->getImage()->getImage() != wxEmptyString) {
		m_imagePathField->SetValue(m_label->getImage()->getImageNameOnly());
		m_addMaskBtn->Enable();
		m_dispImageNbrBox->Disable();
	} else {
		m_imagePathField->SetValue(wxEmptyString);
		m_addMaskBtn->Disable();
		m_dispImageNbrBox->Enable();
	}

	if (m_label->getImage()->getMask() != wxEmptyString) {
		m_maskPathField->SetValue(m_label->getImage()->getRelativeMaskPath());
	} else {
		m_maskPathField->SetValue(wxEmptyString);
	}

	UpdateSpinRanges();
	UpdateDefaultSpinValues();
}

void GUILabelPanel::OnLabelTextChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_labelTextField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_labelTextField);
	m_label->setName(m_labelTextField->GetValue());
}

void GUILabelPanel::OnLabelFontChange(wxFontPickerEvent& WXUNUSED(event)) {
	m_label->setDispLabelFont(m_labelFont->GetSelectedFont());
	m_label->setDispLabelFontSize(m_labelFont->GetFont().GetPointSize());
}

void GUILabelPanel::OnLabelColourChoice(wxCommandEvent& event) {
	if (event.GetId() == ID_GUILABELPANEL_COLOR_CHOICE) {
		if (m_labelColourChoice->GetSelection() == 0) {
			m_labelColourPick->Enable();
		} else {
			m_label->getDispLabelColour()->setSelectedColorIndex(m_labelColourChoice->GetSelection());
			m_labelColourPick->SetColour(m_label->getDispLabelColour()->getColor());
			m_labelColourPick->Disable();
		}
	}
}

void GUILabelPanel::OnLabelColourPick(wxColourPickerEvent& event) {
	if (event.GetId() == ID_GUILABELPANEL_COLOR_PICKER) {
		m_label->getDispLabelColour()->setColorValue(m_labelColourPick->GetColour());
	}
}

void GUILabelPanel::OnFreeXposRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUILABELPANEL_FREE_X_POS_YES) {
		m_freeXposYes->SetValue(true);
		m_label->setFreeXPlacement(true);
		m_dispXposSpin->Enable();
		m_elementPosXSpin->Enable();
		m_spanDrawstopColToRightYes->Disable();
		m_spanDrawstopColToRightNo->Disable();
		m_drawstopColSpin->Disable();
	} else {
		m_freeXposNo->SetValue(true);
		m_label->setFreeXPlacement(false);
		m_dispXposSpin->Disable();
		m_elementPosXSpin->Disable();
		m_spanDrawstopColToRightYes->Enable();
		m_spanDrawstopColToRightNo->Enable();
		m_drawstopColSpin->Enable();
	}
}

void GUILabelPanel::OnFreeYposRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUILABELPANEL_FREE_Y_POS_YES) {
		m_freeYposYes->SetValue(true);
		m_label->setFreeYPlacement(true);
		m_dispYposSpin->Enable();
		m_elementPosYSpin->Enable();
		m_atTopOfDrawstopColYes->Disable();
		m_atTopOfDrawstopColNo->Disable();
	} else  {
		m_freeYposNo->SetValue(true);
		m_label->setFreeYPlacement(false);
		m_dispYposSpin->Disable();
		m_elementPosYSpin->Disable();
		m_atTopOfDrawstopColYes->Enable();
		m_atTopOfDrawstopColNo->Enable();
	}
}

void GUILabelPanel::OnImageNumberChoice(wxCommandEvent& WXUNUSED(event)) {
	int selected = m_dispImageNbrBox->GetSelection();
	m_label->setDispImageNum(selected);
	// we also update sizes depending on the selection
	// index 0, 1, 3, 7, 10 size is 80 x 25
	// index 2, 6, 9 size is 80 x 50
	// index 4, 8, 11 size is 160 x 25
	// index 5, 12 size is 200 x 50
	if (selected == 0 || selected == 1 || selected == 3 || selected == 7 || selected == 10) {
		m_label->setBitmapWidth(80);
		m_label->setBitmapHeight(25);
		m_label->setWidth(80);
		m_label->setHeight(25);
		m_label->setTextRectWidth(80);
		m_label->setTextRectHeight(25);
		m_label->setTextBreakWidth(80);
	} else if (selected == 2 || selected == 6 || selected == 9) {
		m_label->setBitmapWidth(80);
		m_label->setBitmapHeight(50);
		m_label->setWidth(80);
		m_label->setHeight(50);
		m_label->setTextRectWidth(80);
		m_label->setTextRectHeight(50);
		m_label->setTextBreakWidth(80);
	} else if (selected == 4 || selected == 8 || selected == 11) {
		m_label->setBitmapWidth(160);
		m_label->setBitmapHeight(25);
		m_label->setWidth(160);
		m_label->setHeight(25);
		m_label->setTextRectWidth(160);
		m_label->setTextRectHeight(25);
		m_label->setTextBreakWidth(160);
	} else if (selected == 5 || selected == 12) {
		m_label->setBitmapWidth(200);
		m_label->setBitmapHeight(50);
		m_label->setWidth(200);
		m_label->setHeight(50);
		m_label->setTextRectWidth(200);
		m_label->setTextRectHeight(50);
		m_label->setTextBreakWidth(200);
	}
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
}

void GUILabelPanel::OnDrawstopColSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setDispDrawstopCol(m_drawstopColSpin->GetValue());
}

void GUILabelPanel::OnAtTopOfDrawstopColRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUILABELPANEL_AT_TOP_COL_YES) {
		m_label->setDispAtTopOfDrawstopCol(true);
	} else {
		m_label->setDispAtTopOfDrawstopCol(false);
	}
}

void GUILabelPanel::OnSpanDrawstopColRightRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUILABELPANEL_SPAN_RIGHT_YES) {
		m_label->setDispSpanDrawstopColToRight(true);
	} else {
		m_label->setDispSpanDrawstopColToRight(false);
	}
}

void GUILabelPanel::OnAddImageBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			m_label->getImage()->setImage(path);
			int width = img.GetWidth();
			int height = img.GetHeight();
			m_label->setBitmapWidth(width);
			m_label->setBitmapHeight(height);
			m_label->setWidth(width);
			m_label->setHeight(height);
			m_label->setTextRectWidth(width);
			m_label->setTextRectHeight(height);
			m_label->setTextBreakWidth(width);
			UpdateSpinRanges();
			UpdateDefaultSpinValues();
			m_imagePathField->SetValue(m_label->getImage()->getRelativeImagePath());
			m_addMaskBtn->Enable();
			m_dispImageNbrBox->Disable();
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_label->getImage()->getImage() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Image value is not empty! Do you want to empty it?"), wxT("Empty image value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_label->getImage()->setImage(wxEmptyString);
				int width = 0;
				int height = 0;
				m_label->setBitmapWidth(width);
				m_label->setBitmapHeight(height);
				m_label->setWidth(width);
				m_label->setHeight(height);
				m_label->setTextRectWidth(width);
				m_label->setTextRectHeight(height);
				m_label->setTextBreakWidth(width);
				UpdateSpinRanges();
				UpdateDefaultSpinValues();
				m_imagePathField->SetValue(wxEmptyString);
				// if image is removed we also remove mask and disable it
				if (m_label->getImage()->getMask() != wxEmptyString) {
					m_label->getImage()->setMask(wxEmptyString);
					m_maskPathField->SetValue(wxEmptyString);
				}
				m_maskPathField->Disable();
				m_dispImageNbrBox->Enable();
			}
		}
	}
}

void GUILabelPanel::OnAddMaskBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (width == m_label->getBitmapWidth() && height == m_label->getBitmapHeight()) {
				m_label->getImage()->setMask(path);
				m_maskPathField->SetValue(m_label->getImage()->getMaskNameOnly());
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_label->getImage()->getMask() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Mask value is not empty! Do you want to empty it?"), wxT("Empty mask value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_label->getImage()->setMask(wxEmptyString);
				m_maskPathField->SetValue(wxEmptyString);
			}
		}
	}
}

void GUILabelPanel::OnWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	int value = m_widthSpin->GetValue();
	m_label->setWidth(value);

	// if width is changed TextRectWidth and TextBreakWidth spinctrls should update their max values
	int textRectValue = m_textRectWidthSpin->GetValue();
	m_textRectWidthSpin->SetRange(0, value);
	if (textRectValue > value) {
		m_label->setTextRectWidth(value);
		m_textRectWidthSpin->SetValue(value);
	}
	int textBreakValue = m_textBreakWidthSpin->GetValue();
	m_textBreakWidthSpin->SetRange(0, value);
	if (textBreakValue > value) {
		m_label->setTextBreakWidth(value);
		m_textBreakWidthSpin->SetValue(value);
	}
}

void GUILabelPanel::OnHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	int value = m_heightSpin->GetValue();
	m_label->setHeight(value);

	// if height is changed TextRectHeight spin should update max values
	int textHeightValue = m_textRectHeightSpin->GetValue();
	m_textRectHeightSpin->SetRange(0, value);
	if (textHeightValue > value) {
		m_label->setTextRectHeight(value);
		m_textRectHeightSpin->SetValue(value);
	}
}

void GUILabelPanel::OnTileOffsetXSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setTileOffsetX(m_tileOffsetXSpin->GetValue());
}

void GUILabelPanel::OnTileOffsetYSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setTileOffsetY(m_tileOffsetYSpin->GetValue());
}

void GUILabelPanel::OnTextRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setTextRectLeft(m_textRectLeftSpin->GetValue());
}

void GUILabelPanel::OnTextRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setTextRectTop(m_textRectTopSpin->GetValue());
}

void GUILabelPanel::OnTextRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setTextRectWidth(m_textRectWidthSpin->GetValue());
}

void GUILabelPanel::OnTextRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setTextRectHeight(m_textRectHeightSpin->GetValue());
}

void GUILabelPanel::OnTextBreakWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setTextBreakWidth(m_textBreakWidthSpin->GetValue());
}

void GUILabelPanel::OnPositionXSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that dispXpos values are used
	// if it's not -1 then dispXpos should be disabled to indicate that it's no longer used
	int value = m_elementPosXSpin->GetValue();
	m_label->setPosX(value);
	if (value != -1) {
		m_dispXposSpin->Disable();
	} else {
		m_dispXposSpin->Enable();
	}
}

void GUILabelPanel::OnPositionYSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that dispYpos values are used
	// if it's not -1 then dispYpos should be disabled to indicate that it's not used
	int value = m_elementPosYSpin->GetValue();
	m_label->setPosY(value);
	if (value != -1) {
		m_dispYposSpin->Disable();
	} else {
		m_dispYposSpin->Enable();
	}
}

void GUILabelPanel::OnDispXposSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setDispXpos(m_dispXposSpin->GetValue());
}

void GUILabelPanel::OnDispYposSpin(wxSpinEvent& WXUNUSED(event)) {
	m_label->setDispYpos(m_dispYposSpin->GetValue());
}

void GUILabelPanel::OnRemoveLabelBtn(wxCommandEvent& WXUNUSED(event)) {
	wxMessageDialog msg(this, wxT("Are you really sure you want to delete this element?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (msg.ShowModal() == wxID_YES) {
		// then remove this item from the owning panel
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void GUILabelPanel::UpdateSpinRanges() {
	m_dispXposSpin->SetRange(-1, m_label->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - m_label->getWidth());
	m_dispYposSpin->SetRange(-1, m_label->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() - m_label->getHeight());
	m_elementPosXSpin->SetRange(-1, m_label->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - m_label->getWidth());
	m_elementPosYSpin->SetRange(-1, m_label->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() - m_label->getHeight());
	m_widthSpin->SetRange(0, m_label->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue()); // panel width!
	m_heightSpin->SetRange(0, m_label->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue()); // panel height!
	m_tileOffsetXSpin->SetRange(0, m_label->getBitmapWidth());
	m_tileOffsetYSpin->SetRange(0, m_label->getBitmapHeight());
	m_textRectLeftSpin->SetRange(0, m_label->getWidth());
	m_textRectTopSpin->SetRange(0, m_label->getHeight());
	m_textRectWidthSpin->SetRange(0, m_label->getWidth());
	m_textRectHeightSpin->SetRange(0, m_label->getHeight());
	m_textBreakWidthSpin->SetRange(0, m_label->getTextRectWidth());

	// TODO: The ranges should be checked so that extra button and drawstop rows/cols be accounted for some start at 100!
	m_drawstopColSpin->SetRange(1, m_label->getOwningPanel()->getDisplayMetrics()->m_dispDrawstopCols);
}

void GUILabelPanel::UpdateDefaultSpinValues() {
	m_drawstopColSpin->SetValue(m_label->getDispDrawstopCol());
	m_elementPosXSpin->SetValue(m_label->getPosX());
	m_elementPosYSpin->SetValue(m_label->getPosY());
	m_widthSpin->SetValue(m_label->getWidth());
	m_heightSpin->SetValue(m_label->getHeight());
	m_tileOffsetXSpin->SetValue(m_label->getTileOffsetX());
	m_tileOffsetYSpin->SetValue(m_label->getTileOffsetY());
	m_textRectLeftSpin->SetValue(m_label->getTextRectLeft());
	m_textRectTopSpin->SetValue(m_label->getTextRectTop());
	m_textRectWidthSpin->SetValue(m_label->getTextRectWidth());
	m_textRectHeightSpin->SetValue(m_label->getTextRectHeight());
	m_textBreakWidthSpin->SetValue(m_label->getTextBreakWidth());
}

wxString GUILabelPanel::GetPathForImageFile() {
	wxString imageFilePath;
	wxString defaultPath = wxEmptyString;
	if (m_label->getImage()->getImage() != wxEmptyString) {
		wxFileName filePath = wxFileName(m_label->getImage()->getImage());
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
		"PNG files (*.png)|*.png|BMP and GIF files (*.bmp;*.gif)|*.bmp;*.gif|JPG files (*.jpg)|*.jpg|ICO files (*.ico)|*.ico",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return wxEmptyString;

	imageFilePath = fileDialog.GetPath();
	return imageFilePath;
}
