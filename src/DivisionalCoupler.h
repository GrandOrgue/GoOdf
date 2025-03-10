/*
 * DivisionalCoupler.h is part of GoOdf.
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

#ifndef DIVISIONALCOUPLER_H
#define DIVISIONALCOUPLER_H

#include <wx/textfile.h>
#include <list>
#include "Drawstop.h"
#include "Manual.h"

class Organ;

class DivisionalCoupler : public Drawstop {
public:
	DivisionalCoupler();
	~DivisionalCoupler();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, bool usingOldPanelFormat, Organ *readOrgan);

	bool hasBiDirectionalCoupling();
	void setBiDirectionalCoupling(bool isBiDirectional);
	void addAffectedManual(Manual *manual);
	unsigned getNumberOfManuals();
	Manual* getManualAt(unsigned index);
	void removeManualAt(unsigned index);
	bool hasManualReference(Manual *manual);
	void removeManualReference(Manual *manual);

private:
	bool m_biDirectionalCoupling;
	std::list<Manual*> m_affectedManuals;

};

#endif
