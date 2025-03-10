/*
 * Button.h is part of GoOdf.
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

#ifndef BUTTON_H
#define BUTTON_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>

class Button {
public:
	Button();
	~Button();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, bool usingOldPanelFormat);

	wxString getName();
	bool isDisplayed();
	void setName(wxString name);
	int getShortCutKey();
	void setShortCutKey(int shortCutKey);
	bool isDisplayedInverted();
	void setDisplayInverted(bool displayInverted);

protected:
	wxString name;
	bool displayed;
	bool m_displayInInvertedState;
	int shortCutKey;
};

#endif
