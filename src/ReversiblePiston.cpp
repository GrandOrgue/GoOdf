/*
 * ReversiblePiston.cpp is part of GOODF.
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
		outFile->AddLine(wxT("ObjecType=STOP"));
		wxString manId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_stop->getOwningManual()));
		outFile->AddLine(wxT("ManualNumber=") + manId);
		wxString objId = GOODF_functions::number_format(m_stop->getOwningManual()->getIndexOfStop(m_stop));
		outFile->AddLine(wxT("ObjectNumber=") + objId);
	} else if (m_coupler) {
		outFile->AddLine(wxT("ObjecType=COUPLER"));
		wxString manId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_coupler->getOwningManual()));
		outFile->AddLine(wxT("ManualNumber=") + manId);
		wxString objId = GOODF_functions::number_format(m_coupler->getOwningManual()->getIndexOfStop(m_stop));
		outFile->AddLine(wxT("ObjectNumber=") + objId);
	} else if (m_switch) {
		outFile->AddLine(wxT("ObjecType=SWITCH"));
		wxString objId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(m_switch));
		outFile->AddLine(wxT("ObjectNumber=") + objId);
	} else if (m_tremulant) {
		outFile->AddLine(wxT("ObjecType=TREMULANT"));
		wxString objId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(m_tremulant));
		outFile->AddLine(wxT("ObjectNumber=") + objId);
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
