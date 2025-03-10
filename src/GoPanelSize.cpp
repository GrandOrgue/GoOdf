/*
 * GoPanelSize.cpp is part of GoOdf.
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

#include "GoPanelSize.h"

GoPanelSize::GoPanelSize() {
	m_panelSizeNames.Add(wxT("SMALL"));
	m_panelSizeNames.Add(wxT("MEDIUM"));
	m_panelSizeNames.Add(wxT("MEDIUM LARGE"));
	m_panelSizeNames.Add(wxT("LARGE"));
	m_panelSizeNames.Add(wxT("Custom Pixel size"));
	m_selectedSizeNameIndex = -1;
	m_numericalValue = 100;
}

GoPanelSize::~GoPanelSize() {

}

int GoPanelSize::getNumericalValue() const {
	return m_numericalValue;
}

void GoPanelSize::setNumericalValue(int value) {
	if (value > 99 && value < 32001) {
		m_numericalValue = value;
		m_selectedSizeNameIndex = 4;
	}
}

const wxArrayString& GoPanelSize::getPanelSizeNames() const {
	return m_panelSizeNames;
}

int GoPanelSize::getSelectedNameIndex() {
	return m_selectedSizeNameIndex;
}

void GoPanelSize::setSelectedNameIndex(int index, bool isHorizontal) {
	if (index > -1 && (unsigned) index < m_panelSizeNames.size()) {
		m_selectedSizeNameIndex = index;
		switch (index) {

			case 0:
				if (isHorizontal)
					m_numericalValue = 800;
				else
					m_numericalValue = 500;
				break;

			case 1:
				if (isHorizontal)
					m_numericalValue = 1007;
				else
					m_numericalValue = 663;
				break;

			case 2:
				if (isHorizontal)
					m_numericalValue = 1263;
				else
					m_numericalValue = 855;
				break;

			case 3:
				if (isHorizontal)
					m_numericalValue = 1583;
				else
					m_numericalValue = 1095;
				break;

			default:
				break;
		}
	}
}
