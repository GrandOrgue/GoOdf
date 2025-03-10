/*
 * Stop.cpp is part of GoOdf.
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

#include "Stop.h"
#include "GOODF.h"
#include "GOODFFunctions.h"

Stop::Stop() : Drawstop() {
	name = wxT("New Stop");
	m_FirstAccessiblePipeLogicalKeyNumber = 1;
	m_NumberOfAccessiblePipes = 1;
	m_FirstAccessiblePipeLogicalPipeNumber = 1;
	m_internalRank.setName(name);
	m_usingInternalRank = true;
	m_owningManual = NULL;
}

Stop::~Stop() {

}

void Stop::write(wxTextFile *outFile) {
	Drawstop::write(outFile);
	outFile->AddLine(wxT("FirstAccessiblePipeLogicalKeyNumber=") + wxString::Format(wxT("%i"), m_FirstAccessiblePipeLogicalKeyNumber));
	outFile->AddLine(wxT("NumberOfAccessiblePipes=") + wxString::Format(wxT("%i"), m_NumberOfAccessiblePipes));
	if (m_usingInternalRank) {
		outFile->AddLine(wxT("FirstAccessiblePipeLogicalPipeNumber=") + wxString::Format(wxT("%i"), m_FirstAccessiblePipeLogicalPipeNumber));
		m_internalRank.writeFromStop(outFile);
	} else {
		unsigned nbRanks = getNumberOfRanks();
		if (nbRanks > 0) {
			outFile->AddLine(wxT("NumberOfRanks=") + wxString::Format(wxT("%u"), nbRanks));
			unsigned counter = 1;
			for (auto& rankRef : m_referencedRanks) {
				unsigned refRankId = ::wxGetApp().m_frame->m_organ->getIndexOfOrganRank(rankRef.m_rankReference);
				wxString rankId = wxT("Rank") + GOODF_functions::number_format(counter);
				wxString refId = GOODF_functions::number_format(refRankId);
				outFile->AddLine(rankId + wxT("=") + refId);
				if (rankRef.m_firstPipeNumber != 1)
					outFile->AddLine(rankId + wxT("FirstPipeNumber=") + wxString::Format(wxT("%i"), rankRef.m_firstPipeNumber));
				if (rankRef.m_pipeCount != (rankRef.m_rankReference->getNumberOfLogicalPipes() - (rankRef.m_firstPipeNumber - 1)))
					outFile->AddLine(rankId + wxT("PipeCount=") + wxString::Format(wxT("%i"), rankRef.m_pipeCount));
				if (rankRef.m_firstAccessibleKeyNumber != 1)
					outFile->AddLine(rankId + wxT("FirstAccessibleKeyNumber=") + wxString::Format(wxT("%i"), rankRef.m_firstAccessibleKeyNumber));
				counter++;
			}
		}
	}

}

void Stop::read(wxFileConfig *cfg, bool usingOldPanelFormat, Manual* owning_manual, Organ *readOrgan) {
	m_owningManual = owning_manual;
	Drawstop::read(cfg, usingOldPanelFormat, readOrgan);
	int firstPipeKeyNbr = static_cast<int>(cfg->ReadLong("FirstAccessiblePipeLogicalKeyNumber", 1));
	if (firstPipeKeyNbr > 0 && firstPipeKeyNbr < 129) {
		m_FirstAccessiblePipeLogicalKeyNumber = firstPipeKeyNbr;
	}
	int accessiblePipes = static_cast<int>(cfg->ReadLong("NumberOfAccessiblePipes", 1));
	if (accessiblePipes > 0 && accessiblePipes < 193) {
		m_NumberOfAccessiblePipes = accessiblePipes;
	}
	int firstPipePipeNbr = static_cast<int>(cfg->ReadLong("FirstAccessiblePipeLogicalPipeNumber", 1));
	if (firstPipePipeNbr > 0 && firstPipePipeNbr < 193) {
		m_FirstAccessiblePipeLogicalPipeNumber = firstPipePipeNbr;
	}
	int nbrRanks = static_cast<int>(cfg->ReadLong("NumberOfRanks", 0));
	if (nbrRanks > 0 && nbrRanks <= (int) readOrgan->getNumberOfRanks()) {
		// this stop uses references to ranks
		m_usingInternalRank = false;
		for (int i = 0; i < nbrRanks; i++) {
			wxString rankNbrStr = wxT("Rank") + GOODF_functions::number_format(i + 1);
			int refRank = static_cast<int>(cfg->ReadLong(rankNbrStr, 0));
			if (refRank > 0 && refRank <= (int) readOrgan->getNumberOfRanks()) {
				RankReference rankRef;
				rankRef.m_rankReference = readOrgan->getOrganRankAt(refRank - 1);
				int firstPipe = static_cast<int>(cfg->ReadLong(rankNbrStr + "FirstPipeNumber", 1));
				if (firstPipe > 0 && firstPipe <= rankRef.m_rankReference->getNumberOfLogicalPipes()) {
					rankRef.m_firstPipeNumber = firstPipe;
				}
				int pipeCount = static_cast<int>(cfg->ReadLong(rankNbrStr + "PipeCount", rankRef.m_rankReference->getNumberOfLogicalPipes() - (rankRef.m_firstPipeNumber - 1)));
				if (pipeCount > -1 && pipeCount <= (rankRef.m_rankReference->getNumberOfLogicalPipes() - (rankRef.m_firstPipeNumber - 1))) {
					rankRef.m_pipeCount = pipeCount;
				} else {
					rankRef.m_pipeCount = rankRef.m_rankReference->getNumberOfLogicalPipes() - (rankRef.m_firstPipeNumber - 1);
				}
				int firstAccKey = static_cast<int>(cfg->ReadLong(rankNbrStr + "FirstAccessibleKeyNumber", 1));
				if (firstAccKey > 0 && firstAccKey <= m_NumberOfAccessiblePipes) {
					rankRef.m_firstAccessibleKeyNumber = firstAccKey;
				}
				m_referencedRanks.push_back(rankRef);
			}
		}
	} else {
		// this stop uses an internal rank that must be read
		m_usingInternalRank = true;
		m_internalRank.read(cfg, readOrgan);
		m_internalRank.setFirstMidiNoteNumber(m_owningManual->getFirstAccessibleKeyMIDINoteNumber());
	}
}

Rank* Stop::getRankAt(unsigned index) {
	auto iterator = std::next(m_referencedRanks.begin(), index);
	return iterator->m_rankReference;
}

RankReference* Stop::getRankReferenceAt(unsigned index) {
	auto iterator = std::next(m_referencedRanks.begin(), index);
	return &(*iterator);
}

unsigned Stop::getNumberOfRanks() {
	return m_referencedRanks.size();
}

void Stop::addRankReference(Rank *rank) {
	RankReference newRef;
	newRef.m_rankReference = rank;
	newRef.m_pipeCount = rank->getNumberOfLogicalPipes();
	m_referencedRanks.push_back(newRef);
}

void Stop::removeRankReference(Rank *rank) {
	int index = getIndexOfRankReference(rank);
	if (index > -1)
		removeRankReferenceAt(index);
}

void Stop::removeRankReferenceAt(unsigned index) {
	std::list<RankReference>::iterator it = m_referencedRanks.begin();
	std::advance(it, index);
	m_referencedRanks.erase(it);
}

int Stop::getIndexOfRankReference(Rank *rank) {
	int found = -1;
	int counter = 0;
	for (auto& r : m_referencedRanks) {
		if (r.m_rankReference == rank) {
			found = counter;
			break;
		}
		counter++;
	}
	return found;
}

bool Stop::hasRankReference(Rank *rank) {
	bool found = false;
	for (auto& rankRef : m_referencedRanks) {
		if (rankRef.m_rankReference == rank) {
			found = true;
		   	break;
	    }
	}
	return found;
}

int Stop::getFirstPipeLogicalKeyNbr() {
	return m_FirstAccessiblePipeLogicalKeyNumber;
}

void Stop::setFirstPipeLogicalKeyNbr(int keyNbr) {
	m_FirstAccessiblePipeLogicalKeyNumber = keyNbr;
}

int Stop::getNumberOfAccessiblePipes() {
	return m_NumberOfAccessiblePipes;
}

void Stop::setNumberOfAccessiblePipes(int pipes) {
	m_NumberOfAccessiblePipes = pipes;
}

int Stop::getFirstPipeLogicalPipeNbr() {
	return m_FirstAccessiblePipeLogicalPipeNumber;
}

void Stop::setFirstPipeLogicalPipeNbr(int pipeNbr) {
	m_FirstAccessiblePipeLogicalPipeNumber = pipeNbr;
}

bool Stop::isUsingInternalRank() {
	return m_usingInternalRank;
}

void Stop::setUsingInternalRank(bool use) {
	m_usingInternalRank = use;
}

Rank* Stop::getInternalRank() {
	return &m_internalRank;
}

Manual* Stop::getOwningManual() {
	return m_owningManual;
}

void Stop::setOwningManual(Manual *manual) {
	m_owningManual = manual;
}
