/*
 * Manual.cpp is part of GOODF.
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

#include "Manual.h"
#include "GOODF.h"
#include "GOODFFunctions.h"
#include <utility>
#include <vector>
#include <algorithm>

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

void Manual::setIsPedal(bool isPedal) {
	m_thePedal = isPedal;
	if (m_thePedal)
		::wxGetApp().m_frame->m_organ->setHasPedals(true);
	// TODO: maybe need to adjust organ value of has pedals if the manual is deleted?
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
	m_couplers.remove(coupler);
}

void Manual::removeCouplerAt(unsigned index) {
	std::list<Coupler *>::iterator it = m_couplers.begin();
	std::advance(it, index);
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
}

void Manual::removeTremulantAt(unsigned index) {
	std::list<Tremulant *>::iterator it = m_tremulants.begin();
	std::advance(it, index);
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
	m_switches.remove(sw);
}

void Manual::removeGoSwitchAt(unsigned index) {
	std::list<GoSwitch *>::iterator it = m_switches.begin();
	std::advance(it, index);
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
	for (unsigned i = 0; i < 127; i++) {
		wxString key = wxT("MIDIKey") + GOODF_functions::number_format(i);
		originalMidiKeyMap.insert(std::make_pair(key, i));
	}
	m_midiKeyMap = originalMidiKeyMap;
}
