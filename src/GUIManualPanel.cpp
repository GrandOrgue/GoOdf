/*
 * GUIManualPanel.cpp is part of GOODF.
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

#include "GUIManualPanel.h"
#include "GOODFFunctions.h"
#include "GOODF.h"
#include "ManualKeyCopyDialog.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>
#include <algorithm>
#include <wx/msgdlg.h>
#include <wx/filename.h>

// Event table
BEGIN_EVENT_TABLE(GUIManualPanel, wxPanel)
	EVT_RADIOBUTTON(ID_GUIMANUALPANEL_COLOR_INVERTED_YES, GUIManualPanel::OnKeyColorInvertedRadio)
	EVT_RADIOBUTTON(ID_GUIMANUALPANEL_COLOR_INVERTED_NO, GUIManualPanel::OnKeyColorInvertedRadio)
	EVT_RADIOBUTTON(ID_GUIMANUALPANEL_COLOR_WOOD_YES, GUIManualPanel::OnKeyColorWoodRadio)
	EVT_RADIOBUTTON(ID_GUIMANUALPANEL_COLOR_WOOD_NO, GUIManualPanel::OnKeyColorWoodRadio)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_X_SPIN, GUIManualPanel::OnPositionXSpin)
	EVT_SPINCTRL(ID_GUIELEMENT_POS_Y_SPIN, GUIManualPanel::OnPositionYSpin)
	EVT_LISTBOX(ID_GUIMANUALPANEL_KEY_TYPES_BOX, GUIManualPanel::OnAvailableKeyTypesChoice)
	EVT_LISTBOX(ID_GUIMANUALPANEL_KEY_NUMBERS_BOX, GUIManualPanel::OnAvailableKeyNumbersChoice)
	EVT_BUTTON(ID_GUIMANUALPANEL_ADD_KEY_BTN, GUIManualPanel::OnAddKeyBtn)
	EVT_BUTTON(ID_GUIMANUALPANEL_REMOVE_KEY_BTN, GUIManualPanel::OnRemoveKeyBtn)
	EVT_BUTTON(ID_GUIMANUALPANEL_COPY_KEY_BTN, GUIManualPanel::OnCopyKeyBtn)
	EVT_LISTBOX(ID_GUIMANUALPANEL_ADDED_KEYS_BOX, GUIManualPanel::OnAddedKeysChoice)
	EVT_BUTTON(ID_GUIMANUALPANEL_IMAGE_ON_BTN, GUIManualPanel::OnAddImageOnBtn)
	EVT_BUTTON(ID_GUIMANUALPANEL_MASK_ON_BTN, GUIManualPanel::OnAddMaskOnBtn)
	EVT_BUTTON(ID_GUIMANUALPANEL_IMAGE_OFF_BTN, GUIManualPanel::OnAddImageOffBtn)
	EVT_BUTTON(ID_GUIMANUALPANEL_MASK_OFF_BTN, GUIManualPanel::OnAddMaskOffBtn)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_WIDTH_SPIN, GUIManualPanel::OnWidthSpin)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_OFFSET_SPIN, GUIManualPanel::OnOffsetSpin)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_Y_OFFSET_SPIN, GUIManualPanel::OnOffsetYSpin)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_MOUSE_RECT_LEFT_SPIN, GUIManualPanel::OnMouseRectLeftSpin)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_MOUSE_RECT_TOP_SPIN, GUIManualPanel::OnMouseRectTopSpin)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_MOUSE_RECT_WIDTH_SPIN, GUIManualPanel::OnMouseRectWidthSpin)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_MOUSE_RECT_HEIGHT_SPIN, GUIManualPanel::OnMouseRectHeightSpin)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_DISPLAY_KEYS_SPIN, GUIManualPanel::OnDisplayKeysSpin)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_FIRST_NOTE_SPIN, GUIManualPanel::OnFirstNoteSpin)
	EVT_CHOICE(ID_GUIMANUALPANEL_DISP_KEY_CHOICE, GUIManualPanel::OnDisplayKeyChoice)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_BACKEND_MIDI_SPIN, GUIManualPanel::OnBackendMIDIkeySpin)
	EVT_SPINCTRL(ID_GUIMANUALPANEL_FRONTEND_MIDI_SPIN, GUIManualPanel::OnFrontendMIDIkeySpin)
	EVT_BUTTON(ID_GUIMANUALPANEL_REMOVE_BTN, GUIManualPanel::OnRemoveManualBtn)
END_EVENT_TABLE()

GUIManualPanel::GUIManualPanel(wxWindow *parent) : wxPanel(parent) {
	m_manual = NULL;
	m_lastUsedPath = wxEmptyString;
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *colorInvertedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Key color inverted: ")
	);
	firstRow->Add(colorInvertedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_keyColorInvertedYes = new wxRadioButton(
		this,
		ID_GUIMANUALPANEL_COLOR_INVERTED_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	firstRow->Add(m_keyColorInvertedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_keyColorInvertedNo = new wxRadioButton(
		this,
		ID_GUIMANUALPANEL_COLOR_INVERTED_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_keyColorInvertedNo->SetValue(true);
	firstRow->Add(m_keyColorInvertedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	firstRow->AddStretchSpacer();
	wxStaticText *colorWoodText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Key color wooden: ")
	);
	firstRow->Add(colorWoodText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_keyColorWoodYes = new wxRadioButton(
		this,
		ID_GUIMANUALPANEL_COLOR_WOOD_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	firstRow->Add(m_keyColorWoodYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_keyColorWoodNo = new wxRadioButton(
		this,
		ID_GUIMANUALPANEL_COLOR_WOOD_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_keyColorWoodNo->SetValue(true);
	firstRow->Add(m_keyColorWoodNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

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
	wxBoxSizer *thirdRowFirstCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *keyTypesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available key types")
	);
	thirdRowFirstCol->Add(keyTypesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableKeyTypes = new wxListBox(
		this,
		ID_GUIMANUALPANEL_KEY_TYPES_BOX,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	thirdRowFirstCol->Add(m_availableKeyTypes, 1, wxGROW|wxALL, 5);
	thirdRow->Add(thirdRowFirstCol, 1, wxGROW);
	wxBoxSizer *thirdRowSecondCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *keyNbrsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available key numbers")
	);
	thirdRowSecondCol->Add(keyNbrsText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableKeyNumbers = new wxListBox(
		this,
		ID_GUIMANUALPANEL_KEY_NUMBERS_BOX,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	thirdRowSecondCol->Add(m_availableKeyNumbers, 1, wxGROW|wxALL, 5);
	thirdRow->Add(thirdRowSecondCol, 1, wxGROW);
	wxBoxSizer *thirdRowThirdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *emptyText = new wxStaticText (
		this,
		wxID_STATIC,
		wxEmptyString
	);
	thirdRowThirdCol->Add(emptyText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	thirdRowThirdCol->AddStretchSpacer();
	m_addKey = new wxButton(
		this,
		ID_GUIMANUALPANEL_ADD_KEY_BTN,
		wxT("Add key ->"),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	thirdRowThirdCol->Add(m_addKey, 0, wxALL, 5);
	thirdRowThirdCol->AddStretchSpacer();
	m_removeKey = new wxButton(
		this,
		ID_GUIMANUALPANEL_REMOVE_KEY_BTN,
		wxT("Remove key"),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	thirdRowThirdCol->Add(m_removeKey, 0, wxALL, 5);
	thirdRowThirdCol->AddStretchSpacer();
	m_copyKey = new wxButton(
		this,
		ID_GUIMANUALPANEL_COPY_KEY_BTN,
		wxT("Copy key attributes"),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	thirdRowThirdCol->Add(m_copyKey, 0, wxALL, 5);
	thirdRowThirdCol->AddStretchSpacer();
	thirdRow->Add(thirdRowThirdCol, 0, wxGROW);
	wxBoxSizer *thirdRowFourthCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *addedKeysText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("User added keys")
	);
	thirdRowFourthCol->Add(addedKeysText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_addedKeyTypes = new wxListBox(
		this,
		ID_GUIMANUALPANEL_ADDED_KEYS_BOX,
		wxDefaultPosition,
		wxDefaultSize
	);
	thirdRowFourthCol->Add(m_addedKeyTypes, 1, wxGROW|wxALL, 5);
	thirdRow->Add(thirdRowFourthCol, 1, wxGROW);
	panelSizer->Add(thirdRow, 1, wxEXPAND);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imageOnText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("ImageOn: ")
	);
	fourthRow->Add(imageOnText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_imageOnPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fourthRow->Add(m_imageOnPathField, 1, wxEXPAND|wxALL, 5);
	m_addImageOnBtn = new wxButton(
		this,
		ID_GUIMANUALPANEL_IMAGE_ON_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	fourthRow->Add(m_addImageOnBtn, 0, wxALL, 5);
	wxStaticText *imageOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("ImageOff: ")
	);
	fourthRow->Add(imageOffText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_imageOffPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fourthRow->Add(m_imageOffPathField, 1, wxEXPAND|wxALL, 5);
	m_addImageOffBtn = new wxButton(
		this,
		ID_GUIMANUALPANEL_IMAGE_OFF_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	fourthRow->Add(m_addImageOffBtn, 0, wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxEXPAND);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *maskOnText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MaskOn: ")
	);
	fifthRow->Add(maskOnText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maskOnPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fifthRow->Add(m_maskOnPathField, 1, wxEXPAND|wxALL, 5);
	m_addMaskOnBtn = new wxButton(
		this,
		ID_GUIMANUALPANEL_MASK_ON_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	fifthRow->Add(m_addMaskOnBtn, 0, wxALL, 5);
	wxStaticText *maskOffText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MaskOff: ")
	);
	fifthRow->Add(maskOffText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maskOffPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	fifthRow->Add(m_maskOffPathField, 1, wxEXPAND|wxALL, 5);
	m_addMaskOffBtn = new wxButton(
		this,
		ID_GUIMANUALPANEL_MASK_OFF_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	fifthRow->Add(m_addMaskOffBtn, 0, wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxEXPAND);

	wxBoxSizer *sixthRow  = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *widthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Width: ")
	);
	sixthRow->Add(widthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_widthSpin = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		500,
		0
	);
	sixthRow->Add(m_widthSpin, 0, wxEXPAND|wxALL, 5);
	sixthRow->AddStretchSpacer();
	wxStaticText *offsetText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Offset: ")
	);
	sixthRow->Add(offsetText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_offsetSpin = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_OFFSET_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		-500,
		500,
		0
	);
	sixthRow->Add(m_offsetSpin, 0, wxEXPAND|wxALL, 5);
	sixthRow->AddStretchSpacer();
	wxStaticText *yOffsetText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("YOffset: ")
	);
	sixthRow->Add(yOffsetText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_offsetYSpin = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_Y_OFFSET_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		500,
		0
	);
	sixthRow->Add(m_offsetYSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxEXPAND);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *mouseRectLeftText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectLeft: ")
	);
	seventhRow->Add(mouseRectLeftText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectLeftSpin = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_MOUSE_RECT_LEFT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	seventhRow->Add(m_mouseRectLeftSpin, 0, wxEXPAND|wxALL, 5);
	seventhRow->AddStretchSpacer();
	wxStaticText *mouseRectTopText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectTop: ")
	);
	seventhRow->Add(mouseRectTopText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectTopSpin = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_MOUSE_RECT_TOP_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	seventhRow->Add(m_mouseRectTopSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(seventhRow, 0, wxEXPAND);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *mouseRectWidthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectWidth: ")
	);
	eighthRow->Add(mouseRectWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectWidthSpin = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_MOUSE_RECT_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eighthRow->Add(m_mouseRectWidthSpin, 0, wxEXPAND|wxALL, 5);
	eighthRow->AddStretchSpacer();
	wxStaticText *mouseRectHeightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("MouseRectHeight: ")
	);
	eighthRow->Add(mouseRectHeightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_mouseRectHeightSpin = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_MOUSE_RECT_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	eighthRow->Add(m_mouseRectHeightSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eighthRow, 0, wxEXPAND);

	wxBoxSizer *ninthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *dispKeysText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("(Nbr of) Display keys: ")
	);
	ninthRow->Add(dispKeysText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayKeysSpin = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_DISPLAY_KEYS_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		1,
		0
	);
	ninthRow->Add(m_displayKeysSpin, 0, wxEXPAND|wxALL, 5);
	ninthRow->AddStretchSpacer();
	wxStaticText *firstNoteText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("First note (MIDI nbr): ")
	);
	ninthRow->Add(firstNoteText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_firstNoteSpin = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_FIRST_NOTE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		127,
		0
	);
	ninthRow->Add(m_firstNoteSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(ninthRow, 0, wxEXPAND);

	wxBoxSizer *tenthRow = new wxBoxSizer(wxHORIZONTAL);
	tenthRow->AddStretchSpacer();
	wxStaticText *dispKeyMapText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("DisplayKey: ")
	);
	tenthRow->Add(dispKeyMapText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayKeyChoice = new wxChoice(
		this,
		ID_GUIMANUALPANEL_DISP_KEY_CHOICE
	);
	tenthRow->Add(m_displayKeyChoice, 0, wxALL, 5);
	wxStaticText *mapToBackText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("maps to backend MIDI: ")
	);
	tenthRow->Add(mapToBackText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_backendMIDIkey = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_BACKEND_MIDI_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		127,
		0
	);
	tenthRow->Add(m_backendMIDIkey, 0, wxEXPAND|wxALL, 5);
	tenthRow->AddStretchSpacer();
	wxStaticText *mapToFrontText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("and frontend MIDI: ")
	);
	tenthRow->Add(mapToFrontText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_frontendMIDIkey = new wxSpinCtrl(
		this,
		ID_GUIMANUALPANEL_FRONTEND_MIDI_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		127,
		0
	);
	tenthRow->Add(m_frontendMIDIkey, 0, wxEXPAND|wxALL, 5);
	tenthRow->AddStretchSpacer();
	panelSizer->Add(tenthRow, 0, wxEXPAND);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	m_removeManualBtn = new wxButton(
		this,
		ID_GUIMANUALPANEL_REMOVE_BTN,
		wxT("Remove this element")
	);
	bottomRow->Add(m_removeManualBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

GUIManualPanel::~GUIManualPanel() {

}

void GUIManualPanel::setManual(GUIManual *manual) {
	m_manual = manual;
	if (m_manual->getDispKeyColourInverted())
		m_keyColorInvertedYes->SetValue(true);
	else
		m_keyColorInvertedNo->SetValue(true);
	if (m_manual->getDispKeyColourWooden())
		m_keyColorWoodYes->SetValue(true);
	else
		m_keyColorWoodNo->SetValue(true);
	m_elementPosXSpin->SetRange(-1, m_manual->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue());
	m_elementPosYSpin->SetRange(-1, m_manual->getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	m_elementPosXSpin->SetValue(m_manual->getPosX());
	m_elementPosYSpin->SetValue(m_manual->getPosY());
	// TODO: this might change from where it's taken and how it's handled
	if (!m_availableKeyTypes->IsEmpty())
		m_availableKeyTypes->Clear();
	m_availableKeyTypes->InsertItems(m_manual->getAvailableKeytypes(), 0);

	m_addKey->Disable();
	m_removeKey->Disable();
	m_copyKey->Disable();
	UpdateAddedKeyTypes();
	m_imageOnPathField->SetValue(wxEmptyString);
	m_addImageOnBtn->Disable();
	m_maskOnPathField->SetValue(wxEmptyString);
	m_addMaskOnBtn->Disable();
	m_imageOffPathField->SetValue(wxEmptyString);
	m_addImageOffBtn->Disable();
	m_maskOffPathField->SetValue(wxEmptyString);
	m_addMaskOffBtn->Disable();
	m_widthSpin->Disable();
	m_offsetSpin->Disable();
	m_offsetYSpin->Disable();
	m_mouseRectLeftSpin->Disable();
	m_mouseRectTopSpin->Disable();
	m_mouseRectWidthSpin->Disable();
	m_mouseRectHeightSpin->Disable();
	m_displayKeysSpin->SetRange(1, m_manual->getManual()->getNumberOfAccessibleKeys());
	m_displayKeysSpin->SetValue(m_manual->getNumberOfDisplayKeys());
	m_firstNoteSpin->SetValue(m_manual->getDisplayFirstNote());
	SetupKeyChoiceAndMapping();
}

void GUIManualPanel::OnKeyColorInvertedRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIMANUALPANEL_COLOR_INVERTED_YES) {
		m_manual->setDispKeyColourInverted(true);
	} else {
		m_manual->setDispKeyColourInverted(false);
	}
}

void GUIManualPanel::OnKeyColorWoodRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIMANUALPANEL_COLOR_WOOD_YES) {
		m_manual->setDispKeyColurWooden(true);
	} else {
		m_manual->setDispKeyColurWooden(false);
	}
}

void GUIManualPanel::OnPositionXSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setPosX(m_elementPosXSpin->GetValue());
}

void GUIManualPanel::OnPositionYSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setPosY(m_elementPosYSpin->GetValue());
}

void GUIManualPanel::OnAvailableKeyTypesChoice(wxCommandEvent& WXUNUSED(event)) {
	m_addKey->Enable();
	wxArrayInt selections;
	int count = m_availableKeyNumbers->GetSelections(selections);
	if (count > 0) {
		for (int i = 0; i < count; i++)
			m_availableKeyNumbers->Deselect(selections[i]);
	}
}

void GUIManualPanel::OnAvailableKeyNumbersChoice(wxCommandEvent& WXUNUSED(event)) {
	m_addKey->Enable();
	wxArrayInt selections;
	int count = m_availableKeyTypes->GetSelections(selections);
	if (count > 0) {
		for (int i = 0; i < count; i++)
			m_availableKeyTypes->Deselect(selections[i]);
	}
}

void GUIManualPanel::OnAddKeyBtn(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selectedKeyTypes;
	int nbrSelectedKeyTypes = m_availableKeyTypes->GetSelections(selectedKeyTypes);
	wxArrayInt selectedKeyNbrs;
	int nbrSelectedKeyNbrs = m_availableKeyNumbers->GetSelections(selectedKeyNbrs);
	if (nbrSelectedKeyTypes > 0) {
		for (int i = 0; i < nbrSelectedKeyTypes; i++) {
			wxString selectedKeyType = m_manual->getAvailableKeytypes().Item(selectedKeyTypes[i]);
			if (m_addedKeyTypes->FindString(selectedKeyType) == wxNOT_FOUND) {
				m_manual->addKeytype(selectedKeyType);
				m_addedKeyTypes->Append(selectedKeyType);
			}
		}
		unsigned existingKeys = m_manual->getNumberOfKeytypes();
		m_addedKeyTypes->SetSelection(existingKeys - 1);
		// we need to notify the listBox that selection has changed
		wxCommandEvent evt(wxEVT_LISTBOX, ID_GUIMANUALPANEL_ADDED_KEYS_BOX);
		wxPostEvent(this, evt);
	} else if (nbrSelectedKeyNbrs > 0) {
		for (int i = 0; i < nbrSelectedKeyNbrs; i++) {
			wxString selectedKeyType = m_manual->getAvailableKeynumbers().Item(selectedKeyNbrs[i]);
			if (m_addedKeyTypes->FindString(selectedKeyType) == wxNOT_FOUND) {
				m_manual->addKeytype(selectedKeyType);
				m_addedKeyTypes->Append(selectedKeyType);
			}
		}
		unsigned existingKeys = m_manual->getNumberOfKeytypes();
		m_addedKeyTypes->SetSelection(existingKeys - 1);
		// we need to notify the listBox that selection has changed
		wxCommandEvent evt(wxEVT_LISTBOX, ID_GUIMANUALPANEL_ADDED_KEYS_BOX);
		wxPostEvent(this, evt);
	}
}

void GUIManualPanel::OnRemoveKeyBtn(wxCommandEvent& WXUNUSED(event)) {
	int selectedIndex = m_addedKeyTypes->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		m_manual->removeKeytypeAt(selectedIndex);
		m_addedKeyTypes->Delete(selectedIndex);
	}
	if (!m_addedKeyTypes->IsEmpty()) {
		if (selectedIndex > 0) {
			selectedIndex--;
		}
		m_addedKeyTypes->SetSelection(selectedIndex);
		if (m_addedKeyTypes->GetCount() > 1)
			m_copyKey->Enable();
		else
			m_copyKey->Disable();
		// we need to notify the listBox that selection has changed
		wxCommandEvent evt(wxEVT_LISTBOX, ID_GUIMANUALPANEL_ADDED_KEYS_BOX);
		wxPostEvent(this, evt);
	} else {
		m_addImageOnBtn->Disable();
		m_removeKey->Disable();
		UpdateExistingSelectedKeyData();
	}
}

void GUIManualPanel::OnCopyKeyBtn(wxCommandEvent& WXUNUSED(event)) {
	ManualKeyCopyDialog copyDlg(m_manual, m_addedKeyTypes->GetSelection(), this);
	if (copyDlg.ShowModal() == wxID_OK) {
		wxArrayInt selectedTargetKeys;
		if (copyDlg.GetSelectedKeys(selectedTargetKeys)) {
			// Now we have targets to copy properties to
			KEYTYPE *sourceKey = m_manual->getKeytypeAt((unsigned) m_addedKeyTypes->GetSelection());
			for (int i : selectedTargetKeys) {
				KEYTYPE *targetKey = m_manual->getKeytypeAt((unsigned) i);
				targetKey->BitmapHeight = sourceKey->BitmapHeight;
				targetKey->BitmapWidth = sourceKey->BitmapWidth;
				targetKey->ImageOff = sourceKey->ImageOff;
				targetKey->ImageOn = sourceKey->ImageOn;
				targetKey->MouseRectHeight = sourceKey->MouseRectHeight;
				targetKey->MouseRectLeft = sourceKey->MouseRectLeft;
				targetKey->MouseRectTop = sourceKey->MouseRectTop;
				targetKey->MouseRectWidth = sourceKey->MouseRectWidth;
				targetKey->Offset = sourceKey->Offset;
				targetKey->Width = sourceKey->Width;
				targetKey->YOffset = sourceKey->YOffset;
			}
		}
	}
}

void GUIManualPanel::OnAddedKeysChoice(wxCommandEvent& WXUNUSED(event)) {
	m_addImageOnBtn->Enable();
	m_removeKey->Enable();
	if (m_addedKeyTypes->GetCount() > 1)
		m_copyKey->Enable();
	UpdateExistingSelectedKeyData();
}

void GUIManualPanel::OnAddImageOnBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
			key->ImageOn.setImage(path);
			int width = img.GetWidth();
			int height = img.GetHeight();
			key->BitmapWidth = width;
			key->BitmapHeight = height;
			key->Width = width;
			key->MouseRectWidth = width - key->MouseRectLeft;
			key->MouseRectHeight = height - key->MouseRectTop;

			UpdateExistingSelectedKeyData();
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection())->ImageOn.getImage() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Image value is not empty! Do you want to empty it and all images for this key?"), wxT("Empty image value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
				key->ImageOn.setImage(wxEmptyString);
				int width = 0;
				int height = 0;
				key->BitmapWidth = width;
				key->BitmapHeight = height;
				key->Width = width;
				key->MouseRectLeft = 0;
				key->MouseRectTop = 0;
				key->MouseRectWidth = 1;
				key->MouseRectHeight = 1;
				if (key->ImageOn.getMask() != wxEmptyString)
					key->ImageOn.setMask(wxEmptyString);
				if (key->ImageOff.getImage() != wxEmptyString)
					key->ImageOff.setImage(wxEmptyString);
				if (key->ImageOff.getMask() != wxEmptyString)
					key->ImageOff.setMask(wxEmptyString);

				UpdateExistingSelectedKeyData();
			}
		}
	}
}

void GUIManualPanel::OnAddImageOffBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
			if (key->BitmapWidth == width && key->BitmapHeight == height) {
				key->ImageOff.setImage(path);
				UpdateExistingSelectedKeyData();
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection())->ImageOff.getImage() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Image value is not empty! Do you want to empty it and related mask?"), wxT("Empty image value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
				key->ImageOff.setImage(wxEmptyString);
				if (key->ImageOff.getMask() != wxEmptyString)
					key->ImageOff.setMask(wxEmptyString);

				UpdateExistingSelectedKeyData();
			}
		}
	}
}

void GUIManualPanel::OnAddMaskOffBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
			if (key->BitmapWidth == width && key->BitmapHeight == height) {
				key->ImageOff.setMask(path);
				UpdateExistingSelectedKeyData();
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection())->ImageOff.getImage() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Mask value is not empty! Do you want to empty it?"), wxT("Empty mask value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
				key->ImageOff.setMask(wxEmptyString);
				UpdateExistingSelectedKeyData();
			}
		}
	}
}

void GUIManualPanel::OnAddMaskOnBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString path = GetPathForImageFile();
	if (path != wxEmptyString) {
		wxImage img = wxImage(path);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
			if (key->BitmapWidth == width && key->BitmapHeight == height) {
				key->ImageOn.setMask(path);
				UpdateExistingSelectedKeyData();
			}
		}
	} else {
		// the user has clicked cancel which returns an empty string
		if (m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection())->ImageOn.getImage() != wxEmptyString) {
			wxMessageDialog msg(this, wxT("Mask value is not empty! Do you want to empty it?"), wxT("Empty mask value?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then we empty the value in button and panel
				KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
				key->ImageOn.setMask(wxEmptyString);
				UpdateExistingSelectedKeyData();
			}
		}
	}
}

void GUIManualPanel::OnWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->Width = m_widthSpin->GetValue();
}

void GUIManualPanel::OnOffsetSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->Offset = m_offsetSpin->GetValue();
}

void GUIManualPanel::OnOffsetYSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->YOffset = m_offsetYSpin->GetValue();
}

void GUIManualPanel::OnMouseRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->MouseRectLeft = m_mouseRectLeftSpin->GetValue();
}

void GUIManualPanel::OnMouseRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->MouseRectTop = m_mouseRectTopSpin->GetValue();
}

void GUIManualPanel::OnMouseRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->MouseRectWidth = m_mouseRectWidthSpin->GetValue();
}

void GUIManualPanel::OnMouseRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->MouseRectHeight = m_mouseRectHeightSpin->GetValue();
}

void GUIManualPanel::OnDisplayKeysSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setNumberOfDisplayKeys(m_displayKeysSpin->GetValue());
	// this might have affect added keys and mapping
	SetupKeyChoiceAndMapping();
	UpdateAddedKeyTypes();
	UpdateExistingSelectedKeyData();
}

void GUIManualPanel::OnFirstNoteSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setDisplayFirstNote(m_firstNoteSpin->GetValue());
	// this might have affect added keys and mapping
	SetupKeyChoiceAndMapping();
	UpdateAddedKeyTypes();
	UpdateExistingSelectedKeyData();
}

void GUIManualPanel::OnDisplayKeyChoice(wxCommandEvent& WXUNUSED(event)) {
	int selectedIndex = m_displayKeyChoice->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		// since the display key might have changed we must update the values for backend and front end
		m_backendMIDIkey->SetValue(m_manual->getDisplayKeyAt(selectedIndex)->first);
		m_frontendMIDIkey->SetValue(m_manual->getDisplayKeyAt(selectedIndex)->second);
	}
}

void GUIManualPanel::OnBackendMIDIkeySpin(wxSpinEvent& WXUNUSED(event)) {
	int selectedIndex = m_displayKeyChoice->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		m_manual->getDisplayKeyAt(selectedIndex)->first = m_backendMIDIkey->GetValue();
	}
}

void GUIManualPanel::OnFrontendMIDIkeySpin(wxSpinEvent& WXUNUSED(event)) {
	int selectedIndex = m_displayKeyChoice->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		m_manual->getDisplayKeyAt(selectedIndex)->second = m_frontendMIDIkey->GetValue();
	}
}

void GUIManualPanel::OnRemoveManualBtn(wxCommandEvent& WXUNUSED(event)) {
	wxMessageDialog msg(this, wxT("Are you really sure you want to delete this element?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (msg.ShowModal() == wxID_YES) {
		// then remove this item from the owning panel
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

wxString GUIManualPanel::GetPathForImageFile() {
	wxString imageFilePath;
	wxString defaultPath = wxEmptyString;
	if (m_manual->getNumberOfKeytypes() != 0 && m_lastUsedPath == wxEmptyString) {
		if (m_manual->getKeytypeAt(0)->ImageOn.getImage() != wxEmptyString) {
			wxFileName filePath = wxFileName(m_manual->getKeytypeAt(0)->ImageOn.getImage());
			if (filePath.FileExists())
				defaultPath = filePath.GetPath();
		} else {
			defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
		}
	} else {
		wxFileName filePath = wxFileName(m_lastUsedPath);
		if (filePath.FileExists()) {
			defaultPath = filePath.GetPath();
		} else {
			defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
		}
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
	m_lastUsedPath = imageFilePath;
	return imageFilePath;
}

void GUIManualPanel::UpdateExistingSelectedKeyData() {
	// this method is called every time a new selection of a added key is made
	int selectedIndex = m_addedKeyTypes->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		KEYTYPE *currentKey = m_manual->getKeytypeAt(selectedIndex);
		m_imageOnPathField->SetValue(currentKey->ImageOn.getRelativeImagePath());
		if (m_imageOnPathField->GetValue() != wxEmptyString) {
			m_addMaskOnBtn->Enable();
			m_addImageOffBtn->Enable();
		} else {
			m_addMaskOnBtn->Disable();
			m_addImageOffBtn->Disable();
			m_addMaskOffBtn->Disable();
		}
		m_maskOnPathField->SetValue(currentKey->ImageOn.getRelativeMaskPath());
		m_imageOffPathField->SetValue(currentKey->ImageOff.getRelativeImagePath());
		if (m_imageOffPathField->GetValue() != wxEmptyString)
			m_addMaskOffBtn->Enable();
		else
			m_addMaskOffBtn->Disable();
		m_maskOffPathField->SetValue(currentKey->ImageOff.getRelativeMaskPath());

		m_widthSpin->SetValue(currentKey->Width);
		m_offsetSpin->SetValue(currentKey->Offset);
		m_offsetYSpin->SetValue(currentKey->YOffset);
		// update ranges of mouse spin controls before trying to set values
		m_mouseRectLeftSpin->SetRange(0, currentKey->Width - 1);
		m_mouseRectTopSpin->SetRange(0, currentKey->BitmapHeight - 1);
		m_mouseRectWidthSpin->SetRange(1, (currentKey->BitmapWidth - currentKey->MouseRectLeft));
		m_mouseRectHeightSpin->SetRange(1, (currentKey->BitmapHeight - currentKey->MouseRectTop));
		m_mouseRectLeftSpin->SetValue(currentKey->MouseRectLeft);
		m_mouseRectTopSpin->SetValue(currentKey->MouseRectTop);
		m_mouseRectWidthSpin->SetValue(currentKey->MouseRectWidth);
		m_mouseRectHeightSpin->SetValue(currentKey->MouseRectHeight);
		m_widthSpin->Enable();
		m_offsetSpin->Enable();
		m_offsetYSpin->Enable();
		if (currentKey->KeytypeIdentifier.StartsWith(wxT("Key"))) {
			m_mouseRectLeftSpin->Enable();
			m_mouseRectTopSpin->Enable();
			m_mouseRectWidthSpin->Enable();
			m_mouseRectHeightSpin->Enable();
			m_displayKeyChoice->SetSelection(m_manual->getIndexOfKeyNumber(currentKey->KeytypeIdentifier));
			// we need to notify the choice that selection has changed
			wxCommandEvent evt(wxEVT_CHOICE, ID_GUIMANUALPANEL_DISP_KEY_CHOICE);
			wxPostEvent(this, evt);
		} else {
			m_mouseRectLeftSpin->Disable();
			m_mouseRectTopSpin->Disable();
			m_mouseRectWidthSpin->Disable();
			m_mouseRectHeightSpin->Disable();
		}
	} else {
		m_imageOnPathField->SetValue(wxEmptyString);
		m_addImageOnBtn->Disable();
		m_maskOnPathField->SetValue(wxEmptyString);
		m_addMaskOnBtn->Disable();
		m_imageOffPathField->SetValue(wxEmptyString);
		m_addImageOffBtn->Disable();
		m_maskOffPathField->SetValue(wxEmptyString);
		m_addMaskOffBtn->Disable();
		m_widthSpin->SetValue(0);
		m_offsetSpin->SetValue(0);
		m_offsetYSpin->SetValue(0);
		m_mouseRectLeftSpin->SetValue(0);
		m_mouseRectTopSpin->SetValue(0);
		m_mouseRectWidthSpin->SetValue(1);
		m_mouseRectHeightSpin->SetValue(1);
		m_widthSpin->Disable();
		m_offsetSpin->Disable();
		m_offsetYSpin->Disable();
		m_mouseRectLeftSpin->Disable();
		m_mouseRectTopSpin->Disable();
		m_mouseRectWidthSpin->Disable();
		m_mouseRectHeightSpin->Disable();
	}

}

void GUIManualPanel::SetupKeyChoiceAndMapping() {
	// The available key number listbox should be updated
	if (!m_availableKeyNumbers->IsEmpty())
		m_availableKeyNumbers->Clear();
	m_availableKeyNumbers->InsertItems(m_manual->getAvailableKeynumbers(), 0);

	// The key mapping choice should be updated
	m_displayKeyChoice->Clear();
	wxArrayString keyNbrs;
	for (int i = 0; i < m_manual->getNumberOfDisplayKeys(); i++)
		keyNbrs.Add(GOODF_functions::number_format(i + 1));
	m_displayKeyChoice->Insert(keyNbrs, 0);
	m_displayKeyChoice->Select(0);
	m_backendMIDIkey->SetValue(m_manual->getDisplayKeyAt(0)->first);
	m_frontendMIDIkey->SetValue(m_manual->getDisplayKeyAt(0)->second);
}

void GUIManualPanel::UpdateAddedKeyTypes() {
	if (!m_addedKeyTypes->IsEmpty())
		m_addedKeyTypes->Clear();
	wxArrayString keys;
	for (unsigned i = 0; i < m_manual->getNumberOfKeytypes(); i++) {
		keys.Add(m_manual->getKeytypeAt(i)->KeytypeIdentifier);
	}
	if (!keys.IsEmpty())
		m_addedKeyTypes->InsertItems(keys, 0);
}
