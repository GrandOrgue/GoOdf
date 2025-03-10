/*
 * GUITremulant.h is part of GoOdf.
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

#ifndef GUITREMULANT_H
#define GUITREMULANT_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include "Tremulant.h"
#include "GUIButton.h"

class Organ;

class GUITremulant : public GUIButton {
public:
	GUITremulant(Tremulant *tremulant);
	~GUITremulant();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, Organ *readOrgan);

	virtual GUITremulant* clone();
	bool isReferencing(Tremulant *tremulant);
	void updateDisplayName();
	wxString getElementName();

private:
	Tremulant *m_tremulant;
};

#endif
