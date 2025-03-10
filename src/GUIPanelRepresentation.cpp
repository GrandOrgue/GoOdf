/*
 * GUIPanelRepresentation.cpp is part of GoOdf.
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

#include "GUIPanelRepresentation.h"

GUIPanelRepresentation::GUIPanelRepresentation(wxWindow *parent, const wxString& title) : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX) {
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	m_drawingPanel = new GUIRepresentationDrawingPanel(this);
	topSizer->Add(m_drawingPanel, 1, wxEXPAND);
	SetSizerAndFit(topSizer);
}

GUIPanelRepresentation::~GUIPanelRepresentation() {
	if (m_drawingPanel)
		m_drawingPanel->Close();
}

void GUIPanelRepresentation::SetCurrentPanel(GoPanel *thePanel) {
	m_drawingPanel->SetCurrentPanel(thePanel);
	SetTitle(thePanel->getName());
}

void GUIPanelRepresentation::DoUpdateLayout() {
	m_drawingPanel->DoUpdateLayout();
}
