/*
 * Stop.h is part of GoOdf.
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

#ifndef STOP_H
#define STOP_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include "Drawstop.h"
#include <list>
#include "RankReference.h"

class Manual;
class Organ;

class Stop : public Drawstop {
public:
	Stop();
	~Stop();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, bool usingOldPanelFormat, Manual* owning_manual, Organ *readOrgan);

	Rank* getRankAt(unsigned index);
	RankReference* getRankReferenceAt(unsigned index);
	unsigned getNumberOfRanks();
	void addRankReference(Rank *rank);
	void removeRankReference(Rank *rank);
	void removeRankReferenceAt(unsigned index);
	int getIndexOfRankReference(Rank *rank);
	bool hasRankReference(Rank *rank);
	int getFirstPipeLogicalKeyNbr();
	void setFirstPipeLogicalKeyNbr(int keyNbr);
	int getNumberOfAccessiblePipes();
	void setNumberOfAccessiblePipes(int pipes);
	int getFirstPipeLogicalPipeNbr();
	void setFirstPipeLogicalPipeNbr(int pipeNbr);
	bool isUsingInternalRank();
	void setUsingInternalRank(bool use);
	Rank* getInternalRank();
	Manual* getOwningManual();
	void setOwningManual(Manual *manual);

protected:
	int m_FirstAccessiblePipeLogicalKeyNumber; // 1 - 128
	int m_NumberOfAccessiblePipes; // 1 - 192
	int m_FirstAccessiblePipeLogicalPipeNumber; // 1 - 192, needed if using internal rank, not when using referenced ones
	std::list<RankReference> m_referencedRanks;
	Rank m_internalRank;
	bool m_usingInternalRank;
	Manual *m_owningManual;
};

#endif
