/*
 * Manual.cpp is part of GoOdf.
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

#include "Manual.h"
#include "GOODF.h"
#include "GOODFFunctions.h"
#include <utility>
#include <vector>
#include <algorithm>
#include "GUIStop.h"
#include "GUICoupler.h"
#include "GUIDivisional.h"

Manual::Manual() {
	m_name = wxT("New Manual");
	m_numberOfLogicalKeys = 61;
	m_firstAccessibleKeyLogicalKeyNumber = 1;
	m_firstAccessibleKeyMIDINoteNumber = 36;
	m_numberOfAccessibleKeys = 61;
	m_midiInputNumber = 0;
	m_displayed = false;
	m_thePedal = false;

	SetupBasicMidiKeyMap();
}

Manual::~Manual() {

}

void Manual::write(wxTextFile *outFile) {
	outFile->AddLine(wxT("Name=") + m_name);
	outFile->AddLine(wxT("NumberOfLogicalKeys=") + wxString::Format(wxT("%i"), m_numberOfLogicalKeys));
	outFile->AddLine(wxT("FirstAccessibleKeyLogicalKeyNumber=") + wxString::Format(wxT("%i"), m_firstAccessibleKeyLogicalKeyNumber));
	outFile->AddLine(wxT("FirstAccessibleKeyMIDINoteNumber=") + wxString::Format(wxT("%i"), m_firstAccessibleKeyMIDINoteNumber));
	outFile->AddLine(wxT("NumberOfAccessibleKeys=") + wxString::Format(wxT("%i"), m_numberOfAccessibleKeys));
	if (m_midiInputNumber != 0)
		outFile->AddLine(wxT("MIDIInputNumber=") + wxString::Format(wxT("%i"), m_midiInputNumber));
	unsigned nbStops = m_stops.size();
	outFile->AddLine(wxT("NumberOfStops=") + wxString::Format(wxT("%u"), nbStops));
	unsigned counter = 1;
	for (Stop *s : m_stops) {
		wxString stopId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganStop(s));
		outFile->AddLine(wxT("Stop") + GOODF_functions::number_format(counter) + wxT("=") + stopId);
		counter++;
	}
	unsigned nbCouplers = m_couplers.size();
	if (nbCouplers > 0) {
		outFile->AddLine(wxT("NumberOfCouplers=") + wxString::Format(wxT("%u"), nbCouplers));
		counter = 1;
		for (Coupler *c : m_couplers) {
			wxString couplerId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganCoupler(c));
			outFile->AddLine(wxT("Coupler") + GOODF_functions::number_format(counter) + wxT("=") + couplerId);
			counter++;
		}
	}
	unsigned nbDivisionals = m_divisionals.size();
	if (nbDivisionals > 0) {
		outFile->AddLine(wxT("NumberOfDivisionals=") + wxString::Format(wxT("%u"), nbDivisionals));
		counter = 1;
		for (Divisional *d : m_divisionals) {
			wxString divId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganDivisional(d));
			outFile->AddLine(wxT("Divisional") + GOODF_functions::number_format(counter) + wxT("=") + divId);
			counter++;
		}
	}
	unsigned nbTrems = m_tremulants.size();
	if (nbTrems > 0) {
		outFile->AddLine(wxT("NumberOfTremulants=") + wxString::Format(wxT("%u"), nbTrems));
		counter = 1;
		for (Tremulant *t : m_tremulants) {
			wxString tremId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(t));
			outFile->AddLine(wxT("Tremulant") + GOODF_functions::number_format(counter) + wxT("=") + tremId);
			counter++;
		}
	}
	unsigned nbSwitches = m_switches.size();
	if (nbSwitches > 0) {
		outFile->AddLine(wxT("NumberOfSwitches=") + wxString::Format(wxT("%u"), nbSwitches));
		counter = 1;
		for (GoSwitch *sw : m_switches) {
			wxString switchId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(sw));
			outFile->AddLine(wxT("Switch") + GOODF_functions::number_format(counter) + wxT("=") + switchId);
			counter++;
		}
	}
	std::vector<std::pair<wxString, unsigned>> differences;
	std::set_difference(begin(m_midiKeyMap), end(m_midiKeyMap), begin(originalMidiKeyMap), end(originalMidiKeyMap), std::back_inserter(differences));
	for (auto p : differences) {
		wxString identifier = p.first;
		wxString value = GOODF_functions::number_format(p.second);
		outFile->AddLine(identifier + wxT("=") + value);
	}
}

void Manual::read(wxFileConfig *cfg, bool useOldPanelFormat, wxString manId, Organ *readOrgan) {
	m_name = cfg->Read("Name", wxEmptyString);
	int logicalKeys = static_cast<int>(cfg->ReadLong("NumberOfLogicalKeys", 1));
	if (logicalKeys > 0 && logicalKeys < 193) {
		m_numberOfLogicalKeys = logicalKeys;
	}
	int firstAccessibleKey = static_cast<int>(cfg->ReadLong("FirstAccessibleKeyLogicalKeyNumber", 1));
	if (firstAccessibleKey > 0 && firstAccessibleKey < m_numberOfLogicalKeys) {
		m_firstAccessibleKeyLogicalKeyNumber = firstAccessibleKey;
	}
	int firstMidiNote = static_cast<int>(cfg->ReadLong("FirstAccessibleKeyMIDINoteNumber", 36));
	if (firstMidiNote > -1 && firstMidiNote < 128) {
		m_firstAccessibleKeyMIDINoteNumber = firstMidiNote;
	}
	int accessibleKeys = static_cast<int>(cfg->ReadLong("NumberOfAccessibleKeys", 0));
	if (accessibleKeys > -1 && accessibleKeys < 86) {
		m_numberOfAccessibleKeys = accessibleKeys;
	}
	int midiInput = static_cast<int>(cfg->ReadLong("MIDIInputNumber", 0));
	if (midiInput > -1 && midiInput < 201) {
		m_midiInputNumber = midiInput;
	}
	wxString cfgBoolValue = cfg->Read("Displayed", wxEmptyString);
	m_displayed = GOODF_functions::parseBoolean(cfgBoolValue, useOldPanelFormat);
	for (int i = 0; i < 128; i++) {
		wxString mapStr = wxT("MIDIKey") + GOODF_functions::number_format(i);
		int mappingValue = static_cast<int>(cfg->ReadLong(mapStr, i));
		if (mappingValue != i) {
			if (mappingValue >= 0 && mappingValue <= 127) {
				setMidiKeyMapValue(mapStr, mappingValue);
			}
		}
	}
	int nbrSwitches = static_cast<int>(cfg->ReadLong("NumberOfSwitches", 0));
	if (nbrSwitches > 0 && nbrSwitches <= (int) readOrgan->getNumberOfSwitches()) {
		for (int i = 0; i < nbrSwitches; i++) {
			wxString switchNbr = wxT("Switch") + GOODF_functions::number_format(i + 1);
			int swRefNbr = static_cast<int>(cfg->ReadLong(switchNbr, 0));
			if (swRefNbr > 0 && swRefNbr <= (int) readOrgan->getNumberOfSwitches()) {
				addGoSwitch(readOrgan->getOrganSwitchAt(swRefNbr - 1));
			}
		}
	}
	int nbrTrems = static_cast<int>(cfg->ReadLong("NumberOfTremulants", 0));
	if (nbrTrems > 0 && nbrTrems <= (int) readOrgan->getNumberOfTremulants()) {
		for (int i = 0; i < nbrTrems; i++) {
			wxString tremNbr = wxT("Tremulant") + GOODF_functions::number_format(i + 1);
			int tremRefNbr = static_cast<int>(cfg->ReadLong(tremNbr, 0));
			if (tremRefNbr > 0 && tremRefNbr <= (int) readOrgan->getNumberOfTremulants()) {
				addTremulant(readOrgan->getOrganTremulantAt(tremRefNbr - 1));
			}
		}
	}
	int nbrStops = static_cast<int>(cfg->ReadLong("NumberOfStops", 0));

	if (nbrStops > 0 && nbrStops < 1000) {
		for (int i = 0; i < nbrStops; i++) {
			cfg->SetPath(wxT("/") + manId);
			wxString stopNbr = wxT("Stop") + GOODF_functions::number_format(i + 1);
			int stopIdx = static_cast<int>(cfg->ReadLong(stopNbr, 0));
			wxString stopGroup = wxT("Stop") + GOODF_functions::number_format(stopIdx);
			cfg->SetPath("/");
			if (cfg->HasGroup(stopGroup)) {
				cfg->SetPath(wxT("/") + stopGroup);
				Stop s;
				s.read(cfg, useOldPanelFormat, this, readOrgan);
				readOrgan->addStop(s, true);
				addStop(readOrgan->getOrganStopAt(readOrgan->getNumberOfStops() - 1));
				if (s.isUsingInternalRank()) {
					bool rankUsesLegacyXfades = false;
					for (Pipe& p : s.getInternalRank()->m_pipes) {
						if (!p.m_attacks.front().loadRelease && p.m_attacks.front().releaseCrossfadeLength) {
							// This is certainly a legacy x-fade!
							wxLogWarning("[Stop%0.3d] %s uses Pipe999ReleaseCrossfadeLength with LoadRelease=N! You might want to use Tools->Import Legacy X-fades.", readOrgan->getNumberOfStops(), s.getName());
							rankUsesLegacyXfades = true;
						}
						if (rankUsesLegacyXfades) {
							::wxGetApp().m_frame->GetLogWindow()->Show(true);
							break;
						}
					}
				}
				if (s.isDisplayed()) {
					// we must also create a GUI element for that stop from this group information
					int lastStopIdx = m_stops.size() - 1;
					GUIElement *guiStop = new GUIStop(getStopAt(lastStopIdx));
					guiStop->setOwningPanel(readOrgan->getOrganPanelAt(0));
					guiStop->setDisplayName(s.getName());
					readOrgan->getOrganPanelAt(0)->addGuiElement(guiStop);

					GUIStop *stopElement = dynamic_cast<GUIStop*>(guiStop);
					if (stopElement) {
						stopElement->read(cfg, false, readOrgan);
					}
				}
			} else {
				wxLogWarning("[%s] section couldn't be found!", stopGroup);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
	}
	cfg->SetPath(wxT("/") + manId);
}

void Manual::readCouplers(wxFileConfig *cfg, bool useOldPanelFormat, wxString manId, Organ *readOrgan) {
	int nbrCouplers = static_cast<int>(cfg->ReadLong("NumberOfCouplers", 0));

	if (nbrCouplers > 0 && nbrCouplers < 1000) {
		for (int i = 0; i < nbrCouplers; i++) {
			cfg->SetPath(wxT("/") + manId);
			wxString couplerNbr = wxT("Coupler") + GOODF_functions::number_format(i + 1);
			int cplrIdx = static_cast<int>(cfg->ReadLong(couplerNbr, 0));
			wxString couplerGroup = wxT("Coupler") + GOODF_functions::number_format(cplrIdx);
			cfg->SetPath("/");
			if (cfg->HasGroup(couplerGroup)) {
				cfg->SetPath(wxT("/") + couplerGroup);
				Coupler c;
				c.read(cfg, useOldPanelFormat, this, readOrgan);
				readOrgan->addCoupler(c, true);
				addCoupler(readOrgan->getOrganCouplerAt(readOrgan->getNumberOfCouplers() - 1));
				if (c.isDisplayed()) {
					// we must also create a GUI element for that coupler from this group information
					int lastCplrIdx = m_couplers.size() - 1;
					GUIElement *guiCplr = new GUICoupler(getCouplerAt(lastCplrIdx));
					guiCplr->setOwningPanel(readOrgan->getOrganPanelAt(0));
					guiCplr->setDisplayName(c.getName());
					readOrgan->getOrganPanelAt(0)->addGuiElement(guiCplr);

					GUICoupler *cplrElement = dynamic_cast<GUICoupler*>(guiCplr);
					if (cplrElement) {
						cplrElement->read(cfg, false, readOrgan);
					}
				}
			} else {
				wxLogWarning("[%s] section couldn't be found!", couplerGroup);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
	}
}

void Manual::readDivisionals(wxFileConfig *cfg, bool useOldPanelFormat, wxString manId, Organ *readOrgan) {
	int nbrDivisionals = static_cast<int>(cfg->ReadLong("NumberOfDivisionals", 0));
	if (nbrDivisionals > 0 && nbrDivisionals < 1000) {
		for (int i = 0; i < nbrDivisionals; i++) {
			cfg->SetPath(wxT("/") + manId);
			wxString divNbr = wxT("Divisional") + GOODF_functions::number_format(i + 1);
			int divIdx = static_cast<int>(cfg->ReadLong(divNbr, 0));
			wxString divGroup = wxT("Divisional") + GOODF_functions::number_format(divIdx);
			cfg->SetPath("/");
			if (cfg->HasGroup(divGroup)) {
				cfg->SetPath(wxT("/") + divGroup);
				Divisional d;
				d.read(cfg, useOldPanelFormat, this);
				readOrgan->addDivisional(d, true);
				addDivisional(readOrgan->getOrganDivisionalAt(readOrgan->getNumberOfDivisionals() - 1));
				if (d.isDisplayed()) {
					// we must also create a GUI element for that divisional from this group information
					int lastDivIdx = m_divisionals.size() - 1;
					GUIElement *guiDiv = new GUIDivisional(getDivisionalAt(lastDivIdx));
					guiDiv->setOwningPanel(readOrgan->getOrganPanelAt(0));
					guiDiv->setDisplayName(d.getName());
					readOrgan->getOrganPanelAt(0)->addGuiElement(guiDiv);

					GUIDivisional *divElement = dynamic_cast<GUIDivisional*>(guiDiv);
					if (divElement) {
						divElement->read(cfg, true, readOrgan);
					}
				}
			} else {
				wxLogWarning("[%s] section couldn't be found!", divGroup);
				::wxGetApp().m_frame->GetLogWindow()->Show(true);
			}
		}
	}
}

wxString Manual::getName() {
	return m_name;
}

void Manual::setName(wxString name) {
	m_name = name;
}

int Manual::getNumberOfLogicalKeys() {
	return m_numberOfLogicalKeys;
}

void Manual::setNumberOfLogicalKeys(int nbKeys) {
	if (nbKeys > 0 && nbKeys < 193)
		m_numberOfLogicalKeys = nbKeys;
}

int Manual::getFirstAccessibleKeyLogicalKeyNumber() {
	return m_firstAccessibleKeyLogicalKeyNumber;
}

void Manual::setFirstAccessibleKeyLogicalKeyNumber(int firstKeyNbr) {
	if (firstKeyNbr > 0 && firstKeyNbr <= m_numberOfLogicalKeys)
		m_firstAccessibleKeyLogicalKeyNumber = firstKeyNbr;
}

int Manual::getFirstAccessibleKeyMIDINoteNumber() {
	return m_firstAccessibleKeyMIDINoteNumber;
}

void Manual::setFirstAccessibleKeyMIDINoteNumber(int firstNoteNbr) {
	if (firstNoteNbr > -1 && firstNoteNbr < 128)
		m_firstAccessibleKeyMIDINoteNumber = firstNoteNbr;
}

int Manual::getNumberOfAccessibleKeys() {
	return m_numberOfAccessibleKeys;
}

void Manual::setNumberOfAccessibleKeys(int nbKeys) {
	if (nbKeys > -1 && nbKeys < 86)
		m_numberOfAccessibleKeys = nbKeys;
}

int Manual::getMidiInputNumber() {
	return m_midiInputNumber;
}

void Manual::setMidiInputNumber(int midiInputNbr) {
	if (midiInputNbr > -1 && midiInputNbr < 201)
		m_midiInputNumber = midiInputNbr;
}

bool Manual::isThePedal() {
	return m_thePedal;
}

void Manual::setIsPedal(bool isPedal, bool isParsing) {
	m_thePedal = isPedal;
	::wxGetApp().m_frame->m_organ->setHasPedals(isPedal, isParsing);
}

bool Manual::isDisplayed() {
	return m_displayed;
}

unsigned Manual::getNumberOfStops() {
	return m_stops.size();
}

Stop* Manual::getStopAt(unsigned index) {
	auto iterator = std::next(m_stops.begin(), index);
	return (*iterator);
}

void Manual::addStop(Stop* stop) {
	m_stops.push_back(stop);
}

void Manual::removeStop(Stop* stop) {
	// also remove stop from any divisional
	for (auto& d : m_divisionals) {
		if (d->hasStop(stop))
			d->removeStop(stop);
	}
	m_stops.remove(stop);
}

void Manual::removeStopAt(unsigned index) {
	std::list<Stop *>::iterator it = m_stops.begin();
	std::advance(it, index);
	m_stops.erase(it);
}

int Manual::getIndexOfStop(Stop *stop) {
	int index = 0;
	for (auto& s : m_stops) {
		if (s == stop)
			return index;

		index++;
	}
	return -1;
}

void Manual::moveStop(int idxToMove, int destinationIdx) {
	auto theOneToMove = std::next(m_stops.begin(), idxToMove);
	std::list<Stop*>::iterator it = m_stops.begin();

	if (destinationIdx > (int) m_stops.size() - 1) {
		it = m_stops.end();
	} else {
		it = std::next(m_stops.begin(), destinationIdx);
	}

	m_stops.splice(it, m_stops, theOneToMove);
}

unsigned Manual::getNumberOfCouplers() {
	return m_couplers.size();
}

Coupler* Manual::getCouplerAt(unsigned index) {
	auto iterator = std::next(m_couplers.begin(), index);
	return (*iterator);
}

void Manual::addCoupler(Coupler* coupler) {
	m_couplers.push_back(coupler);
}

void Manual::removeCoupler(Coupler* coupler) {
	// also remove coupler from any divisional
	for (auto& d : m_divisionals) {
		if (d->hasCoupler(coupler))
			d->removeCoupler(coupler);
	}
	m_couplers.remove(coupler);
}

void Manual::removeCouplerAt(unsigned index) {
	std::list<Coupler *>::iterator it = m_couplers.begin();
	std::advance(it, index);
	// also remove coupler from any divisional
	for (auto& d : m_divisionals) {
		if (d->hasCoupler(*it))
			d->removeCoupler(*it);
	}
	m_couplers.erase(it);
}

int Manual::getIndexOfCoupler(Coupler *coupler) {
	int index = 0;
	for (auto& c : m_couplers) {
		if (c == coupler)
			return index;

		index++;
	}
	return -1;
}

unsigned Manual::getNumberOfDivisionals() {
	return m_divisionals.size();
}

Divisional* Manual::getDivisionalAt(unsigned index) {
	auto iterator = std::next(m_divisionals.begin(), index);
	return (*iterator);
}

void Manual::addDivisional(Divisional* divisional) {
	m_divisionals.push_back(divisional);
}

void Manual::removeDivisional(Divisional* divisional) {
	m_divisionals.remove(divisional);
}

void Manual::removeDivisionalAt(unsigned index) {
	std::list<Divisional *>::iterator it = m_divisionals.begin();
	std::advance(it, index);
	m_divisionals.erase(it);
}

int  Manual::getIndexOfDivisional(Divisional *divisional) {
	int index = 0;
	for (auto& d : m_divisionals) {
		if (d == divisional)
			return index;

		index++;
	}
	return -1;
}

unsigned Manual::getNumberOfTremulants() {
	return m_tremulants.size();
}

Tremulant* Manual::getTremulantAt(unsigned index) {
	auto iterator = std::next(m_tremulants.begin(), index);
	return (*iterator);
}

void Manual::addTremulant(Tremulant* tremulant) {
	m_tremulants.push_back(tremulant);
}

void Manual::removeTremulant(Tremulant* tremulant) {
	m_tremulants.remove(tremulant);
	// if tremulant is removed from manual it should also be removed from its divisionals
	for (auto& d : m_divisionals) {
		if (d->hasTremulant(tremulant)) {
			d->removeTremulant(tremulant);
		}
	}
}

void Manual::removeTremulantAt(unsigned index) {
	std::list<Tremulant *>::iterator it = m_tremulants.begin();
	std::advance(it, index);
	// if tremulant is removed from manual it should also be removed from its divisionals
	for (auto& d : m_divisionals) {
		if (d->hasTremulant(*it)) {
			d->removeTremulant(*it);
		}
	}
	m_tremulants.erase(it);
}

int Manual::getIndexOfTremulant(Tremulant *tremulant) {
	int index = 0;
	for (auto& t : m_tremulants) {
		if (t == tremulant)
			return index;

		index++;
	}
	return -1;
}

bool Manual::hasTremulantReference(Tremulant *tremulant) {
	bool found = false;
	for (auto& trem : m_tremulants) {
		if (trem == tremulant) {
			found = true;
		   	break;
	    }
	}
	return found;
}

unsigned Manual::getNumberOfGoSwitches() {
	return m_switches.size();
}

GoSwitch* Manual::getGoSwitchAt(unsigned index) {
	auto iterator = std::next(m_switches.begin(), index);
	return (*iterator);
}

void Manual::addGoSwitch(GoSwitch* sw) {
	m_switches.push_back(sw);
}

void Manual::removeGoSwitch(GoSwitch* sw) {
	// also remove switch from any divisional
	for (auto& d : m_divisionals) {
		if (d->hasSwitch(sw))
			d->removeSwitch(sw);
	}
	// and any couplers
	for (auto& c : m_couplers) {
		if (c->hasSwitchReference(sw)) {
			c->removeSwitchReference(sw);
		}
	}
	// as well as any stop
	for (auto& s : m_stops) {
		if (s->hasSwitchReference(sw)) {
			s->removeSwitchReference(sw);
		}
	}
	m_switches.remove(sw);
}

void Manual::removeGoSwitchAt(unsigned index) {
	std::list<GoSwitch*>::iterator it = m_switches.begin();
	std::advance(it, index);
	// also remove switch from any divisional
	for (auto& d : m_divisionals) {
		if (d->hasSwitch(*it))
			d->removeSwitch(*it);
	}
	// and any couplers
	for (auto& c : m_couplers) {
		if (c->hasSwitchReference(*it)) {
			c->removeSwitchReference(*it);
		}
	}
	// as well as any stop
	for (auto& s : m_stops) {
		if (s->hasSwitchReference(*it)) {
			s->removeSwitchReference(*it);
		}
	}
	m_switches.erase(it);
}

int Manual::getIndexOfGoSwitch(GoSwitch *sw) {
	int index = 0;
	for (auto& s : m_switches) {
		if (s == sw)
			return index;

		index++;
	}
	return -1;
}

bool Manual::hasGoSwitchReference(GoSwitch *sw) {
	bool found = false;
	for (auto& a_switch : m_switches) {
		if (a_switch == sw) {
			found = true;
		   	break;
	    }
	}
	return found;
}

int Manual::getMidiKeyMapValue(wxString key) {
	std::map<wxString, unsigned>::iterator it;
	if ((it = m_midiKeyMap.find(key)) != m_midiKeyMap.end())
		return it->second;
	else
		return -1;
}

void Manual::setMidiKeyMapValue(wxString key, int value) {
	if (value > -1 && value < 128) {
		std::map<wxString, unsigned>::iterator it;
		if ((it = m_midiKeyMap.find(key)) != m_midiKeyMap.end())
			it->second = value;
	}
}

void Manual::SetupBasicMidiKeyMap() {
	for (unsigned i = 0; i < 128; i++) {
		wxString key = wxT("MIDIKey") + GOODF_functions::number_format(i);
		originalMidiKeyMap.insert(std::make_pair(key, i));
	}
	m_midiKeyMap = originalMidiKeyMap;
}
