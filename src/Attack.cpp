/*
 * Attack.cpp is part of GoOdf.
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

#include "Attack.h"

Attack::Attack() {
	fileName = wxT("DUMMY");
	fullPath = wxT("DUMMY");
	loadRelease = true;
	attackVelocity = 0;
	maxTimeSinceLastRelease = -1;
	isTremulant = -1;
	maxKeyPressTime = -1;
	attackStart = 0;
	cuePoint = -1;
	releaseEnd = -1;
	loopCrossfadeLength = 0;
	releaseCrossfadeLength = 0;
}

Attack::Attack(const Attack& att) {
	fileName = att.fileName;
	fullPath = att.fullPath;
	loadRelease = att.loadRelease;
	attackVelocity = att.attackVelocity;
	maxTimeSinceLastRelease = att.maxTimeSinceLastRelease;
	isTremulant = att.isTremulant;
	maxKeyPressTime = att.maxKeyPressTime;
	attackStart = att.attackStart;
	cuePoint = att.cuePoint;
	releaseEnd = att.releaseEnd;
	loopCrossfadeLength = att.loopCrossfadeLength;
	releaseCrossfadeLength = att.releaseCrossfadeLength;
	for (Loop l: att.m_loops) {
		addNewLoop(l);
	}
}

Attack::~Attack() {

}

Loop* Attack::getLoopAt(unsigned index) {
	auto iterator = std::next(m_loops.begin(), index);
	return &(*iterator);
}

void Attack::addNewLoop(Loop l) {
	m_loops.push_back(l);
}

void Attack::removeLoopAt(unsigned index) {
	std::list<Loop>::iterator it = m_loops.begin();
	std::advance(it, index);
	m_loops.erase(it);
}
