/*
 * DivisionalCouplerPanel.cpp is part of GOODF.
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

#include "DivisionalCouplerPanel.h"
#include <wx/statline.h>
#include "GOODFFunctions.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include <wx/msgdlg.h>

// Event table
BEGIN_EVENT_TABLE(DivisionalCouplerPanel, wxPanel)
	EVT_TEXT(ID_DIV_CPLR_NAME_TEXT, DivisionalCouplerPanel::OnNameChange)
	EVT_RADIOBUTTON(ID_DIV_CPLR_INVERTED_STATE_YES, DivisionalCouplerPanel::OnDisplayInvertedRadio)
	EVT_RADIOBUTTON(ID_DIV_CPLR_INVERTED_STATE_NO, DivisionalCouplerPanel::OnDisplayInvertedRadio)
	EVT_CHOICE(ID_DIV_CPLR_FUNCTION_CHOICE, DivisionalCouplerPanel::OnFunctionChange)
	EVT_CHOICE(ID_DIV_CPLR_GC_STATE_CHOICE, DivisionalCouplerPanel::OnGcStateChange)
	EVT_RADIOBUTTON(ID_DIV_CPLR_BIDIRECTIONAL_YES, DivisionalCouplerPanel::OnBidirectionalChange)
	EVT_RADIOBUTTON(ID_DIV_CPLR_BIDIRECTIONAL_NO, DivisionalCouplerPanel::OnBidirectionalChange)
	EVT_RADIOBUTTON(ID_DIV_CPLR_STORE_IN_GEN_YES, DivisionalCouplerPanel::OnStoreInGeneralChange)
	EVT_RADIOBUTTON(ID_DIV_CPLR_STORE_IN_GEN_NO, DivisionalCouplerPanel::OnStoreInGeneralChange)
	EVT_RADIOBUTTON(ID_DIV_CPLR_DEFAULT_TO_ENGAGED_YES, DivisionalCouplerPanel::OnDefaultToEngagedChange)
	EVT_RADIOBUTTON(ID_DIV_CPLR_DEFAULT_TO_ENGAGED_NO, DivisionalCouplerPanel::OnDefaultToEngagedChange)
	EVT_BUTTON(ID_DIV_CPLR_REMOVE_BTN, DivisionalCouplerPanel::OnRemoveDivisionalCouplerBtn)
	EVT_BUTTON(ID_DIV_CPLR_ADD_SWITCH_BTN, DivisionalCouplerPanel::OnAddSwitchReferenceBtn)
	EVT_BUTTON(ID_DIV_CPLR_REMOVE_SWITCH_BTN, DivisionalCouplerPanel::OnRemoveSwitchReferenceBtn)
	EVT_LISTBOX(ID_DIV_CPLR_AVAILABLE_SWITCHES, DivisionalCouplerPanel::OnSwitchListboxSelection)
	EVT_LISTBOX(ID_DIV_CPLR_REFERENCED_SWITCHES, DivisionalCouplerPanel::OnReferencedSwitchSelection)
	EVT_BUTTON(ID_DIV_CPLR_ADD_MAN_BTN, DivisionalCouplerPanel::OnAddReferencedManual)
	EVT_BUTTON(ID_DIV_CPLR_REMOVE_MAN_BTN, DivisionalCouplerPanel::OnRemoveReferencedManual)
	EVT_LISTBOX(ID_DIV_CPLR_AVAILABLE_MAN, DivisionalCouplerPanel::OnManualListboxSelection)
	EVT_LISTBOX(ID_DIV_CPLR_REFERENCED_MAN, DivisionalCouplerPanel::OnReferencedManualSelection)
END_EVENT_TABLE()

DivisionalCouplerPanel::DivisionalCouplerPanel(wxWindow *parent) : wxPanel(parent) {
	m_divCplr = NULL;
	m_isFirstRemoval = true;
	functionChoices.Add(wxT("Input"));
	functionChoices.Add(wxT("And"));
	functionChoices.Add(wxT("Nand"));
	functionChoices.Add(wxT("Not"));
	functionChoices.Add(wxT("Nor"));
	functionChoices.Add(wxT("Or"));
	functionChoices.Add(wxT("Xor"));
	gcStateChoices.Add(wxT("No change"));
	gcStateChoices.Add(wxT("Off"));
	gcStateChoices.Add(wxT("On"));

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *divCplrNameText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Divisional coupler name: ")
	);
	firstRow->Add(divCplrNameText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_nameField = new wxTextCtrl(
		this,
		ID_DIV_CPLR_NAME_TEXT,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize
	);
	firstRow->Add(m_nameField, 1, wxEXPAND|wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *functionText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Function: ")
	);
	secondRow->Add(functionText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_functionChoice = new wxChoice(
		this,
		ID_DIV_CPLR_FUNCTION_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		functionChoices
	);
	secondRow->Add(m_functionChoice, 0, wxALL, 5);
	secondRow->AddStretchSpacer();
	wxStaticText *dispInvertedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Display in inverted state: ")
	);
	secondRow->Add(dispInvertedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedYes = new wxRadioButton(
		this,
		ID_DIV_CPLR_INVERTED_STATE_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	secondRow->Add(m_displayInvertedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo = new wxRadioButton(
		this,
		ID_DIV_CPLR_INVERTED_STATE_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	secondRow->Add(m_displayInvertedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_displayInvertedNo->SetValue(true);
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *thirdRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available switches")
	);
	thirdRow1stCol->Add(availableReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableSwitches = new wxListBox(
		this,
		ID_DIV_CPLR_AVAILABLE_SWITCHES
	);
	thirdRow1stCol->Add(m_availableSwitches, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow1stCol, 1, wxEXPAND);
	wxBoxSizer *thirdRow2ndCol = new wxBoxSizer(wxVERTICAL);
	thirdRow2ndCol->AddStretchSpacer();
	m_addReferencedSwitch = new wxButton(
		this,
		ID_DIV_CPLR_ADD_SWITCH_BTN,
		wxT("Add ->")
	);
	thirdRow2ndCol->Add(m_addReferencedSwitch, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	m_removeReferencedSwitch = new wxButton(
		this,
		ID_DIV_CPLR_REMOVE_SWITCH_BTN,
		wxT("<- Remove")
	);
	thirdRow2ndCol->Add(m_removeReferencedSwitch, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	thirdRow2ndCol->AddStretchSpacer();
	thirdRow->Add(thirdRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *thirdRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced switches")
	);
	thirdRow3rdCol->Add(chosenReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedSwitches = new wxListBox(
		this,
		ID_DIV_CPLR_REFERENCED_SWITCHES
	);
	thirdRow3rdCol->Add(m_referencedSwitches, 1, wxEXPAND|wxALL, 5);
	thirdRow->Add(thirdRow3rdCol, 1, wxEXPAND);

	panelSizer->Add(thirdRow, 1, wxEXPAND);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *defaultToEngagedText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Default to engaged: ")
	);
	fourthRow->Add(defaultToEngagedText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedYes = new wxRadioButton(
		this,
		ID_DIV_CPLR_DEFAULT_TO_ENGAGED_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_defaultToEngagedYes->SetValue(true);
	fourthRow->Add(m_defaultToEngagedYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_defaultToEngagedNo = new wxRadioButton(
		this,
		ID_DIV_CPLR_DEFAULT_TO_ENGAGED_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	fourthRow->Add(m_defaultToEngagedNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *gcStateText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("State when GC is activated: ")
	);
	fifthRow->Add(gcStateText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_gcStateChoice = new wxChoice(
		this,
		ID_DIV_CPLR_GC_STATE_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		gcStateChoices
	);
	fifthRow->Add(m_gcStateChoice, 0, wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *storeInGeneralText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Store in generals: ")
	);
	sixthRow->Add(storeInGeneralText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralYes = new wxRadioButton(
		this,
		ID_DIV_CPLR_STORE_IN_GEN_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_storeInGeneralYes->SetValue(true);
	sixthRow->Add(m_storeInGeneralYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_storeInGeneralNo = new wxRadioButton(
		this,
		ID_DIV_CPLR_STORE_IN_GEN_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	sixthRow->Add(m_storeInGeneralNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *biDirectionalText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Is bidirectional: ")
	);
	seventhRow->Add(biDirectionalText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isBiDirectionalYes = new wxRadioButton(
		this,
		ID_DIV_CPLR_BIDIRECTIONAL_YES,
		wxT("Yes"),
		wxDefaultPosition,
		wxDefaultSize,
		wxRB_GROUP
	);
	m_isBiDirectionalYes->SetValue(false);
	seventhRow->Add(m_isBiDirectionalYes, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_isBiDirectionalNo = new wxRadioButton(
		this,
		ID_DIV_CPLR_BIDIRECTIONAL_NO,
		wxT("No"),
		wxDefaultPosition,
		wxDefaultSize
	);
	m_isBiDirectionalNo->SetValue(true);
	seventhRow->Add(m_isBiDirectionalNo, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	panelSizer->Add(seventhRow, 0, wxGROW);

	wxBoxSizer *eightRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *eightRow1stCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *availableManReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available manuals")
	);
	eightRow1stCol->Add(availableManReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_availableManuals = new wxListBox(
		this,
		ID_DIV_CPLR_AVAILABLE_MAN
	);
	eightRow1stCol->Add(m_availableManuals, 1, wxEXPAND|wxALL, 5);
	eightRow->Add(eightRow1stCol, 1, wxEXPAND);
	wxBoxSizer *eightRow2ndCol = new wxBoxSizer(wxVERTICAL);
	eightRow2ndCol->AddStretchSpacer();
	m_addReferencedManual = new wxButton(
		this,
		ID_DIV_CPLR_ADD_MAN_BTN,
		wxT("Add ->")
	);
	eightRow2ndCol->Add(m_addReferencedManual, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	m_removeReferencedManual = new wxButton(
		this,
		ID_DIV_CPLR_REMOVE_MAN_BTN,
		wxT("<- Remove")
	);
	eightRow2ndCol->Add(m_removeReferencedManual, 0, wxGROW|wxALIGN_CENTER|wxALL, 5);
	eightRow2ndCol->AddStretchSpacer();
	eightRow->Add(eightRow2ndCol, 0, wxEXPAND);
	wxBoxSizer *eightRow3rdCol = new wxBoxSizer(wxVERTICAL);
	wxStaticText *chosenManReferencesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Referenced manuals")
	);
	eightRow3rdCol->Add(chosenManReferencesText, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	m_referencedManuals = new wxListBox(
		this,
		ID_DIV_CPLR_REFERENCED_MAN
	);
	eightRow3rdCol->Add(m_referencedManuals, 1, wxEXPAND|wxALL, 5);
	eightRow->Add(eightRow3rdCol, 1, wxEXPAND);
	panelSizer->Add(eightRow, 1, wxEXPAND);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeDivisionalCouplerBtn = new wxButton(
		this,
		ID_DIV_CPLR_REMOVE_BTN,
		wxT("Remove this divisional coupler")
	);
	bottomRow->Add(removeDivisionalCouplerBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

DivisionalCouplerPanel::~DivisionalCouplerPanel() {

}

void DivisionalCouplerPanel::setDivisionalCoupler(DivisionalCoupler *divCplr) {
	m_divCplr = divCplr;
	m_nameField->SetValue(m_divCplr->getName());
	if (m_divCplr->isDisplayedInverted())
		m_displayInvertedYes->SetValue(true);
	else
		m_displayInvertedNo->SetValue(true);
	m_functionChoice->SetSelection(m_functionChoice->FindString(m_divCplr->getFunction()));
	if (m_divCplr->getFunction().IsSameAs(wxT("Input"))) {
		m_availableSwitches->Enable(false);
		m_referencedSwitches->Enable(false);
		m_addReferencedSwitch->Enable(false);
		m_removeReferencedSwitch->Enable(false);
	} else {
		m_availableSwitches->Enable(true);
		m_referencedSwitches->Enable(true);
		m_addReferencedSwitch->Enable(true);
		m_removeReferencedSwitch->Enable(true);
	}
	if (m_divCplr->isDefaultToEngaged())
		m_defaultToEngagedYes->SetValue(true);
	else
		m_defaultToEngagedNo->SetValue(true);
	m_gcStateChoice->SetSelection((unsigned) (m_divCplr->getGcState() + 1));
	if (m_divCplr->isStoreInGeneral())
		m_storeInGeneralYes->SetValue(true);
	else
		m_storeInGeneralNo->SetValue(true);
	if (m_divCplr->hasBiDirectionalCoupling())
		m_isBiDirectionalYes->SetValue(true);
	else
		m_isBiDirectionalNo->SetValue(true);

	// update/populate available switches
	wxArrayString organSwitches;
	unsigned nbSwitches = ::wxGetApp().m_frame->m_organ->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			organSwitches.Add(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(i)->getName());
		}
		m_availableSwitches->Clear();
		m_availableSwitches->InsertItems(organSwitches, 0);
	}
	UpdateReferencedSwitches();
	m_addReferencedSwitch->Disable();
	m_removeReferencedSwitch->Disable();

	// update/populate available manuals
	wxArrayString organManuals;
	unsigned nbManuals = ::wxGetApp().m_frame->m_organ->getNumberOfManuals();
	if (nbManuals > 0) {
		for (unsigned i = 0; i < nbManuals; i++) {
			organManuals.Add(::wxGetApp().m_frame->m_organ->getOrganManualAt(i)->getName());
		}
		m_availableManuals->Clear();
		m_availableManuals->InsertItems(organManuals, 0);
	}
	UpdateReferencedManuals();
	m_addReferencedManual->Disable();
	m_removeReferencedManual->Disable();
}

void DivisionalCouplerPanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void DivisionalCouplerPanel::OnNameChange(wxCommandEvent& WXUNUSED(event)) {
	wxString content = m_nameField->GetValue();
	GOODF_functions::CheckForStartingWhitespace(&content, m_nameField);
	m_divCplr->setName(m_nameField->GetValue());
	wxString updatedLabel = m_nameField->GetValue();
	::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
	::wxGetApp().m_frame->m_organ->organElementHasChanged();
}

void DivisionalCouplerPanel::OnDisplayInvertedRadio(wxCommandEvent& event) {
	if (event.GetId() == ID_DIV_CPLR_INVERTED_STATE_YES) {
		m_displayInvertedYes->SetValue(true);
		m_divCplr->setDisplayInverted(true);
	} else {
		m_displayInvertedNo->SetValue(true);
		m_divCplr->setDisplayInverted(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnFunctionChange(wxCommandEvent& WXUNUSED(event)) {
	wxString selectedText = m_functionChoice->GetString(m_functionChoice->GetSelection());
	m_divCplr->setFunction(selectedText);
	if (m_divCplr->getFunction().IsSameAs(wxT("Input"))) {
		m_availableSwitches->Deselect(m_availableSwitches->GetSelection());
		m_referencedSwitches->Deselect(m_referencedSwitches->GetSelection());
		m_availableSwitches->Enable(false);
		m_referencedSwitches->Enable(false);
		m_addReferencedSwitch->Enable(false);
		m_removeReferencedSwitch->Enable(false);
	} else {
		m_availableSwitches->Enable(true);
		m_referencedSwitches->Enable(true);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnDefaultToEngagedChange(wxCommandEvent& event) {
	if (event.GetId() == ID_DIV_CPLR_DEFAULT_TO_ENGAGED_YES) {
		m_defaultToEngagedYes->SetValue(true);
		m_divCplr->setDefaultToEngaged(true);
	} else {
		m_defaultToEngagedNo->SetValue(true);
		m_divCplr->setDefaultToEngaged(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnGcStateChange(wxCommandEvent& WXUNUSED(event)) {
	int gcValue = (int) m_gcStateChoice->GetSelection() - 1;
	m_divCplr->setGcState(gcValue);
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnStoreInGeneralChange(wxCommandEvent& event) {
	if (event.GetId() == ID_DIV_CPLR_STORE_IN_GEN_YES) {
		m_storeInGeneralYes->SetValue(true);
		m_divCplr->setStoreInGeneral(true);
	} else {
		m_storeInGeneralNo->SetValue(true);
		m_divCplr->setStoreInGeneral(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnBidirectionalChange(wxCommandEvent& event) {
	if (event.GetId() == ID_DIV_CPLR_BIDIRECTIONAL_YES) {
		m_isBiDirectionalYes->SetValue(true);
		m_divCplr->setBiDirectionalCoupling(true);
	} else {
		m_isBiDirectionalNo->SetValue(true);
		m_divCplr->setBiDirectionalCoupling(false);
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnAddReferencedManual(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableManuals->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableManuals->GetSelection();
		if (!m_divCplr->hasManualReference(::wxGetApp().m_frame->m_organ->getOrganManualAt(selected))) {
			m_divCplr->addAffectedManual(::wxGetApp().m_frame->m_organ->getOrganManualAt(selected));
			UpdateReferencedManuals();
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnRemoveReferencedManual(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedManuals->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedManuals->GetSelection();
		m_divCplr->removeManualAt(selected);
		UpdateReferencedManuals();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnRemoveDivisionalCouplerBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this divisional coupler?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			// references to this divisional coupler in generals and gui elements are removed from organ
			// when this divisional coupler itself is removed
			::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
			m_isFirstRemoval = false;
		}
	} else {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
}

void DivisionalCouplerPanel::OnManualListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableManuals->GetSelection() != wxNOT_FOUND) {
		m_addReferencedManual->Enable(true);
	}
}

void DivisionalCouplerPanel::UpdateReferencedManuals() {
	wxArrayString referencedManuals;
	unsigned nbManuals = m_divCplr->getNumberOfManuals();
	if (nbManuals > 0) {
		for (unsigned i = 0; i < nbManuals; i++) {
			referencedManuals.Add(m_divCplr->getManualAt(i)->getName());
		}
		m_referencedManuals->Clear();
		m_referencedManuals->InsertItems(referencedManuals, 0);
	} else {
		m_referencedManuals->Clear();
		m_removeReferencedManual->Disable();
	}
}

void DivisionalCouplerPanel::OnReferencedManualSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedManuals->GetSelection() != wxNOT_FOUND) {
		m_removeReferencedManual->Enable(true);
	}
}

void DivisionalCouplerPanel::OnAddSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableSwitches->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_availableSwitches->GetSelection();
		if (!m_divCplr->hasSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selected))) {
			m_divCplr->addSwitchReference(::wxGetApp().m_frame->m_organ->getOrganSwitchAt(selected));
			UpdateReferencedSwitches();
		}
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnRemoveSwitchReferenceBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedSwitches->GetSelection() != wxNOT_FOUND) {
		unsigned selected = (unsigned) m_referencedSwitches->GetSelection();
		m_divCplr->removeSwitchReference(m_divCplr->getSwitchAtIndex(selected));
		UpdateReferencedSwitches();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void DivisionalCouplerPanel::OnSwitchListboxSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_availableSwitches->GetSelection() != wxNOT_FOUND) {
		m_addReferencedSwitch->Enable(true);
	}
}

void DivisionalCouplerPanel::OnReferencedSwitchSelection(wxCommandEvent& WXUNUSED(event)) {
	if (m_referencedSwitches->GetSelection() != wxNOT_FOUND) {
		m_removeReferencedSwitch->Enable(true);
	}
}

void DivisionalCouplerPanel::UpdateReferencedSwitches() {
	wxArrayString referencedSwitches;
	unsigned nbSwitches = m_divCplr->getNumberOfSwitches();
	if (nbSwitches > 0) {
		for (unsigned i = 0; i < nbSwitches; i++) {
			referencedSwitches.Add(m_divCplr->getSwitchAtIndex(i)->getName());
		}
		m_referencedSwitches->Clear();
		m_referencedSwitches->InsertItems(referencedSwitches, 0);
	} else {
		m_referencedSwitches->Clear();
		m_removeReferencedSwitch->Disable();
	}
}
