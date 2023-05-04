/*
 * EnclosurePanel.h is part of GOODF.
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

#ifndef ENCLOSUREPANEL_H
#define ENCLOSUREPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Enclosure.h"

class EnclosurePanel : public wxPanel {
public:
	EnclosurePanel(wxWindow *parent);
	~EnclosurePanel();

	void setEnclosure(Enclosure *enclosure);
	void setIsFirstRemoval(bool value);

private:
	DECLARE_EVENT_TABLE()

	wxTextCtrl *m_nameField;
	wxSpinCtrl *m_ampMinLvlSpin;
	wxButton *removeEnclosureBtn;

	Enclosure *m_enclosure;
	bool m_isFirstRemoval;

	void OnNameChange(wxCommandEvent& event);
	void OnAmpMinLvlChange(wxSpinEvent& event);
	void OnRemoveEnclosureBtn(wxCommandEvent& event);
	void DoRemoveEnclosure();

};

#endif
