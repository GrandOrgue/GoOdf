/*
 * Tremulant.h is part of GoOdf.
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

#ifndef TREMULANT_H
#define TREMULANT_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include "Drawstop.h"

class Organ;

class Tremulant : public Drawstop {
public:
	Tremulant();
	~Tremulant();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, bool usingOldPanelFormat, Organ *readOrgan);

	int getAmpModDepth();
	void setAmpModDepth(int ampModDepth);
	int getPeriod();
	void setPeriod(int period);
	int getStartRate();
	void setStartRate(int startRate);
	int getStopRate();
	void setStopRate(int stopRate);
	wxString getTremType();
	void setTremType(wxString tremType);

protected:
	wxString tremType;
	int period;
	int startRate;
	int stopRate;
	int ampModDepth;
};

#endif
