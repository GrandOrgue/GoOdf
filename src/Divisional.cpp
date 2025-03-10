/*
 * Divisional.cpp is part of GoOdf.
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

#include "Divisional.h"
#include "GOODF.h"
#include "GOODFFunctions.h"

Divisional::Divisional() : Button() {
	m_owningManual = NULL;
	name = wxT("New Divisional");
	m_protected = false;
}

Divisional::~Divisional() {

}

void Divisional::write(wxTextFile *outFile) {
	outFile->AddLine(wxT("Name=") + name);
	if (m_protected)
		outFile->AddLine(wxT("Protected=Y"));
	unsigned nbStops = getNumberOfStops();
	outFile->AddLine(wxT("NumberOfStops=") + wxString::Format(wxT("%u"), nbStops));
	unsigned counter = 1;
	for (std::pair<Stop*, bool> stop : m_stops) {
		int stopIdx = m_owningManual->getIndexOfStop(stop.first);
		wxString stopId = GOODF_functions::number_format(stopIdx + 1);
		if (stop.second)
			outFile->AddLine(wxT("Stop") + GOODF_functions::number_format(counter) + wxT("=") + stopId);
		else
			outFile->AddLine(wxT("Stop") + GOODF_functions::number_format(counter) + wxT("=-") + stopId);

		counter++;
	}
	unsigned nbCouplers = getNumberOfCouplers();
	outFile->AddLine(wxT("NumberOfCouplers=") + wxString::Format(wxT("%u"), nbCouplers));
	counter = 1;
	for (std::pair<Coupler*, bool> coupler : m_couplers) {
		int couplerIdx = m_owningManual->getIndexOfCoupler(coupler.first);
		wxString couplerId = GOODF_functions::number_format(couplerIdx + 1);
		if (coupler.second)
			outFile->AddLine(wxT("Coupler") + GOODF_functions::number_format(counter) + wxT("=") + couplerId);
		else
			outFile->AddLine(wxT("Coupler") + GOODF_functions::number_format(counter) + wxT("=-") + couplerId);

		counter++;
	}
	unsigned nbTrems = getNumberOfTremulants();
	outFile->AddLine(wxT("NumberOfTremulants=") + wxString::Format(wxT("%u"), nbTrems));
	counter = 1;
	for (std::pair<Tremulant*, bool> trem : m_tremulants) {
		int tremIdx = m_owningManual->getIndexOfTremulant(trem.first);
		wxString tremId = GOODF_functions::number_format(tremIdx + 1);
		if (trem.second)
			outFile->AddLine(wxT("Tremulant") + GOODF_functions::number_format(counter) + wxT("=") + tremId);
		else
			outFile->AddLine(wxT("Tremulant") + GOODF_functions::number_format(counter) + wxT("=-") + tremId);

		counter++;
	}
	unsigned nbSwitches = getNumberOfSwitches();
	outFile->AddLine(wxT("NumberOfSwitches=") + wxString::Format(wxT("%u"), nbSwitches));
	counter = 1;
	for (std::pair<GoSwitch*, bool> sw : m_switches) {
		int switchIdx = m_owningManual->getIndexOfGoSwitch(sw.first);
		wxString switchId = GOODF_functions::number_format(switchIdx + 1);
		if (sw.second)
			outFile->AddLine(wxT("Switch") + GOODF_functions::number_format(counter) + wxT("=") + switchId);
		else
			outFile->AddLine(wxT("Switch") + GOODF_functions::number_format(counter) + wxT("=-") + switchId);

		counter++;
	}
}

void Divisional::read(wxFileConfig *cfg, bool usingOldPanelFormat, Manual *owning_manual) {
	m_owningManual = owning_manual;
	Button::read(cfg, usingOldPanelFormat);
	wxString cfgBoolValue = cfg->Read("Protected", wxEmptyString);
	m_protected = GOODF_functions::parseBoolean(cfgBoolValue, false);
	int nbrStops = static_cast<int>(cfg->ReadLong("NumberOfStops", 0));
	if (nbrStops > 0 && nbrStops <= (int) m_owningManual->getNumberOfStops()) {
		for (int i = 0; i < nbrStops; i++) {
			wxString stopNbr = wxT("Stop") + GOODF_functions::number_format(i + 1);
			wxString stopOnMan = cfg->Read(stopNbr, wxEmptyString);
			long value = 0;
			if (stopOnMan.ToLong(&value)) {
				if (value > 0) {
					// the stop is on
					m_stops.push_back(std::make_pair(m_owningManual->getStopAt(value - 1), true));
				} else {
					// the stop is off
					m_stops.push_back(std::make_pair(m_owningManual->getStopAt(labs(value) - 1), false));
				}
			}
		}
	}
	int nbrCplrs = static_cast<int>(cfg->ReadLong("NumberOfCouplers", 0));
	if (nbrCplrs > 0 && nbrCplrs <= (int) m_owningManual->getNumberOfCouplers()) {
		for (int i = 0; i < nbrCplrs; i++) {
			wxString cplrNbr = wxT("Coupler") + GOODF_functions::number_format(i + 1);
			wxString cplrOnMan = cfg->Read(cplrNbr, wxEmptyString);
			long value = 0;
			if (cplrOnMan.ToLong(&value)) {
				if (value > 0) {
					// the coupler is on
					m_couplers.push_back(std::make_pair(m_owningManual->getCouplerAt(value - 1), true));
				} else {
					// the coupler is off
					m_couplers.push_back(std::make_pair(m_owningManual->getCouplerAt(labs(value) - 1), false));
				}
			}
		}
	}
	int nbrTrems = static_cast<int>(cfg->ReadLong("NumberOfTremulants", 0));
	if (nbrTrems > 0 && nbrTrems <= (int) m_owningManual->getNumberOfTremulants()) {
		for (int i = 0; i < nbrTrems; i++) {
			wxString tremNbr = wxT("Tremulant") + GOODF_functions::number_format(i + 1);
			wxString tremId = cfg->Read(tremNbr, wxEmptyString);
			long value = 0;
			if (tremId.ToLong(&value)) {
				if (labs(value) <= (int) m_owningManual->getNumberOfTremulants()) {
					if (value > 0) {
						// the tremulant is on
						m_tremulants.push_back(std::make_pair(m_owningManual->getTremulantAt(value - 1), true));
					} else {
						// the tremulant is off
						m_tremulants.push_back(std::make_pair(m_owningManual->getTremulantAt(labs(value) - 1), false));
					}
				} else {
					wxLogError("%s value %s is out of range for divisional '%s' since manual '%s' only lists %u tremulants!", tremNbr, tremId, name, m_owningManual->getName(), m_owningManual->getNumberOfTremulants());
					::wxGetApp().m_frame->GetLogWindow()->Show(true);
				}
			}
		}
	}
	int nbrSwitches = static_cast<int>(cfg->ReadLong("NumberOfSwitches", 0));
	if (nbrSwitches > 0 && nbrSwitches <= (int) m_owningManual->getNumberOfGoSwitches()) {
		for (int i = 0; i < nbrSwitches; i++) {
			wxString swNbr = wxT("Switch") + GOODF_functions::number_format(i + 1);
			wxString swId = cfg->Read(swNbr, wxEmptyString);
			long value = 0;
			if (swId.ToLong(&value)) {
				if (labs(value) <= (int) m_owningManual->getNumberOfGoSwitches()) {
					if (value > 0) {
						// the switch is on
						m_switches.push_back(std::make_pair(m_owningManual->getGoSwitchAt(value - 1), true));
					} else {
						// the switch is off
						m_switches.push_back(std::make_pair(m_owningManual->getGoSwitchAt(labs(value) - 1), false));
					}
				} else {
					wxLogError("%s value %s is out of range for divisional '%s' since manual '%s' only lists %u switches!", swNbr, swId, name, m_owningManual->getName(), m_owningManual->getNumberOfGoSwitches());
					::wxGetApp().m_frame->GetLogWindow()->Show(true);
				}
			}
		}
	}
}

bool Divisional::isProtected() {
	return m_protected;
}

void Divisional::setProtected(bool option) {
	m_protected = option;
}

unsigned Divisional::getNumberOfStops() {
	return m_stops.size();
}

void Divisional::addStop(Stop *stop, bool isOn) {
	m_stops.push_back(std::make_pair(stop, isOn));
}

void Divisional::removeStopAt(unsigned index) {
	std::list<std::pair<Stop*, bool>>::iterator it = m_stops.begin();
	std::advance(it, index);
	m_stops.erase(it);
}

void Divisional::removeStop(Stop *stop) {
	auto it = m_stops.begin();
	while (it != m_stops.end()){
	    if((*it).first == stop){
	        it = m_stops.erase(it);// erase and go to next
	    } else{
	        ++it;  // go to next
	    }
	}
}

void Divisional::removeAllStops() {
	m_stops.clear();
}

std::pair<Stop*, bool>* Divisional::getStopPairAt(unsigned index) {
	auto it = std::next(m_stops.begin(), index);
	return &(*it);
}

unsigned Divisional::getNumberOfCouplers() {
	return m_couplers.size();
}

void Divisional::addCoupler(Coupler *coupler, bool isOn) {
	m_couplers.push_back(std::make_pair(coupler, isOn));
}

void Divisional::removeCouplerAt(unsigned index) {
	std::list<std::pair<Coupler*, bool>>::iterator it = m_couplers.begin();
	std::advance(it, index);
	m_couplers.erase(it);
}

void Divisional::removeCoupler(Coupler *coupler) {
	auto it = m_couplers.begin();
	while (it != m_couplers.end()){
	    if((*it).first == coupler){
	        it = m_couplers.erase(it);// erase and go to next
	    } else{
	        ++it;  // go to next
	    }
	}
}

void Divisional::removeAllCouplers() {
	m_couplers.clear();
}

std::pair<Coupler*, bool>* Divisional::getCouplerPairAt(unsigned index) {
	auto it = std::next(m_couplers.begin(), index);
	return &(*it);
}

unsigned Divisional::getNumberOfTremulants() {
	return m_tremulants.size();
}

void Divisional::addTremulant(Tremulant *trem, bool isOn) {
	m_tremulants.push_back(std::make_pair(trem, isOn));
}

void Divisional::removeTremulantAt(unsigned index) {
	std::list<std::pair<Tremulant*, bool>>::iterator it = m_tremulants.begin();
	std::advance(it, index);
	m_tremulants.erase(it);
}

void Divisional::removeTremulant(Tremulant *trem) {
	auto it = m_tremulants.begin();
	while (it != m_tremulants.end()){
	    if((*it).first == trem){
	        it = m_tremulants.erase(it);// erase and go to next
	    } else{
	        ++it;  // go to next
	    }
	}
}

void Divisional::removeAllTremulants() {
	m_tremulants.clear();
}

std::pair<Tremulant*, bool>* Divisional::getTremulantPairAt(unsigned index) {
	auto it = std::next(m_tremulants.begin(), index);
	return &(*it);
}

unsigned Divisional::getNumberOfSwitches() {
	return m_switches.size();
}

void Divisional::addSwitch(GoSwitch *sw, bool isOn) {
	m_switches.push_back(std::make_pair(sw, isOn));
}

void Divisional::removeSwitchAt(unsigned index) {
	std::list<std::pair<GoSwitch*, bool>>::iterator it = m_switches.begin();
	std::advance(it, index);
	m_switches.erase(it);
}

void Divisional::removeSwitch(GoSwitch *sw) {
	unsigned idx = 0;
	for (auto &s : m_switches) {
		if (s.first == sw) {
			removeSwitchAt(idx);
			break;
		}
		idx++;
	}
}

void Divisional::removeAllSwitches() {
	m_switches.clear();
}

std::pair<GoSwitch*, bool>* Divisional::getSwitchPairAt(unsigned index) {
	auto it = std::next(m_switches.begin(), index);
	return &(*it);
}

bool Divisional::hasStop(Stop *stop) {
	bool found = false;
	for (auto& stopPair : m_stops) {
		if (stopPair.first == stop) {
			found = true;
		   	break;
	    }
	}
	return found;
}

bool Divisional::hasCoupler(Coupler *coupler) {
	bool found = false;
	for (auto& couplerPair : m_couplers) {
		if (couplerPair.first == coupler) {
			found = true;
		   	break;
	    }
	}
	return found;
}

bool Divisional::hasTremulant(Tremulant *trem) {
	bool found = false;
	for (auto& tremPair : m_tremulants) {
		if (tremPair.first == trem) {
			found = true;
		   	break;
	    }
	}
	return found;
}

bool Divisional::hasSwitch(GoSwitch *sw) {
	bool found = false;
	for (auto& switchPair : m_switches) {
		if (switchPair.first == sw) {
			found = true;
		   	break;
	    }
	}
	return found;
}

Manual* Divisional::getOwningManual() {
	return m_owningManual;
}

void Divisional::setOwningManual(Manual *manual) {
	m_owningManual = manual;
}
