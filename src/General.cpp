/*
 * General.cpp is part of GOODF.
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

#include "General.h"
#include "GOODF.h"
#include "GOODFFunctions.h"

General::General() : Button() {
	name = wxT("New General");
	m_protected = false;
}

General::~General() {

}

void General::write(wxTextFile *outFile) {
	outFile->AddLine(wxT("Name=") + name);
	if (m_protected)
		outFile->AddLine(wxT("Protected=Y"));
	unsigned nbStops = getNumberOfStops();
	outFile->AddLine(wxT("NumberOfStops=") + wxString::Format(wxT("%u"), nbStops));
	unsigned counter = 1;
	for (std::pair<Stop*, bool> stop : m_stops) {
		// The returned index of organ manual is already adjusted so that the pedal always get index 0 and a manual
		// will start at index 1 in the odf, this must be taken into account when looking for item
		unsigned manId = ::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(stop.first->getOwningManual());
		unsigned actualManualIndex = manId;
		if (!(::wxGetApp().m_frame->m_organ->doesHavePedals()))
			actualManualIndex -= 1;

		int stopIdx = ::wxGetApp().m_frame->m_organ->getOrganManualAt(actualManualIndex)->getIndexOfStop(stop.first) + 1;
		wxString stopId = GOODF_functions::number_format(stopIdx);
		if (stop.second)
			outFile->AddLine(wxT("StopNumber") + GOODF_functions::number_format(counter) + wxT("=") + stopId);
		else
			outFile->AddLine(wxT("StopNumber") + GOODF_functions::number_format(counter) + wxT("=-") + stopId);

		wxString manualNbr = GOODF_functions::number_format(manId);
		outFile->AddLine(wxT("StopManual") + GOODF_functions::number_format(counter) + wxT("=") + manualNbr);

		counter++;
	}
	unsigned nbCouplers = getNumberOfCouplers();
	outFile->AddLine(wxT("NumberOfCouplers=") + wxString::Format(wxT("%u"), nbCouplers));
	counter = 1;
	for (std::pair<Coupler*, bool> coupler : m_couplers) {
		unsigned manId = ::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(coupler.first->getOwningManual());
		unsigned actualManualIndex = manId;
		if (!(::wxGetApp().m_frame->m_organ->doesHavePedals()))
			actualManualIndex -= 1;

		int couplerIdx = ::wxGetApp().m_frame->m_organ->getOrganManualAt(actualManualIndex)->getIndexOfCoupler(coupler.first) + 1;
		wxString couplerId = GOODF_functions::number_format(couplerIdx);
		if (coupler.second)
			outFile->AddLine(wxT("CouplerNumber") + GOODF_functions::number_format(counter) + wxT("=") + couplerId);
		else
			outFile->AddLine(wxT("CouplerNumber") + GOODF_functions::number_format(counter) + wxT("=-") + couplerId);

		wxString manualNbr = GOODF_functions::number_format(manId);
		outFile->AddLine(wxT("CouplerManual") + GOODF_functions::number_format(counter) + wxT("=") + manualNbr);

		counter++;
	}
	unsigned nbTrems = getNumberOfTremulants();
	outFile->AddLine(wxT("NumberOfTremulants=") + wxString::Format(wxT("%u"), nbTrems));
	counter = 1;
	for (std::pair<Tremulant*, bool> trem : m_tremulants) {
		wxString tremId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(trem.first));
		if (trem.second)
			outFile->AddLine(wxT("TremulantNumber") + GOODF_functions::number_format(counter) + wxT("=") + tremId);
		else
			outFile->AddLine(wxT("TremulantNumber") + GOODF_functions::number_format(counter) + wxT("=-") + tremId);

		counter++;
	}
	unsigned nbSwitches = getNumberOfSwitches();
	outFile->AddLine(wxT("NumberOfSwitches=") + wxString::Format(wxT("%u"), nbSwitches));
	counter = 1;
	for (std::pair<GoSwitch*, bool> sw : m_switches) {
		wxString switchId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(sw.first));
		if (sw.second)
			outFile->AddLine(wxT("SwitchNumber") + GOODF_functions::number_format(counter) + wxT("=") + switchId);
		else
			outFile->AddLine(wxT("SwitchNumber") + GOODF_functions::number_format(counter) + wxT("=-") + switchId);

		counter++;
	}
	unsigned nbDivCplrs = getNumberOfDivisionalCouplers();
	outFile->AddLine(wxT("NumberOfDivisionalCouplers=") + wxString::Format(wxT("%u"), nbDivCplrs));
	counter = 1;
	for (std::pair<DivisionalCoupler*, bool> divCplr : m_divisionalCouplers) {
		wxString divCplrId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganDivisionalCoupler(divCplr.first));
		if (divCplr.second)
			outFile->AddLine(wxT("DivisionalCouplerNumber") + GOODF_functions::number_format(counter) + wxT("=") + divCplrId);
		else
			outFile->AddLine(wxT("DivisionalCouplerNumber") + GOODF_functions::number_format(counter) + wxT("=-") + divCplrId);

		counter++;
	}
}

bool General::isProtected() {
	return m_protected;
}

void General::setProtected(bool option) {
	m_protected = option;
}

unsigned General::getNumberOfStops() {
	return m_stops.size();
}

void General::addStop(Stop *stop, bool isOn) {
	m_stops.push_back(std::make_pair(stop, isOn));
}

void General::removeStopAt(unsigned index) {
	std::list<std::pair<Stop*, bool>>::iterator it = m_stops.begin();
	std::advance(it, index);
	m_stops.erase(it);
}

void General::removeStop(Stop *stop) {
	auto it = m_stops.begin();
	while (it != m_stops.end()){
	    if((*it).first == stop){
	        it = m_stops.erase(it);// erase and go to next
	    } else{
	        ++it;  // go to next
	    }
	}
}

void General::removeAllStops() {
	m_stops.clear();
}

std::pair<Stop*, bool>* General::getStopPairAt(unsigned index) {
	auto it = std::next(m_stops.begin(), index);
	return &(*it);
}

unsigned General::getNumberOfCouplers() {
	return m_couplers.size();
}

void General::addCoupler(Coupler *coupler, bool isOn) {
	m_couplers.push_back(std::make_pair(coupler, isOn));
}

void General::removeCouplerAt(unsigned index) {
	std::list<std::pair<Coupler*, bool>>::iterator it = m_couplers.begin();
	std::advance(it, index);
	m_couplers.erase(it);
}

void General::removeCoupler(Coupler *coupler) {
	auto it = m_couplers.begin();
	while (it != m_couplers.end()){
	    if((*it).first == coupler){
	        it = m_couplers.erase(it);// erase and go to next
	    } else{
	        ++it;  // go to next
	    }
	}
}

void General::removeAllCouplers() {
	m_couplers.clear();
}

std::pair<Coupler*, bool>* General::getCouplerPairAt(unsigned index) {
	auto it = std::next(m_couplers.begin(), index);
	return &(*it);
}

unsigned General::getNumberOfTremulants() {
	return m_tremulants.size();
}

void General::addTremulant(Tremulant *trem, bool isOn) {
	m_tremulants.push_back(std::make_pair(trem, isOn));
}

void General::removeTremulantAt(unsigned index) {
	std::list<std::pair<Tremulant*, bool>>::iterator it = m_tremulants.begin();
	std::advance(it, index);
	m_tremulants.erase(it);
}

void General::removeTremulant(Tremulant *trem) {
	auto it = m_tremulants.begin();
	while (it != m_tremulants.end()){
	    if((*it).first == trem){
	        it = m_tremulants.erase(it);// erase and go to next
	    } else{
	        ++it;  // go to next
	    }
	}
}

void General::removeAllTremulants() {
	m_tremulants.clear();
}

std::pair<Tremulant*, bool>* General::getTremulantPairAt(unsigned index) {
	auto it = std::next(m_tremulants.begin(), index);
	return &(*it);
}

unsigned General::getNumberOfSwitches() {
	return m_switches.size();
}

void General::addSwitch(GoSwitch *sw, bool isOn) {
	m_switches.push_back(std::make_pair(sw, isOn));
}

void General::removeSwitchAt(unsigned index) {
	std::list<std::pair<GoSwitch*, bool>>::iterator it = m_switches.begin();
	std::advance(it, index);
	m_switches.erase(it);
}

void General::removeSwitch(GoSwitch *sw) {
	auto it = m_switches.begin();
	while (it != m_switches.end()){
	    if((*it).first == sw){
	        it = m_switches.erase(it);// erase and go to next
	    } else{
	        ++it;  // go to next
	    }
	}
}

void General::removeAllSwitches() {
	m_switches.clear();
}

std::pair<GoSwitch*, bool>* General::getSwitchPairAt(unsigned index) {
	auto it = std::next(m_switches.begin(), index);
	return &(*it);
}

unsigned General::getNumberOfDivisionalCouplers() {
	return m_divisionalCouplers.size();
}

void General::addDivisionalCoupler(DivisionalCoupler *divCoupler, bool isOn) {
	m_divisionalCouplers.push_back(std::make_pair(divCoupler, isOn));
}

void General::removeDivisionalCouplerAt(unsigned index) {
	std::list<std::pair<DivisionalCoupler*, bool>>::iterator it = m_divisionalCouplers.begin();
	std::advance(it, index);
	m_divisionalCouplers.erase(it);
}

void General::removeDivisionalCoupler(DivisionalCoupler *divCplr) {
	auto it = m_divisionalCouplers.begin();
	while (it != m_divisionalCouplers.end()){
	    if((*it).first == divCplr){
	        it = m_divisionalCouplers.erase(it);// erase and go to next
	    } else{
	        ++it;  // go to next
	    }
	}
}

void General::removeAllDivisionalCouplers() {
	m_divisionalCouplers.clear();
}

std::pair<DivisionalCoupler*, bool>* General::getDivisionalCouplerPairAt(unsigned index) {
	auto it = std::next(m_divisionalCouplers.begin(), index);
	return &(*it);
}

bool General::hasStop(Stop *stop) {
	bool found = false;
	for (auto& stopPair : m_stops) {
		if (stopPair.first == stop) {
			found = true;
		   	break;
	    }
	}
	return found;
}

bool General::hasCoupler(Coupler *coupler) {
	bool found = false;
	for (auto& couplerPair : m_couplers) {
		if (couplerPair.first == coupler) {
			found = true;
		   	break;
	    }
	}
	return found;
}

bool General::hasTremulant(Tremulant *trem) {
	bool found = false;
	for (auto& tremPair : m_tremulants) {
		if (tremPair.first == trem) {
			found = true;
		   	break;
	    }
	}
	return found;
}

bool General::hasSwitch(GoSwitch *sw) {
	bool found = false;
	for (auto& switchPair : m_switches) {
		if (switchPair.first == sw) {
			found = true;
		   	break;
	    }
	}
	return found;
}

bool General::hasDivisionalCoupler(DivisionalCoupler *divCoupler) {
	bool found = false;
	for (auto& divCplrPair : m_divisionalCouplers) {
		if (divCplrPair.first == divCoupler) {
			found = true;
		   	break;
	    }
	}
	return found;
}
