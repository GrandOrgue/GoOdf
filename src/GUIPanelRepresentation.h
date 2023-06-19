/*
 * GUIPanelRepresentation.h is part of GOODF.
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

#ifndef GUIPANELREPRESENTATION_H
#define GUIPANELREPRESENTATION_H

#include <wx/wx.h>
#include "GoPanel.h"
#include <vector>

class GUIPanelRepresentation : public wxDialog {
public:
	GUIPanelRepresentation(wxWindow *parent, const wxString& title);
	~GUIPanelRepresentation();

	void SetCurrentPanel(GoPanel *thePanel);
	void DoUpdateLayout();

private:
	DECLARE_EVENT_TABLE()

	wxPanel *m_windowPanel;
	GoPanel *m_currentPanel;

	int m_HackY;
	int m_EnclosureY;
	int m_CenterY;
	int m_CenterWidth;

	wxPoint GetDrawstopPosition(int row, int col);
	wxPoint GetPushbuttonPosition(int row, int col);
	unsigned GetEnclosuresWidth();
	int GetEnclosureY();
	int GetEnclosureX(int enclosureNbr);
	int GetJambLeftRightWidth();
	unsigned GetJambLeftRightHeight();
	int GetJambLeftRightY();
	int GetJambLeftX();
	int GetJambRightX();
	int GetJambTopDrawstop();
	int GetJambTopPiston();
	unsigned GetJambTopHeight();
	unsigned GetJambTopWidth();
	int GetJambTopX();
	int GetJambTopY();
	unsigned GetPistonTopHeight();
	unsigned GetPistonWidth();
	int GetPistonX();
	int GetCenterWidth();
	int GetCenterY();
	int GetCenterX();
	int GetHackY();
	void UpdateLayout();

	void OnPaintEvent(wxPaintEvent& event);
	void DoPaintNow();
	void RenderPanel(wxDC& dc);
	void TileBitmap(wxRect rect, wxDC& dc, wxBitmap& bitmap, int tileOffsetX, int tileOffsetY);
	wxString BreakTextLine(wxString text, int textBreakWidth, wxDC& dc);

};

#endif
