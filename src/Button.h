/*
 * Button.h is part of GOODF.
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

#ifndef BUTTON_H
#define BUTTON_H

#include <wx/wx.h>
#include <wx/textfile.h>

class Button {
public:
	Button();
	~Button();

	void write(wxTextFile *outFile);
	wxString getName();
	bool isDisplayed();
	void setDisplayed(bool displayed);
	bool isDisplayedInInvertedState();
	void setDisplayedInInvertedState(bool displayedInInvertedState);
	void setName(wxString name);
	int getShortCutKey();
	void setShortCutKey(int shortCutKey);

protected:
	wxString name;
	bool displayed;
	bool displayedInInvertedState;
	int shortCutKey;
};

#endif
