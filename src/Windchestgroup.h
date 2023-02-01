/*
 * Windchestgroup.h is part of GOODF.
 * Copyright (C) 2023 Lars Palo
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

#ifndef WINDCHESTGROUP_H
#define WINDCHESTGROUP_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <list>
#include "Enclosure.h"
#include "Tremulant.h"

class Windchestgroup {
public:
	Windchestgroup();
	~Windchestgroup();

	void write(wxTextFile *outFile);
	Enclosure* getEnclosureAt(unsigned index);
	unsigned getNumberOfEnclosures();
	void addEnclosureReference(Enclosure *enclosure);
	void removeEnclosureReference(Enclosure *enclosure);
	void removeEnclosureReferenceAt(unsigned index);
	bool hasEnclosureReference(Enclosure *enclosure);
	Tremulant* getTremulantAt(unsigned index);
	unsigned getNumberOfTremulants();
	void addTremulantReference(Tremulant *tremulant);
	void removeTremulantReference(Tremulant *tremulant);
	void removeTremulantReferenceAt(unsigned index);
	bool hasTremulantReference(Tremulant *tremulant);
	wxString getName();
	void setName(wxString name);

private:
	wxString name;
	std::list<Enclosure *> m_Enclosures;
	std::list<Tremulant *> m_Tremulants;
};

#endif
