/*
 * WindchestgroupPanel.h is part of GOODF.
 * Copyright (C) 2024 Lars Palo and contributors (see AUTHORS)
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

#ifndef WINDCHESTPANEL_H
#define WINDCHESTPANEL_H

#include <wx/wx.h>
#include "Windchestgroup.h"

class WindchestgroupPanel : public wxPanel {
public:
	WindchestgroupPanel(wxWindow *parent);
	~WindchestgroupPanel();

	void setWindchest(Windchestgroup *windchest);
	void setIsFirstRemoval(bool value);
	void setTooltipsEnabled(bool isEnabled);

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxListBox *m_availableEnclosures;
	wxListBox *m_referencedEnclosures;
	wxButton *m_addReferencedEnclosure;
	wxButton *m_removeReferencedEnclosure;
	wxListBox *m_availableTremulants;
	wxListBox *m_referencedTremulants;
	wxButton *m_addReferencedTremulant;
	wxButton *m_removeReferencedTremulant;
	wxButton *removeWindchestBtn;

	Windchestgroup *m_windchest;
	bool m_isFirstRemoval;

	void OnNameChange(wxCommandEvent& event);
	void OnAddReferencedEnclosure(wxCommandEvent& event);
	void OnRemoveReferencedEnclosure(wxCommandEvent& event);
	void OnAddReferencedTremulant(wxCommandEvent& event);
	void OnRemoveReferencedTremulant(wxCommandEvent& event);
	void OnRemoveWindchestBtn(wxCommandEvent& event);
	void DoRemoveWindchest();
	void OnEnclosureListboxSelection(wxCommandEvent& event);
	void OnTremulantListboxSelection(wxCommandEvent& event);
	void UpdateReferencedEnclosures();
	void OnReferencedEnclosureSelection(wxCommandEvent& event);
	void OnReferencedTremulantSelection(wxCommandEvent& event);
	void UpdateReferencedTremulants();

};

#endif
