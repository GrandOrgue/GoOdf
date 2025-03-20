/*
 * OrganFileParser.cpp is part of GoOdf.
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

#include "OrganFileParser.h"
#include <wx/filename.h>
#include <wx/image.h>
#include "GOODF.h"
#include "GOODFFunctions.h"
#include "GUITremulant.h"
#include "GUISwitch.h"
#include "GUIReversiblePiston.h"
#include "GUIDivisionalCoupler.h"
#include "GUIGeneral.h"
#include "GUIDivisional.h"
#include "GUICoupler.h"
#include "GUIStop.h"

OrganFileParser::OrganFileParser(wxString filePath, Organ *organ) {
	m_filePath = filePath;
	m_organ = organ;
	m_fileIsOk = false;
	m_organIsReady = false;
	m_isUsingOldPanelFormat = false;
	m_errorMessage = wxEmptyString;
	m_progressDlg = NULL;

	readIniFile();
	if (m_fileIsOk) {
		m_progressDlg = new wxProgressDialog(
			wxT("Parsing ") + m_filePath,
			wxEmptyString
		);
		parseOrgan();
	}
}

OrganFileParser::~OrganFileParser() {
	if (m_organFile)
		delete m_organFile;
	if (m_progressDlg)
		delete m_progressDlg;
}

void OrganFileParser::parseOrgan() {
	wxFileName odf = wxFileName(m_filePath);
	m_organ->setOdfRoot(odf.GetPath());
	parseOrganSection();
	if (m_errorMessage == wxEmptyString)
		m_organIsReady = true;
}

bool OrganFileParser::isOrganReady() {
	return m_organIsReady;
}

void OrganFileParser::readIniFile() {
	m_organFile = new wxFileConfig(wxEmptyString, wxEmptyString, m_filePath, wxEmptyString, wxCONFIG_USE_NO_ESCAPE_CHARACTERS);
	if (m_organFile->HasGroup(wxT("Organ"))) {
		m_fileIsOk = true;
		if (m_organFile->HasGroup(wxT("Panel000"))) {
			m_isUsingOldPanelFormat = false;
		} else {
			m_isUsingOldPanelFormat = true;
		}
		trimKeyValues();
	} else {
		m_fileIsOk = false;
		m_errorMessage = wxT("No [Organ] section could be found in file!");
	}
}

void OrganFileParser::trimKeyValues() {
	wxString group;
	long group_index;

	m_organFile->SetPath("/");
	bool has_group = m_organFile->GetFirstGroup(group, group_index);
	while (has_group) {
		m_organFile->SetPath(group);

		wxString entry;
		long entry_index;

		bool has_entry = m_organFile->GetFirstEntry(entry, entry_index);
		while (has_entry) {
			wxString value = m_organFile->Read(entry, wxEmptyString);
			bool valueIsChanged = false;
			// remove comments from the key value
			int semicolonPos = value.Find(wxT(";"));
			if (semicolonPos != wxNOT_FOUND) {
				value = value.substr(0, semicolonPos);
				valueIsChanged = true;
			}
			// trim whitespace
			if (value.StartsWith(wxT(" "))) {
				value = value.Trim(false);
				valueIsChanged = true;
			}
			if (value.EndsWith(wxT(" "))) {
				value = value.Trim();
				valueIsChanged = true;
			}

			if (valueIsChanged) {
				m_organFile->DeleteEntry(entry, false);
				m_organFile->Write(entry, value);
			}

			has_entry = m_organFile->GetNextEntry(entry, entry_index);
		}

		m_organFile->SetPath("/");
		has_group = m_organFile->GetNextGroup(group, group_index);
	}
}

void OrganFileParser::parseOrganSection() {
	m_organFile->SetPath("/Organ");

	m_progressDlg->Update(0, wxT("Parsing [Organ] section"));
	m_organ->setChurchName(m_organFile->Read("ChurchName", wxEmptyString));
	m_organ->setChurchAddress(m_organFile->Read("ChurchAddress", wxEmptyString));
	m_organ->setOrganBuilder(m_organFile->Read("OrganBuilder", wxEmptyString));
	m_organ->setOrganBuildDate(m_organFile->Read("OrganBuildDate", wxEmptyString));
	m_organ->setOrganComments(m_organFile->Read("OrganComments", wxEmptyString));
	m_organ->setRecordingDetails(m_organFile->Read("RecordingDetails", wxEmptyString));
	wxString infoFilename = m_organFile->Read("InfoFilename", wxEmptyString);
	wxString infoFile = GOODF_functions::checkIfFileExist(infoFilename, m_organ);
	if (infoFile != wxEmptyString) {
		m_organ->setInfoFilename(infoFile);
	}
	wxString cfgBoolValue = m_organFile->Read("DivisionalsStoreIntermanualCouplers", wxEmptyString);
	m_organ->setDivisionalsStoreIntermanualCouplers(GOODF_functions::parseBoolean(cfgBoolValue, true));
	cfgBoolValue = m_organFile->Read("DivisionalsStoreIntramanualCouplers", wxEmptyString);
	m_organ->setDivisionalsStoreIntramanualCouplers(GOODF_functions::parseBoolean(cfgBoolValue, true));
	cfgBoolValue = m_organFile->Read("DivisionalsStoreTremulants", wxEmptyString);
	m_organ->setDivisionalsStoreTremulants(GOODF_functions::parseBoolean(cfgBoolValue, true));
	cfgBoolValue = m_organFile->Read("GeneralsStoreDivisionalCouplers", wxEmptyString);
	m_organ->setGeneralsStoreDivisionalCouplers(GOODF_functions::parseBoolean(cfgBoolValue, true));
	cfgBoolValue = m_organFile->Read("CombinationsStoreNonDisplayedDrawstops", wxEmptyString);
	m_organ->setCombinationsStoreNonDisplayedDrawstops(GOODF_functions::parseBoolean(cfgBoolValue, true));
	float ampLvl = static_cast<float>(m_organFile->ReadDouble("AmplitudeLevel", 100.0f));
	if (ampLvl >= 0.0f && ampLvl <= 1000.0f)
		m_organ->setAmplitudeLevel(ampLvl);
	float gain = static_cast<float>(m_organFile->ReadDouble("Gain", 0.0f));
	if (gain >= -120.0f && gain <= 40.0f)
		m_organ->setGain(gain);
	float pitchTuning = static_cast<float>(m_organFile->ReadDouble("PitchTuning", 0.0f));
	if (pitchTuning >= -1800.0f && pitchTuning <= 1800.0f)
		m_organ->setPitchTuning(pitchTuning);
	float pitchCorrection = static_cast<float>(m_organFile->ReadDouble("PitchCorrection", 0.0f));
	if (pitchCorrection >= -1800.0f && pitchCorrection <= 1800.0f)
		m_organ->setPitchCorrection(pitchCorrection);
	long trackerDelay = m_organFile->ReadLong("TrackerDelay", 0);
	if (trackerDelay >= 0 && trackerDelay <= 10000) {
		m_organ->setTrackerDelay(static_cast<unsigned>(trackerDelay));
	}
	cfgBoolValue = m_organFile->Read("Percussive", wxEmptyString);
	m_organ->setIsPercussive(GOODF_functions::parseBoolean(cfgBoolValue, false));
	if (m_organ->getIsPercussive()) {
		cfgBoolValue = m_organFile->Read("HasIndependentRelease", wxEmptyString);
		m_organ->setHasIndependentRelease(GOODF_functions::parseBoolean(cfgBoolValue, false));
	}
	cfgBoolValue = m_organFile->Read("HasPedals", wxEmptyString);
	m_organ->setHasPedals(GOODF_functions::parseBoolean(cfgBoolValue, false), true);

	if (m_isUsingOldPanelFormat) {
		// the display metrics must be read from the organ section into Panel000
		m_organ->getOrganPanelAt(0)->getDisplayMetrics()->read(m_organFile);

		// images can also exist that must be transferred to the main panel
		int nbrImages = static_cast<int>(m_organFile->ReadLong("NumberOfImages", 0));
		if (nbrImages > 0 && nbrImages < 1000) {
			for (int i = 0; i < nbrImages; i++) {
				m_organFile->SetPath("/");
				wxString imgGroupName = wxT("Image") + GOODF_functions::number_format(i + 1);
				if (m_organFile->HasGroup(imgGroupName)) {
					m_progressDlg->Update(5, wxT("Parsing old style [") + imgGroupName + wxT("] section"));
					m_organFile->SetPath(wxT("/") + imgGroupName);
					GoImage img;
					img.setOwningPanelWidth(m_organ->getOrganPanelAt(0)->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue());
					img.setOwningPanelHeight(m_organ->getOrganPanelAt(0)->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
					bool imgIsOk = img.read(m_organFile, m_organ);
					if (imgIsOk)
						m_organ->getOrganPanelAt(0)->addImage(img);
					else {
						wxLogWarning("%s is not possible to parse and use!", imgGroupName);
						::wxGetApp().m_frame->GetLogWindow()->Show(true);
					}
				}
			}
			m_organFile->SetPath("/Organ");
		}

		// labels can exist that also should be re created as GUI elements
		int nbrLabels = static_cast<int>(m_organFile->ReadLong("NumberOfLabels", 0));
		if (nbrLabels > 0 && nbrLabels < 1000) {
			for (int i = 0; i < nbrLabels; i++) {
				m_organFile->SetPath("/");
				wxString labelGroupName = wxT("Label") + GOODF_functions::number_format(i + 1);
				m_progressDlg->Update(10, wxT("Parsing old style [") + labelGroupName + wxT("] section"));
				if (m_organFile->HasGroup(labelGroupName)) {
					m_organFile->SetPath(wxT("/") + labelGroupName);
					createGUILabel(m_organ->getOrganPanelAt(0));
				} else {
					wxLogWarning("%s couldn't be found!", labelGroupName);
					::wxGetApp().m_frame->GetLogWindow()->Show(true);
				}
			}
			m_organFile->SetPath("/Organ");
		}
	} else {
		// We need to read the display metrics of panel 000 before any other elements that might be displayed!
		// But the new style GUI elements must be read after all the structural elements have been processed,
		// so that will be done at a later point in the parsing.
		m_organFile->SetPath(wxT("/Panel000"));
		m_organ->getOrganPanelAt(0)->read(m_organFile, wxT("Panel000"), m_organ);
		m_progressDlg->Update(12, wxT("Parsing [Panel000] base section"));
		m_organFile->SetPath("/Organ");
	}

	// parse enclosures
	int nbrEnclosures = static_cast<int>(m_organFile->ReadLong("NumberOfEnclosures", 0));
	if (nbrEnclosures > 0 && nbrEnclosures < 1000) {
		for (int i = 0; i < nbrEnclosures; i++) {
			m_organFile->SetPath("/");
			wxString enclosureGroupName = wxT("Enclosure") + GOODF_functions::number_format(i + 1);
			m_progressDlg->Update(15, wxT("Parsing [") + enclosureGroupName + wxT("] section"));
			if (m_organFile->HasGroup(enclosureGroupName)) {
				m_organFile->SetPath(wxT("/") + enclosureGroupName);
				Enclosure enc;
				enc.read(m_organFile, m_isUsingOldPanelFormat);
				m_organ->addEnclosure(enc, true);
				if (enc.isDisplayed()) {
					int lastEnclosureIdx = m_organ->getNumberOfEnclosures() - 1;
					createGUIEnclosure(m_organ->getOrganPanelAt(0), m_organ->getOrganEnclosureAt(lastEnclosureIdx));
				}
			} else {
				wxLogWarning("%s couldn't be found!", enclosureGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
		m_organFile->SetPath("/Organ");
	}

	// parse switches
	int nbrSwitches = static_cast<int>(m_organFile->ReadLong("NumberOfSwitches", 0));
	if (nbrSwitches > 0 && nbrSwitches < 1000) {
		for (int i = 0; i < nbrSwitches; i++) {
			m_organFile->SetPath("/");
			wxString switchGroupName = wxT("Switch") + GOODF_functions::number_format(i + 1);
			m_progressDlg->Update(20, wxT("Parsing [") + switchGroupName + wxT("] section"));
			if (m_organFile->HasGroup(switchGroupName)) {
				m_organFile->SetPath(wxT("/") + switchGroupName);
				GoSwitch sw;
				sw.read(m_organFile, m_isUsingOldPanelFormat, m_organ);
				m_organ->addSwitch(sw, true);
				if (sw.isDisplayed()) {
					int lastSwitchIdx = m_organ->getNumberOfSwitches() - 1;
					createGUISwitch(m_organ->getOrganPanelAt(0), m_organ->getOrganSwitchAt(lastSwitchIdx));
				}
			} else {
				wxLogWarning("%s couldn't be found!", switchGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
		m_organFile->SetPath("/Organ");
	}

	// parse tremulants
	int nbrTrems = static_cast<int>(m_organFile->ReadLong("NumberOfTremulants", 0));
	if (nbrTrems > 0 && nbrTrems < 1000) {
		for (int i = 0; i < nbrTrems; i++) {
			m_organFile->SetPath("/");
			wxString tremGroupName = wxT("Tremulant") + GOODF_functions::number_format(i + 1);
			m_progressDlg->Update(25, wxT("Parsing [") + tremGroupName + wxT("] section"));
			if (m_organFile->HasGroup(tremGroupName)) {
				m_organFile->SetPath(wxT("/") + tremGroupName);
				Tremulant trem;
				trem.read(m_organFile, m_isUsingOldPanelFormat, m_organ);
				m_organ->addTremulant(trem, true);
				if (trem.isDisplayed()) {
					int lastTremIdx = m_organ->getNumberOfTremulants() - 1;
					createGUITremulant(m_organ->getOrganPanelAt(0), m_organ->getOrganTremulantAt(lastTremIdx));
				}
			} else {
				wxLogWarning("%s couldn't be found!", tremGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
		m_organFile->SetPath("/Organ");
	}

	// parse windchests
	int nbrWindchests = static_cast<int>(m_organFile->ReadLong("NumberOfWindchestGroups", 0));
	if (nbrWindchests > 0 && nbrWindchests < 1000) {
		for (int i = 0; i < nbrWindchests; i++) {
			m_organFile->SetPath("/");
			wxString windchestGroupName = wxT("WindchestGroup") + GOODF_functions::number_format(i + 1);
			m_progressDlg->Update(30, wxT("Parsing [") + windchestGroupName + wxT("] section"));
			if (m_organFile->HasGroup(windchestGroupName)) {
				m_organFile->SetPath(wxT("/") + windchestGroupName);
				Windchestgroup windchest;
				windchest.read(m_organFile, m_organ);
				m_organ->addWindchestgroup(windchest);
			} else {
				wxLogWarning("%s couldn't be found!", windchestGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
		m_organFile->SetPath("/Organ");
	}
	if (nbrWindchests == 0) {
		wxLogWarning("There are no windchestgroups in the organ! The .organ file won't be functional until at least one windchestgroup exist!");
		::wxGetApp().m_frame->GetLogWindow()->Show(true);
	}

	// parse ranks
	int nbrRanks = static_cast<int>(m_organFile->ReadLong("NumberOfRanks", 0));
	if (nbrRanks > 0 && nbrRanks < 1000) {
		for (int i = 0; i < nbrRanks; i++) {
			m_organFile->SetPath("/");
			wxString rankGroupName = wxT("Rank") + GOODF_functions::number_format(i + 1);
			m_progressDlg->Update(35, wxT("Parsing [") + rankGroupName + wxT("] section"));
			if (m_organFile->HasGroup(rankGroupName)) {
				m_organFile->SetPath(wxT("/") + rankGroupName);
				Rank r;
				r.read(m_organFile, m_organ);
				m_organ->addRank(r);
				bool rankUsesLegacyXfades = false;
				for (Pipe& p : r.m_pipes) {
					if (!p.m_attacks.front().loadRelease && p.m_attacks.front().releaseCrossfadeLength) {
						// This is certainly a legacy x-fade!
						wxLogWarning("[Rank%0.3d] %s uses Pipe999ReleaseCrossfadeLength with LoadRelease=N! You might want to use Tools->Import Legacy X-fades.", m_organ->getNumberOfRanks(), r.getName());
						rankUsesLegacyXfades = true;
					}
					if (rankUsesLegacyXfades) {
						::wxGetApp().m_frame->GetLogWindow()->Show(true);
						break;
					}
				}
			} else {
				wxLogWarning("%s couldn't be found!", rankGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
		m_organFile->SetPath("/Organ");
	}

	// parse manuals which contain the stops, couplers and divisionals
	// also they can have tremulant and switch references, but the couplers
	// needs to be parsed after all manuals have been added to the organ
	// and then the divisionals need to be added after the couplers
	int nbrManuals = static_cast<int>(m_organFile->ReadLong("NumberOfManuals", 0));
	if ((nbrManuals > 0 && nbrManuals < 17) || (nbrManuals == 0 && m_organ->doesHavePedals())) {
		if (m_organ->doesHavePedals())
			nbrManuals += 1;
		for (int i = 0; i < nbrManuals; i++) {
			m_organFile->SetPath("/");
			int manIdxNbr = i;
			if (!m_organ->doesHavePedals())
				manIdxNbr += 1;
			wxString manGroupName = wxT("Manual") + GOODF_functions::number_format(manIdxNbr);
			int dlgValue = 40 + (24 / nbrManuals) * i;
			m_progressDlg->Update(dlgValue, wxT("Parsing [") + manGroupName + wxT("] section"));
			if (m_organFile->HasGroup(manGroupName)) {
				m_organFile->SetPath(wxT("/") + manGroupName);
				Manual m;
				if (manIdxNbr == 0)
					m.setIsPedal(true, true);
				m_organ->addManual(m, true);
				Manual *man = m_organ->getOrganManualAt(m_organ->getNumberOfManuals() - 1);
				man->read(m_organFile, m_isUsingOldPanelFormat, manGroupName, m_organ);
				if (man->isDisplayed()) {
					createGUIManual(m_organ->getOrganPanelAt(0), man);
					if (manGroupName.IsSameAs(wxT("Manual000")))
						m_organ->getOrganPanelAt(0)->setHasPedals(true);
				}
			} else {
				wxLogWarning("%s couldn't be found!", manGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}

		for (int i = 0; i < nbrManuals; i++) {
			m_organFile->SetPath("/");
			int manIdxNbr = i;
			if (!m_organ->doesHavePedals())
				manIdxNbr += 1;
			wxString manGroupName = wxT("Manual") + GOODF_functions::number_format(manIdxNbr);
			m_progressDlg->Update(65, wxT("Parsing couplers for [") + manGroupName + wxT("]"));
			if (m_organFile->HasGroup(manGroupName) && i < (int) m_organ->getNumberOfManuals()) {
				m_organFile->SetPath(wxT("/") + manGroupName);
				Manual *man = m_organ->getOrganManualAt(i);
				man->readCouplers(m_organFile, m_isUsingOldPanelFormat, manGroupName, m_organ);
			}
		}

		for (int i = 0; i < nbrManuals; i++) {
			m_organFile->SetPath("/");
			int manIdxNbr = i;
			if (!m_organ->doesHavePedals())
				manIdxNbr += 1;
			wxString manGroupName = wxT("Manual") + GOODF_functions::number_format(manIdxNbr);
			m_progressDlg->Update(66, wxT("Parsing divisionals for [") + manGroupName + wxT("]"));
			if (m_organFile->HasGroup(manGroupName) && i < (int) m_organ->getNumberOfManuals()) {
				m_organFile->SetPath(wxT("/") + manGroupName);
				Manual *man = m_organ->getOrganManualAt(i);
				man->readDivisionals(m_organFile, m_isUsingOldPanelFormat, manGroupName, m_organ);
			}
		}
		m_organFile->SetPath("/Organ");
	}

	// parse reversible pistons
	int nbrPistons = static_cast<int>(m_organFile->ReadLong("NumberOfReversiblePistons", 0));
	if (nbrPistons > 0 && nbrPistons < 33) {
		for (int i = 0; i < nbrPistons; i++) {
			m_organFile->SetPath("/");
			wxString pistonGroupName = wxT("ReversiblePiston") + GOODF_functions::number_format(i + 1);
			m_progressDlg->Update(68, wxT("Parsing [") + pistonGroupName + wxT("] section"));
			if (m_organFile->HasGroup(pistonGroupName)) {
				m_organFile->SetPath(wxT("/") + pistonGroupName);
				ReversiblePiston p;
				p.read(m_organFile, m_isUsingOldPanelFormat, m_organ);
				m_organ->addReversiblePiston(p, true);
				if (p.isDisplayed()) {
					unsigned lastPistonIdx = m_organ->getNumberOfReversiblePistons() - 1;
					createGUIPiston(m_organ->getOrganPanelAt(0), m_organ->getReversiblePistonAt(lastPistonIdx));
				}
			} else {
				wxLogWarning("%s couldn't be found!", pistonGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
		m_organFile->SetPath("/Organ");
	}

	// parse divisional couplers
	int nbrDivCplrs = static_cast<int>(m_organFile->ReadLong("NumberOfDivisionalCouplers", 0));
	if (nbrDivCplrs > 0 && nbrDivCplrs < 9) {
		for (int i = 0; i < nbrDivCplrs; i++) {
			m_organFile->SetPath("/");
			wxString divCplrGroupName = wxT("DivisionalCoupler") + GOODF_functions::number_format(i + 1);
			m_progressDlg->Update(70, wxT("Parsing [") + divCplrGroupName + wxT("] section"));
			if (m_organFile->HasGroup(divCplrGroupName)) {
				m_organFile->SetPath(wxT("/") + divCplrGroupName);
				DivisionalCoupler divCplr;
				divCplr.read(m_organFile, m_isUsingOldPanelFormat, m_organ);
				m_organ->addDivisionalCoupler(divCplr, true);
				if (divCplr.isDisplayed()) {
					unsigned lastIdx = m_organ->getNumberOfOrganDivisionalCouplers() - 1;
					createGUIDivCplr(m_organ->getOrganPanelAt(0), m_organ->getOrganDivisionalCouplerAt(lastIdx));
				}
			} else {
				wxLogWarning("%s couldn't be found!", divCplrGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
		m_organFile->SetPath("/Organ");
	}

	// parse generals
	int nbrGenerals = static_cast<int>(m_organFile->ReadLong("NumberOfGenerals", 0));
	if (nbrGenerals > 0 && nbrGenerals < 100) {
		for (int i = 0; i < nbrGenerals; i++) {
			m_organFile->SetPath("/");
			wxString generalGroupName = wxT("General") + GOODF_functions::number_format(i + 1);
			m_progressDlg->Update(75, wxT("Parsing [") + generalGroupName + wxT("] section"));
			if (m_organFile->HasGroup(generalGroupName)) {
				m_organFile->SetPath(wxT("/") + generalGroupName);
				General g;
				g.read(m_organFile, m_isUsingOldPanelFormat, m_organ);
				m_organ->addGeneral(g, true);
				if (g.isDisplayed()) {
					unsigned lastIdx = m_organ->getNumberOfGenerals() - 1;
					createGUIGeneral(m_organ->getOrganPanelAt(0), m_organ->getOrganGeneralAt(lastIdx));
				}
			} else {
				wxLogWarning("%s couldn't be found!", generalGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
		m_organFile->SetPath("/Organ");
	}

	// parse setter elements from old style (if present)
	// they will all be converted into the new GUI Element style depending on their type
	if (m_isUsingOldPanelFormat) {
		int nbrSetters = static_cast<int>(m_organFile->ReadLong("NumberOfSetterElements", 0));
		if (nbrSetters > 0 && nbrSetters < 1000) {
			for (int i = 0; i < nbrSetters; i++) {
				m_organFile->SetPath("/");
				wxString setterGroupName = wxT("SetterElement") + GOODF_functions::number_format(i + 1);
				m_progressDlg->Update(80, wxT("Parsing old style [") + setterGroupName + wxT("] section"));
				if (m_organFile->HasGroup(setterGroupName)) {
					m_organFile->SetPath(wxT("/") + setterGroupName);
					wxString elementType = m_organFile->Read("Type", wxEmptyString);
					if (elementType != wxEmptyString) {
						createFromSetterElement(m_organ->getOrganPanelAt(0), elementType);
					}
				} else {
					wxLogWarning("%s couldn't be found!", setterGroupName);
					::wxGetApp().m_frame->GetLogWindow()->Show(true);
				}
			}
		}
		m_organFile->SetPath("/Organ");
	}

	// parse panels that has images and gui elements but can also exist in old style version
	int nbrPanels = static_cast<int>(m_organFile->ReadLong("NumberOfPanels", 0));
	if (!m_isUsingOldPanelFormat) {
		// For the new format there exist a [Panel000] as main panel that can have GUI elements that must be read.
		// That panel is already created with the organ and it won't be included in the count of number of panels either.
		// The check if that section exist in the .organ file has already been done.
		m_organFile->SetPath(wxT("/Panel000"));
		m_progressDlg->Update(85, wxT("Parsing [Panel000] GUI elements"));
		parsePanelElements(m_organ->getOrganPanelAt(0), wxT("Panel000"));
		m_organFile->SetPath("/Organ");
	}
	if (nbrPanels > 0 && nbrPanels < 100) {
		for (int i = 0; i < nbrPanels; i++) {
			m_organFile->SetPath("/");
			wxString panelGroupName = wxT("Panel") + GOODF_functions::number_format(i + 1);
			m_progressDlg->Update(90, wxT("Parsing [") + panelGroupName + wxT("] section"));
			if (m_organFile->HasGroup(panelGroupName)) {
				m_organFile->SetPath(wxT("/") + panelGroupName);
				GoPanel p;
				p.read(m_organFile, panelGroupName, m_organ);
				m_organ->addPanel(p);
				parsePanelElements(m_organ->getOrganPanelAt(m_organ->getNumberOfPanels() - 1), panelGroupName);
			} else {
				wxLogWarning("%s couldn't be found!", panelGroupName);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
		m_organFile->SetPath("/Organ");
	}
	m_progressDlg->Update(100, wxT("Whole .organ file has been parsed!"));
}

void OrganFileParser::createGUIEnclosure(GoPanel *targetPanel, Enclosure *enclosure) {
	GUIElement *guiEnc = new GUIEnclosure(enclosure);
	guiEnc->setOwningPanel(targetPanel);
	if (enclosure)
		guiEnc->setDisplayName(enclosure->getName());
	targetPanel->addGuiElement(guiEnc);

	// convert gui element back to enclosure type for parsing
	GUIEnclosure *encElement = dynamic_cast<GUIEnclosure*>(guiEnc);
	if (encElement) {
		encElement->read(m_organFile, m_organ);
	}
}

void OrganFileParser::createGUITremulant(GoPanel *targetPanel, Tremulant *tremulant) {
	GUIElement *guiTrem = new GUITremulant(tremulant);
	guiTrem->setOwningPanel(targetPanel);
	guiTrem->setDisplayName(tremulant->getName());
	guiTrem->setDefaultFont(targetPanel->getDisplayMetrics()->m_dispControlLabelFont);
	targetPanel->addGuiElement(guiTrem);

	GUITremulant *tremElement = dynamic_cast<GUITremulant*>(guiTrem);
	if (tremElement) {
		tremElement->read(m_organFile, m_organ);
	}
}

void OrganFileParser::createGUISwitch(GoPanel *targetPanel, GoSwitch *theSwitch) {
	GUIElement *guiSwitch = new GUISwitch(theSwitch);
	guiSwitch->setOwningPanel(targetPanel);
	if (theSwitch)
		guiSwitch->setDisplayName(theSwitch->getName());
	guiSwitch->setDefaultFont(targetPanel->getDisplayMetrics()->m_dispControlLabelFont);
	targetPanel->addGuiElement(guiSwitch);

	GUISwitch *switchElement = dynamic_cast<GUISwitch*>(guiSwitch);
	if (switchElement) {
		switchElement->read(m_organFile, m_organ);
	}
}

void OrganFileParser::createGUILabel(GoPanel *targetPanel) {
	GUIElement *label = new GUILabel();
	label->setOwningPanel(targetPanel);
	label->updateDisplayName();
	label->setDefaultFont(targetPanel->getDisplayMetrics()->m_dispGroupLabelFont);
	targetPanel->addGuiElement(label);

	GUILabel *theLabel = dynamic_cast<GUILabel*>(label);
	if (theLabel) {
		theLabel->read(m_organFile, m_organ);
	}
}

void OrganFileParser::createGUIManual(GoPanel *targetPanel, Manual *manual) {
	GUIElement *man = new GUIManual(manual);
	man->setOwningPanel(targetPanel);
	man->setDisplayName(manual->getName());
	targetPanel->addGuiElement(man);

	GUIManual *theManual = dynamic_cast<GUIManual*>(man);
	if (theManual) {
		theManual->read(m_organFile, m_organ);
	}
}

void OrganFileParser::createGUIPiston(GoPanel *targetPanel, ReversiblePiston *piston) {
	GUIElement *revPiston = new GUIReversiblePiston(piston);
	revPiston->setOwningPanel(targetPanel);
	revPiston->setDisplayName(piston->getName());
	revPiston->setDefaultFont(targetPanel->getDisplayMetrics()->m_dispControlLabelFont);
	targetPanel->addGuiElement(revPiston);

	GUIReversiblePiston *thePiston = dynamic_cast<GUIReversiblePiston*>(revPiston);
	if (thePiston) {
		thePiston->read(m_organFile, thePiston->isDisplayAsPiston(), m_organ);
	}
}

void OrganFileParser::createGUIDivCplr(GoPanel *targetPanel, DivisionalCoupler *div_cplr) {
	GUIElement *divCoupler = new GUIDivisionalCoupler(div_cplr);
	divCoupler->setOwningPanel(targetPanel);
	divCoupler->setDisplayName(div_cplr->getName());
	divCoupler->setDefaultFont(targetPanel->getDisplayMetrics()->m_dispControlLabelFont);
	targetPanel->addGuiElement(divCoupler);

	GUIDivisionalCoupler *theDivCplr = dynamic_cast<GUIDivisionalCoupler*>(divCoupler);
	if (theDivCplr) {
		theDivCplr->read(m_organFile, theDivCplr->isDisplayAsPiston(), m_organ);
	}
}

void OrganFileParser::createGUIGeneral(GoPanel *targetPanel, General *general) {
	GUIElement *gen = new GUIGeneral(general);
	gen->setOwningPanel(targetPanel);
	if (general)
		gen->setDisplayName(general->getName());
	gen->setDefaultFont(targetPanel->getDisplayMetrics()->m_dispControlLabelFont);
	targetPanel->addGuiElement(gen);

	GUIGeneral *theGeneral = dynamic_cast<GUIGeneral*>(gen);
	if (theGeneral) {
		theGeneral->read(m_organFile, theGeneral->isDisplayAsPiston(), m_organ);
	}
}

void OrganFileParser::createGUIDivisional(GoPanel *targetPanel, Divisional *divisional) {
	GUIElement *guiDiv = new GUIDivisional(divisional);
	guiDiv->setOwningPanel(targetPanel);
	guiDiv->setDefaultFont(targetPanel->getDisplayMetrics()->m_dispControlLabelFont);
	targetPanel->addGuiElement(guiDiv);

	GUIDivisional *divElement = dynamic_cast<GUIDivisional*>(guiDiv);
	if (divElement) {
		divElement->read(m_organFile, true, m_organ);
	}
}

void OrganFileParser::createFromSetterElement(GoPanel *targetPanel, wxString elementType) {
	if (elementType == wxT("CrescendoLabel") ||
		elementType == wxT("CurrFileName") ||
		elementType == wxT("GeneralLabel") ||
		elementType == wxT("PitchLabel") ||
		elementType == wxT("SequencerLabel") ||
		elementType == wxT("TemperamentLabel") ||
		elementType == wxT("TransposeLabel") ) {
		// this setter element is a type of label
		createGUILabel(targetPanel);
		// the label element type must be overridden
		GUIElement *e = targetPanel->getGuiElementAt(targetPanel->getNumberOfGuiElements() - 1);
		e->setType(elementType);
		e->setDisplayName(elementType);
	} else if (elementType.Find("Setter") != wxNOT_FOUND && elementType.Find("DivisionalBank") != wxNOT_FOUND) {
		// we need to get the manual
		wxString manNbrStr = elementType.Mid(6 , 3);
		long value = 0;
		if (!manNbrStr.ToLong(&value)) {
			return;
		}
		int targetManual = (int) value;
		if (targetManual > (int)::wxGetApp().m_frame->m_organ->getNumberOfManuals()) {
			return;
		}
		createGUILabel(targetPanel);
		// the element type must be overridden
		GUIElement *e = targetPanel->getGuiElementAt(targetPanel->getNumberOfGuiElements() - 1);
		e->setType(elementType);
		e->setDisplayName(elementType);
	} else if (elementType == wxT("Swell")) {
		createGUIEnclosure(targetPanel, NULL);
		// the element type must be overridden
		GUIElement *e = targetPanel->getGuiElementAt(targetPanel->getNumberOfGuiElements() - 1);
		e->setType(elementType);
		e->setDisplayName(elementType);
	} else if (elementType.StartsWith("General") && elementType.Len() == 9) {
		createGUIGeneral(targetPanel, NULL);
		// the element type must be overridden
		GUIElement *e = targetPanel->getGuiElementAt(targetPanel->getNumberOfGuiElements() - 1);
		e->setType(elementType);
		e->setDisplayName(elementType);
	} else if (elementType.Find("Setter") != wxNOT_FOUND && elementType.Find("Divisional") != wxNOT_FOUND && elementType.Len() == 22) {
		// we need to get both manual (three X) and the divisional number YYY
		wxString manNbrStr = elementType.Mid(6 , 3);
		long value = 0;
		if (!manNbrStr.ToLong(&value)) {
			return;
		}
		int targetManual = (int) value;
		if (targetManual > (int)::wxGetApp().m_frame->m_organ->getNumberOfManuals()) {
			return;
		}
		wxString divNbrStr = elementType.Mid(19 ,3);
		if (!divNbrStr.ToLong(&value)) {
			return;
		}
		createGUIDivisional(targetPanel, NULL);
		// the element type must be overridden
		GUIElement *e = targetPanel->getGuiElementAt(targetPanel->getNumberOfGuiElements() - 1);
		e->setType(elementType);
		e->setDisplayName(elementType);
	} else if ((elementType.Find("Setter") != wxNOT_FOUND && elementType.Find("DivisionalPrevBank") != wxNOT_FOUND) || (elementType.Find("Setter") != wxNOT_FOUND && elementType.Find("DivisionalNextBank") != wxNOT_FOUND)) {
		// we need to get the manual
		wxString manNbrStr = elementType.Mid(6 , 3);
		long value = 0;
		if (!manNbrStr.ToLong(&value)) {
			return;
		}
		int targetManual = (int) value;
		if (targetManual > (int)::wxGetApp().m_frame->m_organ->getNumberOfManuals()) {
			return;
		}
		createGUIDivisional(targetPanel, NULL);
		// the element type must be overridden
		GUIElement *e = targetPanel->getGuiElementAt(targetPanel->getNumberOfGuiElements() - 1);
		e->setType(elementType);
		e->setDisplayName(elementType);
	} else if (m_organ->getSetterElements().Index(elementType) != wxNOT_FOUND) {
		// any other valid type we simply create as a gui switch
		createGUISwitch(targetPanel, NULL);
		// the element type must be overridden
		GUIElement *e = targetPanel->getGuiElementAt(targetPanel->getNumberOfGuiElements() - 1);
		e->setType(elementType);
		e->setDisplayName(elementType);
	}
}

void OrganFileParser::createGUICoupler(GoPanel *targetPanel, Coupler *coupler) {
	GUIElement *guiCplr = new GUICoupler(coupler);
	guiCplr->setOwningPanel(targetPanel);
	guiCplr->setDisplayName(coupler->getName());
	guiCplr->setDefaultFont(targetPanel->getDisplayMetrics()->m_dispControlLabelFont);
	targetPanel->addGuiElement(guiCplr);

	GUICoupler *cplrElement = dynamic_cast<GUICoupler*>(guiCplr);
	if (cplrElement) {
		cplrElement->read(m_organFile, false, m_organ);
	}
}

void OrganFileParser::createGUIStop(GoPanel *targetPanel, Stop *stop) {
	GUIElement *guiStop = new GUIStop(stop);
	guiStop->setOwningPanel(targetPanel);
	guiStop->setDisplayName(stop->getName());
	guiStop->setDefaultFont(targetPanel->getDisplayMetrics()->m_dispControlLabelFont);
	targetPanel->addGuiElement(guiStop);

	GUIStop *stopElement = dynamic_cast<GUIStop*>(guiStop);
	if (stopElement) {
		stopElement->read(m_organFile, false, m_organ);
	}
}

void OrganFileParser::parsePanelElements(GoPanel *targetPanel, wxString panelId) {
	int nbrGuiElements = static_cast<int>(m_organFile->ReadLong("NumberOfGUIElements", 0));
	if (panelId.IsSameAs(wxT("Panel000"), false) || nbrGuiElements > 0) {
		if (nbrGuiElements > 0 && nbrGuiElements < 1000) {
			for (int i = 0; i < nbrGuiElements; i++) {
				wxString elementGroupName = wxT("Element") + GOODF_functions::number_format(i + 1);
				m_organFile->SetPath(wxT("/"));
				if (m_organFile->HasGroup(panelId + elementGroupName)) {
					m_organFile->SetPath(wxT("/") + panelId + elementGroupName);
					wxString elementType = m_organFile->Read("Type", wxEmptyString);
					if (elementType.IsSameAs(wxT("Divisional"))) {
						int manualIdx = static_cast<int>(m_organFile->ReadLong("Manual", -1));
						if (manualIdx >= 0 && manualIdx <= (int)m_organ->getNumberOfManuals()) {
							if (!m_organ->doesHavePedals() && manualIdx > 0)
								manualIdx -= 1;
							Manual *man = m_organ->getOrganManualAt(manualIdx);
							int divIdx = static_cast<int>(m_organFile->ReadLong("Divisional", 0));
							if (divIdx > 0 && divIdx <= (int)man->getNumberOfDivisionals()) {
								createGUIDivisional(targetPanel, man->getDivisionalAt(divIdx - 1));
							}
						}
					} else if (elementType.IsSameAs(wxT("Coupler"))) {
						int manualIdx = static_cast<int>(m_organFile->ReadLong("Manual", -1));
						if (manualIdx >= 0 && manualIdx <= (int)m_organ->getNumberOfManuals()) {
							if (!m_organ->doesHavePedals() && manualIdx > 0)
								manualIdx -= 1;
							Manual *man = m_organ->getOrganManualAt(manualIdx);
							int cplrIdx = static_cast<int>(m_organFile->ReadLong("Coupler", 0));
							if (cplrIdx > 0 && cplrIdx <= (int)man->getNumberOfCouplers()) {
								createGUICoupler(targetPanel, man->getCouplerAt(cplrIdx - 1));
							}
						}
					} else if (elementType.IsSameAs(wxT("Stop"))) {
						int manualIdx = static_cast<int>(m_organFile->ReadLong("Manual", -1));
						if (manualIdx >= 0 && manualIdx <= (int)m_organ->getNumberOfManuals()) {
							if (!m_organ->doesHavePedals() && manualIdx > 0)
								manualIdx -= 1;
							Manual *man = m_organ->getOrganManualAt(manualIdx);
							int stopIdx = static_cast<int>(m_organFile->ReadLong("Stop", 0));
							if (stopIdx > 0 && stopIdx <= (int)man->getNumberOfStops()) {
								createGUIStop(targetPanel, man->getStopAt(stopIdx - 1));
							}
						}
					} else if (elementType.IsSameAs(wxT("Enclosure"))) {
						int enclosureIdx = static_cast<int>(m_organFile->ReadLong("Enclosure", 0));
						if (enclosureIdx > 0 && enclosureIdx <= (int)m_organ->getNumberOfEnclosures()) {
							createGUIEnclosure(targetPanel, m_organ->getOrganEnclosureAt(enclosureIdx - 1));
						}
					} else if (elementType.IsSameAs(wxT("Tremulant"))) {
						int tremIdx = static_cast<int>(m_organFile->ReadLong("Tremulant", 0));
						if (tremIdx > 0 && tremIdx <= (int)m_organ->getNumberOfTremulants()) {
							createGUITremulant(targetPanel, m_organ->getOrganTremulantAt(tremIdx - 1));
						}
					} else if (elementType.IsSameAs(wxT("DivisionalCoupler"))) {
						int divCplrIdx = static_cast<int>(m_organFile->ReadLong("DivisionalCoupler", 0));
						if (divCplrIdx > 0 && divCplrIdx <= (int)m_organ->getNumberOfOrganDivisionalCouplers()) {
							createGUIDivCplr(targetPanel, m_organ->getOrganDivisionalCouplerAt(divCplrIdx - 1));
						}
					} else if (elementType.IsSameAs(wxT("General"))) {
						int genIdx = static_cast<int>(m_organFile->ReadLong("General", 0));
						if (genIdx > 0 && genIdx <= (int)m_organ->getNumberOfGenerals()) {
							createGUIGeneral(targetPanel, m_organ->getOrganGeneralAt(genIdx - 1));
						}
					} else if (elementType.IsSameAs(wxT("ReversiblePiston"))) {
						int revPistonIdx = static_cast<int>(m_organFile->ReadLong("ReversiblePiston", 0));
						if (revPistonIdx > 0 && revPistonIdx <= (int)m_organ->getNumberOfReversiblePistons()) {
							createGUIPiston(targetPanel, m_organ->getReversiblePistonAt(revPistonIdx - 1));
						}
					} else if (elementType.IsSameAs(wxT("Switch"))) {
						int switchIdx = static_cast<int>(m_organFile->ReadLong("Switch", 0));
						if (switchIdx > 0 && switchIdx <= (int)m_organ->getNumberOfSwitches()) {
							createGUISwitch(targetPanel, m_organ->getOrganSwitchAt(switchIdx - 1));
						}
					} else if (elementType.IsSameAs(wxT("Label"))) {
						createGUILabel(targetPanel);
					} else if (elementType.IsSameAs(wxT("Manual"))) {
						int manIdx = static_cast<int>(m_organFile->ReadLong("Manual", -1));
						if (manIdx >= 0 && manIdx <= (int)m_organ->getNumberOfManuals()) {
							if (!m_organ->doesHavePedals() && manIdx > 0)
								manIdx -= 1;
							createGUIManual(targetPanel, m_organ->getOrganManualAt(manIdx));
						}
					} else {
						// the type can also be a valid setter element
						createFromSetterElement(targetPanel, elementType);
					}
				} else {
					wxLogWarning("%s%s couldn't be found!", panelId, elementGroupName);
					::wxGetApp().m_frame->GetLogWindow()->Show(true);
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		} else {
			wxLogWarning("NumberOfGUIElements=%d is invalid in %s!", nbrGuiElements, panelId);
			::wxGetApp().m_frame->GetLogWindow()->Show(true);
		}
	} else {
		// old style panel elements are read in another way, but they will be converted to the new style
		int nbrManuals = static_cast<int>(m_organFile->ReadLong("NumberOfManuals", 0));
		for (int i = 0; i < nbrManuals; i++) {
			wxString manStr = wxT("Manual") + GOODF_functions::number_format(i + 1);
			int manRefId = static_cast<int>(m_organFile->ReadLong(manStr, -1));
			if (manRefId >= 0 && manRefId <= (int)m_organ->getNumberOfManuals()) {
				int manIdx = manRefId;
				if (!m_organ->doesHavePedals() && manIdx > 0)
					manIdx -= 1;
				wxString thePath = panelId + wxT("Manual") + GOODF_functions::number_format(manRefId);
				m_organFile->SetPath(wxT("/"));
				if (m_organFile->HasGroup(thePath)) {
					m_organFile->SetPath(wxT("/") + thePath);
					createGUIManual(targetPanel, m_organ->getOrganManualAt(manIdx));
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrSetters = static_cast<int>(m_organFile->ReadLong("NumberOfSetterElements", 0));
		for (int i = 0; i < nbrSetters; i++) {
			wxString setterGroupName = panelId + wxT("SetterElement") + GOODF_functions::number_format(i + 1);
			m_organFile->SetPath(wxT("/"));
			if (m_organFile->HasGroup(setterGroupName)) {
				m_organFile->SetPath(wxT("/") + setterGroupName);
				wxString elementType = m_organFile->Read("Type", wxEmptyString);
				if (elementType != wxEmptyString) {
					createFromSetterElement(m_organ->getOrganPanelAt(0), elementType);
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrEnclosures = static_cast<int>(m_organFile->ReadLong("NumberOfEnclosures", 0));
		for (int i = 0; i < nbrEnclosures; i++) {
			wxString encStr = wxT("Enclosure") + GOODF_functions::number_format(i + 1);
			int encRefId = static_cast<int>(m_organFile->ReadLong(encStr, 0));
			if (encRefId > 0 && encRefId <= (int)m_organ->getNumberOfEnclosures()) {
				wxString thePath = panelId + wxT("Enclosure") + GOODF_functions::number_format(encRefId);
				m_organFile->SetPath(wxT("/"));
				if (m_organFile->HasGroup(thePath)) {
					m_organFile->SetPath(wxT("/") + thePath);
					createGUIEnclosure(targetPanel, m_organ->getOrganEnclosureAt(encRefId - 1));
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrTremulants = static_cast<int>(m_organFile->ReadLong("NumberOfTremulants", 0));
		for (int i = 0; i < nbrTremulants; i++) {
			wxString tremStr = wxT("Tremulant") + GOODF_functions::number_format(i + 1);
			int tremRefId = static_cast<int>(m_organFile->ReadLong(tremStr, 0));
			if (tremRefId > 0 && tremRefId <= (int)m_organ->getNumberOfTremulants()) {
				wxString thePath = panelId + wxT("Tremulant") + GOODF_functions::number_format(tremRefId);
				m_organFile->SetPath(wxT("/"));
				if (m_organFile->HasGroup(thePath)) {
					m_organFile->SetPath(wxT("/") + thePath);
					createGUITremulant(targetPanel, m_organ->getOrganTremulantAt(tremRefId - 1));
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrPistons = static_cast<int>(m_organFile->ReadLong("NumberOfReversiblePistons", 0));
		for (int i = 0; i < nbrPistons; i++) {
			wxString pistonStr = wxT("ReversiblePiston") + GOODF_functions::number_format(i + 1);
			int pistonRefId = static_cast<int>(m_organFile->ReadLong(pistonStr, 0));
			if (pistonRefId > 0 && pistonRefId <= (int)m_organ->getNumberOfReversiblePistons()) {
				wxString thePath = panelId + wxT("ReversiblePiston") + GOODF_functions::number_format(pistonRefId);
				m_organFile->SetPath(wxT("/"));
				if (m_organFile->HasGroup(thePath)) {
					m_organFile->SetPath(wxT("/") + thePath);
					createGUIPiston(targetPanel, m_organ->getReversiblePistonAt(pistonRefId - 1));
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrSwitches = static_cast<int>(m_organFile->ReadLong("NumberOfSwitches", 0));
		for (int i = 0; i < nbrSwitches; i++) {
			wxString swStr = wxT("Switch") + GOODF_functions::number_format(i + 1);
			int swRefId = static_cast<int>(m_organFile->ReadLong(swStr, 0));
			if (swRefId > 0 && swRefId <= (int)m_organ->getNumberOfSwitches()) {
				wxString thePath = panelId + wxT("Switch") + GOODF_functions::number_format(swRefId);
				m_organFile->SetPath(wxT("/"));
				if (m_organFile->HasGroup(thePath)) {
					m_organFile->SetPath(wxT("/") + thePath);
					createGUISwitch(targetPanel, m_organ->getOrganSwitchAt(swRefId - 1));
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrGenerals = static_cast<int>(m_organFile->ReadLong("NumberOfGenerals", 0));
		for (int i = 0; i < nbrGenerals; i++) {
			wxString genStr = wxT("General") + GOODF_functions::number_format(i + 1);
			int genRefId = static_cast<int>(m_organFile->ReadLong(genStr, 0));
			if (genRefId > 0 && genRefId <= (int)m_organ->getNumberOfGenerals()) {
				wxString thePath = panelId + wxT("General") + GOODF_functions::number_format(genRefId);
				m_organFile->SetPath(wxT("/"));
				if (m_organFile->HasGroup(thePath)) {
					m_organFile->SetPath(wxT("/") + thePath);
					createGUIGeneral(targetPanel, m_organ->getOrganGeneralAt(genRefId - 1));
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrDivCplrs = static_cast<int>(m_organFile->ReadLong("NumberOfDivisionalCouplers", 0));
		for (int i = 0; i < nbrDivCplrs; i++) {
			wxString divCplrStr = wxT("DivisionalCoupler") + GOODF_functions::number_format(i + 1);
			int divCplrRefId = static_cast<int>(m_organFile->ReadLong(divCplrStr, 0));
			if (divCplrRefId > 0 && divCplrRefId <= (int)m_organ->getNumberOfOrganDivisionalCouplers()) {
				wxString thePath = panelId + wxT("DivisionalCoupler") + GOODF_functions::number_format(divCplrRefId);
				m_organFile->SetPath(wxT("/"));
				if (m_organFile->HasGroup(thePath)) {
					m_organFile->SetPath(wxT("/") + thePath);
					createGUIDivCplr(targetPanel, m_organ->getOrganDivisionalCouplerAt(divCplrRefId - 1));
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrStops = static_cast<int>(m_organFile->ReadLong("NumberOfStops", 0));
		for (int i = 0; i < nbrStops; i++) {
			wxString stopManStr = wxT("Stop") + GOODF_functions::number_format(i + 1) + wxT("Manual");
			wxString stopStr = wxT("Stop") + GOODF_functions::number_format(i + 1);
			int manRefId = static_cast<int>(m_organFile->ReadLong(stopManStr, -1));
			if (manRefId >= 0 && manRefId <= (int)m_organ->getNumberOfManuals()) {
				int manIdx = manRefId;
				if (!m_organ->doesHavePedals() && manIdx > 0)
					manIdx -= 1;
				Manual *man = m_organ->getOrganManualAt(manIdx);
				int stopRefId = static_cast<int>(m_organFile->ReadLong(stopStr, 0));
				if (stopRefId > 0 && stopRefId <= (int)man->getNumberOfStops()) {
					wxString thePath = panelId + wxT("Stop") + GOODF_functions::number_format(i + 1);
					m_organFile->SetPath(wxT("/"));
					if (m_organFile->HasGroup(thePath)) {
						m_organFile->SetPath(wxT("/") + thePath);
						createGUIStop(targetPanel, man->getStopAt(stopRefId - 1));
					}
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrCplrs = static_cast<int>(m_organFile->ReadLong("NumberOfCouplers", 0));
		for (int i = 0; i < nbrCplrs; i++) {
			wxString couplerManStr = wxT("Coupler") + GOODF_functions::number_format(i + 1) + wxT("Manual");
			wxString couplerStr = wxT("Coupler") + GOODF_functions::number_format(i + 1);
			int manRefId = static_cast<int>(m_organFile->ReadLong(couplerManStr, -1));
			if (manRefId >= 0 && manRefId <= (int)m_organ->getNumberOfManuals()) {
				int manIdx = manRefId;
				if (!m_organ->doesHavePedals() && manIdx > 0)
					manIdx -= 1;
				Manual *man = m_organ->getOrganManualAt(manIdx);
				int couplerRefId = static_cast<int>(m_organFile->ReadLong(couplerStr, 0));
				if (couplerRefId > 0 && couplerRefId <= (int)man->getNumberOfCouplers()) {
					wxString thePath = panelId + wxT("Coupler") + GOODF_functions::number_format(i + 1);
					m_organFile->SetPath(wxT("/"));
					if (m_organFile->HasGroup(thePath)) {
						m_organFile->SetPath(wxT("/") + thePath);
						createGUICoupler(targetPanel, man->getCouplerAt(couplerRefId - 1));
					}
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrDivisionals = static_cast<int>(m_organFile->ReadLong("NumberOfDivisionals", 0));
		for (int i = 0; i < nbrDivisionals; i++) {
			wxString divManStr = wxT("Divisional") + GOODF_functions::number_format(i + 1) + wxT("Manual");
			wxString divStr = wxT("Divisional") + GOODF_functions::number_format(i + 1);
			int manRefId = static_cast<int>(m_organFile->ReadLong(divManStr, -1));
			if (manRefId >= 0 && manRefId <= (int)m_organ->getNumberOfManuals()) {
				int manIdx = manRefId;
				if (!m_organ->doesHavePedals() && manIdx > 0)
					manIdx -= 1;
				Manual *man = m_organ->getOrganManualAt(manIdx);
				int divRefId = static_cast<int>(m_organFile->ReadLong(divStr, 0));
				if (divRefId > 0 && divRefId <= (int)man->getNumberOfDivisionals()) {
					wxString thePath = panelId + wxT("Divisional") + GOODF_functions::number_format(i + 1);
					m_organFile->SetPath(wxT("/"));
					if (m_organFile->HasGroup(thePath)) {
						m_organFile->SetPath(wxT("/") + thePath);
						createGUIDivisional(targetPanel, man->getDivisionalAt(divRefId - 1));
					}
				}
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
		int nbrLabels = static_cast<int>(m_organFile->ReadLong("NumberOfLabels", 0));
		for (int i = 0; i < nbrLabels; i++) {
			wxString thePath = panelId + wxT("Label") + GOODF_functions::number_format(i + 1);
			m_organFile->SetPath(wxT("/"));
			if (m_organFile->HasGroup(thePath)) {
				m_organFile->SetPath(wxT("/") + thePath);
				createGUILabel(targetPanel);
			}
			m_organFile->SetPath(wxT("/") + panelId);
		}
	}
}
