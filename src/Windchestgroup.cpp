/*
 * Windchestgroup.cpp is part of GoOdf.
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

#include "Windchestgroup.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

Windchestgroup::Windchestgroup() {
	name = wxT("New windchest");
	m_amplitudeLevel = 100.0f;
	m_gain = 0.0f;
	m_pitchTuning = 0.0f;
	m_pitchCorrection = 0.0f;
	m_trackerDelay = 0;
	m_isPercussive = false;
	m_hasIndependentRelease = false;
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
	if (m_amplitudeLevel != 100)
		outFile->AddLine(wxT("AmplitudeLevel=") + wxString::Format(wxT("%f"), m_amplitudeLevel));
	if (m_gain != 0)
		outFile->AddLine(wxT("Gain=") + wxString::Format(wxT("%f"), m_gain));
	if (m_pitchTuning != 0)
		outFile->AddLine(wxT("PitchTuning=") + wxString::Format(wxT("%f"), m_pitchTuning));
	if (m_pitchCorrection != 0)
		outFile->AddLine(wxT("PitchCorrection=") + wxString::Format(wxT("%f"), m_pitchCorrection));
	if (m_trackerDelay != 0)
		outFile->AddLine(wxT("TrackerDelay=") + wxString::Format(wxT("%u"), m_trackerDelay));
	if (m_isPercussive != ::wxGetApp().m_frame->m_organ->getIsPercussive()) {
		if (m_isPercussive) {
			outFile->AddLine(wxT("Percussive=Y"));
			if (m_hasIndependentRelease) {
				outFile->AddLine(wxT("HasIndependentRelease=Y"));
			}
		} else {
			outFile->AddLine(wxT("Percussive=N"));
		}
	}
}

void Windchestgroup::read(wxFileConfig *cfg, Organ *readOrgan) {
	name = cfg->Read("Name", wxEmptyString);
	if (name == wxEmptyString)
		name = wxString::Format(wxT("Windchest %i"), ((int) readOrgan->getNumberOfWindchestgroups() + 1));
	int nbrEnclosures = static_cast<int>(cfg->ReadLong("NumberOfEnclosures", 0));
	if (nbrEnclosures > 999)
		nbrEnclosures = 999;
	for (int i = 0; i < nbrEnclosures; i++) {
		wxString encNbrStr = wxT("Enclosure") + GOODF_functions::number_format(i + 1);
		int encRefNbr = static_cast<int>(cfg->ReadLong(encNbrStr, 0));
		if (encRefNbr > 0 && encRefNbr <= (int) readOrgan->getNumberOfEnclosures()) {
			addEnclosureReference(readOrgan->getOrganEnclosureAt(encRefNbr - 1));
		}
	}
	int nbrTremulants = static_cast<int>(cfg->ReadLong("NumberOfTremulants", 0));
	if (nbrTremulants > 999)
		nbrTremulants = 999;
	for (int i = 0; i < nbrTremulants; i++) {
		wxString tremNbrStr = wxT("Tremulant") + GOODF_functions::number_format(i + 1);
		int tremRefNbr = static_cast<int>(cfg->ReadLong(tremNbrStr, 0));
		if (tremRefNbr > 0 && tremRefNbr <= (int) readOrgan->getNumberOfTremulants()) {
			addTremulantReference(readOrgan->getOrganTremulantAt(tremRefNbr - 1));
		}
	}

	float ampLvl = static_cast<float>(cfg->ReadDouble("AmplitudeLevel", 100.0f));
	if (ampLvl >= 0.0f && ampLvl <= 1000.0f)
		setAmplitudeLevel(ampLvl);
	float gain = static_cast<float>(cfg->ReadDouble("Gain", 0.0f));
	if (gain >= -120.0f && gain <= 40.0f)
		setGain(gain);
	float pitchTuning = static_cast<float>(cfg->ReadDouble("PitchTuning", 0.0f));
	if (pitchTuning >= -1800.0f && pitchTuning <= 1800.0f)
		setPitchTuning(pitchTuning);
	float pitchCorrection = static_cast<float>(cfg->ReadDouble("PitchCorrection", 0.0f));
	if (pitchCorrection >= -1800.0f && pitchCorrection <= 1800.0f)
		setPitchCorrection(pitchCorrection);
	long trackerDelay = cfg->ReadLong("TrackerDelay", 0);
	if (trackerDelay >= 0 && trackerDelay <= 10000) {
		setTrackerDelay(static_cast<unsigned>(trackerDelay));
	}
	wxString cfgBoolValue = cfg->Read("Percussive", wxEmptyString);
	setIsPercussive(GOODF_functions::parseBoolean(cfgBoolValue, readOrgan->getIsPercussive()));
	cfgBoolValue = cfg->Read("HasIndependentRelease", wxEmptyString);
	setHasIndependentRelease(GOODF_functions::parseBoolean(cfgBoolValue, readOrgan->getHasIndependentRelease()));
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

float Windchestgroup::getAmplitudeLevel() {
	return m_amplitudeLevel;
}

void Windchestgroup::setAmplitudeLevel(float amplitudeLevel) {
	m_amplitudeLevel = amplitudeLevel;
}

float Windchestgroup::getGain() {
	return m_gain;
}

void Windchestgroup::setGain(float gain) {
	m_gain = gain;
}

float Windchestgroup::getPitchTuning() {
	return m_pitchTuning;
}

void Windchestgroup::setPitchTuning(float pitchTuning) {
	m_pitchTuning = pitchTuning;
}

unsigned Windchestgroup::getTrackerDelay() {
	return m_trackerDelay;
}

void Windchestgroup::setTrackerDelay(unsigned trackerDelay) {
	m_trackerDelay = trackerDelay;
}

float Windchestgroup::getPitchCorrection() {
	return m_pitchCorrection;
}

void Windchestgroup::setPitchCorrection(float pitchCorrection) {
	m_pitchCorrection = pitchCorrection;
}

bool Windchestgroup::getIsPercussive() {
	return m_isPercussive;
}

void Windchestgroup::setIsPercussive(bool percussive) {
	m_isPercussive = percussive;
}

bool Windchestgroup::getHasIndependentRelease() {
	return m_hasIndependentRelease;
}

void Windchestgroup::setHasIndependentRelease(bool independentRel) {
	m_hasIndependentRelease = independentRel;
}

bool Windchestgroup::isPipesOnThisWindchest() {
	bool pipesFound = false;

	for (unsigned i = 0; i < ::wxGetApp().m_frame->m_organ->getNumberOfRanks(); i ++) {
		if (pipesFound)
			break;

		Rank *r = ::wxGetApp().m_frame->m_organ->getOrganRankAt(i);
		for (Pipe &p : r->m_pipes) {
			if (p.windchest == this) {
				pipesFound = true;
				break;
			}
		}
	}

	if (!pipesFound) {
		for (unsigned i = 0; i < ::wxGetApp().m_frame->m_organ->getNumberOfStops(); i ++) {
			if (pipesFound)
				break;

			Stop *s = ::wxGetApp().m_frame->m_organ->getOrganStopAt(i);
			if (s->isUsingInternalRank()) {
				Rank *r = s->getInternalRank();
				for (Pipe &p : r->m_pipes) {
					if (p.windchest == this) {
						pipesFound = true;
							break;
					}
				}
			}
		}
	}

	return pipesFound;
}

void Windchestgroup::applyPercussiveRecursively() {
	// apply Percussive value to all separate ranks
	for (unsigned i = 0; i < ::wxGetApp().m_frame->m_organ->getNumberOfRanks(); i ++) {
		Rank *r = ::wxGetApp().m_frame->m_organ->getOrganRankAt(i);
		for (Pipe &p : r->m_pipes) {
			if (p.windchest == this) {
				p.isPercussive = this->getIsPercussive();
				// A percussive pipe without independent releases should not have any releases
				if (p.isPercussive && !p.hasIndependentRelease) {
					if (!p.m_releases.empty())
						p.m_releases.clear();
				}
				// A pipe cannot have HasIndependentRelease=Y if not also Percussive
				if (!p.isPercussive)
					p.hasIndependentRelease = false;
			}
		}
		if (r->getWindchest() == this) {
			r->setPercussive(this->getIsPercussive());
			if (!r->isPercussive())
				r->setIndependentRelease(false);
		}
	}
	// and to any stop with internal rank
	for (unsigned i = 0; i < ::wxGetApp().m_frame->m_organ->getNumberOfStops(); i ++) {
		Stop *s = ::wxGetApp().m_frame->m_organ->getOrganStopAt(i);
		if (s->isUsingInternalRank()) {
			Rank *r = s->getInternalRank();
			for (Pipe &p : r->m_pipes) {
				if (p.windchest == this) {
					p.isPercussive = this->getIsPercussive();
					// A percussive pipe without independent releases should not have any releases
					if (p.isPercussive && !p.hasIndependentRelease) {
						if (!p.m_releases.empty())
							p.m_releases.clear();
					}
					// A pipe cannot have HasIndependentRelease=Y if not also Percussive
					if (!p.isPercussive)
						p.hasIndependentRelease = false;
				}
			}
			if (r->getWindchest() == this) {
				r->setPercussive(this->getIsPercussive());
				if (!r->isPercussive())
					r->setIndependentRelease(false);
			}
		}
	}
}

void Windchestgroup::applyHasIndependentReleaseRecursively() {
	// apply HasIndependentRelease value to all separate ranks
	for (unsigned i = 0; i < ::wxGetApp().m_frame->m_organ->getNumberOfRanks(); i ++) {
		Rank *r = ::wxGetApp().m_frame->m_organ->getOrganRankAt(i);
		for (Pipe &p : r->m_pipes) {
			if (p.windchest == this) {
				if ((this->getHasIndependentRelease() && p.isPercussive) || !this->getHasIndependentRelease())
					p.hasIndependentRelease = this->getHasIndependentRelease();
			}
		}
		if (r->getWindchest() == this) {
			if ((this->getHasIndependentRelease() && r->isPercussive()) || !this->getHasIndependentRelease())
				r->setIndependentRelease(this->getHasIndependentRelease());
		}
	}
	// and to any stop with internal rank
	for (unsigned i = 0; i < ::wxGetApp().m_frame->m_organ->getNumberOfStops(); i ++) {
		Stop *s = ::wxGetApp().m_frame->m_organ->getOrganStopAt(i);
		if (s->isUsingInternalRank()) {
			Rank *r = s->getInternalRank();
			for (Pipe &p : r->m_pipes) {
				if (p.windchest == this) {
					if ((this->getHasIndependentRelease() && p.isPercussive) || !this->getHasIndependentRelease())
						p.hasIndependentRelease = this->getHasIndependentRelease();
				}
			}
			if (r->getWindchest() == this) {
				if ((this->getHasIndependentRelease() && r->isPercussive()) || !this->getHasIndependentRelease())
					r->setIndependentRelease(this->getHasIndependentRelease());
			}
		}
	}
}
