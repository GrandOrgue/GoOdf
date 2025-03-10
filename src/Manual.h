/*
 * Manual.h is part of GoOdf.
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

#ifndef MANUAL_H
#define MANUAL_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include <list>
#include "Stop.h"
#include "Divisional.h"
#include "Tremulant.h"
#include "GoSwitch.h"
#include <map>

class Coupler;
class Organ;

class Manual {
public:
	Manual();
	~Manual();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, bool useOldPanelFormat, wxString manId, Organ *readOrgan);
	void readCouplers(wxFileConfig *cfg, bool useOldPanelFormat, wxString manId, Organ *readOrgan);
	void readDivisionals(wxFileConfig *cfg, bool useOldPanelFormat, wxString manId, Organ *readOrgan);

	wxString getName();
	void setName(wxString name);
	int getNumberOfLogicalKeys();
	void setNumberOfLogicalKeys(int nbKeys);
	int getFirstAccessibleKeyLogicalKeyNumber();
	void setFirstAccessibleKeyLogicalKeyNumber(int firstKeyNbr);
	int getFirstAccessibleKeyMIDINoteNumber();
	void setFirstAccessibleKeyMIDINoteNumber(int firstNoteNbr);
	int getNumberOfAccessibleKeys();
	void setNumberOfAccessibleKeys(int nbKeys);
	int getMidiInputNumber();
	void setMidiInputNumber(int midiInputNbr);
	bool isThePedal();
	void setIsPedal(bool isPedal, bool isParsing = false);
	bool isDisplayed();
	unsigned getNumberOfStops();
	Stop* getStopAt(unsigned index);
	void addStop(Stop *stop);
	void removeStop(Stop *stop);
	void removeStopAt(unsigned index);
	bool hasStopReference(Stop *stop);
	int getIndexOfStop(Stop *stop);
	void moveStop(int idxToMove, int destinationIdx);
	unsigned getNumberOfCouplers();
	Coupler* getCouplerAt(unsigned index);
	void addCoupler(Coupler *coupler);
	void removeCoupler(Coupler *coupler);
	void removeCouplerAt(unsigned index);
	int getIndexOfCoupler(Coupler *coupler);
	unsigned getNumberOfDivisionals();
	Divisional* getDivisionalAt(unsigned index);
	void addDivisional(Divisional *divisional);
	void removeDivisional(Divisional *divisional);
	void removeDivisionalAt(unsigned index);
	int getIndexOfDivisional(Divisional *divisional);
	unsigned getNumberOfTremulants();
	Tremulant* getTremulantAt(unsigned index);
	void addTremulant(Tremulant *tremulant);
	void removeTremulant(Tremulant *tremulant);
	void removeTremulantAt(unsigned index);
	int getIndexOfTremulant(Tremulant *tremulant);
	bool hasTremulantReference(Tremulant *tremulant);
	unsigned getNumberOfGoSwitches();
	GoSwitch* getGoSwitchAt(unsigned index);
	void addGoSwitch(GoSwitch *sw);
	void removeGoSwitch(GoSwitch *sw);
	void removeGoSwitchAt(unsigned index);
	int getIndexOfGoSwitch(GoSwitch *sw);
	bool hasGoSwitchReference(GoSwitch *sw);
	int getMidiKeyMapValue(wxString key);
	void setMidiKeyMapValue(wxString key, int value);

private:
	wxString m_name;
	int m_numberOfLogicalKeys; // 1 - 192
	int m_firstAccessibleKeyLogicalKeyNumber; // 1 - m_numberOfLogicalKeys
	int m_firstAccessibleKeyMIDINoteNumber; // 0 - 127
	int m_numberOfAccessibleKeys; // 0 - 85
	int m_midiInputNumber; // 0 - 200, default 0
	bool m_displayed;
	std::list<Stop*> m_stops;
	std::list<Coupler*> m_couplers;
	std::list<Divisional*> m_divisionals;
	std::list<Tremulant*> m_tremulants;
	std::list<GoSwitch*> m_switches;
	std::map<wxString, unsigned> m_midiKeyMap;
	std::map<wxString, unsigned> originalMidiKeyMap;
	bool m_thePedal;

	void SetupBasicMidiKeyMap();
};

#endif
