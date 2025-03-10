/*
 * Coupler.cpp is part of GoOdf.
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

#include "Coupler.h"
#include "GOODF.h"
#include "GOODFFunctions.h"

Coupler::Coupler() : Drawstop() {
	name = wxT("New Coupler");
	m_unisonOff = false;
	m_owningManual = NULL;
	m_destinationManual = NULL;
	m_destinationKeyshift = 0;
	m_coupleToSubsequentUnisonIntermanualCouplers = false;
	m_coupleToSubsequentUpwardIntermanualCouplers = false;
	m_coupleToSubsequentDownwardIntermanualCouplers = false;
	m_coupleToSubsequentUpwardIntramanualCouplers = false;
	m_coupleToSubsequentDownwardIntramanualCouplers = false;
	m_couplerType = wxT("Normal");
	m_firstMIDINoteNumber = 0;
	m_numberOfKeys = 127;
}

Coupler::~Coupler() {

}

void Coupler::write(wxTextFile *outFile) {
	Drawstop::write(outFile);
	if (m_unisonOff) {
		outFile->AddLine(wxT("UnisonOff=Y"));
	} else {
		outFile->AddLine(wxT("UnisonOff=N"));
		wxString manId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_destinationManual));
		outFile->AddLine(wxT("DestinationManual=") + manId);
		outFile->AddLine(wxT("DestinationKeyshift=") + wxString::Format(wxT("%i"), m_destinationKeyshift));
		if (m_couplerType.IsSameAs(wxT("Normal"))) {
			if (m_coupleToSubsequentUnisonIntermanualCouplers)
				outFile->AddLine(wxT("CoupleToSubsequentUnisonIntermanualCouplers=Y"));
			else
				outFile->AddLine(wxT("CoupleToSubsequentUnisonIntermanualCouplers=N"));
			if (m_coupleToSubsequentUpwardIntermanualCouplers)
				outFile->AddLine(wxT("CoupleToSubsequentUpwardIntermanualCouplers=Y"));
			else
				outFile->AddLine(wxT("CoupleToSubsequentUpwardIntermanualCouplers=N"));
			if (m_coupleToSubsequentDownwardIntermanualCouplers)
				outFile->AddLine(wxT("CoupleToSubsequentDownwardIntermanualCouplers=Y"));
			else
				outFile->AddLine(wxT("CoupleToSubsequentDownwardIntermanualCouplers=N"));
			if (m_coupleToSubsequentUpwardIntramanualCouplers)
				outFile->AddLine(wxT("CoupleToSubsequentUpwardIntramanualCouplers=Y"));
			else
				outFile->AddLine(wxT("CoupleToSubsequentUpwardIntramanualCouplers=N"));
			if (m_coupleToSubsequentDownwardIntramanualCouplers)
				outFile->AddLine(wxT("CoupleToSubsequentDownwardIntramanualCouplers=Y"));
			else
				outFile->AddLine(wxT("CoupleToSubsequentDownwardIntramanualCouplers=N"));
		}
		if (!m_couplerType.IsSameAs(wxT("Normal")))
			outFile->AddLine(wxT("CouplerType=") + m_couplerType);
	}
	if (m_firstMIDINoteNumber != 0)
		outFile->AddLine(wxT("FirstMIDINoteNumber=") + wxString::Format(wxT("%i"), m_firstMIDINoteNumber));
	if (m_numberOfKeys != 127)
		outFile->AddLine(wxT("NumberOfKeys=") + wxString::Format(wxT("%i"), m_numberOfKeys));
}

void Coupler::read(wxFileConfig *cfg, bool usingOldPanelFormat, Manual *owning_manual, Organ *readOrgan) {
	m_owningManual = owning_manual;
	Drawstop::read(cfg, usingOldPanelFormat, readOrgan);
	wxString cfgBoolValue = cfg->Read("UnisonOff", wxEmptyString);
	m_unisonOff = GOODF_functions::parseBoolean(cfgBoolValue, false);
	int destMan = static_cast<int>(cfg->ReadLong("DestinationManual", -1));
	if (destMan >= 0 && destMan <= (int) readOrgan->getNumberOfManuals()) {
		if (!readOrgan->doesHavePedals())
			destMan -= 1;
		m_destinationManual = readOrgan->getOrganManualAt(destMan);
	}
	int destShift = static_cast<int>(cfg->ReadLong("DestinationKeyshift", 0));
	if (destShift > -25 && destShift < 25) {
		m_destinationKeyshift = destShift;
	}
	cfgBoolValue = cfg->Read("CoupleToSubsequentUnisonIntermanualCouplers", wxEmptyString);
	m_coupleToSubsequentUnisonIntermanualCouplers = GOODF_functions::parseBoolean(cfgBoolValue, false);
	cfgBoolValue = cfg->Read("CoupleToSubsequentUpwardIntermanualCouplers", wxEmptyString);
	m_coupleToSubsequentUpwardIntermanualCouplers = GOODF_functions::parseBoolean(cfgBoolValue, false);
	cfgBoolValue = cfg->Read("CoupleToSubsequentDownwardIntermanualCouplers", wxEmptyString);
	m_coupleToSubsequentDownwardIntermanualCouplers = GOODF_functions::parseBoolean(cfgBoolValue, false);
	cfgBoolValue = cfg->Read("CoupleToSubsequentUpwardIntramanualCouplers", wxEmptyString);
	m_coupleToSubsequentUpwardIntramanualCouplers = GOODF_functions::parseBoolean(cfgBoolValue, false);
	cfgBoolValue = cfg->Read("CoupleToSubsequentDownwardIntramanualCouplers", wxEmptyString);
	m_coupleToSubsequentDownwardIntramanualCouplers = GOODF_functions::parseBoolean(cfgBoolValue, false);
	wxString cplrType = cfg->Read("CouplerType", wxT("Normal"));
	if (cplrType.IsSameAs(wxT("Normal"), false) || cplrType.IsSameAs(wxT("Bass"), false) || cplrType.IsSameAs(wxT("Melody"), false)) {
		m_couplerType = cplrType;
	}
	int firstMidiNote = static_cast<int>(cfg->ReadLong("FirstMIDINoteNumber", 0));
	if (firstMidiNote > -1 && firstMidiNote < 128) {
		m_firstMIDINoteNumber = firstMidiNote;
	}
	int nbrKeys = static_cast<int>(cfg->ReadLong("NumberOfKeys", 127));
	if (nbrKeys > -1 && nbrKeys < 128) {
		m_numberOfKeys = nbrKeys;
	}
}

wxString Coupler::getCouplerType() {
	return m_couplerType;
}

void Coupler::setCouplerType(wxString couplerType) {
	m_couplerType = couplerType;
}

bool Coupler::isCoupleToSubsequentDownwardIntermanualCouplers() {
	return m_coupleToSubsequentDownwardIntermanualCouplers;
}

void Coupler::setCoupleToSubsequentDownwardIntermanualCouplers(bool option) {
	m_coupleToSubsequentDownwardIntermanualCouplers = option;
}

bool Coupler::isCoupleToSubsequentDownwardIntramanualCouplers() {
	return m_coupleToSubsequentDownwardIntramanualCouplers;
}

void Coupler::setCoupleToSubsequentDownwardIntramanualCouplers(bool option) {
	m_coupleToSubsequentDownwardIntramanualCouplers = option;
}

bool Coupler::isCoupleToSubsequentUnisonIntermanualCouplers() {
	return m_coupleToSubsequentUnisonIntermanualCouplers;
}

void Coupler::setCoupleToSubsequentUnisonIntermanualCouplers(bool option) {
	m_coupleToSubsequentUnisonIntermanualCouplers = option;
}

bool Coupler::isCoupleToSubsequentUpwardIntermanualCouplers() {
	return m_coupleToSubsequentUpwardIntermanualCouplers;
}

void Coupler::setCoupleToSubsequentUpwardIntermanualCouplers(bool option) {
	m_coupleToSubsequentUpwardIntermanualCouplers = option;
}

bool Coupler::isCoupleToSubsequentUpwardIntramanualCouplers() {
	return m_coupleToSubsequentUpwardIntramanualCouplers;
}

void Coupler::setCoupleToSubsequentUpwardIntramanualCouplers(bool option) {
	m_coupleToSubsequentUpwardIntramanualCouplers = option;
}

int Coupler::getDestinationKeyshift() {
	return m_destinationKeyshift;
}

void Coupler::setDestinationKeyshift(int destinationKeyshift) {
	m_destinationKeyshift = destinationKeyshift;
}

Manual* Coupler::getDestinationManual() {
	return m_destinationManual;
}

void Coupler::setDestinationManual(Manual *destinationManual) {
	m_destinationManual = destinationManual;
}

int Coupler::getFirstMidiNoteNumber() {
	return m_firstMIDINoteNumber;
}

void Coupler::setFirstMidiNoteNumber(int firstMidiNoteNumber) {
	m_firstMIDINoteNumber = firstMidiNoteNumber;
}

int Coupler::getNumberOfKeys() {
	return m_numberOfKeys;
}

void Coupler::setNumberOfKeys(int numberOfKeys) {
	m_numberOfKeys = numberOfKeys;
}

Manual* Coupler::getOwningManual() {
	return m_owningManual;
}

void Coupler::setOwningManual(Manual *owningManual) {
	m_owningManual = owningManual;
}

bool Coupler::isUnisonOff() {
	return m_unisonOff;
}

void Coupler::setUnisonOff(bool option) {
	m_unisonOff = option;
}
