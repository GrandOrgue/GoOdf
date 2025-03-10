/*
 * GoPanelPanel.cpp is part of GoOdf.
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

#include "GoPanelPanel.h"
#include "GOODFFunctions.h"
#include "GOODF.h"
#include "GoImage.h"
#include <wx/statline.h>
#include <wx/msgdlg.h>
#include "GUIElements.h"
#include "GUIManual.h"
#include "GUIStop.h"
#include "GUICoupler.h"
#include "GUIDivisional.h"
#include "GUIEnclosure.h"
#include "GUITremulant.h"
#include "GUISwitch.h"
#include "GUIReversiblePiston.h"
#include "GUIDivisionalCoupler.h"
#include "GUIGeneral.h"
#include "GUILabel.h"

// Event table
BEGIN_EVENT_TABLE(GoPanelPanel, wxPanel)
	EVT_TEXT(ID_PANEL_NAME_TEXT, GoPanelPanel::OnNameChange)
	EVT_TEXT(ID_PANEL_GROUP_TEXT, GoPanelPanel::OnGroupChange)
	EVT_RADIOBUTTON(ID_PANEL_HAS_PED_YES, GoPanelPanel::OnHasPedalRadio)
	EVT_RADIOBUTTON(ID_PANEL_HAS_PED_NO, GoPanelPanel::OnHasPedalRadio)
	EVT_BUTTON(ID_PANEL_ADD_IMAGE_BTN, GoPanelPanel::OnNewImageBtn)
	EVT_LISTBOX(ID_PANEL_ORGAN_ELEMENTS_CHOICE, GoPanelPanel::OnOrganElementChoice)
	EVT_LISTBOX(ID_PANEL_SETTER_ELEMENTS_CHOICE, GoPanelPanel::OnSetterElementChoice)
	EVT_BUTTON(ID_PANEL_ELEMENT_CHOICE_BTN, GoPanelPanel::OnElementChoiceBtn)
	EVT_BUTTON(ID_PANEL_LABEL_ELEMENT_BTN, GoPanelPanel::OnLabelBtn)
	EVT_BUTTON(ID_PANEL_REMOVE_BTN, GoPanelPanel::OnRemovePanelBtn)
	EVT_BUTTON(ID_PANEL_SHOW_PANEL_BTN, GoPanelPanel::OnShowPanelBtn)
END_EVENT_TABLE()

GoPanelPanel::GoPanelPanel(wxWindow *parent) : wxPanel(parent) {
	m_panel = NULL;
	m_isFirstRemoval = true;
	m_guiRepresentation = new GUIPanelRepresentation(this, wxT("Panel"));
	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *panelNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Name: ")
	);
	firstRow->Add(panelNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_PANEL_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	wxStaticText *panelGroupText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Group: ")
	);
	firstRow->Add(panelGroupText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_groupField = new wxTextCtrl(
		this,
		ID_PANEL_GROUP_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_groupField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *hasPedalText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Has Pedal: ")
	);
	secondRow->Add(hasPedalText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasPedalYes = new wxRadioButton(
		this,
		ID_PANEL_HAS_PED_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_hasPedalYes->SetValue(false);
	secondRow->Add(m_hasPedalYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_hasPedalNo = new wxRadioButton(
		this,
		ID_PANEL_HAS_PED_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_hasPedalNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	secondRow->AddStretchSpacer();
	m_addImageBtn = new wxButton(
		this,
		ID_PANEL_ADD_IMAGE_BTN,
		wxT("Add image to panel")
	);
	secondRow->Add(m_addImageBtn, 0, wxALIGN_CENTER|wxALL, 5);
	secondRow->AddStretchSpacer();
	m_showPanelBtn = new wxButton(
		this,
		ID_PANEL_SHOW_PANEL_BTN,
		wxT("Show panel layout")
	);
	secondRow->Add(m_showPanelBtn, 0, wxALIGN_CENTER|wxALL, 5);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *choiceRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *organElementsCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *organElementsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available Organ Elements")
	);
	organElementsCol->Add(organElementsText, 0, wxALL, 5);
	m_organElementsChoice = new wxListBox(
		this,
		ID_PANEL_ORGAN_ELEMENTS_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		0,
		NULL,
		wxLB_EXTENDED
	);
	organElementsCol->Add(m_organElementsChoice, 1, wxEXPAND|wxALL, 5);
	choiceRow->Add(organElementsCol, 1, wxEXPAND);

	wxBoxSizer *setterElementsCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *setterElementsText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available Setter Elements")
	);
	setterElementsCol->Add(setterElementsText, 0, wxALL, 5);
	m_setterElementsChoice = new wxListBox(
		this,
		ID_PANEL_SETTER_ELEMENTS_CHOICE,
		wxDefaultPosition,
		wxDefaultSize
	);
	setterElementsCol->Add(m_setterElementsChoice, 1, wxEXPAND|wxALL, 5);
	choiceRow->Add(setterElementsCol, 1, wxEXPAND);
	panelSizer->Add(choiceRow, 1, wxEXPAND);

	wxBoxSizer *buttonRow = new wxBoxSizer(wxHORIZONTAL);
	m_labelElementBtn = new wxButton(
		this,
		ID_PANEL_LABEL_ELEMENT_BTN,
		wxT("Create an ODF Label GUI element")
	);
	buttonRow->Add(m_labelElementBtn, 0, wxALIGN_CENTER|wxALL, 10);
	buttonRow->AddStretchSpacer();
	buttonRow->AddStretchSpacer();
	m_elementChoiceBtn = new wxButton(
		this,
		ID_PANEL_ELEMENT_CHOICE_BTN,
		wxT("Create a GUI element of any selected item above")
	);
	buttonRow->Add(m_elementChoiceBtn, 0, wxALIGN_CENTER|wxALL, 10);
	buttonRow->AddStretchSpacer();
	panelSizer->Add(buttonRow, 0, wxEXPAND);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *combinationRow = new wxBoxSizer(wxHORIZONTAL);
	combinationRow->AddStretchSpacer();
	wxStaticText *combinationManualText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Divisional for manual: ")
	);
	combinationRow->Add(combinationManualText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_manualChoice = new wxChoice(
		this,
		ID_PANEL_DIVISIONAL_MANUAL_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		availableManuals
	);
	m_manualChoice->Disable();
	combinationRow->Add(m_manualChoice, 0, wxALL, 5);
	combinationRow->AddStretchSpacer();
	wxStaticText *combinationNumberText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Combination number: ")
	);
	combinationRow->Add(combinationNumberText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_combinationNumberSpin = new wxSpinCtrl(
		this,
		ID_PANEL_COMBINATION_NBR_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		50,
		1
	);
	m_combinationNumberSpin->Disable();
	combinationRow->Add(m_combinationNumberSpin, 0, wxEXPAND|wxALL, 5);
	combinationRow->AddStretchSpacer();
	panelSizer->Add (combinationRow, 0, wxGROW);

	wxStaticLine *buttonDivider = new wxStaticLine(this);
	panelSizer->Add(buttonDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	m_removePanelBtn = new wxButton(
		this,
		ID_PANEL_REMOVE_BTN,
		wxT("Remove this panel")
	);
	bottomRow->Add(m_removePanelBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

GoPanelPanel::~GoPanelPanel() {
	if (m_guiRepresentation)
		m_guiRepresentation->Destroy();
}

void GoPanelPanel::setPanel(GoPanel *panel) {
	m_panel = panel;
	m_guiRepresentation->SetCurrentPanel(m_panel);
	if (m_guiRepresentation->IsShown()) {
		m_guiRepresentation->DoUpdateLayout();
	}
	// if this is the main panel certain things cannot be done/changed
	if (::wxGetApp().m_frame->m_organ->getOrganPanelAt(0) == m_panel) {
		m_nameField->ChangeValue(m_panel->getName());
		m_groupField->ChangeValue(m_panel->getGroup());
		m_nameField->Disable();
		m_groupField->Disable();
		m_removePanelBtn->Disable();
	} else {
		m_nameField->Enable();
		m_groupField->Enable();
		m_removePanelBtn->Enable();
		m_nameField->ChangeValue(m_panel->getName());
		m_groupField->ChangeValue(m_panel->getGroup());
	}
	if (m_panel->getHasPedals()) {
		m_hasPedalYes->SetValue(true);
		m_hasPedalNo->SetValue(false);
	} else {
		m_hasPedalYes->SetValue(false);
		m_hasPedalNo->SetValue(true);
	}
	if (!::wxGetApp().m_frame->m_organ->getOrganElements().IsEmpty()) {
		m_organElementsChoice->Clear();
		m_organElementsChoice->InsertItems(::wxGetApp().m_frame->m_organ->getOrganElements(), 0);
		m_organElementsChoice->Enable();
	} else {
		m_organElementsChoice->Clear();
		m_organElementsChoice->Insert(wxT("No organ elements created yet!"), 0);
		m_organElementsChoice->Disable();
	}
	m_setterElementsChoice->Clear();
	m_setterElementsChoice->InsertItems(::wxGetApp().m_frame->m_organ->getSetterElements(), 0);
	wxArrayInt selections;
	int count = m_organElementsChoice->GetSelections(selections);
	if (count < 1 && m_setterElementsChoice->GetSelection() == wxNOT_FOUND) {
		m_elementChoiceBtn->Disable();
	} else {
		m_elementChoiceBtn->Enable();
	}
	// update/populate available manuals
	if (!availableManuals.IsEmpty())
		availableManuals.Empty();
	unsigned nbManuals = ::wxGetApp().m_frame->m_organ->getNumberOfManuals();
	if (nbManuals > 0) {
		for (unsigned i = 0; i < nbManuals; i++)
			availableManuals.Add(::wxGetApp().m_frame->m_organ->getOrganManualAt(i)->getName());

		m_manualChoice->Clear();
		m_manualChoice->Insert(availableManuals, 0);
	}
	ShouldCombinationControlsBeEnabled();
	if (!m_manualChoice->IsEmpty())
		m_manualChoice->SetSelection(0);
	m_combinationNumberSpin->SetValue(1);
}

void GoPanelPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void GoPanelPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_panel->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GoPanelPanel::OnGroupChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_groupField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_groupField);
	m_panel->setGroup(m_groupField->GetValue());
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GoPanelPanel::OnHasPedalRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_PANEL_HAS_PED_YES) {
		m_hasPedalYes->SetValue(true);
		m_panel->setHasPedals(true);
	} else {
		m_hasPedalNo->SetValue(true);
		m_panel->setHasPedals(false);
	}
	updateRepresentationLayout();
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GoPanelPanel::OnNewImageBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_panel->getNumberOfImages() < 999) {
		GoImage image;
		image.setOwningPanelWidth(m_panel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue());
		image.setOwningPanelHeight(m_panel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());

		m_panel->addImage(image);

		::wxGetApp().m_frame->AddImageItemToTree();
	} else {
		wxMessageDialog msg(this, wxT("Panel cannot have more than 999 images!"), wxT("Too many images"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GoPanelPanel::OnRemovePanelBtn(wxCommandEvent& WXUNUSED(event)) {
	// The main panel may not be removed! Should be impossible, but checking anyway...
	if (m_isFirstRemoval) {
		if (::wxGetApp().m_frame->m_organ->getIndexOfOrganPanel(m_panel) > 1) {
			wxMessageDialog msg(this, wxT("Are you really sure you want to delete this panel?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (msg.ShowModal() == wxID_YES) {
				// then remove this panel
				::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
				m_isFirstRemoval = false;
			}
		}
	} else if (::wxGetApp().m_frame->m_organ->getIndexOfOrganPanel(m_panel) > 1) {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void GoPanelPanel::OnOrganElementChoice(wxCommandEvent& WXUNUSED(event)) {
	if (m_setterElementsChoice->GetSelection() != wxNOT_FOUND)
		m_setterElementsChoice->SetSelection(wxNOT_FOUND);

	m_elementChoiceBtn->Enable();
	ShouldCombinationControlsBeEnabled();
}

void GoPanelPanel::OnSetterElementChoice(wxCommandEvent& WXUNUSED(event)) {
	wxArrayInt selections;
	int count = m_organElementsChoice->GetSelections(selections);
	if (count > 0) {
		for (int i = 0; i < count; i++)
			m_organElementsChoice->Deselect(selections[i]);
	}

	m_elementChoiceBtn->Enable();
	ShouldCombinationControlsBeEnabled();
}

void GoPanelPanel::OnElementChoiceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_panel->getNumberOfGuiElements() < 999) {
		wxArrayInt selections;
		int count = m_organElementsChoice->GetSelections(selections);
		if (count > 0) {
			// At least one organ element was selected
			for (unsigned i = 0; i < selections.size(); i++) {
				std::pair<wxString, int> organElement = ::wxGetApp().m_frame->m_organ->getTypeAndIndexOfElement(selections[i]);

				if (organElement.first == wxT("Manual")) {
					GUIElement *choice = new GUIManual(::wxGetApp().m_frame->m_organ->getOrganManualAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				} else if (organElement.first == wxT("Stop")) {
					GUIElement *choice = new GUIStop(::wxGetApp().m_frame->m_organ->getOrganStopAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				} else if (organElement.first == wxT("Coupler")) {
					GUIElement *choice = new GUICoupler(::wxGetApp().m_frame->m_organ->getOrganCouplerAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				} else if (organElement.first == wxT("Divisional")) {
					GUIElement *choice = new GUIDivisional(::wxGetApp().m_frame->m_organ->getOrganDivisionalAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				} else if (organElement.first == wxT("Enclosure")) {
					GUIElement *choice = new GUIEnclosure(::wxGetApp().m_frame->m_organ->getOrganEnclosureAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				} else if (organElement.first == wxT("Tremulant")) {
					GUIElement *choice = new GUITremulant(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				} else if (organElement.first == wxT("Switch")) {
					GUIElement *choice = new GUISwitch(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				} else if (organElement.first == wxT("ReversiblePiston")) {
					GUIElement *choice = new GUIReversiblePiston(::wxGetApp().m_frame->m_organ->getReversiblePistonAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				} else if (organElement.first == wxT("DivisionalCoupler")) {
					GUIElement *choice = new GUIDivisionalCoupler(::wxGetApp().m_frame->m_organ->getOrganDivisionalCouplerAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				} else if (organElement.first == wxT("General")) {
					GUIElement *choice = new GUIGeneral(::wxGetApp().m_frame->m_organ->getOrganGeneralAt(organElement.second));
					choice->setOwningPanel(m_panel);
					choice->updateDisplayName();
					choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
					m_panel->addGuiElement(choice);
					::wxGetApp().m_frame->AddGuiElementToTree(choice->getDisplayName());
				}
			}
		} else if (m_setterElementsChoice->GetSelection() != wxNOT_FOUND) {
			// A setter element was selected
			int selectedIndex = m_setterElementsChoice->GetSelection();
			wxString setterType = ::wxGetApp().m_frame->m_organ->getSetterElements().Item(selectedIndex);
			// depending on setter type, different gui elements are needed
			if (setterType == wxT("CrescendoLabel") ||
				setterType == wxT("CurrFileName") ||
				setterType == wxT("GeneralLabel") ||
				setterType == wxT("PitchLabel") ||
				setterType == wxT("SequencerLabel") ||
				setterType == wxT("TemperamentLabel") ||
				setterType == wxT("TransposeLabel") ) {
				// this setter element is a type of label
				GUIElement *choice = new GUILabel();
				choice->setType(setterType);
				choice->setOwningPanel(m_panel);
				choice->setDisplayName(setterType);
				choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispGroupLabelFont);
				m_panel->addGuiElement(choice);
				::wxGetApp().m_frame->AddGuiElementToTree(setterType);
			} else if (setterType == wxT("SetterXXXDivisionalBank")) {
				// we need to get the manual
				int targetManual = 0;
				if (!m_manualChoice->IsEmpty()) {
					targetManual = m_manualChoice->GetSelection();
				}
				if (!(::wxGetApp().m_frame->m_organ->doesHavePedals()))
					targetManual += 1;
				wxString typeName = wxT("Setter") + GOODF_functions::number_format(targetManual) + wxT("DivisionalBank");
				GUIElement *choice = new GUILabel();
				choice->setOwningPanel(m_panel);
				choice->setType(typeName);
				choice->setDisplayName(typeName);
				choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispGroupLabelFont);
				m_panel->addGuiElement(choice);
				::wxGetApp().m_frame->AddGuiElementToTree(typeName);
			} else if (setterType == wxT("Swell")) {
				GUIElement *choice = new GUIEnclosure(NULL);
				choice->setOwningPanel(m_panel);
				choice->setType(setterType);
				choice->setDisplayName(setterType);
				m_panel->addGuiElement(choice);
				::wxGetApp().m_frame->AddGuiElementToTree(setterType);
			} else if (setterType == wxT("GeneralXX")) {
				// we need to get the number for XX
				int combinationNbr = m_combinationNumberSpin->GetValue();
				wxString generalName = wxT("General") + wxString::Format(wxT("%0.2i"), combinationNbr);
				GUIElement *choice = new GUIGeneral(NULL);
				choice->setOwningPanel(m_panel);
				choice->setType(generalName);
				choice->setDisplayName(generalName);
				choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
				m_panel->addGuiElement(choice);
				::wxGetApp().m_frame->AddGuiElementToTree(generalName);
				m_combinationNumberSpin->SetValue(combinationNbr);
			} else if (setterType == wxT("SetterXXXDivisionalYYY")) {
				// we need to get both manual (three X) and the divisional number YYY
				int targetManual = 0;
				if (!m_manualChoice->IsEmpty()) {
					targetManual = m_manualChoice->GetSelection();
				}
				if (!(::wxGetApp().m_frame->m_organ->doesHavePedals()))
					targetManual += 1;
				int combinationNbr = m_combinationNumberSpin->GetValue();
				wxString divisionalName = wxT("Setter") + GOODF_functions::number_format(targetManual) + wxT("Divisional") + GOODF_functions::number_format(combinationNbr);
				GUIElement *choice = new GUIDivisional(NULL);
				choice->setOwningPanel(m_panel);
				choice->setType(divisionalName);
				choice->setDisplayName(divisionalName);
				choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
				m_panel->addGuiElement(choice);
				::wxGetApp().m_frame->AddGuiElementToTree(divisionalName);
				m_manualChoice->SetSelection(targetManual);
				m_combinationNumberSpin->SetValue(combinationNbr);
			} else if (setterType == wxT("SetterXXXDivisionalPrevBank")) {
				// we need to get the manual
				int targetManual = 0;
				if (!m_manualChoice->IsEmpty()) {
					targetManual = m_manualChoice->GetSelection();
				}
				if (!(::wxGetApp().m_frame->m_organ->doesHavePedals()))
					targetManual += 1;
				wxString typeName = wxT("Setter") + GOODF_functions::number_format(targetManual) + wxT("DivisionalPrevBank");
				GUIElement *choice = new GUIDivisional(NULL);
				choice->setOwningPanel(m_panel);
				choice->setType(typeName);
				choice->setDisplayName(typeName);
				choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
				m_panel->addGuiElement(choice);
				::wxGetApp().m_frame->AddGuiElementToTree(typeName);
			} else if (setterType == wxT("SetterXXXDivisionalNextBank")) {
				// we need to get the manual
				int targetManual = 0;
				if (!m_manualChoice->IsEmpty()) {
					targetManual = m_manualChoice->GetSelection();
				}
				if (!(::wxGetApp().m_frame->m_organ->doesHavePedals()))
					targetManual += 1;
				wxString typeName = wxT("Setter") + GOODF_functions::number_format(targetManual) + wxT("DivisionalNextBank");
				GUIElement *choice = new GUIDivisional(NULL);
				choice->setOwningPanel(m_panel);
				choice->setType(typeName);
				choice->setDisplayName(typeName);
				choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
				m_panel->addGuiElement(choice);
				::wxGetApp().m_frame->AddGuiElementToTree(typeName);
			} else {
				// any other type we create as a gui switch
				GUIElement *choice = new GUISwitch(NULL);
				choice->setOwningPanel(m_panel);
				choice->setType(setterType);
				choice->setDisplayName(setterType);
				choice->setDefaultFont(m_panel->getDisplayMetrics()->m_dispControlLabelFont);
				m_panel->addGuiElement(choice);
				::wxGetApp().m_frame->AddGuiElementToTree(setterType);
			}
			m_setterElementsChoice->SetSelection(selectedIndex);
			wxCommandEvent evt(wxEVT_LISTBOX, ID_PANEL_SETTER_ELEMENTS_CHOICE);
			wxPostEvent(this, evt);
		}
		::wxGetApp().m_frame->m_organ->setModified(true);
	} else {
		wxMessageDialog msg(this, wxT("Panel cannot have more than 999 gui elements!"), wxT("Too many gui elements"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
	m_guiRepresentation->DoUpdateLayout();
}

void GoPanelPanel::OnLabelBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_panel->getNumberOfGuiElements() < 999) {
		GUIElement *label = new GUILabel();
		label->setOwningPanel(m_panel);
		label->setDisplayName(wxT("GUI Label"));
		label->setDefaultFont(m_panel->getDisplayMetrics()->m_dispGroupLabelFont);
		m_panel->addGuiElement(label);

		::wxGetApp().m_frame->AddGuiElementToTree(wxT("GUI Label"));
		m_guiRepresentation->DoUpdateLayout();
	} else {
		wxMessageDialog msg(this, wxT("Panel cannot have more than 999 gui elements!"), wxT("Too many gui elements"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GoPanelPanel::OnShowPanelBtn(wxCommandEvent& WXUNUSED(event)) {
	if (!m_guiRepresentation->IsShown()) {
		m_guiRepresentation->Destroy();
		m_guiRepresentation = new GUIPanelRepresentation(this, wxT("Panel"));
		m_guiRepresentation->SetCurrentPanel(m_panel);
		m_guiRepresentation->Show();
		m_guiRepresentation->DoUpdateLayout();
	}
}

void GoPanelPanel::ShouldCombinationControlsBeEnabled() {
	if (m_setterElementsChoice->GetSelection() != wxNOT_FOUND) {
		// A setter element was selected
		int selectedIndex = m_setterElementsChoice->GetSelection();
		wxString setterType = ::wxGetApp().m_frame->m_organ->getSetterElements().Item(selectedIndex);
		if (setterType == wxT("GeneralXX")) {
			m_manualChoice->Disable();
			m_combinationNumberSpin->SetRange(1, 50);
			m_combinationNumberSpin->Enable();
		} else if (setterType == wxT("SetterXXXDivisionalYYY")) {
			m_manualChoice->Enable();
			m_combinationNumberSpin->SetRange(0, 999);
			m_combinationNumberSpin->Enable();
		} else if (setterType == wxT("SetterXXXDivisionalBank") ||
			setterType == wxT("SetterXXXDivisionalNextBank") ||
			setterType == wxT("SetterXXXDivisionalPrevBank")) {
			// only manual selection should be enabled
			m_manualChoice->Enable();
			m_combinationNumberSpin->SetRange(0, 999);
			m_combinationNumberSpin->Disable();
		} else {
			m_manualChoice->Disable();
			m_combinationNumberSpin->Disable();
		}
	} else {
		m_manualChoice->Disable();
		m_combinationNumberSpin->Disable();
	}
}

void GoPanelPanel::updateRepresentationLayout() {
	if (m_guiRepresentation->IsShown())
		m_guiRepresentation->DoUpdateLayout();
}

void GoPanelPanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_groupField->SetToolTip(wxT("If a group is entered, this panel will be put in a sub menu under that group name in the Panel menu in GrandOrgue. Cannot be set for Main Panel!"));
		m_hasPedalYes->SetToolTip(wxT("If built-in bitmaps and/or built-in layout of GrandOrgue are used the first added manual will be displayed as a pedal."));
		m_hasPedalNo->SetToolTip(wxT("All manuals will be displayed as manuals if built-in bitmaps and/or built-in layout of GrandOrgue are used."));
		m_addImageBtn->SetToolTip(wxT("Custom images can be added to the panel."));
		m_organElementsChoice->SetToolTip(wxT("Here all the available elements of the organ defined above the Panels in the tree will be shown. One or many can be selected and added to the panel as GUI Elements with the button below."));
		m_setterElementsChoice->SetToolTip(wxT("The built-in setter elements of GrandOrgue can be selected and added to the panel as GUI Elements with the button below. NOTE: The combination elements need to have some additional information set below to be functioning as intended."));
		m_labelElementBtn->SetToolTip(wxT("Label elements can be added to the panel as GUI Elements."));
		m_manualChoice->SetToolTip(wxT("For the setter divisionals the manual they affect is chosen here."));
		m_combinationNumberSpin->SetToolTip(wxT("For the setter generals and divisionals the combination number should be set here. NOTE: The divisionals are numbered starting with zero indexing, thus divisional 1 is actually entered as 0, divisional 2 is entered as 1 etc."));
		m_showPanelBtn->SetToolTip(wxT("A graphical rendering of the panel will be shown where adjustments of positioning of elements can be done (which will change the layout method used to absolute positioning for adjusted elements)."));
	} else {
		m_groupField->SetToolTip(wxEmptyString);
		m_hasPedalYes->SetToolTip(wxEmptyString);
		m_hasPedalNo->SetToolTip(wxEmptyString);
		m_addImageBtn->SetToolTip(wxEmptyString);
		m_organElementsChoice->SetToolTip(wxEmptyString);
		m_setterElementsChoice->SetToolTip(wxEmptyString);
		m_elementChoiceBtn->SetToolTip(wxEmptyString);
		m_labelElementBtn->SetToolTip(wxEmptyString);
		m_manualChoice->SetToolTip(wxEmptyString);
		m_combinationNumberSpin->SetToolTip(wxEmptyString);
		m_showPanelBtn->SetToolTip(wxEmptyString);
	}
}

void GoPanelPanel::refreshData() {
	m_nameField->ChangeValue(m_panel->getName());
	m_groupField->ChangeValue(m_panel->getGroup());
}
