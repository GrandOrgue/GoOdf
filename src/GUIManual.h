/*
 * GUIManual.h is part of GoOdf.
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

#ifndef GUIMANUAL_H
#define GUIMANUAL_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include "GUIElements.h"
#include "Manual.h"
#include <list>
#include <vector>
#include "GoImage.h"

class Organ;

struct KEYTYPE {
	wxString KeytypeIdentifier;
	GoImage ImageOn;
	GoImage ImageOff;
	int Width;
	int Offset;
	int YOffset;
	int MouseRectLeft;
	int MouseRectTop;
	int MouseRectWidth;
	int MouseRectHeight;
	int BitmapWidth;
	int BitmapHeight;
	bool ForceWritingWidth;
	bool ForceWritingOffset;
	bool IsSharp;
};

struct MANUAL_RENDER_INFO {
	unsigned width;
	unsigned height;
	int y;
	int x;
	int keys_y;
	int piston_y;
};

struct KEY_INFO {
	unsigned DisplayAsMidiNote;
	bool IsSharp;
	int Xpos;
	int Ypos;
	wxBitmap KeyImage;
};

class GUIManual : public GUIElement {
public:
	GUIManual(Manual *manual);
	~GUIManual();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, Organ *readOrgan);

	virtual GUIManual* clone();
	bool isReferencing(Manual *man);
	void updateDisplayName();

	Manual* getManual();
	void addKeytype(wxString identifier, bool isReading = false);
	unsigned getNumberOfKeytypes();
	KEYTYPE* getKeytypeAt(unsigned index);
	void removeKeytypeAt(unsigned index);
	const wxArrayString& getAvailableKeytypes();
	const wxArrayString& getAvailableKeynumbers();
	bool getDispKeyColourInverted();
	void setDispKeyColourInverted(bool inverted);
	bool getDispKeyColourWooden();
	void setDispKeyColurWooden(bool wooden);
	int getDisplayFirstNote();
	void setDisplayFirstNote(int first);
	int getNumberOfDisplayKeys();
	void setNumberOfDisplayKeys(int nbr);
	std::pair<int, int>* getDisplayKeyAt(unsigned index);
	void setupDefaultDisplayKeys();
	int getIndexOfKeyNumber(wxString keytype);
	int getDispImageNum();
	void setDispImageNum(int nbr);

	MANUAL_RENDER_INFO m_renderInfo;

	void updateKeyInfo();
	KEY_INFO* getKeyInfoAt(unsigned index);
	void setDisplayAsPedal(bool isPedal);
	bool isDisplayedAsPedal();

private:
	Manual *m_manual;
	std::list<KEYTYPE> m_keytypes;
	wxArrayString m_availableKeytypes;
	wxArrayString m_availableKeynumbers;
	std::vector<KEY_INFO> m_keys;

	bool m_dispKeyColourInverted; // (boolean, default: false)
	bool m_dispKeyColourWooden; // (boolean, default: false)
	int m_displayFirstNote; // (integer 0 - 127, default: FirstAccessibleKeyMIDINoteNumber)
	int m_displayKeys;
	std::list<std::pair<int, int>> m_displayKeyMapping;
	std::list<std::pair<int, int>> defaultDisplayKeyMapping;
	/*
		DisplayKeys (integer 1 - NumberOfAccessibleKeys, default: NumberOfAccessibleKeys)
		first = DisplayKey999 (integer 0 - 127, default: FirstAccessibleKeyMIDINoteNumber + 999) 999 is in the range 1 - DisplayKeys
		second = DisplayKey999Note (integer 0 - 127, default: FirstAccessibleKeyMIDINoteNumber + 999)
	*/
	int m_dispImageNum;
	bool m_displayedAsPedal;

	void validateKeyTypes();
	void populateKeyTypes();
	void populateKeyNumbers();
	int baseKeyTypeExistAtIndex(wxString keyNbrType);
	bool keyNbrOverrideBaseKeyWidth(KEYTYPE *key);
};

#endif
