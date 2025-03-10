/*
 * ReversiblePiston.h is part of GoOdf.
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
#ifndef REVERSIBLEPISTON_H
#define REVERSIBLEPISTON_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include "Button.h"
#include "Stop.h"
#include "Coupler.h"
#include "Tremulant.h"
#include "GoSwitch.h"

class Organ;

class ReversiblePiston : public Button {
public:
	ReversiblePiston();
	~ReversiblePiston();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, bool usingOldPanelFormat, Organ *readOrgan);

	Stop* getStop();
	void setStop(Stop* stop);
	Coupler* getCoupler();
	void setCoupler(Coupler* coupler);
	GoSwitch* getSwitch();
	void setSwitch(GoSwitch* sw);
	Tremulant* getTremulant();
	void setTremulant(Tremulant* tremulant);
	wxString getObjecType();
	wxString getManual();

private:
	Stop *m_stop;
	Coupler *m_coupler;
	GoSwitch *m_switch;
	Tremulant *m_tremulant;

};

#endif
