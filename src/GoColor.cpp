/*
 * GoColor.cpp is part of GoOdf.
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

#include "GoColor.h"

GoColor::GoColor() {
	m_colorNames.Add(wxT("Custom color"));
	m_colorNames.Add(wxT("BLACK"));
	m_colorNames.Add(wxT("BLUE"));
	m_colorNames.Add(wxT("DARK BLUE"));
	m_colorNames.Add(wxT("GREEN"));
	m_colorNames.Add(wxT("DARK GREEN"));
	m_colorNames.Add(wxT("CYAN"));
	m_colorNames.Add(wxT("DARK CYAN"));
	m_colorNames.Add(wxT("RED"));
	m_colorNames.Add(wxT("DARK RED"));
	m_colorNames.Add(wxT("MAGENTA"));
	m_colorNames.Add(wxT("DARK MAGENTA"));
	m_colorNames.Add(wxT("YELLOW"));
	m_colorNames.Add(wxT("DARK YELLOW"));
	m_colorNames.Add(wxT("LIGHT GREY"));
	m_colorNames.Add(wxT("DARK GREY"));
	m_colorNames.Add(wxT("WHITE"));
	m_colorNames.Add(wxT("BROWN"));

	m_color = wxColour(0x00, 0x00, 0x00);
	m_selectedColorIndex = 0;
}

GoColor::~GoColor() {

}

wxString GoColor::getHtmlValue() const {
	return m_color.GetAsString(wxC2S_HTML_SYNTAX);
}

wxString GoColor::getColorName() const {
	if (m_selectedColorIndex > 0)
		return m_colorNames.Item(m_selectedColorIndex);
	else
		return getHtmlValue();
}

wxColour GoColor::getColor() const {
	return m_color;
}

void GoColor::setColorValue(wxColour value) {
	m_color = value;
	m_selectedColorIndex = 0;
}

const wxArrayString& GoColor::getColorNames() const {
	return m_colorNames;
}

int GoColor::getSelectedColorIndex() {
	return m_selectedColorIndex;
}

void GoColor::setSelectedColorIndex(int index) {
	if (index > -1 && (unsigned) index < m_colorNames.size()) {
		m_selectedColorIndex = index;
		wxString value = m_colorNames.Item(index);

		if (value == wxT("BLACK"))
			m_color = wxColour(0x00, 0x00, 0x00);
		else if (value == wxT("BLUE"))
			m_color = wxColour(0x00, 0x00, 0xFF);
		else if (value == wxT("DARK BLUE"))
			m_color = wxColour(0x00, 0x00, 0x80);
		else if (value == wxT("GREEN"))
			m_color = wxColour(0x00, 0xFF, 0x00);
		else if (value == wxT("DARK GREEN"))
			m_color = wxColour(0x00, 0x80, 0x00);
		else if (value == wxT("CYAN"))
			m_color = wxColour(0x00, 0xFF, 0xFF);
		else if (value == wxT("DARK CYAN"))
			m_color = wxColour(0x00, 0x80, 0x80);
		else if (value == wxT("RED"))
			m_color = wxColour(0xFF, 0x00, 0x00);
		else if (value == wxT("DARK RED"))
			m_color = wxColour(0x80, 0x00, 0x00);
		else if (value == wxT("MAGENTA"))
			m_color = wxColour(0xFF, 0x00, 0xFF);
		else if (value == wxT("DARK MAGENTA"))
			m_color = wxColour(0x80, 0x00, 0x80);
		else if (value == wxT("YELLOW"))
			m_color = wxColour(0xFF, 0xFF, 0x00);
		else if (value == wxT("DARK YELLOW"))
			m_color = wxColour(0x80, 0x80, 0x00);
		else if (value == wxT("LIGHT GREY"))
			m_color = wxColour(0xC0, 0xC0, 0xC0);
		else if (value == wxT("DARK GREY"))
			m_color = wxColour(0x80, 0x80, 0x80);
		else if (value == wxT("WHITE"))
			m_color = wxColour(0xFF, 0xFF, 0xFF);
		else if (value == wxT("BROWN"))
			m_color = wxColour(0xA5, 0x2A, 0x2A);
	}
}
