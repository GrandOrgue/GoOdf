/*
 * Release.cpp is part of GOODF.
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

#include "Release.h"

Release::Release() {
	fileName = wxEmptyString;
	fullPath = wxEmptyString;
	isTremulant = -1;
	maxKeyPressTime = -1;
	cuePoint = -1;
	releaseEnd = -1;
}

Release::Release(const Release& rel) {
	fileName = rel.fileName;
	fullPath = rel.fullPath;
	isTremulant = rel.isTremulant;
	maxKeyPressTime = rel.maxKeyPressTime;
	cuePoint = rel.cuePoint;
	releaseEnd = rel.releaseEnd;
}

Release::~Release() {

}
