/*
 * GUIPanelRepresentation.cpp is part of GOODF.
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

#include "GUIPanelRepresentation.h"

// Event table
BEGIN_EVENT_TABLE(GUIPanelRepresentation, wxDialog)
	EVT_PAINT(GUIPanelRepresentation::OnPaintEvent)
END_EVENT_TABLE()

GUIPanelRepresentation::GUIPanelRepresentation(wxWindow *parent, const wxString& title) : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX) {
	m_windowPanel = new wxPanel(this, wxID_ANY);
	m_currentPanel = NULL;
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	// m_windowPanel->SetClientSize(800, 500);
	// topSizer->Add(m_windowPanel, 1, wxGROW);
	 m_windowPanel->SetSizer(topSizer);
}

GUIPanelRepresentation::~GUIPanelRepresentation() {

}

void GUIPanelRepresentation::SetCurrentPanel(GoPanel *thePanel) {
	m_currentPanel = thePanel;
	m_windowPanel->SetSize(m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue(), m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	SetClientSize(m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue(), m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
}

void GUIPanelRepresentation::OnPaintEvent(wxPaintEvent& WXUNUSED(event)) {
	wxPaintDC dc(this);
	RenderPanel(dc);
}

void GUIPanelRepresentation::DoPaintNow() {
	wxClientDC dc(this);
	RenderPanel(dc);
}

void GUIPanelRepresentation::RenderPanel(wxDC& dc) {
	if (m_currentPanel->getNumberOfImages() > 0) {
		for (unsigned i = 0; i < m_currentPanel->getNumberOfImages(); i++) {
			dc.DrawBitmap(m_currentPanel->getImageAt(i)->getBitmap(), m_currentPanel->getImageAt(i)->getPositionX(), m_currentPanel->getImageAt(i)->getPositionY(), true);
		}
	}
}

void GUIPanelRepresentation::TileBitmap(wxRect rect, wxDC& dc, wxBitmap& bitmap, int tileOffsetX, int tileOffsetY) {
	int w = bitmap.GetWidth();
	int h = bitmap.GetHeight();

	for (int i = rect.x - tileOffsetX; i < rect.x + rect.width; i += w) {
		for (int j = rect.y; j < rect.y + rect.height; j+= h) {
			dc.DrawBitmap(bitmap, i, j);
		}
	}

}
