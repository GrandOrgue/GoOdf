/*
 * GUIManualPanel.cpp is part of GoOdf.
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
	EVT_RADIOBUTTON(ID_GUIMANUALPANEL_WRITE_WIDTH_YES, GUIManualPanel::OnForceWriteWidthRadio)
	EVT_RADIOBUTTON(ID_GUIMANUALPANEL_WRITE_WIDTH_NO, GUIManualPanel::OnForceWriteWidthRadio)
	EVT_COMBOBOX(ID_GUIMANUALPANEL_IMAGE_NBR_BOX, GUIManualPanel::OnImageNumberChoice)
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
	firstRow->AddStretchSpacer();
	wxStaticText *writeWidthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Force writing width in odf: ")
	);
	firstRow->Add(writeWidthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_forceWriteWidthYes = new wxRadioButton(
		this,
		ID_GUIMANUALPANEL_WRITE_WIDTH_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	firstRow->Add(m_forceWriteWidthYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_forceWriteWidthNo = new wxRadioButton(
		this,
		ID_GUIMANUALPANEL_WRITE_WIDTH_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_forceWriteWidthNo->SetValue(true);
	firstRow->Add(m_forceWriteWidthNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *positionRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imageNbrText = new wxStaticText(
		this,
		wxID_STATIC,
		wxT("DispImageNum: ")
	);
	positionRow->Add(imageNbrText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_dispImageNbrBox = new wxBitmapComboBox(
		this,
		ID_GUIMANUALPANEL_IMAGE_NBR_BOX
	);
	for (unsigned i = 0; i < ::wxGetApp().m_defaultManualBitmaps.size(); i++) {
		wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
		m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_defaultManualBitmaps[i]);
	}
	positionRow->Add(m_dispImageNbrBox, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	positionRow->AddStretchSpacer();
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
		-500,
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
	m_forceWriteWidthNo->SetValue(true);
	m_forceWriteWidthYes->Disable();
	m_forceWriteWidthNo->Disable();
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
	SetupImageNbrBoxContent();
}

void GUIManualPanel::updatePositionValues() {
	if (m_manual) {
		m_elementPosXSpin->SetValue(m_manual->getPosX());
		m_elementPosYSpin->SetValue(m_manual->getPosY());
	}
}

void GUIManualPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_keyColorInvertedYes->SetToolTip(wxT("Use black natural keys for the built-in bitmaps."));
		m_keyColorInvertedNo->SetToolTip(wxT("Use white natural keys for the built-in bitmaps."));
		m_keyColorWoodYes->SetToolTip(wxT("Use wooden coloured natural keys for the built-in bitmaps."));
		m_keyColorWoodNo->SetToolTip(wxT("Use normal white or black natural keys for the built-in bitmaps."));
		m_forceWriteWidthYes->SetToolTip(wxT("This option is only active if a read odf would require the widths to be written."));
		m_forceWriteWidthNo->SetToolTip(wxT("Don't force writing widths to odf. Might change the layout of the keys unexpectedly."));
		m_elementPosXSpin->SetToolTip(wxT("Absolute positioning X coordinate value for the manual. If set to -1 the built-in layout will be used instead."));
		m_elementPosYSpin->SetToolTip(wxT("Absolute positioning Y coordinate value for the manual. If set to -1 the built-in layout will be used instead."));
		m_dispImageNbrBox->SetToolTip(wxT("Choose the built-in key bitmap type here."));
		m_availableKeyTypes->SetToolTip(wxT("The available basic keytypes are listed here."));
		m_availableKeyNumbers->SetToolTip(wxT("The available individual key numbers are listed here."));
		m_addKey->SetToolTip(wxT("Add a selected key type or number."));
		m_removeKey->SetToolTip(wxT("Remove a previously added key type or number."));
		m_copyKey->SetToolTip(wxT("Bring up a dialog for copying key attributes/bitmaps to selected targets."));
		m_addedKeyTypes->SetToolTip(wxT("A list of the added key types and/or key numbers."));
		m_imageOnPathField->SetToolTip(wxT("Relative file path will be displayed here. Use 'Browse...' button to set."));
		m_addImageOnBtn->SetToolTip(wxT("Opens a file dialog where the image 'On' bitmap can be chosen. The .png format is the preferred format for images in GrandOrgue. NOTE: All added bitmaps for the element must have the same pixel size!"));
		m_imageOffPathField->SetToolTip(wxT("Relative file path will be displayed here. Use 'Browse...' button to set."));
		m_addImageOffBtn->SetToolTip(wxT("Opens a file dialog where the image 'Off' bitmap can be chosen. The .png format is the preferred format for images in GrandOrgue. NOTE: All added bitmaps for the element must have the same pixel size!"));
		m_maskOnPathField->SetToolTip(wxT("Relative file path will be displayed here. Use 'Browse...' button to set."));
		m_addMaskOnBtn->SetToolTip(wxT("Opens a file dialog where a mask for the image 'On' bitmap can be chosen. NOTE: All added bitmaps for the element must have the same pixel size!"));
		m_maskOffPathField->SetToolTip(wxT("Relative file path will be displayed here. Use 'Browse...' button to set."));
		m_addMaskOffBtn->SetToolTip(wxT("Opens a file dialog where a mask for the image 'Off' bitmap can be chosen. NOTE: All added bitmaps for the element must have the same pixel size!"));
		m_widthSpin->SetToolTip(wxT("Width of bitmap to use for layout calculations. NOTE: Changing this value does not scale or crop the image! It only decides where the next key will be placed (pedal and manual key layout differ slightly)."));
		m_offsetSpin->SetToolTip(wxT("Offset key on the X-axis from the normal layout."));
		m_offsetYSpin->SetToolTip(wxT("Offset key on the Y-axis from the normal layout."));
		m_mouseRectLeftSpin->SetToolTip(wxT("Controls the left coordinate for where the area of mouse interaction begin."));
		m_mouseRectTopSpin->SetToolTip(wxT("Controls the upper coordinate for where the area of mouse interaction begin."));
		m_mouseRectWidthSpin->SetToolTip(wxT("Set the width of the area of where mouse interaction is possible."));
		m_mouseRectHeightSpin->SetToolTip(wxT("Set the height of the area of where mouse interaction is possible."));
		m_displayKeysSpin->SetToolTip(wxT("Number of keys to display."));
		m_firstNoteSpin->SetToolTip(wxT("Display first key as the following (MIDI note) type."));
		m_displayKeyChoice->SetToolTip(wxT("Map the selected key."));
		m_backendMIDIkey->SetToolTip(wxT("Backend MIDI note key for the manual."));
		m_frontendMIDIkey->SetToolTip(wxT("Display the key as this MIDI note key."));
	} else {
		m_keyColorInvertedYes->SetToolTip(wxEmptyString);
		m_keyColorInvertedNo->SetToolTip(wxEmptyString);
		m_keyColorWoodYes->SetToolTip(wxEmptyString);
		m_keyColorWoodNo->SetToolTip(wxEmptyString);
		m_forceWriteWidthYes->SetToolTip(wxEmptyString);
		m_forceWriteWidthNo->SetToolTip(wxEmptyString);
		m_elementPosXSpin->SetToolTip(wxEmptyString);
		m_elementPosYSpin->SetToolTip(wxEmptyString);
		m_dispImageNbrBox->SetToolTip(wxEmptyString);
		m_availableKeyTypes->SetToolTip(wxEmptyString);
		m_availableKeyNumbers->SetToolTip(wxEmptyString);
		m_addKey->SetToolTip(wxEmptyString);
		m_removeKey->SetToolTip(wxEmptyString);
		m_copyKey->SetToolTip(wxEmptyString);
		m_addedKeyTypes->SetToolTip(wxEmptyString);
		m_imageOnPathField->SetToolTip(wxEmptyString);
		m_addImageOnBtn->SetToolTip(wxEmptyString);
		m_imageOffPathField->SetToolTip(wxEmptyString);
		m_addImageOffBtn->SetToolTip(wxEmptyString);
		m_maskOnPathField->SetToolTip(wxEmptyString);
		m_addMaskOnBtn->SetToolTip(wxEmptyString);
		m_maskOffPathField->SetToolTip(wxEmptyString);
		m_addMaskOffBtn->SetToolTip(wxEmptyString);
		m_widthSpin->SetToolTip(wxEmptyString);
		m_offsetSpin->SetToolTip(wxEmptyString);
		m_offsetYSpin->SetToolTip(wxEmptyString);
		m_mouseRectLeftSpin->SetToolTip(wxEmptyString);
		m_mouseRectTopSpin->SetToolTip(wxEmptyString);
		m_mouseRectWidthSpin->SetToolTip(wxEmptyString);
		m_mouseRectHeightSpin->SetToolTip(wxEmptyString);
		m_displayKeysSpin->SetToolTip(wxEmptyString);
		m_firstNoteSpin->SetToolTip(wxEmptyString);
		m_displayKeyChoice->SetToolTip(wxEmptyString);
		m_backendMIDIkey->SetToolTip(wxEmptyString);
		m_frontendMIDIkey->SetToolTip(wxEmptyString);
	}
}

void GUIManualPanel::OnKeyColorInvertedRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIMANUALPANEL_COLOR_INVERTED_YES) {
		m_manual->setDispKeyColourInverted(true);
	} else {
		m_manual->setDispKeyColourInverted(false);
	}
	SetupImageNbrBoxContent();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIManualPanel::OnKeyColorWoodRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_GUIMANUALPANEL_COLOR_WOOD_YES) {
		m_manual->setDispKeyColurWooden(true);
	} else {
		m_manual->setDispKeyColurWooden(false);
	}
	SetupImageNbrBoxContent();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIManualPanel::OnForceWriteWidthRadio(wxCommandEvent& event) {
	int selectedIndex = m_addedKeyTypes->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		KEYTYPE *currentKey = m_manual->getKeytypeAt(selectedIndex);

		if (event.GetId() == ID_GUIMANUALPANEL_WRITE_WIDTH_YES) {
			currentKey->ForceWritingWidth = true;
		} else {
			currentKey->ForceWritingWidth = false;
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIManualPanel::OnImageNumberChoice(wxCommandEvent& WXUNUSED(event)) {
	m_manual->setDispImageNum(m_dispImageNbrBox->GetSelection() + 1);
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIManualPanel::OnPositionXSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setPosX(m_elementPosXSpin->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIManualPanel::OnPositionYSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setPosY(m_elementPosYSpin->GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
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
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
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
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
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
		m_manual->updateKeyInfo();
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
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
	::wxGetApp().m_frame->m_organ->setModified(true);
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
				m_manual->updateKeyInfo();
				UpdateExistingSelectedKeyData();
				::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
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
				::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
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
	::wxGetApp().m_frame->m_organ->setModified(true);
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
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIManualPanel::OnWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->Width = m_widthSpin->GetValue();
	m_manual->updateKeyInfo();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIManualPanel::OnOffsetSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->Offset = m_offsetSpin->GetValue();
	m_manual->updateKeyInfo();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIManualPanel::OnOffsetYSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->YOffset = m_offsetYSpin->GetValue();
	m_manual->updateKeyInfo();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIManualPanel::OnMouseRectLeftSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->MouseRectLeft = m_mouseRectLeftSpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIManualPanel::OnMouseRectTopSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->MouseRectTop = m_mouseRectTopSpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIManualPanel::OnMouseRectWidthSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->MouseRectWidth = m_mouseRectWidthSpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIManualPanel::OnMouseRectHeightSpin(wxSpinEvent& WXUNUSED(event)) {
	KEYTYPE *key = m_manual->getKeytypeAt(m_addedKeyTypes->GetSelection());
	key->MouseRectHeight = m_mouseRectHeightSpin->GetValue();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GUIManualPanel::OnDisplayKeysSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setNumberOfDisplayKeys(m_displayKeysSpin->GetValue());
	// this might have affect added keys and mapping
	SetupKeyChoiceAndMapping();
	UpdateAddedKeyTypes();
	UpdateExistingSelectedKeyData();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIManualPanel::OnFirstNoteSpin(wxSpinEvent& WXUNUSED(event)) {
	m_manual->setDisplayFirstNote(m_firstNoteSpin->GetValue());
	// this might have affect added keys and mapping
	SetupKeyChoiceAndMapping();
	UpdateAddedKeyTypes();
	UpdateExistingSelectedKeyData();
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
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
		::wxGetApp().m_frame->m_organ->setModified(true);
	}
}

void GUIManualPanel::OnFrontendMIDIkeySpin(wxSpinEvent& WXUNUSED(event)) {
	int selectedIndex = m_displayKeyChoice->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		m_manual->getDisplayKeyAt(selectedIndex)->second = m_frontendMIDIkey->GetValue();
		m_manual->updateKeyInfo();
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GUIManualPanel::OnRemoveManualBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_manual->getOwningPanel()->getIsGuiElementFirstRemoval()) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this element?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			m_manual->getOwningPanel()->setIsGuiElementFirstRemoval(false);
			// then remove this item from the owning panel
			::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
		}
	} else {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
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
	m_lastUsedPath = imageFilePath;
	return imageFilePath;
}

void GUIManualPanel::UpdateExistingSelectedKeyData() {
	// this method is called every time a new selection of a added key is made
	int selectedIndex = m_addedKeyTypes->GetSelection();
	if (selectedIndex != wxNOT_FOUND) {
		KEYTYPE *currentKey = m_manual->getKeytypeAt(selectedIndex);
		m_forceWriteWidthYes->Enable();
		m_forceWriteWidthNo->Enable();
		if (currentKey->ForceWritingWidth)
			m_forceWriteWidthYes->SetValue(true);
		else
			m_forceWriteWidthNo->SetValue(true);
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
		m_forceWriteWidthNo->SetValue(true);
		m_forceWriteWidthYes->Disable();
		m_forceWriteWidthNo->Disable();
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

void GUIManualPanel::SetupImageNbrBoxContent() {
	if (m_manual->getOwningPanel()->getHasPedals() && m_manual->getOwningPanel()->getIsGuiManualTheFirst(m_manual)) {
		// this manual is displayed as a pedal
		m_dispImageNbrBox->Clear();
		if (!m_manual->getDispKeyColourInverted()) {
			// this is the default display, which also is the same for wooden
			for (unsigned i = 0; i < ::wxGetApp().m_defaultPedalBitmaps.size(); i++) {
				wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
				m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_defaultPedalBitmaps[i]);
			}
		} else {
			// this is an inverted display, which also is the same for inverted wooden
			for (unsigned i = 0; i < ::wxGetApp().m_invertedPedalBitmaps.size(); i++) {
				wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
				m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_invertedPedalBitmaps[i]);
			}
		}
	} else {
		// this manual is displayed as a manual
		m_dispImageNbrBox->Clear();
		if (!m_manual->getDispKeyColourInverted() && !m_manual->getDispKeyColourWooden()) {
			// this is the default display
			for (unsigned i = 0; i < ::wxGetApp().m_defaultManualBitmaps.size(); i++) {
				wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
				m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_defaultManualBitmaps[i]);
			}
		} else if (m_manual->getDispKeyColourInverted() && !m_manual->getDispKeyColourWooden()) {
			// this is a normal inverted display
			for (unsigned i = 0; i < ::wxGetApp().m_invertedManualBitmaps.size(); i++) {
				wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
				m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_invertedManualBitmaps[i]);
			}
		} else if (!m_manual->getDispKeyColourInverted() && m_manual->getDispKeyColourWooden()) {
			// this is a wooden display
			for (unsigned i = 0; i < ::wxGetApp().m_woodenManualBitmaps.size(); i++) {
				wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
				m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_woodenManualBitmaps[i]);
			}
		} else if (m_manual->getDispKeyColourInverted() && m_manual->getDispKeyColourWooden()) {
			// this is an inverted wooden display
			for (unsigned i = 0; i < ::wxGetApp().m_invertedWoodenManualBitmaps.size(); i++) {
				wxString imgNumber = wxString::Format(wxT("%d"), i + 1);
				m_dispImageNbrBox->Append(imgNumber, ::wxGetApp().m_invertedWoodenManualBitmaps[i]);
			}
		}
	}
	m_dispImageNbrBox->SetSelection(m_manual->getDispImageNum() - 1);
}
