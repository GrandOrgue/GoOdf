/*
 * Coupler.h is part of GoOdf.
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

#ifndef COUPLER_H
#define COUPLER_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include "Drawstop.h"
#include <list>

class Manual;
class Organ;

class Coupler : public Drawstop {
public:
	Coupler();
	~Coupler();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, bool usingOldPanelFormat, Manual *owning_manual, Organ *readOrgan);

	wxString getCouplerType();
	void setCouplerType(wxString couplerType);
	bool isCoupleToSubsequentDownwardIntermanualCouplers();
	void setCoupleToSubsequentDownwardIntermanualCouplers(bool option);
	bool isCoupleToSubsequentDownwardIntramanualCouplers();
	void setCoupleToSubsequentDownwardIntramanualCouplers(bool option);
	bool isCoupleToSubsequentUnisonIntermanualCouplers();
	void setCoupleToSubsequentUnisonIntermanualCouplers(bool option);
	bool isCoupleToSubsequentUpwardIntermanualCouplers();
	void setCoupleToSubsequentUpwardIntermanualCouplers(bool option);
	bool isCoupleToSubsequentUpwardIntramanualCouplers();
	void setCoupleToSubsequentUpwardIntramanualCouplers(bool option);
	int getDestinationKeyshift();
	void setDestinationKeyshift(int keyshift);
	Manual* getDestinationManual();
	void setDestinationManual(Manual *destinationManual);
	int getFirstMidiNoteNumber();
	void setFirstMidiNoteNumber(int firstMidiNoteNumber);
	int getNumberOfKeys();
	void setNumberOfKeys(int numberOfKeys);
	Manual* getOwningManual();
	void setOwningManual(Manual *owningManual);
	bool isUnisonOff();
	void setUnisonOff(bool option);

protected:
	bool m_unisonOff;
	Manual *m_owningManual;
	Manual *m_destinationManual;
	int m_destinationKeyshift; // -24 - 24
	bool m_coupleToSubsequentUnisonIntermanualCouplers;
	bool m_coupleToSubsequentUpwardIntermanualCouplers;
	bool m_coupleToSubsequentDownwardIntermanualCouplers;
	bool m_coupleToSubsequentUpwardIntramanualCouplers;
	bool m_coupleToSubsequentDownwardIntramanualCouplers;
	wxString m_couplerType;
	int m_firstMIDINoteNumber; // 0 - 127 default 0
	int m_numberOfKeys; // 0 - 127 default 127?

};

#endif
