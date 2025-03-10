/*
 * GUIPanelRepresentation.h is part of GoOdf.
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

#ifndef GUIPANELREPRESENTATION_H
#define GUIPANELREPRESENTATION_H

#include <wx/wx.h>
#include "GoPanel.h"
#include "GUIRepresentationDrawingPanel.h"

class GUIPanelRepresentation : public wxDialog {
public:
	GUIPanelRepresentation(wxWindow *parent, const wxString& title);
	~GUIPanelRepresentation();

	void SetCurrentPanel(GoPanel *thePanel);
	void DoUpdateLayout();

private:
	GUIRepresentationDrawingPanel *m_drawingPanel;

};

#endif
