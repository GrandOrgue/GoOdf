/*
 * ReversiblePiston.cpp is part of GoOdf.
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

#include "ReversiblePiston.h"
#include "GOODF.h"
#include "GOODFFunctions.h"

ReversiblePiston::ReversiblePiston() : Button() {
	m_stop = NULL;
	m_coupler = NULL;
	m_switch = NULL;
	m_tremulant = NULL;
	name = "New Reversible Piston";
}

ReversiblePiston::~ReversiblePiston() {

}

void ReversiblePiston::write(wxTextFile *outFile) {
	Button::write(outFile);
	if (m_stop) {
		outFile->AddLine(wxT("ObjectType=STOP"));
		wxString manId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_stop->getOwningManual()));
		outFile->AddLine(wxT("ManualNumber=") + manId);
		wxString objId = GOODF_functions::number_format(m_stop->getOwningManual()->getIndexOfStop(m_stop) + 1);
		outFile->AddLine(wxT("ObjectNumber=") + objId);
	} else if (m_coupler) {
		outFile->AddLine(wxT("ObjectType=COUPLER"));
		wxString manId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_coupler->getOwningManual()));
		outFile->AddLine(wxT("ManualNumber=") + manId);
		wxString objId = GOODF_functions::number_format(m_coupler->getOwningManual()->getIndexOfCoupler(m_coupler) + 1);
		outFile->AddLine(wxT("ObjectNumber=") + objId);
	} else if (m_switch) {
		outFile->AddLine(wxT("ObjectType=SWITCH"));
		wxString objId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(m_switch) + 1);
		outFile->AddLine(wxT("ObjectNumber=") + objId);
	} else if (m_tremulant) {
		outFile->AddLine(wxT("ObjectType=TREMULANT"));
		wxString objId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(m_tremulant) + 1);
		outFile->AddLine(wxT("ObjectNumber=") + objId);
	}
}

void ReversiblePiston::read(wxFileConfig *cfg, bool usingOldPanelFormat, Organ *readOrgan) {
	Button::read(cfg, usingOldPanelFormat);
	wxString type = cfg->Read("ObjectType", wxEmptyString);
	if (type.IsSameAs(wxT("STOP"), false)) {
		int manNbr = static_cast<int>(cfg->ReadLong("ManualNumber", -1));
		if (manNbr >= 0 && manNbr <= (int) readOrgan->getNumberOfManuals()) {
			if (!readOrgan->doesHavePedals())
				manNbr -= 1;
			int stopNbr = static_cast<int>(cfg->ReadLong("ObjectNumber", 0));
			if (stopNbr > 0 && stopNbr <= (int) readOrgan->getOrganManualAt(manNbr)->getNumberOfStops()) {
				setStop(readOrgan->getOrganManualAt(manNbr)->getStopAt(stopNbr - 1));
			}
		}
	} else if (type.IsSameAs(wxT("COUPLER"), false)) {
		int manNbr = static_cast<int>(cfg->ReadLong("ManualNumber", -1));
		if (manNbr >= 0 && manNbr <= (int) readOrgan->getNumberOfManuals()) {
			if (!readOrgan->doesHavePedals())
				manNbr -= 1;
			int couplerNbr = static_cast<int>(cfg->ReadLong("ObjectNumber", 0));
			if (couplerNbr > 0 && couplerNbr <= (int) readOrgan->getOrganManualAt(manNbr)->getNumberOfCouplers()) {
				setCoupler(readOrgan->getOrganManualAt(manNbr)->getCouplerAt(couplerNbr - 1));
			}
		}
	} else if (type.IsSameAs(wxT("SWITCH"), false)) {
		int switchNbr = static_cast<int>(cfg->ReadLong("ObjectNumber", 0));
		if (switchNbr > 0 && switchNbr <= (int) readOrgan->getNumberOfSwitches()) {
			setSwitch(readOrgan->getOrganSwitchAt(switchNbr - 1));
		}
	} else if (type.IsSameAs(wxT("TREMULANT"), false)) {
		int tremNbr = static_cast<int>(cfg->ReadLong("ObjectNumber", 0));
		if (tremNbr > 0 && tremNbr <= (int) readOrgan->getNumberOfTremulants()) {
			setTremulant(readOrgan->getOrganTremulantAt(tremNbr - 1));
		}
	}
}

Stop* ReversiblePiston::getStop() {
	return m_stop;
}

void ReversiblePiston::setStop(Stop* stop) {
	m_stop = stop;
	if (m_coupler)
		m_coupler = NULL;
	if (m_switch)
		m_switch = NULL;
	if (m_tremulant)
		m_tremulant = NULL;
}

Coupler* ReversiblePiston::getCoupler() {
	return m_coupler;
}

void ReversiblePiston::setCoupler(Coupler* coupler) {
	m_coupler = coupler;
	if (m_stop)
		m_stop = NULL;
	if (m_switch)
		m_switch = NULL;
	if (m_tremulant)
		m_tremulant = NULL;
}

GoSwitch* ReversiblePiston::getSwitch() {
	return m_switch;
}

void ReversiblePiston::setSwitch(GoSwitch* sw) {
	m_switch = sw;
	if (m_stop)
		m_stop = NULL;
	if (m_coupler)
		m_coupler = NULL;
	if (m_tremulant)
		m_tremulant = NULL;
}

Tremulant* ReversiblePiston::getTremulant() {
	return m_tremulant;
}

void ReversiblePiston::setTremulant(Tremulant* tremulant) {
	m_tremulant = tremulant;
	if (m_stop)
		m_stop = NULL;
	if (m_coupler)
		m_coupler = NULL;
	if (m_switch)
		m_switch = NULL;
}

wxString ReversiblePiston::getObjecType() {
	if (m_stop)
		return wxT("STOP");
	else if (m_coupler)
		return wxT("COUPLER");
	else if (m_switch)
		return wxT("SWITCH");
	else if (m_tremulant)
		return wxT("TREMULANT");
	else
		return wxEmptyString;
}

wxString ReversiblePiston::getManual() {
	if (m_stop)
		return m_stop->getOwningManual()->getName();
	else if (m_coupler)
		return m_coupler->getOwningManual()->getName();
	else
		return wxEmptyString;
}
