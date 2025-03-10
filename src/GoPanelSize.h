/*
 * GoPanelSize.h is part of GoOdf.
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

#ifndef GOPANELSIZE_H
#define GOPANELSIZE_H

#include <wx/wx.h>

class GoPanelSize {
public:
	GoPanelSize();
	~GoPanelSize();

	int getNumericalValue() const;
	void setNumericalValue(int value);
	const wxArrayString& getPanelSizeNames() const;
	int getSelectedNameIndex();
	void setSelectedNameIndex(int index, bool isHorizontal);

private:
	wxArrayString m_panelSizeNames;
	int m_numericalValue;
	int m_selectedSizeNameIndex;

};

#endif
