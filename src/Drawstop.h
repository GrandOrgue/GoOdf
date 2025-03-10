/*
 * Drawstop.h is part of GoOdf.
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

#ifndef DRAWSTOP_H
#define DRAWSTOP_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <list>
#include "Button.h"

class GoSwitch;
class Organ;

class Drawstop : public Button {
public:
	Drawstop();
	~Drawstop();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, bool usingOldPanelFormat, Organ *readOrgan);

	bool isDefaultToEngaged();
	void setDefaultToEngaged(bool defaultToEngaged);
	wxString getFunction();
	void setFunction(wxString function);
	int getGcState();
	void setGcState(int gcState);
	unsigned getNumberOfSwitches();
	GoSwitch* getSwitchAtIndex(unsigned index);
	void addSwitchReference(GoSwitch *switchToAdd);
	unsigned getIndexOfSwitch(GoSwitch *switchToFind);
	void removeSwitchReference(GoSwitch *sw);
	void removeSwitchReferenceAt(unsigned index);
	void removeAllSwitchReferences();
	bool hasSwitchReference(GoSwitch *sw);
	bool isStoreInDivisional();
	void setStoreInDivisional(bool storeInDivisional);
	bool isStoreInGeneral();
	void setStoreInGeneral(bool storeInGeneral);

protected:
	wxString function;
	std::list<GoSwitch *> m_switches;
	bool defaultToEngaged;
	int gcState;
	bool storeInDivisional;
	bool storeInGeneral;

};

#endif
