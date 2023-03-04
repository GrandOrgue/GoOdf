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
#include <wx/image.h>
#include "GOODFFunctions.h"
#include "GUITremulant.h"
#include "GUISwitch.h"

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

void OrganFileParser::parseOrganSection() {
	m_organFile.SetPath("/Organ");

	m_organ->setChurchName(m_organFile.Read("ChurchName", wxEmptyString));
	m_organ->setChurchAddress(m_organFile.Read("ChurchAddress", wxEmptyString));
	m_organ->setOrganBuilder(m_organFile.Read("OrganBuilder", wxEmptyString));
	m_organ->setOrganBuildDate(m_organFile.Read("OrganBuildDate", wxEmptyString));
	m_organ->setOrganComments(m_organFile.Read("OrganComments", wxEmptyString));
	m_organ->setRecordingDetails(m_organFile.Read("RecordingDetails", wxEmptyString));
	wxString infoFilename = m_organFile.Read("InfoFilename", wxEmptyString);
	wxString infoFile = GOODF_functions::checkIfFileExist(infoFilename);
	if (infoFile != wxEmptyString) {
		m_organ->setInfoFilename(infoFile);
	}
	wxString cfgBoolValue = m_organFile.Read("DivisionalsStoreIntermanualCouplers", wxEmptyString);
	m_organ->setDivisionalsStoreIntermanualCouplers(GOODF_functions::parseBoolean(cfgBoolValue, true));
	cfgBoolValue = m_organFile.Read("DivisionalsStoreIntramanualCouplers", wxEmptyString);
	m_organ->setDivisionalsStoreIntramanualCouplers(GOODF_functions::parseBoolean(cfgBoolValue, true));
	cfgBoolValue = m_organFile.Read("DivisionalsStoreTremulants", wxEmptyString);
	m_organ->setDivisionalsStoreTremulants(GOODF_functions::parseBoolean(cfgBoolValue, true));
	cfgBoolValue = m_organFile.Read("GeneralsStoreDivisionalCouplers", wxEmptyString);
	m_organ->setGeneralsStoreDivisionalCouplers(GOODF_functions::parseBoolean(cfgBoolValue, true));
	cfgBoolValue = m_organFile.Read("CombinationsStoreNonDisplayedDrawstops", wxEmptyString);
	m_organ->setCombinationsStoreNonDisplayedDrawstops(GOODF_functions::parseBoolean(cfgBoolValue, true));
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
	cfgBoolValue = m_organFile.Read("HasPedals", wxEmptyString);
	m_organ->setHasPedals(GOODF_functions::parseBoolean(cfgBoolValue, false));

	if (m_isUsingOldPanelFormat) {
		// the display metrics must be read from the organ section into Panel000
		m_organ->getOrganPanelAt(0)->getDisplayMetrics()->read(&m_organFile);

		// images can also exist that must be transferred to the main panel
		int nbrImages = static_cast<int>(m_organFile.ReadLong("NumberOfImages", 0));
		if (nbrImages > 0 && nbrImages < 1000) {
			for (int i = 0; i < nbrImages; i++) {
				wxString imgGroupName = wxT("Image") + GOODF_functions::number_format(i + 1);
				if (m_organFile.HasGroup(imgGroupName)) {
					m_organFile.SetPath(wxT("/") + imgGroupName);
					GoImage img;
					img.setOwningPanelWidth(m_organ->getOrganPanelAt(0)->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue());
					img.setOwningPanelHeight(m_organ->getOrganPanelAt(0)->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
					bool imgIsOk = img.read(&m_organFile);
					if (imgIsOk)
						m_organ->getOrganPanelAt(0)->addImage(img);
				}
			}
			m_organFile.SetPath("/Organ");
		}

		// labels can exist that also should be re created as GUI elements
		int nbrLabels = static_cast<int>(m_organFile.ReadLong("NumberOfLabels", 0));
		if (nbrLabels > 0 && nbrLabels < 1000) {
			for (int i = 0; i < nbrLabels; i++) {
				wxString labelGroupName = wxT("Label") + GOODF_functions::number_format(i + 1);
				if (m_organFile.HasGroup(labelGroupName)) {
					m_organFile.SetPath(wxT("/") + labelGroupName);
					createGUILabel(m_organ->getOrganPanelAt(0));
				}
			}
			m_organFile.SetPath("/Organ");
		}
	}

	// parse enclosures
	int nbrEnclosures = static_cast<int>(m_organFile.ReadLong("NumberOfEnclosures", 0));
	if (nbrEnclosures > 0 && nbrEnclosures < 51) {
		for (int i = 0; i < nbrEnclosures; i++) {
			wxString enclosureGroupName = wxT("Enclosure") + GOODF_functions::number_format(i + 1);
			if (m_organFile.HasGroup(enclosureGroupName)) {
				m_organFile.SetPath(wxT("/") + enclosureGroupName);
				Enclosure enc;
				enc.read(&m_organFile, m_isUsingOldPanelFormat);
				m_organ->addEnclosure(enc);
				if (enc.isDisplayed()) {
					int lastEnclosureIdx = m_organ->getNumberOfEnclosures() - 1;
					createGUIEnclosure(m_organ->getOrganPanelAt(0), m_organ->getOrganEnclosureAt(lastEnclosureIdx));
				}

			}
		}
		m_organFile.SetPath("/Organ");
	}

	// parse switches
	int nbrSwitches = static_cast<int>(m_organFile.ReadLong("NumberOfSwitches", 0));
	if (nbrSwitches > 0 && nbrSwitches < 1000) {
		for (int i = 0; i < nbrSwitches; i++) {
			wxString switchGroupName = wxT("Switch") + GOODF_functions::number_format(i + 1);
			if (m_organFile.HasGroup(switchGroupName)) {
				m_organFile.SetPath(wxT("/") + switchGroupName);
				GoSwitch sw;
				sw.read(&m_organFile, m_isUsingOldPanelFormat);
				m_organ->addSwitch(sw);
				if (sw.isDisplayed()) {
					int lastSwitchIdx = m_organ->getNumberOfSwitches() - 1;
					createGUISwitch(m_organ->getOrganPanelAt(0), m_organ->getOrganSwitchAt(lastSwitchIdx));
				}
			}
		}
		m_organFile.SetPath("/Organ");
	}

	// parse tremulants
	int nbrTrems = static_cast<int>(m_organFile.ReadLong("NumberOfTremulants", 0));
	if (nbrTrems > 0 && nbrTrems < 11) {
		for (int i = 0; i < nbrTrems; i++) {
			wxString tremGroupName = wxT("Tremulant") + GOODF_functions::number_format(i + 1);
			if (m_organFile.HasGroup(tremGroupName)) {
				m_organFile.SetPath(wxT("/") + tremGroupName);
				Tremulant trem;
				trem.read(&m_organFile, m_isUsingOldPanelFormat);
				m_organ->addTremulant(trem);
				if (trem.isDisplayed()) {
					int lastTremIdx = m_organ->getNumberOfTremulants() - 1;
					createGUITremulant(m_organ->getOrganPanelAt(0), m_organ->getOrganTremulantAt(lastTremIdx));
				}
			}
		}
		m_organFile.SetPath("/Organ");
	}

	// parse windchests
	int nbrWindchests = static_cast<int>(m_organFile.ReadLong("NumberOfWindchestGroups", 0));
	if (nbrWindchests > 0 && nbrWindchests < 51) {
		for (int i = 0; i < nbrWindchests; i++) {
			wxString windchestGroupName = wxT("WindchestGroup") + GOODF_functions::number_format(i + 1);
			if (m_organFile.HasGroup(windchestGroupName)) {
				m_organFile.SetPath(wxT("/") + windchestGroupName);
				Windchestgroup windchest;
				windchest.read(&m_organFile);
				m_organ->addWindchestgroup(windchest);
			}
		}
		m_organFile.SetPath("/Organ");
	}

	// parse ranks
	int nbrRanks = static_cast<int>(m_organFile.ReadLong("NumberOfRanks", 0));
	if (nbrRanks > 0 && nbrRanks < 1000) {
		for (int i = 0; i < nbrRanks; i++) {
			wxString rankGroupName = wxT("Rank") + GOODF_functions::number_format(i + 1);
			if (m_organFile.HasGroup(rankGroupName)) {
				m_organFile.SetPath(wxT("/") + rankGroupName);
				Rank r;
				r.read(&m_organFile);
				m_organ->addRank(r);
			}
		}
		m_organFile.SetPath("/Organ");
	}

	// parse manuals which contain the stops, couplers and divisionals
	// also they can have tremulant and switch references

	// parse reversible pistons

	// parse divisional couplers

	// parse generals

	// parse setter elements from old style (if present) after manuals are parsed

	// parse panels that has images and gui elements but can also exist in old style version
}

void OrganFileParser::createGUIEnclosure(GoPanel *targetPanel, Enclosure *enclosure) {
	GUIElement *guiEnc = new GUIEnclosure(enclosure);
	guiEnc->setOwningPanel(targetPanel);
	guiEnc->setDisplayName(enclosure->getName());
	targetPanel->addGuiElement(guiEnc);

	// convert gui element back to enclosure type for parsing
	GUIEnclosure *encElement = dynamic_cast<GUIEnclosure*>(guiEnc);
	if (encElement) {
		encElement->read(&m_organFile);
	}
}

void OrganFileParser::createGUITremulant(GoPanel *targetPanel, Tremulant *tremulant) {
	GUIElement *guiTrem = new GUITremulant(tremulant);
	guiTrem->setOwningPanel(targetPanel);
	guiTrem->setDisplayName(tremulant->getName());
	targetPanel->addGuiElement(guiTrem);

	GUITremulant *tremElement = dynamic_cast<GUITremulant*>(guiTrem);
	if (tremElement) {
		tremElement->read(&m_organFile);
	}
}

void OrganFileParser::createGUISwitch(GoPanel *targetPanel, GoSwitch *theSwitch) {
	GUIElement *guiSwitch = new GUISwitch(theSwitch);
	guiSwitch->setOwningPanel(targetPanel);
	guiSwitch->setDisplayName(theSwitch->getName());
	targetPanel->addGuiElement(guiSwitch);

	GUISwitch *switchElement = dynamic_cast<GUISwitch*>(guiSwitch);
	if (switchElement) {
		switchElement->read(&m_organFile);
	}
}

void OrganFileParser::createGUILabel(GoPanel *targetPanel) {
	GUIElement *label = new GUILabel();
	label->setOwningPanel(targetPanel);
	label->setDisplayName(wxT("GUI Label"));
	targetPanel->addGuiElement(label);

	GUILabel *theLabel = dynamic_cast<GUILabel*>(label);
	if (theLabel) {
		theLabel->read(&m_organFile);
	}
}
