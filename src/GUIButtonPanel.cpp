/*
 * GUIButtonPanel.cpp is part of GoOdf.
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

#include "GUIButtonPanel.h"
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>
#include <algorithm>
#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/fontdlg.h>
#include <wx/fontdata.h>

// Event table
BEGIN_EVENT_TABLE(GUIButtonPanel, wxPanel)
	EVT_TEXT(ID_GUIBUTTONPANEL_LABEL_TEXT, GUIButtonPanel::OnLabelTextChange)
	EVT_BUTTON(ID_GUIBUTTONPANEL_FONT_PICKER, GUIButtonPanel::OnLabelFontChange)
	EVT_CHOICE(ID_GUIBUTTONPANEL_COLOR_CHOICE, GUIButtonPanel::OnLabelColourChoice)
	EVT_COLOURPICKER_CHANGED(ID_GUIBUTTONPANEL_COLOR_PICKER, GUIButtonPanel::OnLabelColourPick)
	EVT_RADIOBUTTON(ID_GUIBUTTONPANEL_DISP_AS_PISTON_YES, GUIButtonPanel::OnDisplayAsPistonRadio)
	EVT_RADIOBUTTON(ID_GUIBUTTONPANEL_DISP_AS_PISTON_NO, GUIButtonPanel::OnDisplayAsPistonRadio)
	EVT_RADIOBUTTON(ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_YES, GUIButtonPanel::OnDisplayKeyLabelLeftRadio)
	EVT_RADIOBUTTON(ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_NO, GUIButtonPanel::OnDisplayKeyLabelLeftRadio)
	EVT_COMBOBOX(ID_GUIBUTTONPANEL_IMAGE_NBR_BOX, GUIButtonPanel::OnImageNumberChoice)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_BUTTONROW_SPIN, GUIButtonPanel::OnButtonRowSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_BUTTONCOL_SPIN, GUIButtonPanel::OnButtonColSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_DRAWSTOPROW_SPIN, GUIButtonPanel::OnDrawstopRowSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_DRAWSTOPCOL_SPIN, GUIButtonPanel::OnDrawstopColSpin)
	EVT_BUTTON(ID_GUIBUTTONPANEL_IMAGE_ON_BTN, GUIButtonPanel::OnAddImageOnBtn)
	EVT_BUTTON(ID_GUIBUTTONPANEL_IMAGE_OFF_BTN, GUIButtonPanel::OnAddImageOffBtn)
	EVT_BUTTON(ID_GUIBUTTONPANEL_MASK_ON_BTN, GUIButtonPanel::OnAddMaskOnBtn)
	EVT_BUTTON(ID_GUIBUTTONPANEL_MASK_OFF_BTN, GUIButtonPanel::OnAddMaskOffBtn)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_WIDTH_SPIN, GUIButtonPanel::OnWidthSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_HEIGHT_SPIN, GUIButtonPanel::OnHeightSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TILE_X_SPIN, GUIButtonPanel::OnTileOffsetXSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TILE_Y_SPIN, GUIButtonPanel::OnTileOffsetYSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RECT_LEFT_SPIN, GUIButtonPanel::OnMouseRectLeftSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RECT_TOP_SPIN, GUIButtonPanel::OnMouseRectTopSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RECT_WIDTH_SPIN, GUIButtonPanel::OnMouseRectWidthSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RECT_HEIGHT_SPIN, GUIButtonPanel::OnMouseRectHeightSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_MOUSE_RADIUS_SPIN, GUIButtonPanel::OnMouseRadiusSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_RECT_LEFT_SPIN, GUIButtonPanel::OnTextRectLeftSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_RECT_TOP_SPIN, GUIButtonPanel::OnTextRectTopSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_RECT_WIDTH_SPIN, GUIButtonPanel::OnTextRectWidthSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_RECT_HEIGHT_SPIN, GUIButtonPanel::OnTextRectHeightSpin)
	EVT_SPINCTRL(ID_GUIBUTTONPANEL_TEXT_BREAK_SPIN, GUIButtonPanel::OnTextBreakWidthSpin)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_X_SPIN, GUIButtonPanel::OnPositionXSpin)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_Y_SPIN, GUIButtonPanel::OnPositionYSpin)
	EVT_BUTTON(ID_GUIBUTTONPANEL_REMOVE_BTN, GUIButtonPanel::OnRemoveButtonBtn)
END_EVENT_TABLE()

GUIButtonPanel::GUIButtonPanel(wxWindow *parent) : wxPanel(parent) {
	m_button = NULL;
	GoColor col;
	m_colors = col.getColorNames();
	m_lastUsedImagePath = wxEmptyString;
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
		ID_GUIBUTTONPANEL_LABEL_TEXT,
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
	m_labelFont = new wxButton(
		this,
		ID_GUIBUTTONPANEL_FONT_PICKER
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
		ID_GUIBUTTONPANEL_COLOR_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_colors
	);
	colorRow->Add(m_labelColourChoice, 1, wxEXPAND, 0);
	m_labelColourPick = new wxColourPickerCtrl(
		this,
		ID_GUIBUTTONPANEL_COLOR_PICKER
	);
	colorRow->Add(m_labelColourPick, 1, wxEXPAND, 0);
	panelSizer->Add(colorRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *pistonText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Display as piston: ")
	);
	secondRow->Add(pistonText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayAsPistonYes = new wxRadioButton(
		this,
		ID_GUIBUTTONPANEL_DISP_AS_PISTON_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_displayAsPistonYes->SetValue(false);
	secondRow->Add(m_displayAsPistonYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayAsPistonNo = new wxRadioButton(
		this,
		ID_GUIBUTTONPANEL_DISP_AS_PISTON_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_displayAsPistonNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *keyLabelLeftText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispKeyLabelOnLeft: ")
	);
	secondRow->Add(keyLabelLeftText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayKeyLabelLeftYes = new wxRadioButton(
		this,
		ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_displayKeyLabelLeftYes->SetValue(true);
	secondRow->Add(m_displayKeyLabelLeftYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayKeyLabelLeftNo = new wxRadioButton(
		this,
		ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_displayKeyLabelLeftNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(secondRow, 0, wxGROW);

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
		ID_GUIBUTTONPANEL_IMAGE_NBR_BOX
	);
	for (unsigned i = 0; i < ::wxGetApp().m_scaledDrawstopBitmaps.size(); i++) {
		wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
		m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_scaledDrawstopBitmaps[i]);
	}
	thirdRow->Add(m_dispImageNbrBox, 1, wxEXPAND|wxALL, 5);
	wxStaticText *btnRowText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispButtonRow: ")
	);
	thirdRow->Add(btnRowText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_buttonRowSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_BUTTONROW_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	thirdRow->Add(m_buttonRowSpin, 0, wxEXPAND|wxALL, 5);
	wxStaticText *btnColText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispButtonCol: ")
	);
	thirdRow->Add(btnColText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_buttonColSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_BUTTONCOL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	thirdRow->Add(m_buttonColSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *stopRowText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispDrawstopRow: ")
	);
	fourthRow->Add(stopRowText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopRowSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_DRAWSTOPROW_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	fourthRow->Add(m_drawstopRowSpin, 0, wxEXPAND|wxALL, 5);
	fourthRow->AddStretchSpacer();
	wxStaticText *stopColText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DispDrawstopCol: ")
	);
	fourthRow->Add(stopColText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_drawstopColSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_DRAWSTOPCOL_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	fourthRow->Add(m_drawstopColSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxEXPAND);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imageOnText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("ImageOn: ")
	);
	fifthRow->Add(imageOnText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_imageOnPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fifthRow->Add(m_imageOnPathField, 1, wxEXPAND|wxALL, 5);
	m_addImageOnBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_IMAGE_ON_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	fifthRow->Add(m_addImageOnBtn, 0, wxALL, 5);
	wxStaticText *imageOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("ImageOff: ")
	);
	fifthRow->Add(imageOffText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_imageOffPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fifthRow->Add(m_imageOffPathField, 1, wxEXPAND|wxALL, 5);
	m_addImageOffBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_IMAGE_OFF_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	fifthRow->Add(m_addImageOffBtn, 0, wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxEXPAND);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *maskOnText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MaskOn: ")
	);
	sixthRow->Add(maskOnText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maskOnPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	sixthRow->Add(m_maskOnPathField, 1, wxEXPAND|wxALL, 5);
	m_addMaskOnBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_MASK_ON_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	sixthRow->Add(m_addMaskOnBtn, 0, wxALL, 5);
	wxStaticText *maskOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MaskOff: ")
	);
	sixthRow->Add(maskOffText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maskOffPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	sixthRow->Add(m_maskOffPathField, 1, wxEXPAND|wxALL, 5);
	m_addMaskOffBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_MASK_OFF_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	sixthRow->Add(m_addMaskOffBtn, 0, wxALL, 5);
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
		ID_GUIBUTTONPANEL_WIDTH_SPIN,
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
		ID_GUIBUTTONPANEL_HEIGHT_SPIN,
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
		ID_GUIBUTTONPANEL_TILE_X_SPIN,
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
		ID_GUIBUTTONPANEL_TILE_Y_SPIN,
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
		ID_GUIBUTTONPANEL_MOUSE_RECT_LEFT_SPIN,
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
		ID_GUIBUTTONPANEL_MOUSE_RECT_TOP_SPIN,
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
		ID_GUIBUTTONPANEL_MOUSE_RECT_WIDTH_SPIN,
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
		ID_GUIBUTTONPANEL_MOUSE_RECT_HEIGHT_SPIN,
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
	wxStaticText *mouseRadiusText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRadius: ")
	);
	eleventh->Add(mouseRadiusText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRadiusSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_MOUSE_RADIUS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eleventh->Add(m_mouseRadiusSpin, 0, wxEXPAND|wxALL, 5);
	eleventh->AddStretchSpacer();
	wxStaticText *textBreakText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("TextBreakWidth: ")
	);
	eleventh->Add(textBreakText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_textBreakWidthSpin = new wxSpinCtrl(
		this,
		ID_GUIBUTTONPANEL_TEXT_BREAK_SPIN,
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
		ID_GUIBUTTONPANEL_TEXT_RECT_LEFT_SPIN,
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
		ID_GUIBUTTONPANEL_TEXT_RECT_TOP_SPIN,
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
		ID_GUIBUTTONPANEL_TEXT_RECT_WIDTH_SPIN,
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
		ID_GUIBUTTONPANEL_TEXT_RECT_HEIGHT_SPIN,
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
	m_removeButtonBtn = new wxButton(
		this,
		ID_GUIBUTTONPANEL_REMOVE_BTN,
		wxT("Remove this element")
	);
	bottomRow->Add(m_removeButtonBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

GUIButtonPanel::~GUIButtonPanel() {

}

void GUIButtonPanel::setButton(GUIButton *button) {
	m_button = button;
	m_labelTextField->ChangeValue(m_button->getDispLabelText());
	m_labelFont->SetFont(m_button->getDispLabelFont());
	m_labelFont->SetLabel(m_button->getDispLabelFont().GetFaceName() + wxString::Format(wxT(" %i"), m_button->getDispLabelFontSize()->getSizeValue()));
	if (m_button->getDispLabelColour()->getSelectedColorIndex() == 0) {
		// it's a custom color
		m_labelColourChoice->SetSelection(0);
		m_labelColourPick->Enable();
		m_labelColourPick->SetColour(m_button->getDispLabelColour()->getColor());
	} else {
		// the color is text specified from the available ones in GO
		m_labelColourChoice->SetSelection(m_button->getDispLabelColour()->getSelectedColorIndex());
		m_labelColourPick->SetColour(m_button->getDispLabelColour()->getColor());
		m_labelColourPick->Disable();
	}
	if (m_button->isDisplayAsPiston()) {
		m_displayAsPistonYes->SetValue(true);
		m_displayAsPistonNo->SetValue(false);
		m_buttonRowSpin->Enable();
		m_buttonColSpin->Enable();
		m_drawstopRowSpin->Disable();
		m_drawstopColSpin->Disable();
	} else {
		m_displayAsPistonYes->SetValue(false);
		m_displayAsPistonNo->SetValue(true);
		m_buttonRowSpin->Disable();
		m_buttonColSpin->Disable();
		m_drawstopRowSpin->Enable();
		m_drawstopColSpin->Enable();
	}
	SetupImageNbrBoxContent();
	if (m_button->isDispKeyLabelOnLeft()) {
		m_displayKeyLabelLeftYes->SetValue(true);
		m_displayKeyLabelLeftNo->SetValue(false);
	} else {
		m_displayKeyLabelLeftYes->SetValue(false);
		m_displayKeyLabelLeftNo->SetValue(true);
	}
	m_dispImageNbrBox->SetSelection(m_button->getDispImageNum() - 1);
	m_buttonRowSpin->SetValue(m_button->getDispButtonRow());
	m_buttonColSpin->SetValue(m_button->getDispButtonCol());
	m_drawstopRowSpin->SetValue(m_button->getDispDrawstopRow());
	m_drawstopColSpin->SetValue(m_button->getDispDrawstopCol());
	if (m_button->getImageOn() != wxEmptyString) {
		wxString relativeImageOn = GOODF_functions::removeBaseOdfPath(m_button->getImageOn());
		m_imageOnPathField->SetValue(relativeImageOn);
	} else {
		m_imageOnPathField->SetValue(wxEmptyString);
		m_addMaskOnBtn->Disable();
	}
	if (m_button->getImageOff() != wxEmptyString) {
		wxString relativeImageOff = GOODF_functions::removeBaseOdfPath(m_button->getImageOff());
		m_imageOffPathField->SetValue(relativeImageOff);
	} else {
		m_imageOffPathField->SetValue(wxEmptyString);
		m_addMaskOffBtn->Disable();
	}
	if (m_button->getMaskOn() != wxEmptyString) {
		wxString relativeMaskOn = GOODF_functions::removeBaseOdfPath(m_button->getMaskOn());
		m_maskOnPathField->SetValue(relativeMaskOn);
	} else {
		m_maskOnPathField->SetValue(wxEmptyString);
	}
	if (m_button->getMaskOff() != wxEmptyString) {
		wxString relativeMaskOff = GOODF_functions::removeBaseOdfPath(m_button->getMaskOff());
		m_maskOffPathField->SetValue(relativeMaskOff);
	} else {
		m_maskOffPathField->SetValue(wxEmptyString);
	}
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
}

void GUIButtonPanel::updatePositionValues() {
	if (m_button) {
		m_elementPosXSpin->SetValue(m_button->getPosX());
		m_elementPosYSpin->SetValue(m_button->getPosY());
	}
}

void GUIButtonPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_labelTextField->SetToolTip(wxT("If another string is desired than the button name (or a shorter one for display reasons) it can be overridden here. NOTE: Leading spaces and usage of semicolon (;) is prohibited and any trailing spaces will be removed when organ is saved!"));
		m_labelFont->SetToolTip(wxT("Font face and size can be set here. The default value is derived from the display metrics control label font of the panel."));
		m_labelColourChoice->SetToolTip(wxT("Colour of the font can be chosen here. If a custom value is chosen the picker will be activated."));
		m_labelColourPick->SetToolTip(wxT("A custom colour can be picked here."));
		m_displayAsPistonYes->SetToolTip(wxT("Display element as piston."));
		m_displayAsPistonNo->SetToolTip(wxT("Display element as drawstop."));
		m_displayKeyLabelLeftYes->SetToolTip(wxT("Not used by GrandOrgue!"));
		m_displayKeyLabelLeftNo->SetToolTip(wxT("Not used by GrandOrgue!"));
		m_elementPosXSpin->SetToolTip(wxT("Absolute positioning x coordinate value. If set to -1 the built-in layout with rows/columns will be used instead."));
		m_elementPosYSpin->SetToolTip(wxT("Absolute positioning x coordinate value. If set to -1 the built-in layout with rows/columns will be used instead."));
		m_dispImageNbrBox->SetToolTip(wxT("Choose the built-in bitmap for the element here."));
		m_buttonRowSpin->SetToolTip(wxT("If element is displayed as piston use this value (assuming posY is -1) to determine position."));
		m_buttonColSpin->SetToolTip(wxT("If element is displayed as piston use this value (assuming posX is -1) to determine position."));
		m_drawstopRowSpin->SetToolTip(wxT("If element is displayed as drawstop use this value (assuming posY is -1) to determine position."));
		m_drawstopColSpin->SetToolTip(wxT("If element is displayed as drawstop use this value (assuming posX is -1) to determine position."));
		m_imageOnPathField->SetToolTip(wxT("Relative file path will be displayed here. Use 'Browse...' button to set."));
		m_addImageOnBtn->SetToolTip(wxT("Opens a file dialog where the image 'On' bitmap can be chosen. The .png format is the preferred format for images in GrandOrgue. NOTE: All added bitmaps for the element must have the same pixel size!"));
		m_imageOffPathField->SetToolTip(wxT("Relative file path will be displayed here. Use 'Browse...' button to set."));
		m_addImageOffBtn->SetToolTip(wxT("Opens a file dialog where the image 'Off' bitmap can be chosen. The .png format is the preferred format for images in GrandOrgue. NOTE: All added bitmaps for the element must have the same pixel size!"));
		m_maskOnPathField->SetToolTip(wxT("Relative file path will be displayed here. Use 'Browse...' button to set."));
		m_addMaskOnBtn->SetToolTip(wxT("Opens a file dialog where a mask for the image 'On' bitmap can be chosen. NOTE: All added bitmaps for the element must have the same pixel size!"));
		m_maskOffPathField->SetToolTip(wxT("Relative file path will be displayed here. Use 'Browse...' button to set."));
		m_addMaskOffBtn->SetToolTip(wxT("Opens a file dialog where a mask for the image 'Off' bitmap can be chosen. NOTE: All added bitmaps for the element must have the same pixel size!"));
		m_widthSpin->SetToolTip(wxT("Width of bitmap to use. NOTE: Changing this value does not scale the image! If set greater than real image width - the shown image in GrandOrgue will be tiled. If set smaller than real image width - the shown image in GrandOrgue will be cropped."));
		m_heightSpin->SetToolTip(wxT("Height of bitmap to use. NOTE: Changing this value does not scale the image! If set greater than real image height - the shown image in GrandOrgue will be tiled. If set smaller than real image height - the shown image in GrandOrgue will be cropped."));
		m_tileOffsetXSpin->SetToolTip(wxT("Offset any tiling for the x axis."));
		m_tileOffsetYSpin->SetToolTip(wxT("Offset any tiling for the y axis."));
		m_mouseRectLeftSpin->SetToolTip(wxT("Controls the left coordinate for where the area of mouse interaction begin."));
		m_mouseRectTopSpin->SetToolTip(wxT("Controls the upper coordinate for where the area of mouse interaction begin."));
		m_mouseRectWidthSpin->SetToolTip(wxT("Set the width of the area of where mouse interaction is possible. NOTE: To get full rectangle area interaction the mouse radius must be set to 0, otherwise the interaction area is limited to a circle created with the mouse radius value."));
		m_mouseRectHeightSpin->SetToolTip(wxT("Set the height of the area of where mouse interaction is possible. NOTE: To get full rectangle area interaction the mouse radius must be set to 0, otherwise the interaction area is limited to a circle created with the mouse radius value."));
		m_mouseRadiusSpin->SetToolTip(wxT("Set the radius of the circle area of where mouse interaction is possible."));
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
		m_displayAsPistonYes->SetToolTip(wxEmptyString);
		m_displayAsPistonNo->SetToolTip(wxEmptyString);
		m_displayKeyLabelLeftYes->SetToolTip(wxEmptyString);
		m_displayKeyLabelLeftNo->SetToolTip(wxEmptyString);
		m_elementPosXSpin->SetToolTip(wxEmptyString);
		m_elementPosYSpin->SetToolTip(wxEmptyString);
		m_dispImageNbrBox->SetToolTip(wxEmptyString);
		m_buttonRowSpin->SetToolTip(wxEmptyString);
		m_buttonColSpin->SetToolTip(wxEmptyString);
		m_drawstopRowSpin->SetToolTip(wxEmptyString);
		m_drawstopColSpin->SetToolTip(wxEmptyString);
		m_imageOnPathField->SetToolTip(wxEmptyString);
		m_addImageOnBtn->SetToolTip(wxEmptyString);
		m_imageOffPathField->SetToolTip(wxEmptyString);
		m_addImageOffBtn->SetToolTip(wxEmptyString);
		m_maskOnPathField->SetToolTip(wxEmptyString);
		m_addMaskOnBtn->SetToolTip(wxEmptyString);
		m_maskOffPathField->SetToolTip(wxEmptyString);
		m_addMaskOffBtn->SetToolTip(wxEmptyString);
		m_widthSpin->SetToolTip(wxEmptyString);
		m_heightSpin->SetToolTip(wxEmptyString);
		m_tileOffsetXSpin->SetToolTip(wxEmptyString);
		m_tileOffsetYSpin->SetToolTip(wxEmptyString);
		m_mouseRectLeftSpin->SetToolTip(wxEmptyString);
		m_mouseRectTopSpin->SetToolTip(wxEmptyString);
		m_mouseRectWidthSpin->SetToolTip(wxEmptyString);
		m_mouseRectHeightSpin->SetToolTip(wxEmptyString);
		m_mouseRadiusSpin->SetToolTip(wxEmptyString);
		m_textRectLeftSpin->SetToolTip(wxEmptyString);
		m_textRectTopSpin->SetToolTip(wxEmptyString);
		m_textRectWidthSpin->SetToolTip(wxEmptyString);
		m_textRectHeightSpin->SetToolTip(wxEmptyString);
		m_textBreakWidthSpin->SetToolTip(wxEmptyString);
	}
}

void GUIButtonPanel::refreshData() {
	m_labelTextField->ChangeValue(m_button->getDispLabelText());
}

void GUIButtonPanel::OnLabelTextChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_labelTextField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_labelTextField);
	m_button->setDispLabelText(m_labelTextField->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnLabelFontChange(wxCommandEvent& WXUNUSED(event)) {
	wxFontData fontData;
	fontData.SetInitialFont(m_button->getDispLabelFont());
	wxFontDialog fontDlg(this, fontData);

	if (fontDlg.ShowModal() == wxID_OK) {
		wxFontData selectedFontData = fontDlg.GetFontData();
		wxFont selectedFont = selectedFontData.GetChosenFont();
		m_labelFont->SetFont(selectedFont);
		m_labelFont->SetLabel(selectedFont.GetFaceName() + wxString::Format(wxT(" %i"), selectedFont.GetPointSize()));
		m_button->setDispLabelFont(selectedFont);
		m_button->setDispLabelFontSize(selectedFont.GetPointSize());
	} else {
		if (m_button->getDispLabelFont().GetFaceName() != m_button->getOwningPanel()->getDisplayMetrics()->m_dispControlLabelFont.GetFaceName()) {
			wxMessageDialog fontNameMsg(this, wxT("Do you want to revert to display metrics default?"), wxT("Revert to default font name?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (fontNameMsg.ShowModal() == wxID_YES) {
				wxFont dispFont = m_button->getOwningPanel()->getDisplayMetrics()->m_dispControlLabelFont;
				dispFont.SetPointSize(m_button->getDispLabelFontSize()->getSizeValue());
				m_button->setDispLabelFont(dispFont);
				m_labelFont->SetFont(dispFont);
				m_labelFont->SetLabel(dispFont.GetFaceName() + wxString::Format(wxT(" %i"), dispFont.GetPointSize()));
			}
		}
		if (m_button->getDispLabelFontSize()->getSizeValue() != m_button->getOwningPanel()->getDisplayMetrics()->m_dispControlLabelFont.GetPointSize()) {
			wxMessageDialog fontSizeMsg(this, wxT("Do you want to revert to current display metrics size value?"), wxT("Revert to current default size?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (fontSizeMsg.ShowModal() == wxID_YES) {
				m_button->setDispLabelFontSize(m_button->getOwningPanel()->getDisplayMetrics()->m_dispControlLabelFont.GetPointSize());
				m_labelFont->SetFont(m_button->getDispLabelFont());
				m_labelFont->SetLabel(m_button->getDispLabelFont().GetFaceName() + wxString::Format(wxT(" %i"), m_button->getOwningPanel()->getDisplayMetrics()->m_dispControlLabelFont.GetPointSize()));
			}
		}
	}

	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnLabelColourChoice(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIBUTTONPANEL_COLOR_CHOICE) {
		if (m_labelColourChoice->GetSelection() == 0) {
			m_labelColourPick->Enable();
		} else {
			m_button->getDispLabelColour()->setSelectedColorIndex(m_labelColourChoice->GetSelection());
			m_labelColourPick->SetColour(m_button->getDispLabelColour()->getColor());
			m_labelColourPick->Disable();
			::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
		}
	}
}

void GUIButtonPanel::OnLabelColourPick(wxColourPickerEvent& event) {
	if (event.GetId() == ID_GUIBUTTONPANEL_COLOR_PICKER) {
		m_button->getDispLabelColour()->setColorValue(m_labelColourPick->GetColour());
		::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
	}
}

void GUIButtonPanel::OnDisplayAsPistonRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIBUTTONPANEL_DISP_AS_PISTON_YES) {
		m_displayAsPistonYes->SetValue(true);
		m_button->setDisplayAsPiston(true);
		if (m_button->getPosX() == -1 && m_button->getPosY() == -1) {
			m_buttonRowSpin->Enable();
			m_buttonColSpin->Enable();
			m_drawstopRowSpin->Disable();
			m_drawstopColSpin->Disable();
		}
	} else {
		m_displayAsPistonNo->SetValue(true);
		m_button->setDisplayAsPiston(false);
		if (m_button->getPosX() == -1 && m_button->getPosY() == -1) {
			m_buttonRowSpin->Disable();
			m_buttonColSpin->Disable();
			m_drawstopRowSpin->Enable();
			m_drawstopColSpin->Enable();
		}
	}
	SetupImageNbrBoxContent();
	m_dispImageNbrBox->SetSelection(m_button->getDispImageNum() - 1);
	if (m_button->getImageOn() == wxEmptyString) {
		m_button->updateBuiltinBitmapValues();
		UpdateSpinRanges();
		UpdateDefaultSpinValues();
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnDisplayKeyLabelLeftRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIBUTTONPANEL_DISP_LABEL_LEFT_YES) {
		m_displayKeyLabelLeftYes->SetValue(true);
		m_button->setDispKeyLabelOnLeft(true);
	} else  {
		m_displayKeyLabelLeftNo->SetValue(true);
		m_button->setDispKeyLabelOnLeft(false);
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnImageNumberChoice(wxCommandEvent& WXUNUSED(event)) {
	m_button->setDispImageNum(m_dispImageNbrBox->GetSelection() + 1);
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnButtonRowSpin(wxSpinEvent& WXUNUSED(event)) {
	int rowValue = m_buttonRowSpin->GetValue();
	m_button->setDispButtonRow(rowValue);
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnButtonColSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setDispButtonCol(m_buttonColSpin->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnDrawstopRowSpin(wxSpinEvent& WXUNUSED(event)) {
	int rowValue = m_drawstopRowSpin->GetValue();
	m_button->setDispDrawstopRow(m_drawstopRowSpin->GetValue());
	// depending on the row value the available number of columns might change
	if (rowValue < 100) {
		m_drawstopColSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispDrawstopCols);
	} else {
		m_drawstopColSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispExtraDrawstopCols);
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnDrawstopColSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setDispDrawstopCol(m_drawstopColSpin->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnAddImageOnBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (m_button->getImageOff() == wxEmptyString || (width == m_button->getBitmapWidth() && height == m_button->getBitmapHeight())) {
				m_button->setImageOn(path);
				m_button->setBitmapWidth(width);
				m_button->setBitmapHeight(height);
				m_button->setWidth(width);
				m_button->setHeight(height);
				m_button->setMouseRectWidth(width - m_button->getMouseRectLeft());
				m_button->setMouseRectHeight(height - m_button->getMouseRectTop());
				m_button->setMouseRadius(std::min(width, height) / 2);
				m_button->setTextRectWidth(width - m_button->getTextRectLeft());
				m_button->setTextRectHeight(height - m_button->getTextRectTop());
				m_button->setTextBreakWidth(m_button->getTextRectWidth() - (m_button->getTextRectWidth() < 50 ? 4 : 14));
				UpdateSpinRanges();
				UpdateDefaultSpinValues();
				wxString relativePath = GOODF_functions::removeBaseOdfPath(m_button->getImageOn());
				m_imageOnPathField->SetValue(relativePath);
				m_addMaskOnBtn->Enable();
			} else {
				wxMessageDialog msg(this, wxT("Image on bitmap size doesn't match existing bitmap!"), wxT("Wrong bitmap size"), wxOK|wxCENTRE|wxICON_ERROR);
				msg.ShowModal();
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_button->getImageOn() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Image on value is not empty! Do you want to empty it? (Also removes mask on if set, may remove image/mask off if set and they won't match default values)"), wxT("Empty image on value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_button->setImageOn(wxEmptyString);
				m_button->setMaskOn(wxEmptyString);
				m_button->updateBuiltinBitmapValues();
				UpdateSpinRanges();
				UpdateDefaultSpinValues();
				m_imageOnPathField->SetValue(wxEmptyString);
				m_maskOnPathField->SetValue(wxEmptyString);
				m_addMaskOnBtn->Disable();
				if (m_button->getImageOff() != wxEmptyString) {
					// the off bitmap must be evaluated to whether it may stay or not
					wxImage offImg = wxImage(m_button->getImageOff());
					if (!offImg.IsOk() || offImg.GetWidth() != m_button->getBitmapWidth() || offImg.GetHeight() != m_button->getBitmapHeight()) {
						m_button->setImageOff(wxEmptyString);
						m_button->setMaskOff(wxEmptyString);
						m_imageOffPathField->SetValue(wxEmptyString);
						m_maskOffPathField->SetValue(wxEmptyString);
						m_addMaskOffBtn->Disable();
					}
				}
			}
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIButtonPanel::OnAddImageOffBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (width == m_button->getBitmapWidth() && height == m_button->getBitmapHeight()) {
				m_button->setImageOff(path);
				wxString relativePath = GOODF_functions::removeBaseOdfPath(m_button->getImageOff());
				m_imageOffPathField->SetValue(relativePath);
				m_addMaskOffBtn->Enable();
				::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
			} else {
				wxMessageDialog msg(this, wxT("Image off bitmap size must match on bitmap!"), wxT("Wrong bitmap size"), wxOK|wxCENTRE|wxICON_ERROR);
				msg.ShowModal();
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_button->getImageOff() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Image off value is not empty! Do you want to empty it? (Also removes mask off if set)"), wxT("Empty image off value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_button->setImageOff(wxEmptyString);
				m_button->setMaskOff(wxEmptyString);
				m_imageOffPathField->SetValue(wxEmptyString);
				m_maskOffPathField->SetValue(wxEmptyString);
				m_addMaskOffBtn->Disable();
				::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
			}
		}
	}
}

void GUIButtonPanel::OnAddMaskOnBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (width == m_button->getBitmapWidth() && height == m_button->getBitmapHeight()) {
				m_button->setMaskOn(path);
				wxString relativePath = GOODF_functions::removeBaseOdfPath(m_button->getMaskOn());
				m_maskOnPathField->SetValue(relativePath);
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_button->getMaskOn() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Mask on value is not empty! Do you want to empty it?"), wxT("Empty mask on value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_button->setMaskOn(wxEmptyString);
				m_maskOnPathField->SetValue(wxEmptyString);
			}
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIButtonPanel::OnAddMaskOffBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			if (width == m_button->getBitmapWidth() && height == m_button->getBitmapHeight()) {
				m_button->setMaskOff(path);
				wxString relativePath = GOODF_functions::removeBaseOdfPath(m_button->getMaskOff());
				m_maskOffPathField->SetValue(relativePath);
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_button->getMaskOff() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Mask off value is not empty! Do you want to empty it?"), wxT("Empty mask off value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				m_button->setMaskOff(wxEmptyString);
				m_maskOffPathField->SetValue(wxEmptyString);
			}
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIButtonPanel::OnWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setWidth(m_widthSpin->GetValue());
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setHeight(m_heightSpin->GetValue());
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnTileOffsetXSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTileOffsetX(m_tileOffsetXSpin->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnTileOffsetYSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTileOffsetY(m_tileOffsetYSpin->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnMouseRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRectLeft(m_mouseRectLeftSpin->GetValue());
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIButtonPanel::OnMouseRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRectTop(m_mouseRectTopSpin->GetValue());
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIButtonPanel::OnMouseRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRectWidth(m_mouseRectWidthSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIButtonPanel::OnMouseRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRectHeight(m_mouseRectHeightSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIButtonPanel::OnMouseRadiusSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setMouseRadius(m_mouseRadiusSpin->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIButtonPanel::OnTextRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextRectLeft(m_textRectLeftSpin->GetValue());
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnTextRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextRectTop(m_textRectTopSpin->GetValue());
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnTextRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextRectWidth(m_textRectWidthSpin->GetValue());
	UpdateSpinRanges();
	UpdateDefaultSpinValues();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnTextRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextRectHeight(m_textRectHeightSpin->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnTextBreakWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	m_button->setTextBreakWidth(m_textBreakWidthSpin->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnPositionXSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that button or drawstop row/col values are used
	// if it's not -1 then all they should be reset and disabled
	int value = m_elementPosXSpin->GetValue();
	m_button->setPosX(value);
	if (value != -1) {
		m_button->setDispButtonRow(1);
		m_buttonRowSpin->SetValue(1);
		m_button->setDispButtonCol(1);
		m_buttonColSpin->SetValue(1);
		m_button->setDispDrawstopRow(1);
		m_drawstopRowSpin->SetValue(1);
		m_button->setDispDrawstopCol(1);
		m_drawstopColSpin->SetValue(1);
		m_buttonRowSpin->Disable();
		m_buttonColSpin->Disable();
		m_drawstopRowSpin->Disable();
		m_drawstopColSpin->Disable();
	} else {
		if (m_button->isDisplayAsPiston()) {
			m_buttonRowSpin->Enable();
			m_buttonColSpin->Enable();
			m_drawstopRowSpin->Disable();
			m_drawstopColSpin->Disable();
		} else {
			m_buttonRowSpin->Disable();
			m_buttonColSpin->Disable();
			m_drawstopRowSpin->Enable();
			m_drawstopColSpin->Enable();
		}
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnPositionYSpin(wxSpinEvent& WXUNUSED(event)) {
	// a value of -1 indicate that button or drawstop row/col values are used
	// if it's not -1 then all they should be reset and disabled
	int value = m_elementPosYSpin->GetValue();
	m_button->setPosY(value);
	if (value != -1) {
		m_button->setDispButtonRow(1);
		m_buttonRowSpin->SetValue(1);
		m_button->setDispButtonCol(1);
		m_buttonColSpin->SetValue(1);
		m_button->setDispDrawstopRow(1);
		m_drawstopRowSpin->SetValue(1);
		m_button->setDispDrawstopCol(1);
		m_drawstopColSpin->SetValue(1);
		m_buttonRowSpin->Disable();
		m_buttonColSpin->Disable();
		m_drawstopRowSpin->Disable();
		m_drawstopColSpin->Disable();
	} else {
		if (m_button->isDisplayAsPiston()) {
			m_buttonRowSpin->Enable();
			m_buttonColSpin->Enable();
			m_drawstopRowSpin->Disable();
			m_drawstopColSpin->Disable();
		} else {
			m_buttonRowSpin->Disable();
			m_buttonColSpin->Disable();
			m_drawstopRowSpin->Enable();
			m_drawstopColSpin->Enable();
		}
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::OnRemoveButtonBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_button->getOwningPanel()->getIsGuiElementFirstRemoval()) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this element?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			m_button->getOwningPanel()->setIsGuiElementFirstRemoval(false);
			// then remove this item from the owning panel
			::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
		}
	} else {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIButtonPanel::SetupImageNbrBoxContent() {
	if (m_displayAsPistonYes->GetValue()) {
		// if displayed as piston the image num box should be filled with pistons
		m_dispImageNbrBox->Clear();
		for (unsigned i = 0; i < ::wxGetApp().m_buttonBitmaps.size(); i++) {
			wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
			m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_buttonBitmaps[i]);
		}
	} else {
		// if not displayed as piston the image num box should be filled with drawstops
		m_dispImageNbrBox->Clear();
		for (unsigned i = 0; i < ::wxGetApp().m_scaledDrawstopBitmaps.size(); i++) {
			wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
			m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_scaledDrawstopBitmaps[i]);
		}
	}
}

void GUIButtonPanel::UpdateSpinRanges() {
	m_elementPosXSpin->SetRange(-1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue());
	m_elementPosYSpin->SetRange(-1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	m_widthSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue()); // panel width!
	m_heightSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue()); // panel height!
	m_tileOffsetXSpin->SetRange(0, m_button->getBitmapWidth() - 1);
	m_tileOffsetYSpin->SetRange(0, m_button->getBitmapHeight() - 1);
	m_mouseRectLeftSpin->SetRange(0, m_button->getWidth());
	m_mouseRectTopSpin->SetRange(0, m_button->getHeight());
	if (m_button->getMouseRectWidth() > (m_button->getWidth() - m_button->getMouseRectLeft()))
		m_button->setMouseRectWidth(m_button->getWidth() - m_button->getMouseRectLeft());
	m_mouseRectWidthSpin->SetRange(1, m_button->getWidth() - m_button->getMouseRectLeft());
	if (m_button->getMouseRectHeight() > (m_button->getHeight() - m_button->getMouseRectTop()))
		m_button->setMouseRectHeight(m_button->getHeight() - m_button->getMouseRectTop());
	m_mouseRectHeightSpin->SetRange(1, m_button->getHeight() - m_button->getMouseRectTop());
	if (m_button->getMouseRadius() > (std::max(m_button->getMouseRectWidth(), m_button->getMouseRectHeight()) / 2))
		m_button->setMouseRadius(std::max(m_button->getMouseRectWidth(), m_button->getMouseRectHeight()) / 2);
	m_mouseRadiusSpin->SetRange(0, (std::max(m_button->getMouseRectWidth(), m_button->getMouseRectHeight()) / 2));
	m_textRectLeftSpin->SetRange(0, m_button->getWidth());
	m_textRectTopSpin->SetRange(0, m_button->getHeight());
	if (m_button->getTextRectWidth() > (m_button->getWidth() - m_button->getTextRectLeft()))
		m_button->setTextRectWidth(m_button->getWidth() - m_button->getTextRectLeft());
	m_textRectWidthSpin->SetRange(1, m_button->getWidth() - m_button->getTextRectLeft());
	if (m_button->getTextRectHeight() > (m_button->getHeight() - m_button->getTextRectTop()))
		m_button->setTextRectHeight(m_button->getHeight() - m_button->getTextRectTop());
	m_textRectHeightSpin->SetRange(1, m_button->getHeight() - m_button->getTextRectTop());
	if (m_button->getTextBreakWidth() > m_button->getTextRectWidth())
		m_button->setTextBreakWidth(m_button->getTextRectWidth());
	m_textBreakWidthSpin->SetRange(0, m_button->getTextRectWidth());

	m_buttonRowSpin->SetRange(0, 99 + m_button->getOwningPanel()->getDisplayMetrics()->m_dispExtraButtonRows);
	m_buttonColSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispButtonCols);
	m_drawstopRowSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispExtraDrawstopRows > 0 ? (99 + m_button->getOwningPanel()->getDisplayMetrics()->m_dispExtraDrawstopRows) : 99);
	if (m_drawstopRowSpin->GetValue() < 100)
		m_drawstopColSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispDrawstopCols);
	else
		m_drawstopColSpin->SetRange(1, m_button->getOwningPanel()->getDisplayMetrics()->m_dispExtraDrawstopCols);
}

void GUIButtonPanel::UpdateDefaultSpinValues() {
	m_elementPosXSpin->SetValue(m_button->getPosX());
	m_elementPosYSpin->SetValue(m_button->getPosY());
	m_widthSpin->SetValue(m_button->getWidth());
	m_heightSpin->SetValue(m_button->getHeight());
	m_tileOffsetXSpin->SetValue(m_button->getTileOffsetX());
	m_tileOffsetYSpin->SetValue(m_button->getTileOffsetY());
	m_mouseRectLeftSpin->SetValue(m_button->getMouseRectLeft());
	m_mouseRectTopSpin->SetValue(m_button->getMouseRectTop());
	m_mouseRectWidthSpin->SetValue(m_button->getMouseRectWidth());
	m_mouseRectHeightSpin->SetValue(m_button->getMouseRectHeight());
	m_mouseRadiusSpin->SetValue(m_button->getMouseRadius());
	m_textRectLeftSpin->SetValue(m_button->getTextRectLeft());
	m_textRectTopSpin->SetValue(m_button->getTextRectTop());
	m_textRectWidthSpin->SetValue(m_button->getTextRectWidth());
	m_textRectHeightSpin->SetValue(m_button->getTextRectHeight());
	m_textBreakWidthSpin->SetValue(m_button->getTextBreakWidth());
}

wxString GUIButtonPanel::GetPathForImageFile() {
	wxString imageFilePath;
	wxString defaultPath = m_lastUsedImagePath;
	if (m_button->getImageOn() != wxEmptyString) {
		wxFileName filePath = wxFileName(m_button->getImageOn());
		if (filePath.FileExists())
			defaultPath = filePath.GetPath();
	} else if (defaultPath == wxEmptyString) {
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
	wxFileName selectedFile = wxFileName(imageFilePath);
	m_lastUsedImagePath = selectedFile.GetPath();
	return imageFilePath;
}
