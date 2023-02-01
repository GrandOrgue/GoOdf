/*
 * Windchestgroup.cpp is part of GOODF.
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

#include "Windchestgroup.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

Windchestgroup::Windchestgroup() {
	name = wxT("New windchest");
}

Windchestgroup::~Windchestgroup() {

}

void Windchestgroup::write(wxTextFile *outFile) {
	outFile->AddLine(wxT("Name=") + name);
	unsigned nbEnc = m_Enclosures.size();
	outFile->AddLine(wxT("NumberOfEnclosures=") + wxString::Format(wxT("%u"), nbEnc));
	unsigned i = 0;
	if (!m_Enclosures.empty()) {
		for (auto& enc : m_Enclosures) {
			i++;
			wxString encNumber = GOODF_functions::number_format(i);
			// unsigned refIndex = ::wxGetApp().m_frame->m_organ->getIndexOfOrganEnclosure(enc);
			wxString encRef = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganEnclosure(enc));
			outFile->AddLine(wxT("Enclosure") + encNumber + wxT("=") + encRef);
		}
	}
	unsigned nbTrem = m_Tremulants.size();
	outFile->AddLine(wxT("NumberOfTremulants=") + wxString::Format(wxT("%u"), nbTrem));
	i = 0;
	if (!m_Tremulants.empty()) {
		for (auto& trem : m_Tremulants) {
			i++;
			wxString tremNumber = GOODF_functions::number_format(i);
			// unsigned refIndex = ::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(trem);
			wxString tremRef = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(trem));
			outFile->AddLine(wxT("Tremulant") + tremNumber + wxT("=") + tremRef);
		}
	}
}

Enclosure* Windchestgroup::getEnclosureAt(unsigned index) {
	auto iterator = std::next(m_Enclosures.begin(), index);
	return (*iterator);
}

unsigned Windchestgroup::getNumberOfEnclosures() {
	return m_Enclosures.size();
}

void Windchestgroup::addEnclosureReference(Enclosure *enclosure) {
	m_Enclosures.push_back(enclosure);
}

void Windchestgroup::removeEnclosureReference(Enclosure *enclosure) {
	m_Enclosures.remove(enclosure);
}

void Windchestgroup::removeEnclosureReferenceAt(unsigned index) {
	std::list<Enclosure *>::iterator it = m_Enclosures.begin();
	std::advance(it, index);
	m_Enclosures.erase(it);
}

bool Windchestgroup::hasEnclosureReference(Enclosure *enclosure) {
	bool found = false;
	for (auto& enc : m_Enclosures) {
		if (enc == enclosure) {
			found = true;
			break;
		}
	}
	return found;
}

Tremulant* Windchestgroup::getTremulantAt(unsigned index) {
	auto iterator = std::next(m_Tremulants.begin(), index);
	return (*iterator);
}


unsigned Windchestgroup::getNumberOfTremulants() {
	return m_Tremulants.size();
}

void Windchestgroup::addTremulantReference(Tremulant *tremulant) {
	m_Tremulants.push_back(tremulant);
}

void Windchestgroup::removeTremulantReference(Tremulant *tremulant) {
	m_Tremulants.remove(tremulant);
}

void Windchestgroup::removeTremulantReferenceAt(unsigned index) {
	std::list<Tremulant *>::iterator it = m_Tremulants.begin();
	std::advance(it, index);
	m_Tremulants.erase(it);
}

bool Windchestgroup::hasTremulantReference(Tremulant *tremulant) {
	bool found = false;
	for (auto& trem : m_Tremulants) {
		if (trem == tremulant) {
			found = true;
			break;
		}
	}
	return found;
}

wxString Windchestgroup::getName() {
	return name;
}

void Windchestgroup::setName(wxString name) {
	this->name = name;
}
