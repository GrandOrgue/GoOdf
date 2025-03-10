/*
 * GoFontSize.h is part of GoOdf.
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

#ifndef GOFONTSIZE_H
#define GOFONTSIZE_H

#include <wx/wx.h>

class GoFontSize {
public:
	GoFontSize();
	~GoFontSize();

	wxString getSizeName() const;
	int getSizeValue() const;
	void setSizeValue(int size);
	const wxArrayString& getSizeNames() const;
	int getSelectedSizeIndex();
	void setSelectedSizeIndex(int index);

private:
	wxArrayString m_sizeNames; // SMALL = 6, NORMAL = 7, LARGE = 10
	int m_fontSize; // 1 - 50 default NORMAL
	int m_selectedSizeIndex;

};

#endif
