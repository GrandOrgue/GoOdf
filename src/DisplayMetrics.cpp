/*
 * DisplayMetrics.cpp is part of GoOdf.
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

#include "DisplayMetrics.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

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
	outFile->AddLine(wxT("DispShortcutKeyLabelColour=") + m_dispShortcutKeyLabelColour.getColorName());
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

void DisplayMetrics::read(wxFileConfig *cfg) {
	wxString horizSize = cfg->Read("DispScreenSizeHoriz", wxEmptyString);
	if (horizSize != wxEmptyString) {
		if (horizSize.IsSameAs(wxT("SMALL"), false)) {
			m_dispScreenSizeHoriz.setSelectedNameIndex(0, true);
		} else if (horizSize.IsSameAs(wxT("MEDIUM"), false)) {
			m_dispScreenSizeHoriz.setSelectedNameIndex(1, true);
		} else if (horizSize.IsSameAs(wxT("MEDIUM LARGE"), false)) {
			m_dispScreenSizeHoriz.setSelectedNameIndex(2, true);
		} else if (horizSize.IsSameAs(wxT("LARGE"), false)) {
			m_dispScreenSizeHoriz.setSelectedNameIndex(3, true);
		} else {
			// The value is not a matching string so we try to convert it into a number
			long value = -1;
			if (horizSize.ToLong(&value)) {
				if (value > 99 && value < 32001) {
					m_dispScreenSizeHoriz.setNumericalValue(value);
				}
			}
		}
	}
	wxString vertSize = cfg->Read("DispScreenSizeVert", wxEmptyString);
	if (vertSize != wxEmptyString) {
		if (vertSize.IsSameAs(wxT("SMALL"), false)) {
			m_dispScreenSizeVert.setSelectedNameIndex(0, false);
		} else if (vertSize.IsSameAs(wxT("MEDIUM"), false)) {
			m_dispScreenSizeVert.setSelectedNameIndex(1, false);
		} else if (vertSize.IsSameAs(wxT("MEDIUM LARGE"), false)) {
			m_dispScreenSizeVert.setSelectedNameIndex(2, false);
		} else if (vertSize.IsSameAs(wxT("LARGE"), false)) {
			m_dispScreenSizeVert.setSelectedNameIndex(3, false);
		} else {
			// The value is not a matching string so we try to convert it into a number
			long value = -1;
			if (vertSize.ToLong(&value)) {
				if (value > 99 && value < 32001) {
					m_dispScreenSizeVert.setNumericalValue(value);
				}
			}
		}
	}
	int drawstopBgNum = static_cast<int>(cfg->ReadLong("DispDrawstopBackgroundImageNum", 1));
	if (drawstopBgNum > 0 && drawstopBgNum < 65)
		m_dispDrawstopBackgroundImageNum = drawstopBgNum;
	int consoleBgNum = static_cast<int>(cfg->ReadLong("DispConsoleBackgroundImageNum", 1));
	if (consoleBgNum > 0 && consoleBgNum < 65)
		m_dispConsoleBackgroundImageNum = consoleBgNum;
	int keyHorizBgNum = static_cast<int>(cfg->ReadLong("DispKeyHorizBackgroundImageNum", 1));
	if (keyHorizBgNum > 0 && keyHorizBgNum < 65)
		m_dispKeyHorizBackgroundImageNum = keyHorizBgNum;
	int keyVertBgNum = static_cast<int>(cfg->ReadLong("DispKeyVertBackgroundImageNum", 1));
	if (keyVertBgNum > 0 && keyVertBgNum < 65)
		m_dispKeyVertBackgroundImageNum = keyVertBgNum;
	int insetBgNum = static_cast<int>(cfg->ReadLong("DispDrawstopInsetBackgroundImageNum", 1));
	if (insetBgNum > 0 && insetBgNum < 65)
		m_dispDrawstopInsetBackgroundImageNum = insetBgNum;
	wxString fontName = cfg->Read("DispControlLabelFont", wxEmptyString);
	if (fontName != wxEmptyString)
		m_dispControlLabelFont = wxFont(wxFontInfo(7).FaceName(fontName));
	fontName = cfg->Read("DispShortcutKeyLabelFont", wxEmptyString);
	if (fontName != wxEmptyString)
		m_dispShortcutKeyLabelFont = wxFont(wxFontInfo(7).FaceName(fontName));
	fontName = cfg->Read("DispGroupLabelFont", wxEmptyString);
	if (fontName != wxEmptyString)
		m_dispGroupLabelFont = wxFont(wxFontInfo(7).FaceName(fontName));
	wxString labelColor = cfg->Read("DispShortcutKeyLabelColour", wxEmptyString);
	if (labelColor != wxEmptyString) {
		int colorIdx = m_dispShortcutKeyLabelColour.getColorNames().Index(labelColor, false);
		if (colorIdx != wxNOT_FOUND) {
			m_dispShortcutKeyLabelColour.setSelectedColorIndex(colorIdx);
		} else {
			// is it possible that it's a html format color code instead
			wxColour color;
			color.Set(labelColor);
			if (color.IsOk()) {
				m_dispShortcutKeyLabelColour.setColorValue(color);
			}
		}
	}
	int drawstopCols = static_cast<int>(cfg->ReadLong("DispDrawstopCols", 2));
	if (drawstopCols > 1 && drawstopCols < 13 && drawstopCols % 2 == 0)
		m_dispDrawstopCols = drawstopCols;
	int drawstopRows = static_cast<int>(cfg->ReadLong("DispDrawstopRows", 1));
	if (drawstopRows > 0 && drawstopRows < 21)
		m_dispDrawstopRows = drawstopRows;
	wxString booleanCfgValue = cfg->Read("DispDrawstopColsOffset", wxEmptyString);
	m_dispDrawstopColsOffset = GOODF_functions::parseBoolean(booleanCfgValue, false);
	booleanCfgValue = cfg->Read("DispDrawstopOuterColOffsetUp", wxEmptyString);
	m_dispDrawstopOuterColOffsetUp = GOODF_functions::parseBoolean(booleanCfgValue, false);
	booleanCfgValue = cfg->Read("DispPairDrawstopCols", wxEmptyString);
	m_dispPairDrawstopCols = GOODF_functions::parseBoolean(booleanCfgValue, false);
	int extraDrawstopRows = static_cast<int>(cfg->ReadLong("DispExtraDrawstopRows", 0));
	if (extraDrawstopRows > -1 && extraDrawstopRows < 100)
		m_dispExtraDrawstopRows = extraDrawstopRows;
	int extraDrawstopCols = static_cast<int>(cfg->ReadLong("DispExtraDrawstopCols", 0));
	if (extraDrawstopCols > -1 && extraDrawstopCols < 41)
		m_dispExtraDrawstopCols = extraDrawstopCols;
	int btnCols = static_cast<int>(cfg->ReadLong("DispButtonCols", 1));
	if (btnCols > 0 && btnCols < 33)
		m_dispButtonCols = btnCols;
	int extraBtnRows = static_cast<int>(cfg->ReadLong("DispExtraButtonRows", 0));
	if (extraBtnRows > -1 && extraBtnRows < 100)
		m_dispExtraButtonRows = extraBtnRows;
	booleanCfgValue = cfg->Read("DispExtraPedalButtonRow", wxEmptyString);
	m_dispExtraPedalButtonRow = GOODF_functions::parseBoolean(booleanCfgValue, false);
	booleanCfgValue = cfg->Read("DispExtraPedalButtonRowOffset", wxEmptyString);
	m_dispExtraPedalButtonRowOffset = GOODF_functions::parseBoolean(booleanCfgValue, false);
	booleanCfgValue = cfg->Read("DispExtraPedalButtonRowOffsetRight", wxEmptyString);
	m_dispExtraPedalButtonRowOffsetRight = GOODF_functions::parseBoolean(booleanCfgValue, false);
	booleanCfgValue = cfg->Read("DispButtonsAboveManuals", wxEmptyString);
	m_dispButtonsAboveManuals = GOODF_functions::parseBoolean(booleanCfgValue, false);
	booleanCfgValue = cfg->Read("DispTrimAboveManuals", wxEmptyString);
	m_dispTrimAboveManuals = GOODF_functions::parseBoolean(booleanCfgValue, false);
	booleanCfgValue = cfg->Read("DispTrimBelowManuals", wxEmptyString);
	m_dispTrimBelowManuals = GOODF_functions::parseBoolean(booleanCfgValue, false);
	booleanCfgValue = cfg->Read("DispTrimAboveExtraRows", wxEmptyString);
	m_dispTrimAboveExtraRows = GOODF_functions::parseBoolean(booleanCfgValue, false);
	booleanCfgValue = cfg->Read("DispExtraDrawstopRowsAboveExtraButtonRows", wxEmptyString);
	m_dispExtraDrawstopRowsAboveExtraButtonRows = GOODF_functions::parseBoolean(booleanCfgValue, false);
	int drawstopWidth = static_cast<int>(cfg->ReadLong("DispDrawstopWidth", 78));
	if (drawstopWidth > 0 && drawstopWidth < 151)
		m_dispDrawstopWidth = drawstopWidth;
	int drawstopHeight = static_cast<int>(cfg->ReadLong("DispDrawstopHeight", 69));
	if (drawstopHeight > 0 && drawstopHeight < 151)
		m_dispDrawstopHeight = drawstopHeight;
	int pistonWidth = static_cast<int>(cfg->ReadLong("DispPistonWidth", 44));
	if (pistonWidth > 0 && pistonWidth < 151)
		m_dispPistonWidth = pistonWidth;
	int pistonHeight = static_cast<int>(cfg->ReadLong("DispPistonHeight", 40));
	if (pistonHeight > 0 && pistonHeight < 151)
		m_dispPistonHeight = pistonHeight;
	int enclosureWidth = static_cast<int>(cfg->ReadLong("DispEnclosureWidth", 52));
	if (enclosureWidth > 0 && enclosureWidth < 151)
		m_dispEnclosureWidth = enclosureWidth;
	int enclosureHeight = static_cast<int>(cfg->ReadLong("DispEnclosureHeight", 63));
	if (enclosureHeight > 0 && enclosureHeight < 151)
		m_dispEnclosureHeight = enclosureHeight;
	int pedalHeight = static_cast<int>(cfg->ReadLong("DispPedalHeight", 40));
	if (pedalHeight > 0 && pedalHeight < 501)
		m_dispPedalHeight = pedalHeight;
	int pedalKeyWidth = static_cast<int>(cfg->ReadLong("DispPedalKeyWidth", 7));
	if (pedalKeyWidth > 0 && pedalKeyWidth < 501)
		m_dispPedalKeyWidth = pedalKeyWidth;
	int manualHeight = static_cast<int>(cfg->ReadLong("DispManualHeight", 32));
	if (manualHeight > 0 && manualHeight < 501)
		m_dispManualHeight = manualHeight;
	int manualKeyWidth = static_cast<int>(cfg->ReadLong("DispManualKeyWidth", 12));
	if (manualKeyWidth > 0 && manualKeyWidth < 501)
		m_dispManualKeyWidth = manualKeyWidth;
}

wxBitmap DisplayMetrics::getDrawstopBg() {
	return ::wxGetApp().m_woodBitmaps[m_dispDrawstopBackgroundImageNum - 1];
}

wxBitmap DisplayMetrics::getConsoleBg() {
	return ::wxGetApp().m_woodBitmaps[m_dispConsoleBackgroundImageNum - 1];
}

wxBitmap DisplayMetrics::getKeyHorizBg() {
	return ::wxGetApp().m_woodBitmaps[m_dispKeyHorizBackgroundImageNum - 1];
}

wxBitmap DisplayMetrics::getKeyVertBg() {
	return ::wxGetApp().m_woodBitmaps[m_dispKeyVertBackgroundImageNum - 1];
}

wxBitmap DisplayMetrics::getInsetBg() {
	return ::wxGetApp().m_woodBitmaps[m_dispDrawstopInsetBackgroundImageNum - 1];
}
