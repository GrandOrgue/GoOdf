/*
 * GoSwitch.cpp is part of GoOdf.
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

#include "GoSwitch.h"

GoSwitch::GoSwitch() : Drawstop() {
	name = wxT("New Switch");
}

GoSwitch::~GoSwitch() {

}

void GoSwitch::write(wxTextFile *outFile) {
	Drawstop::write(outFile);
}

void GoSwitch::read(wxFileConfig *cfg, bool usingOldPanelFormat, Organ *readOrgan) {
	Drawstop::read(cfg, usingOldPanelFormat, readOrgan);
}
