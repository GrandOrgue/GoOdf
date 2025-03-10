/*
 * Attack.h is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * GoOdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GoOdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GoOdf. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#ifndef ATTACK_H
#define ATTACK_H

#include <wx/wx.h>
#include "Loop.h"
#include <list>

class Attack {
public:
	Attack();
	Attack(const Attack& att);
	~Attack();

	Loop* getLoopAt(unsigned index);
	void addNewLoop(Loop l);
	void removeLoopAt(unsigned index);

	wxString fileName;
	wxString fullPath;
	bool loadRelease;
	int attackVelocity;
	int maxTimeSinceLastRelease;
	int isTremulant;
	int maxKeyPressTime;
	int attackStart;
	int cuePoint;
	int releaseEnd;
	std::list<Loop> m_loops;
	int loopCrossfadeLength;
	int releaseCrossfadeLength;

};

#endif
