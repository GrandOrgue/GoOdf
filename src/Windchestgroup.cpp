/*
 * Windchestgroup.cpp is part of GOODF.
 * Copyright (C) 2023 Lars Palo and contributors (see AUTHORS)
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
			wxString tremRef = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(trem));
			outFile->AddLine(wxT("Tremulant") + tremNumber + wxT("=") + tremRef);
		}
	}
}

void Windchestgroup::read(wxFileConfig *cfg) {
	name = cfg->Read("Name", wxEmptyString);
	if (name == wxEmptyString)
		name = wxString::Format(wxT("Windchest %i"), ((int) ::wxGetApp().m_frame->m_organ->getNumberOfWindchestgroups() + 1));
	int nbrEnclosures = static_cast<int>(cfg->ReadLong("NumberOfEnclosures", 0));
	if (nbrEnclosures > 50)
		nbrEnclosures = 50;
	for (int i = 0; i < nbrEnclosures; i++) {
		wxString encNbrStr = wxT("Enclosure") + GOODF_functions::number_format(i + 1);
		int encRefNbr = static_cast<int>(cfg->ReadLong(encNbrStr, 0));
		if (encRefNbr > 0 && encRefNbr <= (int) ::wxGetApp().m_frame->m_organ->getNumberOfEnclosures()) {
			addEnclosureReference(::wxGetApp().m_frame->m_organ->getOrganEnclosureAt(encRefNbr - 1));
		}
	}
	int nbrTremulants = static_cast<int>(cfg->ReadLong("NumberOfTremulants", 0));
	if (nbrTremulants > 10)
		nbrTremulants = 10;
	for (int i = 0; i < nbrTremulants; i++) {
		wxString tremNbrStr = wxT("Tremulant") + GOODF_functions::number_format(i + 1);
		int tremRefNbr = static_cast<int>(cfg->ReadLong(tremNbrStr, 0));
		if (tremRefNbr > 0 && tremRefNbr <= (int) ::wxGetApp().m_frame->m_organ->getNumberOfTremulants()) {
			addTremulantReference(::wxGetApp().m_frame->m_organ->getOrganTremulantAt(tremRefNbr - 1));
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
