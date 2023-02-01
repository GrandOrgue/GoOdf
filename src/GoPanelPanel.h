/*
 * GoPanelPanel.h is part of GOODF.
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

#ifndef GOPANELPANEL_H
#define GOPANELPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "GoPanel.h"

class GoPanelPanel : public wxPanel {
public:
	GoPanelPanel(wxWindow *parent);
	~GoPanelPanel();

	void setPanel(GoPanel *panel);

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxTextCtrl *m_groupField;
	wxRadioButton *m_hasPedalYes;
	wxRadioButton *m_hasPedalNo;
	wxButton *m_addImageBtn;
	wxListBox *m_organElementsChoice;
	wxListBox *m_setterElementsChoice;
	wxButton *m_elementChoiceBtn;
	wxButton *m_labelElementBtn;
	wxButton *m_removePanelBtn;
	wxChoice *m_manualChoice;
	wxSpinCtrl *m_combinationNumberSpin;

	GoPanel *m_panel;
	wxArrayString organElementChoices;
	wxArrayString availableManuals;

	void OnNameChange(wxCommandEvent& event);
	void OnGroupChange(wxCommandEvent& event);
	void OnHasPedalRadio(wxCommandEvent& event);

	void OnNewImageBtn(wxCommandEvent& event);
	void OnOrganElementChoice(wxCommandEvent& event);
	void OnSetterElementChoice(wxCommandEvent& event);
	void OnRemovePanelBtn(wxCommandEvent& event);
	void OnElementChoiceBtn(wxCommandEvent& event);
	void OnLabelBtn(wxCommandEvent& event);
	void ShouldCombinationControlsBeEnabled();

};

#endif
