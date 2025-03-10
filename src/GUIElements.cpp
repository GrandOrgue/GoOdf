/*
 * GUIElements.cpp is part of GoOdf.
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

#include "GUIElements.h"

GUIElement::GUIElement() {
	m_owningPanel = NULL;
	m_type = wxEmptyString;
	m_positionX = -1;
	m_positionY = -1;
	m_displayName = wxEmptyString;
}

GUIElement::~GUIElement() {

}

void GUIElement::write(wxTextFile *outFile) {
	outFile->AddLine(wxT("Type=") + m_type);
}

void GUIElement::read(wxFileConfig *cfg) {
	m_type = cfg->Read("Type", wxEmptyString);
}

GUIElement* GUIElement::clone() {
	return nullptr;
}

void GUIElement::updateDisplayName() {

}

void GUIElement::setDefaultFont(wxFont&) {

}

wxBitmap GUIElement::getBitmap() {
	return wxNullBitmap;
}

wxString GUIElement::getElementName() {
	return wxEmptyString;
}

wxString GUIElement::getType() {
	return m_type;
}

void GUIElement::setType(wxString type) {
	m_type = type;
}

int GUIElement::getPosX() {
	return m_positionX;
}

void GUIElement::setPosX(int x) {
	m_positionX = x;
}

int GUIElement::getPosY() {
	return m_positionY;
}

void GUIElement::setPosY(int y) {
	m_positionY = y;
}

void GUIElement::setOwningPanel(GoPanel *panel) {
	m_owningPanel = panel;
}

GoPanel* GUIElement::getOwningPanel() {
	return m_owningPanel;
}

void GUIElement::setDisplayName(wxString name) {
	m_displayName = name;
}

wxString GUIElement::getDisplayName() {
	return m_displayName;
}
