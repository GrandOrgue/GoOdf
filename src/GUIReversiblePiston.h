/*
 * GUIReversiblePiston.h is part of GOODF.
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

#ifndef GUIREVERSIBLEPISTON_H
#define GUIREVERSIBLEPISTON_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include "ReversiblePiston.h"
#include "GUIButton.h"

class GUIReversiblePiston : public GUIButton {
public:
	GUIReversiblePiston(ReversiblePiston *reversiblePiston);
	~GUIReversiblePiston();

	void write(wxTextFile *outFile);
	bool isReferencing(ReversiblePiston *reversiblePiston);
	void updateDisplayName();

private:
	ReversiblePiston *m_reversiblePiston;
};

#endif

