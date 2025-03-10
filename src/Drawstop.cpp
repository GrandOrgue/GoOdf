/*
 * Drawstop.cpp is part of GoOdf.
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

#include "Drawstop.h"
#include "GOODFFunctions.h"
#include "GOODF.h"
#include "GoSwitch.h"

Drawstop::Drawstop() : Button() {
	function = wxT("Input");
	defaultToEngaged = false;
	gcState = 0;
	storeInDivisional = true;
	storeInGeneral = true;
}

Drawstop::~Drawstop() {

}

void Drawstop::write(wxTextFile *outFile) {
	Button::write(outFile);
	if (!function.IsSameAs(wxT("Input")) && !m_switches.empty()) {
		if (function.IsSameAs(wxT("Not"))) {
			// Only the first switch is relevant
			outFile->AddLine(wxT("Function=") + function);
			unsigned refIndex = ::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(m_switches.front());
			wxString formattedSwitch = GOODF_functions::number_format(refIndex);
			outFile->AddLine(wxT("Switch001=") + formattedSwitch);
		} else {
			// The object can have multiple switches
			outFile->AddLine(wxT("Function=") + function);
			unsigned nbSwitches = m_switches.size();
			outFile->AddLine(wxT("SwitchCount=") + wxString::Format(wxT("%u"), nbSwitches));
			unsigned k = 0;
			for (auto& sw : m_switches) {
				k++;
				unsigned refIndex = ::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(sw);
				wxString formattedNumber = GOODF_functions::number_format(k);
				wxString formattedSwitch = GOODF_functions::number_format(refIndex);
				outFile->AddLine(wxT("Switch") + formattedNumber + wxT("=") + formattedSwitch);
			}
		}
	} else {
		// Issue a warning if function is set to something else than Input and referenced switches is empty
		if (!function.IsSameAs(wxT("Input")) && m_switches.empty()) {
			wxLogWarning("%s has function %s and should reference some switch(es) but doesn't! The function value will thus not be written to file!", getName(), getFunction());
			::wxGetApp().m_frame->GetLogWindow()->Show(true);
		}
		if (defaultToEngaged)
			outFile->AddLine(wxT("DefaultToEngaged=Y"));
		else
			outFile->AddLine(wxT("DefaultToEngaged=N"));
	}
	if (gcState && function.IsSameAs(wxT("Input")))
		outFile->AddLine(wxT("GCState=") + wxString::Format(wxT("%i"), gcState));
	if (!storeInDivisional)
		outFile->AddLine(wxT("StoreInDivisional=N"));
	if (!storeInGeneral)
		outFile->AddLine(wxT("StoreInGeneral=N"));
}

void Drawstop::read(wxFileConfig *cfg, bool usingOldPanelFormat, Organ *readOrgan) {
	Button::read(cfg, usingOldPanelFormat);
	function = cfg->Read("Function", wxT("Input"));
	if (!function.IsSameAs(wxT("Input"), false)) {
		int nbrReferencedSwitches = static_cast<int>(cfg->ReadLong("SwitchCount", 0));
		// For a NOT switch only one input can exist and it's not necessary to specify the SwitchCount
		// but for reading we must set the count to one
		if (function.IsSameAs(wxT("Not"), false))
			nbrReferencedSwitches = 1;
		// handle switch references
		for (int i = 0; i < nbrReferencedSwitches; i++) {
			wxString swNbrId = wxT("Switch") + GOODF_functions::number_format(i + 1);
			int swRefNbr = static_cast<int>(cfg->ReadLong(swNbrId, 0));
			// if the number of the referenced switch is lower than the number of switches in the organ it's ok
			if (swRefNbr > 0 && swRefNbr <= (int) readOrgan->getNumberOfSwitches()) {
				// but the index of the switch is actually one lower than in the organ file
				// also check that it's not already added
				if (!hasSwitchReference(readOrgan->getOrganSwitchAt(swRefNbr - 1)))
					addSwitchReference(readOrgan->getOrganSwitchAt(swRefNbr - 1));
				else {
					wxLogWarning("Switch%0.3d (%s) is already added to %s! This additional switch entry will be ignored!", swRefNbr, readOrgan->getOrganSwitchAt(swRefNbr - 1)->getName(), getName());
					::wxGetApp().m_frame->GetLogWindow()->Show(true);
				}
			}
		}
	}
	wxString cfgBoolValue = cfg->Read("DefaultToEngaged", wxEmptyString);
	defaultToEngaged = GOODF_functions::parseBoolean(cfgBoolValue, false);
	int gc_state = static_cast<int>(cfg->ReadLong("GCState", 0));
	if (gc_state > -2 && gc_state < 2)
		gcState = gc_state;
	cfgBoolValue = cfg->Read("StoreInDivisional", wxEmptyString);
	storeInDivisional = GOODF_functions::parseBoolean(cfgBoolValue, true);
	cfgBoolValue = cfg->Read("StoreInGeneral", wxEmptyString);
	storeInGeneral = GOODF_functions::parseBoolean(cfgBoolValue, true);
}

bool Drawstop::isDefaultToEngaged() {
	return defaultToEngaged;
}

void Drawstop::setDefaultToEngaged(bool defaultToEngaged) {
	this->defaultToEngaged = defaultToEngaged;
}

wxString Drawstop::getFunction() {
	return function;
}

void Drawstop::setFunction(wxString function) {
	this->function = function;
}

int Drawstop::getGcState() {
	return gcState;
}

void Drawstop::setGcState(int gcState) {
	this->gcState = gcState;
}

unsigned Drawstop::getNumberOfSwitches() {
	return m_switches.size();
}

GoSwitch* Drawstop::getSwitchAtIndex(unsigned index) {
	auto iterator = std::next(m_switches.begin(), index);
	return (*iterator);
}

void Drawstop::addSwitchReference(GoSwitch *switchToAdd) {
	m_switches.push_back(switchToAdd);
}

unsigned Drawstop::getIndexOfSwitch(GoSwitch *switchToFind) {
	unsigned i = 0;
	bool found = false;
	for (auto& sw : m_switches) {
		i++;
		if (sw == switchToFind) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Drawstop::removeSwitchReference(GoSwitch *sw) {
	m_switches.remove(sw);
}

void Drawstop::removeSwitchReferenceAt(unsigned index) {
	std::list<GoSwitch *>::iterator it = m_switches.begin();
	std::advance(it, index);
	m_switches.erase(it);
}

void Drawstop::removeAllSwitchReferences() {
	m_switches.clear();
}

bool Drawstop::hasSwitchReference(GoSwitch *sw) {
	bool found = false;
	for (auto& a_switch : m_switches) {
		if (a_switch == sw) {
			found = true;
		   	break;
	    }
	}
	return found;
}

bool Drawstop::isStoreInDivisional() {
	return storeInDivisional;
}

void Drawstop::setStoreInDivisional(bool storeInDivisional) {
	this->storeInDivisional = storeInDivisional;
}

bool Drawstop::isStoreInGeneral() {
	return storeInGeneral;
}

void Drawstop::setStoreInGeneral(bool storeInGeneral) {
	this->storeInGeneral = storeInGeneral;
}
