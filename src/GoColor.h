/*
 * GoColor.h is part of GoOdf.
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

#ifndef GOCOLOR_H
#define GOCOLOR_H

#include <wx/wx.h>

class GoColor {
public:
	GoColor();
	~GoColor();

	wxString getHtmlValue() const;
	wxString getColorName() const;
	wxColour getColor() const;
	void setColorValue(wxColour value);
	const wxArrayString& getColorNames() const;
	int getSelectedColorIndex();
	void setSelectedColorIndex(int index);

private:
	wxArrayString m_colorNames;
	wxColour m_color;
	int m_selectedColorIndex;

};

#endif
