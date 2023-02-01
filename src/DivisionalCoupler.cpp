/*
 * DivisionalCoupler.cpp is part of GOODF.
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

#include "DivisionalCoupler.h"
#include "GOODF.h"
#include "GOODFFunctions.h"

DivisionalCoupler::DivisionalCoupler() : Drawstop() {
	name = wxT("New Divisional Coupler");
	storeInDivisional = false;
	m_biDirectionalCoupling = false;
}

DivisionalCoupler::~DivisionalCoupler() {

}

void DivisionalCoupler::write(wxTextFile *outFile) {
	Drawstop::write(outFile);
	if (m_biDirectionalCoupling)
		outFile->AddLine(wxT("BiDirectionalCoupling=Y"));
	else
		outFile->AddLine(wxT("BiDirectionalCoupling=N"));
	outFile->AddLine(wxT("NumberOfManuals=") + wxString::Format(wxT("%u"), getNumberOfManuals()));
	unsigned counter = 1;
	for (Manual* m : m_affectedManuals) {
		wxString manId = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m));
		outFile->AddLine(wxT("Manual") + GOODF_functions::number_format(counter) + wxT("=") + manId);
		counter++;
	}
}

bool DivisionalCoupler::hasBiDirectionalCoupling() {
	return m_biDirectionalCoupling;
}

void DivisionalCoupler::setBiDirectionalCoupling(bool isBiDirectional) {
	m_biDirectionalCoupling = isBiDirectional;
}

void DivisionalCoupler::addAffectedManual(Manual *manual) {
	m_affectedManuals.push_back(manual);
}

unsigned DivisionalCoupler::getNumberOfManuals() {
	return m_affectedManuals.size();
}

Manual* DivisionalCoupler::getManualAt(unsigned index) {
	auto iterator = std::next(m_affectedManuals.begin(), index);
	return (*iterator);
}

void DivisionalCoupler::removeManualAt(unsigned index) {
	std::list<Manual*>::iterator it = m_affectedManuals.begin();
	std::advance(it, index);
	m_affectedManuals.erase(it);
}

bool DivisionalCoupler::hasManualReference(Manual *manual) {
	bool found = false;
	for (auto& a_man : m_affectedManuals) {
		if (a_man == manual) {
			found = true;
		   	break;
	    }
	}
	return found;
}

void DivisionalCoupler::removeManualReference(Manual *manual) {
	m_affectedManuals.remove(manual);
}
