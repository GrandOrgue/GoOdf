/*
 * DisplayMetrics.cpp is part of GOODF.
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

#include "DisplayMetrics.h"

DisplayMetrics::DisplayMetrics() {
	m_dispScreenSizeHoriz.setSelectedNameIndex(0, true);
	m_dispScreenSizeVert.setSelectedNameIndex(0, false);
	m_dispDrawstopBackgroundImageNum = 1;
	m_dispConsoleBackgroundImageNum = 1;
	m_dispKeyHorizBackgroundImageNum = 1;
	m_dispKeyVertBackgroundImageNum = 1;
	m_dispDrawstopInsetBackgroundImageNum = 1;
	m_dispControlLabelFont = wxFont(wxFontInfo(7).FaceName(wxT("Arial")));
	m_dispShortcutKeyLabelFont = wxFont(wxFontInfo(7).FaceName(wxT("Arial")));
	m_dispGroupLabelFont = wxFont(wxFontInfo(7).FaceName(wxT("Arial")));
	m_dispDrawstopCols = 2;
	m_dispDrawstopRows = 1;
	m_dispDrawstopColsOffset = false;
	m_dispDrawstopOuterColOffsetUp = false;
	m_dispPairDrawstopCols = false;
	m_dispExtraDrawstopRows = 0;
	m_dispExtraDrawstopCols = 0;
	m_dispButtonCols = 1;
	m_dispExtraButtonRows = 0;
	m_dispExtraPedalButtonRow = false;
	m_dispExtraPedalButtonRowOffset = false;
	m_dispExtraPedalButtonRowOffsetRight = false;
	m_dispButtonsAboveManuals = false;
	m_dispTrimAboveManuals = false;
	m_dispTrimBelowManuals = false;
	m_dispTrimAboveExtraRows = false;
	m_dispExtraDrawstopRowsAboveExtraButtonRows = false;
	m_dispDrawstopWidth = 78;
	m_dispDrawstopHeight = 69;
	m_dispPistonWidth = 44;
	m_dispPistonHeight = 40;
	m_dispEnclosureWidth = 52;
	m_dispEnclosureHeight = 63;
	m_dispPedalHeight = 40;
	m_dispPedalKeyWidth = 7;
	m_dispManualHeight = 32;
	m_dispManualKeyWidth = 12;
}

DisplayMetrics::~DisplayMetrics() {

}

void DisplayMetrics::write(wxTextFile *outFile) {
	if (m_dispScreenSizeHoriz.getSelectedNameIndex() > 3)
		outFile->AddLine(wxT("DispScreenSizeHoriz=") + wxString::Format(wxT("%i"), m_dispScreenSizeHoriz.getNumericalValue()));
	else {
		wxString sizeName = m_dispScreenSizeHoriz.getPanelSizeNames().Item(m_dispScreenSizeHoriz.getSelectedNameIndex());
		outFile->AddLine(wxT("DispScreenSizeHoriz=") + sizeName);
	}
	if (m_dispScreenSizeVert.getSelectedNameIndex() > 3)
		outFile->AddLine(wxT("DispScreenSizeVert=") + wxString::Format(wxT("%i"), m_dispScreenSizeVert.getNumericalValue()));
	else {
		wxString sizeName = m_dispScreenSizeVert.getPanelSizeNames().Item(m_dispScreenSizeVert.getSelectedNameIndex());
		outFile->AddLine(wxT("DispScreenSizeVert=") + sizeName);
	}
	outFile->AddLine(wxT("DispDrawstopBackgroundImageNum=") + wxString::Format(wxT("%i"), m_dispDrawstopBackgroundImageNum));
	outFile->AddLine(wxT("DispConsoleBackgroundImageNum=") + wxString::Format(wxT("%i"), m_dispConsoleBackgroundImageNum));
	outFile->AddLine(wxT("DispKeyHorizBackgroundImageNum=") + wxString::Format(wxT("%i"), m_dispKeyHorizBackgroundImageNum));
	outFile->AddLine(wxT("DispKeyVertBackgroundImageNum=") + wxString::Format(wxT("%i"), m_dispKeyVertBackgroundImageNum));
	outFile->AddLine(wxT("DispDrawstopInsetBackgroundImageNum=") + wxString::Format(wxT("%i"), m_dispDrawstopInsetBackgroundImageNum));
	outFile->AddLine(wxT("DispControlLabelFont=") + m_dispControlLabelFont.GetFaceName());
	outFile->AddLine(wxT("DispShortcutKeyLabelFont=") + m_dispShortcutKeyLabelFont.GetFaceName());
	outFile->AddLine(wxT("DispShortcutKeyLabelColour=") + m_dispShortcutKeyLabelColour.getHtmlValue());
	outFile->AddLine(wxT("DispGroupLabelFont=") + m_dispGroupLabelFont.GetFaceName());
	outFile->AddLine(wxT("DispDrawstopCols=") + wxString::Format(wxT("%i"), m_dispDrawstopCols));
	outFile->AddLine(wxT("DispDrawstopRows=") + wxString::Format(wxT("%i"), m_dispDrawstopRows));

	if (m_dispDrawstopColsOffset) {
		outFile->AddLine(wxT("DispDrawstopColsOffset=Y"));

		if (m_dispDrawstopOuterColOffsetUp)
			outFile->AddLine(wxT("DispDrawstopOuterColOffsetUp=Y"));
		else
			outFile->AddLine(wxT("DispDrawstopOuterColOffsetUp=N"));
	} else {
		outFile->AddLine(wxT("DispDrawstopColsOffset=N"));
	}

	if (m_dispPairDrawstopCols)
		outFile->AddLine(wxT("DispPairDrawstopCols=Y"));
	else
		outFile->AddLine(wxT("DispPairDrawstopCols=N"));

	outFile->AddLine(wxT("DispExtraDrawstopRows=") + wxString::Format(wxT("%i"), m_dispExtraDrawstopRows));
	outFile->AddLine(wxT("DispExtraDrawstopCols=") + wxString::Format(wxT("%i"), m_dispExtraDrawstopCols));
	outFile->AddLine(wxT("DispButtonCols=") + wxString::Format(wxT("%i"), m_dispButtonCols));
	outFile->AddLine(wxT("DispExtraButtonRows=") + wxString::Format(wxT("%i"), m_dispExtraButtonRows));

	if (m_dispExtraPedalButtonRow) {
		outFile->AddLine(wxT("DispExtraPedalButtonRow=Y"));

		if (m_dispExtraPedalButtonRowOffset)
			outFile->AddLine(wxT("DispExtraPedalButtonRowOffset=Y"));
		else
			outFile->AddLine(wxT("DispExtraPedalButtonRowOffset=N"));

		if (m_dispExtraPedalButtonRowOffsetRight)
			outFile->AddLine(wxT("DispExtraPedalButtonRowOffsetRight=Y"));
		else
			outFile->AddLine(wxT("DispExtraPedalButtonRowOffsetRight=N"));
	} else {
		outFile->AddLine(wxT("DispExtraPedalButtonRow=N"));
	}

	if (m_dispButtonsAboveManuals)
		outFile->AddLine(wxT("DispButtonsAboveManuals=Y"));
	else
		outFile->AddLine(wxT("DispButtonsAboveManuals=N"));

	if (m_dispTrimAboveManuals)
		outFile->AddLine(wxT("DispTrimAboveManuals=Y"));
	else
		outFile->AddLine(wxT("DispTrimAboveManuals=N"));

	if (m_dispTrimBelowManuals)
		outFile->AddLine(wxT("DispTrimBelowManuals=Y"));
	else
		outFile->AddLine(wxT("DispTrimBelowManuals=N"));

	if (m_dispTrimAboveExtraRows)
		outFile->AddLine(wxT("DispTrimAboveExtraRows=Y"));
	else
		outFile->AddLine(wxT("DispTrimAboveExtraRows=N"));

	if (m_dispExtraDrawstopRowsAboveExtraButtonRows)
		outFile->AddLine(wxT("DispExtraDrawstopRowsAboveExtraButtonRows=Y"));
	else
		outFile->AddLine(wxT("DispExtraDrawstopRowsAboveExtraButtonRows=N"));

	if (m_dispDrawstopWidth != 78)
		outFile->AddLine(wxT("DispDrawstopWidth=") + wxString::Format(wxT("%i"), m_dispDrawstopWidth));
	if (m_dispDrawstopHeight != 69)
		outFile->AddLine(wxT("DispDrawstopHeight=") + wxString::Format(wxT("%i"), m_dispDrawstopHeight));
	if (m_dispPistonWidth != 44)
		outFile->AddLine(wxT("DispPistonWidth=") + wxString::Format(wxT("%i"), m_dispPistonWidth));
	if (m_dispPistonHeight != 40)
		outFile->AddLine(wxT("DispPistonHeight=") + wxString::Format(wxT("%i"), m_dispPistonHeight));
	if (m_dispEnclosureWidth != 52)
		outFile->AddLine(wxT("DispEnclosureWidth=") + wxString::Format(wxT("%i"), m_dispEnclosureWidth));
	if (m_dispEnclosureHeight != 63)
		outFile->AddLine(wxT("DispEnclosureHeight=") + wxString::Format(wxT("%i"), m_dispEnclosureHeight));
	if (m_dispPedalHeight != 40)
		outFile->AddLine(wxT("DispPedalHeight=") + wxString::Format(wxT("%i"), m_dispPedalHeight));
	if (m_dispPedalKeyWidth != 7)
		outFile->AddLine(wxT("DispPedalKeyWidth=") + wxString::Format(wxT("%i"), m_dispPedalKeyWidth));
	if (m_dispManualHeight != 32)
		outFile->AddLine(wxT("DispManualHeight=") + wxString::Format(wxT("%i"), m_dispManualHeight));
	if (m_dispManualKeyWidth != 12)
		outFile->AddLine(wxT("DispManualKeyWidth=") + wxString::Format(wxT("%i"), m_dispManualKeyWidth));
}
