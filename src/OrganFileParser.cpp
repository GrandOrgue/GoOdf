/*
 * OrganFileParser.cpp is part of GOODF.
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

#include "OrganFileParser.h"
#include <wx/filename.h>

OrganFileParser::OrganFileParser(wxString filePath, Organ *organ) {
	m_filePath = filePath;
	m_organ = organ;
	m_fileIsOk = false;
	m_organIsReady = false;
	m_isUsingOldPanelFormat = false;
	m_errorMessage = wxEmptyString;

	readIniFile();
	if (m_fileIsOk)
		parseOrgan();
}

OrganFileParser::~OrganFileParser() {

}

void OrganFileParser::parseOrgan() {
	wxFileName odf = wxFileName(m_filePath);
	m_organ->setOdfRoot(odf.GetPath());
	parseOrganSection();
}

bool OrganFileParser::isOrganReady() {
	return m_organIsReady;
}

void OrganFileParser::readIniFile() {
	m_organFile = wxFileConfig(wxEmptyString, wxEmptyString, m_filePath);
	if (m_organFile.HasGroup(wxT("Organ")))
		m_fileIsOk = true;
	else {
		m_fileIsOk = false;
		m_errorMessage = wxT("No [Organ] section could be found in file!");
	}
	if (m_organFile.HasGroup(wxT("Panel000"))) {
		m_isUsingOldPanelFormat = false;
	} else {
		m_isUsingOldPanelFormat = true;
	}
}

bool OrganFileParser::parseBoolean(wxString key, bool defaultValue) {
	wxString value = m_organFile.Read(key, wxEmptyString);
	if (value.IsSameAs(wxT("Y"), false))
		return true;
	if (value.IsSameAs(wxT("N"), false))
		return false;
	return defaultValue;
}

void OrganFileParser::parseOrganSection() {
	m_organFile.SetPath("/Organ");

	m_organ->setChurchName(m_organFile.Read("ChurchName", wxEmptyString));
	m_organ->setChurchAddress(m_organFile.Read("ChurchAddress", wxEmptyString));
	m_organ->setOrganBuilder(m_organFile.Read("OrganBuilder", wxEmptyString));
	m_organ->setOrganBuildDate(m_organFile.Read("OrganBuildDate", wxEmptyString));
	m_organ->setOrganComments(m_organFile.Read("OrganComments", wxEmptyString));
	m_organ->setRecordingDetails(m_organFile.Read("RecordingDetails", wxEmptyString));
	wxString infoFilename = m_organFile.Read("InfoFilename", wxEmptyString);
	if (infoFilename != wxEmptyString) {
		wxString infoFilePath = m_organ->getOdfRoot() + wxFILE_SEP_PATH + infoFilename;
		wxFileName infoFile = wxFileName(infoFilePath);
		if (infoFile.FileExists()) {
			m_organ->setInfoFilename(infoFile.GetFullPath());
		}
	}
	m_organ->setDivisionalsStoreIntermanualCouplers(parseBoolean(wxT("DivisionalsStoreIntermanualCouplers"), true));
	m_organ->setDivisionalsStoreIntramanualCouplers(parseBoolean(wxT("DivisionalsStoreIntramanualCouplers"), true));
	m_organ->setDivisionalsStoreTremulants(parseBoolean(wxT("DivisionalsStoreTremulants"), true));
	m_organ->setGeneralsStoreDivisionalCouplers(parseBoolean(wxT("GeneralsStoreDivisionalCouplers"), true));
	m_organ->setCombinationsStoreNonDisplayedDrawstops(parseBoolean(wxT("CombinationsStoreNonDisplayedDrawstops"), true));
	float ampLvl = static_cast<float>(m_organFile.ReadDouble("AmplitudeLevel", 100.0f));
	if (ampLvl >= 0.0f && ampLvl <= 1000.0f)
		m_organ->setAmplitudeLevel(ampLvl);
	float gain = static_cast<float>(m_organFile.ReadDouble("Gain", 0.0f));
	if (gain >= -120.0f && gain <= 40.0f)
		m_organ->setGain(gain);
	float pitchTuning = static_cast<float>(m_organFile.ReadDouble("PitchTuning", 0.0f));
	if (pitchTuning >= -1800.0f && pitchTuning <= 1800.0f)
		m_organ->setPitchTuning(pitchTuning);
	float pitchCorrection = static_cast<float>(m_organFile.ReadDouble("PitchCorrection", 0.0f));
	if (pitchCorrection >= -1800.0f && pitchCorrection <= 1800.0f)
		m_organ->setPitchCorrection(pitchCorrection);
	long trackerDelay = m_organFile.ReadLong("TrackerDelay", 0);
	if (trackerDelay >= 0 && trackerDelay <= 10000) {
		m_organ->setTrackerDelay(static_cast<unsigned>(trackerDelay));
	}
	m_organ->setHasPedals(parseBoolean(wxT("HasPedals"), false));

	if (m_isUsingOldPanelFormat) {
		parseDisplayMetrics(wxT("/Organ"), m_organ->getOrganPanelAt(0));
	}
}

void OrganFileParser::parseDisplayMetrics(wxString sourcePanel, GoPanel *targetPanel) {
	m_organFile.SetPath(sourcePanel);

	wxString horizSize = m_organFile.Read("DispScreenSizeHoriz", wxEmptyString);
	if (horizSize != wxEmptyString) {
		if (horizSize.IsSameAs(wxT("SMALL"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setSelectedNameIndex(0, true);
		} else if (horizSize.IsSameAs(wxT("MEDIUM"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setSelectedNameIndex(1, true);
		} else if (horizSize.IsSameAs(wxT("MEDIUM LARGE"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setSelectedNameIndex(2, true);
		} else if (horizSize.IsSameAs(wxT("LARGE"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setSelectedNameIndex(3, true);
		} else {
			// The value is not a matching string so we try to convert it into a number
			long value = -1;
			if (horizSize.ToLong(&value)) {
				if (value > 99 && value < 4001) {
					targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setNumericalValue(value);
				}
			}
		}
	}
	wxString vertSize = m_organFile.Read("DispScreenSizeVert", wxEmptyString);
	if (vertSize != wxEmptyString) {
		if (vertSize.IsSameAs(wxT("SMALL"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setSelectedNameIndex(0, true);
		} else if (vertSize.IsSameAs(wxT("MEDIUM"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setSelectedNameIndex(1, true);
		} else if (vertSize.IsSameAs(wxT("MEDIUM LARGE"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setSelectedNameIndex(2, true);
		} else if (vertSize.IsSameAs(wxT("LARGE"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setSelectedNameIndex(3, true);
		} else {
			// The value is not a matching string so we try to convert it into a number
			long value = -1;
			if (vertSize.ToLong(&value)) {
				if (value > 99 && value < 4001) {
					targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setNumericalValue(value);
				}
			}
		}
	}
	int drawstopBgNum = static_cast<int>(m_organFile.ReadLong("DispDrawstopBackgroundImageNum", 1));
	if (drawstopBgNum > 0 && drawstopBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispDrawstopBackgroundImageNum = drawstopBgNum;
	int consoleBgNum = static_cast<int>(m_organFile.ReadLong("DispConsoleBackgroundImageNum", 1));
	if (consoleBgNum > 0 && consoleBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispConsoleBackgroundImageNum = consoleBgNum;
	int keyHorizBgNum = static_cast<int>(m_organFile.ReadLong("DispKeyHorizBackgroundImageNum", 1));
	if (keyHorizBgNum > 0 && keyHorizBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispKeyHorizBackgroundImageNum = keyHorizBgNum;
	int keyVertBgNum = static_cast<int>(m_organFile.ReadLong("DispKeyVertBackgroundImageNum", 1));
	if (keyVertBgNum > 0 && keyVertBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispKeyVertBackgroundImageNum = keyVertBgNum;
	int insetBgNum = static_cast<int>(m_organFile.ReadLong("DispDrawstopInsetBackgroundImageNum", 1));
	if (insetBgNum > 0 && insetBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispDrawstopInsetBackgroundImageNum = insetBgNum;
	wxString fontName = m_organFile.Read("DispControlLabelFont", wxEmptyString);
	if (fontName != wxEmptyString)
		targetPanel->getDisplayMetrics()->m_dispControlLabelFont = wxFont(wxFontInfo(7).FaceName(fontName));
	fontName = m_organFile.Read("DispShortcutKeyLabelFont", wxEmptyString);
	if (fontName != wxEmptyString)
		targetPanel->getDisplayMetrics()->m_dispShortcutKeyLabelFont = wxFont(wxFontInfo(7).FaceName(fontName));
	fontName = m_organFile.Read("DispGroupLabelFont", wxEmptyString);
	if (fontName != wxEmptyString)
		targetPanel->getDisplayMetrics()->m_dispGroupLabelFont = wxFont(wxFontInfo(7).FaceName(fontName));
	int drawstopCols = static_cast<int>(m_organFile.ReadLong("DispDrawstopCols", 2));
	if (drawstopCols > 1 && drawstopCols < 13 && drawstopCols % 2 == 0)
		targetPanel->getDisplayMetrics()->m_dispDrawstopCols = drawstopCols;
	int drawstopRows = static_cast<int>(m_organFile.ReadLong("DispDrawstopRows", 1));
	if (drawstopRows > 0 && drawstopRows < 21)
		targetPanel->getDisplayMetrics()->m_dispDrawstopRows = drawstopRows;
	targetPanel->getDisplayMetrics()->m_dispDrawstopColsOffset = parseBoolean(wxT("DispDrawstopColsOffset"), false);
	targetPanel->getDisplayMetrics()->m_dispDrawstopOuterColOffsetUp = parseBoolean(wxT("DispDrawstopOuterColOffsetUp"), false);
	targetPanel->getDisplayMetrics()->m_dispPairDrawstopCols = parseBoolean(wxT("DispPairDrawstopCols"), false);
	int extraDrawstopRows = static_cast<int>(m_organFile.ReadLong("DispExtraDrawstopRows", 0));
	if (extraDrawstopRows > -1 && extraDrawstopRows < 100)
		targetPanel->getDisplayMetrics()->m_dispExtraDrawstopRows = extraDrawstopRows;
	int extraDrawstopCols = static_cast<int>(m_organFile.ReadLong("DispExtraDrawstopCols", 0));
	if (extraDrawstopCols > -1 && extraDrawstopCols < 41)
		targetPanel->getDisplayMetrics()->m_dispExtraDrawstopCols = extraDrawstopCols;
	int btnCols = static_cast<int>(m_organFile.ReadLong("DispButtonCols", 1));
	if (btnCols > 0 && btnCols < 33)
		targetPanel->getDisplayMetrics()->m_dispButtonCols = btnCols;
	int extraBtnRows = static_cast<int>(m_organFile.ReadLong("DispExtraButtonRows", 0));
	if (extraBtnRows > -1 && extraBtnRows < 100)
		targetPanel->getDisplayMetrics()->m_dispExtraButtonRows = extraBtnRows;
	targetPanel->getDisplayMetrics()->m_dispExtraPedalButtonRow = parseBoolean(wxT("DispExtraPedalButtonRow"), false);
	targetPanel->getDisplayMetrics()->m_dispExtraPedalButtonRowOffset = parseBoolean(wxT("DispExtraPedalButtonRowOffset"), false);
	targetPanel->getDisplayMetrics()->m_dispExtraPedalButtonRowOffsetRight = parseBoolean(wxT("DispExtraPedalButtonRowOffsetRight"), false);
	targetPanel->getDisplayMetrics()->m_dispButtonsAboveManuals = parseBoolean(wxT("DispButtonsAboveManuals"), false);
	targetPanel->getDisplayMetrics()->m_dispTrimAboveManuals = parseBoolean(wxT("DispTrimAboveManuals"), false);
	targetPanel->getDisplayMetrics()->m_dispTrimBelowManuals = parseBoolean(wxT("DispTrimBelowManuals"), false);
	targetPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows = parseBoolean(wxT("DispTrimAboveExtraRows"), false);
	targetPanel->getDisplayMetrics()->m_dispExtraDrawstopRowsAboveExtraButtonRows = parseBoolean(wxT("DispExtraDrawstopRowsAboveExtraButtonRows"), false);
	int drawstopWidth = static_cast<int>(m_organFile.ReadLong("DispDrawstopWidth", 78));
	if (drawstopWidth > 0 && drawstopWidth < 151)
		targetPanel->getDisplayMetrics()->m_dispDrawstopWidth = drawstopWidth;
	int drawstopHeight = static_cast<int>(m_organFile.ReadLong("DispDrawstopHeight", 69));
	if (drawstopHeight > 0 && drawstopHeight < 151)
		targetPanel->getDisplayMetrics()->m_dispDrawstopHeight = drawstopHeight;
	int pistonWidth = static_cast<int>(m_organFile.ReadLong("DispPistonWidth", 44));
	if (pistonWidth > 0 && pistonWidth < 151)
		targetPanel->getDisplayMetrics()->m_dispPistonWidth = pistonWidth;
	int pistonHeight = static_cast<int>(m_organFile.ReadLong("DispPistonHeight", 40));
	if (pistonHeight > 0 && pistonHeight < 151)
		targetPanel->getDisplayMetrics()->m_dispPistonHeight = pistonHeight;
	int enclosureWidth = static_cast<int>(m_organFile.ReadLong("DispEnclosureWidth", 52));
	if (enclosureWidth > 0 && enclosureWidth < 151)
		targetPanel->getDisplayMetrics()->m_dispEnclosureWidth = enclosureWidth;
	int enclosureHeight = static_cast<int>(m_organFile.ReadLong("DispEnclosureHeight", 63));
	if (enclosureHeight > 0 && enclosureHeight < 151)
		targetPanel->getDisplayMetrics()->m_dispEnclosureHeight = enclosureHeight;
	int pedalHeight = static_cast<int>(m_organFile.ReadLong("DispPedalHeight", 40));
	if (pedalHeight > 0 && pedalHeight < 501)
		targetPanel->getDisplayMetrics()->m_dispPedalHeight = pedalHeight;
	int pedalKeyWidth = static_cast<int>(m_organFile.ReadLong("DispPedalKeyWidth", 7));
	if (pedalKeyWidth > 0 && pedalKeyWidth < 501)
		targetPanel->getDisplayMetrics()->m_dispPedalKeyWidth = pedalKeyWidth;
	int manualHeight = static_cast<int>(m_organFile.ReadLong("DispManualHeight", 32));
	if (manualHeight > 0 && manualHeight < 501)
		targetPanel->getDisplayMetrics()->m_dispManualHeight = manualHeight;
	int manualKeyWidth = static_cast<int>(m_organFile.ReadLong("DispManualKeyWidth", 12));
	if (manualKeyWidth > 0 && manualKeyWidth < 501)
		targetPanel->getDisplayMetrics()->m_dispManualKeyWidth = manualKeyWidth;
}
