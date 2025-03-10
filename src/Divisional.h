/*
 * Divisional.h is part of GoOdf.
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
 * along with GoOdf.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#ifndef DIVISIONAL_H
#define DIVISIONAL_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include "Button.h"
#include <list>
#include <utility>
#include "Stop.h"
#include "Coupler.h"
#include "Tremulant.h"
#include "GoSwitch.h"

class Manual;

class Divisional : public Button {
public:
	Divisional();
	~Divisional();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, bool usingOldPanelFormat, Manual *owning_manual);

	bool isProtected();
	void setProtected(bool option);
	unsigned getNumberOfStops();
	void addStop(Stop *stop, bool isOn);
	void removeStopAt(unsigned index);
	void removeStop(Stop *stop);
	void removeAllStops();
	std::pair<Stop*, bool>* getStopPairAt(unsigned index);
	unsigned getNumberOfCouplers();
	void addCoupler(Coupler *coupler, bool isOn);
	void removeCouplerAt(unsigned index);
	void removeCoupler(Coupler *coupler);
	void removeAllCouplers();
	std::pair<Coupler*, bool>* getCouplerPairAt(unsigned index);
	unsigned getNumberOfTremulants();
	void addTremulant(Tremulant *trem, bool isOn);
	void removeTremulantAt(unsigned index);
	void removeAllTremulants();
	void removeTremulant(Tremulant *trem);
	std::pair<Tremulant*, bool>* getTremulantPairAt(unsigned index);
	unsigned getNumberOfSwitches();
	void addSwitch(GoSwitch *sw, bool isOn);
	void removeSwitchAt(unsigned index);
	void removeSwitch(GoSwitch *sw);
	void removeAllSwitches();
	std::pair<GoSwitch*, bool>* getSwitchPairAt(unsigned index);
	bool hasStop(Stop *stop);
	bool hasCoupler(Coupler *coupler);
	bool hasTremulant(Tremulant *trem);
	bool hasSwitch(GoSwitch *sw);
	Manual* getOwningManual();
	void setOwningManual(Manual *manual);


private:
	bool m_protected;
	std::list< std::pair<Stop*, bool> > m_stops;
	std::list< std::pair<Coupler*, bool> > m_couplers;
	std::list< std::pair<Tremulant*, bool> > m_tremulants;
	std::list< std::pair<GoSwitch*, bool> > m_switches;
	
	Manual *m_owningManual;

};

#endif
