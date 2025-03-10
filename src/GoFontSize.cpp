/*
 * GoFontSize.cpp is part of GoOdf.
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

#include "GoFontSize.h"

GoFontSize::GoFontSize() {
	m_sizeNames.Add(wxT("SMALL"));
	m_sizeNames.Add(wxT("NORMAL"));
	m_sizeNames.Add(wxT("LARGE"));
	m_sizeNames.Add(wxT("Numerical value"));

	m_fontSize = 7;
	m_selectedSizeIndex = 1;
}

GoFontSize::~GoFontSize() {

}

wxString GoFontSize::getSizeName() const {
	if (m_selectedSizeIndex > -1 && (unsigned) m_selectedSizeIndex < m_sizeNames.size()) {
		return m_sizeNames.Item(m_selectedSizeIndex);
	} else {
		return wxEmptyString;
	}
}

int GoFontSize::getSizeValue() const {
	return m_fontSize;
}

void GoFontSize::setSizeValue(int size) {
	if (size > 0 && size < 51) {
		m_fontSize = size;
		if (m_fontSize == 6)
			m_selectedSizeIndex = 0;
		else if (m_fontSize == 7)
			m_selectedSizeIndex = 1;
		else if (m_fontSize == 10)
			m_selectedSizeIndex = 2;
		else
			m_selectedSizeIndex = 3;
	}
}

const wxArrayString& GoFontSize::getSizeNames() const {
	return m_sizeNames;
}

int GoFontSize::getSelectedSizeIndex() {
	return m_selectedSizeIndex;
}

void GoFontSize::setSelectedSizeIndex(int index) {
	if (index > -1 && (unsigned) index < m_sizeNames.size()) {
		m_selectedSizeIndex = index;
		wxString sizeName = m_sizeNames.Item(m_selectedSizeIndex);

		if (sizeName == wxT("SMALL"))
			m_fontSize = 6;
		else if (sizeName == wxT("NORMAL"))
			m_fontSize = 7;
		else if (sizeName == wxT("LARGE"))
			m_fontSize = 10;
	}
}
